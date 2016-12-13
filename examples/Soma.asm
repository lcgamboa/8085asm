;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;Soma.asm - soma de numeros em hexadecimal
;Prof. Roberto M. Ziller - 04.01.2000
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
        ORG 2000H
        LDA N           ; traz para A o valor de N
        MOV C,A         ; guarda este valor em C
        LXI H,DADOS     ; aponta par HL p/ inicio da tabela
        MVI B,00H       ; inicializa soma
LOOP:   MOV A,M         ; le dado apontado por HL
        ADD B           ; soma acumulada em A
        MOV B,A         ; e guardada em B
        INX H           ; par HL aponta para o dado seguinte
        DCR C           ; um a menos para fazer
        JNZ LOOP        ; repete ate que C = 0
        LXI H,RESULT    ; aponta HL p/ inicio do resultado
        MOV M,B         ; guarda B na memoria
        JMP $           ; loop infinito, fica parado aqui
        ORG 2020H
RESULT  DB 00H,00H
N       DB 03H
DADOS   DB 01H,02H,03H,04H,65H,76H,87H,98H,A9H
        END

