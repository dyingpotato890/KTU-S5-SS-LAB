#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *INTR, *SYMTAB, *LENGTH, *OPTAB, *OBJECT, *OUTPUT;

int searchTable(FILE* file, char check[]) {
    char label[10];
    int value;
    rewind(file);

    while (fscanf(file, "%s %x", label, &value) != EOF) {
        if (strcmp(check, label) == 0) {
            return value;
        }
    }
    return -1;
}

void main(){
    INTR = fopen("intermediate.txt", "r");
    SYMTAB = fopen("symtab.txt", "r");
    LENGTH = fopen("length.txt", "r");
    OPTAB = fopen("optab.txt", "r");
    OBJECT = fopen("object.txt", "w");
    OUTPUT = fopen("output.txt", "w");

    int finadr, mnemonic, addr, start, prevaddr, lenloc, currentloc, len, symaddr, LOCCTR;
    fscanf(LENGTH, "%x", &finadr);
    fclose(LENGTH);

    char label[10], opcode[10], operand[10];
    fscanf(INTR, "%x %s %s %s", LOCCTR, label, opcode, operand);

    if (strcmp(opcode, "START") == 0){
        fprintf(OUTPUT, "%s\t%s\t%s\n", label, opcode, operand);
        sscanf(operand,"%x",&start);
        fprintf(OBJECT, "H %s %6x %x\n", label, start, finadr - start);
    }
    fscanf(INTR, "%x %s %s %s", &addr, label, opcode, operand);

    fprintf(OBJECT, "T %06x ", addr);
    lenloc = ftell(OBJECT);
    fprintf(OBJECT, "00 ");
    int remaining = 60;

    while (strcmp(opcode, "END") != 0){
        char objcode[20] = "";
        if ((mnemonic = searchTable(OPTAB, opcode)) != -1){
            if ((symaddr = searchTable(SYMTAB, operand)) != -1){
                sprintf(objcode, "%02x%04x ", mnemonic, symaddr);
            }
            else{
                sprintf(objcode, "%02x%04x ", mnemonic, 0);
            }
        }
        else if (strcmp(opcode, "BYTE") == 0){
            if (operand[0] == 'C')
                sprintf(objcode, "%02x ", (int)operand[2]);
            else{
                sprintf(objcode, "%02x ", atoi(operand));
            }
        }
        else if (strcmp(opcode, "WORD") == 0){
            sprintf(objcode, "%06x ", atoi(operand));
        }
        else if (strcmp(opcode, "RESB") == 0){
            int numbytes = atoi(operand);
            currentloc = ftell(OBJECT);

            fseek(OBJECT, lenloc, SEEK_SET);
            fprintf(OBJECT, "%02x", 60 - remaining);

            fseek(OBJECT, currentloc, SEEK_SET);
            fprintf(OBJECT, "\nT %06x ", addr + numbytes);
            remaining = 60;
            lenloc = ftell(OBJECT);
            fprintf(OBJECT, "00 ");
        }

        else if (strcmp(opcode, "RESW") == 0){
            int numbytes = 3 * atoi(operand);
            currentloc = ftell(OBJECT);

            fseek(OBJECT, lenloc, SEEK_SET);
            fprintf(OBJECT, "%02x", 60 - remaining);

            fseek(OBJECT, currentloc, SEEK_SET);
            fprintf(OBJECT, "\nT %06x ", addr + numbytes);
            remaining = 60;
            lenloc = ftell(OBJECT);
            fprintf(OBJECT, "00 ");
        }
        else if(strcmp(opcode,"END")==0)
            break;

        if (strlen(objcode) <= remaining){
            fprintf(OBJECT, "%s", objcode);
            remaining -= strlen(objcode);
        }
        else{
            currentloc = ftell(OBJECT);
            fseek(OBJECT, lenloc, SEEK_SET);
            fprintf(OBJECT, "%02x", 60 - remaining);

            fseek(OBJECT, currentloc, SEEK_SET);
            fprintf(OBJECT, "\nT %06x ", addr);
            remaining = 60;
            lenloc = ftell(OBJECT);
            fprintf(OBJECT, "00 ");
            fprintf(OBJECT, "%s", objcode);
            remaining -= strlen(objcode);
        }
        fprintf(OUTPUT, "%x %s %s %s %s\n", addr, label, opcode, operand, objcode);
        fscanf(INTR,"%x %s %s %s",&addr,label,opcode,operand);
    }
    fprintf(OBJECT,"\nE %06x",start);
       
    fclose(INTR);
    fclose(SYMTAB);
    fclose(OPTAB);
    fclose(OBJECT);
    fclose(OUTPUT);

    puts("Pass 2 Complete!");
}