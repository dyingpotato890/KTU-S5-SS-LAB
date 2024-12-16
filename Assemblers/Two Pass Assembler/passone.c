#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *INPUT, *SYMTAB, *OPTAB, *INTR, *LENGTH;

int searchTable(FILE* file, char check[]){
    char label[10];
    int value;
    rewind(file);

    while (fscanf(file, "%s %x", label, &value) != EOF){
        if (strcmp(check, label) == 0){
            return 1;
        }
    }
    return 0;
}

void main(){
    INPUT = fopen("input.txt", "r");
    SYMTAB = fopen("symtab.txt", "a+");
    OPTAB = fopen("optab.txt", "r");
    INTR = fopen("intermediate.txt", "w");
    LENGTH = fopen("length.txt", "w");

    char label[10], opcode[10], operand[10];
    int startAddress, LOCCTR = 0;

    fscanf(INPUT, "%s %s %x", label, opcode, &startAddress);

    if(strcmp(opcode, "START") == 0){
        LOCCTR = startAddress;
        fprintf(INTR, "%x\t%s\t%s\t%x\n", LOCCTR, label, opcode, startAddress);
        fscanf(INPUT, "%s %s %s", label, opcode, operand);
    }

    while(strcmp(opcode, "END") != 0){
        if (strcmp(label, "") != 0){
            if(strcmp(label, "**") != 0 && searchTable(SYMTAB, label) == 0){
                fprintf(SYMTAB, "%s %04x\n", label, LOCCTR);
            }
            else if(searchTable(SYMTAB, label) ==  1){
                printf("\nDuplicate Label Entry");
            }
        }

        if(searchTable(OPTAB, opcode) == 1){
            LOCCTR += 3;
        }
        else if(strcmp(opcode, "WORD") == 0){
            LOCCTR += 3;
        }
        else if(strcmp(opcode, "RESW") == 0){
            LOCCTR += 3 * atoi(operand);
        }
        else if(strcmp(opcode, "BYTE") == 0){
            LOCCTR += strlen(operand);
        }
        else if(strcmp(opcode, "RESB") == 0){
            LOCCTR += atoi(operand);
        }
        else{
            printf("\nInvalid Opcode!\n");
        }
        fprintf(INTR, "%04x\t%s\t%s\t%s\n", LOCCTR, label, opcode, operand);
        fscanf(INPUT, "%s %s %s", label, opcode, operand);
    }
    fprintf(INTR, "%04x\t%s\t%s\t%s\n", LOCCTR - startAddress, label, opcode, operand);
    fprintf(LENGTH, "%x", LOCCTR);

    fclose(INPUT);
    fclose(SYMTAB);
    fclose(OPTAB);
    fclose(INTR);
    fclose(LENGTH);

    puts("Pass 1 Complete!");
}