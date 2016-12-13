;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;Ints85a.asm - Interrupcoes do 8085
;Prof. Roberto M. Ziller - 04.01.2000
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
MOSTRAA EQU 036EH
DELAY   EQU 05F1H
RST7.5  EQU 20CEH
        ORG 2000H
        LXI SP,20C0H   ; Inicializa pilha
        MVI A,18H      ; Habilita 5.5, 6.5 e 7.5
        SIM
        MVI A,00H
        EI
LOOP:   ADI 01H        ; INR A nao serve, nao atualiza CY
        DAA            ; utiliza CY flag na correcao
        PUSH PSW       ; Salva contagem
        CALL MOSTRAA
        MVI D,05H      ; Atraso de 500ms
        CALL DELAY
        POP PSW        ; Recupera contagem
        JMP LOOP
        ORG RST7.5     ; Desvio da RST 7.5
        HLT
        RET
        END

