;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;Leds.asm - Utilizacao de perifericos
;Prof. Roberto M. Ziller - 04.01.2000
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
COMMAND EQU 20H
LEDS    EQU 22H
DELAY   EQU 05F1H
        ORG 2000H
	LD SP, 20C0H                 ;        LXI SP,20C0H    ; Inicializa pilha
	LD A, 02H                    ;        MVI A,02H       ; Porta B como saida
	OUT (COMMAND),A              ;        OUT COMMAND
	LD A, 01H                    ;        MVI A,01H
LOOP:	OUT (LEDS),A            ;LOOP:   OUT LEDS        ; Acende o LED correspondente a A
	RLCA                         ;        RLC             ; Rotaciona LED a acender
	PUSH AF                      ;        PUSH PSW
	LD D, 02H                    ;        MVI D,02H       ; Atraso de 200ms
	CALL DELAY                   ;        CALL DELAY      ; Espera para ver LED aceso
	POP AF                       ;        POP PSW
	JP LOOP                      ;        JMP LOOP
	RET                          ;        RET
        END 
