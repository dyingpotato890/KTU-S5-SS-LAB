DATA SEGMENT
    LIST DB 52H, 23H, 5AH, 45H, 23H
    COUNT EQU 05H
    LARGEST DB 01H DUP(?)
DATA ENDS
CODE SEGMENT
    ASSUME CS:CODE, DS:DATA

    START:
        MOV AX,DATA
        MOV DS,AX

        MOV SI, OFFSET LIST
        MOV CL, COUNT
        MOV AL,[SI]
    AGAIN:
        CMP AL,[SI+1]
        JNL NEXT
        MOV AL,[SI+1]
    NEXT:
        INC SI
        DEC CL
        JNZ AGAIN

        MOV SI,OFFSET LARGEST
        MOV [SI],AL


        MOV CL,02H
        MOV BL,AL
    PRINT: 
        ROR BL,1
        ROR BL,1
        ROR BL,1
        ROR BL,1

        MOV DL,BL
        AND DL,0FH
        ADD DL,30H
        CMP DL,'9'
        JLE CONTINUE
        ADD DL,07H
        
    CONTINUE:
        MOV AH,02H
        INT 21H
        DEC CL
        JNZ PRINT

        MOV AH,4CH
        INT 21H
CODE ENDS
END START