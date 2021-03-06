*#cpu 8085
*
* DDS MICRO-C 8080/85/Z80 Startup Code and Runtime library
*
* Copyright 1991-2005 Dave Dunfield
* All rights reserved.
*
	ORG	$4000		Place code in memory here
	LXI	SP,$6000	Initial stack goes here
	XRA	A		Get a ZERO
	STA	?heap		Initialize the heap
	CALL	main		Execute user program
* Return to MON85 by issuing a RESTART 1 instruction, which is MON85's
* 'breakpoint' handler. This will save the user registers, and perform
* a context switch back to the monitor.
exit	RST	1		Enter monitor
* Get byte from stack variable: L = [SP+i]
?gstkbi	POP	H		HL = Stack
	MOV	A,M		Get value
	INX	H		Skip
	PUSH	H		Restore stack
	MOV	L,A		L = low address
	MVI	H,0		Zero high
* Get byte from stack variable: L = [SP+HL]
?gstkb	DAD	SP		Offset to stack
	MOV	L,M		Get data byte
	RET
* Put byte into stack variable: [SP+i] = L
?pstkbi	XTHL			HL = immediate address
	MOV	C,M		Get value
	INX	H		Skip
	XTHL			Restore stack
	MVI	B,0		Zero high
* Put byte into stack variable: [SP+BC] = L
?pstkb	PUSH	H		Save value
	MOV	H,B		Get high offset
	MOV	L,C		Get low offset
	POP	B		Get return value
	DAD	SP		Offset to variable
	MOV	M,C		Write high byte
	MOV	H,B		Restore high
	MOV	L,C		Restore low
	RET
* Get word from stack variable: HL = [SP+i]
?gstkwi	POP	H		Get address
	MOV	A,M		Get value
	INX	H		Skip
	PUSH	H		Restore stack
	MVI	H,0		Zero high
* Get word from stack variable: HL = [SP+HL]
?gstkw	DAD	SP		Offset to stack
	MOV	A,M		Get low data byte
	INX	H		Offset to next
	MOV	H,M		Get high data byte
	MOV	L,A		Include low byte
	RET
* Put work into stack variable: [SP+i] = HL
?pstkwi	XTHL			Get address
	MOV	C,M		Get value
	INX	H		Skip
	XTHL			Restore stack
	MVI	B,0		Zero high
* Put word into stack variable: [SP+BC] = HL
?pstkw	PUSH	H		Save value
	MOV	H,B		Get high offset
	MOV	L,C		Get low offset
	POP	B		Restore value
	DAD	SP		Offset to variable
	MOV	M,C		Write low byte
	INX	H		Advance
	MOV	M,B		Write high byte
	MOV	H,B		Restore high
	MOV	L,C		Restore low
	RET
* Get a 16 bit value indirect through index: HL = [DE]
?gind_d	XCHG			HL = Index register
	MOV	E,M		Get low byte
	INX	H		Advance to next
	MOV	D,M		Get high byte
	DCX	H		Restore index
	XCHG			Swap back
	RET
* Put a 16 bit value indirect through index: [DE] = HL
?pind_d	XCHG			Swap
	MOV	M,E		Write low byte
	INX	H		Advance to next
	MOV	M,D		Write high byte
	DCX	H		Restore index
	XCHG			Swap back
	RET
* Get a 16 bit value indirect through HL: HL = [HL]
?gind_h	MOV	A,M		Get low byte
	INX	H		Advance
	MOV	H,M		Get high byte
	MOV	L,A		Include low byte
	RET
* Put a 16 bit value indirect through BC: [BC] = HL
?pind_b	MOV	A,L		Get low byte
	STAX	B		Write it
	INX	B		Advance it
	MOV	A,H		Get high byte
	STAX	B		Write it
	RET
* Sign extend L into H
?sign	MOV	A,L		Get character value
	RLC			Set 'C' if A < 0
	SBB	A		Get sign extension
	MOV	H,A		Set high value
nargs	RET
* Perform a switch statement
?switch	XCHG			HL=Table address, DE=Switch value
?swit1	MOV	C,M		Get low address
	INX	H		Advance to next
	MOV	B,M		Get high address
	INX	H		Advance to next
	MOV	A,B		Test for zero
	ORA	C		End of table?
	JZ	?swit3		Yes, use default
	MOV	A,M		Get low value
	INX	H		Advance to next
	CMP	E		Does it match?
	JNZ	?swit2		No, skip to next
	MOV	A,M		Get high value
	CMP	D		Does it match?
?swit2	INX	H		Advance to next
	JNZ	?swit1		No try next
** Value found, jump to label
	PUSH	B		Save on stack
	RET			Jump [SP++]
** Value not found, use default
?swit3	MOV	A,M		Get low address
	INX	H		Advance to next
	MOV	H,M		Get high address
	MOV	L,A		Include low address
?idcall	PCHL			Perform jump
* Negate: HL = -HL
?neg	DCX	H		Decrement for negate
* Complement: HL = ~HL
?com	MOV	A,H		Get high
	CMA			Complement
	MOV	H,A		Replace value
	MOV	A,L		Get low
	CMA			Complement
	MOV	L,A		Replace value
	RET
* Logical NOT: HL = !HL
?not	MOV	A,H		Get high
	ORA	L		Test for zero
	LXI	H,0		Assume FALSE
	RNZ			Yes, exit
	INX	H		Convert to TRUE
	RET
* Subtract: HL = BC - HL
?sub	MOV	A,C		Get low source
	SUB	L		Subtract low dest
	MOV	L,A		And replace
	MOV	A,B		Get high source
	SBB	H		Subtract high dest
	MOV	H,A		And replace
	RET
* 16 bit multiply: HL = BC * HL
?mul	PUSH	D		Save index
	XCHG			D = Dest
	LXI	H,0		Begin with zero
?mul1	MOV	A,E		Get low dest
	RRC			Test for odd value
	JNC	?mul2		Not odd
	DAD	B		+ dest
?mul2	ANA	A		Clear carry
	MOV	A,D		Get high dest
	RAR			/ 2
	MOV	D,A		Replace
	MOV	A,E		Get low dest
	RAR			/ 2
	MOV	E,A		Replace
	ORA	D		All done (zero)?
	JZ	?mul3		Yes, exit
	MOV	A,C		Get low source
	RAL			* 2
	MOV	C,A		Replace
	MOV	A,B		Get high source
	RAL			* 2
	MOV	B,A		Replace
	ORA	C		All done (zero)?
	JNZ	?mul1		No, keep going
?mul3	POP	D		Restore index
	RET
* 16 bit division: HL = BC / HL
?sdiv	EQU	*		Entry point for signed division
?div	CALL	?mod		Main calculation
	MOV	H,B		Get high result
	MOV	L,C		Get low result
	RET
* 16 bit modulus: HL = BC % HL, BC = BC / HL
?smod	EQU	*		Entry point for signed modulus
?mod	PUSH	D		Save index
	XCHG			DE = P1
	LXI	H,0		Begin with zero
	MVI	A,17		16+1 bit shift
	STA	?temp		Save for later
?mod1	ANA	A		No carry in
?mod2	MOV	A,C		Get low source
	RAL			Shift
	MOV	C,A		Replace
	MOV	A,B		Get high source
	RAL			16 bit Shift
	MOV	B,A		Replace
	LDA	?temp		Restore count
	DCR	A		Reduce count
	JZ	?mod4		Finished, exit
	STA	?temp		Resave count
	MOV	A,L		Get low dest
	RAL			Shift in bit, result *2
	MOV	L,A		Replace
	MOV	A,H		Get high dest
	RAL			16 bit shift
	MOV	H,A		Replace
	CMP	D		Lower?
	JC	?mod1		Yes, do again
	MOV	A,L		Lower?
	JNZ	?mod3		Higher, don't test low
	CMP	E		Yes, do again
	JC	?mod1		Lower, go round again
?mod3	SUB	E		Subtract p1
	MOV	L,A		Resave
	MOV	A,H		Get high
	SBB	D		16 bit subtract
	MOV	H,A		Resave
	STC			Insure carry set
	JMP	?mod2		and proceed
?mod4	POP	D		Restore index
	RET
* Logical AND: HL = BC & HL
?and	MOV	A,C		Get low source
	ANA	L		And with dest
	MOV	L,A		Replace
	MOV	A,B		Get high source
	ANA	H		And with dest
	MOV	H,A		Replace
	RET
* Logical OR: HL = BC | HL
?or	MOV	A,C		Get low source
	ORA	L		Or with dest
	MOV	L,A		Replace
	MOV	A,B		Get high source
	ORA	H		Or with dest
	MOV	H,A		Replace
	RET
* Exclusive OR: HL = BC ^ HL
?xor	MOV	A,C		Get low source
	XRA	L		Xor with dest
	MOV	L,A		Replace
	MOV	A,B		Get high source
	XRA	H		Xor with dest
	MOV	H,A		Replace
	RET
* Shift right: HL = BC >> HL
?shr	MOV	A,H		Get value
	ORA	L		Test for zero (special case)
	JZ	?shr2		zero, exit
?shr1	ANA	A		Clear carry
	MOV	A,B		Get high
	RAR			Shift it
	MOV	B,A		Replace
	MOV	A,C		Get low
	RAR			Shift it
	MOV	C,A		Replace
	DCR	L		Reduce count
	JNZ	?shr1		Do them all
?shr2	MOV	H,B		Copy it
	MOV	L,C		Over
	RET
* Shift left: HL = BC << HL
?shl	MOV	A,H		Get value
	ORA	L		Test for zero (special case)
	JZ	?shl2		zero, exit
?shl1	ANA	A		Clear carry
	MOV	A,C		Get high
	RAL			Shift it
	MOV	C,A		Replace
	MOV	A,B		Get low
	RAL			Shift it
	MOV	B,A		Replace
	DCR	L		Reduce count
	JNZ	?shl1		Do them all
?shl2	MOV	H,B		Copy it
	MOV	L,C		Over
	RET
* Conditional operators
?eq	CALL	?ucomp		BC == HL
	RZ
	DCX	H
	RET
?ne	CALL	?ucomp		BC != HL
	RNZ
	DCX	H
	RET
?gt	PUSH	H		BC > HL (signed)
	MOV	H,B
	MOV	L,C
	POP	B
?lt	CALL	?scomp		BC < HL (signed)
	RC
	DCX	H
	RET
?le	CALL	?scomp		BC <= HL (signed)
	RC
	RZ
	DCX	H
	RET
?ge	CALL	?scomp		BC >= HL (signed)
	RNC
	DCX	H
	RET
?ugt	PUSH	H		BC > HL (unsigned)
	MOV	H,B
	MOV	L,C
	POP	B
?ult	CALL	?ucomp		BC < HL (unsigned)
	RC
	DCX	H
	RET
?ule	CALL	?ucomp		BC <= HL (unsigned)
	RC
	RZ
	DCX	H
	RET
?uge	CALL	?ucomp		BC >= HL (unsigned)
	RNC
	DCX	H
	RET
* Signed compare of BC & HL
?scomp	MOV	A,B		Get HIGH #1
	XRA	H		Sign same as #2?
	JP	?ucomp		Handle as unsigned
	LXI	H,1		Assume TRUE
	ANA	B		Get sign #1
	JM	?scomp1		Less than #2
	ORA	L		Clear 'C' and 'Z', greater than
	RET
?scomp1	STC			Set 'C', indicate less than
	RET
* Unsigned compare of BC & HL
?ucomp	MOV	A,B		Get high
	CMP	H		Match?
	JNZ	?ucomp1		No, exit
	MOV	A,C		Get low
	CMP	L		Set flags
?ucomp1	LXI	H,1		Assume TRUE
	RET
* Temporary location used by compiler and "?mod"
$DD:?temp 2
