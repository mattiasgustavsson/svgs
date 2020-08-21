#define _CRT_NONSTDC_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include "strpool.h"
#include "preproc.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


// debug function to print each token in the stream of preproc tokens
void dbg_print_preproc_tokens( preproc_token_t* tokens, strpool_t* strings ) {
    // list of token type names, in same order as preproc_token_type_t enum
    char const* types[] = { "EOS", "NEWLINE", "WHITESPACE", "HEADER_NAME", "IDENTIFIER", "PP_NUMBER", 
        "CHARACTER_CONSTANT", "STRING_LITERAL", "OPERATOR", "NON_WHITESPACE", "WARNING", "ERROR", };

    for( preproc_token_t* t = tokens; t->type != PREPROC_TOKEN_EOS; ++t ) {
        printf( "%s ",types[ t->type ] );
        if( t->type > PREPROC_TOKEN_WHITESPACE ) {
            char const* str = strpool_cstr( strings, t->lexeme );
            if( t->type == PREPROC_TOKEN_STRING_LITERAL ) {
                printf( "\"%s\"", str );
            } else if( t->type == PREPROC_TOKEN_CHARACTER_CONSTANT ) {
                printf( "'%s'", str );
            } else {
                printf( "%s", str );
            }
        }
        printf( "\n" );
    }    
}


// debug function to generate C source code from a stream of preproc tokens
// comments will be replaced by whitespace, and adjacent whitespace will be merged, as will adjacent newlines
void dbg_c_from_preproc_tokens( char const* filename, preproc_token_t* tokens, strpool_t* strings ) {
    FILE* fp = fopen( filename, "w" );
    for( preproc_token_t* t = tokens; t->type != PREPROC_TOKEN_EOS; ++t ) {
        if( t->type == PREPROC_TOKEN_NEWLINE ) {
            fprintf( fp, "\n" );
        } else if( t->type == PREPROC_TOKEN_WHITESPACE ) {
            fprintf( fp, " " );
        } else {
            char const* str = strpool_cstr( strings, t->lexeme );
            if( t->type == PREPROC_TOKEN_ERROR ) {
                fprintf( fp, "\n#error %s\n\n", str );
            } else if( t->type == PREPROC_TOKEN_WARNING ) {
                fprintf( fp, "\n#pragma message(\"%s\")\n\n", str );
            } else if( t->type == PREPROC_TOKEN_STRING_LITERAL ) {
                fprintf( fp, "\"%s\"", str );
            } else if( t->type == PREPROC_TOKEN_CHARACTER_CONSTANT ) {
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
char* load_source( char const* filename, size_t* length ) {
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
    if( length ) {
        *length = sz;
    }
    return source;
}


typedef struct files_t {
    int count;
    int capacity;
    struct {
        STRPOOL_U32 header_name;
        char* source;
        char const* source_end;
    } *files;
} files_t;


void files_init( files_t* files ) {
    files->count = 0;
    files->capacity = 256;
    files->files = malloc( files->capacity * sizeof( *files->files ) );
}


void files_term( files_t* files ) {
    for( int i = 0; i < files->count; ++i ) {
        free( files->files[ i ].source );
    }
    free( files->files );
}


char const* files_load( files_t* files, STRPOOL_U32 header_name, strpool_t* strings ) {
    if( !header_name ) {
        return NULL;
    }

    for( int i = 0; i < files->count; ++i ) {
        if( header_name == files->files[ i ].header_name ) {
            return files->files[ i ].source;
        }
    }
    
    char const* header_name_cstr = strpool_cstr( strings, header_name );
    if( !header_name_cstr ) {
        return NULL;
    }

    int len = strpool_length( strings, header_name );    
    char* filename = strdup( header_name_cstr );
    filename[ len - 1 ] = '\0'; // remove trailing " or >

    char* source = NULL;
    size_t length = 0;
    if( *filename == '"' ) {
        // load from <location>
        source = load_source( filename + 1, &length ); // +1 to skip leading "
    } else if( *filename == '<' ) {
        // load from "location"
        source = load_source( filename + 1, &length ); // +1 to skip leading <
    }
    free( filename );

    if( source ) {
        if( files->count >= files->capacity ) {
            files->capacity *= 2;
            files->files = realloc( files->files, files->capacity * sizeof( *files->files ) );
        }
        files->files[ files->count ].header_name = header_name;
        files->files[ files->count ].source = source;
        files->files[ files->count ].source_end = (const char*)( ( (uintptr_t)source ) + length );
        ++files->count;
    }

    return source;
}

char const* load_header( void* load_context, STRPOOL_U32 header_name, strpool_t* strings ) {
    return files_load( (files_t*) load_context, header_name, strings );
}


typedef struct pos_info_t {
    STRPOOL_U32 filename;
    int line;
    int column;
} pos_info_t;



pos_info_t get_pos_info( files_t* files, char const* pos ) {
    char const* source = NULL;
    char const* source_end = NULL;
    STRPOOL_U32 filename = 0;
    for( int i = 0; i < files->count; ++i ) {
        if( pos >= files->files[ i ].source && pos <= files->files[ i ].source_end ) {
            source = files->files[ i ].source;
            source_end = files->files[ i ].source_end;
            filename = files->files[ i ].header_name;
        }
    }

    int line = 1;
    int column = 1;
    while( *source != '\0' && source < pos && source < source_end ) {
        if( *source++ == '\n' ) {
            ++line;
            column = 0;
        }
        ++column;
    }
    pos_info_t pos_info;
    pos_info.filename = filename;
    pos_info.line = line;
    pos_info.column = column;
    return pos_info;
}



int main( int argc, char* argv[] ) {
    if( argc <= 1 ) {
        printf( "USAGE:\n    cc main.c\n" );
        return EXIT_FAILURE;
    }

    // configure string pool to not use the counter bits (we don't recycle string handles in this code)
    strpool_config_t config = strpool_default_config;
    config.counter_bits = 0;
    config.index_bits = 32;

    // initialize a string pool for the preproc - used for more efficient storage and comparison of strings
    strpool_t preproc_strings;
    strpool_init( &preproc_strings, &config );

    // source filename
    STRPOOL_U32 source_filename = 0;
    size_t len = strlen( argv[ 1 ] );
    if( argv[ 1 ][ 0 ] == '"' && argv[ 1 ][ len - 1 ] == '"' ) {
        source_filename = (STRPOOL_U32) strpool_inject( &preproc_strings, argv[ 1 ], (int) len );
    } else {
        char* temp_buffer = (char*) malloc( sizeof( char ) * ( len + 3 ) ); // extra space for quotes and terminator
        temp_buffer[ 0 ] = '"';
        memcpy( temp_buffer + 1, argv[ 1 ], len );
        temp_buffer[ len + 1 ] = '"';
        temp_buffer[ len + 2 ] = '\0';
        source_filename = (STRPOOL_U32) strpool_inject( &preproc_strings, temp_buffer, (int) len + 2 );
        free( temp_buffer );
    }

    // init files store
    files_t files;
    files_init( &files);

    // load main source file
    char const* source = files_load( &files, source_filename, &preproc_strings );
    if( !source ) {
        printf( "Failed to read %s\n", strpool_cstr( &preproc_strings, source_filename ) );
        return EXIT_FAILURE;
    }

    // generate stream of preproc tokens for source file
    int warnings_count = 0;
    int errors_count = 0;
    preproc_token_t* preproc_tokens = preproc_tokenize( source, &preproc_strings, &warnings_count, &errors_count );

    // print the results
    //dbg_print_preproc_tokens( preproc_tokens, &preproc_strings );
    //dbg_c_from_preproc_tokens( "out.c", preproc_tokens, &preproc_strings );

    // if there were errors, print each one and their location
    if( errors_count > 0 ) {
        printf( "\npreproc_tokenize FAILED\n" );
    } else {
        printf( "\npreproc_tokenize SUCCESSFUL\n" );
    }

    preproc_token_t* processed_tokens = preproc_process( preproc_tokens, &preproc_strings, 
        load_header, &files, &warnings_count, &errors_count );   

//    dbg_print_preproc_tokens( processed_tokens, &preproc_strings );
    dbg_c_from_preproc_tokens( "out.c", processed_tokens, &preproc_strings );

    // if there were errors or warnings, print each one and their location
    //if( warnings_count > 0 || errors_count > 0 ) {
        for( preproc_token_t* t = processed_tokens; t->type != PREPROC_TOKEN_EOS; ++t ) {
            if( t->type == PREPROC_TOKEN_ERROR ) {
                pos_info_t pos_info = get_pos_info( &files, t->pos );
                char* filename = strdup( strpool_cstr( &preproc_strings, pos_info.filename ) );
                filename[ strlen( filename ) - 1 ] = '\0'; // remove trailing " or >
                char const* message = strpool_cstr( &preproc_strings, t->lexeme );
                printf( "%s(%d,%d): error: %s\n", filename + 1, pos_info.line, pos_info.column, message );
                free( filename );
            } else if( t->type == PREPROC_TOKEN_WARNING ) {
                pos_info_t pos_info = get_pos_info( &files, t->pos );
                char* filename = strdup( strpool_cstr( &preproc_strings, pos_info.filename ) );
                filename[ strlen( filename ) - 1 ] = '\0'; // remove trailing " or >
                char const* message = strpool_cstr( &preproc_strings, t->lexeme );
                printf( "%s(%d,%d): warning: %s\n", filename + 1, pos_info.line, pos_info.column, message );
                free( filename );
            }
        }
        printf( "\n" );
    //} else {
    //    printf( "\nSUCCESSFUL\n" );
    //}

    // cleanup
    preproc_free_tokens( processed_tokens );
    preproc_free_tokens( preproc_tokens );
    files_term( &files );
    return EXIT_SUCCESS;
}


// Implementations

#define STRPOOL_IMPLEMENTATION
#include "strpool.h"

#define PREPROC_IMPLEMENTATION
#include "preproc.h"
