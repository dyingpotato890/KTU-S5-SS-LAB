#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *input, *symtab, *optab, *intr, *length;

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
    input = fopen("input.txt", "r");
    symtab = fopen("symtab.txt", "a+");
    optab = fopen("optab.txt", "r");
    intr = fopen("intermediate.txt", "w");
    length = fopen("length.txt", "w");

    char label[10], opcode[10], operand[10];
    int startAddress, locctr = 0;

    fscanf(input, "%s %s %x", label, opcode, &startAddress);

    if(strcmp(opcode, "START") == 0){
        fprintf(intr, "%s\t%s\t%x\n", label, opcode, startAddress);
        locctr = startAddress;
        fscanf(input, "%s %s %s", label, opcode, operand);
    }

    while(strcmp(opcode, "END") != 0){
        if (strcmp(label, "") != 0){
            if(strcmp(label, "**") != 0 && searchTable(symtab, label) == 0){
                fprintf(symtab, "%s %04x\n", label, locctr);
            }
            else if(searchTable(symtab, label) ==  1){
                printf("\nDuplicate Label Entry");
            }
        }

        if(searchTable(optab, opcode) == 1){
            locctr += 3;
        }
        else if(strcmp(opcode, "WORD") == 0){
            locctr += 3;
        }
        else if(strcmp(opcode, "RESW") == 0){
            locctr += 3 * atoi(operand);
        }
        else if(strcmp(opcode, "BYTE") == 0){
            locctr += 1;
        }
        else if(strcmp(opcode, "RESB") == 0){
            locctr += atoi(operand);
        }
        else{
            printf("\nInvalid Opcode!\n");
        }

        fprintf(intr, "%04x\t%s\t%s\t%s\n", locctr, label, opcode, operand);
        fscanf(input, "%s %s %s", label, opcode, operand);
    }

    fprintf(intr, "%04x\t%s\t%s\t%s\n", locctr - startAddress, label, opcode, operand);
    fprintf(length, "%x", locctr);

    fclose(input);
    fclose(symtab);
    fclose(optab);
    fclose(intr);
    fclose(length);

    puts("Pass 1 Complete!");
}