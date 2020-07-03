#ifndef cpu_h
#define cpu_h


typedef struct cpu_t cpu_t;

cpu_t* cpu_create( mmu_t* mmu );
void cpu_destroy( cpu_t* cpu );


void cpu_reset_cycle_acc( cpu_t* cpu );
void cpu_fetch_decode_execute( cpu_t* cpu, int cycles );


typedef enum cpu_reg_t {
    CPU_REG_IP, CPU_REG_SP, CPU_REG_TR,
    
    CPU_REG_D0, CPU_REG_D1, CPU_REG_D2, CPU_REG_D3, CPU_REG_D4, CPU_REG_D5, CPU_REG_D6, CPU_REG_D7,  
    CPU_REG_D8, CPU_REG_D9, CPU_REG_D10, CPU_REG_D11, CPU_REG_D12, CPU_REG_D13, CPU_REG_D14, CPU_REG_D15, 

    CPU_REG_A0, CPU_REG_A1, CPU_REG_A2, CPU_REG_A3, CPU_REG_A4, CPU_REG_A5, CPU_REG_A6, CPU_REG_A7,  
    CPU_REG_A8, CPU_REG_A9, CPU_REG_A10, CPU_REG_A11, CPU_REG_A12, CPU_REG_A13, CPU_REG_A14, CPU_REG_A15, 

    CPU_REG_F0, CPU_REG_F1, CPU_REG_F2, CPU_REG_F3, CPU_REG_F4, CPU_REG_F5, CPU_REG_F6, CPU_REG_F7,  
    CPU_REG_F8, CPU_REG_F9, CPU_REG_F10, CPU_REG_F11, CPU_REG_F12, CPU_REG_F13, CPU_REG_F14, CPU_REG_F15, 

    CPU_REGCOUNT
} cpu_reg_t;


uint32_t* cpu_ireg( cpu_t* cpu, cpu_reg_t reg );
float* cpu_freg( cpu_t* cpu, cpu_reg_t reg );


#endif /* cpu_h */


#ifdef SVGS_IMPLEMENTATION


struct cpu_t {
    mmu_t* mmu;
    int cycle_accum;
    uint32_t registers[ 256 ];
    int base_cycles[ 256 ];
    int arg_cycles[ 256 ];
    int arg_sizes[ 256 ];
};


cpu_t* cpu_create( mmu_t* mmu ) {
    cpu_t* cpu = (cpu_t*) malloc( sizeof( cpu_t ) );
    memset( cpu, 0, sizeof( *cpu ) );
    
    cpu->mmu = mmu;

    for( int i = 0; i < 256; ++i ) {
        cpu->base_cycles[ i ] = cpu_opinfo_base_cycles( (uint8_t) i );
        cpu->arg_cycles[ i ] = cpu_opinfo_arg_cycles( (cpu_argtype_t) i );
    }
    
    cpu->arg_sizes[ CPU_OPTYPE_NONE ] = 0;
    cpu->arg_sizes[ CPU_OPTYPE_BYTE ] = 1;
    cpu->arg_sizes[ CPU_OPTYPE_WORD ] = 2;
    cpu->arg_sizes[ CPU_OPTYPE_LONG ] = 4;
    cpu->arg_sizes[ CPU_OPTYPE_FLOAT ] = 4;
    
    return cpu;
}


void cpu_destroy( cpu_t* cpu ) {
    free( cpu );
}



void cpu_reset_cycle_acc( cpu_t* cpu ) {
    cpu->cycle_accum = 0;
}


uint32_t* cpu_ireg( cpu_t* cpu, cpu_reg_t reg ) {
    return &cpu->registers[ (uint8_t) reg ];
}


float* cpu_freg( cpu_t* cpu, cpu_reg_t reg ) {
    return (float*) ( &cpu->registers[ (uint8_t) reg ] );
}


INLINE float AS_FLOAT( uint32_t value ) { return *( (float*) &value ); }
INLINE uint32_t F_AS_UINT( float value ) { return *( (uint32_t*) &value ); }
INLINE int32_t F_AS_INT( float value ) { return *( (int32_t*) &value ); }
INLINE uint32_t AS_UINT( int32_t value ) { return *( (uint32_t*) &value ); }
INLINE int32_t AS_INT( uint32_t value ) { return *( (int32_t*) &value ); }


INLINE uint32_t addr_val( cpu_t* cpu, cpu_optype_t type, uint32_t address ) {
    switch( type ) {
        case CPU_OPTYPE_NONE:  return 0; break;
        case CPU_OPTYPE_BYTE:  return mmu_read_byte( cpu->mmu, address ); break;
        case CPU_OPTYPE_WORD:  return mmu_read_word( cpu->mmu, address ); break;
        case CPU_OPTYPE_LONG:  return mmu_read_long( cpu->mmu, address ); break;
        case CPU_OPTYPE_FLOAT: return F_AS_UINT( mmu_read_float( cpu->mmu, address ) ); break;
    }

    return 0;
}


INLINE void addr_out( cpu_t* cpu, cpu_optype_t type, uint32_t address, uint32_t value) {
    switch( type ) {
        case CPU_OPTYPE_NONE:  break;
        case CPU_OPTYPE_BYTE:  mmu_write_byte( cpu->mmu, address, (uint8_t) value ); break;
        case CPU_OPTYPE_WORD:  mmu_write_word( cpu->mmu, address, (uint8_t) value ); break;
        case CPU_OPTYPE_LONG:  mmu_write_long( cpu->mmu, address, (uint8_t) value ); break;
        case CPU_OPTYPE_FLOAT: mmu_write_float( cpu->mmu, address, AS_FLOAT( value ) ); break;
    }
}


INLINE uint32_t decode_arg( cpu_t* cpu, cpu_argtype_t type, cpu_optype_t optype, uint32_t* arg_code, uint32_t* arg_addr, uint32_t* ip ) {
    uint32_t value;
    switch( type ) {
        case CPU_ARGTYPE_NONE: return 0; break;
        case CPU_ARGTYPE_FREG: *arg_code = mmu_read_long( cpu->mmu, *ip ); value = cpu->registers[ *arg_code ]; break;
        case CPU_ARGTYPE_FLOAT: *arg_code = mmu_read_long( cpu->mmu, *ip ); value = *arg_code; break;
        case CPU_ARGTYPE_AREG: *arg_code = mmu_read_long( cpu->mmu, *ip ); value = cpu->registers[ *arg_code ]; break;
        case CPU_ARGTYPE_AT_AREG: *arg_code = mmu_read_long( cpu->mmu, *ip ); value = addr_val( cpu, optype, cpu->registers[ *arg_code ] ); break;
        case CPU_ARGTYPE_AT_AREG_INC: *arg_code = mmu_read_long( cpu->mmu, *ip ); value = addr_val( cpu, optype, cpu->registers[ *arg_code ] ); break;
        case CPU_ARGTYPE_AT_AREG_DEC: *arg_code = mmu_read_long( cpu->mmu, *ip ); value = addr_val( cpu, optype, cpu->registers[ *arg_code ] ); break;
        case CPU_ARGTYPE_DREG: *arg_code = mmu_read_long( cpu->mmu, *ip ); value = cpu->registers[ *arg_code ]; break;
        case CPU_ARGTYPE_STACK: *arg_code = mmu_read_long( cpu->mmu, *ip ); value = mmu_read_long( cpu->mmu, cpu->registers[ CPU_REG_SP ] + *arg_code * sizeof( uint32_t ) ); break; 
        case CPU_ARGTYPE_AT_STACK: *arg_code = mmu_read_long( cpu->mmu, *ip ); value = addr_val( cpu, optype, mmu_read_long( cpu->mmu, cpu->registers[ CPU_REG_SP ] + *arg_code * sizeof( uint32_t ) ) ); break;
        case CPU_ARGTYPE_INTEGER: *arg_code = mmu_read_long( cpu->mmu, *ip ); value = *arg_code;  break;
        case CPU_ARGTYPE_AT_INTEGER: *arg_code = mmu_read_long( cpu->mmu, *ip ); value = addr_val( cpu, optype, *arg_code ); break;
        case CPU_ARGTYPE_LABEL: *arg_code = mmu_read_long( cpu->mmu, *ip ); *ip += sizeof( uint32_t ); return *ip + (int32_t) *arg_code; break; 
        case CPU_ARGTYPE_AT_LABEL:  *arg_code = mmu_read_long( cpu->mmu, *ip ); *ip += sizeof( uint32_t ); *arg_addr = *ip + (int32_t) *arg_code; return addr_val( cpu, optype, *arg_addr ); break;
        default: return 0;
    }

    *ip += sizeof( uint32_t ); 
    return value;
}


INLINE void output_arg( cpu_t* cpu, cpu_argtype_t type, cpu_optype_t optype, uint32_t arg_code, uint32_t arg_addr, uint32_t value ) {
    switch( type ) {
        case CPU_ARGTYPE_AREG: cpu->registers[ arg_code ] = value; break;
        case CPU_ARGTYPE_DREG: cpu->registers[ arg_code ] = value; break;
        case CPU_ARGTYPE_FREG: cpu->registers[ arg_code ] = value; break;
        case CPU_ARGTYPE_AT_AREG: addr_out( cpu, optype, cpu->registers[ arg_code ], value ); break;
        case CPU_ARGTYPE_AT_AREG_INC: addr_out( cpu, optype, cpu->registers[ arg_code ], value ); break;
        case CPU_ARGTYPE_AT_AREG_DEC: addr_out( cpu, optype, cpu->registers[ arg_code ], value ); break;
        case CPU_ARGTYPE_AT_INTEGER: addr_out( cpu, optype, arg_code, value ); break;
        case CPU_ARGTYPE_AT_LABEL: addr_out( cpu, optype, arg_addr, value ); break;
        case CPU_ARGTYPE_NONE:
        case CPU_ARGTYPE_STACK:
        case CPU_ARGTYPE_AT_STACK:
        case CPU_ARGTYPE_INTEGER:
        case CPU_ARGTYPE_LABEL:
        case CPU_ARGTYPE_FLOAT:
        default: break;
    }
}


INLINE void push( cpu_t* cpu, uint32_t value ) {
    uint32_t* sp = &cpu->registers[ CPU_REG_SP ];
    mmu_write_long( cpu->mmu, *sp, value );
    *sp -= sizeof( uint32_t );
}


INLINE uint32_t pop( cpu_t* cpu ) {
    uint32_t* sp = &cpu->registers[ CPU_REG_SP ];
    *sp += sizeof( uint32_t );
    uint32_t value = mmu_read_long( cpu->mmu, *sp );
    return value;
}
    

INLINE int ipow( int base, int exp ) {
    int result = 1;
    while( exp ) {
        if( exp & 1 ) {
            result *= base;
        }
        exp >>= 1;
        base *= base;
    }
    return result;
}   


int do_instruction( cpu_t* cpu ) {
    #define STORE_A( x ) output_arg( cpu, argtype_a, optype, argcode_a, argaddr_a, ( x ) )
    #define STORE_B( x ) output_arg( cpu, argtype_b, optype, argcode_b, argaddr_b, ( x ) )

    uint32_t* ip = &cpu->registers[ CPU_REG_IP ];
    uint32_t* tr = &cpu->registers[ CPU_REG_TR ];

    // fetch/decode op
    uint32_t opcode = mmu_read_long( cpu->mmu, *ip );
    *ip += sizeof( uint32_t );
    uint8_t* data = (uint8_t*) &opcode;
    cpu_op_t op = (cpu_op_t) data[ 0 ];
    cpu_optype_t optype = (cpu_optype_t) data[ 1 ]; 
    cpu_argtype_t argtype_a = (cpu_argtype_t) data[ 2 ]; 
    cpu_argtype_t argtype_b = (cpu_argtype_t) data[ 3 ]; 

    #if defined( _DEBUG )
        uint32_t arg_a = 0;
        uint32_t arg_b = 0;
        uint32_t argcode_a = 0;
        uint32_t argcode_b = 0;
        uint32_t argaddr_a = 0;
        uint32_t argaddr_b = 0;
    #else
        uint32_t arg_a;
        uint32_t arg_b;
        uint32_t argcode_a;
        uint32_t argcode_b;
        uint32_t argaddr_a;
        uint32_t argaddr_b;
    #endif

    // fetch/decode arg A
    arg_a = decode_arg( cpu, argtype_a, optype, &argcode_a, &argaddr_a, ip );

    // fetch/decode arg B
    arg_b = decode_arg( cpu, argtype_b, optype, &argcode_b, &argaddr_b, ip );
    
    // execute
    switch( op ) {
        case CPU_OP_NOP: break;
        case CPU_OP_JSR: push( cpu, *ip ); *ip = arg_a; break;
        case CPU_OP_JMP: *ip = arg_a; break;
        case CPU_OP_RET: *ip = pop( cpu ); break;
        case CPU_OP_SYS: /* sys( arg_a ); */ break; // TODO: sys calls

        case CPU_OP_MOVE: STORE_B( arg_a ); break;
        case CPU_OP_XMOV: 
            for( int i = 0; i < 16; ++i ) 
                addr_out( cpu, optype, cpu->registers[ argcode_b ] + i * cpu->arg_sizes[ optype ], 
                    addr_val( cpu, optype, cpu->registers[ argcode_a ] + i * cpu->arg_sizes[ optype ] ) ); 
            break;
        case CPU_OP_ADD:  STORE_B( arg_a + arg_b ); break;
        case CPU_OP_ADDF: STORE_B( F_AS_UINT( AS_FLOAT( arg_a ) + AS_FLOAT( arg_b ) ) ); break;
        case CPU_OP_SUB:  STORE_B( arg_b - arg_a ); break;
        case CPU_OP_SUBF: STORE_B( F_AS_UINT( AS_FLOAT( arg_b ) - AS_FLOAT( arg_a ) ) ); break;
        case CPU_OP_NEG:  STORE_A( AS_UINT( - AS_INT( arg_a) ) ); break;
        case CPU_OP_NEGF: STORE_A( F_AS_UINT( - AS_FLOAT( arg_a ) ) ); break;
        case CPU_OP_MULS: STORE_B( AS_UINT( AS_INT( arg_a ) * AS_INT( arg_b ) ) ); break;
        case CPU_OP_MULU: STORE_B( arg_a * arg_b ); break;
        case CPU_OP_MUL:  STORE_B( F_AS_UINT( AS_FLOAT( arg_a ) * AS_FLOAT( arg_b ) ) ); break;
        case CPU_OP_DIVS: STORE_B( AS_UINT( AS_INT( arg_b ) / AS_INT( arg_a == 0 ? 1 : arg_a ) ) ); break;
        case CPU_OP_DIVU: STORE_B( arg_b / ( arg_a == 0 ? 1 : arg_a ) ); break;
        case CPU_OP_DIV:  STORE_B( F_AS_UINT( AS_FLOAT( arg_b ) / AS_FLOAT( arg_a ) ) ); break;
        case CPU_OP_ITOF: STORE_B( F_AS_UINT( (float) ( AS_INT( arg_a ) ) ) ); break;
        case CPU_OP_FTOI: STORE_B( AS_UINT( (int) ( AS_FLOAT( arg_a ) ) ) ); break;

        case CPU_OP_CMPU: *tr = arg_b - arg_a; break;
        case CPU_OP_CMPS: *tr = AS_UINT( AS_INT( arg_b ) - AS_INT( arg_a ) );
        case CPU_OP_CMP:  *tr = (uint32_t)( AS_FLOAT( arg_b ) < AS_FLOAT( arg_a ) ? -1 : ( AS_FLOAT( arg_b ) > AS_FLOAT( arg_a ) ? 1 : 0 ) ); break;
        case CPU_OP_TST:  *tr = arg_a; break;
        case CPU_OP_TSTF: *tr = (uint32_t)( AS_FLOAT( arg_a ) < 0 ? -1 : ( AS_FLOAT( arg_a ) > 0 ? 1 : 0 ) ); break;

        case CPU_OP_JE:  if( AS_INT( *tr ) == 0 ) { *ip = arg_a; } break;
        case CPU_OP_JNE: if( AS_INT( *tr ) != 0 ) { *ip = arg_a; } break;
        case CPU_OP_JGE: if( AS_INT( *tr ) >= 0 ) { *ip = arg_a; } break;
        case CPU_OP_JLE: if( AS_INT( *tr ) <= 0 ) { *ip = arg_a; } break;
        case CPU_OP_JG:  if( AS_INT( *tr ) >  0 ) { *ip = arg_a; } break;
        case CPU_OP_JL:  if( AS_INT( *tr ) <  0 ) { *ip = arg_a; } break;
        case CPU_OP_JZ:  if( AS_INT( *tr ) == 0 ) { *ip = arg_a; } break;
        case CPU_OP_JNZ: if( AS_INT( *tr ) != 0 ) { *ip = arg_a; } break;

        case CPU_OP_DJE:  STORE_B( arg_b - 1 ); *tr = arg_b - 1; if( AS_INT( *tr ) == 0 ) { *ip = arg_a; } break;
        case CPU_OP_DJNE: STORE_B( arg_b - 1 ); *tr = arg_b - 1; if( AS_INT( *tr ) != 0 ) { *ip = arg_a; } break;
        case CPU_OP_DJGE: STORE_B( arg_b - 1 ); *tr = arg_b - 1; if( AS_INT( *tr ) >= 0 ) { *ip = arg_a; } break;
        case CPU_OP_DJLE: STORE_B( arg_b - 1 ); *tr = arg_b - 1; if( AS_INT( *tr ) <= 0 ) { *ip = arg_a; } break;
        case CPU_OP_DJG:  STORE_B( arg_b - 1 ); *tr = arg_b - 1; if( AS_INT( *tr ) >  0 ) { *ip = arg_a; } break;
        case CPU_OP_DJL:  STORE_B( arg_b - 1 ); *tr = arg_b - 1; if( AS_INT( *tr ) <  0 ) { *ip = arg_a; } break;
        case CPU_OP_DJZ:  STORE_B( arg_b - 1 ); *tr = arg_b - 1; if( AS_INT( *tr ) == 0 ) { *ip = arg_a; } break;
        case CPU_OP_DJNZ: STORE_B( arg_b - 1 ); *tr = arg_b - 1; if( AS_INT( *tr ) != 0 ) { *ip = arg_a; } break;

        case CPU_OP_PUSH: push( cpu, arg_a ); break;
        case CPU_OP_POP:  STORE_A( pop( cpu ) ); break;

        case CPU_OP_BAND: STORE_B( arg_a & arg_b ); break;
        case CPU_OP_BOR:  STORE_B( arg_a | arg_b ); break;
        case CPU_OP_BXOR: STORE_B( arg_a ^ arg_b ); break;
        case CPU_OP_BNOT: STORE_A( ~arg_a ); break;
        case CPU_OP_BSET: STORE_B( arg_b | ( 1 << arg_a ) ); break;
        case CPU_OP_BCLR: STORE_B( arg_b | ~( 1 << arg_a ) ); break;
        case CPU_OP_BTST: *tr = arg_b & ( 1 << arg_a ); break;
        case CPU_OP_BCHG: STORE_B( arg_b ^ ( 1 << arg_a ) ); break;
        case CPU_OP_ROL:  STORE_B( arg_b << arg_a ); break;
        case CPU_OP_ROR:  STORE_B( arg_b >> arg_a ); break;

        case CPU_OP_MOD:  STORE_B( arg_a == 0 ? 0 : arg_b % arg_a ); break;
        case CPU_OP_MODF: STORE_B( F_AS_UINT( fmodf( AS_FLOAT( arg_b ), AS_FLOAT( arg_a ) ) ) ); break;
        case CPU_OP_ABS:  STORE_B( AS_UINT( abs( AS_INT( arg_a ) ) ) ); break;
        case CPU_OP_ABSF: STORE_B( F_AS_UINT( fabsf( AS_FLOAT( arg_a ) ) ) ); break;            
        case CPU_OP_POW:  STORE_B( AS_UINT( ipow( AS_INT( arg_b ), AS_INT( arg_a ) ) ) ); break;
        case CPU_OP_POWF: STORE_B( F_AS_UINT( powf( AS_FLOAT( arg_b ), AS_FLOAT( arg_a ) ) ) ); break;

        case CPU_OP_SIN:  STORE_B( F_AS_UINT( sinf( AS_FLOAT( arg_a ) ) ) ); break;
        case CPU_OP_COS:  STORE_B( F_AS_UINT( cosf( AS_FLOAT( arg_a ) ) ) ); break;
        case CPU_OP_TAN:  STORE_B( F_AS_UINT( tanf( AS_FLOAT( arg_a ) ) ) ); break;
        case CPU_OP_ASIN: STORE_B( F_AS_UINT( asinf( AS_FLOAT( arg_a ) ) ) ); break;
        case CPU_OP_ACOS: STORE_B( F_AS_UINT( acosf( AS_FLOAT( arg_a ) ) ) ); break;
        case CPU_OP_ATAN: STORE_B( F_AS_UINT( atanf( AS_FLOAT( arg_a ) ) ) ); break;
        case CPU_OP_EXP:  STORE_B( F_AS_UINT( expf( AS_FLOAT( arg_a ) ) ) ); break;
        case CPU_OP_LOG:  STORE_B( F_AS_UINT( logf( AS_FLOAT( arg_a ) ) ) ); break;
        case CPU_OP_SQRT: STORE_B( F_AS_UINT( sqrtf( AS_FLOAT( arg_a ) ) ) ); break;

        default: return 4; // Default cycle count for unknown ops
    }

    int m = ( op == CPU_OP_XMOV ? 16 : 1 );

    if( argtype_a == CPU_ARGTYPE_AT_AREG_INC ) cpu->registers[ argcode_a ] += m * cpu->arg_sizes[ optype ];
    if( argtype_b == CPU_ARGTYPE_AT_AREG_INC ) cpu->registers[ argcode_b ] += m * cpu->arg_sizes[ optype ];
    if( argtype_a == CPU_ARGTYPE_AT_AREG_DEC ) cpu->registers[ argcode_a ] -= m * cpu->arg_sizes[ optype ];
    if( argtype_b == CPU_ARGTYPE_AT_AREG_DEC ) cpu->registers[ argcode_b ] -= m * cpu->arg_sizes[ optype ];

    return cpu->base_cycles[ op ] + cpu->arg_cycles[ argtype_a ] + cpu->arg_cycles[ argtype_b ];
}


void cpu_fetch_decode_execute( cpu_t* cpu, int cycles ) {   
    while( cpu->cycle_accum < cycles ) {
        cpu->cycle_accum += ( do_instruction( cpu ) + 1 ) & ~1 ;
    }
    cpu->cycle_accum -= cycles;
}


#endif /* SVGS_IMPLEMENTATION */