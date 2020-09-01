#ifndef preproc_h
#define preproc_h

#ifndef PREPROC_STR
    #define PREPROC_STR char const*
#endif

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
    PREPROC_STR lexeme; // will be 0 (empty string) for token types EOS, NEWLINE and WHITESPACE
    char const* pos; // pointer to start location (in source data) of this token
} preproc_token_t;


// returns a stream of tokens, the last token indicated by type PREPROC_TOKEN_EOS. 'source' is expected to be
// a zero-terminated string. 'strings' is assumed to be initialized already. the optional param 'errors_count'
// will be assigned the total number of errors encountered. information on individual errors can be found by scanning
// the returned token stream for tokens of type PREPROC_TOKEN_ERROR. the token stream returned should be released
// by calling free
preproc_token_t* preproc_tokenize( char const* source, void* string_pool, int* warnings_count, int* errors_count );


typedef char const* (*preproc_load_file_t)( void* load_context, PREPROC_STR header_name, strpool_t* strings );


preproc_token_t* preproc_process( preproc_token_t* tokens, strpool_t* strings, preproc_load_file_t load_func, 
    void* load_context, int* warnings_count, int* errors_count );


void preproc_free_tokens( preproc_token_t* tokens );


char* preproc_generate_source_code( preproc_token_t* tokens );

void preproc_free_source_code( char* source_code );

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


// default string handlers if no custom one is specified
#if !defined( PREPROC_STR_ALLOC ) || !defined( PREPROC_STR_CSTR ) || !defined( PREPROC_STR_EQ ) || !defined( PREPROC_STR_NULL )
    #define PREPROC_STR char const*
    #define PREPROC_STR_NULL NULL
    #define PREPROC_STR_ALLOC( pool, str, len ) ( alloc_string( (pool), (str), (len) ) )
    #define PREPROC_STR_CSTR( pool, handle ) ( (void) (pool), (handle) )
    #define PREPROC_STR_EQ( pool, a, b ) ( (void)(pool), ( strcmp( (a), (b) ) == 0 ) )

    static PREPROC_STR alloc_string( void* pool, char const* str, size_t len ) {
        (void) pool;
        char* copy = (char*) malloc( len + 1 );
        memcpy( copy, str, len );
        return copy;
    }
#endif


// convenience macros for more readable expressions
#define CSTR( str ) PREPROC_STR_CSTR( (ctx)->strings, (str) )
#define STR_EQ( a, b ) PREPROC_STR_EQ( (ctx)->strings, (a), (b) )
#define STR_VALID( str ) ( !PREPROC_STR_EQ( (ctx)->strings, (str), (PREPROC_STR_NULL) ) )
#define NULL_STR (PREPROC_STR_NULL)


// handler for exit-on-error. default is to longjmp to the end of main processing function, do cleanup and return a
// proper error message. if for some reason longjmp is undesireable, it can be disabled by defining `PREPROC_NO_LONGJMP`
// in which case we just call `exit` after printing the error message to `stderr`.
#ifdef PREPROC_NO_LONGJMP
    #define ALLOC_FAIL_EXIT(message,pos) \
        do { \
            fprintf( stderr, "%s\n", message ); exit( EXIT_FAILURE ); \
        } while( 0 )
#else
    #include <setjmp.h>

    #define ALLOC_FAIL_EXIT(message,pos) \
        do { \
            ctx->alloc_error = PREPROC_STR_ALLOC( ctx->strings, message, strlen( message ) ); \
            longjmp( ctx->exit_jump, 1 ); \
        } while( 0 )
#endif


// internal token types, will never be present in the token stream returned to external caller
#define TOKEN_MACRO_ARG -1


typedef struct token_t {
    #ifdef _DEBUG
        char const* debug_pos;
    #endif
    preproc_token_t** tokens;
    int index;
} token_t;


preproc_token_type_t token_type( token_t token ) {
    return (*token.tokens)[ token.index ].type;
}


PREPROC_STR token_lexeme( token_t token ) {
    return (*token.tokens)[ token.index ].lexeme;
}


int token_arg_index( token_t token ) {
    return (int)*((short*)(&(*token.tokens)[ token.index ].lexeme));
}


char const* token_pos( token_t token ) {
    return (*token.tokens)[ token.index ].pos;
}


preproc_token_t get_preproc_token( token_t token ) {
    return (*token.tokens)[ token.index ];
}


static token_t make_token( preproc_token_t** tokens, int index ) {
    token_t token;
    token.tokens = tokens;
    token.index = index;
    #ifdef _DEBUG
        token.debug_pos = (*token.tokens)[ token.index ].pos;
    #endif
    return token;
}


static token_t token_next( token_t token ) {
    if( token_type( token ) == PREPROC_TOKEN_EOS ) {
        return token;
    }
    ++token.index;
    #ifdef _DEBUG
        token.debug_pos = (*token.tokens)[ token.index ].pos;
    #endif
    return token;
}

static token_t token_next_ignore_eos( token_t token ) {
    ++token.index;
    #ifdef _DEBUG
        token.debug_pos = (*token.tokens)[ token.index ].pos;
    #endif
    return token;
}

#ifdef _DEBUG
    preproc_token_t* debug_token( token_t* t ) {
        return &(*t->tokens)[ t->index ];
    }
#endif


// context used during tokenization process
typedef struct tokenizer_t {
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
        PREPROC_STR alloc_error;
        char const* alloc_error_pos;
    #endif
} tokenizer_t;


// returns the next char, after merging lines ending in the '\' character
static char get_char( tokenizer_t* ctx ) {
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
static char peek_char( tokenizer_t* ctx ) {
    char const* source = ctx->source;
    char const* current_char_pos = ctx->current_char_pos;
    char c = get_char( ctx );
    ctx->source = source;
    ctx->current_char_pos = current_char_pos;
    return c;
}


// helper function for building strings. will resize the accumulation buffer as needed
static size_t preproc_accumulate( tokenizer_t* ctx, size_t pos, char c ) {
    if( pos >= ctx->accumulation_buffer_capacity ) {
        ctx->accumulation_buffer_capacity *= 2;
        void* new_buffer = realloc( ctx->accumulation_buffer, sizeof( char) * ctx->accumulation_buffer_capacity );
        if( !new_buffer ) {
            ALLOC_FAIL_EXIT( "Failed to reallocate accumulation buffer", ctx->current_char_pos );
        }
        ctx->accumulation_buffer = (char*) new_buffer;
    }
    ctx->accumulation_buffer[ pos ] = c;
    return pos + 1;
}


// helper function to add a string to the string pool (really just to not have to do the necessary casts everywhere)
static PREPROC_STR add_string( void* strings, char const* str, size_t len ) {
    if( len == 0 ) {
        len = strlen( str );
    }
    return PREPROC_STR_ALLOC( strings, str, len );
}


// helper function to add a warning message to the string pool, and also increase the warning count
static PREPROC_STR tokenizer_warning( tokenizer_t* ctx, char const* message ) {
    ++ctx->warnings_count;
    return add_string( ctx->strings, message, strlen( message ) );
}


// helper function to add an error message to the string pool, and also increase the error count
static PREPROC_STR tokenizer_error( tokenizer_t* ctx, char const* message ) {
    ++ctx->errors_count;
    return add_string( ctx->strings, message, strlen( message ) );
}


// list of all operators and punctuators, ordered by length
static char const* operators[] = {
    //"sizeof", // handled as a special case when tokenizing identifiers
    "...","<<=",">>=",
    "<<",">>","<=",">=","==","!=","*=","/=","%=","+=","-=","&=","^=","|=","->","++","--","&&","||","##",
    "[","]","(",")",".","&","*","+","-","~","!","/","%","<",">","^","|","?",":","=",",","#","{","}",";",
};


// by using only two characters, determine if it matches the first two chars of any of the operators/punctuators
// returns NULL if there is no match. if a match is found, we can read one more char and call the other helper
// function 'is_operator_three_char' to check if it match one of the three-char operators/punctuators
static char const* is_operator_two_char( char a, char b ) {
    int const count = sizeof( operators ) / sizeof( *operators );
    for( int i = 0; i < count; ++i ) {
        if( operators[ i ][ 0 ] == a ) {
            if( operators[ i ][ 1 ] == '\0' ) {
                return operators[ i ];
            } else if( operators[ i ][ 1 ] == b ) {
                return operators[ i ];
            }           
        }
    }
    return NULL;
}


// by using three characters, determine the exact match to one of the operators/punctuators. returns NULL if no match
static char const* is_operator_three_char( char a, char b, char c ) {
    int const count = sizeof( operators ) / sizeof( *operators );
    for( int i = 0; i < count; ++i ) {

        if( operators[ i ][ 0 ] == a ) {
            if( operators[ i ][ 1 ] == '\0' ) {
                return operators[ i ];
            } else if( operators[ i ][ 1 ] == b ) {
                if( operators[ i ][ 2 ] == '\0' ) {
                    return operators[ i ];
                } else if( operators[ i ][ 2 ] == c ) {
                    return operators[ i ];
                }
            }           
        }
    }
    return NULL;
}


// starts with the current char and reads more chars as needed until a full token has been scanned, then returns it
// the 'in_include_directive' param should be non-zero if the preceding tokens for the '#include' directive
static preproc_token_t lex_next_token( tokenizer_t* ctx, int in_include_directive ) {
    // token to return - initial values will be overridden as needed
    preproc_token_t token;
    token.type = PREPROC_TOKEN_ERROR;
    token.lexeme = NULL_STR;
    token.pos = ctx->current_char_pos;

    // End of stream
    if( ctx->current_char == '\0' )  {
        token.type = PREPROC_TOKEN_EOS;
        return token;
    }
    
    // Whitespace
    if( isspace( (unsigned char) ctx->current_char ) && ctx->current_char != '\n' ) {
        while( isspace( (unsigned char) ctx->current_char ) && ctx->current_char != '\n' ) {
            ctx->current_char = get_char( ctx );
        }
        token.type = PREPROC_TOKEN_WHITESPACE;    
        return token;
    }

    // Line comment
    if( ctx->current_char == '/' && peek_char( ctx ) == '/' ) {
        while( ctx->current_char && ctx->current_char != '\n' )  {
            ctx->current_char = get_char( ctx );
        }
        token.type = PREPROC_TOKEN_WHITESPACE;    
        return token;
    }

    // Block comment
    if( ctx->current_char == '/' && peek_char( ctx ) == '*' ) {
        ctx->current_char = get_char( ctx );
        ctx->current_char = get_char( ctx );
        while( !( ctx->current_char == '*' && peek_char( ctx ) == '/' ) )  {
            if( ctx->current_char == '\0' ) {
                token.type = PREPROC_TOKEN_ERROR;
                token.lexeme = tokenizer_error( ctx, "unexpected end of file found in comment" );
                return token;
            }
            ctx->current_char = get_char( ctx );
        }
        ctx->current_char = get_char( ctx );
        ctx->current_char = get_char( ctx );
        token.type = PREPROC_TOKEN_WHITESPACE;    
        return token;
    }

    // Newline
    if( ctx->current_char == '\n' ) {
        ctx->current_char = get_char( ctx );
        token.type = PREPROC_TOKEN_NEWLINE;    
        return token;
    }

    // Identifier
    if( isalpha( (unsigned char) ctx->current_char ) || ctx->current_char == '_' ) { 
        size_t acc_pos = 0;
        while( isalnum( (unsigned char) ctx->current_char ) || ctx->current_char == '_' ) {
            acc_pos = preproc_accumulate( ctx, acc_pos, ctx->current_char );
            ctx->current_char = get_char( ctx );
        }
        token.type = PREPROC_TOKEN_IDENTIFIER;
        if( acc_pos == strlen( "sizeof") && strncmp( ctx->accumulation_buffer, "sizeof", acc_pos ) == 0 ) {
            token.type = PREPROC_TOKEN_OPERATOR;
        }
        token.lexeme = add_string( ctx->strings, ctx->accumulation_buffer, acc_pos );
        return token;
    }

    // Header name
    if( in_include_directive && ( ctx->current_char == '<' || ctx->current_char == '"' ) ) {
        char terminator = ctx->current_char == '<' ? '>' : '"';
        size_t acc_pos = 0;
        acc_pos = preproc_accumulate( ctx, acc_pos, ctx->current_char );
        ctx->current_char = get_char( ctx );
        while( ctx->current_char != terminator ) {
            if( !ctx->current_char || ctx->current_char == '\n') {
                token.type = PREPROC_TOKEN_ERROR;
                token.lexeme = tokenizer_error( ctx, 
                    terminator == '>' ? "missing '>'" : "newline in constant" );
                return token;
            }
            acc_pos = preproc_accumulate( ctx, acc_pos, ctx->current_char );
            ctx->current_char = get_char( ctx );
        }
        acc_pos = preproc_accumulate( ctx, acc_pos, ctx->current_char );
        ctx->current_char = get_char( ctx );
        token.lexeme = add_string( ctx->strings, ctx->accumulation_buffer, acc_pos );
        token.type = PREPROC_TOKEN_HEADER_NAME;
        return token;
    }

    // PP Number
    if( isdigit( (unsigned char) ctx->current_char ) || ( ctx->current_char == '.' 
        && isdigit( (unsigned char) peek_char( ctx ) ) ) ) {   

        size_t acc_pos = 0;
        while( isdigit( (unsigned char) ctx->current_char ) ) {
            acc_pos = preproc_accumulate( ctx, acc_pos, ctx->current_char );
            ctx->current_char = get_char( ctx );
        }
        if( ctx->current_char == '.' ) {
            acc_pos = preproc_accumulate( ctx, acc_pos, ctx->current_char );
            ctx->current_char = get_char( ctx );
        }
        while( isdigit( (unsigned char) ctx->current_char ) ) {
            acc_pos = preproc_accumulate( ctx, acc_pos, ctx->current_char );
            ctx->current_char = get_char( ctx );
        }
        if( ctx->current_char == 'e' || ctx->current_char == 'E' ) {
            acc_pos = preproc_accumulate( ctx, acc_pos, ctx->current_char );
            ctx->current_char = get_char( ctx );
            if( ctx->current_char == '+' || ctx->current_char == '-' ) {
                acc_pos = preproc_accumulate( ctx, acc_pos, ctx->current_char );
                ctx->current_char = get_char( ctx );
            }
            while( isdigit( (unsigned char) ctx->current_char ) ) {
                acc_pos = preproc_accumulate( ctx, acc_pos, ctx->current_char );
                ctx->current_char = get_char( ctx );
            }
        }
        while( isalpha( (unsigned char) ctx->current_char ) ) {
            acc_pos = preproc_accumulate( ctx, acc_pos, ctx->current_char );
            ctx->current_char = get_char( ctx );
        }
        token.lexeme = add_string( ctx->strings, ctx->accumulation_buffer, acc_pos );
        token.type = PREPROC_TOKEN_PP_NUMBER;
        return token;
    } 

    // Operators and punctuators
    if( is_operator_two_char( ctx->current_char, peek_char( ctx ) ) ) {
        char prev_char = ctx->current_char;
        ctx->current_char = get_char( ctx );
        char const* str = is_operator_three_char( prev_char, ctx->current_char, 
            peek_char( ctx ) );
        size_t len = strlen( str );
        if( len == 2 ) {
            ctx->current_char = get_char( ctx );
        } else if( len == 3 ) {
            ctx->current_char = get_char( ctx );
            ctx->current_char = get_char( ctx );
        } 
        token.lexeme = add_string( ctx->strings, str, len );
        token.type = PREPROC_TOKEN_OPERATOR;
        return token;
    }

    // String literals
    // TODO: escape sequences
    if( ctx->current_char == '"' ) {   
        ctx->current_char = get_char( ctx );
        size_t acc_pos = 0;
        ctx->accumulation_buffer[ 0 ] = '\0'; // TODO: Make sure to handle zero-len acc buffer everywher it is used
        while( ctx->current_char != '"' ) {
            if( ctx->current_char == '\n' ) {
                token.type = PREPROC_TOKEN_ERROR;
                token.lexeme = tokenizer_error( ctx, "newline in constant" );
                return token;
            }
            acc_pos = preproc_accumulate( ctx, acc_pos, ctx->current_char );
            if( ctx->current_char == '\\' && peek_char( ctx ) == '"' ) {
                ctx->current_char = get_char( ctx );
                acc_pos = preproc_accumulate( ctx, acc_pos, ctx->current_char );
            }
            ctx->current_char = get_char( ctx );
        }
        ctx->current_char = get_char( ctx );

        token.lexeme = add_string( ctx->strings, ctx->accumulation_buffer, acc_pos );
        token.type = PREPROC_TOKEN_STRING_LITERAL ;
        return token;
    }

    // Char constants
    // TODO: escape sequences
    if( ctx->current_char == '\'' ) {   
        ctx->current_char = get_char( ctx );
        size_t acc_pos = 0;
        while( ctx->current_char != '\'' ) {
            if( ctx->current_char == '\n' ) {
                token.type = PREPROC_TOKEN_ERROR;
                token.lexeme = tokenizer_error( ctx, "newline in constant" );
                return token;
            }
            acc_pos = preproc_accumulate( ctx, acc_pos, ctx->current_char );
            if( ctx->current_char == '\\' && ( peek_char( ctx ) == '\'' 
                || peek_char( ctx ) == '\\' ) ) {

                ctx->current_char = get_char( ctx );
                acc_pos = preproc_accumulate( ctx, acc_pos, ctx->current_char );
            }
            ctx->current_char = get_char( ctx );
        }
        ctx->current_char = get_char( ctx );

        token.lexeme = add_string( ctx->strings, ctx->accumulation_buffer, acc_pos );
        token.type = PREPROC_TOKEN_CHARACTER_CONSTANT;
        return token;
    }

    // Catchall for any characters not caught by the above - will probably be rejected in later stages
    token.type = PREPROC_TOKEN_NON_WHITESPACE;
    token.lexeme = add_string( ctx->strings, &ctx->current_char, 1 );   
    ctx->current_char = get_char( ctx );
    return token;
}


// to make implementation easier, for some token types (whitespace, newline and non-whitespace) consecutive tokens are 
// collapsed to a single token of that type, and lexemes merged. thuis is a helper function to do that
static size_t merge_tokens( tokenizer_t* ctx, preproc_token_t* token, preproc_token_t const* tokens, 
    size_t count ) {
    if( count > 0 ) { // we can't merge if no preceding token exitt
        preproc_token_t const* prev_token = &tokens[ count - 1 ];
        if( token->type == prev_token->type ) { // only merge if both tokens are the same tyoe
            // copy lexeme of first token to accumulation buffer
            size_t pos = 0;
            if( STR_VALID( prev_token->lexeme ) ) {
                char const* str = CSTR( prev_token->lexeme );
                int len = strlen( str );
                for( int i = 0; i < len; ++i ) {
                    pos = preproc_accumulate( ctx, pos, str[ i ] );
                }
            }
            
            // append lexeme of second token to accumulation buffer
            if( STR_VALID( token->lexeme ) ) {
                char const* str = CSTR( token->lexeme );
                int len = strlen( str );
                for( int i = 0; i < len; ++i ) {
                    pos = preproc_accumulate( ctx, pos, str[ i ] );
                }
            }

            if( pos > 0 ) {
                token->lexeme = add_string( ctx->strings, ctx->accumulation_buffer, pos );
            }

            --count; // so when this token gets added after we loop around, it overwrites the one we merged with
        }
    }
    return count;
}


static preproc_token_t alloc_fail_output[ 2 ]; // TODO: use thread local if available


preproc_token_t* preproc_tokenize( char const* source, strpool_t* strings, int* warnings_count, int* errors_count ) {

    // initialize the ctx context
    tokenizer_t context;
    tokenizer_t* ctx = &context;
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
            ALLOC_FAIL_EXIT( "Failed to allocate initial token output stream", source );
        }

        ctx->accumulation_buffer = (char*) malloc( sizeof( char ) * ctx->accumulation_buffer_capacity );
        if( !ctx->accumulation_buffer ) {
            ALLOC_FAIL_EXIT( "Failed to allocate initial accumulation buffer", source );
        }

            // add to string pool the strings we need to compare against during tokenization
        PREPROC_STR ident_include = add_string( ctx->strings, "include", 0 );  
        PREPROC_STR ident_hash = add_string( ctx->strings, "#", 0 );

        // initialize the current_char field with the first character from source, as lex_next_token expects this
        ctx->current_char = get_char( ctx );

        // get the first token before we enter the while loop
        preproc_token_t token = lex_next_token( ctx, 0 ); // we know we are not in an #include
    
        // tokenize until end of file
        while( token.type != PREPROC_TOKEN_EOS ) {
            // reallocate tokens array if needed
            if( count >= capacity - 3 ) { // -3 to ensure room for EOS token and possible newline (+warning) at the end
                capacity *= 2;
                void* new_tokens = realloc( tokens, capacity * sizeof( preproc_token_t ) );            
                if( !new_tokens ) {
                    ALLOC_FAIL_EXIT( "Failed to reallocate token output stream", source );
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
                && STR_EQ( tokens[ i - 1 ].lexeme, ident_include ) ) {

                --i; // skip past 'include'
                if( i > 0 && tokens[ i - 1 ].type == PREPROC_TOKEN_WHITESPACE ) {
                    --i; // skip whitespace between '#' and 'include', if any
                }
                // check if token is '#' symbol
                if( i > 0 && tokens[ i - 1 ].type == PREPROC_TOKEN_OPERATOR 
                    && STR_EQ( tokens[ i - 1 ].lexeme, ident_hash ) ) {
                
                    in_include_directive = 1; // all conditions are met, we are just after an #include directive
                }
            }

            // read next token
            token = lex_next_token( ctx, in_include_directive );

            // merge tokens of type 'whitespace', 'non-whitespace' and 'newline'
            if( token.type == PREPROC_TOKEN_NON_WHITESPACE || token.type == PREPROC_TOKEN_WHITESPACE 
                || token.type == PREPROC_TOKEN_NEWLINE ) {
                count = merge_tokens( ctx, &token, tokens, count );
            }
        }

        // ensure newline at end of file
        if( count < 1 || tokens[ count - 1 ].type != PREPROC_TOKEN_NEWLINE ) {
            preproc_token_t warning;
            warning.type = PREPROC_TOKEN_WARNING;
            warning.lexeme = tokenizer_warning( ctx, "no newline at end of file" );
            warning.pos = token.pos;
            tokens[ count++ ] = warning;

            preproc_token_t newline;
            newline.type = PREPROC_TOKEN_NEWLINE;
            newline.lexeme = NULL_STR;
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
        alloc_fail_output[ 0 ].type = PREPROC_TOKEN_ERROR;
        alloc_fail_output[ 0 ].lexeme = ctx->alloc_error;
        alloc_fail_output[ 0 ].pos = ctx->alloc_error_pos;
        alloc_fail_output[ 1 ].type = PREPROC_TOKEN_EOS;
        alloc_fail_output[ 1 ].lexeme = NULL_STR;
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


static token_t skip_whitespace( token_t token ) {
    while( token_type( token ) == PREPROC_TOKEN_WHITESPACE ) {
        token = token_next( token );
    }
    return token;
}


static token_t skip_whitespace_newline( token_t token ) {
    while( token_type( token ) == PREPROC_TOKEN_WHITESPACE || token_type( token ) == PREPROC_TOKEN_NEWLINE ) {
        token = token_next( token );
    }
    return token;
}


static token_t skip_to_next_line( token_t token ) {
    while( token_type( token ) != PREPROC_TOKEN_EOS ) {
        if( token_type( token ) == PREPROC_TOKEN_NEWLINE ) {
            token = token_next( token );
            break;
        }
        token = token_next( token );
    }
    return token;
}


typedef struct macro_def_t {
    PREPROC_STR name;
    char const* pos;
    int is_func_like;
    int arg_count;
    token_t arg_list;
    token_t replacement_list;
    int is_being_expanded;
} macro_def_t;


typedef struct expansion_context_t {
    token_t tokens;
} expansion_context_t;


typedef struct expanded_arg_t {
    token_t original;
    token_t expanded;
} expanded_arg_t;


typedef struct context_t {
    strpool_t* strings;
    preproc_load_file_t load_func;
    void* load_context;
    int warnings_count;
    int errors_count;

    PREPROC_STR ident_hash;
    PREPROC_STR ident_lparen;
    PREPROC_STR ident_rparen;
    PREPROC_STR ident_comma;
    PREPROC_STR ident_if;
    PREPROC_STR ident_ifdef;
    PREPROC_STR ident_ifndef;
    PREPROC_STR ident_elif;
    PREPROC_STR ident_else;
    PREPROC_STR ident_endif;
    PREPROC_STR ident_include;
    PREPROC_STR ident_define;
    PREPROC_STR ident_undef;
    PREPROC_STR ident_line;
    PREPROC_STR ident_error;
    PREPROC_STR ident_pragma;
    PREPROC_STR ident_defined;

    size_t condition_stack_count;
    size_t condition_stack_capacity;
    int* condition_stack;

    size_t macro_defs_count;
    size_t macro_defs_capacity;
    macro_def_t* macro_defs;

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
        PREPROC_STR alloc_error;
        char const* alloc_error_pos;
    #endif
} context_t;


// forward declares
static token_t process_stream( context_t* ctx, token_t tokens );
static int try_func_like_macro( context_t* ctx, token_t* tokens, int parent_args_count, token_t parent_args );
static int try_func_like_macro_no_parent_args( context_t* ctx, token_t* tokens );
static int try_obj_like_macro( context_t* ctx, token_t* tokens );



static void output_token( context_t* ctx, preproc_token_t token ) {
    // reallocate tokens array if needed
    if( ctx->output_count >= ctx->output_capacity - 1 ) { // -1 to ensure room for EOS token at the end
        ctx->output_capacity *= 2;
        void* new_tokens = realloc( ctx->output, ctx->output_capacity * sizeof( preproc_token_t ) );            
        if( !new_tokens ) {
            ALLOC_FAIL_EXIT( "Failed to reallocate token output buffer", token_pos( token ) );
        }
        ctx->output = (preproc_token_t*) new_tokens;            
    }

    // add token to output
    assert( token.type != TOKEN_MACRO_ARG ); // TOKEN_MACRO_ARG is internal only, should never be output
    ctx->output[ ctx->output_count++ ] = token;
}


static PREPROC_STR format_message( context_t* ctx, char const* message, va_list args ) {
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
            ALLOC_FAIL_EXIT( "Failed to allocate temp buffer in format_message", NULL );
        }
    }
	#ifdef _WIN32
		_vsnprintf( buf, size + 1, message, args );
	#else
		vsnprintf( buf, size + 1, message, args );
	#endif
    PREPROC_STR formatted_string = add_string( ctx->strings, buf, strlen( buf ) );
    if( buf != fixed_buf ) {
        free( buf );
    }
    return formatted_string;
}


static void emit_warning( context_t* ctx, char const* pos, char const* message, ... ) {
	va_list args;
	va_start( args, message );
    PREPROC_STR formatted_string = format_message( ctx, message, args );
    va_end( args );

    preproc_token_t token;
    token.type = PREPROC_TOKEN_WARNING;
    token.pos = pos;
    token.lexeme = formatted_string;
    ++ctx->warnings_count;
    output_token( ctx, token );
}


static void emit_error( context_t* ctx, char const* pos, char const* message, ... ) {
	va_list args;
	va_start( args, message );
    PREPROC_STR formatted_string = format_message( ctx, message, args );
    va_end( args );

    preproc_token_t token;
    token.type = PREPROC_TOKEN_ERROR;
    token.pos = pos;
    token.lexeme = formatted_string;
    ++ctx->errors_count;
    output_token( ctx, token );
}


static void add_macro_def( context_t* ctx, PREPROC_STR name, char const* pos, int is_func_like, int arg_count,
    token_t arg_list, token_t replacement_list ) {
    
    // reallocate macro array if needed
    if( ctx->macro_defs_count >= ctx->macro_defs_capacity ) {
        ctx->macro_defs_capacity *= 2;
        void* new_macros = realloc( ctx->macro_defs, ctx->macro_defs_capacity * sizeof( macro_def_t ) );            
        if( !new_macros ) {
            ALLOC_FAIL_EXIT( "Failed to reallocate macro def array", pos );
        }
        ctx->macro_defs = (macro_def_t*) new_macros;            
    }

    // add token 
    macro_def_t macro;
    macro.name = name;
    macro.pos = pos;
    macro.is_func_like = is_func_like;
    macro.arg_count = arg_count;
    macro.arg_list = arg_list;
    macro.replacement_list = replacement_list;
    macro.is_being_expanded = 0;
    ctx->macro_defs[ ctx->macro_defs_count++ ] = macro;
}


static void remove_macro_def( context_t* ctx, PREPROC_STR name ) {
    for( size_t i = 0; i < ctx->macro_defs_count; ++i ) {
        if( STR_EQ( ctx->macro_defs[ i ].name, name ) ) {
            ctx->macro_defs[ i ] = ctx->macro_defs[ --ctx->macro_defs_count ];
            return;
        }
    }
}


static macro_def_t* lookup_macro_def( context_t* ctx, PREPROC_STR name ) {
    // TODO: use a hash table for faster lookup of macro def from string id
    for( size_t i = 0; i < ctx->macro_defs_count; ++i ) {
        macro_def_t* macro_def = &ctx->macro_defs[ i ];
        if( STR_EQ( macro_def->name, name ) ) {
            return macro_def;
        }
    }
    return NULL;
}


static void add_macro_token( context_t* ctx, preproc_token_t token ) {
    // reallocate macro tokens array if needed
    if( ctx->macro_tokens_count >= ctx->macro_tokens_capacity ) {
        ctx->macro_tokens_capacity *= 2;
        void* new_tokens = realloc( ctx->macro_tokens, ctx->macro_tokens_capacity * sizeof( preproc_token_t ) );            
        if( !new_tokens ) {
            ALLOC_FAIL_EXIT( "Failed to reallocate macro tokens array", pos );
        }
        ctx->macro_tokens = (preproc_token_t*) new_tokens;            
    }

    // add token 
    ctx->macro_tokens[ ctx->macro_tokens_count++ ] = token;
}


static void add_macro_token_eos( context_t* ctx, char const* pos ) {
    preproc_token_t eos_token;
    eos_token.type = PREPROC_TOKEN_EOS;
    eos_token.lexeme = NULL_STR;
    eos_token.pos = pos;
    add_macro_token( ctx, eos_token );
}


static token_t get_macro_tokens( context_t* ctx ) {
    return make_token( &ctx->macro_tokens, ctx->macro_tokens_count );
}


static void add_args_token( context_t* ctx, preproc_token_t token ) {
    // reallocate args tokens array if needed
    if( ctx->args_tokens_count >= ctx->args_tokens_capacity ) {
        ctx->args_tokens_capacity *= 2;
        void* new_tokens = realloc( ctx->args_tokens, ctx->args_tokens_capacity * sizeof( preproc_token_t ) );            
        if( !new_tokens ) {
            ALLOC_FAIL_EXIT( "Failed to reallocate args tokens array", pos );
        }
        ctx->args_tokens = (preproc_token_t*) new_tokens;            
    }

    // add token 
    ctx->args_tokens[ ctx->args_tokens_count++ ] = token;
}


static void add_args_token_eos( context_t* ctx, char const* pos ) {
    preproc_token_t eos_token;
    eos_token.type = PREPROC_TOKEN_EOS;
    eos_token.lexeme = NULL_STR;
    eos_token.pos = pos;
    add_args_token( ctx, eos_token );
}


static token_t get_args_tokens( context_t* ctx ) {
    return make_token( &ctx->args_tokens, ctx->args_tokens_count );
}


static void condition_stack_push( context_t* ctx, int value ) {
    // reallocate condition stack array if needed
    if( ctx->condition_stack_count >= ctx->condition_stack_capacity ) {
        ctx->condition_stack_capacity *= 2;
        void* new_array = realloc( ctx->condition_stack, ctx->condition_stack_capacity * sizeof( int ) );            
        if( !new_array ) {
            ALLOC_FAIL_EXIT( "Failed to reallocate condition stack array", pos );
        }
        ctx->condition_stack = (int*) new_array;            
    }

    // add token 
    ctx->condition_stack[ ctx->condition_stack_count++ ] = value;
}


static int condition_stack_pop( context_t* ctx ) {
    if( ctx->condition_stack_count <= 0 ) {
        return 0; // TODO: report error? or assert?
    }
    return ctx->condition_stack[ --ctx->condition_stack_count ];
}


static token_t process_include( context_t* ctx, token_t token ) {
    token = skip_whitespace( token );
    if( token_type( token ) == PREPROC_TOKEN_HEADER_NAME ) {
        char const* source = ctx->load_func( ctx->load_context, token_lexeme( token ), ctx->strings );
        if( source ) {
            preproc_token_t* source_tokens = preproc_tokenize( source, ctx->strings, NULL, NULL ); 
            token_t tokens = make_token( &source_tokens, 0 );
            process_stream( ctx, tokens );
            preproc_free_tokens( source_tokens );
        } else {
            emit_error( ctx, token_pos( token ), "Cannot open include file: '%s': No such file or directory", 
                CSTR( token_lexeme( token ) ) );
       }
        token = token_next( token ); // skip header name
        token = skip_whitespace( token );

        if( token_type( token ) != PREPROC_TOKEN_NEWLINE ) {
            emit_warning( ctx, token_pos( token ), 
                "unexpected tokens following preprocessor directive - expected a newline" );
        }
        return skip_to_next_line( token );
    } else {
        // TODO: expand and include if it evaluates to string
    }

    return token;
}


static token_t process_define( context_t* ctx, token_t token ) {
    token = skip_whitespace( token );
    
    if( token_type( token ) != PREPROC_TOKEN_IDENTIFIER ) {
        emit_error( ctx, token_pos( token ), "#define syntax" );
        return skip_to_next_line( token );
    }

    // macro name
    PREPROC_STR name = token_lexeme( token );
    char const* pos = token_pos( token );
    token = token_next( token ); // skip macro name

    // function-like macro
    int arg_count = 0;
    token_t arg_list = { NULL, 0 };
    if( token_type( token ) == PREPROC_TOKEN_OPERATOR && STR_EQ( token_lexeme( token ), ctx->ident_lparen ) ) {
        token = token_next( token ); // skip lparen
        token = skip_whitespace( token );

        arg_list = get_macro_tokens( ctx );
        while( !( token_type( token ) == PREPROC_TOKEN_OPERATOR && STR_EQ( token_lexeme( token ), ctx->ident_rparen ) ) ) {
            if( token_type( token ) != PREPROC_TOKEN_IDENTIFIER ) {               
                if( token_type( token ) == PREPROC_TOKEN_NEWLINE ) {
                    emit_error( ctx, token_pos( token ), "'%s': unexpected in macro parameter list", "\\n" );
                } else {
                    emit_error( ctx, token_pos( token ), "'%s': unexpected in macro parameter list", 
                        CSTR( token_lexeme( token ) ) );
                }
                return skip_to_next_line( token );
            }
            token_t temp_arg = arg_list;
            for( int i = 0; i < arg_count; ++i ) {
                PREPROC_STR arg_lexeme = token_lexeme( temp_arg );
                temp_arg = token_next( temp_arg );
                if( STR_EQ( arg_lexeme, token_lexeme( token ) ) ) {
                    emit_error( ctx, token_pos( token ), "cannot reuse macro parameter name '%s'", 
                        CSTR( token_lexeme( token ) ) );
                    return skip_to_next_line( token );                
                }
            }
            add_macro_token( ctx, get_preproc_token( token ) );
            ++arg_count;
            token = token_next( token ); // skip identifier
            token = skip_whitespace( token );
                
            if( token_type( token ) == PREPROC_TOKEN_OPERATOR && STR_EQ( token_lexeme( token ), ctx->ident_comma ) ) {
                token = token_next( token );
                token = skip_whitespace( token );
                if( token_type( token ) == PREPROC_TOKEN_OPERATOR && STR_EQ( token_lexeme( token ), ctx->ident_rparen ) ) {
                    emit_error( ctx, token_pos( token ), "')': unexpected in macro parameter list" );
                    return skip_to_next_line( token );
                }
            }
        }
        token = token_next( token ); // skip rparen

        add_macro_token_eos( ctx, token_pos( token ) );
    }

    token = skip_whitespace( token );

    // parse replacement list
    token_t replacement_list = get_macro_tokens( ctx );
    while( token_type( token ) != PREPROC_TOKEN_NEWLINE ) {
        preproc_token_t token_to_add = get_preproc_token( token );
        if( token_type( token ) == PREPROC_TOKEN_IDENTIFIER ) {
            token_t temp_arg = arg_list;
            for( int i = 0; i < arg_count; ++i ) {
                PREPROC_STR arg_lexeme = token_lexeme( temp_arg );
                temp_arg = token_next( temp_arg );
                if( STR_EQ( arg_lexeme, token_lexeme( token ) ) ) {
                    token_to_add.type = TOKEN_MACRO_ARG;
                    *((short*)(&token_to_add.lexeme)) = (short)i;
                    break;
                }
            }           
        }
        add_macro_token( ctx, token_to_add );
        token = token_next( token ); 
    }
    add_macro_token_eos( ctx, token_pos( token ) );

    // check if macro exists
    macro_def_t* macro_def = lookup_macro_def( ctx, name );
    if( macro_def ) {
        // check if definitions match
        if( ( macro_def->arg_list.tokens && !arg_list.tokens ) || ( !macro_def->arg_list.tokens && arg_list.tokens ) ) {

            emit_warning( ctx, pos, "'%s': macro redefinition", CSTR( name ) );
            emit_warning( ctx, macro_def->pos, "see previous definition of '%s'", CSTR( name ) );
        } else if( arg_list.tokens ) {
            int def_arg_count = 0;
            for( token_t arg = macro_def->arg_list; token_type( arg )!= PREPROC_TOKEN_EOS; arg = token_next( arg ) ) {
                ++def_arg_count;
            }
            int new_arg_count = 0;
            for( token_t arg = arg_list; token_type( arg ) != PREPROC_TOKEN_EOS; arg = token_next( arg ) ) {
                ++new_arg_count;
            }
            if( def_arg_count != new_arg_count ) {
                // number of arguments differ, emit warning
                emit_warning( ctx, pos, "'%s': macro redefinition", CSTR( name ) );
                emit_warning( ctx, macro_def->pos, "see previous definition of '%s'", CSTR( name ) );
            } else {
                for( token_t repl = macro_def->replacement_list, new_repl = replacement_list; 
                    !( token_type( repl ) == PREPROC_TOKEN_EOS && token_type( repl ) == PREPROC_TOKEN_EOS ); 
                    repl = token_next( repl ), new_repl = token_next( new_repl ) ) {
                    if( token_type( repl ) != token_type( new_repl ) 
                        || !STR_EQ( token_lexeme( repl ), token_lexeme( new_repl ) ) ) {

                        // if the definitions don't match, emit a warning
                        emit_warning( ctx, pos, "'%s': macro redefinition", CSTR( name ) );
                        emit_warning( ctx, macro_def->pos, "see previous definition of '%s'", CSTR( name ) );
                        break;
                    }
                }
            }
        }

        // remove previous definition of macro
        remove_macro_def( ctx, name );
    }

    // store macro
    add_macro_def( ctx, name, pos, arg_list.tokens ? 1 : 0, arg_count, arg_list, replacement_list );

    return skip_to_next_line( token );
}


static token_t process_undef( context_t* ctx, token_t token ) {
    token = skip_whitespace( token );
    
    if( token_type( token ) != PREPROC_TOKEN_IDENTIFIER ) {
        emit_error( ctx, token_pos( token ), "#undef expected an identifier" );
        return skip_to_next_line( token );
    }

    // macro name
    PREPROC_STR name = token_lexeme( token );
    token = token_next( token ); // skip macro name

    remove_macro_def( ctx, name );

    token = skip_whitespace( token );
    if( token_type( token ) != PREPROC_TOKEN_NEWLINE ) {
        emit_warning( ctx, token_pos( token ), 
            "unexpected tokens following preprocessor directive - expected a newline" );
        return skip_to_next_line( token );   
    }

    return skip_to_next_line( token );
}


static token_t skip_to_next_if_section( context_t* ctx, token_t token ) {
    int count = 0;
    while( count >= 0 ) {
        if( token_type( token ) == PREPROC_TOKEN_EOS ) {
            // ERROR
        }
        token = skip_whitespace( token );
        if( token_type( token ) == PREPROC_TOKEN_OPERATOR && STR_EQ( token_lexeme( token ), ctx->ident_hash ) ) {
            token_t hash_token = token;
            token = token_next( token );
            token = skip_whitespace( token );
            if( token_type( token ) == PREPROC_TOKEN_IDENTIFIER ) {
                PREPROC_STR lexeme = token_lexeme( token );
                if( STR_EQ( lexeme, ctx->ident_if ) || STR_EQ( lexeme, ctx->ident_ifdef ) || STR_EQ( lexeme, ctx->ident_ifndef ) ) {
                    ++count;
                } else if( STR_EQ( lexeme, ctx->ident_endif ) ) {
                    --count;
                    if( count < 0 ) {
                        return hash_token;
                    }
                }
            }
        }
        token = skip_to_next_line( token );
    }
    return token;
}


static token_t process_ifdef( context_t* ctx, token_t token, int invert_condition ) {
    token = skip_whitespace( token );
    
    if( token_type( token ) != PREPROC_TOKEN_IDENTIFIER ) {
        emit_error( ctx, token_pos( token ), "#ifdef expected an identifier" );
        return skip_to_next_line( token );
    }

    // macro name
    PREPROC_STR name = token_lexeme( token );
    token = token_next( token ); // skip macro name

    token = skip_whitespace( token );
    if( token_type( token ) != PREPROC_TOKEN_NEWLINE ) {
        emit_warning( ctx, token_pos( token ), 
            "unexpected tokens following preprocessor directive - expected a newline" );
    }
    token = skip_to_next_line( token );   

    macro_def_t* macro_def = lookup_macro_def( ctx, name );
    int cond = ( !invert_condition && macro_def ) || ( invert_condition && !macro_def );

    condition_stack_push( ctx, cond );
    if( !cond ) {
        token = skip_to_next_if_section( ctx, token );
    }
    return token;
}


static token_t process_if( context_t* ctx, token_t token ) {
    token = skip_whitespace( token );
    
    // TODO: evaluate expression
    int cond = 0;
    token = skip_to_next_line( token );   

    condition_stack_push( ctx, cond );
    if( !cond ) {
        token = skip_to_next_if_section( ctx, token );
    }
    return token;
}


static token_t process_elif( context_t* ctx, token_t token ) {
    if( ctx->condition_stack_count <= 0 ) {
        emit_error( ctx, token_pos( token ), "unexpected #elif" );
        return skip_to_next_line( token );   
    }

    token = skip_whitespace( token );

    // TODO: evaluate expression
    int cond = 0;
    token = skip_to_next_line( token );   

    int branch_taken = condition_stack_pop( ctx );
    cond = cond && !branch_taken;

    condition_stack_push( ctx, cond ? 1 : branch_taken );
    if( !cond ) {
        token = skip_to_next_if_section( ctx, token );
    }
    return token;
}


static token_t process_else( context_t* ctx, token_t token ) {
    if( ctx->condition_stack_count <= 0 ) {
        emit_error( ctx, token_pos( token ), "unexpected #else" );
        return skip_to_next_line( token );   
    }

    token = skip_to_next_line( token );   

    int branch_taken = condition_stack_pop( ctx );;
    condition_stack_push( ctx, 1 );
    if( branch_taken ) {
        token = skip_to_next_if_section( ctx, token );
    }
    return token;
}


static token_t process_endif( context_t* ctx, token_t token ) {
    if( ctx->condition_stack_count <= 0 ) {
        emit_error( ctx, token_pos( token ), "unexpected #endif" );
        return skip_to_next_line( token );   
    }

    condition_stack_pop( ctx );;
    return skip_to_next_line( token );   
}


static token_t process_if_section_or_control_line( context_t* ctx, token_t token ) {
    token = skip_whitespace(  token );

    // identify directive
    if( token_type( token ) == PREPROC_TOKEN_IDENTIFIER ) {
        PREPROC_STR lexeme = token_lexeme( token );
        char const* pos = token_pos( token );
        token = token_next( token ); // skip identifier
        if( STR_EQ( lexeme, ctx->ident_if) ) {
            // #if
            return process_if( ctx, token );
        } else if( STR_EQ( lexeme, ctx->ident_ifdef) ) {
            // #ifdef
            return process_ifdef( ctx, token, 0 );
        } else if( STR_EQ( lexeme, ctx->ident_ifndef) ) {
            // #ifndef
            return process_ifdef( ctx, token, 1 );
        } else if( STR_EQ( lexeme, ctx->ident_elif) ) {
            // #elif
            return process_elif( ctx, token );
        } else if( STR_EQ( lexeme, ctx->ident_else) ) {
            // #else
            return process_else( ctx, token );
        } else if( STR_EQ( lexeme, ctx->ident_endif) ) {
            // #endif
            return process_endif( ctx, token );
        } else if( STR_EQ( lexeme, ctx->ident_include) ) {
            // #include
            return process_include( ctx, token );
        } else if( STR_EQ( lexeme, ctx->ident_define) ) {
            // #define
            return process_define( ctx, token );
        } else if( STR_EQ( lexeme, ctx->ident_undef) ) {
            // #undef
            return process_undef( ctx, token );
        } else if( STR_EQ( lexeme, ctx->ident_line) ) {
            // #line
            // TODO: implement
            return skip_to_next_line( token );   
        } else if( STR_EQ( lexeme, ctx->ident_error) ) {
            // #error
            // TODO: implement
            return skip_to_next_line( token );   
        } else if( STR_EQ( lexeme, ctx->ident_pragma) ) {
            // #pragma
            // the compiler does not use any #pragma directives yet, so just ignore rest of line
            return skip_to_next_line( token );   
        } else {
            emit_error( ctx, pos, "invalid preprocessor command '%s'", CSTR( lexeme ) );
            return skip_to_next_line( token );   
        }
    } else {        
        PREPROC_STR lexeme = token_lexeme( token );
        emit_error( ctx, token_pos( token ), "expected preprocessor directive, found '%s'", CSTR( lexeme ) );
        return skip_to_next_line( token );   
    }
}


token_t find_argument( token_t args, int index ) {
    int current_index = 0;
    token_t arg = args;
    while( current_index < index ) {
        if( token_type( arg ) == PREPROC_TOKEN_EOS ) {
            ++current_index;
        }
        arg = token_next_ignore_eos( arg);
    }
    return arg;
}


static token_t parse_macro_args( context_t* ctx, token_t token, token_t* out_args, int* out_arg_count, 
    int parent_args_count, token_t parent_args ) {

    char const* pos = token_pos( token );
    token_t args = get_args_tokens( ctx );
    int arg_count = 0;
    int paren_match = 0;
    int something_added = 0;
    while( !( token_type( token ) == PREPROC_TOKEN_OPERATOR && STR_EQ( token_lexeme( token ), ctx->ident_rparen ) 
        && paren_match == 0 ) ) {
        
        if( token_type( token ) == PREPROC_TOKEN_EOS ) {
            emit_error( ctx, token_pos( token ), "unexpected end of file in macro expansion" );
            return token;
        } else if( token_type( token ) == PREPROC_TOKEN_OPERATOR && STR_EQ( token_lexeme( token ), ctx->ident_comma ) 
            && paren_match == 0 ) {

            ++arg_count;
            something_added = 1;
            add_args_token_eos( ctx, pos );
            token = token_next( token );
        } else if( token_type( token ) == PREPROC_TOKEN_OPERATOR && STR_EQ( token_lexeme( token ), ctx->ident_lparen ) ) {
            ++paren_match;
            add_args_token( ctx, get_preproc_token( token ) );            
            something_added = 1;
            token = token_next( token );
        } else if( token_type( token ) == PREPROC_TOKEN_OPERATOR && STR_EQ( token_lexeme( token ), ctx->ident_rparen ) ) {
            --paren_match;
            add_args_token( ctx, get_preproc_token( token ) );            
            something_added = 1;
            token = token_next( token );
        } else if( token_type( token ) == TOKEN_MACRO_ARG ) {
            int arg_index = token_arg_index( token );
            if( arg_index < parent_args_count ) {
                token_t arg = find_argument( parent_args, arg_index );
                while( token_type( arg ) != PREPROC_TOKEN_EOS ) {
                    size_t arg_pos = ctx->args_tokens_count;
                    size_t output_pos = ctx->output_count;
                    if( try_func_like_macro_no_parent_args( ctx, &arg ) ) {
                        ctx->args_tokens_count = arg_pos;
                        for( size_t i = output_pos; i < ctx->output_count; ++i ) {
                            add_args_token( ctx, ctx->output[ i ] );
                        }
                        ctx->output_count = output_pos;
                        continue;
                    }
                    ctx->args_tokens_count = arg_pos;
                    
                    if( try_obj_like_macro( ctx, &arg ) ) {
                        ctx->args_tokens_count = arg_pos;
                        for( size_t i = output_pos; i < ctx->output_count; ++i ) {
                            add_args_token( ctx, ctx->output[ i ] );
                        }
                        ctx->output_count = output_pos;
                        continue;
                    }           
                    ctx->args_tokens_count = arg_pos;

                    add_args_token( ctx, get_preproc_token( arg ) );
                    arg = token_next( arg );
                }
            }
            something_added = 1;
            token = token_next( token );
        } else {
            add_args_token( ctx, get_preproc_token( token ) );            
            if( token_type( token ) != PREPROC_TOKEN_WHITESPACE && token_type( token ) != PREPROC_TOKEN_NEWLINE ) {
                something_added = 1;
            }
            token = token_next( token );
        }
    }
    token = token_next( token ); // Skip rparen

    if( something_added ) {
        ++arg_count;
    }

    add_args_token_eos( ctx, pos );

    if( out_args ) {
        *out_args = args;
    }

    if( out_arg_count ) {
        *out_arg_count = arg_count;
    }

    return token;
}


static int try_stringify( context_t* ctx, token_t* tokens, token_t args, int arg_count, char const* pos ) {
    token_t token = *tokens;

    if( token_type( token ) != PREPROC_TOKEN_OPERATOR || !STR_EQ( token_lexeme( token ), ctx->ident_hash ) ) {
        return 0;
    }

    token = token_next( token );
    token = skip_whitespace( token );

    if( token_type( token ) != TOKEN_MACRO_ARG ) {
        return 0;
    }

    int arg_index = token_arg_index( token );
    token = token_next( token );
    if( arg_index < arg_count ) {
        char accumulation[ 256 ] = ""; // TODO: use dynamically allocated buffer in context_t
        int ignore_whitespace = 1;
        token_t arg = find_argument( args, arg_index );
        while( token_type( skip_whitespace_newline( arg ) ) != PREPROC_TOKEN_EOS ) {
            preproc_token_type_t type = token_type( arg );
            if( type == PREPROC_TOKEN_NEWLINE || type == PREPROC_TOKEN_WHITESPACE ) {
                if( !ignore_whitespace ) {
                    strcat( accumulation, " " );
                    ignore_whitespace = 1;
                }
            } else {
                char const* str = CSTR( token_lexeme( arg ) );
                if( type == PREPROC_TOKEN_STRING_LITERAL ) {
                    strcat( accumulation, "\"" );
                    strcat( accumulation, str );
                    strcat( accumulation, "\"" );
                    ignore_whitespace = 0;
                } else if( type == PREPROC_TOKEN_CHARACTER_CONSTANT ) {
                    strcat( accumulation, "'" );
                    strcat( accumulation, str );
                    strcat( accumulation, "'" );
                    ignore_whitespace = 0;
                } else {
                    strcat( accumulation, str ? str : "" );
                    ignore_whitespace = 0;
                }
            }

            arg = token_next( arg );
        }
        preproc_token_t string_token;
        string_token.type = PREPROC_TOKEN_STRING_LITERAL;
        string_token.lexeme = PREPROC_STR_ALLOC( ctx->strings, accumulation, strlen( accumulation ) );
        string_token.pos = pos;
        token = token_next( token );
        output_token( ctx, string_token );
    }
    
    *tokens = token;
    return 1;
}


static int try_macro_argument( context_t* ctx, token_t* tokens, token_t args, int arg_count ) {
    token_t token = *tokens;

    if( token_type( token ) != TOKEN_MACRO_ARG ) {
        return 0;
    }

    int arg_index = token_arg_index( token );
    token = token_next( token );

    if( arg_index < arg_count ) {
        token_t arg = find_argument( args, arg_index );
        while( token_type( arg ) != PREPROC_TOKEN_EOS ) {
            if( try_func_like_macro_no_parent_args( ctx, &arg ) ) {
                continue;
            }
            if( try_obj_like_macro( ctx, &arg ) ) {
                continue;
            }           
            output_token( ctx, get_preproc_token( arg ) );
            arg = token_next( arg );
        }
    }

    *tokens = token;
    return 1;
}


static int try_func_like_macro( context_t* ctx, token_t* tokens, int parent_args_count, token_t parent_args ) {
    token_t token = *tokens;

    // check if token is identifier
    if( token_type( token ) != PREPROC_TOKEN_IDENTIFIER ) {
        return 0;
    }

    // check if identifier is a macro name and that macro is a func-like macro
    macro_def_t* macro_def = lookup_macro_def( ctx, token_lexeme( token ) );
    if( !macro_def || !macro_def->is_func_like ) {
        return 0;
    }

    // if we are already in the process of a macro expansion for this macro, don't expand it further
    if( macro_def->is_being_expanded ) {
        return 0;
    }

    token = token_next( token ); // skip macro identifier
    token = skip_whitespace( token ); // skip whitespace, if any

    // if it is not followed by a left parentheses, it is not a func-like macro invokation, just a regular identifier
    if( token_type( token ) != PREPROC_TOKEN_OPERATOR || !STR_EQ( token_lexeme( token ), ctx->ident_lparen ) ) {
        return 0;
    }

    token = token_next( token ); // skip left paren

    // parse argument list
    int arg_count = 0;
    token_t args = { NULL, 0 };
    token = parse_macro_args( ctx, token, &args, &arg_count, parent_args_count, parent_args );

    // make sure argument counts match
    if( arg_count < macro_def->arg_count ) {
        emit_warning( ctx, token_pos( *tokens ), "not enough arguments for function-like macro invocation '%s'", 
            CSTR( macro_def->name ) );
    }
    if( arg_count > macro_def->arg_count ) {
        emit_warning( ctx, token_pos( *tokens ), "too many arguments for function-like macro invocation '%s'", 
            CSTR( macro_def->name ) );
    }

    // expand replacement list
    token_t replacement = macro_def->replacement_list;
    while( token_type( replacement ) != PREPROC_TOKEN_EOS ) {
        // stringify
        if( try_stringify( ctx, &replacement, args, arg_count, token_pos( *tokens ) ) ) {
            continue;
        }

        // func-like macro
        ++macro_def->is_being_expanded;
        int func_like = try_func_like_macro( ctx, &replacement, arg_count, args );
        --macro_def->is_being_expanded;
        if( func_like ) {
            continue;
        }

        // obj-like macro
        ++macro_def->is_being_expanded;
        int obj_like = try_obj_like_macro( ctx, &replacement );
        --macro_def->is_being_expanded;
        if( obj_like ) {
            continue;
        }
        
        // macro argument
        if( try_macro_argument( ctx, &replacement, args, arg_count ) ) {
            continue;
        }

        output_token( ctx, get_preproc_token( replacement ) );
        replacement = token_next( replacement );
    }

    *tokens = token;
    return 1;
}


static int try_func_like_macro_no_parent_args( context_t* ctx, token_t* tokens ) {
    preproc_token_t dummy;
    dummy.type = PREPROC_TOKEN_EOS;
    dummy.lexeme = NULL_STR;
    dummy.pos = token_pos( *tokens );
    preproc_token_t* dummy_ptr = &dummy;
    token_t dummy_args = make_token( &dummy_ptr, 0 );

    return try_func_like_macro( ctx, tokens, 0, dummy_args );
}


static int try_obj_like_macro( context_t* ctx, token_t* tokens ) {
    token_t token = *tokens;

    // check if token is identifier
    if( token_type( token ) != PREPROC_TOKEN_IDENTIFIER ) {
        return 0;
    }

    // check if identifier is a macro name and that macro is an obj-like macro
    macro_def_t* macro_def = lookup_macro_def( ctx, token_lexeme( token ) );
    if( !macro_def || macro_def->is_func_like ) {
        return 0;
    }

    // if we are already in the process of a macro expansion for this macro, don't expand it further
    if( macro_def->is_being_expanded ) {
        return 0;
    }

    token = token_next( token ); // skip macro identifier

    token_t replacement = macro_def->replacement_list;
    while( token_type( replacement ) != PREPROC_TOKEN_EOS ) {
        // func-like macro
        ++macro_def->is_being_expanded;
        int func_like = try_func_like_macro_no_parent_args( ctx, &replacement );
        --macro_def->is_being_expanded;
        if( func_like ) {
            continue;
        }

        // obj-like macro
        ++macro_def->is_being_expanded;
        int obj_like = try_obj_like_macro( ctx, &replacement );
        --macro_def->is_being_expanded;
        if( obj_like ) {
            continue;
        }
       
        output_token( ctx, get_preproc_token( replacement ) );
        replacement = token_next( replacement );
    }

    *tokens = token;
    return 1;
}


static int try_if_section_or_control_line( context_t* ctx, token_t* tokens ) {
    token_t token = *tokens;
    
    token = skip_whitespace( token ); // skip leading whitespace

    // check if it starts with a # symbol
    if( token_type( token ) != PREPROC_TOKEN_OPERATOR || !STR_EQ( token_lexeme( token ), ctx->ident_hash ) ) {
        return 0; // not an if section or control line
    }
    token = token_next( token ); // skip '#' symbol
    token = process_if_section_or_control_line( ctx, token ); // process the line
    
    *tokens = token; 
    return 1;
}


static token_t process_stream( context_t* ctx, token_t tokens ) {
    token_t token = tokens;
    int new_line = 1;
    while( token_type( token ) != PREPROC_TOKEN_EOS ) {
        // if section or control line
        if( new_line && try_if_section_or_control_line( ctx, &token ) ) {
            continue;
        }

        // newline
        if( token_type( token ) == PREPROC_TOKEN_NEWLINE ) {
            output_token( ctx, get_preproc_token( token ) );
            token = token_next( token );
            new_line = 1;            
            continue;
        }
        new_line = 0;

        // func-like macro
        if( try_func_like_macro_no_parent_args( ctx, &token ) ) {
            continue;
        }

        // obj-like macro
        if( try_obj_like_macro( ctx, &token ) ) {
            continue;
        }

        ctx->args_tokens_count = 0; // TODO: handle inside try_func_like_macro, only assert that it is zero here

        // none of the above, so just output token as is
        output_token( ctx, get_preproc_token( token ) );

        // update warnings count (if token was a warning)
        if( token_type( token ) == PREPROC_TOKEN_WARNING ) {
            ++ctx->warnings_count;
        }
        
        // update errors count (if token was an error)
        if( token_type( token ) == PREPROC_TOKEN_ERROR ) {
            ++ctx->errors_count;
        }

        token = token_next( token ); // skip the added token
    }

    return token;
}


preproc_token_t* preproc_process( preproc_token_t* source_tokens, strpool_t* strings, preproc_load_file_t load_func, 
    void* load_context, int* warnings_count, int* errors_count ) {

    // ctx context
    context_t context;
    context_t* ctx = &context;
    memset( ctx, 0, sizeof( *ctx ) );
    ctx->strings = strings;
    ctx->load_func = load_func;
    ctx->load_context = load_context;
    ctx->warnings_count = 0;
    ctx->errors_count = 0;
    
    // predefine identifiers
    ctx->ident_hash = add_string( strings, "#", 0 );
    ctx->ident_lparen = add_string( strings, "(", 0 );
    ctx->ident_rparen = add_string( strings, ")", 0 );
    ctx->ident_comma = add_string( strings, ",", 0 );
    ctx->ident_if = add_string( strings, "if", 0 );
    ctx->ident_ifdef = add_string( strings, "ifdef", 0 );
    ctx->ident_ifndef = add_string( strings, "ifndef", 0 );
    ctx->ident_elif = add_string( strings, "elif", 0 );
    ctx->ident_else = add_string( strings, "else", 0 );
    ctx->ident_endif = add_string( strings, "endif", 0 );
    ctx->ident_include = add_string( strings, "include", 0 );
    ctx->ident_define = add_string( strings, "define", 0 );
    ctx->ident_undef = add_string( strings, "undef", 0 );
    ctx->ident_line = add_string( strings, "line", 0 );
    ctx->ident_error = add_string( strings, "error", 0 );
    ctx->ident_pragma = add_string( strings, "pragma", 0 );
    ctx->ident_defined = add_string( strings, "defined", 0 );

    preproc_token_t* output = NULL;
    if( setjmp( ctx->exit_jump ) == 0 ) {
        // allocate memory for macro defs
        ctx->macro_defs_count = 0;
        ctx->macro_defs_capacity = 256;
        ctx->macro_defs = (macro_def_t*) malloc( ctx->macro_defs_capacity * sizeof( macro_def_t ) );
        if( !ctx->macro_defs ) {        
            ALLOC_FAIL_EXIT( "Failed to allocate initial macro defs array", tokens->pos ); 
        }

        // allocate memory for condition stack
        ctx->condition_stack_count = 0;
        ctx->condition_stack_capacity = 256;
        ctx->condition_stack = (int*) malloc( ctx->condition_stack_capacity * sizeof( int ) );
        if( !ctx->condition_stack ) {        
            ALLOC_FAIL_EXIT( "Failed to allocate initial condition stack array", token->pos ); 
        }

        // allocate memory for macro tokens
        ctx->macro_tokens_count = 0;
        ctx->macro_tokens_capacity = 256;
        ctx->macro_tokens = (preproc_token_t*) malloc( ctx->macro_tokens_capacity * sizeof( preproc_token_t ) );
        if( !ctx->macro_tokens ) {        
            ALLOC_FAIL_EXIT( "Failed to allocate initial macro tokens array", tokens->pos ); 
        }

        // allocate memory for args tokens
        ctx->args_tokens_count = 0;
        ctx->args_tokens_capacity = 256;
        ctx->args_tokens = (preproc_token_t*) malloc( ctx->args_tokens_capacity * sizeof( preproc_token_t ) );
        if( !ctx->args_tokens ) {        
            ALLOC_FAIL_EXIT( "Failed to allocate initial args tokens array", tokens->pos ); 
        }

        // allocate memory for output stream
        ctx->output_count = 0;
        ctx->output_capacity = 4096;
        ctx->output = (preproc_token_t*) malloc( ctx->output_capacity * sizeof( preproc_token_t ) );
        if( !ctx->output ) {
            ALLOC_FAIL_EXIT( "Failed to allocate initial output array", tokens->pos ); 
        }

        // process token stream
        token_t tokens = make_token( &source_tokens, 0 );
        token_t token = process_stream( ctx, tokens );

        // add end-of-stream token
        output_token( ctx, get_preproc_token( token ) );

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
        alloc_fail_output[ 0 ].type = PREPROC_TOKEN_ERROR;
        alloc_fail_output[ 0 ].lexeme = ctx->alloc_error;
        alloc_fail_output[ 0 ].pos = ctx->alloc_error_pos;
        alloc_fail_output[ 1 ].type = PREPROC_TOKEN_EOS;
        alloc_fail_output[ 1 ].lexeme = NULL_STR;
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
    if( tokens != alloc_fail_output ) {
        free( tokens );
    }
}


char* preproc_generate_source_code( preproc_token_t* tokens ) {
    (void) tokens;
    // TODO: Implement
    return NULL;
}


void preproc_free_source_code( char* source_code ) {
    (void) source_code;
    // TODO: Implement
}


#endif /* PREPROC_IMPLEMENTATION */
