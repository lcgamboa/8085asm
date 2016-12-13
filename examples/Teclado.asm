;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;Teclado.asm - leitura de numeros do teclado
;Prof. Roberto M. Ziller - 04.01.2000
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
LETECLA EQU 02E7H       ; Sinonimos utilizados abaixo
MOSTRAD EQU 0363H       ;
MOSTRAA EQU 036EH       ;
       
        ORG 2000H
       
        LXI SP,20C0H    ; Inicializa pilha
LOOP:   CALL LETECLA    ; Le primeiro numero
        MOV D,A
        MVI E,0H
        PUSH D
        CALL MOSTRAD    ; Apresenta primeiro numero
        POP D
        CALL LETECLA    ; Le segundo numero
        MOV E,A
        ADD D           ; Resultado em A
        PUSH PSW
        CALL MOSTRAD    ; Apresenta os dois numeros
        POP PSW
        CALL MOSTRAA    ; Apresenta resultado
        JMP LOOP
        END
