#ifndef preproc_h
#define preproc_h

#include "strpool.h"
#include <stddef.h>

typedef enum preproc_token_type_t {
    PREPROC_TOKEN_EOS,
    PREPROC_TOKEN_NEWLINE,
    PREPROC_TOKEN_WHITESPACE,
    PREPROC_TOKEN_HEADER_NAME,
    PREPROC_TOKEN_IDENTIFIER,
    PREPROC_TOKEN_PP_NUMBER,
    PREPROC_TOKEN_CHARACTER_CONSTANT,
    PREPROC_TOKEN_STRING_LITERAL,
    PREPROC_TOKEN_OPERATOR,
    PREPROC_TOKEN_NON_WHITESPACE,
    PREPROC_TOKEN_WARNING,
    PREPROC_TOKEN_ERROR,
} preproc_token_type_t;


typedef struct preproc_token_t {
    preproc_token_type_t type; 
    STRPOOL_U32 lexeme; // will be 0 (empty string) for token types EOS, NEWLINE and WHITESPACE
    char const* pos; // pointer to start location (in source data) of this token
} preproc_token_t;


// returns a stream of tokens, the last token indicated by type PREPROC_TOKEN_EOS. 'source' is expected to be
// a zero-terminated string. 'strings' is assumed to be initialized already. the optional param 'errors_count'
// will be assigned the total number of errors encountered. information on individual errors can be found by scanning
// the returned token stream for tokens of type PREPROC_TOKEN_ERROR. the token stream returned should be released
// by calling free
// TODO: make this function internal, and have `preproc_process` be the main API function
preproc_token_t* preproc_tokenize( char const* source, strpool_t* strings, int* warnings_count, int* errors_count );


typedef char const* (*preproc_load_file_t)( void* load_context, STRPOOL_U32 header_name, strpool_t* strings );


preproc_token_t* preproc_process( preproc_token_t* tokens, strpool_t* strings, preproc_load_file_t load_func, 
    void* load_context, int* warnings_count, int* errors_count );


void preproc_free_tokens( preproc_token_t* tokens );


size_t preproc_generate_source_code( preproc_token_t* tokens, char* output, size_t capacity );


#endif /* preproc_h */

/*
----------------------
    IMPLEMENTATION
----------------------
*/

#ifdef PREPROC_IMPLEMENTATION
#undef PREPROC_IMPLEMENTATION

#include <stddef.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#ifdef PREPROC_NO_LONGJMP
    #define PREPROC_ALLOC_FAIL_EXIT(message,pos) do { fprintf( stderr, "%s\n", message ); exit( EXIT_FAILURE ); } while( 0 )
#else
    #include <setjmp.h>
    #define PREPROC_ALLOC_FAIL_EXIT(message,pos) do { ctx->alloc_error = (STRPOOL_U32) strpool_inject( ctx->strings, message, (int) strlen( message ) ); longjmp( ctx->exit_jump, 1 ); } while( 0 )
#endif

// Internal token types
#define PREPROC_TOKEN_MACRO_ARG -1

static preproc_token_t* preproc_token_next( preproc_token_t* token ) {
    return token->type == PREPROC_TOKEN_EOS ? token : token + 1;
}


// context used during tokenization process
typedef struct preproc_tokenizer_t {
    char const* source; // source data passed to preproc_tokenize
    char current_char; // last character that was read but have not yet been used
    char const* current_char_pos; // position in source data of current last character read
    int warnings_count; // total number of warnings encountered
    int errors_count; // total number of errors encountered
    strpool_t* strings; // the string pool passed to preproc_tokenize
    char* accumulation_buffer; // temp char buffer used for building strings character by character. resized as needed
    size_t accumulation_buffer_capacity; // current capacity of accumulation buffer

    #ifndef PREPROC_NO_LONGJMP
        jmp_buf exit_jump;
        STRPOOL_U32 alloc_error;
        char const* alloc_error_pos;
    #endif
} preproc_tokenizer_t;


// returns the next char, after merging lines ending in the '\' character
static char preproc_get_char( preproc_tokenizer_t* ctx ) {
    // TODO: handle trigraphs (before line continuation '\' is handled)
    ctx->current_char_pos = ctx->source;
    char c = *ctx->source;
    if( c ) {
        ++ctx->source;
    }

    while( c == '\\' && *ctx->source == '\n' ) {
        ++ctx->source;
        c = *ctx->source;
        if( c ) {
            ++ctx->source;
        }
    }
    return c;
}


// returns the next char, but does not step forward - just a look-ahead
static char preproc_peek_char( preproc_tokenizer_t* ctx ) {
    char const* source = ctx->source;
    char const* current_char_pos = ctx->current_char_pos;
    char c = preproc_get_char( ctx );
    ctx->source = source;
    ctx->current_char_pos = current_char_pos;
    return c;
}


// helper function for building strings. will resize the accumulation buffer as needed
static size_t preproc_accumulate( preproc_tokenizer_t* ctx, size_t pos, char c ) {
    if( pos >= ctx->accumulation_buffer_capacity ) {
        ctx->accumulation_buffer_capacity *= 2;
        void* new_buffer = realloc( ctx->accumulation_buffer, sizeof( char) * ctx->accumulation_buffer_capacity );
        if( !new_buffer ) {
            PREPROC_ALLOC_FAIL_EXIT( "Failed to reallocate accumulation buffer", ctx->current_char_pos );
        }
        ctx->accumulation_buffer = (char*) new_buffer;
    }
    ctx->accumulation_buffer[ pos ] = c;
    return pos + 1;
}


// helper function to add a string to the string pool (really just to not have to do the necessary casts everywhere)
static STRPOOL_U32 preproc_add_string( strpool_t* strpool, char const* str, size_t len ) {
    if( len == 0 ) {
        len = strlen( str );
    }
    return (STRPOOL_U32) strpool_inject( strpool, str, (int)len );
}


// helper function to add a warning message to the string pool, and also increase the warning count
static STRPOOL_U32 preproc_tokenizer_warning( preproc_tokenizer_t* ctx, char const* message ) {
    ++ctx->warnings_count;
    return preproc_add_string( ctx->strings, message, strlen( message ) );
}


// helper function to add an error message to the string pool, and also increase the error count
static STRPOOL_U32 preproc_tokenizer_error( preproc_tokenizer_t* ctx, char const* message ) {
    ++ctx->errors_count;
    return preproc_add_string( ctx->strings, message, strlen( message ) );
}


// list of all operators and punctuators, ordered by length
static char const* preproc_operators[] = {
    //"sizeof", // handled as a special case when tokenizing identifiers
    "...","<<=",">>=",
    "<<",">>","<=",">=","==","!=","*=","/=","%=","+=","-=","&=","^=","|=","->","++","--","&&","||","##",
    "[","]","(",")",".","&","*","+","-","~","!","/","%","<",">","^","|","?",":","=",",","#","{","}",";",
};


// by using only two characters, determine if it matches the first two chars of any of the operators/punctuators
// returns NULL if there is no match. if a match is found, we can read one more char and call the other helper
// function 'is_operator_three_char' to check if it match one of the three-char operators/punctuators
static char const* is_operator_two_char( char a, char b ) {
    int const count = sizeof( preproc_operators ) / sizeof( *preproc_operators );
    for( int i = 0; i < count; ++i ) {
        if( preproc_operators[ i ][ 0 ] == a ) {
            if( preproc_operators[ i ][ 1 ] == '\0' ) {
                return preproc_operators[ i ];
            } else if( preproc_operators[ i ][ 1 ] == b ) {
                return preproc_operators[ i ];
            }           
        }
    }
    return NULL;
}


// by using three characters, determine the exact match to one of the operators/punctuators. returns NULL if no match
static char const* is_operator_three_char( char a, char b, char c ) {
    int const count = sizeof( preproc_operators ) / sizeof( *preproc_operators );
    for( int i = 0; i < count; ++i ) {

        if( preproc_operators[ i ][ 0 ] == a ) {
            if( preproc_operators[ i ][ 1 ] == '\0' ) {
                return preproc_operators[ i ];
            } else if( preproc_operators[ i ][ 1 ] == b ) {
                if( preproc_operators[ i ][ 2 ] == '\0' ) {
                    return preproc_operators[ i ];
                } else if( preproc_operators[ i ][ 2 ] == c ) {
                    return preproc_operators[ i ];
                }
            }           
        }
    }
    return NULL;
}


// starts with the current char and reads more chars as needed until a full token has been scanned, then returns it
// the 'in_include_directive' param should be non-zero if the preceding tokens for the '#include' directive
static preproc_token_t preproc_next_token( preproc_tokenizer_t* ctx, int in_include_directive ) {
    // token to return - initial values will be overridden as needed
    preproc_token_t token;
    token.type = PREPROC_TOKEN_ERROR;
    token.lexeme = 0;
    token.pos = ctx->current_char_pos;

    // End of stream
    if( ctx->current_char == '\0' )  {
        token.type = PREPROC_TOKEN_EOS;
        return token;
    }
    
    // Whitespace
    if( isspace( (unsigned char) ctx->current_char ) && ctx->current_char != '\n' ) {
        while( isspace( (unsigned char) ctx->current_char ) && ctx->current_char != '\n' ) {
            ctx->current_char = preproc_get_char( ctx );
        }
        token.type = PREPROC_TOKEN_WHITESPACE;    
        return token;
    }

    // Line comment
    if( ctx->current_char == '/' && preproc_peek_char( ctx ) == '/' ) {
        while( ctx->current_char && ctx->current_char != '\n' )  {
            ctx->current_char = preproc_get_char( ctx );
        }
        token.type = PREPROC_TOKEN_WHITESPACE;    
        return token;
    }

    // Block comment
    if( ctx->current_char == '/' && preproc_peek_char( ctx ) == '*' ) {
        ctx->current_char = preproc_get_char( ctx );
        ctx->current_char = preproc_get_char( ctx );
        while( !( ctx->current_char == '*' && preproc_peek_char( ctx ) == '/' ) )  {
            if( ctx->current_char == '\0' ) {
                token.type = PREPROC_TOKEN_ERROR;
                token.lexeme = preproc_tokenizer_error( ctx, "unexpected end of file found in comment" );
                return token;
            }
            ctx->current_char = preproc_get_char( ctx );
        }
        ctx->current_char = preproc_get_char( ctx );
        ctx->current_char = preproc_get_char( ctx );
        token.type = PREPROC_TOKEN_WHITESPACE;    
        return token;
    }

    // Newline
    if( ctx->current_char == '\n' ) {
        ctx->current_char = preproc_get_char( ctx );
        token.type = PREPROC_TOKEN_NEWLINE;    
        return token;
    }

    // Identifier
    if( isalpha( (unsigned char) ctx->current_char ) || ctx->current_char == '_' ) { 
        size_t acc_pos = 0;
        while( isalnum( (unsigned char) ctx->current_char ) || ctx->current_char == '_' ) {
            acc_pos = preproc_accumulate( ctx, acc_pos, ctx->current_char );
            ctx->current_char = preproc_get_char( ctx );
        }
        token.type = PREPROC_TOKEN_IDENTIFIER;
        if( acc_pos == strlen( "sizeof") && strncmp( ctx->accumulation_buffer, "sizeof", acc_pos ) == 0 ) {
            token.type = PREPROC_TOKEN_OPERATOR;
        }
        token.lexeme = preproc_add_string( ctx->strings, ctx->accumulation_buffer, acc_pos );
        return token;
    }

    // Header name
    if( in_include_directive && ( ctx->current_char == '<' || ctx->current_char == '"' ) ) {
        char terminator = ctx->current_char == '<' ? '>' : '"';
        size_t acc_pos = 0;
        acc_pos = preproc_accumulate( ctx, acc_pos, ctx->current_char );
        ctx->current_char = preproc_get_char( ctx );
        while( ctx->current_char != terminator ) {
            if( !ctx->current_char || ctx->current_char == '\n') {
                token.type = PREPROC_TOKEN_ERROR;
                token.lexeme = preproc_tokenizer_error( ctx, 
                    terminator == '>' ? "missing '>'" : "newline in constant" );
                return token;
            }
            acc_pos = preproc_accumulate( ctx, acc_pos, ctx->current_char );
            ctx->current_char = preproc_get_char( ctx );
        }
        acc_pos = preproc_accumulate( ctx, acc_pos, ctx->current_char );
        ctx->current_char = preproc_get_char( ctx );
        token.lexeme = preproc_add_string( ctx->strings, ctx->accumulation_buffer, acc_pos );
        token.type = PREPROC_TOKEN_HEADER_NAME;
        return token;
    }

    // PP Number
    if( isdigit( (unsigned char) ctx->current_char ) || ( ctx->current_char == '.' 
        && isdigit( (unsigned char) preproc_peek_char( ctx ) ) ) ) {   

        size_t acc_pos = 0;
        while( isdigit( (unsigned char) ctx->current_char ) ) {
            acc_pos = preproc_accumulate( ctx, acc_pos, ctx->current_char );
            ctx->current_char = preproc_get_char( ctx );
        }
        if( ctx->current_char == '.' ) {
            acc_pos = preproc_accumulate( ctx, acc_pos, ctx->current_char );
            ctx->current_char = preproc_get_char( ctx );
        }
        while( isdigit( (unsigned char) ctx->current_char ) ) {
            acc_pos = preproc_accumulate( ctx, acc_pos, ctx->current_char );
            ctx->current_char = preproc_get_char( ctx );
        }
        if( ctx->current_char == 'e' || ctx->current_char == 'E' ) {
            acc_pos = preproc_accumulate( ctx, acc_pos, ctx->current_char );
            ctx->current_char = preproc_get_char( ctx );
            if( ctx->current_char == '+' || ctx->current_char == '-' ) {
                acc_pos = preproc_accumulate( ctx, acc_pos, ctx->current_char );
                ctx->current_char = preproc_get_char( ctx );
            }
            while( isdigit( (unsigned char) ctx->current_char ) ) {
                acc_pos = preproc_accumulate( ctx, acc_pos, ctx->current_char );
                ctx->current_char = preproc_get_char( ctx );
            }
        }
        while( isalpha( (unsigned char) ctx->current_char ) ) {
            acc_pos = preproc_accumulate( ctx, acc_pos, ctx->current_char );
            ctx->current_char = preproc_get_char( ctx );
        }
        token.lexeme = preproc_add_string( ctx->strings, ctx->accumulation_buffer, acc_pos );
        token.type = PREPROC_TOKEN_PP_NUMBER;
        return token;
    } 

    // Operators and punctuators
    if( is_operator_two_char( ctx->current_char, preproc_peek_char( ctx ) ) ) {
        char prev_char = ctx->current_char;
        ctx->current_char = preproc_get_char( ctx );
        char const* str = is_operator_three_char( prev_char, ctx->current_char, 
            preproc_peek_char( ctx ) );
        size_t len = strlen( str );
        if( len == 2 ) {
            ctx->current_char = preproc_get_char( ctx );
        } else if( len == 3 ) {
            ctx->current_char = preproc_get_char( ctx );
            ctx->current_char = preproc_get_char( ctx );
        } 
        token.lexeme = preproc_add_string( ctx->strings, str, len );
        token.type = PREPROC_TOKEN_OPERATOR;
        return token;
    }

    // String literals
    // TODO: escape sequences
    if( ctx->current_char == '"' ) {   
        ctx->current_char = preproc_get_char( ctx );
        size_t acc_pos = 0;
        ctx->accumulation_buffer[ 0 ] = '\0'; // TODO: Make sure to handle zero-len acc buffer everywher it is used
        while( ctx->current_char != '"' ) {
            if( ctx->current_char == '\n' ) {
                token.type = PREPROC_TOKEN_ERROR;
                token.lexeme = preproc_tokenizer_error( ctx, "newline in constant" );
                return token;
            }
            acc_pos = preproc_accumulate( ctx, acc_pos, ctx->current_char );
            if( ctx->current_char == '\\' && preproc_peek_char( ctx ) == '"' ) {
                ctx->current_char = preproc_get_char( ctx );
                acc_pos = preproc_accumulate( ctx, acc_pos, ctx->current_char );
            }
            ctx->current_char = preproc_get_char( ctx );
        }
        ctx->current_char = preproc_get_char( ctx );

        token.lexeme = preproc_add_string( ctx->strings, ctx->accumulation_buffer, acc_pos );
        token.type = PREPROC_TOKEN_STRING_LITERAL ;
        return token;
    }

    // Char constants
    // TODO: escape sequences
    if( ctx->current_char == '\'' ) {   
        ctx->current_char = preproc_get_char( ctx );
        size_t acc_pos = 0;
        while( ctx->current_char != '\'' ) {
            if( ctx->current_char == '\n' ) {
                token.type = PREPROC_TOKEN_ERROR;
                token.lexeme = preproc_tokenizer_error( ctx, "newline in constant" );
                return token;
            }
            acc_pos = preproc_accumulate( ctx, acc_pos, ctx->current_char );
            if( ctx->current_char == '\\' && ( preproc_peek_char( ctx ) == '\'' 
                || preproc_peek_char( ctx ) == '\\' ) ) {

                ctx->current_char = preproc_get_char( ctx );
                acc_pos = preproc_accumulate( ctx, acc_pos, ctx->current_char );
            }
            ctx->current_char = preproc_get_char( ctx );
        }
        ctx->current_char = preproc_get_char( ctx );

        token.lexeme = preproc_add_string( ctx->strings, ctx->accumulation_buffer, acc_pos );
        token.type = PREPROC_TOKEN_CHARACTER_CONSTANT;
        return token;
    }

    // Catchall for any characters not caught by the above - will probably be rejected in later stages
    token.type = PREPROC_TOKEN_NON_WHITESPACE;
    token.lexeme = preproc_add_string( ctx->strings, &ctx->current_char, 1 );   
    ctx->current_char = preproc_get_char( ctx );
    return token;
}


// to make implementation easier, for some token types (whitespace, newline and non-whitespace) consecutive tokens are 
// collapsed to a single token of that type, and lexemes merged. thuis is a helper function to do that
static size_t preproc_merge_tokens( preproc_tokenizer_t* ctx, preproc_token_t* token, preproc_token_t const* tokens, 
    size_t count ) {
    if( count > 0 ) { // we can't merge if no preceding token exitt
        preproc_token_t const* prev_token = &tokens[ count - 1 ];
        if( token->type == prev_token->type ) { // only merge if both tokens are the same tyoe
            // copy lexeme of first token to accumulation buffer
            size_t pos = 0;
            if( prev_token->lexeme ) {
                char const* str = strpool_cstr( ctx->strings, prev_token->lexeme );
                int len = strpool_length( ctx->strings, prev_token->lexeme );
                for( int i = 0; i < len; ++i ) {
                    pos = preproc_accumulate( ctx, pos, str[ i ] );
                }
            }
            
            // append lexeme of second token to accumulation buffer
            if( token->lexeme ) {
                char const* str = strpool_cstr( ctx->strings, token->lexeme );
                int len = strpool_length( ctx->strings, token->lexeme );
                for( int i = 0; i < len; ++i ) {
                    pos = preproc_accumulate( ctx, pos, str[ i ] );
                }
            }

            if( pos > 0 ) {
                token->lexeme = preproc_add_string( ctx->strings, ctx->accumulation_buffer, pos );
            }

            --count; // so when this token gets added after we loop around, it overwrites the one we merged with
        }
    }
    return count;
}


preproc_token_t* preproc_tokenize( char const* source, strpool_t* strings, int* warnings_count, int* errors_count ) {

    // initialize the ctx context
    preproc_tokenizer_t context;
    preproc_tokenizer_t* ctx = &context;
    memset( ctx, 0, sizeof( *ctx ) );

    ctx->source = source;
    ctx->current_char = '\0';
    ctx->current_char_pos = source;
    ctx->warnings_count = 0;
    ctx->errors_count = 0;
    ctx->strings = strings;
    ctx->accumulation_buffer_capacity = 256;
    
    preproc_token_t* tokens = NULL;
    if( setjmp( ctx->exit_jump ) == 0 ) {
        
        // allocate memory for token stream
        size_t count = 0;
        size_t capacity = 4096;
        tokens = (preproc_token_t*) malloc( capacity * sizeof( preproc_token_t ) );
        if( !tokens ) {
            PREPROC_ALLOC_FAIL_EXIT( "Failed to allocate initial token output stream", source );
        }

        ctx->accumulation_buffer = (char*) malloc( sizeof( char ) * ctx->accumulation_buffer_capacity );
        if( !ctx->accumulation_buffer ) {
            PREPROC_ALLOC_FAIL_EXIT( "Failed to allocate initial accumulation buffer", source );
        }

            // add to string pool the strings we need to compare against during tokenization
        STRPOOL_U32 identifier_include = preproc_add_string( ctx->strings, "include", 0 );  
        STRPOOL_U32 identifier_hash = preproc_add_string( ctx->strings, "#", 0 );

        // initialize the current_char field with the first character from source, as preproc_next_token expects this
        ctx->current_char = preproc_get_char( ctx );

        // get the first token before we enter the while loop
        preproc_token_t token = preproc_next_token( ctx, 0 ); // we know we are not in an #include
    
        // tokenize until end of file
        while( token.type != PREPROC_TOKEN_EOS ) {
            // reallocate tokens array if needed
            if( count >= capacity - 3 ) { // -3 to ensure room for EOS token and possible newline (+warning) at the end
                capacity *= 2;
                void* new_tokens = realloc( tokens, capacity * sizeof( preproc_token_t ) );            
                if( !new_tokens ) {
                    PREPROC_ALLOC_FAIL_EXIT( "Failed to reallocate token output stream", source );
                }
                tokens = (preproc_token_t*) new_tokens;            
            }

            // add token to stream
            tokens[ count++ ] = token; 

            // look a few tokens back in the stream to see if we are in an #include directive (to parse header names)
            int in_include_directive = 0;
            size_t i = count;
            if( i > 0 &&tokens[ i - 1 ].type == PREPROC_TOKEN_WHITESPACE ) {
                --i; // skip whitespace after 'include', if any
            }
            // check if token is the 'include' identifier
            if( i > 0 && tokens[ i - 1 ].type == PREPROC_TOKEN_IDENTIFIER 
                && tokens[ i - 1 ].lexeme == identifier_include ) {

                --i; // skip past 'include'
                if( i > 0 && tokens[ i - 1 ].type == PREPROC_TOKEN_WHITESPACE ) {
                    --i; // skip whitespace between '#' and 'include', if any
                }
                // check if token is '#' symbol
                if( i > 0 && tokens[ i - 1 ].type == PREPROC_TOKEN_OPERATOR 
                    && tokens[ i - 1 ].lexeme == identifier_hash ) {
                
                    in_include_directive = 1; // all conditions are met, we are just after an #include directive
                }
            }

            // read next token
            token = preproc_next_token( ctx, in_include_directive );

            // merge tokens of type 'whitespace', 'non-whitespace' and 'newline'
            if( token.type == PREPROC_TOKEN_NON_WHITESPACE || token.type == PREPROC_TOKEN_WHITESPACE 
                || token.type == PREPROC_TOKEN_NEWLINE ) {
                count = preproc_merge_tokens( ctx, &token, tokens, count );
            }
        }

        // ensure newline at end of file
        if( count < 1 || tokens[ count - 1 ].type != PREPROC_TOKEN_NEWLINE ) {
            preproc_token_t warning;
            warning.type = PREPROC_TOKEN_WARNING;
            warning.lexeme = preproc_tokenizer_warning( ctx, "no newline at end of file" );
            warning.pos = token.pos;
            tokens[ count++ ] = warning;

            preproc_token_t newline;
            newline.type = PREPROC_TOKEN_NEWLINE;
            newline.lexeme = 0;
            newline.pos = token.pos;
            tokens[ count++ ] = newline;
        }
    
        tokens[ count ] = token; // add end-of-stream token
   
        // return warning count
        if( warnings_count ) {
            *warnings_count = ctx->warnings_count;
        }

        // return error count
        if( errors_count ) {
            *errors_count = ctx->errors_count;
        }

    } else {
        static preproc_token_t alloc_fail_output[ 2 ]; // TODO: use thread local if available
        alloc_fail_output[ 0 ].type = PREPROC_TOKEN_ERROR;
        alloc_fail_output[ 0 ].lexeme = ctx->alloc_error;
        alloc_fail_output[ 0 ].pos = ctx->alloc_error_pos;
        alloc_fail_output[ 1 ].type = PREPROC_TOKEN_EOS;
        alloc_fail_output[ 1 ].lexeme = 0;
        alloc_fail_output[ 1 ].pos = ctx->alloc_error_pos;
        if( tokens ) {
            free( tokens );
        }
        tokens = alloc_fail_output;
    }

    if( ctx->accumulation_buffer ) { 
        free( ctx->accumulation_buffer );
    }

    // return token stream
    return tokens;
}


static preproc_token_t* preproc_skip_whitespace( preproc_token_t* token ) {
    while( token->type == PREPROC_TOKEN_WHITESPACE ) {
        token = preproc_token_next( token );
    }
    return token;
}


static preproc_token_t* preproc_skip_to_next_line( preproc_token_t* token ) {
    while( token->type != PREPROC_TOKEN_EOS ) {
        if( token->type == PREPROC_TOKEN_NEWLINE ) {
            token = preproc_token_next( token );
            break;
        }
        token = preproc_token_next( token );
    }
    return token;
}


typedef struct preproc_macro_def_t {
    STRPOOL_U32 name;
    char const* pos;
    int arg_count;
    preproc_token_t* argument_list; // NULL for obj-like macros
    preproc_token_t* replacement_list;
    int is_being_expanded;
} preproc_macro_def_t;


typedef struct preproc_expansion_context_t {
    preproc_token_t* tokens;
} preproc_expansion_context_t;


typedef struct preproc_expanded_arg_t {
    preproc_token_t* token;
} preproc_expanded_arg_t;


typedef struct preproc_ctx_t {
    strpool_t* strings;
    preproc_load_file_t load_func;
    void* load_context;
    int warnings_count;
    int errors_count;

    STRPOOL_U32 identifier_hash;
    STRPOOL_U32 identifier_lparen;
    STRPOOL_U32 identifier_rparen;
    STRPOOL_U32 identifier_comma;
    STRPOOL_U32 identifier_if;
    STRPOOL_U32 identifier_ifdef;
    STRPOOL_U32 identifier_ifndef;
    STRPOOL_U32 identifier_elif;
    STRPOOL_U32 identifier_else;
    STRPOOL_U32 identifier_endif;
    STRPOOL_U32 identifier_include;
    STRPOOL_U32 identifier_define;
    STRPOOL_U32 identifier_undef;
    STRPOOL_U32 identifier_line;
    STRPOOL_U32 identifier_error;
    STRPOOL_U32 identifier_pragma;
    STRPOOL_U32 identifier_defined;

    size_t macro_defs_count;
    size_t macro_defs_capacity;
    preproc_macro_def_t* macro_defs;

    size_t macro_tokens_count;
    size_t macro_tokens_capacity;
    preproc_token_t* macro_tokens;

    size_t args_tokens_count;
    size_t args_tokens_capacity;
    preproc_token_t* args_tokens;

    size_t output_count;
    size_t output_capacity;
    preproc_token_t* output;

    #ifndef PREPROC_NO_LONGJMP
        jmp_buf exit_jump;
        STRPOOL_U32 alloc_error;
        char const* alloc_error_pos;
    #endif
} preproc_ctx_t;


// TODO: handle reallocation when holding pointers to element in the array. Should not use preproc_token_t directly
// instead should make it a [type,index] pair, where type indicates which array it is from and index is which element.
// To access a field, use accessor method `preproc_token_type( ctx, token )`, `preproc_token_lexeme( ctx, token )`

static preproc_token_t* preproc_process_stream( preproc_ctx_t* ctx, preproc_token_t* tokens );


static void preproc_output( preproc_ctx_t* ctx, preproc_token_t* token ) {
    // reallocate tokens array if needed
    if( ctx->output_count >= ctx->output_capacity - 1 ) { // -1 to ensure room for EOS token at the end
        ctx->output_capacity *= 2;
        void* new_tokens = realloc( ctx->output, ctx->output_capacity * sizeof( preproc_token_t ) );            
        if( !new_tokens ) {
            PREPROC_ALLOC_FAIL_EXIT( "Failed to reallocate token output buffer", token->pos );
        }
        ctx->output = (preproc_token_t*) new_tokens;            
    }

    // add token to output
    ctx->output[ ctx->output_count++ ] = *token;
}


static STRPOOL_U32 preproc_format( preproc_ctx_t* ctx, char const* message, va_list args ) {
    char fixed_buf[ 256 ];
	#ifdef _WIN32
		size_t size = _vscprintf( message, args );
	#else
	    size_t size = vsnprintf( fixed_buf, 0, message, args );
	#endif
    char* buf = fixed_buf;
    if( size + 1 >= sizeof( fixed_buf ) ) {
	    buf = (char*) malloc( size + 1 );
        if( !buf ) {
            PREPROC_ALLOC_FAIL_EXIT( "Failed to allocate temp buffer in preproc_format", NULL );
        }
    }
	#ifdef _WIN32
		_vsnprintf( buf, size + 1, message, args );
	#else
		vsnprintf( buf, size + 1, message, args );
	#endif
    STRPOOL_U32 formatted_string = preproc_add_string( ctx->strings, buf, strlen( buf ) );
    if( buf != fixed_buf ) {
        free( buf );
    }
    return formatted_string;
}


static void preproc_warning( preproc_ctx_t* ctx, char const* pos, char const* message, ... ) {
	va_list args;
	va_start( args, message );
    STRPOOL_U32 formatted_string = preproc_format( ctx, message, args );
    va_end( args );

    preproc_token_t token;
    token.type = PREPROC_TOKEN_WARNING;
    token.pos = pos;
    token.lexeme = formatted_string;
    ++ctx->warnings_count;
    preproc_output( ctx, &token );
}


static void preproc_error( preproc_ctx_t* ctx, char const* pos, char const* message, ... ) {
	va_list args;
	va_start( args, message );
    STRPOOL_U32 formatted_string = preproc_format( ctx, message, args );
    va_end( args );

    preproc_token_t token;
    token.type = PREPROC_TOKEN_ERROR;
    token.pos = pos;
    token.lexeme = formatted_string;
    ++ctx->errors_count;
    preproc_output( ctx, &token );
}


static void preproc_add_macro_def( preproc_ctx_t* ctx, STRPOOL_U32 name, char const* pos, int arg_count,
    preproc_token_t* argument_list, preproc_token_t* replacement_list ) {
    
    // reallocate macro array if needed
    if( ctx->macro_defs_count >= ctx->macro_defs_capacity ) {
        ctx->macro_defs_capacity *= 2;
        void* new_macros = realloc( ctx->macro_defs, ctx->macro_defs_capacity * sizeof( preproc_macro_def_t ) );            
        if( !new_macros ) {
            PREPROC_ALLOC_FAIL_EXIT( "Failed to reallocate macro def array", pos );
        }
        ctx->macro_defs = (preproc_macro_def_t*) new_macros;            
    }

    // add token 
    preproc_macro_def_t macro;
    macro.name = name;
    macro.pos = pos;
    macro.arg_count = arg_count;
    macro.argument_list = argument_list;
    macro.replacement_list = replacement_list;
    macro.is_being_expanded = 0;
    ctx->macro_defs[ ctx->macro_defs_count++ ] = macro;
}


static void preproc_remove_macro_def( preproc_ctx_t* ctx, STRPOOL_U32 name ) {
    for( size_t i = 0; i < ctx->macro_defs_count; ++i ) {
        if( ctx->macro_defs[ i ].name == name ) {
            ctx->macro_defs[ i ] = ctx->macro_defs[ --ctx->macro_defs_count ];
            return;
        }
    }
}


static preproc_macro_def_t* preproc_get_macro_def( preproc_ctx_t* ctx, STRPOOL_U32 name ) {
    // TODO: use a hash table for faster lookup of macro def from string id
    for( size_t i = 0; i < ctx->macro_defs_count; ++i ) {
        preproc_macro_def_t* macro_def = &ctx->macro_defs[ i ];
        if( macro_def->name == name ) {
            return macro_def;
        }
    }
    return NULL;
}


static void preproc_add_macro_token( preproc_ctx_t* ctx, preproc_token_t* token ) {
    // reallocate macro tokens array if needed
    if( ctx->macro_tokens_count >= ctx->macro_tokens_capacity ) {
        ctx->macro_tokens_capacity *= 2;
        void* new_tokens = realloc( ctx->macro_tokens, ctx->macro_tokens_capacity * sizeof( preproc_token_t ) );            
        if( !new_tokens ) {
            PREPROC_ALLOC_FAIL_EXIT( "Failed to reallocate macro tokens array", pos );
        }
        ctx->macro_tokens = (preproc_token_t*) new_tokens;            
    }

    // add token 
    ctx->macro_tokens[ ctx->macro_tokens_count++ ] = *token;
}


static void preproc_add_macro_token_eos( preproc_ctx_t* ctx, char const* pos ) {
    preproc_token_t eos_token;
    eos_token.type = PREPROC_TOKEN_EOS;
    eos_token.lexeme = 0;
    eos_token.pos = pos;
    preproc_add_macro_token( ctx, &eos_token );
}


static preproc_token_t* preproc_get_macro_tokens( preproc_ctx_t* ctx ) {
    return &ctx->macro_tokens[ ctx->macro_tokens_count ];
}


static void preproc_restore_macro_tokens( preproc_ctx_t* ctx, int restore_point ) {
    ctx->macro_tokens_count -= ( ctx->macro_tokens_count - restore_point );
}


static int preproc_get_macro_tokens_restore_point( preproc_ctx_t* ctx ) {
    return ctx->macro_tokens_count;
}



static void preproc_add_args_token( preproc_ctx_t* ctx, preproc_token_t* token ) {
    // reallocate args tokens array if needed
    if( ctx->args_tokens_count >= ctx->args_tokens_capacity ) {
        ctx->args_tokens_capacity *= 2;
        void* new_tokens = realloc( ctx->args_tokens, ctx->args_tokens_capacity * sizeof( preproc_token_t ) );            
        if( !new_tokens ) {
            PREPROC_ALLOC_FAIL_EXIT( "Failed to reallocate args tokens array", pos );
        }
        ctx->args_tokens = (preproc_token_t*) new_tokens;            
    }

    // add token 
    ctx->args_tokens[ ctx->args_tokens_count++ ] = *token;
}


static void preproc_add_args_token_eos( preproc_ctx_t* ctx, char const* pos ) {
    preproc_token_t eos_token;
    eos_token.type = PREPROC_TOKEN_EOS;
    eos_token.lexeme = 0;
    eos_token.pos = pos;
    preproc_add_args_token( ctx, &eos_token );
}


static preproc_token_t* preproc_get_args_tokens( preproc_ctx_t* ctx ) {
    return &ctx->args_tokens[ ctx->args_tokens_count ];
}


static void preproc_restore_args_tokens( preproc_ctx_t* ctx, int restore_point ) {
    ctx->args_tokens_count -= ( ctx->args_tokens_count - restore_point );
}


static int preproc_get_args_tokens_restore_point( preproc_ctx_t* ctx ) {
    return ctx->args_tokens_count;
}


static preproc_token_t* preproc_do_include( preproc_ctx_t* ctx, preproc_token_t* token ) {
    token = preproc_skip_whitespace( token );
    if( token->type == PREPROC_TOKEN_HEADER_NAME ) {
        char const* source = ctx->load_func( ctx->load_context, token->lexeme, 
            ctx->strings );
        if( source ) {
            preproc_token_t* source_tokens = preproc_tokenize( source, ctx->strings, NULL, NULL ); 
            preproc_process_stream( ctx, source_tokens );
            preproc_free_tokens( source_tokens );
        } else {
            preproc_error( ctx, token->pos, "Cannot open include file: '%s': No such file or directory", strpool_cstr( ctx->strings, token->lexeme ) );
       }
        token = preproc_token_next( token ); // skip header name
        token = preproc_skip_whitespace( token );

        if( token->type != PREPROC_TOKEN_NEWLINE ) {
            preproc_warning( ctx, token->pos, "unexpected tokens following preprocessor directive - expected a newline" );
        }
        return preproc_skip_to_next_line( token );
    } else {
        // TODO: expand and include if it evaluates to string
    }

    return token;
}


static preproc_token_t* preproc_do_define( preproc_ctx_t* ctx, preproc_token_t* token ) {
    token = preproc_skip_whitespace( token );
    
    if( token->type != PREPROC_TOKEN_IDENTIFIER ) {
        preproc_error( ctx, token->pos, "#define syntax" );
        return preproc_skip_to_next_line( token );
    }

    // macro name
    STRPOOL_U32 name = token->lexeme;
    char const* pos = token->pos;
    token = preproc_token_next( token ); // skip macro name

    // function-like macro
    int arg_count = 0;
    preproc_token_t* argument_list = NULL;
    if( token->type == PREPROC_TOKEN_OPERATOR && token->lexeme == ctx->identifier_lparen ) {
        token = preproc_token_next( token ); // skip lparen
        token = preproc_skip_whitespace( token );

        argument_list = preproc_get_macro_tokens( ctx );
        while( !( token->type == PREPROC_TOKEN_OPERATOR && token->lexeme == ctx->identifier_rparen ) ) {
            if( token->type != PREPROC_TOKEN_IDENTIFIER ) {               
                if( token->type == PREPROC_TOKEN_NEWLINE ) {
                    preproc_error( ctx, token->pos, "'%s': unexpected in macro parameter list", "\\n" );
                } else {
                    preproc_error( ctx, token->pos, "'%s': unexpected in macro parameter list", strpool_cstr( ctx->strings, token->lexeme ) );
                }
                return preproc_skip_to_next_line( token );
            }
            for( int i = 0; i < arg_count; ++i ) {
                if( argument_list[ i ].lexeme == token->lexeme ) {
                    preproc_error( ctx, token->pos, "cannot reuse macro parameter name '%s'", strpool_cstr( ctx->strings, token->lexeme ) );
                    return preproc_skip_to_next_line( token );                
                }
            }
            preproc_add_macro_token( ctx, token );
            ++arg_count;
            token = preproc_token_next( token ); // skip identifier
            token = preproc_skip_whitespace( token );
                
            if( token->type == PREPROC_TOKEN_OPERATOR && token->lexeme == ctx->identifier_comma ) {
                token = preproc_token_next( token );
                token = preproc_skip_whitespace( token );
                if( token->type == PREPROC_TOKEN_OPERATOR && token->lexeme == ctx->identifier_rparen ) {
                    preproc_error( ctx, token->pos, "')': unexpected in macro parameter list" );
                    return preproc_skip_to_next_line( token );
                }
            }
        }
        token = preproc_token_next( token ); // skip rparen

        preproc_add_macro_token_eos( ctx, token->pos );
    }

    token = preproc_skip_whitespace( token );

    // parse replacement list
    preproc_token_t* replacement_list = preproc_get_macro_tokens( ctx );
    while( token->type != PREPROC_TOKEN_NEWLINE ) {
        preproc_token_t token_to_add = *token;
        if( token->type == PREPROC_TOKEN_IDENTIFIER && argument_list ) {
            for( int i = 0; i < arg_count; ++i ) {
                if( argument_list[ i ].lexeme == token->lexeme ) {
                    token_to_add.type = PREPROC_TOKEN_MACRO_ARG;
                    token_to_add.lexeme = i;
                    break;
                }
            }           
        }
        preproc_add_macro_token( ctx, &token_to_add );
        token = preproc_token_next( token ); 
    }
    preproc_add_macro_token_eos( ctx, token->pos );

    // check if macro exists
    preproc_macro_def_t* macro_def = preproc_get_macro_def( ctx, name );
    if( macro_def ) {
        // check if definitions match
        if( ( macro_def->argument_list && !argument_list ) || ( !macro_def->argument_list && argument_list ) ) {
            preproc_warning( ctx, pos, "'%s': macro redefinition", strpool_cstr( ctx->strings, name ) );
            preproc_warning( ctx, macro_def->pos, "see previous definition of '%s'", strpool_cstr( ctx->strings, name ) );
        } else if( argument_list ) {
            int def_arg_count = 0;
            for( preproc_token_t* arg = macro_def->argument_list; arg->type != PREPROC_TOKEN_EOS; ++arg ) {
                ++def_arg_count;
            }
            int new_arg_count = 0;
            for( preproc_token_t* arg = argument_list; arg->type != PREPROC_TOKEN_EOS; ++arg ) {
                ++new_arg_count;
            }
            if( def_arg_count != new_arg_count ) {
                // number of arguments differ, emit warning
                preproc_warning( ctx, pos, "'%s': macro redefinition", strpool_cstr( ctx->strings, name ) );
                preproc_warning( ctx, macro_def->pos, "see previous definition of '%s'", strpool_cstr( ctx->strings, name ) );
            } else {
                for( preproc_token_t* repl = macro_def->replacement_list, *new_repl = replacement_list; 
                    !( repl->type == PREPROC_TOKEN_EOS && repl->type == PREPROC_TOKEN_EOS ); 
                    ++repl, ++new_repl ) {
                    if( repl->type != new_repl->type || repl->lexeme != new_repl->lexeme ) {
                        // if the definitions don't match, emit a warning
                        preproc_warning( ctx, pos, "'%s': macro redefinition", strpool_cstr( ctx->strings, name ) );
                        preproc_warning( ctx, macro_def->pos, "see previous definition of '%s'", strpool_cstr( ctx->strings, name ) );
                        break;
                    }
                }
            }
        }

        // remove previous definition of macro
        preproc_remove_macro_def( ctx, name );
    }

    // store macro
    preproc_add_macro_def( ctx, name, pos, arg_count, argument_list, replacement_list );

    return preproc_skip_to_next_line( token );
}


static preproc_token_t* preproc_do_undef( preproc_ctx_t* ctx, preproc_token_t* token ) {
    token = preproc_skip_whitespace( token );
    
    if( token->type != PREPROC_TOKEN_IDENTIFIER ) {
        preproc_error( ctx, token->pos, "#undef expected an identifier" );
        return preproc_skip_to_next_line( token );
    }

    // macro name
    STRPOOL_U32 name = token->lexeme;
    token = preproc_token_next( token ); // skip macro name

    preproc_remove_macro_def( ctx, name );

    token = preproc_skip_whitespace( token );
    if( token->type != PREPROC_TOKEN_NEWLINE ) {
        preproc_warning( ctx, token->pos, "unexpected tokens following preprocessor directive - expected a newline" );
        return preproc_skip_to_next_line( token );   
    }

    return preproc_skip_to_next_line( token );
}


static preproc_token_t* preproc_do_if_section_or_control_line( preproc_ctx_t* ctx, preproc_token_t* token ) {
    token = preproc_skip_whitespace(  token );

    // identify directive
    if( token->type == PREPROC_TOKEN_IDENTIFIER ) {
        STRPOOL_U32 lexeme = token->lexeme;
        char const* pos = token->pos;
        token = preproc_token_next( token ); // skip identifier
        if( lexeme == ctx->identifier_if ) {
        } else if( lexeme == ctx->identifier_ifdef ) {
        } else if( lexeme == ctx->identifier_ifndef ) {
        } else if( lexeme == ctx->identifier_elif ) {
        } else if( lexeme == ctx->identifier_else ) {
        } else if( lexeme == ctx->identifier_endif ) {
        } else if( lexeme == ctx->identifier_include ) {
            return preproc_do_include( ctx, token );
        } else if( lexeme == ctx->identifier_define ) {
            return preproc_do_define( ctx, token );
        } else if( lexeme == ctx->identifier_undef ) {
            return preproc_do_undef( ctx, token );
        } else if( lexeme == ctx->identifier_line ) {
        } else if( lexeme == ctx->identifier_error ) {
        } else if( lexeme == ctx->identifier_pragma ) {
        } else if( lexeme == ctx->identifier_defined ) {
        } else {
            preproc_error( ctx, pos, "invalid preprocessor command '%s'", strpool_cstr( ctx->strings, lexeme ) );
            return preproc_skip_to_next_line( token );   
        }
    } else {        
        preproc_error( ctx, token->pos, "expected preprocessor directive, found '%s'", strpool_cstr( ctx->strings, token->lexeme ) );
        return preproc_skip_to_next_line( token );   
    }

    // skip until end of line
    // TODO: this is temporary, should be removed once all cases above are handled
    while( token->type != PREPROC_TOKEN_EOS ) {
        if( token->type == PREPROC_TOKEN_NEWLINE ) {
            token = preproc_token_next( token );
            break;
        } else {
            token = preproc_token_next( token );
        }
    }

    return token;
}


// TODO: Make this iterative instead of recursive, and hold an explicit data stack if needed
static void preproc_replacement_list( preproc_ctx_t* ctx, preproc_token_t* replacement_list, int arg_count,
    preproc_expanded_arg_t* args, preproc_token_t* expanded, char const* pos ) {

    preproc_token_t* replacement = replacement_list;
    while( replacement->type != PREPROC_TOKEN_EOS ) {
        preproc_macro_def_t* macro_def = NULL;
        if( replacement->type == PREPROC_TOKEN_IDENTIFIER ) {
            macro_def = preproc_get_macro_def( ctx, replacement->lexeme );
            if( macro_def ) {
                if( macro_def->is_being_expanded ) {
                    macro_def = NULL;
                } else {
                    macro_def->is_being_expanded = 1;
                }
            }
        }
        
        if( replacement->type == PREPROC_TOKEN_MACRO_ARG && args ) {
            if( (int) replacement->lexeme < arg_count ) {
                preproc_token_t* arg = args[ replacement->lexeme ].token;
                while( arg->type != PREPROC_TOKEN_EOS ) {
                    preproc_add_macro_token( ctx, arg );
                    arg = preproc_token_next( arg );
                }
            }
        } else if( !macro_def ) {
            preproc_add_macro_token( ctx, replacement );
        } else {
            preproc_replacement_list( ctx, macro_def->replacement_list, arg_count, args, expanded, pos );
            macro_def->is_being_expanded = 0;
        }
        ++replacement;
    }
}


// TODO: Make this iterative instead of recursive, and hold an explicit data stack if needed
static preproc_token_t* preproc_expand_obj_like_macro( preproc_ctx_t* ctx, preproc_macro_def_t* macro_def, 
    char const* pos ) {

    macro_def->is_being_expanded = 1;
    preproc_token_t* expanded = preproc_get_macro_tokens( ctx );
    preproc_replacement_list( ctx, macro_def->replacement_list, 0, NULL, expanded, pos );

    macro_def->is_being_expanded = 0;
    preproc_add_macro_token_eos( ctx, pos );
    return expanded;
}


static preproc_token_t* preproc_expand_func_like_macro( preproc_ctx_t* ctx, preproc_macro_def_t* func_macro_def, 
    preproc_token_t* token, preproc_token_t** out_expanded ) {
    
    char const* pos = token->pos;
    preproc_expanded_arg_t expanded_args[ 256 ]; // TODO: use dynamic allocation from a pool in preproc_ctx
    preproc_expanded_arg_t* arg = expanded_args;
    arg->token = preproc_get_args_tokens( ctx );
    int paren_match = 0;
    while( !( token->type == PREPROC_TOKEN_OPERATOR && token->lexeme == ctx->identifier_rparen && paren_match == 0 ) ) {
        if( token->type == PREPROC_TOKEN_EOS ) {
            preproc_error( ctx, token->pos, "unexpected end of file in macro expansion" );
            return token;
        } else if( token->type == PREPROC_TOKEN_IDENTIFIER ) {
            preproc_macro_def_t* macro_def = preproc_get_macro_def( ctx, token->lexeme );
            if( !macro_def ) {
                preproc_add_args_token( ctx, token );            
                token = preproc_token_next( token );
            } else {
                if( macro_def->argument_list ) {
                    // func-like macro
                    preproc_token_t prev_token = *token; // save identifier in case we need to output as-is
                    token = preproc_token_next( token ); // skip macro identifier
                    if( token->type == PREPROC_TOKEN_OPERATOR && token->lexeme == ctx->identifier_lparen ) {
                        // func-like macro invocation
                        token = preproc_token_next( token ); // skip left paren
                        int restore_point_macro = preproc_get_macro_tokens_restore_point( ctx );
                        int restore_point_args = preproc_get_args_tokens_restore_point( ctx );
                        preproc_token_t* expanded = NULL;
                        token = preproc_expand_func_like_macro( ctx, macro_def, token, &expanded );                                
                        preproc_restore_args_tokens( ctx, restore_point_args );
                        while( expanded->type != PREPROC_TOKEN_EOS ) {
                            preproc_token_t expanded_token = *expanded;
                            expanded_token.pos = pos;
                            preproc_add_args_token( ctx, &expanded_token );
                            ++expanded;
                        }       
                        preproc_restore_macro_tokens( ctx, restore_point_macro );
                    } else {
                        // macro is a func-like macro, but this is not a func-like macro invocation
                        preproc_add_args_token( ctx, &prev_token );            
                    }                            
                } else {
                    int restore_point = preproc_get_macro_tokens_restore_point( ctx );
                    preproc_token_t* expanded = preproc_expand_obj_like_macro( ctx, macro_def, token->pos );
                    while( expanded->type != PREPROC_TOKEN_EOS ) {
                        preproc_token_t expanded_token = *expanded;
                        expanded_token.pos = token->pos;
                        preproc_add_args_token( ctx, &expanded_token );
                        ++expanded;
                    }       
                    preproc_restore_macro_tokens( ctx, restore_point );
                    token = preproc_token_next( token );
                }
            }
        } else if( token->type == PREPROC_TOKEN_OPERATOR && token->lexeme == ctx->identifier_comma  && paren_match == 0 ) {
            preproc_add_args_token_eos( ctx, token->pos );
            ++arg;
            arg->token = preproc_get_args_tokens( ctx );
            token = preproc_token_next( token );
        } else if( token->type == PREPROC_TOKEN_OPERATOR && token->lexeme == ctx->identifier_lparen ) {
            ++paren_match;
            preproc_add_args_token( ctx, token );            
            token = preproc_token_next( token );
        } else if( token->type == PREPROC_TOKEN_OPERATOR && token->lexeme == ctx->identifier_rparen ) {
            --paren_match;
            preproc_add_args_token( ctx, token );            
            token = preproc_token_next( token );
        } else {
            preproc_add_args_token( ctx, token );            
            token = preproc_token_next( token );
        }
    }
    preproc_add_args_token_eos( ctx, token->pos );
    token = preproc_token_next( token );

    int arg_count = ( arg - expanded_args ) + ( arg->token->type != PREPROC_TOKEN_EOS ? 1 : 0 );

    if( arg_count < func_macro_def->arg_count ) {
        preproc_warning( ctx, pos, "not enough arguments for function-like macro invocation '%s'", 
            strpool_cstr( ctx->strings, func_macro_def->name ) );
    }

    if( arg_count > func_macro_def->arg_count ) {
        preproc_warning( ctx, pos, "too many arguments for function-like macro invocation '%s'", 
            strpool_cstr(  ctx->strings, func_macro_def->name ) );
    }

    func_macro_def->is_being_expanded = 1;
    preproc_token_t* expanded = preproc_get_macro_tokens( ctx );
    preproc_replacement_list( ctx, func_macro_def->replacement_list, arg_count, expanded_args, expanded, pos );

    func_macro_def->is_being_expanded = 0;
    preproc_add_macro_token_eos( ctx, token->pos );

    if( out_expanded ) {
        *out_expanded = expanded;
    }
    return token;
}


static preproc_token_t* preproc_process_stream( preproc_ctx_t* ctx, preproc_token_t* tokens ) {
    preproc_token_t* token = tokens;
    while( token->type != PREPROC_TOKEN_EOS ) {
        preproc_token_t* first_non_whitespace = preproc_skip_whitespace( token );
        if( first_non_whitespace->type == PREPROC_TOKEN_OPERATOR 
            && first_non_whitespace->lexeme == ctx->identifier_hash ) {
            first_non_whitespace = preproc_token_next( first_non_whitespace ); // skip '#' symbol
            token = preproc_do_if_section_or_control_line( ctx, first_non_whitespace );
        } else {
            while( token->type != PREPROC_TOKEN_EOS ) {
                if( token->type == PREPROC_TOKEN_IDENTIFIER ) {
                    preproc_macro_def_t* macro_def = preproc_get_macro_def( ctx, token->lexeme );
                    if( !macro_def ) {
                        preproc_output( ctx, token );
                        token = preproc_token_next( token ); // skip the added token
                    } else {
                        if( macro_def->argument_list ) {
                            preproc_token_t prev_token = *token; // save identifier in case we need to output as-is
                            token = preproc_token_next( token ); // skip macro identifier
                            // func-like macro
                            if( token->type == PREPROC_TOKEN_OPERATOR && token->lexeme == ctx->identifier_lparen ) {
                                token = preproc_token_next( token ); // skip left paren
                                // func-like macro invocation
                                int restore_point = preproc_get_macro_tokens_restore_point( ctx );
                                char const* pos = token->pos;
                                preproc_token_t* expanded = NULL;
                                token = preproc_expand_func_like_macro( ctx, macro_def, token, &expanded );                                
                                while( expanded->type != PREPROC_TOKEN_EOS ) {
                                    preproc_token_t expanded_token = *expanded;
                                    expanded_token.pos = pos;
                                    preproc_output( ctx, &expanded_token );
                                    ++expanded;
                                }       
                                preproc_restore_macro_tokens( ctx, restore_point );
                            } else {
                                // macro is a func-like macro, but this is not a func-like macro invocation
                                // so just output as is (it's just a regular identifier)
                                preproc_output( ctx, &prev_token );
                            }                            
                        } else {
                            int restore_point = preproc_get_macro_tokens_restore_point( ctx );
                            preproc_token_t* expanded = preproc_expand_obj_like_macro( ctx, macro_def, token->pos );
                            while( expanded->type != PREPROC_TOKEN_EOS ) {
                                preproc_token_t expanded_token = *expanded;
                                expanded_token.pos = token->pos;
                                preproc_output( ctx, &expanded_token );
                                ++expanded;
                            }       
                            preproc_restore_macro_tokens( ctx, restore_point );
                            token = preproc_token_next( token ); // skip macro identifier
                        }
                    }
                } else {
                    preproc_output( ctx, token );
                
                    if( token->type == PREPROC_TOKEN_WARNING ) {
                        ++ctx->warnings_count;
                    }
                    if( token->type == PREPROC_TOKEN_ERROR ) {
                        ++ctx->errors_count;
                    }

                    if( token->type == PREPROC_TOKEN_NEWLINE ) {
                        token = preproc_token_next( token );  // skip the added token
                        break;
                    } else {
                        token = preproc_token_next( token );  // skip the added token
                    }
                }
            }
        }
    }
    return token;
}


preproc_token_t* preproc_process( preproc_token_t* tokens, strpool_t* strings, preproc_load_file_t load_func, 
    void* load_context, int* warnings_count, int* errors_count ) {

    // ctx context
    preproc_ctx_t context;
    preproc_ctx_t* ctx = &context;
    memset( ctx, 0, sizeof( *ctx ) );
    ctx->strings = strings;
    ctx->load_func = load_func;
    ctx->load_context = load_context;
    ctx->warnings_count = 0;
    ctx->errors_count = 0;
    
    // predefine identifiers
    ctx->identifier_hash = preproc_add_string( strings, "#", 0 );
    ctx->identifier_lparen = preproc_add_string( strings, "(", 0 );
    ctx->identifier_rparen = preproc_add_string( strings, ")", 0 );
    ctx->identifier_comma = preproc_add_string( strings, ",", 0 );
    ctx->identifier_if = preproc_add_string( strings, "if", 0 );
    ctx->identifier_ifdef = preproc_add_string( strings, "ifdef", 0 );
    ctx->identifier_ifndef = preproc_add_string( strings, "ifndef", 0 );
    ctx->identifier_elif = preproc_add_string( strings, "elif", 0 );
    ctx->identifier_else = preproc_add_string( strings, "else", 0 );
    ctx->identifier_endif = preproc_add_string( strings, "endif", 0 );
    ctx->identifier_include = preproc_add_string( strings, "include", 0 );
    ctx->identifier_define = preproc_add_string( strings, "define", 0 );
    ctx->identifier_undef = preproc_add_string( strings, "undef", 0 );
    ctx->identifier_line = preproc_add_string( strings, "line", 0 );
    ctx->identifier_error = preproc_add_string( strings, "error", 0 );
    ctx->identifier_pragma = preproc_add_string( strings, "pragma", 0 );
    ctx->identifier_defined = preproc_add_string( strings, "defined", 0 );

    preproc_token_t* output = NULL;
    if( setjmp( ctx->exit_jump ) == 0 ) {
        // allocate memory for macro defs
        ctx->macro_defs_count = 0;
        ctx->macro_defs_capacity = 256;
        ctx->macro_defs = (preproc_macro_def_t*) malloc( ctx->macro_defs_capacity * sizeof( preproc_macro_def_t ) );
        if( !ctx->macro_defs ) {        
            PREPROC_ALLOC_FAIL_EXIT( "Failed to allocate initial macro defs array", tokens->pos ); 
        }

        // allocate memory for macro tokens
        ctx->macro_tokens_count = 0;
        ctx->macro_tokens_capacity = 256;
        ctx->macro_tokens = (preproc_token_t*) malloc( ctx->macro_tokens_capacity * sizeof( preproc_token_t ) );
        if( !ctx->macro_tokens ) {        
            PREPROC_ALLOC_FAIL_EXIT( "Failed to allocate initial macro tokens array", tokens->pos ); 
        }

        // allocate memory for args tokens
        ctx->args_tokens_count = 0;
        ctx->args_tokens_capacity = 256;
        ctx->args_tokens = (preproc_token_t*) malloc( ctx->args_tokens_capacity * sizeof( preproc_token_t ) );
        if( !ctx->args_tokens ) {        
            PREPROC_ALLOC_FAIL_EXIT( "Failed to allocate initial args tokens array", tokens->pos ); 
        }

        // allocate memory for output stream
        ctx->output_count = 0;
        ctx->output_capacity = 4096;
        ctx->output = (preproc_token_t*) malloc( ctx->output_capacity * sizeof( preproc_token_t ) );
        if( !ctx->output ) {
            PREPROC_ALLOC_FAIL_EXIT( "Failed to allocate initial output array", tokens->pos ); 
        }

        // process token stream
        preproc_token_t* token = preproc_process_stream( ctx, tokens );

        // add end-of-stream token
        preproc_output( ctx, token );

        // return warning count
        if( warnings_count ) {
            *warnings_count = ctx->warnings_count;
        }

        // return error count
        if( errors_count ) {
            *errors_count = ctx->errors_count;
        }
        
        output = ctx->output;
    } else {
        static preproc_token_t alloc_fail_output[ 2 ]; // TODO: use thread local if available
        alloc_fail_output[ 0 ].type = PREPROC_TOKEN_ERROR;
        alloc_fail_output[ 0 ].lexeme = ctx->alloc_error;
        alloc_fail_output[ 0 ].pos = ctx->alloc_error_pos;
        alloc_fail_output[ 1 ].type = PREPROC_TOKEN_EOS;
        alloc_fail_output[ 1 ].lexeme = 0;
        alloc_fail_output[ 1 ].pos = ctx->alloc_error_pos;
        output = alloc_fail_output;
        if( ctx->output ) {
            free( ctx->output );
        }
    }

    if( ctx->args_tokens ) { 
        free( ctx->args_tokens );
    }
    if( ctx->macro_tokens ) {
        free( ctx->macro_tokens );
    }
    if( ctx->macro_defs ) {
        free( ctx->macro_defs );
    }
    return output;
}


void preproc_free_tokens( preproc_token_t* tokens ) {
    free( tokens );
}


size_t preproc_generate_source_code( preproc_token_t* tokens, char* output, size_t capacity ) {
    (void) tokens, output, capacity;
    // TODO: implement
    return 0;
}


#endif /* PREPROC_IMPLEMENTATION */
