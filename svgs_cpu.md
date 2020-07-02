The SVGS CPU Instruction Set
=========================
Here's a list of all the available instructions for the Super VGS CPU, their syntax, effect, what datatypes and operands they work on, and their cycle counts.

nop
---

### Syntax
	nop

### Function
No operation

### Operand Types and Cycle Counts
No operands. Cycle count: 2


jsr
---

### Syntax
	jsr argA

### Function
Jump to subroutine at location specified by argA. Will push the current value of IP onto the stack before the jump.

### Operand Types and Cycle Counts

argA     | cycles
---------|-------
areg     | 4
@areg    | 12
@areg+   | 12
@areg-   | 12
dreg     | 4
sp(ofs)  | 8
@sp(ofs) | 16
ival     | 8
@ival    | 20
label    | 8
@label   | 20
freg     | -
fval     | -


jmp
---

### Syntax
	jmp argA

### Function
Jump to location specified by argA.

### Operand Types and Cycle Counts

argA     | cycles
---------|-------
areg     | 4
@areg    | 12
@areg+   | 12
@areg-   | 12
dreg     | 4
sp(ofs)  | 8
@sp(ofs) | 16
ival     | 8
@ival    | 20
label    | 8
@label   | 20
freg     | -
fval     | -


ret
---

### Syntax
	ret

### Function
Return from subroutine call made by jsr. Will pop the return address off the stack.

### Operand Types and Cycle Counts
No operands. Cycle count: 4


sys
---

### Syntax
	sys argA

### Function
Make a BIOS call. See the BIOS documentation for available BIOS calls.

### Operand Types and Cycle Counts

argA     | cycles
---------|-------
areg     | 32
@areg    | 40
@areg+   | 40
@areg-   | 40
dreg     | 32
sp(ofs)  | 36
@sp(ofs) | 44
ival     | 36
@ival    | 48
label    | 36
@label   | 48
freg     | -
fval     | -


move
----

### Syntax
	move<.b/.w/.l/.f> argA, argB

### Function
Move the value specified by argA to the location specified by argB.

### Operand Types and Cycle Counts

|**argA**/**argB**| areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
|-----------------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|
|areg             | 2       | 10      | 10      | 10      | 2       | 6       | 14      | -       | 18      | -       | 18      | 2       | -       |
|@areg            | 10      | 18      | 18      | 18      | 10      | 14      | 22      | -       | 26      | -       | 26      | 10      | -       |
|@areg+           | 10      | 18      | 18      | 18      | 10      | 14      | 22      | -       | 26      | -       | 26      | 10      | -       |
|@areg-           | 10      | 18      | 18      | 18      | 10      | 14      | 22      | -       | 26      | -       | 26      | 10      | -       |
|dreg             | 2       | 10      | 10      | 10      | 2       | 6       | 14      | -       | 18      | -       | 18      | 2       | -       |
|sp(ofs)          | 6       | 14      | 14      | 14      | 6       | 10      | 18      | -       | 22      | -       | 22      | 6       | -       |
|@sp(ofs)         | 14      | 22      | 22      | 22      | 14      | 18      | 26      | -       | 30      | -       | 30      | 14      | -       |
|ival             | 6       | 14      | 14      | 14      | 6       | 10      | 18      | -       | 22      | -       | 22      | 6       | -       |
|@ival            | 18      | 26      | 26      | 26      | 18      | 22      | 30      | -       | 34      | -       | 34      | 18      | -       |
|label            | 6       | 14      | 14      | 14      | 6       | 10      | 18      | -       | 22      | -       | 22      | 6       | -       |
|@label           | 18      | 26      | 26      | 26      | 18      | 22      | 30      | -       | 34      | -       | 34      | 18      | -       |
|freg             | 2       | 10      | 10      | 10      | 2       | 6       | 14      | -       | 18      | -       | 18      | 2       | -       |
|fval             | 6       | 14      | 14      | 14      | 6       | 10      | 18      | -       | 22      | -       | 22      | 6       | -       |


xmov
----

### Syntax
	xmov<.b/.w/.l/.f> argA, argB

### Function
Move the value specified by argA to the location specified by argB. This will be repeated 16 times.

### Operand Types and Cycle Counts

|**argA**/**argB**| areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
|-----------------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|
|areg             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|@areg            | -       | 24      | 24      | 24      | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|@areg+           | -       | 24      | 24      | 24      | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|@areg-           | -       | 24      | 24      | 24      | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|dreg             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|sp(ofs)          | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|@sp(ofs)         | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|ival             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|@ival            | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|label            | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|@label           | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|freg             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|fval             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |


add
---

### Syntax
	add<.b/.w/.l> argA, argB

### Function
Add the value specified by argA to the value at the location specified by argB, and store the result at the location specified by argB.

### Operand Types and Cycle Counts

|**argA**/**argB**| areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
|-----------------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|
|areg             | 4       | 12      | 12      | 12      | 4       | 8       | 16      | -       | 20      | -       | 20      | -       | -       |
|@areg            | 12      | 20      | 20      | 20      | 12      | 16      | 24      | -       | 28      | -       | 28      | -       | -       |
|@areg+           | 12      | 20      | 20      | 20      | 12      | 16      | 24      | -       | 28      | -       | 28      | -       | -       |
|@areg-           | 12      | 20      | 20      | 20      | 12      | 16      | 24      | -       | 28      | -       | 28      | -       | -       |
|dreg             | 4       | 12      | 12      | 12      | 4       | 8       | 16      | -       | 20      | -       | 20      | -       | -       |
|sp(ofs)          | 8       | 16      | 16      | 16      | 8       | 12      | 20      | -       | 24      | -       | 24      | -       | -       |
|@sp(ofs)         | 16      | 24      | 24      | 24      | 16      | 20      | 28      | -       | 32      | -       | 32      | -       | -       |
|ival             | 8       | 16      | 16      | 16      | 8       | 12      | 20      | -       | 24      | -       | 24      | -       | -       |
|@ival            | 20      | 28      | 28      | 28      | 20      | 24      | 32      | -       | 36      | -       | 36      | -       | -       |
|label            | 8       | 16      | 16      | 16      | 8       | 12      | 20      | -       | 24      | -       | 24      | -       | -       |
|@label           | 20      | 28      | 28      | 28      | 20      | 24      | 32      | -       | 36      | -       | 36      | -       | -       |
|freg             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|fval             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |


add
---

### Syntax
	add.f argA, argB

### Function
Add the value specified by argA to the value at the location specified by argB, and store the result at the location specified by argB.

### Operand Types and Cycle Counts

|**argA**/**argB**| areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
|-----------------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|
|areg             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|@areg            | -       | 22      | -       | -       | -       | 18      | 26      | -       | 30      | -       | 30      | 14      | -       |
|@areg+           | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|@areg-           | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|dreg             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|sp(ofs)          | -       | 18      | -       | -       | -       | 14      | 22      | -       | 26      | -       | 26      | 10      | -       |
|@sp(ofs)         | -       | 26      | -       | -       | -       | 22      | 30      | -       | 34      | -       | 34      | 18      | -       |
|ival             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|@ival            | -       | 30      | -       | -       | -       | 26      | 34      | -       | 38      | -       | 38      | 22      | -       |
|label            | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|@label           | -       | 30      | -       | -       | -       | 26      | 34      | -       | 38      | -       | 38      | 22      | -       |
|freg             | -       | 14      | -       | -       | -       | 10      | 18      | -       | 22      | -       | 22      | 6       | -       |
|fval             | -       | 18      | -       | -       | -       | 14      | 22      | -       | 26      | -       | 26      | 10      | -       |


sub
---

### Syntax
	sub<.b/.w/.l> argA, argB

### Function
Subtract the value specified by argA from the value at the location specified by argB, and store the result at the location specified by argB.

### Operand Types and Cycle Counts

|**argA**/**argB**| areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
|-----------------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|
|areg             | 4       | 12      | 12      | 12      | 4       | 8       | 16      | -       | 20      | -       | 20      | -       | -       |
|@areg            | 12      | 20      | 20      | 20      | 12      | 16      | 24      | -       | 28      | -       | 28      | -       | -       |
|@areg+           | 12      | 20      | 20      | 20      | 12      | 16      | 24      | -       | 28      | -       | 28      | -       | -       |
|@areg-           | 12      | 20      | 20      | 20      | 12      | 16      | 24      | -       | 28      | -       | 28      | -       | -       |
|dreg             | 4       | 12      | 12      | 12      | 4       | 8       | 16      | -       | 20      | -       | 20      | -       | -       |
|sp(ofs)          | 8       | 16      | 16      | 16      | 8       | 12      | 20      | -       | 24      | -       | 24      | -       | -       |
|@sp(ofs)         | 16      | 24      | 24      | 24      | 16      | 20      | 28      | -       | 32      | -       | 32      | -       | -       |
|ival             | 8       | 16      | 16      | 16      | 8       | 12      | 20      | -       | 24      | -       | 24      | -       | -       |
|@ival            | 20      | 28      | 28      | 28      | 20      | 24      | 32      | -       | 36      | -       | 36      | -       | -       |
|label            | 8       | 16      | 16      | 16      | 8       | 12      | 20      | -       | 24      | -       | 24      | -       | -       |
|@label           | 20      | 28      | 28      | 28      | 20      | 24      | 32      | -       | 36      | -       | 36      | -       | -       |
|freg             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|fval             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |


sub
---

### Syntax
	sub.f argA, argB

### Function
Subtract the value specified by argA from the value at the location specified by argB, and store the result at the location specified by argB.

### Operand Types and Cycle Counts

|**argA**/**argB**| areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
|-----------------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|
|areg             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|@areg            | -       | 22      | -       | -       | -       | 18      | 26      | -       | 30      | -       | 30      | 14      | -       |
|@areg+           | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|@areg-           | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|dreg             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|sp(ofs)          | -       | 18      | -       | -       | -       | 14      | 22      | -       | 26      | -       | 26      | 10      | -       |
|@sp(ofs)         | -       | 26      | -       | -       | -       | 22      | 30      | -       | 34      | -       | 34      | 18      | -       |
|ival             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|@ival            | -       | 30      | -       | -       | -       | 26      | 34      | -       | 38      | -       | 38      | 22      | -       |
|label            | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|@label           | -       | 30      | -       | -       | -       | 26      | 34      | -       | 38      | -       | 38      | 22      | -       |
|freg             | -       | 14      | -       | -       | -       | 10      | 18      | -       | 22      | -       | 22      | 6       | -       |
|fval             | -       | 18      | -       | -       | -       | 14      | 22      | -       | 26      | -       | 26      | 10      | -       |


neg
---

### Syntax
	neg<.b/.w/.l> argA

### Function
Negate the value at the location specified by argA

### Operand Types and Cycle Counts

argA     | cycles
---------|-------
areg     | 2
@areg    | 10
@areg+   | 10
@areg-   | 10
dreg     | 2
sp(ofs)  | 6
@sp(ofs) | 14
ival     | -
@ival    | 18
label    | -
@label   | 18
freg     | -
fval     | -


neg
---

### Syntax
	neg.f argA

### Function
Negate the value at the location specified by argA

### Operand Types and Cycle Counts

argA     | cycles
---------|-------
areg     | -
@areg    | 10
@areg+   | -
@areg-   | -
dreg     | -
sp(ofs)  | 6
@sp(ofs) | 14
ival     | -
@ival    | 18
label    | -
@label   | 18
freg     | 2
fval     | -


muls
----

### Syntax
	muls<.b/.w/.l> argA, argB

### Function
Multiply the value specified by argA and the value at the location specified by argB, and store the result at the location specified by argB. Treat argA and argB as signed integer numbers.

### Operand Types and Cycle Counts

|**argA**/**argB**| areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
|-----------------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|
|areg             | 6       | 14      | 14      | 14      | 6       | 10      | 18      | -       | 22      | -       | 22      | -       | -       |
|@areg            | 14      | 22      | 22      | 22      | 14      | 18      | 26      | -       | 30      | -       | 30      | -       | -       |
|@areg+           | 14      | 22      | 22      | 22      | 14      | 18      | 26      | -       | 30      | -       | 30      | -       | -       |
|@areg-           | 14      | 22      | 22      | 22      | 14      | 18      | 26      | -       | 30      | -       | 30      | -       | -       |
|dreg             | 6       | 14      | 14      | 14      | 6       | 10      | 18      | -       | 22      | -       | 22      | -       | -       |
|sp(ofs)          | 10      | 18      | 18      | 18      | 10      | 14      | 22      | -       | 26      | -       | 26      | -       | -       |
|@sp(ofs)         | 18      | 26      | 26      | 26      | 18      | 22      | 30      | -       | 34      | -       | 34      | -       | -       |
|ival             | 10      | 18      | 18      | 18      | 10      | 14      | 22      | -       | 26      | -       | 26      | -       | -       |
|@ival            | 22      | 30      | 30      | 30      | 22      | 26      | 34      | -       | 38      | -       | 38      | -       | -       |
|label            | 10      | 18      | 18      | 18      | 10      | 14      | 22      | -       | 26      | -       | 26      | -       | -       |
|@label           | 22      | 30      | 30      | 30      | 22      | 26      | 34      | -       | 38      | -       | 38      | -       | -       |
|freg             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|fval             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |


mulu
----

### Syntax
	mulu<.b/.w/.l> argA, argB

### Function
Multiply the value specified by argA and the value at the location specified by argB, and store the result at the location specified by argB. Treat argA and argB as unsigned integer numbers.

### Operand Types and Cycle Counts

|**argA**/**argB**| areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
|-----------------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|
|areg             | 6       | 14      | 14      | 14      | 6       | 10      | 18      | -       | 22      | -       | 22      | -       | -       |
|@areg            | 14      | 22      | 22      | 22      | 14      | 18      | 26      | -       | 30      | -       | 30      | -       | -       |
|@areg+           | 14      | 22      | 22      | 22      | 14      | 18      | 26      | -       | 30      | -       | 30      | -       | -       |
|@areg-           | 14      | 22      | 22      | 22      | 14      | 18      | 26      | -       | 30      | -       | 30      | -       | -       |
|dreg             | 6       | 14      | 14      | 14      | 6       | 10      | 18      | -       | 22      | -       | 22      | -       | -       |
|sp(ofs)          | 10      | 18      | 18      | 18      | 10      | 14      | 22      | -       | 26      | -       | 26      | -       | -       |
|@sp(ofs)         | 18      | 26      | 26      | 26      | 18      | 22      | 30      | -       | 34      | -       | 34      | -       | -       |
|ival             | 10      | 18      | 18      | 18      | 10      | 14      | 22      | -       | 26      | -       | 26      | -       | -       |
|@ival            | 22      | 30      | 30      | 30      | 22      | 26      | 34      | -       | 38      | -       | 38      | -       | -       |
|label            | 10      | 18      | 18      | 18      | 10      | 14      | 22      | -       | 26      | -       | 26      | -       | -       |
|@label           | 22      | 30      | 30      | 30      | 22      | 26      | 34      | -       | 38      | -       | 38      | -       | -       |
|freg             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|fval             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |


mul
---

### Syntax
	mul.f argA, argB

### Function
Multiply the value specified by argA and the value at the location specified by argB, and store the result at the location specified by argB. Treat argA and argB as floating point numbers.

### Operand Types and Cycle Counts

|**argA**/**argB**| areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
|-----------------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|
|areg             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|@areg            | -       | 24      | -       | -       | -       | 20      | 28      | -       | 32      | -       | 32      | 16      | -       |
|@areg+           | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|@areg-           | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|dreg             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|sp(ofs)          | -       | 20      | -       | -       | -       | 16      | 24      | -       | 28      | -       | 28      | 12      | -       |
|@sp(ofs)         | -       | 28      | -       | -       | -       | 24      | 32      | -       | 36      | -       | 36      | 20      | -       |
|ival             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|@ival            | -       | 32      | -       | -       | -       | 28      | 36      | -       | 40      | -       | 40      | 24      | -       |
|label            | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|@label           | -       | 32      | -       | -       | -       | 28      | 36      | -       | 40      | -       | 40      | 24      | -       |
|freg             | -       | 16      | -       | -       | -       | 12      | 20      | -       | 24      | -       | 24      | 8       | -       |
|fval             | -       | 20      | -       | -       | -       | 16      | 24      | -       | 28      | -       | 28      | 12      | -       |


divs
----

### Syntax
	divs<.b/.w/.l> argA, argB

### Function
Divide the value at the location specified by argB by the value specified by argA, and store the result at the location specified by argB. Treat argA and argB as signed integer numbers.

### Operand Types and Cycle Counts

|**argA**/**argB**| areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
|-----------------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|
|areg             | 8       | 16      | 16      | 16      | 8       | 12      | 20      | -       | 24      | -       | 24      | -       | -       |
|@areg            | 16      | 24      | 24      | 24      | 16      | 20      | 28      | -       | 32      | -       | 32      | -       | -       |
|@areg+           | 16      | 24      | 24      | 24      | 16      | 20      | 28      | -       | 32      | -       | 32      | -       | -       |
|@areg-           | 16      | 24      | 24      | 24      | 16      | 20      | 28      | -       | 32      | -       | 32      | -       | -       |
|dreg             | 8       | 16      | 16      | 16      | 8       | 12      | 20      | -       | 24      | -       | 24      | -       | -       |
|sp(ofs)          | 12      | 20      | 20      | 20      | 12      | 16      | 24      | -       | 28      | -       | 28      | -       | -       |
|@sp(ofs)         | 20      | 28      | 28      | 28      | 20      | 24      | 32      | -       | 36      | -       | 36      | -       | -       |
|ival             | 12      | 20      | 20      | 20      | 12      | 16      | 24      | -       | 28      | -       | 28      | -       | -       |
|@ival            | 24      | 32      | 32      | 32      | 24      | 28      | 36      | -       | 40      | -       | 40      | -       | -       |
|label            | 12      | 20      | 20      | 20      | 12      | 16      | 24      | -       | 28      | -       | 28      | -       | -       |
|@label           | 24      | 32      | 32      | 32      | 24      | 28      | 36      | -       | 40      | -       | 40      | -       | -       |
|freg             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|fval             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |


divu
----

### Syntax
	divu<.b/.w/.l> argA, argB

### Function
Divide the value at the location specified by argB by the value specified by argA, and store the result at the location specified by argB. Treat argA and argB as unsigned integer numbers.

### Operand Types and Cycle Counts

|**argA**/**argB**| areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
|-----------------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|
|areg             | 8       | 16      | 16      | 16      | 8       | 12      | 20      | -       | 24      | -       | 24      | -       | -       |
|@areg            | 16      | 24      | 24      | 24      | 16      | 20      | 28      | -       | 32      | -       | 32      | -       | -       |
|@areg+           | 16      | 24      | 24      | 24      | 16      | 20      | 28      | -       | 32      | -       | 32      | -       | -       |
|@areg-           | 16      | 24      | 24      | 24      | 16      | 20      | 28      | -       | 32      | -       | 32      | -       | -       |
|dreg             | 8       | 16      | 16      | 16      | 8       | 12      | 20      | -       | 24      | -       | 24      | -       | -       |
|sp(ofs)          | 12      | 20      | 20      | 20      | 12      | 16      | 24      | -       | 28      | -       | 28      | -       | -       |
|@sp(ofs)         | 20      | 28      | 28      | 28      | 20      | 24      | 32      | -       | 36      | -       | 36      | -       | -       |
|ival             | 12      | 20      | 20      | 20      | 12      | 16      | 24      | -       | 28      | -       | 28      | -       | -       |
|@ival            | 24      | 32      | 32      | 32      | 24      | 28      | 36      | -       | 40      | -       | 40      | -       | -       |
|label            | 12      | 20      | 20      | 20      | 12      | 16      | 24      | -       | 28      | -       | 28      | -       | -       |
|@label           | 24      | 32      | 32      | 32      | 24      | 28      | 36      | -       | 40      | -       | 40      | -       | -       |
|freg             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|fval             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |


div
---

### Syntax
	div.f argA, argB

### Function
Divide the value at the location specified by argB by the value specified by argA, and store the result at the location specified by argB. Treat argA and argB as floating point numbers.

### Operand Types and Cycle Counts

|**argA**/**argB**| areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
|-----------------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|
|areg             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|@areg            | -       | 26      | -       | -       | -       | 22      | 30      | -       | 34      | -       | 34      | 18      | -       |
|@areg+           | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|@areg-           | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|dreg             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|sp(ofs)          | -       | 22      | -       | -       | -       | 18      | 26      | -       | 30      | -       | 30      | 14      | -       |
|@sp(ofs)         | -       | 30      | -       | -       | -       | 26      | 34      | -       | 38      | -       | 38      | 22      | -       |
|ival             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|@ival            | -       | 34      | -       | -       | -       | 30      | 38      | -       | 42      | -       | 42      | 26      | -       |
|label            | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|@label           | -       | 34      | -       | -       | -       | 30      | 38      | -       | 42      | -       | 42      | 26      | -       |
|freg             | -       | 18      | -       | -       | -       | 14      | 22      | -       | 26      | -       | 26      | 10      | -       |
|fval             | -       | 22      | -       | -       | -       | 18      | 26      | -       | 30      | -       | 30      | 14      | -       |


itof
----

### Syntax
	itof<.b/.w/.l> argA, argB

### Function
Convert the value specified by argA from an integer value into an floating point value, and store the result at the location specified by argB.

### Operand Types and Cycle Counts

|**argA**/**argB**| areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
|-----------------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|
|areg             | -       | 10      | -       | -       | -       | 6       | 14      | -       | 18      | -       | 18      | 2       | -       |
|@areg            | -       | 18      | -       | -       | -       | 14      | 22      | -       | 26      | -       | 26      | 10      | -       |
|@areg+           | -       | 18      | -       | -       | -       | 14      | 22      | -       | 26      | -       | 26      | 10      | -       |
|@areg-           | -       | 18      | -       | -       | -       | 14      | 22      | -       | 26      | -       | 26      | 10      | -       |
|dreg             | -       | 10      | -       | -       | -       | 6       | 14      | -       | 18      | -       | 18      | 2       | -       |
|sp(ofs)          | -       | 14      | -       | -       | -       | 10      | 18      | -       | 22      | -       | 22      | 6       | -       |
|@sp(ofs)         | -       | 22      | -       | -       | -       | 18      | 26      | -       | 30      | -       | 30      | 14      | -       |
|ival             | -       | 14      | -       | -       | -       | 10      | 18      | -       | 22      | -       | 22      | 6       | -       |
|@ival            | -       | 26      | -       | -       | -       | 22      | 30      | -       | 34      | -       | 34      | 18      | -       |
|label            | -       | 14      | -       | -       | -       | 10      | 18      | -       | 22      | -       | 22      | 6       | -       |
|@label           | -       | 26      | -       | -       | -       | 22      | 30      | -       | 34      | -       | 34      | 18      | -       |
|freg             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|fval             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |


ftoi
----

### Syntax
	ftoi<.b/.w/.l> argA, argB

### Function
Convert the value specified by argA from a floating point value into an integer value, and store the result at the location specified by argB.

### Operand Types and Cycle Counts

|**argA**/**argB**| areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
|-----------------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|
|areg             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|@areg            | 10      | 18      | 18      | 18      | 10      | 14      | 22      | -       | 26      | -       | 26      | -       | -       |
|@areg+           | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|@areg-           | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|dreg             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|sp(ofs)          | 6       | 14      | 14      | 14      | 6       | 10      | 18      | -       | 22      | -       | 22      | -       | -       |
|@sp(ofs)         | 14      | 22      | 22      | 22      | 14      | 18      | 26      | -       | 30      | -       | 30      | -       | -       |
|ival             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|@ival            | 18      | 26      | 26      | 26      | 18      | 22      | 30      | -       | 34      | -       | 34      | -       | -       |
|label            | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|@label           | 18      | 26      | 26      | 26      | 18      | 22      | 30      | -       | 34      | -       | 34      | -       | -       |
|freg             | 2       | 10      | 10      | 10      | 2       | 6       | 14      | -       | 18      | -       | 18      | -       | -       |
|fval             | 6       | 14      | 14      | 14      | 6       | 10      | 18      | -       | 22      | -       | 22      | -       | -       |


cmpu
----

### Syntax
	cmpu<.b/.w/.l> argA, argB

### Function
Compare the value specified by argA with the value specified by argB. Treat argA and argB as unsigned integers. The result is used in the next call to je, jne, jge, jle, jg, or jl.

### Operand Types and Cycle Counts

|**argA**/**argB**| areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
|-----------------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|
|areg             | 2       | 10      | 10      | 10      | 2       | 6       | 14      | 6       | 18      | 6       | 18      | -       | -       |
|@areg            | 10      | 18      | 18      | 18      | 10      | 14      | 22      | 14      | 26      | 14      | 26      | -       | -       |
|@areg+           | 10      | 18      | 18      | 18      | 10      | 14      | 22      | 14      | 26      | 14      | 26      | -       | -       |
|@areg-           | 10      | 18      | 18      | 18      | 10      | 14      | 22      | 14      | 26      | 14      | 26      | -       | -       |
|dreg             | 2       | 10      | 10      | 10      | 2       | 6       | 14      | 6       | 18      | 6       | 18      | -       | -       |
|sp(ofs)          | 6       | 14      | 14      | 14      | 6       | 10      | 18      | 10      | 22      | 10      | 22      | -       | -       |
|@sp(ofs)         | 14      | 22      | 22      | 22      | 14      | 18      | 26      | 18      | 30      | 18      | 30      | -       | -       |
|ival             | 6       | 14      | 14      | 14      | 6       | 10      | 18      | 10      | 22      | 10      | 22      | -       | -       |
|@ival            | 18      | 26      | 26      | 26      | 18      | 22      | 30      | 22      | 34      | 22      | 34      | -       | -       |
|label            | 6       | 14      | 14      | 14      | 6       | 10      | 18      | 10      | 22      | 10      | 22      | -       | -       |
|@label           | 18      | 26      | 26      | 26      | 18      | 22      | 30      | 22      | 34      | 22      | 34      | -       | -       |
|freg             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|fval             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |


cmps
----

### Syntax
	cmps<.b/.w/.l> argA, argB

### Function
Compare the value specified by argA with the value specified by argB. Treat argA and argB as signed integers. The result is used in the next call to je, jne, jge, jle, jg, or jl.

### Operand Types and Cycle Counts

|**argA**/**argB**| areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
|-----------------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|
|areg             | 2       | 10      | 10      | 10      | 2       | 6       | 14      | 6       | 18      | 6       | 18      | -       | -       |
|@areg            | 10      | 18      | 18      | 18      | 10      | 14      | 22      | 14      | 26      | 14      | 26      | -       | -       |
|@areg+           | 10      | 18      | 18      | 18      | 10      | 14      | 22      | 14      | 26      | 14      | 26      | -       | -       |
|@areg-           | 10      | 18      | 18      | 18      | 10      | 14      | 22      | 14      | 26      | 14      | 26      | -       | -       |
|dreg             | 2       | 10      | 10      | 10      | 2       | 6       | 14      | 6       | 18      | 6       | 18      | -       | -       |
|sp(ofs)          | 6       | 14      | 14      | 14      | 6       | 10      | 18      | 10      | 22      | 10      | 22      | -       | -       |
|@sp(ofs)         | 14      | 22      | 22      | 22      | 14      | 18      | 26      | 18      | 30      | 18      | 30      | -       | -       |
|ival             | 6       | 14      | 14      | 14      | 6       | 10      | 18      | 10      | 22      | 10      | 22      | -       | -       |
|@ival            | 18      | 26      | 26      | 26      | 18      | 22      | 30      | 22      | 34      | 22      | 34      | -       | -       |
|label            | 6       | 14      | 14      | 14      | 6       | 10      | 18      | 10      | 22      | 10      | 22      | -       | -       |
|@label           | 18      | 26      | 26      | 26      | 18      | 22      | 30      | 22      | 34      | 22      | 34      | -       | -       |
|freg             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|fval             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |


cmp
---

### Syntax
	cmp.f argA, argB

### Function
Compare the value specified by argA with the value specified by argB. Treat argA and argB as floating point numbers. The result is used in the next call to je, jne, jge, jle, jg, or jl.

### Operand Types and Cycle Counts

|**argA**/**argB**| areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
|-----------------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|
|areg             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|@areg            | -       | 18      | -       | -       | -       | 14      | 22      | -       | 26      | -       | 26      | 10      | 14      |
|@areg+           | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|@areg-           | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|dreg             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|sp(ofs)          | -       | 14      | -       | -       | -       | 10      | 18      | -       | 22      | -       | 22      | 6       | 10      |
|@sp(ofs)         | -       | 22      | -       | -       | -       | 18      | 26      | -       | 30      | -       | 30      | 14      | 18      |
|ival             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|@ival            | -       | 26      | -       | -       | -       | 22      | 30      | -       | 34      | -       | 34      | 18      | 22      |
|label            | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|@label           | -       | 26      | -       | -       | -       | 22      | 30      | -       | 34      | -       | 34      | 18      | 22      |
|freg             | -       | 10      | -       | -       | -       | 6       | 14      | -       | 18      | -       | 18      | 2       | 6       |
|fval             | -       | 14      | -       | -       | -       | 10      | 18      | -       | 22      | -       | 22      | 6       | 10      |


tst
---

### Syntax
	tst<.b/.w/.l> argA

### Function
Test if value specified by argA is zero. The result is used in the next call to jz or jnz.

### Operand Types and Cycle Counts

argA     | cycles
---------|-------
areg     | 2
@areg    | 10
@areg+   | 10
@areg-   | 10
dreg     | 2
sp(ofs)  | 6
@sp(ofs) | 14
ival     | 6
@ival    | 18
label    | 6
@label   | 18
freg     | -
fval     | -


tstf
----

### Syntax
	tstf.f argA

### Function
Test if value specified by argA is zero. The result is used in the next call to jz or jnz.

### Operand Types and Cycle Counts

argA     | cycles
---------|-------
areg     | -
@areg    | 10
@areg+   | -
@areg-   | -
dreg     | -
sp(ofs)  | 6
@sp(ofs) | 14
ival     | -
@ival    | 18
label    | -
@label   | 18
freg     | 2
fval     | 6


je
--

### Syntax
	je argA

### Function
Jump to location specified by argA, if in the last compare (cmpu, cmps or cmp), argA was equal to argB.

### Operand Types and Cycle Counts

argA     | cycles
---------|-------
areg     | 4
@areg    | 12
@areg+   | 12
@areg-   | 12
dreg     | 4
sp(ofs)  | 8
@sp(ofs) | 16
ival     | 8
@ival    | 20
label    | 8
@label   | 20
freg     | -
fval     | -


jne
---

### Syntax
	jne argA

### Function
Jump to location specified by argA, if in the last compare (cmpu, cmps or cmp), argA was not equal to argB.

### Operand Types and Cycle Counts

argA     | cycles
---------|-------
areg     | 4
@areg    | 12
@areg+   | 12
@areg-   | 12
dreg     | 4
sp(ofs)  | 8
@sp(ofs) | 16
ival     | 8
@ival    | 20
label    | 8
@label   | 20
freg     | -
fval     | -


jge
---

### Syntax
	jge argA

### Function
Jump to location specified by argA, if in the last compare (cmpu, cmps or cmp), argA was greater than or equal to argB.

### Operand Types and Cycle Counts

argA     | cycles
---------|-------
areg     | 4
@areg    | 12
@areg+   | 12
@areg-   | 12
dreg     | 4
sp(ofs)  | 8
@sp(ofs) | 16
ival     | 8
@ival    | 20
label    | 8
@label   | 20
freg     | -
fval     | -


jle
---

### Syntax
	jle argA

### Function
Jump to location specified by argA, if in the last compare (cmpu, cmps or cmp), argA was less than or equal to argB.

### Operand Types and Cycle Counts

argA     | cycles
---------|-------
areg     | 4
@areg    | 12
@areg+   | 12
@areg-   | 12
dreg     | 4
sp(ofs)  | 8
@sp(ofs) | 16
ival     | 8
@ival    | 20
label    | 8
@label   | 20
freg     | -
fval     | -


jg
--

### Syntax
	jg argA

### Function
Jump to location specified by argA, if in the last compare (cmpu, cmps or cmp), argA was greater than argB.

### Operand Types and Cycle Counts

argA     | cycles
---------|-------
areg     | 4
@areg    | 12
@areg+   | 12
@areg-   | 12
dreg     | 4
sp(ofs)  | 8
@sp(ofs) | 16
ival     | 8
@ival    | 20
label    | 8
@label   | 20
freg     | -
fval     | -


jl
--

### Syntax
	jl argA

### Function
Jump to location specified by argA, if in the last compare (cmpu, cmps or cmp), argA was less than argB.

### Operand Types and Cycle Counts

argA     | cycles
---------|-------
areg     | 4
@areg    | 12
@areg+   | 12
@areg-   | 12
dreg     | 4
sp(ofs)  | 8
@sp(ofs) | 16
ival     | 8
@ival    | 20
label    | 8
@label   | 20
freg     | -
fval     | -


jz
--

### Syntax
	jz argA

### Function
Jump to location specified by argA, if in the last test (tst or btst), the result was zero.

### Operand Types and Cycle Counts

argA     | cycles
---------|-------
areg     | 4
@areg    | 12
@areg+   | 12
@areg-   | 12
dreg     | 4
sp(ofs)  | 8
@sp(ofs) | 16
ival     | 8
@ival    | 20
label    | 8
@label   | 20
freg     | -
fval     | -


jnz
---

### Syntax
	jnz argA

### Function
Jump to location specified by argA, if in the last test (tst or btst), the result was non-zero

### Operand Types and Cycle Counts

argA     | cycles
---------|-------
areg     | 4
@areg    | 12
@areg+   | 12
@areg-   | 12
dreg     | 4
sp(ofs)  | 8
@sp(ofs) | 16
ival     | 8
@ival    | 20
label    | 8
@label   | 20
freg     | -
fval     | -


dje
---

### Syntax
	dje argA, argB

### Function
Decrement argB, compare it to zero, and jump to location specified by argA, if argB was equal to zero.

### Operand Types and Cycle Counts

|**argA**/**argB**| areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
|-----------------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|
|areg             | 4       | 12      | 12      | 12      | 4       | 8       | 16      | -       | 20      | -       | 20      | -       | -       |
|@areg            | 12      | 20      | 20      | 20      | 12      | 16      | 24      | -       | 28      | -       | 28      | -       | -       |
|@areg+           | 12      | 20      | 20      | 20      | 12      | 16      | 24      | -       | 28      | -       | 28      | -       | -       |
|@areg-           | 12      | 20      | 20      | 20      | 12      | 16      | 24      | -       | 28      | -       | 28      | -       | -       |
|dreg             | 4       | 12      | 12      | 12      | 4       | 8       | 16      | -       | 20      | -       | 20      | -       | -       |
|sp(ofs)          | 8       | 16      | 16      | 16      | 8       | 12      | 20      | -       | 24      | -       | 24      | -       | -       |
|@sp(ofs)         | 16      | 24      | 24      | 24      | 16      | 20      | 28      | -       | 32      | -       | 32      | -       | -       |
|ival             | 8       | 16      | 16      | 16      | 8       | 12      | 20      | -       | 24      | -       | 24      | -       | -       |
|@ival            | 20      | 28      | 28      | 28      | 20      | 24      | 32      | -       | 36      | -       | 36      | -       | -       |
|label            | 8       | 16      | 16      | 16      | 8       | 12      | 20      | -       | 24      | -       | 24      | -       | -       |
|@label           | 20      | 28      | 28      | 28      | 20      | 24      | 32      | -       | 36      | -       | 36      | -       | -       |
|freg             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|fval             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |


djne
----

### Syntax
	djne argA, argB

### Function
Decrement argB, compare it to zero, and jump to location specified by argA, if argB was not equal to zero.

### Operand Types and Cycle Counts

|**argA**/**argB**| areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
|-----------------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|
|areg             | 4       | 12      | 12      | 12      | 4       | 8       | 16      | -       | 20      | -       | 20      | -       | -       |
|@areg            | 12      | 20      | 20      | 20      | 12      | 16      | 24      | -       | 28      | -       | 28      | -       | -       |
|@areg+           | 12      | 20      | 20      | 20      | 12      | 16      | 24      | -       | 28      | -       | 28      | -       | -       |
|@areg-           | 12      | 20      | 20      | 20      | 12      | 16      | 24      | -       | 28      | -       | 28      | -       | -       |
|dreg             | 4       | 12      | 12      | 12      | 4       | 8       | 16      | -       | 20      | -       | 20      | -       | -       |
|sp(ofs)          | 8       | 16      | 16      | 16      | 8       | 12      | 20      | -       | 24      | -       | 24      | -       | -       |
|@sp(ofs)         | 16      | 24      | 24      | 24      | 16      | 20      | 28      | -       | 32      | -       | 32      | -       | -       |
|ival             | 8       | 16      | 16      | 16      | 8       | 12      | 20      | -       | 24      | -       | 24      | -       | -       |
|@ival            | 20      | 28      | 28      | 28      | 20      | 24      | 32      | -       | 36      | -       | 36      | -       | -       |
|label            | 8       | 16      | 16      | 16      | 8       | 12      | 20      | -       | 24      | -       | 24      | -       | -       |
|@label           | 20      | 28      | 28      | 28      | 20      | 24      | 32      | -       | 36      | -       | 36      | -       | -       |
|freg             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|fval             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |


djge
----

### Syntax
	djge argA, argB

### Function
Decrement argB, compare it to zero, and jump to location specified by argA, if argB was greater than or equal to zero.

### Operand Types and Cycle Counts

|**argA**/**argB**| areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
|-----------------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|
|areg             | 4       | 12      | 12      | 12      | 4       | 8       | 16      | -       | 20      | -       | 20      | -       | -       |
|@areg            | 12      | 20      | 20      | 20      | 12      | 16      | 24      | -       | 28      | -       | 28      | -       | -       |
|@areg+           | 12      | 20      | 20      | 20      | 12      | 16      | 24      | -       | 28      | -       | 28      | -       | -       |
|@areg-           | 12      | 20      | 20      | 20      | 12      | 16      | 24      | -       | 28      | -       | 28      | -       | -       |
|dreg             | 4       | 12      | 12      | 12      | 4       | 8       | 16      | -       | 20      | -       | 20      | -       | -       |
|sp(ofs)          | 8       | 16      | 16      | 16      | 8       | 12      | 20      | -       | 24      | -       | 24      | -       | -       |
|@sp(ofs)         | 16      | 24      | 24      | 24      | 16      | 20      | 28      | -       | 32      | -       | 32      | -       | -       |
|ival             | 8       | 16      | 16      | 16      | 8       | 12      | 20      | -       | 24      | -       | 24      | -       | -       |
|@ival            | 20      | 28      | 28      | 28      | 20      | 24      | 32      | -       | 36      | -       | 36      | -       | -       |
|label            | 8       | 16      | 16      | 16      | 8       | 12      | 20      | -       | 24      | -       | 24      | -       | -       |
|@label           | 20      | 28      | 28      | 28      | 20      | 24      | 32      | -       | 36      | -       | 36      | -       | -       |
|freg             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|fval             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |


djle
----

### Syntax
	djle argA, argB

### Function
Decrement argB, compare it to zero, and jump to location specified by argA, if argB was less than or equal to zero.

### Operand Types and Cycle Counts

|**argA**/**argB**| areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
|-----------------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|
|areg             | 4       | 12      | 12      | 12      | 4       | 8       | 16      | -       | 20      | -       | 20      | -       | -       |
|@areg            | 12      | 20      | 20      | 20      | 12      | 16      | 24      | -       | 28      | -       | 28      | -       | -       |
|@areg+           | 12      | 20      | 20      | 20      | 12      | 16      | 24      | -       | 28      | -       | 28      | -       | -       |
|@areg-           | 12      | 20      | 20      | 20      | 12      | 16      | 24      | -       | 28      | -       | 28      | -       | -       |
|dreg             | 4       | 12      | 12      | 12      | 4       | 8       | 16      | -       | 20      | -       | 20      | -       | -       |
|sp(ofs)          | 8       | 16      | 16      | 16      | 8       | 12      | 20      | -       | 24      | -       | 24      | -       | -       |
|@sp(ofs)         | 16      | 24      | 24      | 24      | 16      | 20      | 28      | -       | 32      | -       | 32      | -       | -       |
|ival             | 8       | 16      | 16      | 16      | 8       | 12      | 20      | -       | 24      | -       | 24      | -       | -       |
|@ival            | 20      | 28      | 28      | 28      | 20      | 24      | 32      | -       | 36      | -       | 36      | -       | -       |
|label            | 8       | 16      | 16      | 16      | 8       | 12      | 20      | -       | 24      | -       | 24      | -       | -       |
|@label           | 20      | 28      | 28      | 28      | 20      | 24      | 32      | -       | 36      | -       | 36      | -       | -       |
|freg             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|fval             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |


djg
---

### Syntax
	djg argA, argB

### Function
Decrement argB, compare it to zero, and jump to location specified by argA, if argB was greater than zero.

### Operand Types and Cycle Counts

|**argA**/**argB**| areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
|-----------------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|
|areg             | 4       | 12      | 12      | 12      | 4       | 8       | 16      | -       | 20      | -       | 20      | -       | -       |
|@areg            | 12      | 20      | 20      | 20      | 12      | 16      | 24      | -       | 28      | -       | 28      | -       | -       |
|@areg+           | 12      | 20      | 20      | 20      | 12      | 16      | 24      | -       | 28      | -       | 28      | -       | -       |
|@areg-           | 12      | 20      | 20      | 20      | 12      | 16      | 24      | -       | 28      | -       | 28      | -       | -       |
|dreg             | 4       | 12      | 12      | 12      | 4       | 8       | 16      | -       | 20      | -       | 20      | -       | -       |
|sp(ofs)          | 8       | 16      | 16      | 16      | 8       | 12      | 20      | -       | 24      | -       | 24      | -       | -       |
|@sp(ofs)         | 16      | 24      | 24      | 24      | 16      | 20      | 28      | -       | 32      | -       | 32      | -       | -       |
|ival             | 8       | 16      | 16      | 16      | 8       | 12      | 20      | -       | 24      | -       | 24      | -       | -       |
|@ival            | 20      | 28      | 28      | 28      | 20      | 24      | 32      | -       | 36      | -       | 36      | -       | -       |
|label            | 8       | 16      | 16      | 16      | 8       | 12      | 20      | -       | 24      | -       | 24      | -       | -       |
|@label           | 20      | 28      | 28      | 28      | 20      | 24      | 32      | -       | 36      | -       | 36      | -       | -       |
|freg             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|fval             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |


djl
---

### Syntax
	djl argA, argB

### Function
Decrement argB, compare it to zero, and jump to location specified by argA, if argB was less than zero.

### Operand Types and Cycle Counts

|**argA**/**argB**| areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
|-----------------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|
|areg             | 4       | 12      | 12      | 12      | 4       | 8       | 16      | -       | 20      | -       | 20      | -       | -       |
|@areg            | 12      | 20      | 20      | 20      | 12      | 16      | 24      | -       | 28      | -       | 28      | -       | -       |
|@areg+           | 12      | 20      | 20      | 20      | 12      | 16      | 24      | -       | 28      | -       | 28      | -       | -       |
|@areg-           | 12      | 20      | 20      | 20      | 12      | 16      | 24      | -       | 28      | -       | 28      | -       | -       |
|dreg             | 4       | 12      | 12      | 12      | 4       | 8       | 16      | -       | 20      | -       | 20      | -       | -       |
|sp(ofs)          | 8       | 16      | 16      | 16      | 8       | 12      | 20      | -       | 24      | -       | 24      | -       | -       |
|@sp(ofs)         | 16      | 24      | 24      | 24      | 16      | 20      | 28      | -       | 32      | -       | 32      | -       | -       |
|ival             | 8       | 16      | 16      | 16      | 8       | 12      | 20      | -       | 24      | -       | 24      | -       | -       |
|@ival            | 20      | 28      | 28      | 28      | 20      | 24      | 32      | -       | 36      | -       | 36      | -       | -       |
|label            | 8       | 16      | 16      | 16      | 8       | 12      | 20      | -       | 24      | -       | 24      | -       | -       |
|@label           | 20      | 28      | 28      | 28      | 20      | 24      | 32      | -       | 36      | -       | 36      | -       | -       |
|freg             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|fval             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |


djz
---

### Syntax
	djz argA, argB

### Function
Decrement argB, compare it to zero, and jump to location specified by argA, if argB was zero.

### Operand Types and Cycle Counts

|**argA**/**argB**| areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
|-----------------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|
|areg             | 4       | 12      | 12      | 12      | 4       | 8       | 16      | -       | 20      | -       | 20      | -       | -       |
|@areg            | 12      | 20      | 20      | 20      | 12      | 16      | 24      | -       | 28      | -       | 28      | -       | -       |
|@areg+           | 12      | 20      | 20      | 20      | 12      | 16      | 24      | -       | 28      | -       | 28      | -       | -       |
|@areg-           | 12      | 20      | 20      | 20      | 12      | 16      | 24      | -       | 28      | -       | 28      | -       | -       |
|dreg             | 4       | 12      | 12      | 12      | 4       | 8       | 16      | -       | 20      | -       | 20      | -       | -       |
|sp(ofs)          | 8       | 16      | 16      | 16      | 8       | 12      | 20      | -       | 24      | -       | 24      | -       | -       |
|@sp(ofs)         | 16      | 24      | 24      | 24      | 16      | 20      | 28      | -       | 32      | -       | 32      | -       | -       |
|ival             | 8       | 16      | 16      | 16      | 8       | 12      | 20      | -       | 24      | -       | 24      | -       | -       |
|@ival            | 20      | 28      | 28      | 28      | 20      | 24      | 32      | -       | 36      | -       | 36      | -       | -       |
|label            | 8       | 16      | 16      | 16      | 8       | 12      | 20      | -       | 24      | -       | 24      | -       | -       |
|@label           | 20      | 28      | 28      | 28      | 20      | 24      | 32      | -       | 36      | -       | 36      | -       | -       |
|freg             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|fval             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |


djnz
----

### Syntax
	djnz argA, argB

### Function
Decrement argB, compare it to zero, and jump to location specified by argA, if argB was non-zero

### Operand Types and Cycle Counts

|**argA**/**argB**| areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
|-----------------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|
|areg             | 4       | 12      | 12      | 12      | 4       | 8       | 16      | -       | 20      | -       | 20      | -       | -       |
|@areg            | 12      | 20      | 20      | 20      | 12      | 16      | 24      | -       | 28      | -       | 28      | -       | -       |
|@areg+           | 12      | 20      | 20      | 20      | 12      | 16      | 24      | -       | 28      | -       | 28      | -       | -       |
|@areg-           | 12      | 20      | 20      | 20      | 12      | 16      | 24      | -       | 28      | -       | 28      | -       | -       |
|dreg             | 4       | 12      | 12      | 12      | 4       | 8       | 16      | -       | 20      | -       | 20      | -       | -       |
|sp(ofs)          | 8       | 16      | 16      | 16      | 8       | 12      | 20      | -       | 24      | -       | 24      | -       | -       |
|@sp(ofs)         | 16      | 24      | 24      | 24      | 16      | 20      | 28      | -       | 32      | -       | 32      | -       | -       |
|ival             | 8       | 16      | 16      | 16      | 8       | 12      | 20      | -       | 24      | -       | 24      | -       | -       |
|@ival            | 20      | 28      | 28      | 28      | 20      | 24      | 32      | -       | 36      | -       | 36      | -       | -       |
|label            | 8       | 16      | 16      | 16      | 8       | 12      | 20      | -       | 24      | -       | 24      | -       | -       |
|@label           | 20      | 28      | 28      | 28      | 20      | 24      | 32      | -       | 36      | -       | 36      | -       | -       |
|freg             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|fval             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |


push
----

### Syntax
	push<.b/.w/.l/.f> argA

### Function
Push the value specified by argA onto the stack.

### Operand Types and Cycle Counts

argA     | cycles
---------|-------
areg     | 4
@areg    | 12
@areg+   | 12
@areg-   | 12
dreg     | 4
sp(ofs)  | 8
@sp(ofs) | 16
ival     | 8
@ival    | 20
label    | 8
@label   | 20
freg     | 4
fval     | 8


pop
---

### Syntax
	pop<.b/.w/.l/.f> argA

### Function
Pop a value off the stack, and store it at the location specified by argA.

### Operand Types and Cycle Counts

argA     | cycles
---------|-------
areg     | 4
@areg    | 12
@areg+   | 12
@areg-   | 12
dreg     | 4
sp(ofs)  | 8
@sp(ofs) | 16
ival     | -
@ival    | 20
label    | -
@label   | 20
freg     | 4
fval     | -


and
---

### Syntax
	and<.b/.w/.l> argA, argB

### Function
Perform a bitwise AND on the value specified by argA and the value at the location specified by argB, and store the result at the location specified by argB.

### Operand Types and Cycle Counts

|**argA**/**argB**| areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
|-----------------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|
|areg             | 2       | 10      | 10      | 10      | 2       | 6       | 14      | -       | 18      | -       | 18      | -       | -       |
|@areg            | 10      | 18      | 18      | 18      | 10      | 14      | 22      | -       | 26      | -       | 26      | -       | -       |
|@areg+           | 10      | 18      | 18      | 18      | 10      | 14      | 22      | -       | 26      | -       | 26      | -       | -       |
|@areg-           | 10      | 18      | 18      | 18      | 10      | 14      | 22      | -       | 26      | -       | 26      | -       | -       |
|dreg             | 2       | 10      | 10      | 10      | 2       | 6       | 14      | -       | 18      | -       | 18      | -       | -       |
|sp(ofs)          | 6       | 14      | 14      | 14      | 6       | 10      | 18      | -       | 22      | -       | 22      | -       | -       |
|@sp(ofs)         | 14      | 22      | 22      | 22      | 14      | 18      | 26      | -       | 30      | -       | 30      | -       | -       |
|ival             | 6       | 14      | 14      | 14      | 6       | 10      | 18      | -       | 22      | -       | 22      | -       | -       |
|@ival            | 18      | 26      | 26      | 26      | 18      | 22      | 30      | -       | 34      | -       | 34      | -       | -       |
|label            | 6       | 14      | 14      | 14      | 6       | 10      | 18      | -       | 22      | -       | 22      | -       | -       |
|@label           | 18      | 26      | 26      | 26      | 18      | 22      | 30      | -       | 34      | -       | 34      | -       | -       |
|freg             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|fval             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |


or
--

### Syntax
	or<.b/.w/.l> argA, argB

### Function
Perform a bitwise OR on the value specified by argA and the value at the location specified by argB, and store the result at the location specified by argB.

### Operand Types and Cycle Counts

|**argA**/**argB**| areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
|-----------------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|
|areg             | 2       | 10      | 10      | 10      | 2       | 6       | 14      | -       | 18      | -       | 18      | -       | -       |
|@areg            | 10      | 18      | 18      | 18      | 10      | 14      | 22      | -       | 26      | -       | 26      | -       | -       |
|@areg+           | 10      | 18      | 18      | 18      | 10      | 14      | 22      | -       | 26      | -       | 26      | -       | -       |
|@areg-           | 10      | 18      | 18      | 18      | 10      | 14      | 22      | -       | 26      | -       | 26      | -       | -       |
|dreg             | 2       | 10      | 10      | 10      | 2       | 6       | 14      | -       | 18      | -       | 18      | -       | -       |
|sp(ofs)          | 6       | 14      | 14      | 14      | 6       | 10      | 18      | -       | 22      | -       | 22      | -       | -       |
|@sp(ofs)         | 14      | 22      | 22      | 22      | 14      | 18      | 26      | -       | 30      | -       | 30      | -       | -       |
|ival             | 6       | 14      | 14      | 14      | 6       | 10      | 18      | -       | 22      | -       | 22      | -       | -       |
|@ival            | 18      | 26      | 26      | 26      | 18      | 22      | 30      | -       | 34      | -       | 34      | -       | -       |
|label            | 6       | 14      | 14      | 14      | 6       | 10      | 18      | -       | 22      | -       | 22      | -       | -       |
|@label           | 18      | 26      | 26      | 26      | 18      | 22      | 30      | -       | 34      | -       | 34      | -       | -       |
|freg             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|fval             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |


xor
---

### Syntax
	xor<.b/.w/.l> argA, argB

### Function
Perform a bitwise XOR on the value specified by argA and the value at the location specified by argB, and store the result at the location specified by argB.

### Operand Types and Cycle Counts

|**argA**/**argB**| areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
|-----------------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|
|areg             | 2       | 10      | 10      | 10      | 2       | 6       | 14      | -       | 18      | -       | 18      | -       | -       |
|@areg            | 10      | 18      | 18      | 18      | 10      | 14      | 22      | -       | 26      | -       | 26      | -       | -       |
|@areg+           | 10      | 18      | 18      | 18      | 10      | 14      | 22      | -       | 26      | -       | 26      | -       | -       |
|@areg-           | 10      | 18      | 18      | 18      | 10      | 14      | 22      | -       | 26      | -       | 26      | -       | -       |
|dreg             | 2       | 10      | 10      | 10      | 2       | 6       | 14      | -       | 18      | -       | 18      | -       | -       |
|sp(ofs)          | 6       | 14      | 14      | 14      | 6       | 10      | 18      | -       | 22      | -       | 22      | -       | -       |
|@sp(ofs)         | 14      | 22      | 22      | 22      | 14      | 18      | 26      | -       | 30      | -       | 30      | -       | -       |
|ival             | 6       | 14      | 14      | 14      | 6       | 10      | 18      | -       | 22      | -       | 22      | -       | -       |
|@ival            | 18      | 26      | 26      | 26      | 18      | 22      | 30      | -       | 34      | -       | 34      | -       | -       |
|label            | 6       | 14      | 14      | 14      | 6       | 10      | 18      | -       | 22      | -       | 22      | -       | -       |
|@label           | 18      | 26      | 26      | 26      | 18      | 22      | 30      | -       | 34      | -       | 34      | -       | -       |
|freg             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|fval             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |


not
---

### Syntax
	not<.b/.w/.l> argA

### Function
Perform a bitwise NOT on the value at the location specified by argA.

### Operand Types and Cycle Counts

argA     | cycles
---------|-------
areg     | 2
@areg    | 10
@areg+   | 10
@areg-   | 10
dreg     | 2
sp(ofs)  | 6
@sp(ofs) | 14
ival     | -
@ival    | 18
label    | -
@label   | 18
freg     | -
fval     | -


bset
----

### Syntax
	bset<.b/.w/.l> argA, argB

### Function
Set the bit at bit index specified by argA, in the value at the location specified by argB. Bit indices start at 0 and increase with significance.

### Operand Types and Cycle Counts

|**argA**/**argB**| areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
|-----------------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|
|areg             | 2       | 10      | 10      | 10      | 2       | 6       | 14      | -       | 18      | -       | 18      | -       | -       |
|@areg            | 10      | 18      | 18      | 18      | 10      | 14      | 22      | -       | 26      | -       | 26      | -       | -       |
|@areg+           | 10      | 18      | 18      | 18      | 10      | 14      | 22      | -       | 26      | -       | 26      | -       | -       |
|@areg-           | 10      | 18      | 18      | 18      | 10      | 14      | 22      | -       | 26      | -       | 26      | -       | -       |
|dreg             | 2       | 10      | 10      | 10      | 2       | 6       | 14      | -       | 18      | -       | 18      | -       | -       |
|sp(ofs)          | 6       | 14      | 14      | 14      | 6       | 10      | 18      | -       | 22      | -       | 22      | -       | -       |
|@sp(ofs)         | 14      | 22      | 22      | 22      | 14      | 18      | 26      | -       | 30      | -       | 30      | -       | -       |
|ival             | 6       | 14      | 14      | 14      | 6       | 10      | 18      | -       | 22      | -       | 22      | -       | -       |
|@ival            | 18      | 26      | 26      | 26      | 18      | 22      | 30      | -       | 34      | -       | 34      | -       | -       |
|label            | 6       | 14      | 14      | 14      | 6       | 10      | 18      | -       | 22      | -       | 22      | -       | -       |
|@label           | 18      | 26      | 26      | 26      | 18      | 22      | 30      | -       | 34      | -       | 34      | -       | -       |
|freg             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|fval             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |


bclr
----

### Syntax
	bclr<.b/.w/.l> argA, argB

### Function
Clear the bit at bit index specified by argA, in the value at the location specified by argB. Bit indices start at 0 and increase with significance.

### Operand Types and Cycle Counts

|**argA**/**argB**| areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
|-----------------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|
|areg             | 2       | 10      | 10      | 10      | 2       | 6       | 14      | -       | 18      | -       | 18      | -       | -       |
|@areg            | 10      | 18      | 18      | 18      | 10      | 14      | 22      | -       | 26      | -       | 26      | -       | -       |
|@areg+           | 10      | 18      | 18      | 18      | 10      | 14      | 22      | -       | 26      | -       | 26      | -       | -       |
|@areg-           | 10      | 18      | 18      | 18      | 10      | 14      | 22      | -       | 26      | -       | 26      | -       | -       |
|dreg             | 2       | 10      | 10      | 10      | 2       | 6       | 14      | -       | 18      | -       | 18      | -       | -       |
|sp(ofs)          | 6       | 14      | 14      | 14      | 6       | 10      | 18      | -       | 22      | -       | 22      | -       | -       |
|@sp(ofs)         | 14      | 22      | 22      | 22      | 14      | 18      | 26      | -       | 30      | -       | 30      | -       | -       |
|ival             | 6       | 14      | 14      | 14      | 6       | 10      | 18      | -       | 22      | -       | 22      | -       | -       |
|@ival            | 18      | 26      | 26      | 26      | 18      | 22      | 30      | -       | 34      | -       | 34      | -       | -       |
|label            | 6       | 14      | 14      | 14      | 6       | 10      | 18      | -       | 22      | -       | 22      | -       | -       |
|@label           | 18      | 26      | 26      | 26      | 18      | 22      | 30      | -       | 34      | -       | 34      | -       | -       |
|freg             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|fval             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |


btst
----

### Syntax
	btst<.b/.w/.l> argA, argB

### Function
Test the bit at bit index specified by argA, in the value at the location specified by argB. Bit indices start at 0 and increase with significance. The result is used in the next call to jz or jnz.

### Operand Types and Cycle Counts

|**argA**/**argB**| areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
|-----------------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|
|areg             | 2       | 10      | 10      | 10      | 2       | 6       | 14      | -       | 18      | -       | 18      | -       | -       |
|@areg            | 10      | 18      | 18      | 18      | 10      | 14      | 22      | -       | 26      | -       | 26      | -       | -       |
|@areg+           | 10      | 18      | 18      | 18      | 10      | 14      | 22      | -       | 26      | -       | 26      | -       | -       |
|@areg-           | 10      | 18      | 18      | 18      | 10      | 14      | 22      | -       | 26      | -       | 26      | -       | -       |
|dreg             | 2       | 10      | 10      | 10      | 2       | 6       | 14      | -       | 18      | -       | 18      | -       | -       |
|sp(ofs)          | 6       | 14      | 14      | 14      | 6       | 10      | 18      | -       | 22      | -       | 22      | -       | -       |
|@sp(ofs)         | 14      | 22      | 22      | 22      | 14      | 18      | 26      | -       | 30      | -       | 30      | -       | -       |
|ival             | 6       | 14      | 14      | 14      | 6       | 10      | 18      | -       | 22      | -       | 22      | -       | -       |
|@ival            | 18      | 26      | 26      | 26      | 18      | 22      | 30      | -       | 34      | -       | 34      | -       | -       |
|label            | 6       | 14      | 14      | 14      | 6       | 10      | 18      | -       | 22      | -       | 22      | -       | -       |
|@label           | 18      | 26      | 26      | 26      | 18      | 22      | 30      | -       | 34      | -       | 34      | -       | -       |
|freg             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|fval             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |


bchg
----

### Syntax
	bchg<.b/.w/.l> argA, argB

### Function
Flip the bit at bit index specified by argA, in the value at the location specified by argB. Bit indices start at 0 and increase with significance.

### Operand Types and Cycle Counts

|**argA**/**argB**| areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
|-----------------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|
|areg             | 2       | 10      | 10      | 10      | 2       | 6       | 14      | -       | 18      | -       | 18      | -       | -       |
|@areg            | 10      | 18      | 18      | 18      | 10      | 14      | 22      | -       | 26      | -       | 26      | -       | -       |
|@areg+           | 10      | 18      | 18      | 18      | 10      | 14      | 22      | -       | 26      | -       | 26      | -       | -       |
|@areg-           | 10      | 18      | 18      | 18      | 10      | 14      | 22      | -       | 26      | -       | 26      | -       | -       |
|dreg             | 2       | 10      | 10      | 10      | 2       | 6       | 14      | -       | 18      | -       | 18      | -       | -       |
|sp(ofs)          | 6       | 14      | 14      | 14      | 6       | 10      | 18      | -       | 22      | -       | 22      | -       | -       |
|@sp(ofs)         | 14      | 22      | 22      | 22      | 14      | 18      | 26      | -       | 30      | -       | 30      | -       | -       |
|ival             | 6       | 14      | 14      | 14      | 6       | 10      | 18      | -       | 22      | -       | 22      | -       | -       |
|@ival            | 18      | 26      | 26      | 26      | 18      | 22      | 30      | -       | 34      | -       | 34      | -       | -       |
|label            | 6       | 14      | 14      | 14      | 6       | 10      | 18      | -       | 22      | -       | 22      | -       | -       |
|@label           | 18      | 26      | 26      | 26      | 18      | 22      | 30      | -       | 34      | -       | 34      | -       | -       |
|freg             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|fval             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |


rol
---

### Syntax
	rol<.b/.w/.l> argA, argB

### Function
Perform a bitwise left shift of the value at the location specified by argB. Number of bits to shift is the value specified by argA.

### Operand Types and Cycle Counts

|**argA**/**argB**| areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
|-----------------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|
|areg             | 2       | 10      | 10      | 10      | 2       | 6       | 14      | -       | 18      | -       | 18      | -       | -       |
|@areg            | 10      | 18      | 18      | 18      | 10      | 14      | 22      | -       | 26      | -       | 26      | -       | -       |
|@areg+           | 10      | 18      | 18      | 18      | 10      | 14      | 22      | -       | 26      | -       | 26      | -       | -       |
|@areg-           | 10      | 18      | 18      | 18      | 10      | 14      | 22      | -       | 26      | -       | 26      | -       | -       |
|dreg             | 2       | 10      | 10      | 10      | 2       | 6       | 14      | -       | 18      | -       | 18      | -       | -       |
|sp(ofs)          | 6       | 14      | 14      | 14      | 6       | 10      | 18      | -       | 22      | -       | 22      | -       | -       |
|@sp(ofs)         | 14      | 22      | 22      | 22      | 14      | 18      | 26      | -       | 30      | -       | 30      | -       | -       |
|ival             | 6       | 14      | 14      | 14      | 6       | 10      | 18      | -       | 22      | -       | 22      | -       | -       |
|@ival            | 18      | 26      | 26      | 26      | 18      | 22      | 30      | -       | 34      | -       | 34      | -       | -       |
|label            | 6       | 14      | 14      | 14      | 6       | 10      | 18      | -       | 22      | -       | 22      | -       | -       |
|@label           | 18      | 26      | 26      | 26      | 18      | 22      | 30      | -       | 34      | -       | 34      | -       | -       |
|freg             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|fval             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |


ror
---

### Syntax
	ror<.b/.w/.l> argA, argB

### Function
Perform a bitwise right shift of the value at the location specified by argB. Number of bits to shift is the value specified by argA.

### Operand Types and Cycle Counts

|**argA**/**argB**| areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
|-----------------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|
|areg             | 2       | 10      | 10      | 10      | 2       | 6       | 14      | -       | 18      | -       | 18      | -       | -       |
|@areg            | 10      | 18      | 18      | 18      | 10      | 14      | 22      | -       | 26      | -       | 26      | -       | -       |
|@areg+           | 10      | 18      | 18      | 18      | 10      | 14      | 22      | -       | 26      | -       | 26      | -       | -       |
|@areg-           | 10      | 18      | 18      | 18      | 10      | 14      | 22      | -       | 26      | -       | 26      | -       | -       |
|dreg             | 2       | 10      | 10      | 10      | 2       | 6       | 14      | -       | 18      | -       | 18      | -       | -       |
|sp(ofs)          | 6       | 14      | 14      | 14      | 6       | 10      | 18      | -       | 22      | -       | 22      | -       | -       |
|@sp(ofs)         | 14      | 22      | 22      | 22      | 14      | 18      | 26      | -       | 30      | -       | 30      | -       | -       |
|ival             | 6       | 14      | 14      | 14      | 6       | 10      | 18      | -       | 22      | -       | 22      | -       | -       |
|@ival            | 18      | 26      | 26      | 26      | 18      | 22      | 30      | -       | 34      | -       | 34      | -       | -       |
|label            | 6       | 14      | 14      | 14      | 6       | 10      | 18      | -       | 22      | -       | 22      | -       | -       |
|@label           | 18      | 26      | 26      | 26      | 18      | 22      | 30      | -       | 34      | -       | 34      | -       | -       |
|freg             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|fval             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |


mod
---

### Syntax
	mod<.b/.w/.l> argA, argB

### Function
Perform MOD on argB by argA and store the result at the location specified by argB.

### Operand Types and Cycle Counts

|**argA**/**argB**| areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
|-----------------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|
|areg             | 4       | 12      | 12      | 12      | 4       | 8       | 16      | -       | 20      | -       | 20      | 4       | -       |
|@areg            | 12      | 20      | 20      | 20      | 12      | 16      | 24      | -       | 28      | -       | 28      | 12      | -       |
|@areg+           | 12      | 20      | 20      | 20      | 12      | 16      | 24      | -       | 28      | -       | 28      | 12      | -       |
|@areg-           | 12      | 20      | 20      | 20      | 12      | 16      | 24      | -       | 28      | -       | 28      | 12      | -       |
|dreg             | 4       | 12      | 12      | 12      | 4       | 8       | 16      | -       | 20      | -       | 20      | 4       | -       |
|sp(ofs)          | 8       | 16      | 16      | 16      | 8       | 12      | 20      | -       | 24      | -       | 24      | 8       | -       |
|@sp(ofs)         | 16      | 24      | 24      | 24      | 16      | 20      | 28      | -       | 32      | -       | 32      | 16      | -       |
|ival             | 8       | 16      | 16      | 16      | 8       | 12      | 20      | -       | 24      | -       | 24      | 8       | -       |
|@ival            | 20      | 28      | 28      | 28      | 20      | 24      | 32      | -       | 36      | -       | 36      | 20      | -       |
|label            | 8       | 16      | 16      | 16      | 8       | 12      | 20      | -       | 24      | -       | 24      | 8       | -       |
|@label           | 20      | 28      | 28      | 28      | 20      | 24      | 32      | -       | 36      | -       | 36      | 20      | -       |
|freg             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|fval             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |


mod
---

### Syntax
	mod.f argA, argB

### Function
Perform MOD on argB by argA and store the result at the location specified by argB.

### Operand Types and Cycle Counts

|**argA**/**argB**| areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
|-----------------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|
|areg             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|@areg            | -       | 24      | -       | -       | -       | 20      | 28      | -       | 32      | -       | 32      | 16      | -       |
|@areg+           | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|@areg-           | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|dreg             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|sp(ofs)          | -       | 20      | -       | -       | -       | 16      | 24      | -       | 28      | -       | 28      | 12      | -       |
|@sp(ofs)         | -       | 28      | -       | -       | -       | 24      | 32      | -       | 36      | -       | 36      | 20      | -       |
|ival             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|@ival            | -       | 32      | -       | -       | -       | 28      | 36      | -       | 40      | -       | 40      | 24      | -       |
|label            | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|@label           | -       | 32      | -       | -       | -       | 28      | 36      | -       | 40      | -       | 40      | 24      | -       |
|freg             | -       | 16      | -       | -       | -       | 12      | 20      | -       | 24      | -       | 24      | 8       | -       |
|fval             | -       | 20      | -       | -       | -       | 16      | 24      | -       | 28      | -       | 28      | 12      | -       |


abs
---

### Syntax
	abs<.b/.w/.l> argA, argB

### Function
Perform ABS on argA and store the result at the location specified by argB.

### Operand Types and Cycle Counts

|**argA**/**argB**| areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
|-----------------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|
|areg             | 4       | 12      | 12      | 12      | 4       | 8       | 16      | -       | 20      | -       | 20      | 4       | -       |
|@areg            | 12      | 20      | 20      | 20      | 12      | 16      | 24      | -       | 28      | -       | 28      | 12      | -       |
|@areg+           | 12      | 20      | 20      | 20      | 12      | 16      | 24      | -       | 28      | -       | 28      | 12      | -       |
|@areg-           | 12      | 20      | 20      | 20      | 12      | 16      | 24      | -       | 28      | -       | 28      | 12      | -       |
|dreg             | 4       | 12      | 12      | 12      | 4       | 8       | 16      | -       | 20      | -       | 20      | 4       | -       |
|sp(ofs)          | 8       | 16      | 16      | 16      | 8       | 12      | 20      | -       | 24      | -       | 24      | 8       | -       |
|@sp(ofs)         | 16      | 24      | 24      | 24      | 16      | 20      | 28      | -       | 32      | -       | 32      | 16      | -       |
|ival             | 8       | 16      | 16      | 16      | 8       | 12      | 20      | -       | 24      | -       | 24      | 8       | -       |
|@ival            | 20      | 28      | 28      | 28      | 20      | 24      | 32      | -       | 36      | -       | 36      | 20      | -       |
|label            | 8       | 16      | 16      | 16      | 8       | 12      | 20      | -       | 24      | -       | 24      | 8       | -       |
|@label           | 20      | 28      | 28      | 28      | 20      | 24      | 32      | -       | 36      | -       | 36      | 20      | -       |
|freg             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|fval             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |


abs
---

### Syntax
	abs.f argA, argB

### Function
Perform ABS on argA and store the result at the location specified by argB.

### Operand Types and Cycle Counts

|**argA**/**argB**| areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
|-----------------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|
|areg             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|@areg            | -       | 24      | -       | -       | -       | 20      | 28      | -       | 32      | -       | 32      | 16      | -       |
|@areg+           | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|@areg-           | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|dreg             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|sp(ofs)          | -       | 20      | -       | -       | -       | 16      | 24      | -       | 28      | -       | 28      | 12      | -       |
|@sp(ofs)         | -       | 28      | -       | -       | -       | 24      | 32      | -       | 36      | -       | 36      | 20      | -       |
|ival             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|@ival            | -       | 32      | -       | -       | -       | 28      | 36      | -       | 40      | -       | 40      | 24      | -       |
|label            | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|@label           | -       | 32      | -       | -       | -       | 28      | 36      | -       | 40      | -       | 40      | 24      | -       |
|freg             | -       | 16      | -       | -       | -       | 12      | 20      | -       | 24      | -       | 24      | 8       | -       |
|fval             | -       | 20      | -       | -       | -       | 16      | 24      | -       | 28      | -       | 28      | 12      | -       |


pow
---

### Syntax
	pow<.b/.w/.l> argA, argB

### Function
Perform POW on argB by argA and store the result at the location specified by argB.

### Operand Types and Cycle Counts

|**argA**/**argB**| areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
|-----------------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|
|areg             | 4       | 12      | 12      | 12      | 4       | 8       | 16      | -       | 20      | -       | 20      | 4       | -       |
|@areg            | 12      | 20      | 20      | 20      | 12      | 16      | 24      | -       | 28      | -       | 28      | 12      | -       |
|@areg+           | 12      | 20      | 20      | 20      | 12      | 16      | 24      | -       | 28      | -       | 28      | 12      | -       |
|@areg-           | 12      | 20      | 20      | 20      | 12      | 16      | 24      | -       | 28      | -       | 28      | 12      | -       |
|dreg             | 4       | 12      | 12      | 12      | 4       | 8       | 16      | -       | 20      | -       | 20      | 4       | -       |
|sp(ofs)          | 8       | 16      | 16      | 16      | 8       | 12      | 20      | -       | 24      | -       | 24      | 8       | -       |
|@sp(ofs)         | 16      | 24      | 24      | 24      | 16      | 20      | 28      | -       | 32      | -       | 32      | 16      | -       |
|ival             | 8       | 16      | 16      | 16      | 8       | 12      | 20      | -       | 24      | -       | 24      | 8       | -       |
|@ival            | 20      | 28      | 28      | 28      | 20      | 24      | 32      | -       | 36      | -       | 36      | 20      | -       |
|label            | 8       | 16      | 16      | 16      | 8       | 12      | 20      | -       | 24      | -       | 24      | 8       | -       |
|@label           | 20      | 28      | 28      | 28      | 20      | 24      | 32      | -       | 36      | -       | 36      | 20      | -       |
|freg             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|fval             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |


pow
---

### Syntax
	pow.f argA, argB

### Function
Perform POW on argB by argA and store the result at the location specified by argB.

### Operand Types and Cycle Counts

|**argA**/**argB**| areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
|-----------------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|
|areg             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|@areg            | -       | 24      | -       | -       | -       | 20      | 28      | -       | 32      | -       | 32      | 16      | -       |
|@areg+           | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|@areg-           | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|dreg             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|sp(ofs)          | -       | 20      | -       | -       | -       | 16      | 24      | -       | 28      | -       | 28      | 12      | -       |
|@sp(ofs)         | -       | 28      | -       | -       | -       | 24      | 32      | -       | 36      | -       | 36      | 20      | -       |
|ival             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|@ival            | -       | 32      | -       | -       | -       | 28      | 36      | -       | 40      | -       | 40      | 24      | -       |
|label            | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|@label           | -       | 32      | -       | -       | -       | 28      | 36      | -       | 40      | -       | 40      | 24      | -       |
|freg             | -       | 16      | -       | -       | -       | 12      | 20      | -       | 24      | -       | 24      | 8       | -       |
|fval             | -       | 20      | -       | -       | -       | 16      | 24      | -       | 28      | -       | 28      | 12      | -       |


sin
---

### Syntax
	sin.f argA, argB

### Function
Calculate SIN of the value specified by argA, and store the result at the location specified by argB.

### Operand Types and Cycle Counts

|**argA**/**argB**| areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
|-----------------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|
|areg             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|@areg            | -       | 24      | -       | -       | -       | 20      | 28      | -       | 32      | -       | 32      | 16      | -       |
|@areg+           | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|@areg-           | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|dreg             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|sp(ofs)          | -       | 20      | -       | -       | -       | 16      | 24      | -       | 28      | -       | 28      | 12      | -       |
|@sp(ofs)         | -       | 28      | -       | -       | -       | 24      | 32      | -       | 36      | -       | 36      | 20      | -       |
|ival             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|@ival            | -       | 32      | -       | -       | -       | 28      | 36      | -       | 40      | -       | 40      | 24      | -       |
|label            | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|@label           | -       | 32      | -       | -       | -       | 28      | 36      | -       | 40      | -       | 40      | 24      | -       |
|freg             | -       | 16      | -       | -       | -       | 12      | 20      | -       | 24      | -       | 24      | 8       | -       |
|fval             | -       | 20      | -       | -       | -       | 16      | 24      | -       | 28      | -       | 28      | 12      | -       |


cos
---

### Syntax
	cos.f argA, argB

### Function
Calculate COS of the value specified by argA, and store the result at the location specified by argB.

### Operand Types and Cycle Counts

|**argA**/**argB**| areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
|-----------------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|
|areg             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|@areg            | -       | 24      | -       | -       | -       | 20      | 28      | -       | 32      | -       | 32      | 16      | -       |
|@areg+           | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|@areg-           | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|dreg             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|sp(ofs)          | -       | 20      | -       | -       | -       | 16      | 24      | -       | 28      | -       | 28      | 12      | -       |
|@sp(ofs)         | -       | 28      | -       | -       | -       | 24      | 32      | -       | 36      | -       | 36      | 20      | -       |
|ival             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|@ival            | -       | 32      | -       | -       | -       | 28      | 36      | -       | 40      | -       | 40      | 24      | -       |
|label            | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|@label           | -       | 32      | -       | -       | -       | 28      | 36      | -       | 40      | -       | 40      | 24      | -       |
|freg             | -       | 16      | -       | -       | -       | 12      | 20      | -       | 24      | -       | 24      | 8       | -       |
|fval             | -       | 20      | -       | -       | -       | 16      | 24      | -       | 28      | -       | 28      | 12      | -       |


tan
---

### Syntax
	tan.f argA, argB

### Function
Calculate TAN of the value specified by argA, and store the result at the location specified by argB.

### Operand Types and Cycle Counts

|**argA**/**argB**| areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
|-----------------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|
|areg             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|@areg            | -       | 24      | -       | -       | -       | 20      | 28      | -       | 32      | -       | 32      | 16      | -       |
|@areg+           | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|@areg-           | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|dreg             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|sp(ofs)          | -       | 20      | -       | -       | -       | 16      | 24      | -       | 28      | -       | 28      | 12      | -       |
|@sp(ofs)         | -       | 28      | -       | -       | -       | 24      | 32      | -       | 36      | -       | 36      | 20      | -       |
|ival             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|@ival            | -       | 32      | -       | -       | -       | 28      | 36      | -       | 40      | -       | 40      | 24      | -       |
|label            | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|@label           | -       | 32      | -       | -       | -       | 28      | 36      | -       | 40      | -       | 40      | 24      | -       |
|freg             | -       | 16      | -       | -       | -       | 12      | 20      | -       | 24      | -       | 24      | 8       | -       |
|fval             | -       | 20      | -       | -       | -       | 16      | 24      | -       | 28      | -       | 28      | 12      | -       |


asin
----

### Syntax
	asin.f argA, argB

### Function
Calculate ASIN of the value specified by argA, and store the result at the location specified by argB.

### Operand Types and Cycle Counts

|**argA**/**argB**| areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
|-----------------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|
|areg             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|@areg            | -       | 24      | -       | -       | -       | 20      | 28      | -       | 32      | -       | 32      | 16      | -       |
|@areg+           | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|@areg-           | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|dreg             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|sp(ofs)          | -       | 20      | -       | -       | -       | 16      | 24      | -       | 28      | -       | 28      | 12      | -       |
|@sp(ofs)         | -       | 28      | -       | -       | -       | 24      | 32      | -       | 36      | -       | 36      | 20      | -       |
|ival             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|@ival            | -       | 32      | -       | -       | -       | 28      | 36      | -       | 40      | -       | 40      | 24      | -       |
|label            | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|@label           | -       | 32      | -       | -       | -       | 28      | 36      | -       | 40      | -       | 40      | 24      | -       |
|freg             | -       | 16      | -       | -       | -       | 12      | 20      | -       | 24      | -       | 24      | 8       | -       |
|fval             | -       | 20      | -       | -       | -       | 16      | 24      | -       | 28      | -       | 28      | 12      | -       |


acos
----

### Syntax
	acos.f argA, argB

### Function
Calculate ACOS of the value specified by argA, and store the result at the location specified by argB.

### Operand Types and Cycle Counts

|**argA**/**argB**| areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
|-----------------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|
|areg             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|@areg            | -       | 24      | -       | -       | -       | 20      | 28      | -       | 32      | -       | 32      | 16      | -       |
|@areg+           | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|@areg-           | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|dreg             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|sp(ofs)          | -       | 20      | -       | -       | -       | 16      | 24      | -       | 28      | -       | 28      | 12      | -       |
|@sp(ofs)         | -       | 28      | -       | -       | -       | 24      | 32      | -       | 36      | -       | 36      | 20      | -       |
|ival             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|@ival            | -       | 32      | -       | -       | -       | 28      | 36      | -       | 40      | -       | 40      | 24      | -       |
|label            | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|@label           | -       | 32      | -       | -       | -       | 28      | 36      | -       | 40      | -       | 40      | 24      | -       |
|freg             | -       | 16      | -       | -       | -       | 12      | 20      | -       | 24      | -       | 24      | 8       | -       |
|fval             | -       | 20      | -       | -       | -       | 16      | 24      | -       | 28      | -       | 28      | 12      | -       |


atan
----

### Syntax
	atan.f argA, argB

### Function
Calculate ATAN of the value specified by argA, and store the result at the location specified by argB.

### Operand Types and Cycle Counts

|**argA**/**argB**| areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
|-----------------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|
|areg             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|@areg            | -       | 24      | -       | -       | -       | 20      | 28      | -       | 32      | -       | 32      | 16      | -       |
|@areg+           | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|@areg-           | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|dreg             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|sp(ofs)          | -       | 20      | -       | -       | -       | 16      | 24      | -       | 28      | -       | 28      | 12      | -       |
|@sp(ofs)         | -       | 28      | -       | -       | -       | 24      | 32      | -       | 36      | -       | 36      | 20      | -       |
|ival             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|@ival            | -       | 32      | -       | -       | -       | 28      | 36      | -       | 40      | -       | 40      | 24      | -       |
|label            | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|@label           | -       | 32      | -       | -       | -       | 28      | 36      | -       | 40      | -       | 40      | 24      | -       |
|freg             | -       | 16      | -       | -       | -       | 12      | 20      | -       | 24      | -       | 24      | 8       | -       |
|fval             | -       | 20      | -       | -       | -       | 16      | 24      | -       | 28      | -       | 28      | 12      | -       |


exp
---

### Syntax
	exp.f argA, argB

### Function
Calculate EXP of the value specified by argA, and store the result at the location specified by argB.

### Operand Types and Cycle Counts

|**argA**/**argB**| areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
|-----------------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|
|areg             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|@areg            | -       | 24      | -       | -       | -       | 20      | 28      | -       | 32      | -       | 32      | 16      | -       |
|@areg+           | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|@areg-           | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|dreg             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|sp(ofs)          | -       | 20      | -       | -       | -       | 16      | 24      | -       | 28      | -       | 28      | 12      | -       |
|@sp(ofs)         | -       | 28      | -       | -       | -       | 24      | 32      | -       | 36      | -       | 36      | 20      | -       |
|ival             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|@ival            | -       | 32      | -       | -       | -       | 28      | 36      | -       | 40      | -       | 40      | 24      | -       |
|label            | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|@label           | -       | 32      | -       | -       | -       | 28      | 36      | -       | 40      | -       | 40      | 24      | -       |
|freg             | -       | 16      | -       | -       | -       | 12      | 20      | -       | 24      | -       | 24      | 8       | -       |
|fval             | -       | 20      | -       | -       | -       | 16      | 24      | -       | 28      | -       | 28      | 12      | -       |


log
---

### Syntax
	log.f argA, argB

### Function
Calculate LOG of the value specified by argA, and store the result at the location specified by argB.

### Operand Types and Cycle Counts

|**argA**/**argB**| areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
|-----------------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|
|areg             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|@areg            | -       | 24      | -       | -       | -       | 20      | 28      | -       | 32      | -       | 32      | 16      | -       |
|@areg+           | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|@areg-           | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|dreg             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|sp(ofs)          | -       | 20      | -       | -       | -       | 16      | 24      | -       | 28      | -       | 28      | 12      | -       |
|@sp(ofs)         | -       | 28      | -       | -       | -       | 24      | 32      | -       | 36      | -       | 36      | 20      | -       |
|ival             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|@ival            | -       | 32      | -       | -       | -       | 28      | 36      | -       | 40      | -       | 40      | 24      | -       |
|label            | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|@label           | -       | 32      | -       | -       | -       | 28      | 36      | -       | 40      | -       | 40      | 24      | -       |
|freg             | -       | 16      | -       | -       | -       | 12      | 20      | -       | 24      | -       | 24      | 8       | -       |
|fval             | -       | 20      | -       | -       | -       | 16      | 24      | -       | 28      | -       | 28      | 12      | -       |


sqrt
----

### Syntax
	sqrt.f argA, argB

### Function
Calculate SQRT of the value specified by argA, and store the result at the location specified by argB.

### Operand Types and Cycle Counts

|**argA**/**argB**| areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
|-----------------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|
|areg             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|@areg            | -       | 24      | -       | -       | -       | 20      | 28      | -       | 32      | -       | 32      | 16      | -       |
|@areg+           | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|@areg-           | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|dreg             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|sp(ofs)          | -       | 20      | -       | -       | -       | 16      | 24      | -       | 28      | -       | 28      | 12      | -       |
|@sp(ofs)         | -       | 28      | -       | -       | -       | 24      | 32      | -       | 36      | -       | 36      | 20      | -       |
|ival             | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|@ival            | -       | 32      | -       | -       | -       | 28      | 36      | -       | 40      | -       | 40      | 24      | -       |
|label            | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       | -       |
|@label           | -       | 32      | -       | -       | -       | 28      | 36      | -       | 40      | -       | 40      | 24      | -       |
|freg             | -       | 16      | -       | -       | -       | 12      | 20      | -       | 24      | -       | 24      | 8       | -       |
|fval             | -       | 20      | -       | -       | -       | 16      | 24      | -       | 28      | -       | 28      | 12      | -       |

