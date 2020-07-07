	move.b	0, d0		; d0 hold current palette offset
restart:				; once a whole screen of pixels is filled, we jump back here
	add.l	1, d0		; increment pal offset after a whole screen has been filled
	move.l	$30000, a0	; for now, $30000 is hardcoded screen address (will change)
loop:					; after one pixel us drawn, we jump back here
	move.l	a0, d1		; put current pixel address into d1
	add.l	d0, d1		; add pal offset to d1
	move.b	d1, @a0+	; move lower 8 bits to the address indicated by a0, and increment a0
	cmpu.w	$3fa00, a0  ; check if a0 has reached the end of the screen (320x200 hardcoded size)
	je		restart		; if comparison was equal we have filled screen so jump to label 'restart'
	jmp		loop		; if we have not filled the screen, jump to label 'loop'
