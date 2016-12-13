;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;Chaves.asm - Utilizacao de perifericos
;Prof. Roberto M. Ziller - 04.01.2000
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
COMMAND EQU 20H
CHAVES  EQU 21H
LEDS    EQU 22H
        ORG 2000H
        MVI A,02H     ; Porta A como entrada, B como saida
        OUT COMMAND
LOOP:   IN CHAVES     ; Le chaves para o acumulador
        OUT LEDS      ; Acende os LEDs correspondentes
        JMP LOOP
        RET
        END
