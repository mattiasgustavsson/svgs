#define _CRT_NONSTDC_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include "preprocessor.h"
#include "strpool.h"

#include <stdlib.h>
#include <stdio.h>

// counts the number of line break characters from the start of 'source' to 'pos'
int line_number_from_pos( char const* source, char const* pos ) {
    int line = 1;
    while( *source != '\0' && source < pos ) {
        if( *source++ == '\n' ) {
            ++line;
        }
    }
    return line;
}


// debug function to print each token in the stream of preprocessor tokens
void dbg_print_preprocessor_tokens( preprocessor_token_t* tokens, strpool_t* strings ) {
    // list of token type names, in same order as preprocessor_token_type_t enum
    char const* types[] = { "EOS", "NEWLINE", "WHITESPACE", "HEADER_NAME", "IDENTIFIER", "PP_NUMBER", 
        "CHARACTER_CONSTANT", "STRING_LITERAL", "OPERATOR_PUNCTUATOR", "NON_WHITESPACE", "ERROR", };

    for( preprocessor_token_t* t = tokens; t->type != PREPROCESSOR_TOKEN_EOS; ++t ) {
        printf( "%s ",types[ t->type ] );
        if( t->type > PREPROCESSOR_TOKEN_WHITESPACE ) {
            char const* str = strpool_cstr( strings, t->lexeme );
            if( t->type == PREPROCESSOR_TOKEN_STRING_LITERAL ) {
                printf( "\"%s\"", str );
            } else if( t->type == PREPROCESSOR_TOKEN_CHARACTER_CONSTANT ) {
                printf( "'%s'", str );
            } else {
                printf( "%s", str );
            }
        }
        printf( "\n" );
    }    
}


// debug function to generate C source code from a stream of preprocessor tokens
// comments will be replaced by whitespace, and adjacent whitespace will be merged, as will adjacent newlines
void dbg_c_from_preprocessor_tokens( char const* filename, preprocessor_token_t* tokens, strpool_t* strings ) {
    FILE* fp = fopen( filename, "w" );
    for( preprocessor_token_t* t = tokens; t->type != PREPROCESSOR_TOKEN_EOS; ++t ) {
        if( t->type == PREPROCESSOR_TOKEN_NEWLINE ) {
            fprintf( fp, "\n" );
        } else if( t->type == PREPROCESSOR_TOKEN_WHITESPACE ) {
            fprintf( fp, " " );
        } else {
            char const* str = strpool_cstr( strings, t->lexeme );
            if( t->type == PREPROCESSOR_TOKEN_ERROR ) {
                fprintf( fp, "\n#error %s\n", str );
            } else if( t->type == PREPROCESSOR_TOKEN_STRING_LITERAL ) {
                fprintf( fp, "\"%s\"", str );
            } else if( t->type == PREPROCESSOR_TOKEN_CHARACTER_CONSTANT ) {
                fprintf( fp, "'%s'", str );
            } else {
                fprintf( fp, "%s", str );
            }
        }
    }    
    fclose( fp );
}


// loads the specified text file. returns NULL if it can't be read, otherwise a pointer to a zero-terminated 
// string containing the file. this pointer should be released by calling 'free'
char* load_source( char const* filename ) {
    FILE* fp = fopen( filename, "r" );
    if( !fp ) {
        return NULL;
    }
    fseek( fp, 0, SEEK_END );
    size_t sz = ftell( fp );
    fseek( fp, 0, SEEK_SET );
    char* source = (char*) malloc( sz + 1 );
    sz = fread( source, 1, sz, fp );
    source[ sz ] = '\0';
    fclose( fp );
    return source;
}


int main( int argc, char* argv[] ) {
    
    if( argc <= 1 ) {
        printf( "USAGE:\n    cc main.c\n" );
        return EXIT_FAILURE;
    }

    char* source = load_source( argv[ 1 ] );
    if( !source ) {
        printf( "Failed to read %s\n", argv[ 1 ] );
        return EXIT_FAILURE;
    }

    // configure string pool to not use the counter bits (we don't recycle string handles in this code)
    strpool_config_t config = strpool_default_config;
    config.counter_bits = 0;
    config.index_bits = 32;

    // initialize a string pool for the preprocessor - used for more efficient storage and comparison of strings
    strpool_t preprocessor_strings;
    strpool_init( &preprocessor_strings, &config );

    // generate stream of preprocessor tokens for source file
    int errors_count = 0;
    preprocessor_token_t* tokens = preprocessor_tokenize( source, &preprocessor_strings, &errors_count );

    // print the results
    dbg_print_preprocessor_tokens( tokens, &preprocessor_strings );
    dbg_c_from_preprocessor_tokens( "out.c", tokens, &preprocessor_strings );

    // if there were errors, print each one and their location
    if( errors_count > 0 ) {
        printf( "\n%d error(s):\n", errors_count );
        for( preprocessor_token_t* t = tokens; t->type != PREPROCESSOR_TOKEN_EOS; ++t ) {
            if( t->type == PREPROCESSOR_TOKEN_ERROR ) {
                int line = line_number_from_pos( source, t->pos );
                char const* message = strpool_cstr( &preprocessor_strings, t->lexeme );
                printf( "%s(%d): error: %s\n", argv[ 1 ], line, message );
            }
        }
        printf( "\n" );
    } else {
        printf( "\nSUCCESSFUL\n" );
    }

    // cleanup
    free( source );
    free( tokens );
    return EXIT_SUCCESS;
}


// Implementations

#define STRPOOL_IMPLEMENTATION
#include "strpool.h"

#define PREPROCESSOR_IMPLEMENTATION
#include "preprocessor.h"
