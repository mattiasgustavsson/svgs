#ifndef asm_h
#define asm_h

typedef enum asm_error_t {
	ASM_ERROR_NO_ERROR,
	ASM_ERROR_NODATA,
	ASM_ERROR_UNEXPECTEDTOKEN,
	ASM_ERROR_INVALIDOPERANDS,
	ASM_ERROR_COLONEXPECTED,
	ASM_ERROR_UNDEFINEDLABEL,
} asm_error_t;

void* asm_assemble( char const* input, size_t input_size, size_t* output_size, asm_error_t* error, int* error_line );

void asm_free( void* ptr );

#endif /* asm_h */



#ifdef SVGS_IMPLEMENTATION


#ifndef _WIN32 
    #define stricmp strcasecmp
    #define strnicmp strncasecmp

    void strlwr( char* str ) {
        while( *str ) {
            *str = tolower( *str );
            ++str;
        }
    }    
#endif


typedef enum tokentype_t {
	TOKENTYPE_INVALID,
	TOKENTYPE_NEWLINE,
	TOKENTYPE_INTEGER,
	TOKENTYPE_HEXADECIMAL,
	TOKENTYPE_BINARY,
	TOKENTYPE_FLOAT,
	TOKENTYPE_IDENTIFIER,
	TOKENTYPE_COLON,
	TOKENTYPE_SEMICOLON,
	TOKENTYPE_STRING,
	TOKENTYPE_COMMA,
	TOKENTYPE_AT,
	TOKENTYPE_DATA_REGISTER,
	TOKENTYPE_ADDRESS_REGISTER,
	TOKENTYPE_ADDRESS_REGISTER_INC,
	TOKENTYPE_ADDRESS_REGISTER_DEC,
	TOKENTYPE_FLOAT_REGISTER,
	TOKENTYPE_INSTRUCTION,
	TOKENTYPE_STACK_INDEX,
	TOKENTYPE_DEFINE_CONSTANT,
	TOKENTYPE_DEFINE_CONSTANT_BLOCK,
	TOKENTYPE_DEFINE_STORAGE,
	TOKENTYPE_INCLUDE,
	TOKENTYPE_INCBIN,

	TOKENTYPE_END_OF_DATA,
} tokentype_t;


typedef struct label_t {
	char* label;
	uint32_t offset;
} label_t;


typedef struct label_to_resolve_t {
	char* label;
	uint32_t location_to_resolve_to;
	int line;
} label_to_resolve_t;


typedef struct string_array_t {
    char** items;
    int count;
    int capacity;
} string_array_t;


typedef struct label_array_t {
    label_t* items;
    int count;
    int capacity;
} label_array_t;


typedef struct label_to_resolve_array_t {
    label_to_resolve_t* items;
    int count;
    int capacity;
} label_to_resolve_array_t;


typedef struct outbuf_t {
    void* data;
    size_t size;
    size_t capacity;
} outbuf_t;


void string_array_init( string_array_t* array, int initial_capacity ) {
    array->items = (char**) malloc( sizeof( char* ) * initial_capacity );
    array->count = 0;
    array->capacity = initial_capacity;
}


void string_array_term( string_array_t* array ) {
    for( int i = 0; i < array->count; ++i ) {
        free( array->items[ i ] );
    }
    free( array->items );
}


void string_array_add( string_array_t* array, char* string ) {
    if( array->count >= array->capacity ) {
        array->capacity *= 2;
        array->items = (char**) realloc( array->items, sizeof( char* ) * array->capacity );
    }
    array->items[ array->count++ ] = string;
}


void label_array_init( label_array_t* array, int initial_capacity ) {
    array->items = (label_t*) malloc( sizeof( label_t ) * initial_capacity );
    array->count = 0;
    array->capacity = initial_capacity;
}


void label_array_term( label_array_t* array ) {
    for( int i = 0; i < array->count; ++i ) {
        free( array->items[ i ].label );
    }
    free( array->items );
}


void label_array_add( label_array_t* array, label_t label ) {
    if( array->count >= array->capacity ) {
        array->capacity *= 2;
        array->items = (label_t*) realloc( array->items, sizeof( label_t ) * array->capacity );
    }
    array->items[ array->count++ ] = label;
}


void label_to_resolve_array_init( label_to_resolve_array_t* array, int initial_capacity ) {
    array->items = (label_to_resolve_t*) malloc( sizeof( label_to_resolve_t ) * initial_capacity );
    array->count = 0;
    array->capacity = initial_capacity;
}


void label_to_resolve_array_term( label_to_resolve_array_t* array ) {
    for( int i = 0; i < array->count; ++i ) {
        free( array->items[ i ].label );
    }
    free( array->items );
}


void label_to_resolve_array_add( label_to_resolve_array_t* array, label_to_resolve_t label ) {
    if( array->count >= array->capacity ) {
        array->capacity *= 2;
        array->items = (label_to_resolve_t*) realloc( array->items, sizeof( label_to_resolve_t ) * array->capacity );
    }
    array->items[ array->count++ ] = label;
}


void label_to_resolve_array_remove( label_to_resolve_array_t* array, int index ) {
    array->items[ index ] = array->items[ --array->count ];
}


void outbuf_init( outbuf_t* outbuf, size_t initial_capacity ) {
    outbuf->data = malloc( initial_capacity );
    outbuf->size = 0;
    outbuf->capacity = initial_capacity;
}


void outbuf_term( outbuf_t* outbuf ) {
    free( outbuf->data );
}


void outbuf_append( outbuf_t* outbuf, void* data, size_t size ) {
    if( outbuf->size + size > outbuf->capacity ) {
        while( outbuf->size + size > outbuf->capacity ) { 
            outbuf->capacity *= 2;
        }
        outbuf->data = realloc( outbuf->data, outbuf->capacity );
    }
    memcpy( (void*)( ( (uintptr_t) outbuf->data ) + outbuf->size ), data, size );
    outbuf->size += size;
}


static tokentype_t get_next_token( string_array_t* lines, int* lexeme_index_0, int* lexeme_index_1, int* current_line, tokentype_t* current_token, char* current_lexeme );
static bool skip_to_next_line( string_array_t* lines, int* current_line, tokentype_t* current_token, int* lexeme_index_0, int* lexeme_index_1) ;
static cpu_argtype_t parse_operand( string_array_t* lines, int* lexeme_index_0, int* lexeme_index_1, int* current_line, tokentype_t* current_token, char* current_lexeme );
static uint32_t get_operand_value( cpu_argtype_t operand, const char* lexeme );
static uint32_t get_label_offset( label_array_t* labels, const char* label );
static void add_label_to_resolve( label_to_resolve_array_t* labels_to_resolve, const char* label, uint32_t location, int current_line );
static void release_memory( label_array_t* labels, label_to_resolve_array_t* labels_to_resolve, string_array_t* lines, outbuf_t* outbuf );	

static bool is_whitespace( char c );
static bool is_number( char c );
static bool is_identifier( char c );
static bool is_delimiter( char c );
static bool is_identifier_str( const char* str );
static bool is_stack_index( const char* str );
static bool is_integer( const char* str );
static bool is_float( const char* str );
static bool is_binary( const char* str );
static bool is_hexadecimal( const char* str );
static uint32_t integer_from_string( const char* string );
static float float_from_string( const char* string );


typedef struct instruction_t {
	cpu_op_t operation;
	cpu_optype_t type;
	cpu_argtype_t operand_a;
	cpu_argtype_t operand_b;
} instruction_t;


static bool find_instruction( const char* token, cpu_argtype_t operand_a, cpu_argtype_t operand_b, instruction_t* instruction ) {
	if( !token || strlen( token ) == 0 || strlen( token ) > 15 ) {
		return false;
	}

	char mnemonic[ 16 ];
	cpu_optype_t type = CPU_OPTYPE_NONE;
	const char* end = strchr( token, '.');
	if( !end ) {
		strcpy( mnemonic, token );
	} else {
		int l = (int) (end - token);
		strncpy( mnemonic, token, (size_t)l );
		mnemonic[ l ] = 0;
		switch( token[ l + 1 ] ) {
			case 'b': type = CPU_OPTYPE_BYTE; break;
			case 'w': type = CPU_OPTYPE_WORD; break;
			case 'l': type = CPU_OPTYPE_LONG; break;
			case 'f': type = CPU_OPTYPE_FLOAT; break;
		}
	}

	for( int i = 0; i < 256; ++i ) {
		if( !cpu_opinfo_mnemonic( (uint8_t) i ) ) {
			break;
		}
		if( stricmp( mnemonic, cpu_opinfo_mnemonic( (uint8_t) i ) ) == 0
		&& cpu_opinfo_optype( (uint8_t) i, type )
		&& cpu_opinfo_argtype_a( (uint8_t) i, operand_a ) 
		&& cpu_opinfo_argtype_b( (uint8_t) i, operand_b ) )
			{
			instruction->operation = cpu_opinfo_operation( (uint8_t) i );
			instruction->type = type;
			instruction->operand_a = operand_a;
			instruction->operand_b = operand_b;
			return true;
			}
	}

	return false;
}

static bool is_instruction(const char* token)
	{
	if( !token || strlen( token ) == 0 || strlen( token ) > 15 )
		{
		return false;
		}
	char mnemonic[ 16 ];
	cpu_optype_t type = CPU_OPTYPE_NONE;
	const char* end = strchr( token, '.');
	if( !end )
		{
		strcpy( mnemonic, token );
		}
	else
		{
		int l = (int) (end - token);
		strncpy( mnemonic, token, (size_t)l );
		mnemonic[ l ] = 0;
		switch( token[ l + 1 ] )
			{
			case 'b': type = CPU_OPTYPE_BYTE; break;
			case 'w': type = CPU_OPTYPE_WORD; break;
			case 'l': type = CPU_OPTYPE_LONG; break;
			case 'f': type = CPU_OPTYPE_FLOAT; break;
			}
		}
	for( int i = 0; i < 256; ++i )
		{
		if( !cpu_opinfo_mnemonic( (uint8_t) i ) )
			{
			break;
			}
		if( stricmp( mnemonic, cpu_opinfo_mnemonic( (uint8_t) i ) ) == 0 && cpu_opinfo_optype( (uint8_t) i, type ) )
			{
			return true;
			}
		}

	return false;
	}


void* asm_assemble( char const* input, size_t input_size, size_t* output_size, asm_error_t* error, int* error_line )
	{
    string_array_t lines;
    string_array_init( &lines, 256 );

	// Extract each line
	char const* start = input;
	char const* end = input;
	while( start - input < (ptrdiff_t)input_size && end - input < (ptrdiff_t)input_size ) {
		++end;
		if( *end == '\n' || *end == '\r' || *end == '\0' || end - input >= (ptrdiff_t)input_size ) {
			char* str = (char*) malloc( ( end - start + 2 ) * sizeof( char ) );
			size_t length = (size_t)( end - start );
			strncpy( str, start, length );
			str[ length ] = '\n';
            str[ length + 1 ] = '\0';
            string_array_add( &lines, str );
			start = end + 1;
			if( start - input < (ptrdiff_t)input_size && ( *start == '\r' || *start == '\n' || *start == '\0' ) ) {
				++start;
				++end;
			}
		}
	}

	if( lines.count == 0 ) {
        if( error ) {
	    	*error = ASM_ERROR_NODATA;
        }
        return NULL;
	}


	// Assemble 

	int current_line = 0;
	tokentype_t current_token = TOKENTYPE_INVALID;
	char current_lexeme[ 256 ] = "";
	int lexeme_index_0 = 0;
	int lexeme_index_1 = 0;

	char current_mnemonic[ 256 ];
	char current_operand_a[ 256 ];
	char current_operand_b[ 256 ];
	char current_label[ 256 ];

	cpu_optype_t current_dc_type = CPU_OPTYPE_NONE;
	
	label_array_t labels;
    label_array_init( &labels, 256 );

    label_to_resolve_array_t labels_to_resolve;
    label_to_resolve_array_init( &labels_to_resolve, 256 );

    outbuf_t output;
    outbuf_init( &output, 1024 );

	while( get_next_token( &lines, &lexeme_index_0, &lexeme_index_1, &current_line, &current_token, current_lexeme ) != TOKENTYPE_END_OF_DATA ) {
		// Check all tokens that can possibly be at the start of a statement
		switch( current_token ) {
			// Just skip over comments
			case TOKENTYPE_SEMICOLON: {
				skip_to_next_line( &lines, &current_line, &current_token, &lexeme_index_0, &lexeme_index_1 );
			} break;

			// On newline, just skip forward
			case TOKENTYPE_NEWLINE: {
				skip_to_next_line( &lines, &current_line, &current_token, &lexeme_index_0, &lexeme_index_1 );
			} break;

			// Instructions
			case TOKENTYPE_INSTRUCTION: {
				// Save the mnemonic for this instruction
				strcpy( current_mnemonic, current_lexeme );
				strcpy( current_operand_a, "" );
				strcpy( current_operand_b, "" );
				
				// Check first operand
				cpu_argtype_t operand_a = parse_operand( &lines, &lexeme_index_0, &lexeme_index_1, &current_line, &current_token, current_lexeme );
				cpu_argtype_t operand_b = CPU_ARGTYPE_NONE;
				if( operand_a != CPU_ARGTYPE_NONE ) {
					strcpy( current_operand_a, current_lexeme );

					// If next token is a comma, there's another operand
					if( get_next_token( &lines, &lexeme_index_0, &lexeme_index_1, &current_line, &current_token, current_lexeme ) == TOKENTYPE_COMMA ) {
						operand_b = parse_operand( &lines, &lexeme_index_0, &lexeme_index_1, &current_line, &current_token, current_lexeme );
						if( operand_b != CPU_ARGTYPE_NONE ) {
							strcpy( current_operand_b, current_lexeme );
						}
					}
				}

				// Done with instruction parsing, so ignore what's left on this line
				skip_to_next_line( &lines, &current_line, &current_token, &lexeme_index_0, &lexeme_index_1 );
				
				// Try and find an instruction to match the given combination of mnemonic and operands
				instruction_t instruction;
				bool instruction_found = find_instruction( current_mnemonic, operand_a, operand_b, &instruction );
				if( !instruction_found ) {
					release_memory( &labels, &labels_to_resolve, &lines, &output );

					// Return Error
                    if( error_line ) {
					    *error_line = current_line;
                    }
                    if( error ) {
                        *error = ASM_ERROR_INVALIDOPERANDS; 
                    }
					return NULL;
				} else {
					// Write the opcode to the output stream
					uint8_t data[ 4 ];
					data[ 0 ] = (uint8_t) instruction.operation;
					data[ 1 ] = (uint8_t) instruction.type;
					data[ 2 ] = (uint8_t) instruction.operand_a;
					data[ 3 ] = (uint8_t) instruction.operand_b;
					outbuf_append( &output, (uint32_t*) data, sizeof( uint32_t ) );

					// Process first operand, if any
					if( operand_a != CPU_ARGTYPE_NONE )
						{
						// Get the value to write for operand
						uint32_t operand_avalue = get_operand_value( operand_a, current_operand_a );
						
						// Write the value to the output stream
						outbuf_append( &output, (uint32_t*) &operand_avalue, sizeof( uint32_t ) );

						// If the operand was a label, we need to replace it with an offset
						if( operand_a == CPU_ARGTYPE_LABEL || operand_a == CPU_ARGTYPE_AT_LABEL ) {
							uint32_t label_offset = get_label_offset( &labels, current_operand_a );
							if( label_offset == 0xffffffff ) {
								// If the offset was not found, add it to list of labels to resolve later
								add_label_to_resolve( &labels_to_resolve, current_operand_a, (uint32_t)output.size - sizeof(uint32_t), current_line );
							} else {
								// Change the already written value to be a relative offset
								int relative_offset = (int)( label_offset - output.size );
								int* location = (int*) ( ( (uintptr_t)output.data ) + output.size - sizeof( uint32_t ) );
								*location = relative_offset;
							}
						}

						// If there's another operand, process it
						if( operand_b!=CPU_ARGTYPE_NONE ) {
							// Get the value to write for operand
							uint32_t operand_bvalue = get_operand_value( operand_b, current_operand_b );
							
							// Write the value to the output stream
							outbuf_append( &output, (uint32_t*) &operand_bvalue, sizeof( uint32_t ) );

							// If the operand was a label, we need to replace it with an offset
							if( operand_b == CPU_ARGTYPE_LABEL || operand_b == CPU_ARGTYPE_AT_LABEL ) {
								uint32_t label_offset = get_label_offset( &labels, current_operand_b );
								if( label_offset == 0xffffffff ) {
									// If the offset was not found, add it to list of labels to resolve later
									add_label_to_resolve( &labels_to_resolve, current_operand_b, (uint32_t) output.size - sizeof( uint32_t ), current_line );
								} else {
									// Change the already written value to be a relative offset
									int relative_offset = (int)( label_offset - output.size );
									int* location = (int*)( ( (uintptr_t)output.data ) + output.size - sizeof(uint32_t) );
									*location = relative_offset;
								}
							}
						}
					}
				}
			} break;

			// Labels
			case TOKENTYPE_IDENTIFIER: {
				strcpy( current_label, current_lexeme );
				if( get_next_token( &lines, &lexeme_index_0, &lexeme_index_1, &current_line, &current_token, current_lexeme ) == TOKENTYPE_COLON ) {
					// Add label to the list
					label_t label;
					label.label = strdup( current_label );
					label.offset = (uint32_t) output.size;
					label_array_add( &labels, label );

					// Resolve labels that couldn't be resolved as they where encountered
					for( int i = labels_to_resolve.count - 1; i >= 0; --i ) {
						// Check if this resolve entry was for this label
						if( stricmp( labels_to_resolve.items[ i ].label, label.label ) == 0 ) {
							// Fill in the offset at the location where the label was previously found
							int* address = (int*) ( ( (uintptr_t)output.data ) + labels_to_resolve.items[ i ].location_to_resolve_to );
							int relative_offset = (int)( label.offset - ( labels_to_resolve.items[ i ].location_to_resolve_to + sizeof( uint32_t ) ) );
							*address = relative_offset;

							// Remove the resolved entry
							label_to_resolve_array_remove( &labels_to_resolve, i );
						}
					}
				} else {
					release_memory( &labels, &labels_to_resolve, &lines, &output );

					// Return Error
                    if( error_line ) {
					    *error_line = current_line;
                    }
                    if( error ) {
                        *error = ASM_ERROR_COLONEXPECTED;
                    }
                    return NULL;
				}
					

			} break;

			// dc.b/dc.w/dc.l/dc.f
			case TOKENTYPE_DEFINE_CONSTANT: {
				if( stricmp( current_lexeme, "dc.b" ) == 0 ) {
					current_dc_type = CPU_OPTYPE_BYTE;
				} else if ( stricmp( current_lexeme, "dc.w" ) == 0 ) {
					current_dc_type = CPU_OPTYPE_WORD;
				} else if ( stricmp( current_lexeme, "dc.l" ) ==0 ) {
					current_dc_type = CPU_OPTYPE_LONG;
				} else if ( stricmp( current_lexeme, "dc.f" ) ==0 ) {
					current_dc_type = CPU_OPTYPE_FLOAT;
				}

				get_next_token( &lines, &lexeme_index_0, &lexeme_index_1, &current_line, &current_token, current_lexeme );
				while( current_token != TOKENTYPE_SEMICOLON && current_token != TOKENTYPE_NEWLINE && current_token != TOKENTYPE_END_OF_DATA ) {
					switch (current_token) {
						case TOKENTYPE_INTEGER:
						case TOKENTYPE_HEXADECIMAL:
						case TOKENTYPE_BINARY: {
							uint32_t integer = integer_from_string( current_lexeme );
							if( current_dc_type == CPU_OPTYPE_BYTE ) {
								uint8_t value= (uint8_t) integer;
								outbuf_append( &output, (uint8_t*) &value, sizeof( uint8_t ) );
							} else if ( current_dc_type == CPU_OPTYPE_WORD ) {
								uint16_t value = (uint16_t) integer;
								outbuf_append( &output, (uint16_t*) &value, sizeof( uint16_t ) );
							} else if ( current_dc_type == CPU_OPTYPE_LONG ) {
								outbuf_append( &output, (uint32_t*) &integer, sizeof( uint32_t ) );
							}
						} break;

						case TOKENTYPE_FLOAT: {
							float value = float_from_string( current_lexeme );
							outbuf_append( &output, (uint32_t*) &value, sizeof( uint32_t ) );
						} break;

						case TOKENTYPE_STRING: {
                            uint32_t len = (uint32_t) strlen( current_lexeme );
							for( uint32_t i = 0; i < len; ++i ) {
								uint8_t character = (uint8_t) current_lexeme[ i ];
								if( current_dc_type == CPU_OPTYPE_BYTE ) {
									outbuf_append( &output, (uint8_t*) &character, sizeof( uint8_t ) );
								} else if ( current_dc_type == CPU_OPTYPE_WORD ) {
									uint16_t value = (uint16_t) character;
									outbuf_append( &output, (uint16_t*) &value, sizeof( uint16_t ) );
								} else if ( current_dc_type == CPU_OPTYPE_LONG ) {
									uint32_t value = (uint32_t)character;
									outbuf_append( &output, (uint32_t*) &value, sizeof( uint32_t ) );
								}
							}
						} break;

						case TOKENTYPE_COMMA: {
							// Do nothing
						} break;

						// Unexpected token
                        case TOKENTYPE_INVALID:
                        case TOKENTYPE_NEWLINE:
                        case TOKENTYPE_IDENTIFIER:
                        case TOKENTYPE_COLON:
                        case TOKENTYPE_SEMICOLON:
                        case TOKENTYPE_AT:
                        case TOKENTYPE_DATA_REGISTER:
                        case TOKENTYPE_ADDRESS_REGISTER:
                        case TOKENTYPE_ADDRESS_REGISTER_INC:
                        case TOKENTYPE_ADDRESS_REGISTER_DEC:
                        case TOKENTYPE_FLOAT_REGISTER:
                        case TOKENTYPE_INSTRUCTION:
                        case TOKENTYPE_STACK_INDEX:
                        case TOKENTYPE_DEFINE_CONSTANT:
                        case TOKENTYPE_DEFINE_CONSTANT_BLOCK:
                        case TOKENTYPE_DEFINE_STORAGE:
                        case TOKENTYPE_INCLUDE:
                        case TOKENTYPE_INCBIN:
                        case TOKENTYPE_END_OF_DATA:
						default: {
							release_memory( &labels, &labels_to_resolve, &lines, &output );

							// Return error
                            if( error_line ) {
							    *error_line = current_line;
                            }
							if( error ) {
                                *error = ASM_ERROR_UNEXPECTEDTOKEN;
                            }
                            return NULL;
						} break;
					}

					get_next_token( &lines, &lexeme_index_0, &lexeme_index_1, &current_line, &current_token, current_lexeme );
				}
			} break;

			// dcb.b/dcb.w/dcb.l/dcb.f
			case TOKENTYPE_DEFINE_CONSTANT_BLOCK: {
				if( stricmp( current_lexeme, "dcb.b" ) == 0 ) {
					current_dc_type = CPU_OPTYPE_BYTE;
				} else if ( stricmp( current_lexeme, "dcb.w" ) == 0 ) {
					current_dc_type = CPU_OPTYPE_WORD;
				} else if ( stricmp( current_lexeme, "dcb.l" ) == 0 ) {
					current_dc_type = CPU_OPTYPE_LONG;
				} else if ( stricmp( current_lexeme, "dcb.f" ) == 0 ) {
					current_dc_type = CPU_OPTYPE_FLOAT;
				}

				get_next_token( &lines, &lexeme_index_0, &lexeme_index_1, &current_line, &current_token, current_lexeme );
				if( current_token != TOKENTYPE_INTEGER && current_token != TOKENTYPE_HEXADECIMAL && current_token != TOKENTYPE_BINARY ) {
					release_memory( &labels, &labels_to_resolve, &lines, &output );

					// Return error
                    if( error_line ) {
					    *error_line = current_line;
                    }
                    if( error ) {
					    *error = ASM_ERROR_UNEXPECTEDTOKEN;
                    }
                    return NULL;
				}
				
				uint32_t count = integer_from_string( current_lexeme );

				if( get_next_token( &lines, &lexeme_index_0, &lexeme_index_1, &current_line, &current_token, current_lexeme ) != TOKENTYPE_COMMA ) {
					release_memory( &labels, &labels_to_resolve, &lines, &output );

					// Return error
                    if( error_line ) {
					    *error_line = current_line;
                    }
                    if( error ) {
					    *error = ASM_ERROR_UNEXPECTEDTOKEN;
                    }
                    return NULL;
				}

				get_next_token( &lines, &lexeme_index_0, &lexeme_index_1, &current_line, &current_token, current_lexeme );
				if( current_token != TOKENTYPE_INTEGER && current_token != TOKENTYPE_HEXADECIMAL && current_token != TOKENTYPE_BINARY ) {
					release_memory( &labels, &labels_to_resolve, &lines, &output );

					// Return error
                    if( error_line ) {
					    *error_line = current_line;
                    }
                    if( error ) {
					    *error = ASM_ERROR_UNEXPECTEDTOKEN;
                    }
                    return NULL;
				}

				uint32_t integer = integer_from_string( current_lexeme );

				for (uint32_t i = 0; i < count; ++i ) {
					if( current_dc_type == CPU_OPTYPE_BYTE ) {
						uint8_t value = (uint8_t) integer;
						outbuf_append( &output, (uint8_t*) &value, sizeof( uint8_t ) );
					} else if( current_dc_type == CPU_OPTYPE_WORD ) {
						uint16_t value = (uint16_t) integer;
						outbuf_append( &output, (uint16_t*) &value, sizeof( uint16_t ) );
					} else if( current_dc_type == CPU_OPTYPE_LONG ) {
						outbuf_append( &output, (uint32_t*) &integer, sizeof( uint32_t ) );
					}
				}
			} break;

			// ds.b/ds.w/ds.l/ds.f
			case TOKENTYPE_DEFINE_STORAGE: {
				if( stricmp( current_lexeme, "ds.b" ) == 0 ) {
					current_dc_type = CPU_OPTYPE_BYTE;
				} else if( stricmp( current_lexeme, "ds.w" ) == 0 ) {
					current_dc_type = CPU_OPTYPE_WORD;
				}
				else if( stricmp( current_lexeme, "ds.l" ) == 0 ) {
					current_dc_type=CPU_OPTYPE_LONG;
				} else if ( stricmp( current_lexeme, "ds.f" ) == 0 ) {
					current_dc_type=CPU_OPTYPE_FLOAT;
				}

				if( get_next_token( &lines, &lexeme_index_0, &lexeme_index_1, &current_line, &current_token, current_lexeme ) != TOKENTYPE_INTEGER ) {
					release_memory( &labels, &labels_to_resolve, &lines, &output );

					// Return error
                    if( error_line ) {
					    *error_line = current_line;
                    }
                    if( error ) {
					    *error = ASM_ERROR_UNEXPECTEDTOKEN;
                    }
                    return NULL;
				}
				
				uint32_t count = integer_from_string( current_lexeme );
                (void) count;
                // TODO: Implememt storage
				} break;

			// include
			case TOKENTYPE_INCLUDE: {
				// TODO: Implement include
			} break;

            // incbin
			case TOKENTYPE_INCBIN: {
                // TODO: Implement incbin
			} break;

			// Unexpected token
            case TOKENTYPE_INVALID:
            case TOKENTYPE_INTEGER:
            case TOKENTYPE_HEXADECIMAL:
            case TOKENTYPE_BINARY:
            case TOKENTYPE_FLOAT:
            case TOKENTYPE_COLON:
            case TOKENTYPE_STRING:
            case TOKENTYPE_COMMA:
            case TOKENTYPE_AT:
            case TOKENTYPE_DATA_REGISTER:
            case TOKENTYPE_ADDRESS_REGISTER:
            case TOKENTYPE_ADDRESS_REGISTER_INC:
            case TOKENTYPE_ADDRESS_REGISTER_DEC:
            case TOKENTYPE_FLOAT_REGISTER:
            case TOKENTYPE_STACK_INDEX:
            case TOKENTYPE_END_OF_DATA:
			default: {
				release_memory( &labels, &labels_to_resolve, &lines, &output );

				// Return error
                if( error_line ) {
					*error_line = current_line;
                }
                if( error ) {
					*error = ASM_ERROR_UNEXPECTEDTOKEN;
                }
                return NULL;
			} break;
		}

	}

	if( labels_to_resolve.count > 0 ) {
	    // Release memory
	    release_memory( &labels, &labels_to_resolve, &lines, &output );

        if( error_line ) {
		    *error_line = labels_to_resolve.items[ 0 ].line;
        }
        if( error ) {
		    *error = ASM_ERROR_UNDEFINEDLABEL;
        }
        return NULL;
	}

	// Release memory
	release_memory( &labels, &labels_to_resolve, &lines, NULL );
    
    if( error ) {
        *error = ASM_ERROR_NO_ERROR;
    }
    if( output_size ) {
        *output_size = output.size;
    }
	return output.data;
}


#define max(a,b)    (((a) > (b)) ? (a) : (b))
#define min(a,b)    (((a) < (b)) ? (a) : (b))



tokentype_t get_next_token( string_array_t* lines, int* lexeme_index_0, int* lexeme_index_1, int* current_line, tokentype_t* current_token, char* current_lexeme )
	{
    *lexeme_index_0 = *lexeme_index_1;

	// Check if we've reached the end of the line
	if( *current_line >= lines->count || *lexeme_index_0 >= (int)strlen( lines->items[ *current_line ] ) ) {
		// Proceed to the next line. If there isn't one, return EOD
        if( !skip_to_next_line( lines, current_line, current_token, lexeme_index_0, lexeme_index_1 ) ) {
			*current_lexeme= '\0';
            return TOKENTYPE_END_OF_DATA;
		}
	}

	// Skip whitespace
	while( is_whitespace( lines->items[ *current_line] [ *lexeme_index_0 ] ) ) {
		*lexeme_index_0 = *lexeme_index_0 + 1;
	}

	// Found start of lexeme, now scan for end
	*lexeme_index_1 = *lexeme_index_0;
	while( *lexeme_index_1 < (int)strlen( lines->items[ *current_line ]) && !is_delimiter( lines->items[ *current_line ][ *lexeme_index_1 ] ) ) {
		*lexeme_index_1 = *lexeme_index_1 + 1;
	}
	
	// Manually advance the end by one for single-character lexemes
	if( *lexeme_index_0 == *lexeme_index_1 ) {
		*lexeme_index_1 = *lexeme_index_1 + 1;
	}
	
	// Extract the lexeme 
	strncpy( current_lexeme, &lines->items[ *current_line ][ *lexeme_index_0 ], (size_t)( *lexeme_index_1 - *lexeme_index_0 ) );
	current_lexeme[ *lexeme_index_1 - *lexeme_index_0 ] = '\0'; // Terminate the string
	strlwr( current_lexeme ); // Convert to lower case


    ///////
	// Identify token

	// First, do the single character tokens
	if( strlen( current_lexeme ) == 1 ) {
		if( *current_lexeme == '"' ) {
			// For strings, we build a token from one quote to the next
			*current_token = TOKENTYPE_STRING;
    		*lexeme_index_0 = *lexeme_index_0 + 1;
    		*lexeme_index_1 = *lexeme_index_1 + 1;
			while( lines->items[ *current_line ][ *lexeme_index_1 ] != '"' && lines->items[ *current_line ][ *lexeme_index_1 ] != '\n' && lines->items[ *current_line ][ *lexeme_index_1 ] != '\0' ){
        		*lexeme_index_1 = *lexeme_index_1 + 1;
			}
			strncpy( current_lexeme, &lines->items[ *current_line ][ *lexeme_index_0 ], (size_t)( *lexeme_index_1 - *lexeme_index_0 ) );
			current_lexeme[ *lexeme_index_1 - *lexeme_index_0 ] = '\0'; // Terminate the string
        	*lexeme_index_1 = *lexeme_index_1 + 1;
		} else if( *current_lexeme == ',' ) {
			*current_token = TOKENTYPE_COMMA;
		} else if( *current_lexeme == ':' ) {
			*current_token = TOKENTYPE_COLON;
		} else if( *current_lexeme == ';' ) {
			*current_token = TOKENTYPE_NEWLINE;
		} else if ( *current_lexeme == '@' ) {
			*current_token = TOKENTYPE_AT;
		} else if ( *current_lexeme == '\n' ) {
			*current_token = TOKENTYPE_NEWLINE;
		}
	}

	// Do multicharacter tokens

	if( is_integer( current_lexeme ) ) {
		*current_token = TOKENTYPE_INTEGER;
	} 
    
    if( is_hexadecimal( current_lexeme ) ) {
		*current_token = TOKENTYPE_HEXADECIMAL;
	} 
    
    if( is_binary( current_lexeme ) ) {
		*current_token = TOKENTYPE_BINARY;
	} 
    
    if( is_float( current_lexeme ) ) {
		*current_token = TOKENTYPE_FLOAT;
	} 
    
    if( is_identifier_str( current_lexeme ) ) {
		*current_token = TOKENTYPE_IDENTIFIER;
		
		// See if this is the sp or ip register
		if( stricmp( current_lexeme, "ip" ) == 0 || stricmp( current_lexeme, "sp" ) == 0 ) {
			*current_token = TOKENTYPE_ADDRESS_REGISTER;
		}

		// See if this is the tr register
		if( stricmp( current_lexeme, "tr" ) == 0 ) {
			*current_token = TOKENTYPE_DATA_REGISTER;
		}

		// See if it's d0-d15
		if( tolower( *current_lexeme ) =='d' && is_integer( &current_lexeme[ 1 ] ) ) {
			int index = atoi( &current_lexeme[ 1 ] );
			if( index >=0 && index <= 15 ) {
				*current_token = TOKENTYPE_DATA_REGISTER;
			}
		}
		// See if it's a0-a15
		if( tolower( *current_lexeme ) =='a' )
			{
			int l = (int) strlen( current_lexeme ) - 1;
			if( current_lexeme[ l ] == '+' ) {
				char lex[ 4 ] = { 0, 0, 0, 0 };
				strncpy( lex, current_lexeme, (size_t) l );
				if( is_integer( &lex[ 1 ]  ) ) {
					int index = atoi( &lex[ 1 ] );
					if( index >= 0 && index <= 15 ) {
						*current_token = TOKENTYPE_ADDRESS_REGISTER_INC;
					}
				}
			} else if( current_lexeme[ l ] == '-' ) {
				char lex[ 4 ] = { 0, 0, 0, 0 };
				strncpy( lex, current_lexeme, (size_t) l );
				if( is_integer( &lex[ 1 ] ) ) {
					int index = atoi( &lex[ 1 ] );
					if( index >=0 && index <= 15 ) {
						*current_token = TOKENTYPE_ADDRESS_REGISTER_DEC;
					}
			    }
			} else if( is_integer( &current_lexeme[ 1 ] ) ) {
				int index = atoi( &current_lexeme[ 1 ] );
				if( index >=0 && index <= 15 ) {
					*current_token = TOKENTYPE_ADDRESS_REGISTER;
				}
			}
		}		
        
        // See if it's f0-f31
		if( tolower( *current_lexeme ) =='f' && is_integer( &current_lexeme[ 1 ] ) ) {
			int index = atoi( &current_lexeme[ 1 ] );
			if( index >= 0 && index <= 15 ) {
				*current_token = TOKENTYPE_FLOAT_REGISTER;
			}
		}
	}

	// Check for "define constant" directive
	if( stricmp( current_lexeme, "dc.b" ) == 0 || stricmp( current_lexeme, "dc.w" ) == 0 || stricmp( current_lexeme, "dc.l" ) == 0 || stricmp( current_lexeme, "dc.f" ) == 0 ) {
		*current_token = TOKENTYPE_DEFINE_CONSTANT;
	}

	// Check for "define constant block" directive
	if( stricmp( current_lexeme, "dcb.b" ) == 0 || stricmp( current_lexeme, "dcb.w" ) == 0 || stricmp( current_lexeme, "dcb.l") == 0 || stricmp( current_lexeme, "dcb.f" ) == 0 ) {
		*current_token = TOKENTYPE_DEFINE_CONSTANT_BLOCK;
	}

	// Check for "define storage" directive
	if( stricmp( current_lexeme, "ds.b" ) == 0 || stricmp( current_lexeme, "ds.w" ) == 0 || stricmp( current_lexeme, "ds.l" ) == 0 || stricmp( current_lexeme, "ds.f" ) == 0 ) {
		*current_token = TOKENTYPE_DEFINE_STORAGE;
	}

	// Check for "include" directive
	if( stricmp( current_lexeme, "include" ) == 0 ) {
		*current_token = TOKENTYPE_INCLUDE;
	}

	// Check for "incbin" directive
	if( stricmp( current_lexeme, "incbin" ) == 0 ) {
		*current_token = TOKENTYPE_INCBIN;
	}

	// Check if token is an instruction
	if( is_instruction( current_lexeme ) ) {
		*current_token = TOKENTYPE_INSTRUCTION;
	}

	if( is_stack_index( current_lexeme ) ) {
		*current_token = TOKENTYPE_STACK_INDEX;

		// Extract Index
		char buf[ 256 ];
		strncpy( buf, current_lexeme + 3, min( 255, strlen( current_lexeme ) - 4 ) );
		buf[ min( 255, strlen( current_lexeme ) - 4 ) ] = '\0';
		strcpy( current_lexeme, buf );		
	}

    return *current_token;
}


bool skip_to_next_line( string_array_t* lines, int* current_line, tokentype_t* current_token, int* lexeme_index_0, int* lexeme_index_1 ) {
    *current_line = *current_line + 1;
	*current_token = TOKENTYPE_INVALID;
	*lexeme_index_0 = 0;
	*lexeme_index_1 = 0;
	if( *current_line >= lines->count) {
		return false;
	}
	
    return true;
}


int get_register_index( const char* register_name ) {
	if( !register_name || *register_name == '\0' ) {
		return -1;
	}

	if( stricmp( register_name, "ip" ) == 0 ) {
		return (int)CPU_REG_IP;
	} else if( stricmp( register_name, "sp" ) == 0 ) {
		return (int)CPU_REG_SP;
	} else if( stricmp( register_name, "tr" ) == 0 ) {
		return (int)CPU_REG_TR;
	} else if( tolower( *register_name ) == 'd' ) {
		if( is_integer( &register_name[ 1 ] ) ) {
			return (int)CPU_REG_D0 + atoi( &register_name[ 1 ] );
		}
	} else if( tolower( *register_name ) == 'a' ) {
		int l = (int) strlen( register_name ) - 1;
		if( register_name[ l ] == '+' ) {
			char reg[ 4 ] = { 0, 0, 0, 0 };
			strncpy( reg, register_name, (size_t) l );
			if( is_integer( &reg[ 1 ] ) ) {
				return (int)CPU_REG_A0 + atoi( &reg[ 1 ] );
			}
		} else if( register_name[ l ] == '-' )
			{
			char reg[ 4 ] = { 0, 0, 0, 0 };
			strncpy( reg, register_name, (size_t) l );
			if( is_integer( &reg[ 1 ] ) ) {
				return (int)CPU_REG_A0 + atoi( &reg[ 1 ] );
			}
		} else if( is_integer( &register_name[ 1 ] ) ) {
			return (int)CPU_REG_A0 + atoi( &register_name[ 1 ] );
		}
	} else if( tolower( *register_name ) == 'f' ) {
		if( is_integer( &register_name[ 1 ] ) ) {
			return (int)CPU_REG_F0 + atoi( &register_name[ 1 ] );
		}
	}

	return -1;	
}


uint32_t get_operand_value( cpu_argtype_t operand, const char* lexeme ) {
	switch( operand ) {
		case CPU_ARGTYPE_STACK:
		case CPU_ARGTYPE_AT_STACK: {
			return integer_from_string( lexeme );
		} break;

		case CPU_ARGTYPE_AREG:
		case CPU_ARGTYPE_AT_AREG:
		case CPU_ARGTYPE_AT_AREG_INC:
		case CPU_ARGTYPE_AT_AREG_DEC:
		case CPU_ARGTYPE_DREG:
		case CPU_ARGTYPE_FREG: {
			return (uint32_t)get_register_index( lexeme );
		} break;

		case CPU_ARGTYPE_INTEGER:
		case CPU_ARGTYPE_AT_INTEGER: {
			return integer_from_string( lexeme );
		} break;

		case CPU_ARGTYPE_FLOAT: {
			float value = float_from_string( lexeme );
			uint32_t intval = *( (uint32_t*) &value );
			return intval;
		} break;

        case CPU_ARGTYPE_NONE:
        case CPU_ARGTYPE_LABEL:
        case CPU_ARGTYPE_AT_LABEL: {
            return 0;
        } break;
	}

    return 0;
}


uint32_t get_label_offset( label_array_t* labels, const char* label ) {
	for( int i = 0; i < labels->count; ++i ) {
		if( stricmp( labels->items[ i ].label, label ) == 0 ) {
			return labels->items[ i ].offset;
		}
	}
	return 0xffffffff;
}


void add_label_to_resolve( label_to_resolve_array_t* labels_to_resolve, const char* label, uint32_t location, int current_line ) {
	label_to_resolve_t entry;
	entry.label = strdup( label );
	entry.location_to_resolve_to = location;
	entry.line = current_line;
	label_to_resolve_array_add( labels_to_resolve, entry );
}


void release_memory( label_array_t* labels, label_to_resolve_array_t* labels_to_resolve, string_array_t* lines, outbuf_t* outbuf ) {
    if( labels ) { 
        label_array_term( labels );
    }
    if( labels_to_resolve ) {
        label_to_resolve_array_term( labels_to_resolve );
    }
    if( lines ) {
        string_array_term( lines );
    }
    if( outbuf ) {
        outbuf_term( outbuf );
    }
}


cpu_argtype_t parse_operand( string_array_t* lines, int* lexeme_index_0, int* lexeme_index_1, int* current_line, tokentype_t* current_token, char* current_lexeme ) {
	get_next_token( lines, lexeme_index_0, lexeme_index_1, current_line, current_token, current_lexeme );

	switch( *current_token ) {
		case TOKENTYPE_INTEGER:
		case TOKENTYPE_HEXADECIMAL:
		case TOKENTYPE_BINARY: {
			return CPU_ARGTYPE_INTEGER;			
		} break;

		case TOKENTYPE_FLOAT: {
			return CPU_ARGTYPE_FLOAT;
		} break;

		case TOKENTYPE_IDENTIFIER: {
			return CPU_ARGTYPE_LABEL;
		} break;

		case TOKENTYPE_STACK_INDEX: {
			return CPU_ARGTYPE_STACK;
		} break;

		case TOKENTYPE_AT: {
			get_next_token( lines, lexeme_index_0, lexeme_index_1, current_line, current_token, current_lexeme );

			switch( *current_token ) {
				case TOKENTYPE_STACK_INDEX: {
					return CPU_ARGTYPE_AT_STACK;
				} break;

				case TOKENTYPE_INTEGER:
				case TOKENTYPE_HEXADECIMAL:
				case TOKENTYPE_BINARY: {
					return CPU_ARGTYPE_AT_INTEGER;				
				} break;
								
				case TOKENTYPE_IDENTIFIER: {
					return CPU_ARGTYPE_AT_LABEL;
				} break;

				case TOKENTYPE_ADDRESS_REGISTER: {
					return CPU_ARGTYPE_AT_AREG;
				} break;

				case TOKENTYPE_ADDRESS_REGISTER_INC: {
					return CPU_ARGTYPE_AT_AREG_INC;
				} break;

				case TOKENTYPE_ADDRESS_REGISTER_DEC: {
					return CPU_ARGTYPE_AT_AREG_DEC;
				} break;

                case TOKENTYPE_INVALID:
                case TOKENTYPE_NEWLINE:
                case TOKENTYPE_FLOAT:
                case TOKENTYPE_COLON:
                case TOKENTYPE_SEMICOLON:
                case TOKENTYPE_STRING:
                case TOKENTYPE_COMMA:
                case TOKENTYPE_AT:
                case TOKENTYPE_DATA_REGISTER:
                case TOKENTYPE_FLOAT_REGISTER:
                case TOKENTYPE_INSTRUCTION:
                case TOKENTYPE_DEFINE_CONSTANT:
                case TOKENTYPE_DEFINE_CONSTANT_BLOCK:
                case TOKENTYPE_DEFINE_STORAGE:
                case TOKENTYPE_INCLUDE:
                case TOKENTYPE_INCBIN:
                case TOKENTYPE_END_OF_DATA: {
                    return CPU_ARGTYPE_NONE;
                }
            }
		} break;

		case TOKENTYPE_DATA_REGISTER: {
			return CPU_ARGTYPE_DREG;
		} break;


		case TOKENTYPE_ADDRESS_REGISTER: {
			return CPU_ARGTYPE_AREG;
		} break;

		case TOKENTYPE_FLOAT_REGISTER: {
			return CPU_ARGTYPE_FREG;
		} break;
        
        case TOKENTYPE_INVALID:           
        case TOKENTYPE_NEWLINE:
        case TOKENTYPE_COLON:
        case TOKENTYPE_SEMICOLON:
        case TOKENTYPE_STRING:
        case TOKENTYPE_COMMA:
        case TOKENTYPE_ADDRESS_REGISTER_INC:
        case TOKENTYPE_ADDRESS_REGISTER_DEC:
        case TOKENTYPE_INSTRUCTION:
        case TOKENTYPE_DEFINE_CONSTANT:
        case TOKENTYPE_DEFINE_CONSTANT_BLOCK:
        case TOKENTYPE_DEFINE_STORAGE:
        case TOKENTYPE_INCLUDE:
        case TOKENTYPE_INCBIN:
        case TOKENTYPE_END_OF_DATA: {
        	return CPU_ARGTYPE_NONE;
        }
    }

	return CPU_ARGTYPE_NONE;
}


static bool is_whitespace( char c ) { 
	if( c == ' ' ) {
		return true;
	}

	if( c == '\t' ) {
		return true;
	}

	return false;
}


static bool is_number( char c ) { 
	if( c >= '0' && c <= '9' ) {
		return true;
	}

	return false;
}


static bool is_identifier( char c ) { 
	if( c >= '0' && c <= '9' ) {
		return true;
	}

	if( c >= 'A' && c <= 'Z' ) {
		return true;
	}

	if( c >= 'a' && c <= 'z' ) {
		return true;
	}

	if( c == '_' ) {
		return true;
	}

	return false;
}


static bool is_delimiter( char c ) { 
	if( c== ';' ) {
		return true;
	}

	if( c == ':' ) {
		return true;
	}

	if( c==',' ) {
		return true;
	}

	if( c == '"' ) {
		return true;
	}

	if( c == '@' ) {
		return true;
	}

	if( c == '\n' ) {
		return true;
	}

	if( is_whitespace( c ) ) {
		return true;
	}

	return false;
}


static bool is_identifier_str( const char* str ) { 
	// Return false on invalid string
	if( !str ) {
		return false;
	}

	// If it's an empty string, it's not a valid identifier
	if( *str == '\0' ) {
		return false;
	}

	// If it starts with a number, it's not a valid identifier
	if( is_number( *str ) ) {
		return false;
	}

	// Check each character to make sure it is valid for identifier
    int len = (int) strlen( str );
	for( int i = 0; i < len; ++i ) {
		if( !is_identifier( str[ i ] ) ) {
            // Recognize a0+, a0- etc as identifiers
			if( str[ i ] == '+' || str[ i ] == '-' ) {
				if( i == len - 1 && *str == 'a' ) {
					for( int j = 1; j < i; ++j ) {
						if( !is_number( str[ j ] ) ) {
							return false;
						}
					}
                    return true;
			    }
		    }
		return false;
		}
	}

	// Tests passed, the string is a valid identifier
	return true;
	}


static bool is_stack_index( const char* str ) { 
	// Return false on invalid string
	if( !str ) {
		return false;
	}

	// If it's an empty string, it's not a valid stackindex
	if( *str == '\0' ) {
		return false;
	}

	// If it doesn't start with sp(, it's not a valid stack index
	if( strnicmp( str, "sp(", 3 ) != 0 ) {
		return false;
	}

    int len = (int) strlen( str );

	// If it doesn't end with ), it's not a valid stack index
	if( str[ len - 1 ] != ')' ) {
		return false;
	}

	// Check each character to make sure it is valid number
	for( int i = 3; i < len - 1; ++i ) {
		if( !is_number( str[ i ] ) ) {
			return false;
		}
	}

	// Tests passed, the string is a valid stack index
	return true;
}


static bool is_integer( const char* str ) { 
	// Return false on invalid string
	if( !str ) {
		return false;
	}

	// If it's an empty string, it's not a valid integer
	if( *str == '\0' ) {
		return false;
	}

	// Check each character to make sure it is valid for an integer
    int len = (int) strlen( str );
	for( int i = 0; i < len; ++i ) {
		// The minus may only appear at the start, and there must be at least one more character 
		if( str[ i ] == '-' ) {
			if( i != 0 || str[ 1 ] == 0 ) {
				return false;
			}
		}
		// And the rest of the string needs to be numbers
		else if( !is_number( str[ i ] ) ) {
			return false;
		}
	}

	// Tests passed, the string is a valid integer
	return true;
}


static bool is_float( const char* str ) { 
	// Return false on invalid string
	if( !str ) {
		return false;
	}

	// If it's an empty string, it's not a valid float
    if( *str == '\0' ) {
		return false;
	}

	// Make sure the string starts with the symbol for float (#)
	if( *str != '#' ) {
		return false;
	}

	// Check each character to make sure it is valid for a float
	bool decimalPointFound = false;
    int len = (int) strlen( str );
	for( int i = 1; i < len; ++i ) {
		// The minus may only appear at the start, and there must be at least one more character 
		if( str[ i ] == '-' ) {
			if( i != 1 || str[ 2 ] == '\0' ) {
				return false;
			}
		}
		// The decimal point may occur only once
		else if( str[ i ] == '.' ) {
			if( decimalPointFound ) {
				return false;
			}
			decimalPointFound = true;
		}
		// And the rest of the string needs to be numbers
		else if( !is_number( str[ i ] ) ) {
			return false;
		}
	}

	// Tests passed, the string is a valid float
	return true;
}


static bool is_binary( const char* str ) { 
	// Return false on invalid string
	if( !str ) {
		return false;
	}

	// If it's an empty string, it's not a valid binary number
	if( *str == '\0' ) {
		return false;
	}

	// Make sure the string starts with the symbol for binary numbers (%)
	if( *str != '%' ) {
		return false;
	}

	// The string must contain at least one more character
	if( str[ 1 ] == '\0' ) {
		return false;
	}

	// Check each character to make sure it is valid for a binary number
    int len = (int) strlen( str );
	for( int i = 1; i < len; ++i ) {
		// Only 0 and 1 are allowed
		if( str[ i ] != '0' && str[ i ] != '1' ) {
			return false;
		}
	}

	// Tests passed, the string is a valid binary number
	return true;
}


static bool is_hexadecimal( const char* str ) { 
	// Return false on invalid string
	if( !str ) {
		return false;
	}

	// If it's an empty string, it's not a valid hex number
	if( *str == '\0' ) {
		return false;
	}

	// Make sure the string starts with the symbol for hex numbers ($)
	if( *str != '$' ) {
		return false;
	}

	// The string must contain at least one more character
	if( str[ 1 ] == '\0' ) {
		return false;
	}

	// Check each character to make sure it is valid for a hex number
    int len = (int) strlen( str );
	for( int i = 1; i < len; ++i ) {
		// Only 0-9 and A-F are allowed
		if( ( str[ i ] < '0' || str[ i ] > '9' ) && ( tolower( str[ i ] ) < 'a' || tolower( str[ i ] ) > 'f' ) ) {
			return false;
		}
	}

	// Tests passed, the string is a valid hex number
	return true;
}


static uint32_t integer_from_string( const char* string ) {
	if( string == 0 || *string == 0 ) {
		return 0;
	}

	// Hexadecimal
	if( *string == '$' ) {
		int i = 1;
		uint32_t result = 0;
		while( string[ i ] != '\0' ) {
			result <<= 4;
			int chr = tolower( string[ i ] );
			if( chr >= '0' && chr <= '9' ) {
				result |= ( chr - '0' );
			} else if( chr >= 'a' && chr <= 'f') {
				result |= 10 + ( chr - 'a' );
			}
			++i;
		}
		return result;
		}

	// Binary
	if( *string == '%' ) {
		int i = 1;
		uint32_t result = 0;
		while( string[ i ] != '\0' ) {
			result <<= 1;
			if( string[ i ] == '1' ) {
				result |= 1;
			}
			++i;
		}
		return result;
	}

	// Integer
	return (uint32_t) atoi( string );
	}


static float float_from_string( const char* string ) {
	if( string == 0 || *string == '\0' || *string != '#' ) {
		return 0.0f;
	}

	return (float)atof( &string[ 1 ] );
}


void asm_free( void* ptr ) {
    free( ptr );
}


#endif /* SVGS_IMPLEMENTATION */
