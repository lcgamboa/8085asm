;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;BaseConv.asm - programa de conversao de base
;Recebe a base de entrada (numero decimal de 2 digitos)
;Recebe um numero de 4 digitos nessa base, sem verifica-lo
;Recebe a base de saida (um numero decimal de 2 digitos)
;Ambas as bases devem estar entre 2 e 16, inclusive
;Converte o numero da base de entrada para a base de saida
;Apresenta o resultado no display
;Campos de enderecos e dados formam um numero de 6 digitos
;Prof. Roberto m. Ziller - 04.01.2000
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
LETECLA EQU 02E7H
MOSTRAD EQU 0363H
MOSTRAA EQU 036EH

        ORG 2000H
        LXI SP,2100H
LOOP:   CALL LENUM10 ; BASE DE ENTRADA LIDA (DECIMAL)
        MOV C,A
        CALL LENUM   ; LE NUMERO DE 4 DIGS, CONVERTE P/ HEXA
        PUSH D
        CALL MOSTRAD ; APRESENTA NUMERO A CONVERTER EM HEXA
        POP D   
        CALL LENUM10 ; BASE DE SAIDA
        MOV C,A
        CALL HEXCONV ; CONVERTE. RESULTADO EM D, E e A
        PUSH PSW
        CALL MOSTRAD
        POP PSW
        CALL MOSTRAA
        JMP LOOP

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;HEXCONV - converte numero hexa para a base de saida
;Recebe numero no par DE e base de saida em C
;Retorna resultado como numero de 6 digitos em D, E e A
;Utiliza todos os regs
;Chama a sub-rotina DIV
HEXCONV:
        MVI B,00H       ; CONTADOR DE DIGITOS DO RESULTADO
MORE:   CALL DIV        ; QUOCIENTE EM DE E RESTO EM L
        PUSH H          ; L CONTEM DIGITO DO RESULTADO 
        INR B           ; CONTA DIGITOS
        MOV A,D         
        CPI 00H         ; SE D <> 0, TEM MAIS
        JNZ MORE 
        MOV A,E
        CPI 00H         ; SE D == 0 MAS E <> 0, TEM MAIS
        JNZ MORE
        MVI A,00H       ; TRIPLA DEA CONTEM 0 PARA COMECAR
GETDIG: MVI H,04H       ; RESGATE DOS DIGITOS DA PILHA
ROTATE: ORA A           ; ZERA CARRY FLAG
        RAL             ; BIT MAIS SIGNIFICATIVO DE A EM CY
        MOV L,A
        MOV A,E         ; ROTACIONA DIGITOS DA TRIPLA DEA
        RAL             ; BIT MAIS SIGNIFICATIVO DE A EM E
        MOV E,A
        MOV A,D
        RAL             ; BIT MAIS SIGNIFICATIVO DE E EM D
        MOV D,A
        MOV A,L
        DCR H
        JNZ ROTATE
        POP H           ; L CONTEM DIGITO PARA A TRIPLA DEA
        ORA L           ; INTRODUZ DIGITO RESGATADO DA PILHA
        DCR B           ; TEM MAIS DIGITOS?
        JNZ GETDIG      
        RET             ; NUMERO CONVERTIDO EM DEA
        
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;DIV - sub-rotina para divisao
;Recebe dividendo em DE e divisor em C
;Retorna quociente em DE e resto em L
;Utiliza regs A, C, D, E, H e L
DIV:    LXI H,0000H
        MOV A,E
TEST1:  CMP C
        JC TEST2
BACK:   SUB C
        INX H
        JMP TEST1
TEST2:  MOV E,A
        MOV A,D
        CPI 00H
        JZ DONE
        DCR D
        MOV A,E
        JMP BACK
DONE:   XCHG
        RET

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;MULT - sub-rotina para multiplicacao
;Recebe multiplicando em DE e multiplicador em C
;Retorna resultado em DE, preservando o multiplicador em C
;Utiliza regs A, C, D, E, H e L
MULT:   XCHG
        LXI D,0000H
        MOV A,C
        CPI 00H
        RZ
        PUSH B
MUL:    MOV A,E
        ADD L
        MOV E,A
        MOV A,D
        ADC H
        MOV D,A
        DCR C
        JNZ MUL
        POP B
        RET

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;LENUM
;Le numero de 4 digitos na base contida em C 
;Converte p/ hexa e retorna esse valor no par DE
;Utiliza todos os regs
;Chama MULT
LENUM:  LXI D,0000H
        MVI B,04H
MULBAS: CALL MULT      ; MULTIPLICA PAR DE PELA BASE EM C
        CALL LETECLA
        ADD E
        MOV E,A
        JNC CONT
        INR D
CONT:   DCR B
        JNZ MULBAS
        RET

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;LENUM10 - le um numero decimal de dois digitos
;Retorna valor hexa do numero lido no acumulador
;Utiliza os regs A, B, H e L
;Chama LETECLA
LENUM10:
        CALL LETECLA
        RLC
        MOV B,A
        RLC
        RLC
        ADD B
        MOV B,A
        CALL LETECLA
        ADD B
        RET
        END
