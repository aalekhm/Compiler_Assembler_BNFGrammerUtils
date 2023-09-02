
; EDI, ESI, ESP, EBP, EAX, EBX, ECX, EDX, R8, R9, R10, R11, R12, R13, R14, R15
; RIP, CS, DS, FS, SS, ES, GS, CF, ZF, PF, AF, SF, TF, IF, DF, OF

section   .text
_start:

	HLT
	
	CALL 	555										; CALL 	<const32>
	CALL 	LABEL_0			
	
	RET	
		
	; Variants of 'NOT'	
	NOT		EAX										; NOT 	<reg>
				
	NOT 	[ EBX ]									; NOT	<mem>
	NOT 	[ EBX * 1 ]
	NOT 	[ EBX - 16 ]
	NOT 	[ EBX + 32 ]
	NOT 	[ EBX + ECX ]
	NOT 	[ EBX + ECX * 1 ]
	NOT 	[ EBX + ECX * 2 + 5 ]
	NOT 	[ EBX + ECX * 4 - 15 ]
				
	; Variants of 'NEG'			
	NEG		EAX										; NEG 	<reg>
				
	NEG 	[ EBX ]									; NEG	<mem>
	NEG 	[ EBX * 1 ]
	NEG 	[ EBX - 16 ]
	NEG 	[ EBX + 32 ]
	NEG 	[ EBX + ECX ]
	NEG 	[ EBX + ECX * 1 ]
	NEG 	[ EBX + ECX * 2 + 5 ]
	NEG 	[ EBX + ECX * 4 - 15 ]
			
	; Variants of 'MOV'		
	MOV 	EAX, EBX								; MOV 	<reg>, <reg>
			
	MOV 	EAX, [ EBX ]							; MOV 	<reg>, <mem>
	MOV 	EAX, [ EBX * 1 ]
	MOV 	EAX, [ EBX - 16 ]
	MOV 	EAX, [ EBX + 32 ]
	MOV 	EAX, [ EBX + ECX ]
	MOV 	EAX, [ EBX + ECX * 1 ]
	MOV 	EAX, [ EBX + ECX * 2 + 5 ]
	MOV 	EAX, [ EBX + ECX * 4 - 15 ]

	MOV 	EAX, 555								; MOV 	<reg>, <const32>
	MOV 	EAX, LABEL_0		
					
					
	MOV 	[ EBX ], EAX							; MOV 	<mem>, <reg>
	MOV 	[ EBX * 1 ], EAX
	MOV 	[ EBX - 16 ], EAX
	MOV 	[ EBX + 32 ], EAX
	MOV 	[ EBX + ECX ], EAX
	MOV 	[ EBX + ECX * 1 ], EAX
	MOV 	[ EBX + ECX * 2 + 5 ], EAX
	MOV 	[ EBX + ECX * 4 - 15 ], EAX
		
	MOV 	[ EBX ], 555							; MOV 	<mem>, <const32>
	MOV 	[ EBX * 1 ], 555	
	MOV 	[ EBX - 16 ], 555	
	MOV 	[ EBX + 32 ], 555	
	MOV 	[ EBX + ECX ], 555	
	MOV 	[ EBX + ECX * 1 ], 555	
	MOV 	[ EBX + ECX * 2 + 5 ], LABEL_0
	MOV 	[ EBX + ECX * 4 - 15 ], LABEL_0

	; Variants of 'PUSH'	
	PUSH	EAX										; PUSH 	<reg>
			
	PUSH 	[ EBX ]									; PUSH 	<mem>
	PUSH 	[ EBX * 1 ]
	PUSH 	[ EBX - 16 ]
	PUSH 	[ EBX + 32 ]
	PUSH 	[ EBX + ECX ]
	PUSH 	[ EBX + ECX * 1 ]
	PUSH 	[ EBX + ECX * 2 + 5 ]
	PUSH 	[ EBX + ECX * 4 - 15 ]

	PUSH 	LABEL_0									; PUSH 	<const32>
	PUSH 	555		
			
	; Variants of 'POP'		
	POP		EAX										; POP 	<reg>
			
	POP 	[ EBX ]									; POP 	<mem>
	POP 	[ EBX * 1 ]
	POP 	[ EBX - 16 ]
	POP 	[ EBX + 32 ]
	POP 	[ EBX + ECX ]
	POP 	[ EBX + ECX * 1 ]
	POP 	[ EBX + ECX * 2 + 5 ]
	POP 	[ EBX + ECX * 4 - 15 ]
	
	; Variants of 'LEA'	
	LEA 	EAX, [ EBX ]							; LEA 	<reg>, <mem>
	LEA 	EAX, [ EBX * 1 ]
	LEA 	EAX, [ EBX - 16 ]
	LEA 	EAX, [ EBX + 32 ]
	LEA 	EAX, [ EBX + ECX ]
	LEA 	EAX, [ EBX + ECX * 1 ]
	LEA 	EAX, [ EBX + ECX * 2 + 5 ]
	LEA 	EAX, [ EBX + ECX * 4 - 15 ]

	; Variants of 'ADD'	
	ADD 	EAX, EBX								; ADD 	<reg>, <reg>
	
	ADD 	EAX, [ EBX ]							; ADD 	<reg>, <mem>
	ADD 	EAX, [ EBX * 1 ]
	ADD 	EAX, [ EBX - 16 ]
	ADD 	EAX, [ EBX + 32 ]
	ADD 	EAX, [ EBX + ECX ]
	ADD 	EAX, [ EBX + ECX * 1 ]
	ADD 	EAX, [ EBX + ECX * 2 + 5 ]
	ADD 	EAX, [ EBX + ECX * 4 - 15 ]

	ADD 	EAX, 555								; ADD 	<reg>, <const32>
	ADD 	EAX, LABEL_0	
				
	ADD 	[ EBX ], EAX							; ADD 	<mem>, <reg>
	ADD 	[ EBX * 1 ], EAX
	ADD 	[ EBX - 16 ], EAX
	ADD 	[ EBX + 32 ], EAX
	ADD 	[ EBX + ECX ], EAX
	ADD 	[ EBX + ECX * 1 ], EAX
	ADD 	[ EBX + ECX * 2 + 5 ], EAX
	ADD 	[ EBX + ECX * 4 - 15 ], EAX

	ADD 	[ EBX ], 555							; ADD 	<mem>, <const32>
	ADD 	[ EBX * 1 ], 555
	ADD 	[ EBX - 16 ], 555
	ADD 	[ EBX + 32 ], 555
	ADD 	[ EBX + ECX ], 555
	ADD 	[ EBX + ECX * 1 ], 555
	ADD 	[ EBX + ECX * 2 + 5 ], 555
	ADD 	[ EBX + ECX * 4 - 15 ], 555

	; Variants of 'SUB'	
	SUB 	EAX, EBX								; SUB 	<reg>, <reg>
	
	SUB 	EAX, [ EBX ]							; SUB 	<reg>, <mem>
	SUB 	EAX, [ EBX * 1 ]
	SUB 	EAX, [ EBX - 16 ]
	SUB 	EAX, [ EBX + 32 ]
	SUB 	EAX, [ EBX + ECX ]
	SUB 	EAX, [ EBX + ECX * 1 ]
	SUB 	EAX, [ EBX + ECX * 2 + 5 ]
	SUB 	EAX, [ EBX + ECX * 4 - 15 ]

	SUB 	EAX, 555								; SUB 	<reg>, <const32>
	SUB 	EAX, LABEL_0	

	SUB 	[ EBX ], EAX							; SUB 	<mem>, <reg>
	SUB 	[ EBX * 1 ], EAX
	SUB 	[ EBX - 16 ], EAX
	SUB 	[ EBX + 32 ], EAX
	SUB 	[ EBX + ECX ], EAX
	SUB 	[ EBX + ECX * 1 ], EAX
	SUB 	[ EBX + ECX * 2 + 5 ], EAX
	SUB 	[ EBX + ECX * 4 - 15 ], EAX
	
	SUB 	[ EBX ], 555							; SUB 	<mem>, <const32>
	SUB 	[ EBX * 1 ], 555
	SUB 	[ EBX - 16 ], 555
	SUB 	[ EBX + 32 ], 555
	SUB 	[ EBX + ECX ], 555
	SUB 	[ EBX + ECX * 1 ], 555
	SUB 	[ EBX + ECX * 2 + 5 ], LABEL_0
	SUB 	[ EBX + ECX * 4 - 15 ], LABEL_0

	; Variants of 'INC'	
	INC 	EAX										; INC 	<reg>
		
	INC 	[ EBX ]									; INC 	<mem>
	INC 	[ EBX * 1 ]
	INC 	[ EBX - 16 ]
	INC 	[ EBX + 32 ]
	INC 	[ EBX + ECX ]
	INC 	[ EBX + ECX * 1 ]
	INC 	[ EBX + ECX * 2 + 5 ]
	INC 	[ EBX + ECX * 4 - 15 ]

	; Variants of 'DEC'		
	DEC 	EAX										; DEC 	<reg>
		
	DEC 	[ EBX ]									; DEC 	<mem>
	DEC 	[ EBX * 1 ]
	DEC 	[ EBX - 16 ]
	DEC 	[ EBX + 32 ]
	DEC 	[ EBX + ECX ]
	DEC 	[ EBX + ECX * 1 ]
	DEC 	[ EBX + ECX * 2 + 5 ]
	DEC 	[ EBX + ECX * 4 - 15 ]
		
	; Variants of 'IMUL'	
	IMUL 	EAX, EBX								; IMUL 	<reg>, <reg>
	
	IMUL 	EAX, [ EBX ]							; IMUL 	<reg>, <mem>
	IMUL 	EAX, [ EBX * 1 ]
	IMUL 	EAX, [ EBX - 16 ]
	IMUL 	EAX, [ EBX + 32 ]
	IMUL 	EAX, [ EBX + ECX ]
	IMUL 	EAX, [ EBX + ECX * 1 ]
	IMUL 	EAX, [ EBX + ECX * 2 + 5 ]
	IMUL 	EAX, [ EBX + ECX * 4 - 15 ]

	IMUL 	EBX, 555								; IMUL 	<reg>, <const32>
	IMUL 	EBX, LABEL_0	

	IMUL 	EAX, EAX, 555							; IMUL 	<reg>, <reg>, <const32>
	IMUL 	EAX, EAX, LABEL_0
	
	IMUL 	EAX, [ EBX ], 555						; IMUL 	<reg>, <mem>, <const32>
	IMUL 	EAX, [ EBX * 1 ], 555
	IMUL 	EAX, [ EBX - 16 ], 555
	IMUL 	EAX, [ EBX + 32 ], 555
	IMUL 	EAX, [ EBX + ECX ], 555
	IMUL 	EAX, [ EBX + ECX * 1 ], 555
	IMUL 	EAX, [ EBX + ECX * 2 + 5 ], LABEL_0
	IMUL 	EAX, [ EBX + ECX * 4 - 15 ], LABEL_0

	IMUL 	EAX, 555, 111							; IMUL 	<reg>, <const32>, <const32>
	
	; Variants of 'IDIV'
	IDIV 	EAX										; IDIV 	<reg>
	
	IDIV 	[ EBX ]									; IDIV 	<mem>
	IDIV 	[ EBX * 1 ]
	IDIV 	[ EBX - 16 ]
	IDIV 	[ EBX + 32 ]
	IDIV 	[ EBX + ECX ]
	IDIV 	[ EBX + ECX * 1 ]
	IDIV 	[ EBX + ECX * 2 + 5 ]
	IDIV 	[ EBX + ECX * 4 - 15 ]

	; Variants of 'AND'		
	AND 	EAX, EBX								; ADD 	<reg>, <reg>
	
	AND 	EAX, [ EBX ]							; ADD 	<reg>, <mem>
	AND 	EAX, [ EBX * 1 ]
	AND 	EAX, [ EBX - 16 ]
	AND 	EAX, [ EBX + 32 ]
	AND 	EAX, [ EBX + ECX ]
	AND 	EAX, [ EBX + ECX * 1 ]
	AND 	EAX, [ EBX + ECX * 2 + 5 ]
	AND 	EAX, [ EBX + ECX * 4 - 15 ]

	AND 	EAX, 555								; ADD 	<reg>, <const32>
	AND 	EAX, LABEL_0		

	AND 	[ EBX ], EAX							; ADD 	<mem>, <reg>
	AND 	[ EBX * 1 ], EAX
	AND 	[ EBX - 16 ], EAX
	AND 	[ EBX + 32 ], EAX
	AND 	[ EBX + ECX ], EAX
	AND 	[ EBX + ECX * 1 ], EAX
	AND 	[ EBX + ECX * 2 + 5 ], EAX
	AND 	[ EBX + ECX * 4 - 15 ], EAX

	AND 	[ EBX ], 555							; ADD 	<mem>, <const32>
	AND 	[ EBX * 1 ], 555
	AND 	[ EBX - 16 ], 555
	AND 	[ EBX + 32 ], 555
	AND 	[ EBX + ECX ], 555
	AND 	[ EBX + ECX * 1 ], 555
	AND 	[ EBX + ECX * 2 + 5 ], LABEL_0
	AND 	[ EBX + ECX * 4 - 15 ], LABEL_0

	; Variants of 'OR'		
	OR 		EAX, EBX								; OR 	<reg>, <reg>
		
	OR 		EAX, [ EBX ]							; OR 	<reg>, <mem>
	OR 		EAX, [ EBX * 1 ]
	OR 		EAX, [ EBX - 16 ]
	OR 		EAX, [ EBX + 32 ]
	OR 		EAX, [ EBX + ECX ]
	OR 		EAX, [ EBX + ECX * 1 ]
	OR 		EAX, [ EBX + ECX * 2 + 5 ]
	OR 		EAX, [ EBX + ECX * 4 - 15 ]

	OR 		EAX, 555								; OR 	<reg>, <const32>
	OR 		EAX, LABEL_0			
	
	OR 		[ EBX ], EAX							; OR 	<mem>, <reg>
	OR 		[ EBX * 1 ], EAX
	OR 		[ EBX - 16 ], EAX
	OR 		[ EBX + 32 ], EAX
	OR 		[ EBX + ECX ], EAX
	OR 		[ EBX + ECX * 1 ], EAX
	OR 		[ EBX + ECX * 2 + 5 ], EAX
	OR 		[ EBX + ECX * 4 - 15 ], EAX

	OR 		[ EBX ], 555							; OR 	<mem>, <const32>
	OR 		[ EBX * 1 ], 555
	OR 		[ EBX - 16 ], 555
	OR 		[ EBX + 32 ], 555
	OR 		[ EBX + ECX ], 555
	OR 		[ EBX + ECX * 1 ], 555
	OR 		[ EBX + ECX * 2 + 5 ], LABEL_0
	OR 		[ EBX + ECX * 4 - 15 ], LABEL_0

	; Variants of 'XOR'		
	XOR 	EAX, EBX								; XOR 	<reg>, <reg>

	XOR 	EAX, [ EBX ]							; XOR 	<reg>, <mem>
	XOR 	EAX, [ EBX * 1 ]
	XOR 	EAX, [ EBX - 16 ]
	XOR 	EAX, [ EBX + 32 ]
	XOR 	EAX, [ EBX + ECX ]
	XOR 	EAX, [ EBX + ECX * 1 ]
	XOR 	EAX, [ EBX + ECX * 2 + 5 ]
	XOR 	EAX, [ EBX + ECX * 4 - 15 ]

	XOR 	EAX, 555								; XOR 	<reg>, <const32>
	XOR 	EAX, LABEL_0		

	XOR 	[ EBX ], EAX							; XOR 	<mem>, <reg>
	XOR 	[ EBX * 1 ], EAX
	XOR 	[ EBX - 16 ], EAX
	XOR 	[ EBX + 32 ], EAX
	XOR 	[ EBX + ECX ], EAX
	XOR 	[ EBX + ECX * 1 ], EAX
	XOR 	[ EBX + ECX * 2 + 5 ], EAX
	XOR 	[ EBX + ECX * 4 - 15 ], EAX

	XOR 	[ EBX ], 555							; XOR 	<mem>, <const32>
	XOR 	[ EBX * 1 ], 555
	XOR 	[ EBX - 16 ], 555
	XOR 	[ EBX + 32 ], 555
	XOR 	[ EBX + ECX ], 555
	XOR 	[ EBX + ECX * 1 ], 555
	XOR 	[ EBX + ECX * 2 + 5 ], LABEL_0
	XOR 	[ EBX + ECX * 4 - 15 ], LABEL_0

	; Variants of 'SHL'
	SHL 	EAX, EBX								; SHL 	<reg>, <reg>
	
	SHL 	EAX, 555								; SHL 	<reg>, <const32>
	SHL 	EAX, LABEL_0		

	SHL 	[ EBX ], EAX							; SHL 	<mem>, <reg>
	SHL 	[ EBX * 1 ], EAX
	SHL 	[ EBX - 16 ], EAX
	SHL 	[ EBX + 32 ], EAX
	SHL 	[ EBX + ECX ], EAX
	SHL 	[ EBX + ECX * 1 ], EAX
	SHL 	[ EBX + ECX * 2 + 5 ], EAX
	SHL 	[ EBX + ECX * 4 - 15 ], EAX

	SHL 	[ EBX ], 555							; SHL 	<mem>, <const32>
	SHL 	[ EBX * 1 ], 555
	SHL 	[ EBX - 16 ], 555
	SHL 	[ EBX + 32 ], 555
	SHL 	[ EBX + ECX ], 555
	SHL 	[ EBX + ECX * 1 ], 555
	SHL 	[ EBX + ECX * 2 + 5 ], LABEL_0
	SHL 	[ EBX + ECX * 4 - 15 ], LABEL_0

	; Variants of 'SHR'
	SHR 	EAX, EBX								; SHR 	<reg>, <reg>
	
	SHR 	EAX, 555								; SHR 	<reg>, <const32>
	SHR 	EAX, LABEL_0		

	SHR 	[ EBX ], EAX							; SHR 	<mem>, <reg>
	SHR 	[ EBX * 1 ], EAX
	SHR 	[ EBX - 16 ], EAX
	SHR 	[ EBX + 32 ], EAX
	SHR 	[ EBX + ECX ], EAX
	SHR 	[ EBX + ECX * 1 ], EAX
	SHR 	[ EBX + ECX * 2 + 5 ], EAX
	SHR 	[ EBX + ECX * 4 - 15 ], EAX

	SHR 	[ EBX ], 555							; SHR 	<mem>, <const32>
	SHR 	[ EBX * 1 ], 555
	SHR 	[ EBX - 16 ], 555
	SHR 	[ EBX + 32 ], 555
	SHR 	[ EBX + ECX ], 555
	SHR 	[ EBX + ECX * 1 ], 555
	SHR 	[ EBX + ECX * 2 + 5 ], 555
	SHR 	[ EBX + ECX * 4 - 15 ], 555

	; Variants of 'JMP'
	JMP		555
	JMP		LABEL_0
	
	JE		555
	JE		LABEL_0
	
	JNE		555
	JNE		LABEL_0
	
	JZ		555
	JZ		LABEL_0
	
	JG		555
	JG		LABEL_0
	
	JGE		555
	JGE		LABEL_0
	
	JL		555
	JL		LABEL_0
	
	JLE		555
	JLE		LABEL_0
	
	; Variants of 'CMP'
	CMP		EAX, EBX								; CMP 	<reg>, <reg>
	
	CMP 	EAX, [ EBX ]							; CMP 	<reg>, <mem>
	CMP 	EAX, [ EBX * 1 ]
	CMP 	EAX, [ EBX - 16 ]
	CMP 	EAX, [ EBX + 32 ]
	CMP 	EAX, [ EBX + ECX ]
	CMP 	EAX, [ EBX + ECX * 1 ]
	CMP 	EAX, [ EBX + ECX * 2 + 5 ]
	CMP 	EAX, [ EBX + ECX * 4 - 15 ]

	CMP 	EAX, 555								; CMP 	<reg>, <const32>
	CMP 	EAX, LABEL_0	

	CMP 	[ EBX ], EAX							; CMP 	<mem>, <reg>
	CMP 	[ EBX * 1 ], EAX
	CMP 	[ EBX - 16 ], EAX
	CMP 	[ EBX + 32 ], EAX
	CMP 	[ EBX + ECX ], EAX
	CMP 	[ EBX + ECX * 1 ], EAX
	CMP 	[ EBX + ECX * 2 + 5 ], EAX
	CMP 	[ EBX + ECX * 4 - 15 ], EAX

	CMP 	[ EBX ], 555							; CMP 	<mem>, <const32>
	CMP 	[ EBX * 1 ], LABEL_0	
	CMP 	[ EBX - 16 ], 123
	CMP 	[ EBX + 32 ], 456
	CMP 	[ EBX + ECX ], 786
	CMP 	[ EBX + ECX * 1 ], 786
	CMP 	[ EBX + ECX * 2 + 5 ], 786
	CMP 	[ EBX + ECX * 4 - 15 ], 786

section   .data
; LABEL_0:  db        "Hello, World", 10      ; note the newline at the end
