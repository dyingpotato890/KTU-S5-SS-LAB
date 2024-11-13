DATA SEGMENT
    MSG1 DB 10,13,'ENTER THE STRING: $'
    MSG2 DB 10,13,'THE REVERSED STRING IS $'
    STR1 DB 50 DUP(0)
DATA ENDS
CODE SEGMENT
    ASSUME CS:CODE, DS:DATA
    START:
        MOV AX,DATA
        MOV DS,AX

        LEA DX,MSG1
        MOV AH,09H
        INT 21H

        LEA SI,STR1
        LEA DI,STR1

        MOV AH,01H

    DOTHIS:
        INT 21H

        CMP AL,0DH
        JE TERMINATE

        MOV [DI],AL
        INC DI
        JMP DOTHIS
    TERMINATE:
        MOV AL,'$'
        MOV [DI],AL
    REVERSE:
        DEC DI
        MOV AL,[DI]
        XCHG [SI],AL
        MOV [DI],AL
        INC SI
        CMP SI,DI
        JL REVERSE

       LEA DX,MSG2
       MOV AH,09H
       INT 21H

       LEA DX,STR1
       INT 21H

       MOV AH,4CH
       INT 21H
CODE ENDS
END START 

