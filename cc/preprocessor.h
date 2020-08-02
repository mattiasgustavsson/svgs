#ifndef preprocessor_h
#define preprocessor_h

#include "strpool.h"

typedef enum preprocessor_token_type_t {
    PREPROCESSOR_TOKEN_EOS,
    PREPROCESSOR_TOKEN_NEWLINE,
    PREPROCESSOR_TOKEN_WHITESPACE,
    PREPROCESSOR_TOKEN_HEADER_NAME,
    PREPROCESSOR_TOKEN_IDENTIFIER,
    PREPROCESSOR_TOKEN_PP_NUMBER,
    PREPROCESSOR_TOKEN_CHARACTER_CONSTANT,
    PREPROCESSOR_TOKEN_STRING_LITERAL,
    PREPROCESSOR_TOKEN_OPERATOR_PUNCTUATOR,
    PREPROCESSOR_TOKEN_NON_WHITESPACE,
    PREPROCESSOR_TOKEN_ERROR,
} preprocessor_token_type_t;


typedef struct preprocessor_token_t {
    preprocessor_token_type_t type; 
    STRPOOL_U32 lexeme; // will be 0 (empty string) for token types EOS, NEWLINE and WHITESPACE
    char const* pos; // pointer to start location (in source data) of this token
} preprocessor_token_t;


// returns a stream of tokens, the last token indicated by type PREPROCESSOR_TOKEN_EOS. 'source' is expected to be
// a zero-terminated string. 'string_pool' is assumed to be initialized already. the optional param 'errors_count'
// will be assigned the total number of errors encountered. information on individual numbers can be find by scanning
// the returned token stream for tokens of type PREPROCESSOR_TOKEN_ERROR. the token stream returned should be released
// by calling free
preprocessor_token_t* preprocessor_tokenize( char const* source, strpool_t* string_pool, int* errors_count );


#endif /* preprocessor_h */

/*
----------------------
    IMPLEMENTATION
----------------------
*/

#ifdef PREPROCESSOR_IMPLEMENTATION
#undef PREPROCESSOR_IMPLEMENTATION

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

// context used during tokenization process
typedef struct preprocessor_tokenizer_t {
    char const* source; // source data passed to preprocessor_tokenize
    char current_char; // last character that was read but have not yet been used
    char const* current_char_pos; // position in source data of current last character read
    int errors_count; // total number of errors encountered
    strpool_t* string_pool; // the string pool passed to preprocessor_tokenize
    char* accumulation_buffer; // temp char buffer used for building strings character by character. resized as needed
    size_t accumulation_buffer_capacity; // current capacity of accumulation buffer
} preprocessor_tokenizer_t;


// returns the next char, after merging lines ending in the '\' character
static char preprocessor_get_char( preprocessor_tokenizer_t* preprocessor ) {
    preprocessor->current_char_pos = preprocessor->source;
    char c = *preprocessor->source;
    if( c ) {
        ++preprocessor->source;
    }
    while( c == '\\' && *preprocessor->source == '\n' ) {
        ++preprocessor->source;
        c = *preprocessor->source;
        if( c ) {
            ++preprocessor->source;
        }
    }
    return c;
}


// returns the next char, but does not step forward - just a look-ahead
static char preprocessor_peek_char( preprocessor_tokenizer_t* preprocessor ) {
    char const* source = preprocessor->source;
    char const* current_char_pos = preprocessor->current_char_pos;
    char c = preprocessor_get_char( preprocessor );
    preprocessor->source = source;
    preprocessor->current_char_pos = current_char_pos;
    return c;
}


// helper function for building strings. will resize the accumulation buffer as needed
static size_t preprocessor_accumulate( preprocessor_tokenizer_t* preprocessor, size_t pos, char c ) {
    if( pos >= preprocessor->accumulation_buffer_capacity ) {
        preprocessor->accumulation_buffer_capacity *= 2;
        preprocessor->accumulation_buffer = (char*) realloc( preprocessor->accumulation_buffer, 
            sizeof( char) * preprocessor->accumulation_buffer_capacity );
        assert( preprocessor->accumulation_buffer ); // TODO: handle allocation fail
    }
    preprocessor->accumulation_buffer[ pos ] = c;
    return pos + 1;
}


// helper function to add a string to the string pool (really just to not have to do the necessary casts everywhere)
static STRPOOL_U32 preprocessor_add_string( preprocessor_tokenizer_t* preprocessor, char const* str, size_t len ) {
    return (STRPOOL_U32) strpool_inject( preprocessor->string_pool, str, (int)len );
}


// helper function to add an errror message to the string pool, and also increase the error count
static STRPOOL_U32 preprocessor_error( preprocessor_tokenizer_t* preprocessor, char const* message ) {
    ++preprocessor->errors_count;
    return preprocessor_add_string( preprocessor, message, strlen( message ) );
}


// list of all operators and punctuators, ordered by length
static char const* preprocessor_operators_punctuators[] = {
    //"sizeof", // handled as a special case when tokenizing identifiers
    "...","<<=",">>=",
    "<<",">>","<=",">=","==","!=","*=","/=","%=","+=","-=","&=","^=","|=","->","++","--","&&","||","##",
    "[","]","(",")",".","&","*","+","-","~","!","/","%","<",">","^","|","?",":","=",",","#","{","}",";",
};


// by using only two characters, determine if it matches the first two chars of any of the operators/punctuators
// returns NULL if there is no match. if a match is found, we can read one more char and call the other helper
// function 'is_operator_punctuator_three_char' to check if it match one of the three-char operators/punctuators
static char const* is_operator_punctuator_two_char( char a, char b ) {
    int const count = sizeof( preprocessor_operators_punctuators ) / sizeof( *preprocessor_operators_punctuators );
    for( int i = 0; i < count; ++i ) {
        if( preprocessor_operators_punctuators[ i ][ 0 ] == a ) {
            if( preprocessor_operators_punctuators[ i ][ 1 ] == '\0' ) {
                return preprocessor_operators_punctuators[ i ];
            } else if( preprocessor_operators_punctuators[ i ][ 1 ] == b ) {
                return preprocessor_operators_punctuators[ i ];
            }           
        }
    }
    return NULL;
}


// by using three characters, determine the exact match to one of the operators/punctuators. returns NULL if no match
static char const* is_operator_punctuator_three_char( char a, char b, char c ) {
    int const count = sizeof( preprocessor_operators_punctuators ) / sizeof( *preprocessor_operators_punctuators );
    for( int i = 0; i < count; ++i ) {

        if( preprocessor_operators_punctuators[ i ][ 0 ] == a ) {
            if( preprocessor_operators_punctuators[ i ][ 1 ] == '\0' ) {
                return preprocessor_operators_punctuators[ i ];
            } else if( preprocessor_operators_punctuators[ i ][ 1 ] == b ) {
                if( preprocessor_operators_punctuators[ i ][ 2 ] == '\0' ) {
                    return preprocessor_operators_punctuators[ i ];
                } else if( preprocessor_operators_punctuators[ i ][ 2 ] == c ) {
                    return preprocessor_operators_punctuators[ i ];
                }
            }           
        }
    }
    return NULL;
}


// starts with the current char and reads more chars as needed until a full token has been scanned, then returns it
// the 'in_include_directive' param should be non-zero if the preceding tokens for the '#include' directive
static preprocessor_token_t preprocessor_next_token( preprocessor_tokenizer_t* preprocessor, int in_include_directive ) {
    // token to return - initial values will be overridden as needed
    preprocessor_token_t token;
    token.type = PREPROCESSOR_TOKEN_ERROR;
    token.lexeme = 0;
    token.pos = preprocessor->current_char_pos;

    // End of stream
    if( preprocessor->current_char == '\0' )  {
        token.type = PREPROCESSOR_TOKEN_EOS;
        return token;
    }
    
    // Whitespace
    if( isspace( (unsigned char) preprocessor->current_char ) && preprocessor->current_char != '\n' ) {
        while( isspace( (unsigned char) preprocessor->current_char ) && preprocessor->current_char != '\n' ) {
            preprocessor->current_char = preprocessor_get_char( preprocessor );
        }
        token.type = PREPROCESSOR_TOKEN_WHITESPACE;    
        return token;
    }

    // Line comment
    if( preprocessor->current_char == '/' && preprocessor_peek_char( preprocessor ) == '/' ) {
        while( preprocessor->current_char && preprocessor->current_char != '\n' )  {
            preprocessor->current_char = preprocessor_get_char( preprocessor );
        }
        token.type = PREPROCESSOR_TOKEN_WHITESPACE;    
        return token;
    }

    // Block comment
    if( preprocessor->current_char == '/' && preprocessor_peek_char( preprocessor ) == '*' ) {
        preprocessor->current_char = preprocessor_get_char( preprocessor );
        preprocessor->current_char = preprocessor_get_char( preprocessor );
        while( !( preprocessor->current_char == '*' && preprocessor_peek_char( preprocessor ) == '/' ) )  {
            if( preprocessor->current_char == '\0' ) {
                token.type = PREPROCESSOR_TOKEN_ERROR;
                token.lexeme = preprocessor_error( preprocessor, "unexpected end of file found in comment" );
                return token;
            }
            preprocessor->current_char = preprocessor_get_char( preprocessor );
        }
        preprocessor->current_char = preprocessor_get_char( preprocessor );
        preprocessor->current_char = preprocessor_get_char( preprocessor );
        token.type = PREPROCESSOR_TOKEN_WHITESPACE;    
        return token;
    }

    // Newline
    if( preprocessor->current_char == '\n' ) {
        preprocessor->current_char = preprocessor_get_char( preprocessor );
        token.type = PREPROCESSOR_TOKEN_NEWLINE;    
        return token;
    }

    // Identifier
    if( isalpha( (unsigned char) preprocessor->current_char ) || preprocessor->current_char == '_' ) { 
        size_t acc_pos = 0;
        while( isalnum( (unsigned char) preprocessor->current_char ) || preprocessor->current_char == '_' ) {
            acc_pos = preprocessor_accumulate( preprocessor, acc_pos, preprocessor->current_char );
            preprocessor->current_char = preprocessor_get_char( preprocessor );
        }
        token.type = PREPROCESSOR_TOKEN_IDENTIFIER;
        if( strncmp( preprocessor->accumulation_buffer, "sizeof", acc_pos ) == 0 ) {
            token.type = PREPROCESSOR_TOKEN_OPERATOR_PUNCTUATOR;
        }
        token.lexeme = preprocessor_add_string( preprocessor, preprocessor->accumulation_buffer, acc_pos );
        return token;
    }

    // Header name
    if( in_include_directive && ( preprocessor->current_char == '<' || preprocessor->current_char == '"' ) ) {
        char terminator = preprocessor->current_char == '<' ? '>' : '"';
        size_t acc_pos = 0;
        acc_pos = preprocessor_accumulate( preprocessor, acc_pos, preprocessor->current_char );
        preprocessor->current_char = preprocessor_get_char( preprocessor );
        while( preprocessor->current_char != terminator ) {
            if( !preprocessor->current_char || preprocessor->current_char == '\n') {
                token.type = PREPROCESSOR_TOKEN_ERROR;
                token.lexeme = preprocessor_error( preprocessor, 
                    terminator == '>' ? "missing '<'" : "newline in constant" );
                return token;
            }
            acc_pos = preprocessor_accumulate( preprocessor, acc_pos, preprocessor->current_char );
            preprocessor->current_char = preprocessor_get_char( preprocessor );
        }
        acc_pos = preprocessor_accumulate( preprocessor, acc_pos, preprocessor->current_char );
        preprocessor->current_char = preprocessor_get_char( preprocessor );
        token.lexeme = preprocessor_add_string( preprocessor, preprocessor->accumulation_buffer, acc_pos );
        token.type = PREPROCESSOR_TOKEN_HEADER_NAME;
        return token;
    }

    // PP Number
    if( isdigit( (unsigned char) preprocessor->current_char ) || ( preprocessor->current_char == '.' 
        && isdigit( (unsigned char) preprocessor_peek_char( preprocessor ) ) ) ) {   

        size_t acc_pos = 0;
        while( isdigit( (unsigned char) preprocessor->current_char ) ) {
            acc_pos = preprocessor_accumulate( preprocessor, acc_pos, preprocessor->current_char );
            preprocessor->current_char = preprocessor_get_char( preprocessor );
        }
        if( preprocessor->current_char == '.' ) {
            acc_pos = preprocessor_accumulate( preprocessor, acc_pos, preprocessor->current_char );
            preprocessor->current_char = preprocessor_get_char( preprocessor );
        }
        while( isdigit( (unsigned char) preprocessor->current_char ) ) {
            acc_pos = preprocessor_accumulate( preprocessor, acc_pos, preprocessor->current_char );
            preprocessor->current_char = preprocessor_get_char( preprocessor );
        }
        if( preprocessor->current_char == 'e' || preprocessor->current_char == 'E' ) {
            acc_pos = preprocessor_accumulate( preprocessor, acc_pos, preprocessor->current_char );
            preprocessor->current_char = preprocessor_get_char( preprocessor );
            if( preprocessor->current_char == '+' || preprocessor->current_char == '-' ) {
                acc_pos = preprocessor_accumulate( preprocessor, acc_pos, preprocessor->current_char );
                preprocessor->current_char = preprocessor_get_char( preprocessor );
            }
            while( isdigit( (unsigned char) preprocessor->current_char ) ) {
                acc_pos = preprocessor_accumulate( preprocessor, acc_pos, preprocessor->current_char );
                preprocessor->current_char = preprocessor_get_char( preprocessor );
            }
        }
        while( isalpha( (unsigned char) preprocessor->current_char ) ) {
            acc_pos = preprocessor_accumulate( preprocessor, acc_pos, preprocessor->current_char );
            preprocessor->current_char = preprocessor_get_char( preprocessor );
        }
        token.lexeme = preprocessor_add_string( preprocessor, preprocessor->accumulation_buffer, acc_pos );
        token.type = PREPROCESSOR_TOKEN_PP_NUMBER;
        return token;
    } 

    // Operators and punctuators
    if( is_operator_punctuator_two_char( preprocessor->current_char, preprocessor_peek_char( preprocessor ) ) ) {
        char prev_char = preprocessor->current_char;
        preprocessor->current_char = preprocessor_get_char( preprocessor );
        char const* str = is_operator_punctuator_three_char( prev_char, preprocessor->current_char, 
            preprocessor_peek_char( preprocessor ) );
        size_t len = strlen( str );
        if( len == 2 ) {
            preprocessor->current_char = preprocessor_get_char( preprocessor );
        } else if( len == 3 ) {
            preprocessor->current_char = preprocessor_get_char( preprocessor );
            preprocessor->current_char = preprocessor_get_char( preprocessor );
        } 
        token.lexeme = preprocessor_add_string( preprocessor, str, len );
        token.type = PREPROCESSOR_TOKEN_OPERATOR_PUNCTUATOR;
        return token;
    }

    // String literals
    // TODO: escape sequences
    if( preprocessor->current_char == '"' ) {   
        preprocessor->current_char = preprocessor_get_char( preprocessor );
        size_t acc_pos = 0;
        while( preprocessor->current_char != '"' ) {
            if( preprocessor->current_char == '\n' ) {
                token.type = PREPROCESSOR_TOKEN_ERROR;
                token.lexeme = preprocessor_error( preprocessor, "newline in constant" );
                return token;
            }
            acc_pos = preprocessor_accumulate( preprocessor, acc_pos, preprocessor->current_char );
            if( preprocessor->current_char == '\\' && preprocessor_peek_char( preprocessor ) == '"' ) {
                preprocessor->current_char = preprocessor_get_char( preprocessor );
                acc_pos = preprocessor_accumulate( preprocessor, acc_pos, preprocessor->current_char );
            }
            preprocessor->current_char = preprocessor_get_char( preprocessor );
        }
        preprocessor->current_char = preprocessor_get_char( preprocessor );

        token.lexeme = preprocessor_add_string( preprocessor, preprocessor->accumulation_buffer, acc_pos );
        token.type = PREPROCESSOR_TOKEN_STRING_LITERAL ;
        return token;
    }

    // Char constants
    // TODO: escape sequences
    if( preprocessor->current_char == '\'' ) {   
        preprocessor->current_char = preprocessor_get_char( preprocessor );
        size_t acc_pos = 0;
        while( preprocessor->current_char != '\'' ) {
            if( preprocessor->current_char == '\n' ) {
                token.type = PREPROCESSOR_TOKEN_ERROR;
                token.lexeme = preprocessor_error( preprocessor, "newline in constant" );
                return token;
            }
            acc_pos = preprocessor_accumulate( preprocessor, acc_pos, preprocessor->current_char );
            if( preprocessor->current_char == '\\' && ( preprocessor_peek_char( preprocessor ) == '\'' 
                || preprocessor_peek_char( preprocessor ) == '\\' ) ) {

                preprocessor->current_char = preprocessor_get_char( preprocessor );
                acc_pos = preprocessor_accumulate( preprocessor, acc_pos, preprocessor->current_char );
            }
            preprocessor->current_char = preprocessor_get_char( preprocessor );
        }
        preprocessor->current_char = preprocessor_get_char( preprocessor );

        token.lexeme = preprocessor_add_string( preprocessor, preprocessor->accumulation_buffer, acc_pos );
        token.type = PREPROCESSOR_TOKEN_CHARACTER_CONSTANT;
        return token;
    }

    // Catchall for any characters not caught by the above - will probably be rejected in later stages
    token.type = PREPROCESSOR_TOKEN_NON_WHITESPACE;
    token.lexeme = preprocessor_add_string( preprocessor, &preprocessor->current_char, 1 );   
    preprocessor->current_char = preprocessor_get_char( preprocessor );
    return token;
}


// to make implementation easier, for some token types (whitespace, newline and non-whitespace) consecutive tokens are 
// collapsed to a single token of that type, and lexemes merged. thuis is a helper function to do that
static size_t preprocessor_merge_tokens( preprocessor_tokenizer_t* preprocessor, preprocessor_token_t* token, 
    preprocessor_token_t const* tokens, size_t count ) {
    
    if( count > 0 ) { // we can't merge if no preceding token exitt
        preprocessor_token_t const* prev_token = &tokens[ count - 1 ];
        if( token->type == prev_token->type ) { // only merge if both tokens are the same tyoe
            // copy lexeme of first token to accumulation buffer
            size_t pos = 0;
            char const* str = strpool_cstr( preprocessor->string_pool, prev_token->lexeme );
            int len = strpool_length( preprocessor->string_pool, prev_token->lexeme );
            for( int i = 0; i < len; ++i ) {
                pos = preprocessor_accumulate( preprocessor, pos, str[ i ] );
            }
            
            // append lexeme of second token to accumulation buffer
            str = strpool_cstr( preprocessor->string_pool, token->lexeme );
            len = strpool_length( preprocessor->string_pool, token->lexeme );
            for( int i = 0; i < len; ++i ) {
                pos = preprocessor_accumulate( preprocessor, pos, str[ i ] );
            }
            token->lexeme = preprocessor_add_string( preprocessor, preprocessor->accumulation_buffer, pos );

            --count; // so when this token gets added after we loop around, it overwrites the one we merged with
        }
    }
    return count;
}


preprocessor_token_t* preprocessor_tokenize( char const* source, strpool_t* string_pool, int* errors_count ) {
    // allocate memory for token stream
    size_t count = 0;
    size_t capacity = 4096;
    preprocessor_token_t* tokens = (preprocessor_token_t*) malloc( capacity * sizeof( preprocessor_token_t ) );
    assert( tokens ); // TODO: handle allocation fail

    // initialize the preprocessor context
    preprocessor_tokenizer_t preprocessor;
    preprocessor.source = source;
    preprocessor.current_char = '\0';
    preprocessor.current_char_pos = source;
    preprocessor.errors_count = 0;
    preprocessor.string_pool = string_pool;
    preprocessor.accumulation_buffer_capacity = 256;
    preprocessor.accumulation_buffer = (char*) malloc( sizeof( char ) * preprocessor.accumulation_buffer_capacity );
    assert( preprocessor.accumulation_buffer ); // TODO: handle allocation fail

    // add to string pool the strings we need to compare against during tokenization
    STRPOOL_U32 identifier_include = preprocessor_add_string( &preprocessor, "include", strlen( "include" ) );  
    STRPOOL_U32 identifier_hash = preprocessor_add_string( &preprocessor, "#", strlen( "#" ) );

    // initialize the current_char field with the first character from source, as preprocessor_next_token expects this
    preprocessor.current_char = preprocessor_get_char( &preprocessor );

    // get the first token before we enter the while loop
    preprocessor_token_t token = preprocessor_next_token( &preprocessor, 0 ); // we know we are not in an #include
    
    // tokenize until end of file
    while( token.type != PREPROCESSOR_TOKEN_EOS ) {
        // reallocate tokens array if needed
        if( count >= capacity - 1 ) { // -1 to ensure room for EOS token at the end
            capacity *= 2;
            tokens = (preprocessor_token_t*) realloc( tokens, capacity * sizeof( preprocessor_token_t ) );            
            assert( tokens ); // TODO: handle allocation fail
        }

        // add token to stream
        tokens[ count++ ] = token; 

        // look a few tokens back in the stream to see if we are in an #include directive (to parse header names)
        int in_include_directive = 0;
        size_t i = count;
        if( i > 0 &&tokens[ i - 1 ].type == PREPROCESSOR_TOKEN_WHITESPACE ) {
            --i; // skip whitespace after 'include', if any
        }
        // check if token is the 'include' identifier
        if( i > 0 && tokens[ i - 1 ].type == PREPROCESSOR_TOKEN_IDENTIFIER 
            && tokens[ i - 1 ].lexeme == identifier_include ) {

            --i; // skip past 'include'
            if( i > 0 && tokens[ i - 1 ].type == PREPROCESSOR_TOKEN_WHITESPACE ) {
                --i; // skip whitespace between '#' and 'include', if any
            }
            // check if token is '#' symbol
            if( i > 0 && tokens[ i - 1 ].type == PREPROCESSOR_TOKEN_OPERATOR_PUNCTUATOR 
                && tokens[ i - 1 ].lexeme == identifier_hash ) {
                
                in_include_directive = 1; // all conditions are met, we are just after an #include directive
            }
        }

        // read next token
        token = preprocessor_next_token( &preprocessor, in_include_directive );

        // merge tokens of type 'whitespace', 'non-whitespace' and 'newline'
        if( token.type == PREPROCESSOR_TOKEN_NON_WHITESPACE || token.type == PREPROCESSOR_TOKEN_WHITESPACE 
            || token.type == PREPROCESSOR_TOKEN_NEWLINE ) {
            count = preprocessor_merge_tokens( &preprocessor, &token, tokens, count );
        }
    }
    
    tokens[ count ] = token; // add end-of-stream token
   
    // return error count
    if( errors_count ) {
        *errors_count = preprocessor.errors_count;
    }

    // return token stream
    return tokens;
}

#endif /* PREPROCESSOR_IMPLEMENTATION */
