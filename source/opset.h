#ifndef opset_h
#define opset_h


typedef enum cpu_op_t {
	CPU_OP_NOP, CPU_OP_JSR, CPU_OP_JMP, CPU_OP_RET,	CPU_OP_SYS,
	CPU_OP_MOVE, CPU_OP_XMOV, CPU_OP_ADD, CPU_OP_ADDF, CPU_OP_SUB, CPU_OP_SUBF, CPU_OP_NEG, CPU_OP_NEGF, 
	CPU_OP_MULS, CPU_OP_MULU, CPU_OP_MUL, CPU_OP_DIVS, CPU_OP_DIVU, CPU_OP_DIV, CPU_OP_ITOF, CPU_OP_FTOI,
	CPU_OP_CMPU, CPU_OP_CMPS, CPU_OP_CMP, CPU_OP_TST, CPU_OP_TSTF, 
	CPU_OP_JE, CPU_OP_JNE, CPU_OP_JGE, CPU_OP_JLE, CPU_OP_JG, CPU_OP_JL, CPU_OP_JZ, CPU_OP_JNZ, 
	CPU_OP_DJE, CPU_OP_DJNE, CPU_OP_DJGE, CPU_OP_DJLE, CPU_OP_DJG, CPU_OP_DJL, CPU_OP_DJZ, CPU_OP_DJNZ, 
	CPU_OP_PUSH, CPU_OP_POP,
	CPU_OP_BAND, CPU_OP_BOR, CPU_OP_BXOR, CPU_OP_BNOT, CPU_OP_BSET, CPU_OP_BCLR, CPU_OP_BTST, CPU_OP_BCHG, CPU_OP_ROL, CPU_OP_ROR,
	CPU_OP_MOD, CPU_OP_MODF, CPU_OP_ABS, CPU_OP_ABSF, CPU_OP_POW, CPU_OP_POWF,
	CPU_OP_SIN, CPU_OP_COS, CPU_OP_TAN, CPU_OP_ASIN, CPU_OP_ACOS, CPU_OP_ATAN, CPU_OP_EXP, CPU_OP_LOG, CPU_OP_SQRT,
} cpu_op_t;


typedef enum cpu_optype_t {
	CPU_OPTYPE_NONE, CPU_OPTYPE_BYTE, CPU_OPTYPE_WORD, CPU_OPTYPE_LONG, CPU_OPTYPE_FLOAT,
} cpu_optype_t;


typedef enum cpu_argtype_t {
	CPU_ARGTYPE_NONE, CPU_ARGTYPE_AREG, CPU_ARGTYPE_AT_AREG, CPU_ARGTYPE_AT_AREG_INC, CPU_ARGTYPE_AT_AREG_DEC, CPU_ARGTYPE_DREG, CPU_ARGTYPE_STACK, 
	CPU_ARGTYPE_AT_STACK, CPU_ARGTYPE_INTEGER, CPU_ARGTYPE_AT_INTEGER, CPU_ARGTYPE_LABEL, CPU_ARGTYPE_AT_LABEL, CPU_ARGTYPE_FREG, CPU_ARGTYPE_FLOAT,
} cpu_argtype_t;


const char* cpu_opinfo_mnemonic( uint8_t i );
enum cpu_op_t cpu_opinfo_operation( uint8_t i );
bool cpu_opinfo_optype( uint8_t i, enum cpu_optype_t type );
bool cpu_opinfo_argtype_a( uint8_t i, enum cpu_argtype_t type );
bool cpu_opinfo_argtype_b( uint8_t i, enum cpu_argtype_t type );
int cpu_opinfo_base_cycles( uint8_t i );
int cpu_opinfo_arg_cycles( enum cpu_argtype_t type );

void cpu_gen_docs( char const* filename );


#endif /* opset_h */


#ifdef SVGS_IMPLEMENTATION

#define FOP_NONE		( ( 1ULL << CPU_OPTYPE_NONE ) )
#define FOP_BYTE		( ( 1ULL << CPU_OPTYPE_BYTE ) )
#define FOP_WORD		( ( 1ULL << CPU_OPTYPE_WORD ) )
#define FOP_LONG		( ( 1ULL << CPU_OPTYPE_LONG ) )
#define FOP_FLOAT	    ( ( 1ULL << CPU_OPTYPE_FLOAT ) )
#define FOP_INTEGER	    ( FOP_BYTE | FOP_WORD | FOP_LONG)
#define FOP_ANY		    ( FOP_BYTE | FOP_WORD | FOP_LONG | FOP_FLOAT)

#define FAA_NONE		( ( 1ULL << ( CPU_OPTYPE_FLOAT + 1ULL + CPU_ARGTYPE_NONE ) ) )
#define FAA_AREG		( ( 1ULL << ( CPU_OPTYPE_FLOAT + 1ULL + CPU_ARGTYPE_AREG ) ) )
#define FAA_AT_AREG		( ( 1ULL << ( CPU_OPTYPE_FLOAT + 1ULL + CPU_ARGTYPE_AT_AREG ) ) )
#define FAA_AT_AREG_INC	( ( 1ULL << ( CPU_OPTYPE_FLOAT + 1ULL + CPU_ARGTYPE_AT_AREG_INC ) ) )
#define FAA_AT_AREG_DEC	( ( 1ULL << ( CPU_OPTYPE_FLOAT + 1ULL + CPU_ARGTYPE_AT_AREG_DEC ) ) )
#define FAA_DREG		( ( 1ULL << ( CPU_OPTYPE_FLOAT + 1ULL + CPU_ARGTYPE_DREG ) ) )
#define FAA_STACK		( ( 1ULL << ( CPU_OPTYPE_FLOAT + 1ULL + CPU_ARGTYPE_STACK ) ) )
#define FAA_AT_STACK	( ( 1ULL << ( CPU_OPTYPE_FLOAT + 1ULL + CPU_ARGTYPE_AT_STACK ) ) )
#define FAA_INTEGER		( ( 1ULL << ( CPU_OPTYPE_FLOAT + 1ULL + CPU_ARGTYPE_INTEGER ) ) )
#define FAA_AT_INTEGER	( ( 1ULL << ( CPU_OPTYPE_FLOAT + 1ULL + CPU_ARGTYPE_AT_INTEGER ) ) )
#define FAA_LABEL		( ( 1ULL << ( CPU_OPTYPE_FLOAT + 1ULL + CPU_ARGTYPE_LABEL ) ) )
#define FAA_AT_LABEL	( ( 1ULL << ( CPU_OPTYPE_FLOAT + 1ULL + CPU_ARGTYPE_AT_LABEL ) ) )
#define FAA_FREG		( ( 1ULL << ( CPU_OPTYPE_FLOAT + 1ULL + CPU_ARGTYPE_FREG ) ) )
#define FAA_FLOAT		( ( 1ULL << ( CPU_OPTYPE_FLOAT + 1ULL + CPU_ARGTYPE_FLOAT ) ) )
#define FAA_INT_LVAL	( FAA_AREG | FAA_AT_AREG | FAA_AT_AREG_INC  | FAA_AT_AREG_DEC | FAA_DREG | FAA_STACK | FAA_AT_STACK | FAA_AT_INTEGER | FAA_AT_LABEL)
#define FAA_INT_RVAL	( FAA_INT_LVAL | FAA_INTEGER | FAA_LABEL)
#define FAA_FLT_LVAL	( FAA_AT_AREG | FAA_STACK | FAA_AT_STACK | FAA_AT_INTEGER | FAA_AT_LABEL | FAA_FREG)
#define FAA_FLT_RVAL	( FAA_FLT_LVAL | FAA_FLOAT)
#define FAA_ANY_LVAL	( FAA_INT_LVAL | FAA_FLT_LVAL)
#define FAA_ANY_RVAL	( FAA_INT_RVAL | FAA_FLT_RVAL)
#define FAA_ANYATREG	( FAA_AT_AREG | FAA_AT_AREG_INC | FAA_AT_AREG_DEC)

#define FAB_NONE		( ( 1ULL << ( CPU_OPTYPE_FLOAT + 1ULL + CPU_ARGTYPE_FLOAT + 1ULL + CPU_ARGTYPE_NONE ) ) )
#define FAB_AREG		( ( 1ULL << ( CPU_OPTYPE_FLOAT + 1ULL + CPU_ARGTYPE_FLOAT + 1ULL + CPU_ARGTYPE_AREG ) ) )
#define FAB_AT_AREG		( ( 1ULL << ( CPU_OPTYPE_FLOAT + 1ULL + CPU_ARGTYPE_FLOAT + 1ULL + CPU_ARGTYPE_AT_AREG ) ) )
#define FAB_AT_AREG_INC	( ( 1ULL << ( CPU_OPTYPE_FLOAT + 1ULL + CPU_ARGTYPE_FLOAT + 1ULL + CPU_ARGTYPE_AT_AREG_INC ) ) )
#define FAB_AT_AREG_DEC	( ( 1ULL << ( CPU_OPTYPE_FLOAT + 1ULL + CPU_ARGTYPE_FLOAT + 1ULL + CPU_ARGTYPE_AT_AREG_DEC ) ) )
#define FAB_DREG		( ( 1ULL << ( CPU_OPTYPE_FLOAT + 1ULL + CPU_ARGTYPE_FLOAT + 1ULL + CPU_ARGTYPE_DREG ) ) )
#define FAB_STACK		( ( 1ULL << ( CPU_OPTYPE_FLOAT + 1ULL + CPU_ARGTYPE_FLOAT + 1ULL + CPU_ARGTYPE_STACK ) ) )
#define FAB_AT_STACK	( ( 1ULL << ( CPU_OPTYPE_FLOAT + 1ULL + CPU_ARGTYPE_FLOAT + 1ULL + CPU_ARGTYPE_AT_STACK ) ) )
#define FAB_INTEGER		( ( 1ULL << ( CPU_OPTYPE_FLOAT + 1ULL + CPU_ARGTYPE_FLOAT + 1ULL + CPU_ARGTYPE_INTEGER ) ) )
#define FAB_AT_INTEGER	( ( 1ULL << ( CPU_OPTYPE_FLOAT + 1ULL + CPU_ARGTYPE_FLOAT + 1ULL + CPU_ARGTYPE_AT_INTEGER ) ) )
#define FAB_LABEL		( ( 1ULL << ( CPU_OPTYPE_FLOAT + 1ULL + CPU_ARGTYPE_FLOAT + 1ULL + CPU_ARGTYPE_LABEL ) ) )
#define FAB_AT_LABEL	( ( 1ULL << ( CPU_OPTYPE_FLOAT + 1ULL + CPU_ARGTYPE_FLOAT + 1ULL + CPU_ARGTYPE_AT_LABEL ) ) )
#define FAB_FREG		( ( 1ULL << ( CPU_OPTYPE_FLOAT + 1ULL + CPU_ARGTYPE_FLOAT + 1ULL + CPU_ARGTYPE_FREG ) ) )
#define FAB_FLOAT		( ( 1ULL << ( CPU_OPTYPE_FLOAT + 1ULL + CPU_ARGTYPE_FLOAT + 1ULL + CPU_ARGTYPE_FLOAT ) ) )
#define FAB_INT_LVAL	( FAB_AREG | FAB_AT_AREG | FAB_AT_AREG_INC | FAB_AT_AREG_DEC | FAB_DREG | FAB_STACK | FAB_AT_STACK | FAB_AT_INTEGER | FAB_AT_LABEL)
#define FAB_INT_RVAL	( FAB_INT_LVAL | FAB_INTEGER | FAB_LABEL)
#define FAB_FLT_LVAL	( FAB_AT_AREG | FAB_STACK | FAB_AT_STACK | FAB_AT_INTEGER | FAB_AT_LABEL | FAB_FREG)
#define FAB_FLT_RVAL	( FAB_FLT_LVAL | FAB_FLOAT)
#define FAB_ANY_LVAL	( FAB_INT_LVAL | FAB_FLT_LVAL)
#define FAB_ANY_RVAL	( FAB_INT_RVAL | FAB_FLT_RVAL)
#define FAB_ANYATREG	( FAB_AT_AREG | FAB_AT_AREG_INC | FAB_AT_AREG_DEC)


typedef struct cpu_opinfo_t {
	cpu_op_t op;
	int base_cycles;
	uint64_t valid_types;
	const char* mnemonic;
	const char* description;
} cpu_opinfo_t;
			 

static cpu_opinfo_t opinfo[ 256 ] = {
	{ CPU_OP_NOP,   2, FOP_NONE | FAA_NONE		| FAB_NONE, "nop", "No operation" },
	{ CPU_OP_JSR,   4, FOP_NONE | FAA_INT_RVAL	| FAB_NONE, "jsr", "Jump to subroutine at location specified by argA. Will push the current value of IP onto the stack before the jump." },
	{ CPU_OP_JMP,   4, FOP_NONE | FAA_INT_RVAL	| FAB_NONE, "jmp", "Jump to location specified by argA." },
	{ CPU_OP_RET,   4, FOP_NONE | FAA_NONE		| FAB_NONE, "ret", "Return from subroutine call made by jsr. Will pop the return address off the stack." },
	{ CPU_OP_SYS,  32, FOP_NONE | FAA_INT_RVAL	| FAB_NONE, "sys", "Make a BIOS call. See the BIOS documentation for available BIOS calls." },

	{ CPU_OP_MOVE,  2, FOP_ANY		| FAA_ANY_RVAL | FAB_ANY_LVAL, "move", "Move the value specified by argA to the location specified by argB." },
	{ CPU_OP_XMOV,  8, FOP_ANY		| FAA_ANYATREG | FAB_ANYATREG, "xmov", "Move the value specified by argA to the location specified by argB. This will be repeated 16 times." },
	{ CPU_OP_ADD,   4, FOP_INTEGER	| FAA_INT_RVAL | FAB_INT_LVAL, "add",  "Add the value specified by argA to the value at the location specified by argB, and store the result at the location specified by argB." },
	{ CPU_OP_ADDF,  6, FOP_FLOAT	| FAA_FLT_RVAL | FAB_FLT_LVAL, "add",  "Add the value specified by argA to the value at the location specified by argB, and store the result at the location specified by argB." },
	{ CPU_OP_SUB,   4, FOP_INTEGER	| FAA_INT_RVAL | FAB_INT_LVAL, "sub",  "Subtract the value specified by argA from the value at the location specified by argB, and store the result at the location specified by argB." },
	{ CPU_OP_SUBF,  6, FOP_FLOAT	| FAA_FLT_RVAL | FAB_FLT_LVAL, "sub",  "Subtract the value specified by argA from the value at the location specified by argB, and store the result at the location specified by argB." },
	{ CPU_OP_NEG,   2, FOP_INTEGER	| FAA_INT_LVAL | FAB_NONE,	   "neg",  "Negate the value at the location specified by argA" },
	{ CPU_OP_NEGF,  2, FOP_FLOAT	| FAA_FLT_LVAL | FAB_NONE,     "neg",  "Negate the value at the location specified by argA" },
	{ CPU_OP_MULS,  6, FOP_INTEGER  | FAA_INT_RVAL | FAB_INT_LVAL, "muls", "Multiply the value specified by argA and the value at the location specified by argB, and store the result at the location specified by argB. Treat argA and argB as signed integer numbers." },
	{ CPU_OP_MULU,  6, FOP_INTEGER  | FAA_INT_RVAL | FAB_INT_LVAL, "mulu", "Multiply the value specified by argA and the value at the location specified by argB, and store the result at the location specified by argB. Treat argA and argB as unsigned integer numbers." },
	{ CPU_OP_MUL,   8, FOP_FLOAT	| FAA_FLT_RVAL | FAB_FLT_LVAL, "mul",  "Multiply the value specified by argA and the value at the location specified by argB, and store the result at the location specified by argB. Treat argA and argB as floating point numbers." },
	{ CPU_OP_DIVS,  8, FOP_INTEGER  | FAA_INT_RVAL | FAB_INT_LVAL, "divs", "Divide the value at the location specified by argB by the value specified by argA, and store the result at the location specified by argB. Treat argA and argB as signed integer numbers." },
	{ CPU_OP_DIVU,  8, FOP_INTEGER  | FAA_INT_RVAL | FAB_INT_LVAL, "divu", "Divide the value at the location specified by argB by the value specified by argA, and store the result at the location specified by argB. Treat argA and argB as unsigned integer numbers." },
	{ CPU_OP_DIV,  10, FOP_FLOAT	| FAA_FLT_RVAL | FAB_FLT_LVAL, "div",  "Divide the value at the location specified by argB by the value specified by argA, and store the result at the location specified by argB. Treat argA and argB as floating point numbers." },
	{ CPU_OP_ITOF,  2, FOP_INTEGER  | FAA_INT_RVAL | FAB_FLT_LVAL, "itof", "Convert the value specified by argA from an integer value into an floating point value, and store the result at the location specified by argB." },
	{ CPU_OP_FTOI,  2, FOP_INTEGER  | FAA_FLT_RVAL | FAB_INT_LVAL, "ftoi", "Convert the value specified by argA from a floating point value into an integer value, and store the result at the location specified by argB." },
			
	{ CPU_OP_CMPU,  2, FOP_INTEGER  | FAA_INT_RVAL | FAB_INT_RVAL, "cmpu", "Compare the value specified by argA with the value specified by argB. Treat argA and argB as unsigned integers. The result is used in the next call to je, jne, jge, jle, jg, or jl." },
	{ CPU_OP_CMPS,  2, FOP_INTEGER  | FAA_INT_RVAL | FAB_INT_RVAL, "cmps", "Compare the value specified by argA with the value specified by argB. Treat argA and argB as signed integers. The result is used in the next call to je, jne, jge, jle, jg, or jl." },
	{ CPU_OP_CMP,   2, FOP_FLOAT	| FAA_FLT_RVAL | FAB_FLT_RVAL, "cmp",  "Compare the value specified by argA with the value specified by argB. Treat argA and argB as floating point numbers. The result is used in the next call to je, jne, jge, jle, jg, or jl." },
	{ CPU_OP_TST,   2, FOP_INTEGER  | FAA_INT_RVAL | FAB_NONE,	   "tst",  "Test if value specified by argA is zero. The result is used in the next call to jz or jnz." },
	{ CPU_OP_TSTF,  2, FOP_FLOAT	| FAA_FLT_RVAL | FAB_NONE,     "tstf", "Test if value specified by argA is zero. The result is used in the next call to jz or jnz." },

	{ CPU_OP_JE,    4, FOP_NONE	| FAA_INT_RVAL | FAB_NONE,	   "je",   "Jump to location specified by argA, if in the last compare (cmpu, cmps or cmp), argA was equal to argB." },
	{ CPU_OP_JNE,   4, FOP_NONE	| FAA_INT_RVAL | FAB_NONE,	   "jne",  "Jump to location specified by argA, if in the last compare (cmpu, cmps or cmp), argA was not equal to argB." },
	{ CPU_OP_JGE,   4, FOP_NONE	| FAA_INT_RVAL | FAB_NONE,     "jge",  "Jump to location specified by argA, if in the last compare (cmpu, cmps or cmp), argA was greater than or equal to argB." },
	{ CPU_OP_JLE,   4, FOP_NONE	| FAA_INT_RVAL | FAB_NONE,     "jle",  "Jump to location specified by argA, if in the last compare (cmpu, cmps or cmp), argA was less than or equal to argB." },
	{ CPU_OP_JG,	4, FOP_NONE	| FAA_INT_RVAL | FAB_NONE,     "jg",   "Jump to location specified by argA, if in the last compare (cmpu, cmps or cmp), argA was greater than argB." },
	{ CPU_OP_JL,	4, FOP_NONE	| FAA_INT_RVAL | FAB_NONE,     "jl",   "Jump to location specified by argA, if in the last compare (cmpu, cmps or cmp), argA was less than argB." },
	{ CPU_OP_JZ,	4, FOP_NONE	| FAA_INT_RVAL | FAB_NONE,     "jz",   "Jump to location specified by argA, if in the last test (tst or btst), the result was zero." },
	{ CPU_OP_JNZ,   4, FOP_NONE	| FAA_INT_RVAL | FAB_NONE,     "jnz",  "Jump to location specified by argA, if in the last test (tst or btst), the result was non-zero" },
			 	  
	{ CPU_OP_DJE,	4, FOP_NONE	| FAA_INT_RVAL | FAB_INT_LVAL, "dje",  "Decrement argB, compare it to zero, and jump to location specified by argA, if argB was equal to zero." },
	{ CPU_OP_DJNE,  4, FOP_NONE	| FAA_INT_RVAL | FAB_INT_LVAL, "djne", "Decrement argB, compare it to zero, and jump to location specified by argA, if argB was not equal to zero." },
	{ CPU_OP_DJGE,  4, FOP_NONE	| FAA_INT_RVAL | FAB_INT_LVAL, "djge", "Decrement argB, compare it to zero, and jump to location specified by argA, if argB was greater than or equal to zero." },
	{ CPU_OP_DJLE,  4, FOP_NONE	| FAA_INT_RVAL | FAB_INT_LVAL, "djle", "Decrement argB, compare it to zero, and jump to location specified by argA, if argB was less than or equal to zero." },
	{ CPU_OP_DJG,	4, FOP_NONE	| FAA_INT_RVAL | FAB_INT_LVAL, "djg",  "Decrement argB, compare it to zero, and jump to location specified by argA, if argB was greater than zero." },
	{ CPU_OP_DJL,	4, FOP_NONE	| FAA_INT_RVAL | FAB_INT_LVAL, "djl",  "Decrement argB, compare it to zero, and jump to location specified by argA, if argB was less than zero." },
	{ CPU_OP_DJZ,	4, FOP_NONE	| FAA_INT_RVAL | FAB_INT_LVAL, "djz",  "Decrement argB, compare it to zero, and jump to location specified by argA, if argB was zero." },
	{ CPU_OP_DJNZ,  4, FOP_NONE	| FAA_INT_RVAL | FAB_INT_LVAL, "djnz", "Decrement argB, compare it to zero, and jump to location specified by argA, if argB was non-zero" },

	{ CPU_OP_PUSH,  4, FOP_ANY | FAA_ANY_RVAL | FAB_NONE, "push", "Push the value specified by argA onto the stack." },
	{ CPU_OP_POP,   4, FOP_ANY | FAA_ANY_LVAL | FAB_NONE, "pop",  "Pop a value off the stack, and store it at the location specified by argA." },
			
	{ CPU_OP_BAND,  2, FOP_INTEGER | FAA_INT_RVAL | FAB_INT_LVAL, "and",  "Perform a bitwise AND on the value specified by argA and the value at the location specified by argB, and store the result at the location specified by argB." },
	{ CPU_OP_BOR,	2, FOP_INTEGER | FAA_INT_RVAL | FAB_INT_LVAL, "or",   "Perform a bitwise OR on the value specified by argA and the value at the location specified by argB, and store the result at the location specified by argB." },
	{ CPU_OP_BXOR,  2, FOP_INTEGER | FAA_INT_RVAL | FAB_INT_LVAL, "xor",  "Perform a bitwise XOR on the value specified by argA and the value at the location specified by argB, and store the result at the location specified by argB." },
	{ CPU_OP_BNOT,  2, FOP_INTEGER | FAA_INT_LVAL | FAB_NONE,	   "not",  "Perform a bitwise NOT on the value at the location specified by argA." },
	{ CPU_OP_BSET,  2, FOP_INTEGER | FAA_INT_RVAL | FAB_INT_LVAL, "bset", "Set the bit at bit index specified by argA, in the value at the location specified by argB. Bit indices start at 0 and increase with significance." },
	{ CPU_OP_BCLR,  2, FOP_INTEGER | FAA_INT_RVAL | FAB_INT_LVAL, "bclr", "Clear the bit at bit index specified by argA, in the value at the location specified by argB. Bit indices start at 0 and increase with significance." },
	{ CPU_OP_BTST,  2, FOP_INTEGER | FAA_INT_RVAL | FAB_INT_LVAL, "btst", "Test the bit at bit index specified by argA, in the value at the location specified by argB. Bit indices start at 0 and increase with significance. The result is used in the next call to jz or jnz." },
	{ CPU_OP_BCHG,  2, FOP_INTEGER | FAA_INT_RVAL | FAB_INT_LVAL, "bchg", "Flip the bit at bit index specified by argA, in the value at the location specified by argB. Bit indices start at 0 and increase with significance." },
	{ CPU_OP_ROL,   2, FOP_INTEGER | FAA_INT_RVAL | FAB_INT_LVAL, "rol",  "Perform a bitwise left shift of the value at the location specified by argB. Number of bits to shift is the value specified by argA." },
	{ CPU_OP_ROR,   2, FOP_INTEGER | FAA_INT_RVAL | FAB_INT_LVAL, "ror",  "Perform a bitwise right shift of the value at the location specified by argB. Number of bits to shift is the value specified by argA." },
			
	{ CPU_OP_MOD,   4, FOP_INTEGER  | FAA_INT_RVAL | FAB_ANY_LVAL, "mod", "Perform MOD on argB by argA and store the result at the location specified by argB." },
	{ CPU_OP_MODF,  8, FOP_FLOAT	| FAA_FLT_RVAL | FAB_FLT_LVAL, "mod", "Perform MOD on argB by argA and store the result at the location specified by argB." },
	{ CPU_OP_ABS,   4, FOP_INTEGER  | FAA_INT_RVAL | FAB_ANY_LVAL, "abs", "Perform ABS on argA and store the result at the location specified by argB." },
	{ CPU_OP_ABSF,  8, FOP_FLOAT	| FAA_FLT_RVAL | FAB_FLT_LVAL, "abs", "Perform ABS on argA and store the result at the location specified by argB." },
	{ CPU_OP_POW,   4, FOP_INTEGER  | FAA_INT_RVAL | FAB_ANY_LVAL, "pow", "Perform POW on argB by argA and store the result at the location specified by argB." },
	{ CPU_OP_POWF,  8, FOP_FLOAT	| FAA_FLT_RVAL | FAB_FLT_LVAL, "pow", "Perform POW on argB by argA and store the result at the location specified by argB." },
			
	{ CPU_OP_SIN,   8, FOP_FLOAT	| FAA_FLT_RVAL | FAB_FLT_LVAL, "sin",  "Calculate SIN of the value specified by argA, and store the result at the location specified by argB." },
	{ CPU_OP_COS,   8, FOP_FLOAT	| FAA_FLT_RVAL | FAB_FLT_LVAL, "cos",  "Calculate COS of the value specified by argA, and store the result at the location specified by argB." },
	{ CPU_OP_TAN,   8, FOP_FLOAT	| FAA_FLT_RVAL | FAB_FLT_LVAL, "tan",  "Calculate TAN of the value specified by argA, and store the result at the location specified by argB." },
	{ CPU_OP_ASIN,  8, FOP_FLOAT	| FAA_FLT_RVAL | FAB_FLT_LVAL, "asin", "Calculate ASIN of the value specified by argA, and store the result at the location specified by argB." },
	{ CPU_OP_ACOS,  8, FOP_FLOAT	| FAA_FLT_RVAL | FAB_FLT_LVAL, "acos", "Calculate ACOS of the value specified by argA, and store the result at the location specified by argB." },
	{ CPU_OP_ATAN,  8, FOP_FLOAT	| FAA_FLT_RVAL | FAB_FLT_LVAL, "atan", "Calculate ATAN of the value specified by argA, and store the result at the location specified by argB." },
	{ CPU_OP_EXP,   8, FOP_FLOAT	| FAA_FLT_RVAL | FAB_FLT_LVAL, "exp",  "Calculate EXP of the value specified by argA, and store the result at the location specified by argB." },
	{ CPU_OP_LOG,   8, FOP_FLOAT	| FAA_FLT_RVAL | FAB_FLT_LVAL, "log",  "Calculate LOG of the value specified by argA, and store the result at the location specified by argB." },
	{ CPU_OP_SQRT,  8, FOP_FLOAT	| FAA_FLT_RVAL | FAB_FLT_LVAL, "sqrt", "Calculate SQRT of the value specified by argA, and store the result at the location specified by argB." },
};	  


static int arg_cycles[ 256 ] = {
	0, // NONE,
	0, // AREG, 
	8, // AT_AREG 
	8, // AT_AREG_INC
	8, // AT_AREG_DEC 
	0, // DREG 
	4, // STACK, 
	12, // AT_STACK, 
	4, // INTEGER, 
	16, // AT_INTEGER, 
	4, // LABEL, 
	16, // AT_LABEL,	
	0, // FREG, 
	4, // FLOAT,
};


const char* cpu_opinfo_mnemonic( uint8_t i ) {
	return opinfo[ i ].mnemonic;
}


cpu_op_t cpu_opinfo_operation( uint8_t i ) {
	return opinfo[ i ].op;
}


bool cpu_opinfo_optype( uint8_t i, cpu_optype_t type ) {
	return ( opinfo[ i ].valid_types & ( 1ULL << type ) ) != 0;
}


bool cpu_opinfo_argtype_a( uint8_t i, cpu_argtype_t type ) {
	return ( opinfo[ i ].valid_types & ( 1ULL << ( CPU_OPTYPE_FLOAT + 1ULL + type ) ) ) != 0;
}


bool cpu_opinfo_argtype_b( uint8_t i, cpu_argtype_t type ) {
	return ( opinfo[ i ].valid_types & ( 1ULL << ( CPU_OPTYPE_FLOAT + 1ULL + CPU_ARGTYPE_FLOAT + 1ULL + type ) ) ) != 0;
}


int cpu_opinfo_base_cycles( uint8_t i ) {
	return opinfo[ i ].base_cycles;
}


int cpu_opinfo_arg_cycles( cpu_argtype_t type ) {
	return arg_cycles[ (uint8_t) type ];
}


void cpu_gen_docs( char const* filename ) {
	
    FILE* fp = fopen( filename, "w" );	
	fprintf( fp,"The SVGS CPU Instruction Set\n=========================\nHere's a list of all the available instructions for the Super VGS CPU, their syntax, effect, what datatypes and operands they work on, and their cycle counts.\n" );
			
	int current_index = 0;
	int start_index = 0;
	int instruction_count = sizeof( opinfo ) / sizeof( *opinfo );
	while( current_index <= instruction_count ) {
		if( opinfo[ start_index ].op != opinfo[ current_index ].op ) {
			char const* mnemonic = cpu_opinfo_mnemonic( (uint8_t) start_index );
			fprintf( fp,"\n%s\n", mnemonic );
			for( int i = 0; i < (int) strlen( mnemonic ); ++i ) {
				fprintf( fp,"-" );
            }
			fprintf( fp,"\n" );

			char typestring[ 256 ] = "";
			bool hasByte = false;
			bool hasWord = false;
			bool hasLong = false;
			bool hasFloat = false;
			for( int i = start_index; i < current_index; ++i ) {
				if( cpu_opinfo_optype( (uint8_t) i, CPU_OPTYPE_BYTE ) ) {
					hasByte = true;
				}
				if( cpu_opinfo_optype( (uint8_t) i, CPU_OPTYPE_WORD ) ) {
					hasWord = true;
				}
				if( cpu_opinfo_optype( (uint8_t) i, CPU_OPTYPE_LONG ) ) {
					hasLong=true;
				}
				if( cpu_opinfo_optype( (uint8_t) i, CPU_OPTYPE_FLOAT ) ) {
					hasFloat=true;
				}
			}
			
            bool hasType = hasByte | hasWord | hasLong | hasFloat;
			if( hasType ) {
				if( !( hasFloat && !hasByte && !hasWord && !hasLong ) ) {
					strcat( typestring, "<" );
				}
				if( hasByte ) {
					strcat( typestring, ".b" );
					if( hasWord || hasLong || hasFloat ) {
						strcat(typestring,"/");
					}
				}
				if( hasWord ) {
					strcat( typestring, ".w" );
					if( hasLong || hasFloat ) {
						strcat( typestring, "/" );
					}
				}
				if( hasLong ) {
					strcat( typestring, ".l" );
					if( hasFloat ) {
						strcat( typestring, "/" );
					}
				}
				if( hasFloat ) {
					strcat( typestring, ".f" );
				}
				if( !( hasFloat && !hasByte && !hasWord && !hasLong ) ) {
					strcat( typestring, ">" );
				}
			}					
			
			char* operandstring = "";
			if( !cpu_opinfo_argtype_a( (uint8_t) start_index, CPU_ARGTYPE_NONE ) && cpu_opinfo_argtype_b( (uint8_t) start_index, CPU_ARGTYPE_NONE ) ) {
				operandstring = " argA";
			} else if( !cpu_opinfo_argtype_a( (uint8_t) start_index, CPU_ARGTYPE_NONE ) && !cpu_opinfo_argtype_b( (uint8_t) start_index, CPU_ARGTYPE_NONE ) ) {
				operandstring = " argA, argB";
			}

			fprintf( fp, "\n### Syntax\n\t%s%s%s\n", mnemonic, typestring, operandstring );

			fprintf( fp, "\n### Function\n%s\n", opinfo[ start_index ].description );

			char* ops[] = { "None", "areg", "@areg", "@areg+", "@areg-", "dreg", "sp(ofs)", "@sp(ofs)", "ival", "@ival", "label", "@label", "freg", "fval", };
			fprintf( fp, "\n### Operand Types and Cycle Counts\n");
			if( !cpu_opinfo_argtype_a( (uint8_t) start_index, CPU_ARGTYPE_NONE ) && !cpu_opinfo_argtype_b( (uint8_t) start_index, CPU_ARGTYPE_NONE ) ) {
				fprintf( fp, "\n|**argA**/**argB**|" );
				for( int b = CPU_ARGTYPE_AREG; b <= CPU_ARGTYPE_FLOAT; ++b ) {
					fprintf( fp, " %-8s|", ops[ b ] );
                }
				fprintf( fp, "\n|-----------------|" );
				for( int b = CPU_ARGTYPE_AREG; b <= CPU_ARGTYPE_FLOAT; ++b ) {
					fprintf( fp, "---------|");
                }
				fprintf( fp, "\n");
				for( int a = CPU_ARGTYPE_AREG; a <= CPU_ARGTYPE_FLOAT; ++a ) {                    
					fprintf( fp, "|%-16s |", ops[ a ] );
					for( int b = CPU_ARGTYPE_AREG; b<= CPU_ARGTYPE_FLOAT; ++b ) {
						if( !cpu_opinfo_argtype_a( (uint8_t) start_index, (cpu_argtype_t) a ) || !cpu_opinfo_argtype_b( (uint8_t) start_index, (cpu_argtype_t) b ) ) {
							fprintf( fp, " -       |" );
						} else {
							int cycles = opinfo[ start_index ].base_cycles + arg_cycles[ a ] + arg_cycles[ b ];
    						cycles = ( cycles + 1 ) & ~1;
							fprintf( fp, " %-8d|", cycles );
						}
					}
					fprintf( fp, "\n");
				}
				fprintf( fp, "\n" );
			} else if( !cpu_opinfo_argtype_a( (uint8_t) start_index, CPU_ARGTYPE_NONE ) )
				{
				fprintf( fp, "\nargA     | cycles\n" );
				fprintf( fp, "---------|-------\n" );

				for( int a = CPU_ARGTYPE_AREG; a <= CPU_ARGTYPE_FLOAT; ++a ) {
					fprintf( fp, "%-8s | ", ops[ a ] );
					if( !cpu_opinfo_argtype_a( (uint8_t) start_index, (cpu_argtype_t) a ) ) {
						fprintf( fp, "-" );
					} else {
						int cycles = opinfo[ start_index ].base_cycles + arg_cycles[ a ];
						cycles = ( cycles + 1 ) & ~1;
						fprintf( fp, "%d", cycles );
					}
					fprintf( fp, "\n" );
				}
				fprintf( fp, "\n" );
			} else {
				int cycles = opinfo[ start_index ].base_cycles;
				cycles = ( cycles + 1 ) & ~1;
				fprintf( fp, "No operands. Cycle count: %d\n\n", cycles );
			}
			
			start_index = current_index;
		}
		++current_index;
	}

	fclose( fp );
}	

#endif /* SVGS_IMPLEMENTATION */
