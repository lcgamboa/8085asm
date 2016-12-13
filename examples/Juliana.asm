;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;Juliana.asm - conversor de datas
;Prof. Roberto M. Ziller - 04.01.2000
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
LETECLA EQU 02E7H
MOSTRAD EQU 0363H

        ORG 2000H

        LXI SP,20C0H

LE1:    CALL LETECLA    ; 1. digito do dia
        CPI 0AH
        JP LE1
        RLC
        RLC
        RLC
        RLC
        MOV E,A
LE2:    CALL LETECLA    ; 2. digito do dia
        CPI 0AH
        JP LE2
        ORA E
        MOV E,A         
        MVI D,00H       ; E contem dia em BCD e D = 0
LE3:    CALL LETECLA    ; 1. digito do mes
        CPI 0AH
        JP LE3
        MVI C,00H
        CPI 00
        JZ LE4
        MVI C,10D
LE4:    CALL LETECLA    ; 2. digito do mes
        CPI 0AH
        JP LE4
        ADD C
        SUI 01H
        MOV C,A         ; C = mes-1 = numero de meses a somar
        CPI 00H
        JZ MOSTRA       ; se mes = janeiro, esta pronto  
        LXI H,DIAS
        MOV A,E         ; A inicia com o dia...
SOMA:   ADD M           ; ... e recebe os dias de cada mes
        DAA
        JNC CONT
        INR D           ; "vai 1"
CONT:   INX H
        DCR C
        JNZ SOMA
        MOV E,A
MOSTRA: CALL MOSTRAD    ; resultado no campo de enderecos
        JMP LE1         ; volta ao passo 1 do enunciado

DIAS    DB 31H,28H,31H,30H,31H,30H,31H,31H,30H,31H,30H,31H
        END

