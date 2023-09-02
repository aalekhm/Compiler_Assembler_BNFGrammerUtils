
; C
; int func(int i, int j)
; {
; 	int k = 10;
; 	int l = k * 2 + i - j;
; 	
; 	int m = 8;
;     int n = 4;
;     int o = 2;
; 
; 	int result = (i - (j * k + l) * (m - n / o)); // i j k * l + m n o / - * -
; 	
; 	return result;
; }
; 
; void main()
; {
; 	int result = func(10, 11);
; }

section   .text
_start:
									; Assembly
									; int func(int, int)
MOV		EBP, ESP					; Mark 'ESP' as our current STACK frame 'EBP'
SUB		ESP, 6						; decrement 'ESP' by num of local function variables(6 => k, l, result, m, n, o)

									; func() implementation starts
; int k = 10;
MOV		[EBP - 1], 10				; save '10' on the 1st local variable pos on the STACK

; int l = k * 2 + i - j;
MOV		R0, [EBP - 1]				; save 'k' in R0
MOV		R1, 2						; save '2' in R1
IMUL	R0, R1						; multiply R0 = k * 2
MOV		R1, [EBP]					; save 'i' in R1
ADD		R0, R1						; add R0 = R0 + j
MOV		R1, [EBP + 1]				; save 'j' in R1
SUB		R0, R1						; subtract R0 = R0 - j
MOV		[EBP - 2], R0				; save result into the 2nd local variable pos on to the stack for 'l'

; int m = 8;
MOV		[EBP - 3], 8				; save '8' on the 3rd local variable pos on the STACK

; int n = 4;
MOV		[EBP - 4], 4				; save '4' on the 4th local variable pos on the STACK

; int o = 2;
MOV		[EBP - 5], 2				; save '2' on the 5th local variable pos on the STACK

									; result = (i - (j * k + l) * (m - n / o)); // i j k * l + m n o / - * -
MOV 	R0, [EBP + 1]				; R0 = j
IMUL	R0, [EBP - 1]				; R0 = j * k
ADD		R0, [EBP - 2]				; R0 = (j * k) + l
MOV		EAX, [EBP - 4]				; EAX = n
IDIV	[EBP - 5]					; EAX = n / o
MOV		R2, [EBP - 3]				; R2 = m
SUB		R2, EAX						; R2 = m - (n / o) ==> R2 = R2 - EAX
IMUL	R0, R2						; (j * k + l) * (m - n / o) ==> R0 * R2
MOV		R1, [EBP]					; R1 = i
SUB		R1, R0						; i - (j * k + l) * (m - n / o) ==> R1 - R0
MOV		[EBP - 6], R1				;

MOV		EAX, [EBP - 6]				; move return value 'result' in 'EAX'
; <------ EPILOGUE ------>
MOV		ESP, EBP					; move 'ESP' STACK pointer to 'EBP'
ADD		ESP, 2						; move 'ESP' by amount equal to the num of function arguments(2 => i, j)
POP		ESP							; reclaim last STACK Frame's 'ESP'
POP		EBP							; reclaim last STACK Frame's 'EBP'
RET									; should do something  similar to 'POP EIP'.
; </------ EPILOGUE ------>
											; main()
											; CALL	#func
; <------ PROLOGUE ------>
PUSH	37							; push return address onto the STACK
PUSH	EBP							; save last STACK Frame's Base Pointer 'EBP'
PUSH	ESP							; save last STACK Frame's Stack Pointer 'ESP'
PUSH	10							; 1st param for func()
PUSH 	11							; 2nd param for func()
; </------ PROLOGUE ------>
JMP		0							; jump to func() implementation
HLT									; halt the program
