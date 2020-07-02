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
No operands. Cycle count: 4


jsr
---

### Syntax
	jsr argA

### Function
Jump to subroutine at location specified by argA. Will push the current value of IP onto the stack before the jump.

### Operand Types and Cycle Counts

argA     | cycles
---------|-------
areg     | 8
@areg    | 16
@areg+   | 16
@areg-   | 16
dreg     | 8
sp(ofs)  | 12
@sp(ofs) | 20
ival     | 12
@ival    | 24
label    | 12
@label   | 24
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
areg     | 8
@areg    | 16
@areg+   | 16
@areg-   | 16
dreg     | 8
sp(ofs)  | 12
@sp(ofs) | 20
ival     | 12
@ival    | 24
label    | 12
@label   | 24
freg     | -
fval     | -


ret
---

### Syntax
	ret

### Function
Return from subroutine call made by jsr. Will pop the return address off the stack.

### Operand Types and Cycle Counts
No operands. Cycle count: 8


sys
---

### Syntax
	sys argA

### Function
Make a BIOS call. See the BIOS documentation for available BIOS calls.

### Operand Types and Cycle Counts

argA     | cycles
---------|-------
areg     | 64
@areg    | 72
@areg+   | 72
@areg-   | 72
dreg     | 64
sp(ofs)  | 68
@sp(ofs) | 76
ival     | 68
@ival    | 80
label    | 68
@label   | 80
freg     | -
fval     | -


move
----

### Syntax
	move<.b/.w/.l/.f> argA, argB

### Function
Move the value specified by argA to the location specified by argB.

### Operand Types and Cycle Counts

         |**argB** | areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) |
---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |
areg     |         | 4       | 12      | 12      | 12      | 4       | 8       |
@areg    |         | 12      | 20      | 20      | 20      | 12      | 16      |
@areg+   |         | 12      | 20      | 20      | 20      | 12      | 16      |
@areg-   |         | 12      | 20      | 20      | 20      | 12      | 16      |
dreg     |         | 4       | 12      | 12      | 12      | 4       | 8       |
sp(ofs)  |         | 8       | 16      | 16      | 16      | 8       | 12      |
@sp(ofs) |         | 16      | 24      | 24      | 24      | 16      | 20      |
ival     |         | 8       | 16      | 16      | 16      | 8       | 12      |
@ival    |         | 20      | 28      | 28      | 28      | 20      | 24      |
label    |         | 8       | 16      | 16      | 16      | 8       | 12      |
@label   |         | 20      | 28      | 28      | 28      | 20      | 24      |
freg     |         | 4       | 12      | 12      | 12      | 4       | 8       |
fval     |         | 8       | 16      | 16      | 16      | 8       | 12      |


         |**argB** | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
---------|---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |         |
areg     |         | 16      | -       | 20      | -       | 20      | 4       | -       |
@areg    |         | 24      | -       | 28      | -       | 28      | 12      | -       |
@areg+   |         | 24      | -       | 28      | -       | 28      | 12      | -       |
@areg-   |         | 24      | -       | 28      | -       | 28      | 12      | -       |
dreg     |         | 16      | -       | 20      | -       | 20      | 4       | -       |
sp(ofs)  |         | 20      | -       | 24      | -       | 24      | 8       | -       |
@sp(ofs) |         | 28      | -       | 32      | -       | 32      | 16      | -       |
ival     |         | 20      | -       | 24      | -       | 24      | 8       | -       |
@ival    |         | 32      | -       | 36      | -       | 36      | 20      | -       |
label    |         | 20      | -       | 24      | -       | 24      | 8       | -       |
@label   |         | 32      | -       | 36      | -       | 36      | 20      | -       |
freg     |         | 16      | -       | 20      | -       | 20      | 4       | -       |
fval     |         | 20      | -       | 24      | -       | 24      | 8       | -       |


xmov
----

### Syntax
	xmov<.b/.w/.l/.f> argA, argB

### Function
Move the value specified by argA to the location specified by argB. This will be repeated 16 times.

### Operand Types and Cycle Counts

         |**argB** | areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) |
---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |
areg     |         | -       | -       | -       | -       | -       | -       |
@areg    |         | -       | 32      | 32      | 32      | -       | -       |
@areg+   |         | -       | 32      | 32      | 32      | -       | -       |
@areg-   |         | -       | 32      | 32      | 32      | -       | -       |
dreg     |         | -       | -       | -       | -       | -       | -       |
sp(ofs)  |         | -       | -       | -       | -       | -       | -       |
@sp(ofs) |         | -       | -       | -       | -       | -       | -       |
ival     |         | -       | -       | -       | -       | -       | -       |
@ival    |         | -       | -       | -       | -       | -       | -       |
label    |         | -       | -       | -       | -       | -       | -       |
@label   |         | -       | -       | -       | -       | -       | -       |
freg     |         | -       | -       | -       | -       | -       | -       |
fval     |         | -       | -       | -       | -       | -       | -       |


         |**argB** | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
---------|---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |         |
areg     |         | -       | -       | -       | -       | -       | -       | -       |
@areg    |         | -       | -       | -       | -       | -       | -       | -       |
@areg+   |         | -       | -       | -       | -       | -       | -       | -       |
@areg-   |         | -       | -       | -       | -       | -       | -       | -       |
dreg     |         | -       | -       | -       | -       | -       | -       | -       |
sp(ofs)  |         | -       | -       | -       | -       | -       | -       | -       |
@sp(ofs) |         | -       | -       | -       | -       | -       | -       | -       |
ival     |         | -       | -       | -       | -       | -       | -       | -       |
@ival    |         | -       | -       | -       | -       | -       | -       | -       |
label    |         | -       | -       | -       | -       | -       | -       | -       |
@label   |         | -       | -       | -       | -       | -       | -       | -       |
freg     |         | -       | -       | -       | -       | -       | -       | -       |
fval     |         | -       | -       | -       | -       | -       | -       | -       |


add
---

### Syntax
	add<.b/.w/.l> argA, argB

### Function
Add the value specified by argA to the value at the location specified by argB, and store the result at the location specified by argB.

### Operand Types and Cycle Counts

         |**argB** | areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) |
---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |
areg     |         | 8       | 16      | 16      | 16      | 8       | 12      |
@areg    |         | 16      | 24      | 24      | 24      | 16      | 20      |
@areg+   |         | 16      | 24      | 24      | 24      | 16      | 20      |
@areg-   |         | 16      | 24      | 24      | 24      | 16      | 20      |
dreg     |         | 8       | 16      | 16      | 16      | 8       | 12      |
sp(ofs)  |         | 12      | 20      | 20      | 20      | 12      | 16      |
@sp(ofs) |         | 20      | 28      | 28      | 28      | 20      | 24      |
ival     |         | 12      | 20      | 20      | 20      | 12      | 16      |
@ival    |         | 24      | 32      | 32      | 32      | 24      | 28      |
label    |         | 12      | 20      | 20      | 20      | 12      | 16      |
@label   |         | 24      | 32      | 32      | 32      | 24      | 28      |
freg     |         | -       | -       | -       | -       | -       | -       |
fval     |         | -       | -       | -       | -       | -       | -       |


         |**argB** | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
---------|---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |         |
areg     |         | 20      | -       | 24      | -       | 24      | -       | -       |
@areg    |         | 28      | -       | 32      | -       | 32      | -       | -       |
@areg+   |         | 28      | -       | 32      | -       | 32      | -       | -       |
@areg-   |         | 28      | -       | 32      | -       | 32      | -       | -       |
dreg     |         | 20      | -       | 24      | -       | 24      | -       | -       |
sp(ofs)  |         | 24      | -       | 28      | -       | 28      | -       | -       |
@sp(ofs) |         | 32      | -       | 36      | -       | 36      | -       | -       |
ival     |         | 24      | -       | 28      | -       | 28      | -       | -       |
@ival    |         | 36      | -       | 40      | -       | 40      | -       | -       |
label    |         | 24      | -       | 28      | -       | 28      | -       | -       |
@label   |         | 36      | -       | 40      | -       | 40      | -       | -       |
freg     |         | -       | -       | -       | -       | -       | -       | -       |
fval     |         | -       | -       | -       | -       | -       | -       | -       |


add
---

### Syntax
	add.f argA, argB

### Function
Add the value specified by argA to the value at the location specified by argB, and store the result at the location specified by argB.

### Operand Types and Cycle Counts

         |**argB** | areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) |
---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |
areg     |         | -       | -       | -       | -       | -       | -       |
@areg    |         | -       | 28      | -       | -       | -       | 24      |
@areg+   |         | -       | -       | -       | -       | -       | -       |
@areg-   |         | -       | -       | -       | -       | -       | -       |
dreg     |         | -       | -       | -       | -       | -       | -       |
sp(ofs)  |         | -       | 24      | -       | -       | -       | 20      |
@sp(ofs) |         | -       | 32      | -       | -       | -       | 28      |
ival     |         | -       | -       | -       | -       | -       | -       |
@ival    |         | -       | 36      | -       | -       | -       | 32      |
label    |         | -       | -       | -       | -       | -       | -       |
@label   |         | -       | 36      | -       | -       | -       | 32      |
freg     |         | -       | 20      | -       | -       | -       | 16      |
fval     |         | -       | 24      | -       | -       | -       | 20      |


         |**argB** | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
---------|---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |         |
areg     |         | -       | -       | -       | -       | -       | -       | -       |
@areg    |         | 32      | -       | 36      | -       | 36      | 20      | -       |
@areg+   |         | -       | -       | -       | -       | -       | -       | -       |
@areg-   |         | -       | -       | -       | -       | -       | -       | -       |
dreg     |         | -       | -       | -       | -       | -       | -       | -       |
sp(ofs)  |         | 28      | -       | 32      | -       | 32      | 16      | -       |
@sp(ofs) |         | 36      | -       | 40      | -       | 40      | 24      | -       |
ival     |         | -       | -       | -       | -       | -       | -       | -       |
@ival    |         | 40      | -       | 44      | -       | 44      | 28      | -       |
label    |         | -       | -       | -       | -       | -       | -       | -       |
@label   |         | 40      | -       | 44      | -       | 44      | 28      | -       |
freg     |         | 24      | -       | 28      | -       | 28      | 12      | -       |
fval     |         | 28      | -       | 32      | -       | 32      | 16      | -       |


sub
---

### Syntax
	sub<.b/.w/.l> argA, argB

### Function
Subtract the value specified by argA from the value at the location specified by argB, and store the result at the location specified by argB.

### Operand Types and Cycle Counts

         |**argB** | areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) |
---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |
areg     |         | 8       | 16      | 16      | 16      | 8       | 12      |
@areg    |         | 16      | 24      | 24      | 24      | 16      | 20      |
@areg+   |         | 16      | 24      | 24      | 24      | 16      | 20      |
@areg-   |         | 16      | 24      | 24      | 24      | 16      | 20      |
dreg     |         | 8       | 16      | 16      | 16      | 8       | 12      |
sp(ofs)  |         | 12      | 20      | 20      | 20      | 12      | 16      |
@sp(ofs) |         | 20      | 28      | 28      | 28      | 20      | 24      |
ival     |         | 12      | 20      | 20      | 20      | 12      | 16      |
@ival    |         | 24      | 32      | 32      | 32      | 24      | 28      |
label    |         | 12      | 20      | 20      | 20      | 12      | 16      |
@label   |         | 24      | 32      | 32      | 32      | 24      | 28      |
freg     |         | -       | -       | -       | -       | -       | -       |
fval     |         | -       | -       | -       | -       | -       | -       |


         |**argB** | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
---------|---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |         |
areg     |         | 20      | -       | 24      | -       | 24      | -       | -       |
@areg    |         | 28      | -       | 32      | -       | 32      | -       | -       |
@areg+   |         | 28      | -       | 32      | -       | 32      | -       | -       |
@areg-   |         | 28      | -       | 32      | -       | 32      | -       | -       |
dreg     |         | 20      | -       | 24      | -       | 24      | -       | -       |
sp(ofs)  |         | 24      | -       | 28      | -       | 28      | -       | -       |
@sp(ofs) |         | 32      | -       | 36      | -       | 36      | -       | -       |
ival     |         | 24      | -       | 28      | -       | 28      | -       | -       |
@ival    |         | 36      | -       | 40      | -       | 40      | -       | -       |
label    |         | 24      | -       | 28      | -       | 28      | -       | -       |
@label   |         | 36      | -       | 40      | -       | 40      | -       | -       |
freg     |         | -       | -       | -       | -       | -       | -       | -       |
fval     |         | -       | -       | -       | -       | -       | -       | -       |


sub
---

### Syntax
	sub.f argA, argB

### Function
Subtract the value specified by argA from the value at the location specified by argB, and store the result at the location specified by argB.

### Operand Types and Cycle Counts

         |**argB** | areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) |
---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |
areg     |         | -       | -       | -       | -       | -       | -       |
@areg    |         | -       | 28      | -       | -       | -       | 24      |
@areg+   |         | -       | -       | -       | -       | -       | -       |
@areg-   |         | -       | -       | -       | -       | -       | -       |
dreg     |         | -       | -       | -       | -       | -       | -       |
sp(ofs)  |         | -       | 24      | -       | -       | -       | 20      |
@sp(ofs) |         | -       | 32      | -       | -       | -       | 28      |
ival     |         | -       | -       | -       | -       | -       | -       |
@ival    |         | -       | 36      | -       | -       | -       | 32      |
label    |         | -       | -       | -       | -       | -       | -       |
@label   |         | -       | 36      | -       | -       | -       | 32      |
freg     |         | -       | 20      | -       | -       | -       | 16      |
fval     |         | -       | 24      | -       | -       | -       | 20      |


         |**argB** | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
---------|---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |         |
areg     |         | -       | -       | -       | -       | -       | -       | -       |
@areg    |         | 32      | -       | 36      | -       | 36      | 20      | -       |
@areg+   |         | -       | -       | -       | -       | -       | -       | -       |
@areg-   |         | -       | -       | -       | -       | -       | -       | -       |
dreg     |         | -       | -       | -       | -       | -       | -       | -       |
sp(ofs)  |         | 28      | -       | 32      | -       | 32      | 16      | -       |
@sp(ofs) |         | 36      | -       | 40      | -       | 40      | 24      | -       |
ival     |         | -       | -       | -       | -       | -       | -       | -       |
@ival    |         | 40      | -       | 44      | -       | 44      | 28      | -       |
label    |         | -       | -       | -       | -       | -       | -       | -       |
@label   |         | 40      | -       | 44      | -       | 44      | 28      | -       |
freg     |         | 24      | -       | 28      | -       | 28      | 12      | -       |
fval     |         | 28      | -       | 32      | -       | 32      | 16      | -       |


neg
---

### Syntax
	neg<.b/.w/.l> argA

### Function
Negate the value at the location specified by argA

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
@areg    | 12
@areg+   | -
@areg-   | -
dreg     | -
sp(ofs)  | 8
@sp(ofs) | 16
ival     | -
@ival    | 20
label    | -
@label   | 20
freg     | 4
fval     | -


muls
----

### Syntax
	muls<.b/.w/.l> argA, argB

### Function
Multiply the value specified by argA and the value at the location specified by argB, and store the result at the location specified by argB. Treat argA and argB as signed integer numbers.

### Operand Types and Cycle Counts

         |**argB** | areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) |
---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |
areg     |         | 12      | 20      | 20      | 20      | 12      | 16      |
@areg    |         | 20      | 28      | 28      | 28      | 20      | 24      |
@areg+   |         | 20      | 28      | 28      | 28      | 20      | 24      |
@areg-   |         | 20      | 28      | 28      | 28      | 20      | 24      |
dreg     |         | 12      | 20      | 20      | 20      | 12      | 16      |
sp(ofs)  |         | 16      | 24      | 24      | 24      | 16      | 20      |
@sp(ofs) |         | 24      | 32      | 32      | 32      | 24      | 28      |
ival     |         | 16      | 24      | 24      | 24      | 16      | 20      |
@ival    |         | 28      | 36      | 36      | 36      | 28      | 32      |
label    |         | 16      | 24      | 24      | 24      | 16      | 20      |
@label   |         | 28      | 36      | 36      | 36      | 28      | 32      |
freg     |         | -       | -       | -       | -       | -       | -       |
fval     |         | -       | -       | -       | -       | -       | -       |


         |**argB** | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
---------|---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |         |
areg     |         | 24      | -       | 28      | -       | 28      | -       | -       |
@areg    |         | 32      | -       | 36      | -       | 36      | -       | -       |
@areg+   |         | 32      | -       | 36      | -       | 36      | -       | -       |
@areg-   |         | 32      | -       | 36      | -       | 36      | -       | -       |
dreg     |         | 24      | -       | 28      | -       | 28      | -       | -       |
sp(ofs)  |         | 28      | -       | 32      | -       | 32      | -       | -       |
@sp(ofs) |         | 36      | -       | 40      | -       | 40      | -       | -       |
ival     |         | 28      | -       | 32      | -       | 32      | -       | -       |
@ival    |         | 40      | -       | 44      | -       | 44      | -       | -       |
label    |         | 28      | -       | 32      | -       | 32      | -       | -       |
@label   |         | 40      | -       | 44      | -       | 44      | -       | -       |
freg     |         | -       | -       | -       | -       | -       | -       | -       |
fval     |         | -       | -       | -       | -       | -       | -       | -       |


mulu
----

### Syntax
	mulu<.b/.w/.l> argA, argB

### Function
Multiply the value specified by argA and the value at the location specified by argB, and store the result at the location specified by argB. Treat argA and argB as unsigned integer numbers.

### Operand Types and Cycle Counts

         |**argB** | areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) |
---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |
areg     |         | 12      | 20      | 20      | 20      | 12      | 16      |
@areg    |         | 20      | 28      | 28      | 28      | 20      | 24      |
@areg+   |         | 20      | 28      | 28      | 28      | 20      | 24      |
@areg-   |         | 20      | 28      | 28      | 28      | 20      | 24      |
dreg     |         | 12      | 20      | 20      | 20      | 12      | 16      |
sp(ofs)  |         | 16      | 24      | 24      | 24      | 16      | 20      |
@sp(ofs) |         | 24      | 32      | 32      | 32      | 24      | 28      |
ival     |         | 16      | 24      | 24      | 24      | 16      | 20      |
@ival    |         | 28      | 36      | 36      | 36      | 28      | 32      |
label    |         | 16      | 24      | 24      | 24      | 16      | 20      |
@label   |         | 28      | 36      | 36      | 36      | 28      | 32      |
freg     |         | -       | -       | -       | -       | -       | -       |
fval     |         | -       | -       | -       | -       | -       | -       |


         |**argB** | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
---------|---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |         |
areg     |         | 24      | -       | 28      | -       | 28      | -       | -       |
@areg    |         | 32      | -       | 36      | -       | 36      | -       | -       |
@areg+   |         | 32      | -       | 36      | -       | 36      | -       | -       |
@areg-   |         | 32      | -       | 36      | -       | 36      | -       | -       |
dreg     |         | 24      | -       | 28      | -       | 28      | -       | -       |
sp(ofs)  |         | 28      | -       | 32      | -       | 32      | -       | -       |
@sp(ofs) |         | 36      | -       | 40      | -       | 40      | -       | -       |
ival     |         | 28      | -       | 32      | -       | 32      | -       | -       |
@ival    |         | 40      | -       | 44      | -       | 44      | -       | -       |
label    |         | 28      | -       | 32      | -       | 32      | -       | -       |
@label   |         | 40      | -       | 44      | -       | 44      | -       | -       |
freg     |         | -       | -       | -       | -       | -       | -       | -       |
fval     |         | -       | -       | -       | -       | -       | -       | -       |


mul
---

### Syntax
	mul.f argA, argB

### Function
Multiply the value specified by argA and the value at the location specified by argB, and store the result at the location specified by argB. Treat argA and argB as floating point numbers.

### Operand Types and Cycle Counts

         |**argB** | areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) |
---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |
areg     |         | -       | -       | -       | -       | -       | -       |
@areg    |         | -       | 32      | -       | -       | -       | 28      |
@areg+   |         | -       | -       | -       | -       | -       | -       |
@areg-   |         | -       | -       | -       | -       | -       | -       |
dreg     |         | -       | -       | -       | -       | -       | -       |
sp(ofs)  |         | -       | 28      | -       | -       | -       | 24      |
@sp(ofs) |         | -       | 36      | -       | -       | -       | 32      |
ival     |         | -       | -       | -       | -       | -       | -       |
@ival    |         | -       | 40      | -       | -       | -       | 36      |
label    |         | -       | -       | -       | -       | -       | -       |
@label   |         | -       | 40      | -       | -       | -       | 36      |
freg     |         | -       | 24      | -       | -       | -       | 20      |
fval     |         | -       | 28      | -       | -       | -       | 24      |


         |**argB** | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
---------|---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |         |
areg     |         | -       | -       | -       | -       | -       | -       | -       |
@areg    |         | 36      | -       | 40      | -       | 40      | 24      | -       |
@areg+   |         | -       | -       | -       | -       | -       | -       | -       |
@areg-   |         | -       | -       | -       | -       | -       | -       | -       |
dreg     |         | -       | -       | -       | -       | -       | -       | -       |
sp(ofs)  |         | 32      | -       | 36      | -       | 36      | 20      | -       |
@sp(ofs) |         | 40      | -       | 44      | -       | 44      | 28      | -       |
ival     |         | -       | -       | -       | -       | -       | -       | -       |
@ival    |         | 44      | -       | 48      | -       | 48      | 32      | -       |
label    |         | -       | -       | -       | -       | -       | -       | -       |
@label   |         | 44      | -       | 48      | -       | 48      | 32      | -       |
freg     |         | 28      | -       | 32      | -       | 32      | 16      | -       |
fval     |         | 32      | -       | 36      | -       | 36      | 20      | -       |


divs
----

### Syntax
	divs<.b/.w/.l> argA, argB

### Function
Divide the value at the location specified by argB by the value specified by argA, and store the result at the location specified by argB. Treat argA and argB as signed integer numbers.

### Operand Types and Cycle Counts

         |**argB** | areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) |
---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |
areg     |         | 16      | 24      | 24      | 24      | 16      | 20      |
@areg    |         | 24      | 32      | 32      | 32      | 24      | 28      |
@areg+   |         | 24      | 32      | 32      | 32      | 24      | 28      |
@areg-   |         | 24      | 32      | 32      | 32      | 24      | 28      |
dreg     |         | 16      | 24      | 24      | 24      | 16      | 20      |
sp(ofs)  |         | 20      | 28      | 28      | 28      | 20      | 24      |
@sp(ofs) |         | 28      | 36      | 36      | 36      | 28      | 32      |
ival     |         | 20      | 28      | 28      | 28      | 20      | 24      |
@ival    |         | 32      | 40      | 40      | 40      | 32      | 36      |
label    |         | 20      | 28      | 28      | 28      | 20      | 24      |
@label   |         | 32      | 40      | 40      | 40      | 32      | 36      |
freg     |         | -       | -       | -       | -       | -       | -       |
fval     |         | -       | -       | -       | -       | -       | -       |


         |**argB** | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
---------|---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |         |
areg     |         | 28      | -       | 32      | -       | 32      | -       | -       |
@areg    |         | 36      | -       | 40      | -       | 40      | -       | -       |
@areg+   |         | 36      | -       | 40      | -       | 40      | -       | -       |
@areg-   |         | 36      | -       | 40      | -       | 40      | -       | -       |
dreg     |         | 28      | -       | 32      | -       | 32      | -       | -       |
sp(ofs)  |         | 32      | -       | 36      | -       | 36      | -       | -       |
@sp(ofs) |         | 40      | -       | 44      | -       | 44      | -       | -       |
ival     |         | 32      | -       | 36      | -       | 36      | -       | -       |
@ival    |         | 44      | -       | 48      | -       | 48      | -       | -       |
label    |         | 32      | -       | 36      | -       | 36      | -       | -       |
@label   |         | 44      | -       | 48      | -       | 48      | -       | -       |
freg     |         | -       | -       | -       | -       | -       | -       | -       |
fval     |         | -       | -       | -       | -       | -       | -       | -       |


divu
----

### Syntax
	divu<.b/.w/.l> argA, argB

### Function
Divide the value at the location specified by argB by the value specified by argA, and store the result at the location specified by argB. Treat argA and argB as unsigned integer numbers.

### Operand Types and Cycle Counts

         |**argB** | areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) |
---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |
areg     |         | 16      | 24      | 24      | 24      | 16      | 20      |
@areg    |         | 24      | 32      | 32      | 32      | 24      | 28      |
@areg+   |         | 24      | 32      | 32      | 32      | 24      | 28      |
@areg-   |         | 24      | 32      | 32      | 32      | 24      | 28      |
dreg     |         | 16      | 24      | 24      | 24      | 16      | 20      |
sp(ofs)  |         | 20      | 28      | 28      | 28      | 20      | 24      |
@sp(ofs) |         | 28      | 36      | 36      | 36      | 28      | 32      |
ival     |         | 20      | 28      | 28      | 28      | 20      | 24      |
@ival    |         | 32      | 40      | 40      | 40      | 32      | 36      |
label    |         | 20      | 28      | 28      | 28      | 20      | 24      |
@label   |         | 32      | 40      | 40      | 40      | 32      | 36      |
freg     |         | -       | -       | -       | -       | -       | -       |
fval     |         | -       | -       | -       | -       | -       | -       |


         |**argB** | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
---------|---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |         |
areg     |         | 28      | -       | 32      | -       | 32      | -       | -       |
@areg    |         | 36      | -       | 40      | -       | 40      | -       | -       |
@areg+   |         | 36      | -       | 40      | -       | 40      | -       | -       |
@areg-   |         | 36      | -       | 40      | -       | 40      | -       | -       |
dreg     |         | 28      | -       | 32      | -       | 32      | -       | -       |
sp(ofs)  |         | 32      | -       | 36      | -       | 36      | -       | -       |
@sp(ofs) |         | 40      | -       | 44      | -       | 44      | -       | -       |
ival     |         | 32      | -       | 36      | -       | 36      | -       | -       |
@ival    |         | 44      | -       | 48      | -       | 48      | -       | -       |
label    |         | 32      | -       | 36      | -       | 36      | -       | -       |
@label   |         | 44      | -       | 48      | -       | 48      | -       | -       |
freg     |         | -       | -       | -       | -       | -       | -       | -       |
fval     |         | -       | -       | -       | -       | -       | -       | -       |


div
---

### Syntax
	div.f argA, argB

### Function
Divide the value at the location specified by argB by the value specified by argA, and store the result at the location specified by argB. Treat argA and argB as floating point numbers.

### Operand Types and Cycle Counts

         |**argB** | areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) |
---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |
areg     |         | -       | -       | -       | -       | -       | -       |
@areg    |         | -       | 36      | -       | -       | -       | 32      |
@areg+   |         | -       | -       | -       | -       | -       | -       |
@areg-   |         | -       | -       | -       | -       | -       | -       |
dreg     |         | -       | -       | -       | -       | -       | -       |
sp(ofs)  |         | -       | 32      | -       | -       | -       | 28      |
@sp(ofs) |         | -       | 40      | -       | -       | -       | 36      |
ival     |         | -       | -       | -       | -       | -       | -       |
@ival    |         | -       | 44      | -       | -       | -       | 40      |
label    |         | -       | -       | -       | -       | -       | -       |
@label   |         | -       | 44      | -       | -       | -       | 40      |
freg     |         | -       | 28      | -       | -       | -       | 24      |
fval     |         | -       | 32      | -       | -       | -       | 28      |


         |**argB** | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
---------|---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |         |
areg     |         | -       | -       | -       | -       | -       | -       | -       |
@areg    |         | 40      | -       | 44      | -       | 44      | 28      | -       |
@areg+   |         | -       | -       | -       | -       | -       | -       | -       |
@areg-   |         | -       | -       | -       | -       | -       | -       | -       |
dreg     |         | -       | -       | -       | -       | -       | -       | -       |
sp(ofs)  |         | 36      | -       | 40      | -       | 40      | 24      | -       |
@sp(ofs) |         | 44      | -       | 48      | -       | 48      | 32      | -       |
ival     |         | -       | -       | -       | -       | -       | -       | -       |
@ival    |         | 48      | -       | 52      | -       | 52      | 36      | -       |
label    |         | -       | -       | -       | -       | -       | -       | -       |
@label   |         | 48      | -       | 52      | -       | 52      | 36      | -       |
freg     |         | 32      | -       | 36      | -       | 36      | 20      | -       |
fval     |         | 36      | -       | 40      | -       | 40      | 24      | -       |


itof
----

### Syntax
	itof<.b/.w/.l> argA, argB

### Function
Convert the value specified by argA from an integer value into an floating point value, and store the result at the location specified by argB.

### Operand Types and Cycle Counts

         |**argB** | areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) |
---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |
areg     |         | -       | 12      | -       | -       | -       | 8       |
@areg    |         | -       | 20      | -       | -       | -       | 16      |
@areg+   |         | -       | 20      | -       | -       | -       | 16      |
@areg-   |         | -       | 20      | -       | -       | -       | 16      |
dreg     |         | -       | 12      | -       | -       | -       | 8       |
sp(ofs)  |         | -       | 16      | -       | -       | -       | 12      |
@sp(ofs) |         | -       | 24      | -       | -       | -       | 20      |
ival     |         | -       | 16      | -       | -       | -       | 12      |
@ival    |         | -       | 28      | -       | -       | -       | 24      |
label    |         | -       | 16      | -       | -       | -       | 12      |
@label   |         | -       | 28      | -       | -       | -       | 24      |
freg     |         | -       | -       | -       | -       | -       | -       |
fval     |         | -       | -       | -       | -       | -       | -       |


         |**argB** | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
---------|---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |         |
areg     |         | 16      | -       | 20      | -       | 20      | 4       | -       |
@areg    |         | 24      | -       | 28      | -       | 28      | 12      | -       |
@areg+   |         | 24      | -       | 28      | -       | 28      | 12      | -       |
@areg-   |         | 24      | -       | 28      | -       | 28      | 12      | -       |
dreg     |         | 16      | -       | 20      | -       | 20      | 4       | -       |
sp(ofs)  |         | 20      | -       | 24      | -       | 24      | 8       | -       |
@sp(ofs) |         | 28      | -       | 32      | -       | 32      | 16      | -       |
ival     |         | 20      | -       | 24      | -       | 24      | 8       | -       |
@ival    |         | 32      | -       | 36      | -       | 36      | 20      | -       |
label    |         | 20      | -       | 24      | -       | 24      | 8       | -       |
@label   |         | 32      | -       | 36      | -       | 36      | 20      | -       |
freg     |         | -       | -       | -       | -       | -       | -       | -       |
fval     |         | -       | -       | -       | -       | -       | -       | -       |


ftoi
----

### Syntax
	ftoi<.b/.w/.l> argA, argB

### Function
Convert the value specified by argA from a floating point value into an integer value, and store the result at the location specified by argB.

### Operand Types and Cycle Counts

         |**argB** | areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) |
---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |
areg     |         | -       | -       | -       | -       | -       | -       |
@areg    |         | 12      | 20      | 20      | 20      | 12      | 16      |
@areg+   |         | -       | -       | -       | -       | -       | -       |
@areg-   |         | -       | -       | -       | -       | -       | -       |
dreg     |         | -       | -       | -       | -       | -       | -       |
sp(ofs)  |         | 8       | 16      | 16      | 16      | 8       | 12      |
@sp(ofs) |         | 16      | 24      | 24      | 24      | 16      | 20      |
ival     |         | -       | -       | -       | -       | -       | -       |
@ival    |         | 20      | 28      | 28      | 28      | 20      | 24      |
label    |         | -       | -       | -       | -       | -       | -       |
@label   |         | 20      | 28      | 28      | 28      | 20      | 24      |
freg     |         | 4       | 12      | 12      | 12      | 4       | 8       |
fval     |         | 8       | 16      | 16      | 16      | 8       | 12      |


         |**argB** | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
---------|---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |         |
areg     |         | -       | -       | -       | -       | -       | -       | -       |
@areg    |         | 24      | -       | 28      | -       | 28      | -       | -       |
@areg+   |         | -       | -       | -       | -       | -       | -       | -       |
@areg-   |         | -       | -       | -       | -       | -       | -       | -       |
dreg     |         | -       | -       | -       | -       | -       | -       | -       |
sp(ofs)  |         | 20      | -       | 24      | -       | 24      | -       | -       |
@sp(ofs) |         | 28      | -       | 32      | -       | 32      | -       | -       |
ival     |         | -       | -       | -       | -       | -       | -       | -       |
@ival    |         | 32      | -       | 36      | -       | 36      | -       | -       |
label    |         | -       | -       | -       | -       | -       | -       | -       |
@label   |         | 32      | -       | 36      | -       | 36      | -       | -       |
freg     |         | 16      | -       | 20      | -       | 20      | -       | -       |
fval     |         | 20      | -       | 24      | -       | 24      | -       | -       |


cmpu
----

### Syntax
	cmpu<.b/.w/.l> argA, argB

### Function
Compare the value specified by argA with the value specified by argB. Treat argA and argB as unsigned integers. The result is used in the next call to je, jne, jge, jle, jg, or jl.

### Operand Types and Cycle Counts

         |**argB** | areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) |
---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |
areg     |         | 4       | 12      | 12      | 12      | 4       | 8       |
@areg    |         | 12      | 20      | 20      | 20      | 12      | 16      |
@areg+   |         | 12      | 20      | 20      | 20      | 12      | 16      |
@areg-   |         | 12      | 20      | 20      | 20      | 12      | 16      |
dreg     |         | 4       | 12      | 12      | 12      | 4       | 8       |
sp(ofs)  |         | 8       | 16      | 16      | 16      | 8       | 12      |
@sp(ofs) |         | 16      | 24      | 24      | 24      | 16      | 20      |
ival     |         | 8       | 16      | 16      | 16      | 8       | 12      |
@ival    |         | 20      | 28      | 28      | 28      | 20      | 24      |
label    |         | 8       | 16      | 16      | 16      | 8       | 12      |
@label   |         | 20      | 28      | 28      | 28      | 20      | 24      |
freg     |         | -       | -       | -       | -       | -       | -       |
fval     |         | -       | -       | -       | -       | -       | -       |


         |**argB** | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
---------|---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |         |
areg     |         | 16      | 8       | 20      | 8       | 20      | -       | -       |
@areg    |         | 24      | 16      | 28      | 16      | 28      | -       | -       |
@areg+   |         | 24      | 16      | 28      | 16      | 28      | -       | -       |
@areg-   |         | 24      | 16      | 28      | 16      | 28      | -       | -       |
dreg     |         | 16      | 8       | 20      | 8       | 20      | -       | -       |
sp(ofs)  |         | 20      | 12      | 24      | 12      | 24      | -       | -       |
@sp(ofs) |         | 28      | 20      | 32      | 20      | 32      | -       | -       |
ival     |         | 20      | 12      | 24      | 12      | 24      | -       | -       |
@ival    |         | 32      | 24      | 36      | 24      | 36      | -       | -       |
label    |         | 20      | 12      | 24      | 12      | 24      | -       | -       |
@label   |         | 32      | 24      | 36      | 24      | 36      | -       | -       |
freg     |         | -       | -       | -       | -       | -       | -       | -       |
fval     |         | -       | -       | -       | -       | -       | -       | -       |


cmps
----

### Syntax
	cmps<.b/.w/.l> argA, argB

### Function
Compare the value specified by argA with the value specified by argB. Treat argA and argB as signed integers. The result is used in the next call to je, jne, jge, jle, jg, or jl.

### Operand Types and Cycle Counts

         |**argB** | areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) |
---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |
areg     |         | 4       | 12      | 12      | 12      | 4       | 8       |
@areg    |         | 12      | 20      | 20      | 20      | 12      | 16      |
@areg+   |         | 12      | 20      | 20      | 20      | 12      | 16      |
@areg-   |         | 12      | 20      | 20      | 20      | 12      | 16      |
dreg     |         | 4       | 12      | 12      | 12      | 4       | 8       |
sp(ofs)  |         | 8       | 16      | 16      | 16      | 8       | 12      |
@sp(ofs) |         | 16      | 24      | 24      | 24      | 16      | 20      |
ival     |         | 8       | 16      | 16      | 16      | 8       | 12      |
@ival    |         | 20      | 28      | 28      | 28      | 20      | 24      |
label    |         | 8       | 16      | 16      | 16      | 8       | 12      |
@label   |         | 20      | 28      | 28      | 28      | 20      | 24      |
freg     |         | -       | -       | -       | -       | -       | -       |
fval     |         | -       | -       | -       | -       | -       | -       |


         |**argB** | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
---------|---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |         |
areg     |         | 16      | 8       | 20      | 8       | 20      | -       | -       |
@areg    |         | 24      | 16      | 28      | 16      | 28      | -       | -       |
@areg+   |         | 24      | 16      | 28      | 16      | 28      | -       | -       |
@areg-   |         | 24      | 16      | 28      | 16      | 28      | -       | -       |
dreg     |         | 16      | 8       | 20      | 8       | 20      | -       | -       |
sp(ofs)  |         | 20      | 12      | 24      | 12      | 24      | -       | -       |
@sp(ofs) |         | 28      | 20      | 32      | 20      | 32      | -       | -       |
ival     |         | 20      | 12      | 24      | 12      | 24      | -       | -       |
@ival    |         | 32      | 24      | 36      | 24      | 36      | -       | -       |
label    |         | 20      | 12      | 24      | 12      | 24      | -       | -       |
@label   |         | 32      | 24      | 36      | 24      | 36      | -       | -       |
freg     |         | -       | -       | -       | -       | -       | -       | -       |
fval     |         | -       | -       | -       | -       | -       | -       | -       |


cmp
---

### Syntax
	cmp.f argA, argB

### Function
Compare the value specified by argA with the value specified by argB. Treat argA and argB as floating point numbers. The result is used in the next call to je, jne, jge, jle, jg, or jl.

### Operand Types and Cycle Counts

         |**argB** | areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) |
---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |
areg     |         | -       | -       | -       | -       | -       | -       |
@areg    |         | -       | 20      | -       | -       | -       | 16      |
@areg+   |         | -       | -       | -       | -       | -       | -       |
@areg-   |         | -       | -       | -       | -       | -       | -       |
dreg     |         | -       | -       | -       | -       | -       | -       |
sp(ofs)  |         | -       | 16      | -       | -       | -       | 12      |
@sp(ofs) |         | -       | 24      | -       | -       | -       | 20      |
ival     |         | -       | -       | -       | -       | -       | -       |
@ival    |         | -       | 28      | -       | -       | -       | 24      |
label    |         | -       | -       | -       | -       | -       | -       |
@label   |         | -       | 28      | -       | -       | -       | 24      |
freg     |         | -       | 12      | -       | -       | -       | 8       |
fval     |         | -       | 16      | -       | -       | -       | 12      |


         |**argB** | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
---------|---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |         |
areg     |         | -       | -       | -       | -       | -       | -       | -       |
@areg    |         | 24      | -       | 28      | -       | 28      | 12      | 16      |
@areg+   |         | -       | -       | -       | -       | -       | -       | -       |
@areg-   |         | -       | -       | -       | -       | -       | -       | -       |
dreg     |         | -       | -       | -       | -       | -       | -       | -       |
sp(ofs)  |         | 20      | -       | 24      | -       | 24      | 8       | 12      |
@sp(ofs) |         | 28      | -       | 32      | -       | 32      | 16      | 20      |
ival     |         | -       | -       | -       | -       | -       | -       | -       |
@ival    |         | 32      | -       | 36      | -       | 36      | 20      | 24      |
label    |         | -       | -       | -       | -       | -       | -       | -       |
@label   |         | 32      | -       | 36      | -       | 36      | 20      | 24      |
freg     |         | 16      | -       | 20      | -       | 20      | 4       | 8       |
fval     |         | 20      | -       | 24      | -       | 24      | 8       | 12      |


tst
---

### Syntax
	tst<.b/.w/.l> argA

### Function
Test if value specified by argA is zero. The result is used in the next call to jz or jnz.

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
@areg    | 12
@areg+   | -
@areg-   | -
dreg     | -
sp(ofs)  | 8
@sp(ofs) | 16
ival     | -
@ival    | 20
label    | -
@label   | 20
freg     | 4
fval     | 8


je
--

### Syntax
	je argA

### Function
Jump to location specified by argA, if in the last compare (cmpu, cmps or cmp), argA was equal to argB.

### Operand Types and Cycle Counts

argA     | cycles
---------|-------
areg     | 8
@areg    | 16
@areg+   | 16
@areg-   | 16
dreg     | 8
sp(ofs)  | 12
@sp(ofs) | 20
ival     | 12
@ival    | 24
label    | 12
@label   | 24
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
areg     | 8
@areg    | 16
@areg+   | 16
@areg-   | 16
dreg     | 8
sp(ofs)  | 12
@sp(ofs) | 20
ival     | 12
@ival    | 24
label    | 12
@label   | 24
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
areg     | 8
@areg    | 16
@areg+   | 16
@areg-   | 16
dreg     | 8
sp(ofs)  | 12
@sp(ofs) | 20
ival     | 12
@ival    | 24
label    | 12
@label   | 24
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
areg     | 8
@areg    | 16
@areg+   | 16
@areg-   | 16
dreg     | 8
sp(ofs)  | 12
@sp(ofs) | 20
ival     | 12
@ival    | 24
label    | 12
@label   | 24
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
areg     | 8
@areg    | 16
@areg+   | 16
@areg-   | 16
dreg     | 8
sp(ofs)  | 12
@sp(ofs) | 20
ival     | 12
@ival    | 24
label    | 12
@label   | 24
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
areg     | 8
@areg    | 16
@areg+   | 16
@areg-   | 16
dreg     | 8
sp(ofs)  | 12
@sp(ofs) | 20
ival     | 12
@ival    | 24
label    | 12
@label   | 24
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
areg     | 8
@areg    | 16
@areg+   | 16
@areg-   | 16
dreg     | 8
sp(ofs)  | 12
@sp(ofs) | 20
ival     | 12
@ival    | 24
label    | 12
@label   | 24
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
areg     | 8
@areg    | 16
@areg+   | 16
@areg-   | 16
dreg     | 8
sp(ofs)  | 12
@sp(ofs) | 20
ival     | 12
@ival    | 24
label    | 12
@label   | 24
freg     | -
fval     | -


dje
---

### Syntax
	dje argA, argB

### Function
Decrement argB, compare it to zero, and jump to location specified by argA, if argB was equal to zero.

### Operand Types and Cycle Counts

         |**argB** | areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) |
---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |
areg     |         | 8       | 16      | 16      | 16      | 8       | 12      |
@areg    |         | 16      | 24      | 24      | 24      | 16      | 20      |
@areg+   |         | 16      | 24      | 24      | 24      | 16      | 20      |
@areg-   |         | 16      | 24      | 24      | 24      | 16      | 20      |
dreg     |         | 8       | 16      | 16      | 16      | 8       | 12      |
sp(ofs)  |         | 12      | 20      | 20      | 20      | 12      | 16      |
@sp(ofs) |         | 20      | 28      | 28      | 28      | 20      | 24      |
ival     |         | 12      | 20      | 20      | 20      | 12      | 16      |
@ival    |         | 24      | 32      | 32      | 32      | 24      | 28      |
label    |         | 12      | 20      | 20      | 20      | 12      | 16      |
@label   |         | 24      | 32      | 32      | 32      | 24      | 28      |
freg     |         | -       | -       | -       | -       | -       | -       |
fval     |         | -       | -       | -       | -       | -       | -       |


         |**argB** | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
---------|---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |         |
areg     |         | 20      | -       | 24      | -       | 24      | -       | -       |
@areg    |         | 28      | -       | 32      | -       | 32      | -       | -       |
@areg+   |         | 28      | -       | 32      | -       | 32      | -       | -       |
@areg-   |         | 28      | -       | 32      | -       | 32      | -       | -       |
dreg     |         | 20      | -       | 24      | -       | 24      | -       | -       |
sp(ofs)  |         | 24      | -       | 28      | -       | 28      | -       | -       |
@sp(ofs) |         | 32      | -       | 36      | -       | 36      | -       | -       |
ival     |         | 24      | -       | 28      | -       | 28      | -       | -       |
@ival    |         | 36      | -       | 40      | -       | 40      | -       | -       |
label    |         | 24      | -       | 28      | -       | 28      | -       | -       |
@label   |         | 36      | -       | 40      | -       | 40      | -       | -       |
freg     |         | -       | -       | -       | -       | -       | -       | -       |
fval     |         | -       | -       | -       | -       | -       | -       | -       |


djne
----

### Syntax
	djne argA, argB

### Function
Decrement argB, compare it to zero, and jump to location specified by argA, if argB was not equal to zero.

### Operand Types and Cycle Counts

         |**argB** | areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) |
---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |
areg     |         | 8       | 16      | 16      | 16      | 8       | 12      |
@areg    |         | 16      | 24      | 24      | 24      | 16      | 20      |
@areg+   |         | 16      | 24      | 24      | 24      | 16      | 20      |
@areg-   |         | 16      | 24      | 24      | 24      | 16      | 20      |
dreg     |         | 8       | 16      | 16      | 16      | 8       | 12      |
sp(ofs)  |         | 12      | 20      | 20      | 20      | 12      | 16      |
@sp(ofs) |         | 20      | 28      | 28      | 28      | 20      | 24      |
ival     |         | 12      | 20      | 20      | 20      | 12      | 16      |
@ival    |         | 24      | 32      | 32      | 32      | 24      | 28      |
label    |         | 12      | 20      | 20      | 20      | 12      | 16      |
@label   |         | 24      | 32      | 32      | 32      | 24      | 28      |
freg     |         | -       | -       | -       | -       | -       | -       |
fval     |         | -       | -       | -       | -       | -       | -       |


         |**argB** | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
---------|---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |         |
areg     |         | 20      | -       | 24      | -       | 24      | -       | -       |
@areg    |         | 28      | -       | 32      | -       | 32      | -       | -       |
@areg+   |         | 28      | -       | 32      | -       | 32      | -       | -       |
@areg-   |         | 28      | -       | 32      | -       | 32      | -       | -       |
dreg     |         | 20      | -       | 24      | -       | 24      | -       | -       |
sp(ofs)  |         | 24      | -       | 28      | -       | 28      | -       | -       |
@sp(ofs) |         | 32      | -       | 36      | -       | 36      | -       | -       |
ival     |         | 24      | -       | 28      | -       | 28      | -       | -       |
@ival    |         | 36      | -       | 40      | -       | 40      | -       | -       |
label    |         | 24      | -       | 28      | -       | 28      | -       | -       |
@label   |         | 36      | -       | 40      | -       | 40      | -       | -       |
freg     |         | -       | -       | -       | -       | -       | -       | -       |
fval     |         | -       | -       | -       | -       | -       | -       | -       |


djge
----

### Syntax
	djge argA, argB

### Function
Decrement argB, compare it to zero, and jump to location specified by argA, if argB was greater than or equal to zero.

### Operand Types and Cycle Counts

         |**argB** | areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) |
---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |
areg     |         | 8       | 16      | 16      | 16      | 8       | 12      |
@areg    |         | 16      | 24      | 24      | 24      | 16      | 20      |
@areg+   |         | 16      | 24      | 24      | 24      | 16      | 20      |
@areg-   |         | 16      | 24      | 24      | 24      | 16      | 20      |
dreg     |         | 8       | 16      | 16      | 16      | 8       | 12      |
sp(ofs)  |         | 12      | 20      | 20      | 20      | 12      | 16      |
@sp(ofs) |         | 20      | 28      | 28      | 28      | 20      | 24      |
ival     |         | 12      | 20      | 20      | 20      | 12      | 16      |
@ival    |         | 24      | 32      | 32      | 32      | 24      | 28      |
label    |         | 12      | 20      | 20      | 20      | 12      | 16      |
@label   |         | 24      | 32      | 32      | 32      | 24      | 28      |
freg     |         | -       | -       | -       | -       | -       | -       |
fval     |         | -       | -       | -       | -       | -       | -       |


         |**argB** | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
---------|---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |         |
areg     |         | 20      | -       | 24      | -       | 24      | -       | -       |
@areg    |         | 28      | -       | 32      | -       | 32      | -       | -       |
@areg+   |         | 28      | -       | 32      | -       | 32      | -       | -       |
@areg-   |         | 28      | -       | 32      | -       | 32      | -       | -       |
dreg     |         | 20      | -       | 24      | -       | 24      | -       | -       |
sp(ofs)  |         | 24      | -       | 28      | -       | 28      | -       | -       |
@sp(ofs) |         | 32      | -       | 36      | -       | 36      | -       | -       |
ival     |         | 24      | -       | 28      | -       | 28      | -       | -       |
@ival    |         | 36      | -       | 40      | -       | 40      | -       | -       |
label    |         | 24      | -       | 28      | -       | 28      | -       | -       |
@label   |         | 36      | -       | 40      | -       | 40      | -       | -       |
freg     |         | -       | -       | -       | -       | -       | -       | -       |
fval     |         | -       | -       | -       | -       | -       | -       | -       |


djle
----

### Syntax
	djle argA, argB

### Function
Decrement argB, compare it to zero, and jump to location specified by argA, if argB was less than or equal to zero.

### Operand Types and Cycle Counts

         |**argB** | areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) |
---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |
areg     |         | 8       | 16      | 16      | 16      | 8       | 12      |
@areg    |         | 16      | 24      | 24      | 24      | 16      | 20      |
@areg+   |         | 16      | 24      | 24      | 24      | 16      | 20      |
@areg-   |         | 16      | 24      | 24      | 24      | 16      | 20      |
dreg     |         | 8       | 16      | 16      | 16      | 8       | 12      |
sp(ofs)  |         | 12      | 20      | 20      | 20      | 12      | 16      |
@sp(ofs) |         | 20      | 28      | 28      | 28      | 20      | 24      |
ival     |         | 12      | 20      | 20      | 20      | 12      | 16      |
@ival    |         | 24      | 32      | 32      | 32      | 24      | 28      |
label    |         | 12      | 20      | 20      | 20      | 12      | 16      |
@label   |         | 24      | 32      | 32      | 32      | 24      | 28      |
freg     |         | -       | -       | -       | -       | -       | -       |
fval     |         | -       | -       | -       | -       | -       | -       |


         |**argB** | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
---------|---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |         |
areg     |         | 20      | -       | 24      | -       | 24      | -       | -       |
@areg    |         | 28      | -       | 32      | -       | 32      | -       | -       |
@areg+   |         | 28      | -       | 32      | -       | 32      | -       | -       |
@areg-   |         | 28      | -       | 32      | -       | 32      | -       | -       |
dreg     |         | 20      | -       | 24      | -       | 24      | -       | -       |
sp(ofs)  |         | 24      | -       | 28      | -       | 28      | -       | -       |
@sp(ofs) |         | 32      | -       | 36      | -       | 36      | -       | -       |
ival     |         | 24      | -       | 28      | -       | 28      | -       | -       |
@ival    |         | 36      | -       | 40      | -       | 40      | -       | -       |
label    |         | 24      | -       | 28      | -       | 28      | -       | -       |
@label   |         | 36      | -       | 40      | -       | 40      | -       | -       |
freg     |         | -       | -       | -       | -       | -       | -       | -       |
fval     |         | -       | -       | -       | -       | -       | -       | -       |


djg
---

### Syntax
	djg argA, argB

### Function
Decrement argB, compare it to zero, and jump to location specified by argA, if argB was greater than zero.

### Operand Types and Cycle Counts

         |**argB** | areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) |
---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |
areg     |         | 8       | 16      | 16      | 16      | 8       | 12      |
@areg    |         | 16      | 24      | 24      | 24      | 16      | 20      |
@areg+   |         | 16      | 24      | 24      | 24      | 16      | 20      |
@areg-   |         | 16      | 24      | 24      | 24      | 16      | 20      |
dreg     |         | 8       | 16      | 16      | 16      | 8       | 12      |
sp(ofs)  |         | 12      | 20      | 20      | 20      | 12      | 16      |
@sp(ofs) |         | 20      | 28      | 28      | 28      | 20      | 24      |
ival     |         | 12      | 20      | 20      | 20      | 12      | 16      |
@ival    |         | 24      | 32      | 32      | 32      | 24      | 28      |
label    |         | 12      | 20      | 20      | 20      | 12      | 16      |
@label   |         | 24      | 32      | 32      | 32      | 24      | 28      |
freg     |         | -       | -       | -       | -       | -       | -       |
fval     |         | -       | -       | -       | -       | -       | -       |


         |**argB** | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
---------|---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |         |
areg     |         | 20      | -       | 24      | -       | 24      | -       | -       |
@areg    |         | 28      | -       | 32      | -       | 32      | -       | -       |
@areg+   |         | 28      | -       | 32      | -       | 32      | -       | -       |
@areg-   |         | 28      | -       | 32      | -       | 32      | -       | -       |
dreg     |         | 20      | -       | 24      | -       | 24      | -       | -       |
sp(ofs)  |         | 24      | -       | 28      | -       | 28      | -       | -       |
@sp(ofs) |         | 32      | -       | 36      | -       | 36      | -       | -       |
ival     |         | 24      | -       | 28      | -       | 28      | -       | -       |
@ival    |         | 36      | -       | 40      | -       | 40      | -       | -       |
label    |         | 24      | -       | 28      | -       | 28      | -       | -       |
@label   |         | 36      | -       | 40      | -       | 40      | -       | -       |
freg     |         | -       | -       | -       | -       | -       | -       | -       |
fval     |         | -       | -       | -       | -       | -       | -       | -       |


djl
---

### Syntax
	djl argA, argB

### Function
Decrement argB, compare it to zero, and jump to location specified by argA, if argB was less than zero.

### Operand Types and Cycle Counts

         |**argB** | areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) |
---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |
areg     |         | 8       | 16      | 16      | 16      | 8       | 12      |
@areg    |         | 16      | 24      | 24      | 24      | 16      | 20      |
@areg+   |         | 16      | 24      | 24      | 24      | 16      | 20      |
@areg-   |         | 16      | 24      | 24      | 24      | 16      | 20      |
dreg     |         | 8       | 16      | 16      | 16      | 8       | 12      |
sp(ofs)  |         | 12      | 20      | 20      | 20      | 12      | 16      |
@sp(ofs) |         | 20      | 28      | 28      | 28      | 20      | 24      |
ival     |         | 12      | 20      | 20      | 20      | 12      | 16      |
@ival    |         | 24      | 32      | 32      | 32      | 24      | 28      |
label    |         | 12      | 20      | 20      | 20      | 12      | 16      |
@label   |         | 24      | 32      | 32      | 32      | 24      | 28      |
freg     |         | -       | -       | -       | -       | -       | -       |
fval     |         | -       | -       | -       | -       | -       | -       |


         |**argB** | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
---------|---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |         |
areg     |         | 20      | -       | 24      | -       | 24      | -       | -       |
@areg    |         | 28      | -       | 32      | -       | 32      | -       | -       |
@areg+   |         | 28      | -       | 32      | -       | 32      | -       | -       |
@areg-   |         | 28      | -       | 32      | -       | 32      | -       | -       |
dreg     |         | 20      | -       | 24      | -       | 24      | -       | -       |
sp(ofs)  |         | 24      | -       | 28      | -       | 28      | -       | -       |
@sp(ofs) |         | 32      | -       | 36      | -       | 36      | -       | -       |
ival     |         | 24      | -       | 28      | -       | 28      | -       | -       |
@ival    |         | 36      | -       | 40      | -       | 40      | -       | -       |
label    |         | 24      | -       | 28      | -       | 28      | -       | -       |
@label   |         | 36      | -       | 40      | -       | 40      | -       | -       |
freg     |         | -       | -       | -       | -       | -       | -       | -       |
fval     |         | -       | -       | -       | -       | -       | -       | -       |


djz
---

### Syntax
	djz argA, argB

### Function
Decrement argB, compare it to zero, and jump to location specified by argA, if argB was zero.

### Operand Types and Cycle Counts

         |**argB** | areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) |
---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |
areg     |         | 8       | 16      | 16      | 16      | 8       | 12      |
@areg    |         | 16      | 24      | 24      | 24      | 16      | 20      |
@areg+   |         | 16      | 24      | 24      | 24      | 16      | 20      |
@areg-   |         | 16      | 24      | 24      | 24      | 16      | 20      |
dreg     |         | 8       | 16      | 16      | 16      | 8       | 12      |
sp(ofs)  |         | 12      | 20      | 20      | 20      | 12      | 16      |
@sp(ofs) |         | 20      | 28      | 28      | 28      | 20      | 24      |
ival     |         | 12      | 20      | 20      | 20      | 12      | 16      |
@ival    |         | 24      | 32      | 32      | 32      | 24      | 28      |
label    |         | 12      | 20      | 20      | 20      | 12      | 16      |
@label   |         | 24      | 32      | 32      | 32      | 24      | 28      |
freg     |         | -       | -       | -       | -       | -       | -       |
fval     |         | -       | -       | -       | -       | -       | -       |


         |**argB** | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
---------|---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |         |
areg     |         | 20      | -       | 24      | -       | 24      | -       | -       |
@areg    |         | 28      | -       | 32      | -       | 32      | -       | -       |
@areg+   |         | 28      | -       | 32      | -       | 32      | -       | -       |
@areg-   |         | 28      | -       | 32      | -       | 32      | -       | -       |
dreg     |         | 20      | -       | 24      | -       | 24      | -       | -       |
sp(ofs)  |         | 24      | -       | 28      | -       | 28      | -       | -       |
@sp(ofs) |         | 32      | -       | 36      | -       | 36      | -       | -       |
ival     |         | 24      | -       | 28      | -       | 28      | -       | -       |
@ival    |         | 36      | -       | 40      | -       | 40      | -       | -       |
label    |         | 24      | -       | 28      | -       | 28      | -       | -       |
@label   |         | 36      | -       | 40      | -       | 40      | -       | -       |
freg     |         | -       | -       | -       | -       | -       | -       | -       |
fval     |         | -       | -       | -       | -       | -       | -       | -       |


djnz
----

### Syntax
	djnz argA, argB

### Function
Decrement argB, compare it to zero, and jump to location specified by argA, if argB was non-zero

### Operand Types and Cycle Counts

         |**argB** | areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) |
---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |
areg     |         | 8       | 16      | 16      | 16      | 8       | 12      |
@areg    |         | 16      | 24      | 24      | 24      | 16      | 20      |
@areg+   |         | 16      | 24      | 24      | 24      | 16      | 20      |
@areg-   |         | 16      | 24      | 24      | 24      | 16      | 20      |
dreg     |         | 8       | 16      | 16      | 16      | 8       | 12      |
sp(ofs)  |         | 12      | 20      | 20      | 20      | 12      | 16      |
@sp(ofs) |         | 20      | 28      | 28      | 28      | 20      | 24      |
ival     |         | 12      | 20      | 20      | 20      | 12      | 16      |
@ival    |         | 24      | 32      | 32      | 32      | 24      | 28      |
label    |         | 12      | 20      | 20      | 20      | 12      | 16      |
@label   |         | 24      | 32      | 32      | 32      | 24      | 28      |
freg     |         | -       | -       | -       | -       | -       | -       |
fval     |         | -       | -       | -       | -       | -       | -       |


         |**argB** | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
---------|---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |         |
areg     |         | 20      | -       | 24      | -       | 24      | -       | -       |
@areg    |         | 28      | -       | 32      | -       | 32      | -       | -       |
@areg+   |         | 28      | -       | 32      | -       | 32      | -       | -       |
@areg-   |         | 28      | -       | 32      | -       | 32      | -       | -       |
dreg     |         | 20      | -       | 24      | -       | 24      | -       | -       |
sp(ofs)  |         | 24      | -       | 28      | -       | 28      | -       | -       |
@sp(ofs) |         | 32      | -       | 36      | -       | 36      | -       | -       |
ival     |         | 24      | -       | 28      | -       | 28      | -       | -       |
@ival    |         | 36      | -       | 40      | -       | 40      | -       | -       |
label    |         | 24      | -       | 28      | -       | 28      | -       | -       |
@label   |         | 36      | -       | 40      | -       | 40      | -       | -       |
freg     |         | -       | -       | -       | -       | -       | -       | -       |
fval     |         | -       | -       | -       | -       | -       | -       | -       |


push
----

### Syntax
	push<.b/.w/.l/.f> argA

### Function
Push the value specified by argA onto the stack.

### Operand Types and Cycle Counts

argA     | cycles
---------|-------
areg     | 8
@areg    | 16
@areg+   | 16
@areg-   | 16
dreg     | 8
sp(ofs)  | 12
@sp(ofs) | 20
ival     | 12
@ival    | 24
label    | 12
@label   | 24
freg     | 8
fval     | 12


pop
---

### Syntax
	pop<.b/.w/.l/.f> argA

### Function
Pop a value off the stack, and store it at the location specified by argA.

### Operand Types and Cycle Counts

argA     | cycles
---------|-------
areg     | 8
@areg    | 16
@areg+   | 16
@areg-   | 16
dreg     | 8
sp(ofs)  | 12
@sp(ofs) | 20
ival     | -
@ival    | 24
label    | -
@label   | 24
freg     | 8
fval     | -


and
---

### Syntax
	and<.b/.w/.l> argA, argB

### Function
Perform a bitwise AND on the value specified by argA and the value at the location specified by argB, and store the result at the location specified by argB.

### Operand Types and Cycle Counts

         |**argB** | areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) |
---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |
areg     |         | 4       | 12      | 12      | 12      | 4       | 8       |
@areg    |         | 12      | 20      | 20      | 20      | 12      | 16      |
@areg+   |         | 12      | 20      | 20      | 20      | 12      | 16      |
@areg-   |         | 12      | 20      | 20      | 20      | 12      | 16      |
dreg     |         | 4       | 12      | 12      | 12      | 4       | 8       |
sp(ofs)  |         | 8       | 16      | 16      | 16      | 8       | 12      |
@sp(ofs) |         | 16      | 24      | 24      | 24      | 16      | 20      |
ival     |         | 8       | 16      | 16      | 16      | 8       | 12      |
@ival    |         | 20      | 28      | 28      | 28      | 20      | 24      |
label    |         | 8       | 16      | 16      | 16      | 8       | 12      |
@label   |         | 20      | 28      | 28      | 28      | 20      | 24      |
freg     |         | -       | -       | -       | -       | -       | -       |
fval     |         | -       | -       | -       | -       | -       | -       |


         |**argB** | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
---------|---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |         |
areg     |         | 16      | -       | 20      | -       | 20      | -       | -       |
@areg    |         | 24      | -       | 28      | -       | 28      | -       | -       |
@areg+   |         | 24      | -       | 28      | -       | 28      | -       | -       |
@areg-   |         | 24      | -       | 28      | -       | 28      | -       | -       |
dreg     |         | 16      | -       | 20      | -       | 20      | -       | -       |
sp(ofs)  |         | 20      | -       | 24      | -       | 24      | -       | -       |
@sp(ofs) |         | 28      | -       | 32      | -       | 32      | -       | -       |
ival     |         | 20      | -       | 24      | -       | 24      | -       | -       |
@ival    |         | 32      | -       | 36      | -       | 36      | -       | -       |
label    |         | 20      | -       | 24      | -       | 24      | -       | -       |
@label   |         | 32      | -       | 36      | -       | 36      | -       | -       |
freg     |         | -       | -       | -       | -       | -       | -       | -       |
fval     |         | -       | -       | -       | -       | -       | -       | -       |


or
--

### Syntax
	or<.b/.w/.l> argA, argB

### Function
Perform a bitwise OR on the value specified by argA and the value at the location specified by argB, and store the result at the location specified by argB.

### Operand Types and Cycle Counts

         |**argB** | areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) |
---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |
areg     |         | 4       | 12      | 12      | 12      | 4       | 8       |
@areg    |         | 12      | 20      | 20      | 20      | 12      | 16      |
@areg+   |         | 12      | 20      | 20      | 20      | 12      | 16      |
@areg-   |         | 12      | 20      | 20      | 20      | 12      | 16      |
dreg     |         | 4       | 12      | 12      | 12      | 4       | 8       |
sp(ofs)  |         | 8       | 16      | 16      | 16      | 8       | 12      |
@sp(ofs) |         | 16      | 24      | 24      | 24      | 16      | 20      |
ival     |         | 8       | 16      | 16      | 16      | 8       | 12      |
@ival    |         | 20      | 28      | 28      | 28      | 20      | 24      |
label    |         | 8       | 16      | 16      | 16      | 8       | 12      |
@label   |         | 20      | 28      | 28      | 28      | 20      | 24      |
freg     |         | -       | -       | -       | -       | -       | -       |
fval     |         | -       | -       | -       | -       | -       | -       |


         |**argB** | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
---------|---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |         |
areg     |         | 16      | -       | 20      | -       | 20      | -       | -       |
@areg    |         | 24      | -       | 28      | -       | 28      | -       | -       |
@areg+   |         | 24      | -       | 28      | -       | 28      | -       | -       |
@areg-   |         | 24      | -       | 28      | -       | 28      | -       | -       |
dreg     |         | 16      | -       | 20      | -       | 20      | -       | -       |
sp(ofs)  |         | 20      | -       | 24      | -       | 24      | -       | -       |
@sp(ofs) |         | 28      | -       | 32      | -       | 32      | -       | -       |
ival     |         | 20      | -       | 24      | -       | 24      | -       | -       |
@ival    |         | 32      | -       | 36      | -       | 36      | -       | -       |
label    |         | 20      | -       | 24      | -       | 24      | -       | -       |
@label   |         | 32      | -       | 36      | -       | 36      | -       | -       |
freg     |         | -       | -       | -       | -       | -       | -       | -       |
fval     |         | -       | -       | -       | -       | -       | -       | -       |


xor
---

### Syntax
	xor<.b/.w/.l> argA, argB

### Function
Perform a bitwise XOR on the value specified by argA and the value at the location specified by argB, and store the result at the location specified by argB.

### Operand Types and Cycle Counts

         |**argB** | areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) |
---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |
areg     |         | 4       | 12      | 12      | 12      | 4       | 8       |
@areg    |         | 12      | 20      | 20      | 20      | 12      | 16      |
@areg+   |         | 12      | 20      | 20      | 20      | 12      | 16      |
@areg-   |         | 12      | 20      | 20      | 20      | 12      | 16      |
dreg     |         | 4       | 12      | 12      | 12      | 4       | 8       |
sp(ofs)  |         | 8       | 16      | 16      | 16      | 8       | 12      |
@sp(ofs) |         | 16      | 24      | 24      | 24      | 16      | 20      |
ival     |         | 8       | 16      | 16      | 16      | 8       | 12      |
@ival    |         | 20      | 28      | 28      | 28      | 20      | 24      |
label    |         | 8       | 16      | 16      | 16      | 8       | 12      |
@label   |         | 20      | 28      | 28      | 28      | 20      | 24      |
freg     |         | -       | -       | -       | -       | -       | -       |
fval     |         | -       | -       | -       | -       | -       | -       |


         |**argB** | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
---------|---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |         |
areg     |         | 16      | -       | 20      | -       | 20      | -       | -       |
@areg    |         | 24      | -       | 28      | -       | 28      | -       | -       |
@areg+   |         | 24      | -       | 28      | -       | 28      | -       | -       |
@areg-   |         | 24      | -       | 28      | -       | 28      | -       | -       |
dreg     |         | 16      | -       | 20      | -       | 20      | -       | -       |
sp(ofs)  |         | 20      | -       | 24      | -       | 24      | -       | -       |
@sp(ofs) |         | 28      | -       | 32      | -       | 32      | -       | -       |
ival     |         | 20      | -       | 24      | -       | 24      | -       | -       |
@ival    |         | 32      | -       | 36      | -       | 36      | -       | -       |
label    |         | 20      | -       | 24      | -       | 24      | -       | -       |
@label   |         | 32      | -       | 36      | -       | 36      | -       | -       |
freg     |         | -       | -       | -       | -       | -       | -       | -       |
fval     |         | -       | -       | -       | -       | -       | -       | -       |


not
---

### Syntax
	not<.b/.w/.l> argA

### Function
Perform a bitwise NOT on the value at the location specified by argA.

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
freg     | -
fval     | -


bset
----

### Syntax
	bset<.b/.w/.l> argA, argB

### Function
Set the bit at bit index specified by argA, in the value at the location specified by argB. Bit indices start at 0 and increase with significance.

### Operand Types and Cycle Counts

         |**argB** | areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) |
---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |
areg     |         | 4       | 12      | 12      | 12      | 4       | 8       |
@areg    |         | 12      | 20      | 20      | 20      | 12      | 16      |
@areg+   |         | 12      | 20      | 20      | 20      | 12      | 16      |
@areg-   |         | 12      | 20      | 20      | 20      | 12      | 16      |
dreg     |         | 4       | 12      | 12      | 12      | 4       | 8       |
sp(ofs)  |         | 8       | 16      | 16      | 16      | 8       | 12      |
@sp(ofs) |         | 16      | 24      | 24      | 24      | 16      | 20      |
ival     |         | 8       | 16      | 16      | 16      | 8       | 12      |
@ival    |         | 20      | 28      | 28      | 28      | 20      | 24      |
label    |         | 8       | 16      | 16      | 16      | 8       | 12      |
@label   |         | 20      | 28      | 28      | 28      | 20      | 24      |
freg     |         | -       | -       | -       | -       | -       | -       |
fval     |         | -       | -       | -       | -       | -       | -       |


         |**argB** | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
---------|---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |         |
areg     |         | 16      | -       | 20      | -       | 20      | -       | -       |
@areg    |         | 24      | -       | 28      | -       | 28      | -       | -       |
@areg+   |         | 24      | -       | 28      | -       | 28      | -       | -       |
@areg-   |         | 24      | -       | 28      | -       | 28      | -       | -       |
dreg     |         | 16      | -       | 20      | -       | 20      | -       | -       |
sp(ofs)  |         | 20      | -       | 24      | -       | 24      | -       | -       |
@sp(ofs) |         | 28      | -       | 32      | -       | 32      | -       | -       |
ival     |         | 20      | -       | 24      | -       | 24      | -       | -       |
@ival    |         | 32      | -       | 36      | -       | 36      | -       | -       |
label    |         | 20      | -       | 24      | -       | 24      | -       | -       |
@label   |         | 32      | -       | 36      | -       | 36      | -       | -       |
freg     |         | -       | -       | -       | -       | -       | -       | -       |
fval     |         | -       | -       | -       | -       | -       | -       | -       |


bclr
----

### Syntax
	bclr<.b/.w/.l> argA, argB

### Function
Clear the bit at bit index specified by argA, in the value at the location specified by argB. Bit indices start at 0 and increase with significance.

### Operand Types and Cycle Counts

         |**argB** | areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) |
---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |
areg     |         | 4       | 12      | 12      | 12      | 4       | 8       |
@areg    |         | 12      | 20      | 20      | 20      | 12      | 16      |
@areg+   |         | 12      | 20      | 20      | 20      | 12      | 16      |
@areg-   |         | 12      | 20      | 20      | 20      | 12      | 16      |
dreg     |         | 4       | 12      | 12      | 12      | 4       | 8       |
sp(ofs)  |         | 8       | 16      | 16      | 16      | 8       | 12      |
@sp(ofs) |         | 16      | 24      | 24      | 24      | 16      | 20      |
ival     |         | 8       | 16      | 16      | 16      | 8       | 12      |
@ival    |         | 20      | 28      | 28      | 28      | 20      | 24      |
label    |         | 8       | 16      | 16      | 16      | 8       | 12      |
@label   |         | 20      | 28      | 28      | 28      | 20      | 24      |
freg     |         | -       | -       | -       | -       | -       | -       |
fval     |         | -       | -       | -       | -       | -       | -       |


         |**argB** | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
---------|---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |         |
areg     |         | 16      | -       | 20      | -       | 20      | -       | -       |
@areg    |         | 24      | -       | 28      | -       | 28      | -       | -       |
@areg+   |         | 24      | -       | 28      | -       | 28      | -       | -       |
@areg-   |         | 24      | -       | 28      | -       | 28      | -       | -       |
dreg     |         | 16      | -       | 20      | -       | 20      | -       | -       |
sp(ofs)  |         | 20      | -       | 24      | -       | 24      | -       | -       |
@sp(ofs) |         | 28      | -       | 32      | -       | 32      | -       | -       |
ival     |         | 20      | -       | 24      | -       | 24      | -       | -       |
@ival    |         | 32      | -       | 36      | -       | 36      | -       | -       |
label    |         | 20      | -       | 24      | -       | 24      | -       | -       |
@label   |         | 32      | -       | 36      | -       | 36      | -       | -       |
freg     |         | -       | -       | -       | -       | -       | -       | -       |
fval     |         | -       | -       | -       | -       | -       | -       | -       |


btst
----

### Syntax
	btst<.b/.w/.l> argA, argB

### Function
Test the bit at bit index specified by argA, in the value at the location specified by argB. Bit indices start at 0 and increase with significance. The result is used in the next call to jz or jnz.

### Operand Types and Cycle Counts

         |**argB** | areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) |
---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |
areg     |         | 4       | 12      | 12      | 12      | 4       | 8       |
@areg    |         | 12      | 20      | 20      | 20      | 12      | 16      |
@areg+   |         | 12      | 20      | 20      | 20      | 12      | 16      |
@areg-   |         | 12      | 20      | 20      | 20      | 12      | 16      |
dreg     |         | 4       | 12      | 12      | 12      | 4       | 8       |
sp(ofs)  |         | 8       | 16      | 16      | 16      | 8       | 12      |
@sp(ofs) |         | 16      | 24      | 24      | 24      | 16      | 20      |
ival     |         | 8       | 16      | 16      | 16      | 8       | 12      |
@ival    |         | 20      | 28      | 28      | 28      | 20      | 24      |
label    |         | 8       | 16      | 16      | 16      | 8       | 12      |
@label   |         | 20      | 28      | 28      | 28      | 20      | 24      |
freg     |         | -       | -       | -       | -       | -       | -       |
fval     |         | -       | -       | -       | -       | -       | -       |


         |**argB** | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
---------|---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |         |
areg     |         | 16      | -       | 20      | -       | 20      | -       | -       |
@areg    |         | 24      | -       | 28      | -       | 28      | -       | -       |
@areg+   |         | 24      | -       | 28      | -       | 28      | -       | -       |
@areg-   |         | 24      | -       | 28      | -       | 28      | -       | -       |
dreg     |         | 16      | -       | 20      | -       | 20      | -       | -       |
sp(ofs)  |         | 20      | -       | 24      | -       | 24      | -       | -       |
@sp(ofs) |         | 28      | -       | 32      | -       | 32      | -       | -       |
ival     |         | 20      | -       | 24      | -       | 24      | -       | -       |
@ival    |         | 32      | -       | 36      | -       | 36      | -       | -       |
label    |         | 20      | -       | 24      | -       | 24      | -       | -       |
@label   |         | 32      | -       | 36      | -       | 36      | -       | -       |
freg     |         | -       | -       | -       | -       | -       | -       | -       |
fval     |         | -       | -       | -       | -       | -       | -       | -       |


bchg
----

### Syntax
	bchg<.b/.w/.l> argA, argB

### Function
Flip the bit at bit index specified by argA, in the value at the location specified by argB. Bit indices start at 0 and increase with significance.

### Operand Types and Cycle Counts

         |**argB** | areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) |
---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |
areg     |         | 4       | 12      | 12      | 12      | 4       | 8       |
@areg    |         | 12      | 20      | 20      | 20      | 12      | 16      |
@areg+   |         | 12      | 20      | 20      | 20      | 12      | 16      |
@areg-   |         | 12      | 20      | 20      | 20      | 12      | 16      |
dreg     |         | 4       | 12      | 12      | 12      | 4       | 8       |
sp(ofs)  |         | 8       | 16      | 16      | 16      | 8       | 12      |
@sp(ofs) |         | 16      | 24      | 24      | 24      | 16      | 20      |
ival     |         | 8       | 16      | 16      | 16      | 8       | 12      |
@ival    |         | 20      | 28      | 28      | 28      | 20      | 24      |
label    |         | 8       | 16      | 16      | 16      | 8       | 12      |
@label   |         | 20      | 28      | 28      | 28      | 20      | 24      |
freg     |         | -       | -       | -       | -       | -       | -       |
fval     |         | -       | -       | -       | -       | -       | -       |


         |**argB** | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
---------|---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |         |
areg     |         | 16      | -       | 20      | -       | 20      | -       | -       |
@areg    |         | 24      | -       | 28      | -       | 28      | -       | -       |
@areg+   |         | 24      | -       | 28      | -       | 28      | -       | -       |
@areg-   |         | 24      | -       | 28      | -       | 28      | -       | -       |
dreg     |         | 16      | -       | 20      | -       | 20      | -       | -       |
sp(ofs)  |         | 20      | -       | 24      | -       | 24      | -       | -       |
@sp(ofs) |         | 28      | -       | 32      | -       | 32      | -       | -       |
ival     |         | 20      | -       | 24      | -       | 24      | -       | -       |
@ival    |         | 32      | -       | 36      | -       | 36      | -       | -       |
label    |         | 20      | -       | 24      | -       | 24      | -       | -       |
@label   |         | 32      | -       | 36      | -       | 36      | -       | -       |
freg     |         | -       | -       | -       | -       | -       | -       | -       |
fval     |         | -       | -       | -       | -       | -       | -       | -       |


rol
---

### Syntax
	rol<.b/.w/.l> argA, argB

### Function
Perform a bitwise left shift of the value at the location specified by argB. Number of bits to shift is the value specified by argA.

### Operand Types and Cycle Counts

         |**argB** | areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) |
---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |
areg     |         | 4       | 12      | 12      | 12      | 4       | 8       |
@areg    |         | 12      | 20      | 20      | 20      | 12      | 16      |
@areg+   |         | 12      | 20      | 20      | 20      | 12      | 16      |
@areg-   |         | 12      | 20      | 20      | 20      | 12      | 16      |
dreg     |         | 4       | 12      | 12      | 12      | 4       | 8       |
sp(ofs)  |         | 8       | 16      | 16      | 16      | 8       | 12      |
@sp(ofs) |         | 16      | 24      | 24      | 24      | 16      | 20      |
ival     |         | 8       | 16      | 16      | 16      | 8       | 12      |
@ival    |         | 20      | 28      | 28      | 28      | 20      | 24      |
label    |         | 8       | 16      | 16      | 16      | 8       | 12      |
@label   |         | 20      | 28      | 28      | 28      | 20      | 24      |
freg     |         | -       | -       | -       | -       | -       | -       |
fval     |         | -       | -       | -       | -       | -       | -       |


         |**argB** | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
---------|---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |         |
areg     |         | 16      | -       | 20      | -       | 20      | -       | -       |
@areg    |         | 24      | -       | 28      | -       | 28      | -       | -       |
@areg+   |         | 24      | -       | 28      | -       | 28      | -       | -       |
@areg-   |         | 24      | -       | 28      | -       | 28      | -       | -       |
dreg     |         | 16      | -       | 20      | -       | 20      | -       | -       |
sp(ofs)  |         | 20      | -       | 24      | -       | 24      | -       | -       |
@sp(ofs) |         | 28      | -       | 32      | -       | 32      | -       | -       |
ival     |         | 20      | -       | 24      | -       | 24      | -       | -       |
@ival    |         | 32      | -       | 36      | -       | 36      | -       | -       |
label    |         | 20      | -       | 24      | -       | 24      | -       | -       |
@label   |         | 32      | -       | 36      | -       | 36      | -       | -       |
freg     |         | -       | -       | -       | -       | -       | -       | -       |
fval     |         | -       | -       | -       | -       | -       | -       | -       |


ror
---

### Syntax
	ror<.b/.w/.l> argA, argB

### Function
Perform a bitwise right shift of the value at the location specified by argB. Number of bits to shift is the value specified by argA.

### Operand Types and Cycle Counts

         |**argB** | areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) |
---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |
areg     |         | 4       | 12      | 12      | 12      | 4       | 8       |
@areg    |         | 12      | 20      | 20      | 20      | 12      | 16      |
@areg+   |         | 12      | 20      | 20      | 20      | 12      | 16      |
@areg-   |         | 12      | 20      | 20      | 20      | 12      | 16      |
dreg     |         | 4       | 12      | 12      | 12      | 4       | 8       |
sp(ofs)  |         | 8       | 16      | 16      | 16      | 8       | 12      |
@sp(ofs) |         | 16      | 24      | 24      | 24      | 16      | 20      |
ival     |         | 8       | 16      | 16      | 16      | 8       | 12      |
@ival    |         | 20      | 28      | 28      | 28      | 20      | 24      |
label    |         | 8       | 16      | 16      | 16      | 8       | 12      |
@label   |         | 20      | 28      | 28      | 28      | 20      | 24      |
freg     |         | -       | -       | -       | -       | -       | -       |
fval     |         | -       | -       | -       | -       | -       | -       |


         |**argB** | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
---------|---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |         |
areg     |         | 16      | -       | 20      | -       | 20      | -       | -       |
@areg    |         | 24      | -       | 28      | -       | 28      | -       | -       |
@areg+   |         | 24      | -       | 28      | -       | 28      | -       | -       |
@areg-   |         | 24      | -       | 28      | -       | 28      | -       | -       |
dreg     |         | 16      | -       | 20      | -       | 20      | -       | -       |
sp(ofs)  |         | 20      | -       | 24      | -       | 24      | -       | -       |
@sp(ofs) |         | 28      | -       | 32      | -       | 32      | -       | -       |
ival     |         | 20      | -       | 24      | -       | 24      | -       | -       |
@ival    |         | 32      | -       | 36      | -       | 36      | -       | -       |
label    |         | 20      | -       | 24      | -       | 24      | -       | -       |
@label   |         | 32      | -       | 36      | -       | 36      | -       | -       |
freg     |         | -       | -       | -       | -       | -       | -       | -       |
fval     |         | -       | -       | -       | -       | -       | -       | -       |


mod
---

### Syntax
	mod<.b/.w/.l> argA, argB

### Function
Perform MOD on argB by argA and store the result at the location specified by argB.

### Operand Types and Cycle Counts

         |**argB** | areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) |
---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |
areg     |         | 8       | 16      | 16      | 16      | 8       | 12      |
@areg    |         | 16      | 24      | 24      | 24      | 16      | 20      |
@areg+   |         | 16      | 24      | 24      | 24      | 16      | 20      |
@areg-   |         | 16      | 24      | 24      | 24      | 16      | 20      |
dreg     |         | 8       | 16      | 16      | 16      | 8       | 12      |
sp(ofs)  |         | 12      | 20      | 20      | 20      | 12      | 16      |
@sp(ofs) |         | 20      | 28      | 28      | 28      | 20      | 24      |
ival     |         | 12      | 20      | 20      | 20      | 12      | 16      |
@ival    |         | 24      | 32      | 32      | 32      | 24      | 28      |
label    |         | 12      | 20      | 20      | 20      | 12      | 16      |
@label   |         | 24      | 32      | 32      | 32      | 24      | 28      |
freg     |         | -       | -       | -       | -       | -       | -       |
fval     |         | -       | -       | -       | -       | -       | -       |


         |**argB** | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
---------|---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |         |
areg     |         | 20      | -       | 24      | -       | 24      | 8       | -       |
@areg    |         | 28      | -       | 32      | -       | 32      | 16      | -       |
@areg+   |         | 28      | -       | 32      | -       | 32      | 16      | -       |
@areg-   |         | 28      | -       | 32      | -       | 32      | 16      | -       |
dreg     |         | 20      | -       | 24      | -       | 24      | 8       | -       |
sp(ofs)  |         | 24      | -       | 28      | -       | 28      | 12      | -       |
@sp(ofs) |         | 32      | -       | 36      | -       | 36      | 20      | -       |
ival     |         | 24      | -       | 28      | -       | 28      | 12      | -       |
@ival    |         | 36      | -       | 40      | -       | 40      | 24      | -       |
label    |         | 24      | -       | 28      | -       | 28      | 12      | -       |
@label   |         | 36      | -       | 40      | -       | 40      | 24      | -       |
freg     |         | -       | -       | -       | -       | -       | -       | -       |
fval     |         | -       | -       | -       | -       | -       | -       | -       |


mod
---

### Syntax
	mod.f argA, argB

### Function
Perform MOD on argB by argA and store the result at the location specified by argB.

### Operand Types and Cycle Counts

         |**argB** | areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) |
---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |
areg     |         | -       | -       | -       | -       | -       | -       |
@areg    |         | -       | 32      | -       | -       | -       | 28      |
@areg+   |         | -       | -       | -       | -       | -       | -       |
@areg-   |         | -       | -       | -       | -       | -       | -       |
dreg     |         | -       | -       | -       | -       | -       | -       |
sp(ofs)  |         | -       | 28      | -       | -       | -       | 24      |
@sp(ofs) |         | -       | 36      | -       | -       | -       | 32      |
ival     |         | -       | -       | -       | -       | -       | -       |
@ival    |         | -       | 40      | -       | -       | -       | 36      |
label    |         | -       | -       | -       | -       | -       | -       |
@label   |         | -       | 40      | -       | -       | -       | 36      |
freg     |         | -       | 24      | -       | -       | -       | 20      |
fval     |         | -       | 28      | -       | -       | -       | 24      |


         |**argB** | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
---------|---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |         |
areg     |         | -       | -       | -       | -       | -       | -       | -       |
@areg    |         | 36      | -       | 40      | -       | 40      | 24      | -       |
@areg+   |         | -       | -       | -       | -       | -       | -       | -       |
@areg-   |         | -       | -       | -       | -       | -       | -       | -       |
dreg     |         | -       | -       | -       | -       | -       | -       | -       |
sp(ofs)  |         | 32      | -       | 36      | -       | 36      | 20      | -       |
@sp(ofs) |         | 40      | -       | 44      | -       | 44      | 28      | -       |
ival     |         | -       | -       | -       | -       | -       | -       | -       |
@ival    |         | 44      | -       | 48      | -       | 48      | 32      | -       |
label    |         | -       | -       | -       | -       | -       | -       | -       |
@label   |         | 44      | -       | 48      | -       | 48      | 32      | -       |
freg     |         | 28      | -       | 32      | -       | 32      | 16      | -       |
fval     |         | 32      | -       | 36      | -       | 36      | 20      | -       |


abs
---

### Syntax
	abs<.b/.w/.l> argA, argB

### Function
Perform ABS on argA and store the result at the location specified by argB.

### Operand Types and Cycle Counts

         |**argB** | areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) |
---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |
areg     |         | 8       | 16      | 16      | 16      | 8       | 12      |
@areg    |         | 16      | 24      | 24      | 24      | 16      | 20      |
@areg+   |         | 16      | 24      | 24      | 24      | 16      | 20      |
@areg-   |         | 16      | 24      | 24      | 24      | 16      | 20      |
dreg     |         | 8       | 16      | 16      | 16      | 8       | 12      |
sp(ofs)  |         | 12      | 20      | 20      | 20      | 12      | 16      |
@sp(ofs) |         | 20      | 28      | 28      | 28      | 20      | 24      |
ival     |         | 12      | 20      | 20      | 20      | 12      | 16      |
@ival    |         | 24      | 32      | 32      | 32      | 24      | 28      |
label    |         | 12      | 20      | 20      | 20      | 12      | 16      |
@label   |         | 24      | 32      | 32      | 32      | 24      | 28      |
freg     |         | -       | -       | -       | -       | -       | -       |
fval     |         | -       | -       | -       | -       | -       | -       |


         |**argB** | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
---------|---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |         |
areg     |         | 20      | -       | 24      | -       | 24      | 8       | -       |
@areg    |         | 28      | -       | 32      | -       | 32      | 16      | -       |
@areg+   |         | 28      | -       | 32      | -       | 32      | 16      | -       |
@areg-   |         | 28      | -       | 32      | -       | 32      | 16      | -       |
dreg     |         | 20      | -       | 24      | -       | 24      | 8       | -       |
sp(ofs)  |         | 24      | -       | 28      | -       | 28      | 12      | -       |
@sp(ofs) |         | 32      | -       | 36      | -       | 36      | 20      | -       |
ival     |         | 24      | -       | 28      | -       | 28      | 12      | -       |
@ival    |         | 36      | -       | 40      | -       | 40      | 24      | -       |
label    |         | 24      | -       | 28      | -       | 28      | 12      | -       |
@label   |         | 36      | -       | 40      | -       | 40      | 24      | -       |
freg     |         | -       | -       | -       | -       | -       | -       | -       |
fval     |         | -       | -       | -       | -       | -       | -       | -       |


abs
---

### Syntax
	abs.f argA, argB

### Function
Perform ABS on argA and store the result at the location specified by argB.

### Operand Types and Cycle Counts

         |**argB** | areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) |
---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |
areg     |         | -       | -       | -       | -       | -       | -       |
@areg    |         | -       | 32      | -       | -       | -       | 28      |
@areg+   |         | -       | -       | -       | -       | -       | -       |
@areg-   |         | -       | -       | -       | -       | -       | -       |
dreg     |         | -       | -       | -       | -       | -       | -       |
sp(ofs)  |         | -       | 28      | -       | -       | -       | 24      |
@sp(ofs) |         | -       | 36      | -       | -       | -       | 32      |
ival     |         | -       | -       | -       | -       | -       | -       |
@ival    |         | -       | 40      | -       | -       | -       | 36      |
label    |         | -       | -       | -       | -       | -       | -       |
@label   |         | -       | 40      | -       | -       | -       | 36      |
freg     |         | -       | 24      | -       | -       | -       | 20      |
fval     |         | -       | 28      | -       | -       | -       | 24      |


         |**argB** | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
---------|---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |         |
areg     |         | -       | -       | -       | -       | -       | -       | -       |
@areg    |         | 36      | -       | 40      | -       | 40      | 24      | -       |
@areg+   |         | -       | -       | -       | -       | -       | -       | -       |
@areg-   |         | -       | -       | -       | -       | -       | -       | -       |
dreg     |         | -       | -       | -       | -       | -       | -       | -       |
sp(ofs)  |         | 32      | -       | 36      | -       | 36      | 20      | -       |
@sp(ofs) |         | 40      | -       | 44      | -       | 44      | 28      | -       |
ival     |         | -       | -       | -       | -       | -       | -       | -       |
@ival    |         | 44      | -       | 48      | -       | 48      | 32      | -       |
label    |         | -       | -       | -       | -       | -       | -       | -       |
@label   |         | 44      | -       | 48      | -       | 48      | 32      | -       |
freg     |         | 28      | -       | 32      | -       | 32      | 16      | -       |
fval     |         | 32      | -       | 36      | -       | 36      | 20      | -       |


pow
---

### Syntax
	pow<.b/.w/.l> argA, argB

### Function
Perform POW on argB by argA and store the result at the location specified by argB.

### Operand Types and Cycle Counts

         |**argB** | areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) |
---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |
areg     |         | 8       | 16      | 16      | 16      | 8       | 12      |
@areg    |         | 16      | 24      | 24      | 24      | 16      | 20      |
@areg+   |         | 16      | 24      | 24      | 24      | 16      | 20      |
@areg-   |         | 16      | 24      | 24      | 24      | 16      | 20      |
dreg     |         | 8       | 16      | 16      | 16      | 8       | 12      |
sp(ofs)  |         | 12      | 20      | 20      | 20      | 12      | 16      |
@sp(ofs) |         | 20      | 28      | 28      | 28      | 20      | 24      |
ival     |         | 12      | 20      | 20      | 20      | 12      | 16      |
@ival    |         | 24      | 32      | 32      | 32      | 24      | 28      |
label    |         | 12      | 20      | 20      | 20      | 12      | 16      |
@label   |         | 24      | 32      | 32      | 32      | 24      | 28      |
freg     |         | -       | -       | -       | -       | -       | -       |
fval     |         | -       | -       | -       | -       | -       | -       |


         |**argB** | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
---------|---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |         |
areg     |         | 20      | -       | 24      | -       | 24      | 8       | -       |
@areg    |         | 28      | -       | 32      | -       | 32      | 16      | -       |
@areg+   |         | 28      | -       | 32      | -       | 32      | 16      | -       |
@areg-   |         | 28      | -       | 32      | -       | 32      | 16      | -       |
dreg     |         | 20      | -       | 24      | -       | 24      | 8       | -       |
sp(ofs)  |         | 24      | -       | 28      | -       | 28      | 12      | -       |
@sp(ofs) |         | 32      | -       | 36      | -       | 36      | 20      | -       |
ival     |         | 24      | -       | 28      | -       | 28      | 12      | -       |
@ival    |         | 36      | -       | 40      | -       | 40      | 24      | -       |
label    |         | 24      | -       | 28      | -       | 28      | 12      | -       |
@label   |         | 36      | -       | 40      | -       | 40      | 24      | -       |
freg     |         | -       | -       | -       | -       | -       | -       | -       |
fval     |         | -       | -       | -       | -       | -       | -       | -       |


pow
---

### Syntax
	pow.f argA, argB

### Function
Perform POW on argB by argA and store the result at the location specified by argB.

### Operand Types and Cycle Counts

         |**argB** | areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) |
---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |
areg     |         | -       | -       | -       | -       | -       | -       |
@areg    |         | -       | 32      | -       | -       | -       | 28      |
@areg+   |         | -       | -       | -       | -       | -       | -       |
@areg-   |         | -       | -       | -       | -       | -       | -       |
dreg     |         | -       | -       | -       | -       | -       | -       |
sp(ofs)  |         | -       | 28      | -       | -       | -       | 24      |
@sp(ofs) |         | -       | 36      | -       | -       | -       | 32      |
ival     |         | -       | -       | -       | -       | -       | -       |
@ival    |         | -       | 40      | -       | -       | -       | 36      |
label    |         | -       | -       | -       | -       | -       | -       |
@label   |         | -       | 40      | -       | -       | -       | 36      |
freg     |         | -       | 24      | -       | -       | -       | 20      |
fval     |         | -       | 28      | -       | -       | -       | 24      |


         |**argB** | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
---------|---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |         |
areg     |         | -       | -       | -       | -       | -       | -       | -       |
@areg    |         | 36      | -       | 40      | -       | 40      | 24      | -       |
@areg+   |         | -       | -       | -       | -       | -       | -       | -       |
@areg-   |         | -       | -       | -       | -       | -       | -       | -       |
dreg     |         | -       | -       | -       | -       | -       | -       | -       |
sp(ofs)  |         | 32      | -       | 36      | -       | 36      | 20      | -       |
@sp(ofs) |         | 40      | -       | 44      | -       | 44      | 28      | -       |
ival     |         | -       | -       | -       | -       | -       | -       | -       |
@ival    |         | 44      | -       | 48      | -       | 48      | 32      | -       |
label    |         | -       | -       | -       | -       | -       | -       | -       |
@label   |         | 44      | -       | 48      | -       | 48      | 32      | -       |
freg     |         | 28      | -       | 32      | -       | 32      | 16      | -       |
fval     |         | 32      | -       | 36      | -       | 36      | 20      | -       |


sin
---

### Syntax
	sin.f argA, argB

### Function
Calculate SIN of the value specified by argA, and store the result at the location specified by argB.

### Operand Types and Cycle Counts

         |**argB** | areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) |
---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |
areg     |         | -       | -       | -       | -       | -       | -       |
@areg    |         | -       | 32      | -       | -       | -       | 28      |
@areg+   |         | -       | -       | -       | -       | -       | -       |
@areg-   |         | -       | -       | -       | -       | -       | -       |
dreg     |         | -       | -       | -       | -       | -       | -       |
sp(ofs)  |         | -       | 28      | -       | -       | -       | 24      |
@sp(ofs) |         | -       | 36      | -       | -       | -       | 32      |
ival     |         | -       | -       | -       | -       | -       | -       |
@ival    |         | -       | 40      | -       | -       | -       | 36      |
label    |         | -       | -       | -       | -       | -       | -       |
@label   |         | -       | 40      | -       | -       | -       | 36      |
freg     |         | -       | 24      | -       | -       | -       | 20      |
fval     |         | -       | 28      | -       | -       | -       | 24      |


         |**argB** | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
---------|---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |         |
areg     |         | -       | -       | -       | -       | -       | -       | -       |
@areg    |         | 36      | -       | 40      | -       | 40      | 24      | -       |
@areg+   |         | -       | -       | -       | -       | -       | -       | -       |
@areg-   |         | -       | -       | -       | -       | -       | -       | -       |
dreg     |         | -       | -       | -       | -       | -       | -       | -       |
sp(ofs)  |         | 32      | -       | 36      | -       | 36      | 20      | -       |
@sp(ofs) |         | 40      | -       | 44      | -       | 44      | 28      | -       |
ival     |         | -       | -       | -       | -       | -       | -       | -       |
@ival    |         | 44      | -       | 48      | -       | 48      | 32      | -       |
label    |         | -       | -       | -       | -       | -       | -       | -       |
@label   |         | 44      | -       | 48      | -       | 48      | 32      | -       |
freg     |         | 28      | -       | 32      | -       | 32      | 16      | -       |
fval     |         | 32      | -       | 36      | -       | 36      | 20      | -       |


cos
---

### Syntax
	cos.f argA, argB

### Function
Calculate COS of the value specified by argA, and store the result at the location specified by argB.

### Operand Types and Cycle Counts

         |**argB** | areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) |
---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |
areg     |         | -       | -       | -       | -       | -       | -       |
@areg    |         | -       | 32      | -       | -       | -       | 28      |
@areg+   |         | -       | -       | -       | -       | -       | -       |
@areg-   |         | -       | -       | -       | -       | -       | -       |
dreg     |         | -       | -       | -       | -       | -       | -       |
sp(ofs)  |         | -       | 28      | -       | -       | -       | 24      |
@sp(ofs) |         | -       | 36      | -       | -       | -       | 32      |
ival     |         | -       | -       | -       | -       | -       | -       |
@ival    |         | -       | 40      | -       | -       | -       | 36      |
label    |         | -       | -       | -       | -       | -       | -       |
@label   |         | -       | 40      | -       | -       | -       | 36      |
freg     |         | -       | 24      | -       | -       | -       | 20      |
fval     |         | -       | 28      | -       | -       | -       | 24      |


         |**argB** | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
---------|---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |         |
areg     |         | -       | -       | -       | -       | -       | -       | -       |
@areg    |         | 36      | -       | 40      | -       | 40      | 24      | -       |
@areg+   |         | -       | -       | -       | -       | -       | -       | -       |
@areg-   |         | -       | -       | -       | -       | -       | -       | -       |
dreg     |         | -       | -       | -       | -       | -       | -       | -       |
sp(ofs)  |         | 32      | -       | 36      | -       | 36      | 20      | -       |
@sp(ofs) |         | 40      | -       | 44      | -       | 44      | 28      | -       |
ival     |         | -       | -       | -       | -       | -       | -       | -       |
@ival    |         | 44      | -       | 48      | -       | 48      | 32      | -       |
label    |         | -       | -       | -       | -       | -       | -       | -       |
@label   |         | 44      | -       | 48      | -       | 48      | 32      | -       |
freg     |         | 28      | -       | 32      | -       | 32      | 16      | -       |
fval     |         | 32      | -       | 36      | -       | 36      | 20      | -       |


tan
---

### Syntax
	tan.f argA, argB

### Function
Calculate TAN of the value specified by argA, and store the result at the location specified by argB.

### Operand Types and Cycle Counts

         |**argB** | areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) |
---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |
areg     |         | -       | -       | -       | -       | -       | -       |
@areg    |         | -       | 32      | -       | -       | -       | 28      |
@areg+   |         | -       | -       | -       | -       | -       | -       |
@areg-   |         | -       | -       | -       | -       | -       | -       |
dreg     |         | -       | -       | -       | -       | -       | -       |
sp(ofs)  |         | -       | 28      | -       | -       | -       | 24      |
@sp(ofs) |         | -       | 36      | -       | -       | -       | 32      |
ival     |         | -       | -       | -       | -       | -       | -       |
@ival    |         | -       | 40      | -       | -       | -       | 36      |
label    |         | -       | -       | -       | -       | -       | -       |
@label   |         | -       | 40      | -       | -       | -       | 36      |
freg     |         | -       | 24      | -       | -       | -       | 20      |
fval     |         | -       | 28      | -       | -       | -       | 24      |


         |**argB** | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
---------|---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |         |
areg     |         | -       | -       | -       | -       | -       | -       | -       |
@areg    |         | 36      | -       | 40      | -       | 40      | 24      | -       |
@areg+   |         | -       | -       | -       | -       | -       | -       | -       |
@areg-   |         | -       | -       | -       | -       | -       | -       | -       |
dreg     |         | -       | -       | -       | -       | -       | -       | -       |
sp(ofs)  |         | 32      | -       | 36      | -       | 36      | 20      | -       |
@sp(ofs) |         | 40      | -       | 44      | -       | 44      | 28      | -       |
ival     |         | -       | -       | -       | -       | -       | -       | -       |
@ival    |         | 44      | -       | 48      | -       | 48      | 32      | -       |
label    |         | -       | -       | -       | -       | -       | -       | -       |
@label   |         | 44      | -       | 48      | -       | 48      | 32      | -       |
freg     |         | 28      | -       | 32      | -       | 32      | 16      | -       |
fval     |         | 32      | -       | 36      | -       | 36      | 20      | -       |


asin
----

### Syntax
	asin.f argA, argB

### Function
Calculate ASIN of the value specified by argA, and store the result at the location specified by argB.

### Operand Types and Cycle Counts

         |**argB** | areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) |
---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |
areg     |         | -       | -       | -       | -       | -       | -       |
@areg    |         | -       | 32      | -       | -       | -       | 28      |
@areg+   |         | -       | -       | -       | -       | -       | -       |
@areg-   |         | -       | -       | -       | -       | -       | -       |
dreg     |         | -       | -       | -       | -       | -       | -       |
sp(ofs)  |         | -       | 28      | -       | -       | -       | 24      |
@sp(ofs) |         | -       | 36      | -       | -       | -       | 32      |
ival     |         | -       | -       | -       | -       | -       | -       |
@ival    |         | -       | 40      | -       | -       | -       | 36      |
label    |         | -       | -       | -       | -       | -       | -       |
@label   |         | -       | 40      | -       | -       | -       | 36      |
freg     |         | -       | 24      | -       | -       | -       | 20      |
fval     |         | -       | 28      | -       | -       | -       | 24      |


         |**argB** | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
---------|---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |         |
areg     |         | -       | -       | -       | -       | -       | -       | -       |
@areg    |         | 36      | -       | 40      | -       | 40      | 24      | -       |
@areg+   |         | -       | -       | -       | -       | -       | -       | -       |
@areg-   |         | -       | -       | -       | -       | -       | -       | -       |
dreg     |         | -       | -       | -       | -       | -       | -       | -       |
sp(ofs)  |         | 32      | -       | 36      | -       | 36      | 20      | -       |
@sp(ofs) |         | 40      | -       | 44      | -       | 44      | 28      | -       |
ival     |         | -       | -       | -       | -       | -       | -       | -       |
@ival    |         | 44      | -       | 48      | -       | 48      | 32      | -       |
label    |         | -       | -       | -       | -       | -       | -       | -       |
@label   |         | 44      | -       | 48      | -       | 48      | 32      | -       |
freg     |         | 28      | -       | 32      | -       | 32      | 16      | -       |
fval     |         | 32      | -       | 36      | -       | 36      | 20      | -       |


acos
----

### Syntax
	acos.f argA, argB

### Function
Calculate ACOS of the value specified by argA, and store the result at the location specified by argB.

### Operand Types and Cycle Counts

         |**argB** | areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) |
---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |
areg     |         | -       | -       | -       | -       | -       | -       |
@areg    |         | -       | 32      | -       | -       | -       | 28      |
@areg+   |         | -       | -       | -       | -       | -       | -       |
@areg-   |         | -       | -       | -       | -       | -       | -       |
dreg     |         | -       | -       | -       | -       | -       | -       |
sp(ofs)  |         | -       | 28      | -       | -       | -       | 24      |
@sp(ofs) |         | -       | 36      | -       | -       | -       | 32      |
ival     |         | -       | -       | -       | -       | -       | -       |
@ival    |         | -       | 40      | -       | -       | -       | 36      |
label    |         | -       | -       | -       | -       | -       | -       |
@label   |         | -       | 40      | -       | -       | -       | 36      |
freg     |         | -       | 24      | -       | -       | -       | 20      |
fval     |         | -       | 28      | -       | -       | -       | 24      |


         |**argB** | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
---------|---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |         |
areg     |         | -       | -       | -       | -       | -       | -       | -       |
@areg    |         | 36      | -       | 40      | -       | 40      | 24      | -       |
@areg+   |         | -       | -       | -       | -       | -       | -       | -       |
@areg-   |         | -       | -       | -       | -       | -       | -       | -       |
dreg     |         | -       | -       | -       | -       | -       | -       | -       |
sp(ofs)  |         | 32      | -       | 36      | -       | 36      | 20      | -       |
@sp(ofs) |         | 40      | -       | 44      | -       | 44      | 28      | -       |
ival     |         | -       | -       | -       | -       | -       | -       | -       |
@ival    |         | 44      | -       | 48      | -       | 48      | 32      | -       |
label    |         | -       | -       | -       | -       | -       | -       | -       |
@label   |         | 44      | -       | 48      | -       | 48      | 32      | -       |
freg     |         | 28      | -       | 32      | -       | 32      | 16      | -       |
fval     |         | 32      | -       | 36      | -       | 36      | 20      | -       |


atan
----

### Syntax
	atan.f argA, argB

### Function
Calculate ATAN of the value specified by argA, and store the result at the location specified by argB.

### Operand Types and Cycle Counts

         |**argB** | areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) |
---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |
areg     |         | -       | -       | -       | -       | -       | -       |
@areg    |         | -       | 32      | -       | -       | -       | 28      |
@areg+   |         | -       | -       | -       | -       | -       | -       |
@areg-   |         | -       | -       | -       | -       | -       | -       |
dreg     |         | -       | -       | -       | -       | -       | -       |
sp(ofs)  |         | -       | 28      | -       | -       | -       | 24      |
@sp(ofs) |         | -       | 36      | -       | -       | -       | 32      |
ival     |         | -       | -       | -       | -       | -       | -       |
@ival    |         | -       | 40      | -       | -       | -       | 36      |
label    |         | -       | -       | -       | -       | -       | -       |
@label   |         | -       | 40      | -       | -       | -       | 36      |
freg     |         | -       | 24      | -       | -       | -       | 20      |
fval     |         | -       | 28      | -       | -       | -       | 24      |


         |**argB** | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
---------|---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |         |
areg     |         | -       | -       | -       | -       | -       | -       | -       |
@areg    |         | 36      | -       | 40      | -       | 40      | 24      | -       |
@areg+   |         | -       | -       | -       | -       | -       | -       | -       |
@areg-   |         | -       | -       | -       | -       | -       | -       | -       |
dreg     |         | -       | -       | -       | -       | -       | -       | -       |
sp(ofs)  |         | 32      | -       | 36      | -       | 36      | 20      | -       |
@sp(ofs) |         | 40      | -       | 44      | -       | 44      | 28      | -       |
ival     |         | -       | -       | -       | -       | -       | -       | -       |
@ival    |         | 44      | -       | 48      | -       | 48      | 32      | -       |
label    |         | -       | -       | -       | -       | -       | -       | -       |
@label   |         | 44      | -       | 48      | -       | 48      | 32      | -       |
freg     |         | 28      | -       | 32      | -       | 32      | 16      | -       |
fval     |         | 32      | -       | 36      | -       | 36      | 20      | -       |


exp
---

### Syntax
	exp.f argA, argB

### Function
Calculate EXP of the value specified by argA, and store the result at the location specified by argB.

### Operand Types and Cycle Counts

         |**argB** | areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) |
---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |
areg     |         | -       | -       | -       | -       | -       | -       |
@areg    |         | -       | 32      | -       | -       | -       | 28      |
@areg+   |         | -       | -       | -       | -       | -       | -       |
@areg-   |         | -       | -       | -       | -       | -       | -       |
dreg     |         | -       | -       | -       | -       | -       | -       |
sp(ofs)  |         | -       | 28      | -       | -       | -       | 24      |
@sp(ofs) |         | -       | 36      | -       | -       | -       | 32      |
ival     |         | -       | -       | -       | -       | -       | -       |
@ival    |         | -       | 40      | -       | -       | -       | 36      |
label    |         | -       | -       | -       | -       | -       | -       |
@label   |         | -       | 40      | -       | -       | -       | 36      |
freg     |         | -       | 24      | -       | -       | -       | 20      |
fval     |         | -       | 28      | -       | -       | -       | 24      |


         |**argB** | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
---------|---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |         |
areg     |         | -       | -       | -       | -       | -       | -       | -       |
@areg    |         | 36      | -       | 40      | -       | 40      | 24      | -       |
@areg+   |         | -       | -       | -       | -       | -       | -       | -       |
@areg-   |         | -       | -       | -       | -       | -       | -       | -       |
dreg     |         | -       | -       | -       | -       | -       | -       | -       |
sp(ofs)  |         | 32      | -       | 36      | -       | 36      | 20      | -       |
@sp(ofs) |         | 40      | -       | 44      | -       | 44      | 28      | -       |
ival     |         | -       | -       | -       | -       | -       | -       | -       |
@ival    |         | 44      | -       | 48      | -       | 48      | 32      | -       |
label    |         | -       | -       | -       | -       | -       | -       | -       |
@label   |         | 44      | -       | 48      | -       | 48      | 32      | -       |
freg     |         | 28      | -       | 32      | -       | 32      | 16      | -       |
fval     |         | 32      | -       | 36      | -       | 36      | 20      | -       |


log
---

### Syntax
	log.f argA, argB

### Function
Calculate LOG of the value specified by argA, and store the result at the location specified by argB.

### Operand Types and Cycle Counts

         |**argB** | areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) |
---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |
areg     |         | -       | -       | -       | -       | -       | -       |
@areg    |         | -       | 32      | -       | -       | -       | 28      |
@areg+   |         | -       | -       | -       | -       | -       | -       |
@areg-   |         | -       | -       | -       | -       | -       | -       |
dreg     |         | -       | -       | -       | -       | -       | -       |
sp(ofs)  |         | -       | 28      | -       | -       | -       | 24      |
@sp(ofs) |         | -       | 36      | -       | -       | -       | 32      |
ival     |         | -       | -       | -       | -       | -       | -       |
@ival    |         | -       | 40      | -       | -       | -       | 36      |
label    |         | -       | -       | -       | -       | -       | -       |
@label   |         | -       | 40      | -       | -       | -       | 36      |
freg     |         | -       | 24      | -       | -       | -       | 20      |
fval     |         | -       | 28      | -       | -       | -       | 24      |


         |**argB** | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
---------|---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |         |
areg     |         | -       | -       | -       | -       | -       | -       | -       |
@areg    |         | 36      | -       | 40      | -       | 40      | 24      | -       |
@areg+   |         | -       | -       | -       | -       | -       | -       | -       |
@areg-   |         | -       | -       | -       | -       | -       | -       | -       |
dreg     |         | -       | -       | -       | -       | -       | -       | -       |
sp(ofs)  |         | 32      | -       | 36      | -       | 36      | 20      | -       |
@sp(ofs) |         | 40      | -       | 44      | -       | 44      | 28      | -       |
ival     |         | -       | -       | -       | -       | -       | -       | -       |
@ival    |         | 44      | -       | 48      | -       | 48      | 32      | -       |
label    |         | -       | -       | -       | -       | -       | -       | -       |
@label   |         | 44      | -       | 48      | -       | 48      | 32      | -       |
freg     |         | 28      | -       | 32      | -       | 32      | 16      | -       |
fval     |         | 32      | -       | 36      | -       | 36      | 20      | -       |


sqrt
----

### Syntax
	sqrt.f argA, argB

### Function
Calculate SQRT of the value specified by argA, and store the result at the location specified by argB.

### Operand Types and Cycle Counts

         |**argB** | areg    | @areg   | @areg+  | @areg-  | dreg    | sp(ofs) |
---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |
areg     |         | -       | -       | -       | -       | -       | -       |
@areg    |         | -       | 32      | -       | -       | -       | 28      |
@areg+   |         | -       | -       | -       | -       | -       | -       |
@areg-   |         | -       | -       | -       | -       | -       | -       |
dreg     |         | -       | -       | -       | -       | -       | -       |
sp(ofs)  |         | -       | 28      | -       | -       | -       | 24      |
@sp(ofs) |         | -       | 36      | -       | -       | -       | 32      |
ival     |         | -       | -       | -       | -       | -       | -       |
@ival    |         | -       | 40      | -       | -       | -       | 36      |
label    |         | -       | -       | -       | -       | -       | -       |
@label   |         | -       | 40      | -       | -       | -       | 36      |
freg     |         | -       | 24      | -       | -       | -       | 20      |
fval     |         | -       | 28      | -       | -       | -       | 24      |


         |**argB** | @sp(ofs)| ival    | @ival   | label   | @label  | freg    | fval    |
---------|---------|---------|---------|---------|---------|---------|---------|---------|
**argA** |         |         |         |         |         |         |         |         |
areg     |         | -       | -       | -       | -       | -       | -       | -       |
@areg    |         | 36      | -       | 40      | -       | 40      | 24      | -       |
@areg+   |         | -       | -       | -       | -       | -       | -       | -       |
@areg-   |         | -       | -       | -       | -       | -       | -       | -       |
dreg     |         | -       | -       | -       | -       | -       | -       | -       |
sp(ofs)  |         | 32      | -       | 36      | -       | 36      | 20      | -       |
@sp(ofs) |         | 40      | -       | 44      | -       | 44      | 28      | -       |
ival     |         | -       | -       | -       | -       | -       | -       | -       |
@ival    |         | 44      | -       | 48      | -       | 48      | 32      | -       |
label    |         | -       | -       | -       | -       | -       | -       | -       |
@label   |         | 44      | -       | 48      | -       | 48      | 32      | -       |
freg     |         | 28      | -       | 32      | -       | 32      | 16      | -       |
fval     |         | 32      | -       | 36      | -       | 36      | 20      | -       |

