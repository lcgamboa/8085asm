;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;Mult.asm - Multiplicacao de numeros de dois digitos
;Prof. Roberto M. Ziller - 04.01.2000
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
LETECLA EQU 02E7H       ; 
MOSTRAD EQU 0363H       ; sinonimos para sub-rotinas em ROM
        ORG 2000H       ; endereco inicial do programa
INICIO: LXI SP,20C0H    ; define inicio da pilha
LOOP0:  CALL LE_DADO    ; leitura de N1 (acumulador)
        MOV B,A         ; copia N1 para o registrador B
        CALL LE_DADO    ; leitura de N2 (acumulador)
        MOV C,A         ; copia N2 para o registrador C
        CALL MULT       ; DE = C * B
        CALL MOSTRAD    ; apresenta resultado
        JMP LOOP0       ; reinicia

LE_DADO:CALL LETECLA    ; le o digito mais significativo
        RLC
        RLC
        RLC
        RLC             ; multiplica-o por 16
        MOV E,A         ; digito mais significativo em E
        CALL LETECLA    ; le digito menos significativo
        ORA E           ; compoe numero de dois digitos
        RET             ; retorna com numero em A
MULT:   MVI D,00        ; inicializacoes
        MVI A,00
LOOP1:  ADD B           ; soma N1 ao acumulador
        JNC CONT        ; "vai 1"? 
        INR D           ; incrementa D se a soma passa de FF
CONT:   DCR C 
        JNZ LOOP1       ; repete a soma N2 vezes
        MOV E,A         ; transfere o resultado para E
        RET             ; retorno com resultado no par DE

        END
