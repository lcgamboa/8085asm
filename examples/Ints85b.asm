;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;Ints85b.asm - Interrupcoes do 8085
;Prof. Roberto M. Ziller - 04.01.2000
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
MOSTRAA EQU 036EH
DELAY   EQU 05F1H
RST7.5  EQU 20CEH
        ORG 2000H
        LXI SP,20C0H    ; Inicializa pilha
        MVI A,18H
        SIM             ; Habilita 5.5, 6.5 e 7.5
        MVI A,00H 
        STA STATE       ; Flag de reset da contagem
        STA COUNT       ; Inicializa contagem
        EI
LOOP:   LDA COUNT
        ADI 01H         ; INR A não serve, não atualiza CY
        DAA             ; Utiliza CY na correção
        STA COUNT
        CALL MOSTRAA
        MVI D,05H       ; Atraso de 500ms
        CALL DELAY
        LDA STATE
        CPI 00H         ; Testa critério de reinicialização
        JZ LOOP         ; Continua contando se STATE = 0
        MVI A,00H
        STA COUNT       ; Caso contrário, zera a contagem
        STA STATE       ; Reinicializa flag
        JMP LOOP
HNDLR:  PUSH PSW
        MVI A,01H
        STA STATE
        EI
        POP PSW
        RET
STATE   DB 00H          ; Flag global alterado pelo tratador
COUNT   DB 00H          ; Contagem
        ORG RST7.5      ; Desvio da RST 7.5
        JMP HNDLR
        END
