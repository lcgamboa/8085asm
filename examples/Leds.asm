;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;Leds.asm - Utilizacao de perifericos
;Prof. Roberto M. Ziller - 04.01.2000
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
COMMAND EQU 20H
LEDS    EQU 22H
DELAY   EQU 05F1H
        ORG 2000H
        LXI SP,20C0H    ; Inicializa pilha
        MVI A,02H       ; Porta B como saida
        OUT COMMAND
        MVI A,01H
LOOP:   OUT LEDS        ; Acende o LED correspondente a A
        RLC             ; Rotaciona LED a acender
        PUSH PSW
        MVI D,02H       ; Atraso de 200ms
        CALL DELAY      ; Espera para ver LED aceso
        POP PSW
        JMP LOOP
        RET
        END 
