#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *intr, *symtab, *length, *optab, *object, *output;

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
    intr = fopen("intermediate.txt", "r");
    symtab = fopen("symtab.txt", "r");
    length = fopen("length.txt", "r");
    optab = fopen("optab.txt", "r");
    object = fopen("object.txt", "w");
    output = fopen("output.txt", "w");

    int finadr, mnemonic, addr, start, prevaddr, lenloc, currentloc, len, symaddr, LOCCTR;
    fscanf(length, "%x", &finadr);
    fclose(length);

    char label[10], opcode[10], operand[10];
    fscanf(intr, "%x %s %s %s", LOCCTR, label, opcode, operand);

    if (strcmp(opcode, "START") == 0){
        fprintf(output, "%s\t%s\t%s\n", label, opcode, operand);
        sscanf(operand,"%x",&start);
        fprintf(object, "H %s %6x %x\n", label, start, finadr - start);
    }
    fscanf(intr, "%x %s %s %s", &addr, label, opcode, operand);

    fprintf(object, "T %06x ", addr);
    lenloc = ftell(object);
    fprintf(object, "00 ");
    int remaining = 60;

    while (strcmp(opcode, "END") != 0){
        char objcode[20] = "";
        if ((mnemonic = searchTable(optab, opcode)) != -1){
            if ((symaddr = searchTable(symtab, operand)) != -1){
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
            currentloc = ftell(object);

            fseek(object, lenloc, SEEK_SET);
            fprintf(object, "%02x", 60 - remaining);

            fseek(object, currentloc, SEEK_SET);
            fprintf(object, "\nT %06x ", addr + numbytes);
            remaining = 60;
            lenloc = ftell(object);
            fprintf(object, "00 ");
        }

        else if (strcmp(opcode, "RESW") == 0){
            int numbytes = 3 * atoi(operand);
            currentloc = ftell(object);

            fseek(object, lenloc, SEEK_SET);
            fprintf(object, "%02x", 60 - remaining);

            fseek(object, currentloc, SEEK_SET);
            fprintf(object, "\nT %06x ", addr + numbytes);
            remaining = 60;
            lenloc = ftell(object);
            fprintf(object, "00 ");
        }
        else if(strcmp(opcode,"END")==0)
            break;

        if (strlen(objcode) <= remaining){
            fprintf(object, "%s", objcode);
            remaining -= strlen(objcode);
        }
        else{
            currentloc = ftell(object);
            fseek(object, lenloc, SEEK_SET);
            fprintf(object, "%02x", 60 - remaining);

            fseek(object, currentloc, SEEK_SET);
            fprintf(object, "\nT %06x ", addr);
            remaining = 60;
            lenloc = ftell(object);
            fprintf(object, "00 ");
            fprintf(object, "%s", objcode);
            remaining -= strlen(objcode);
        }
        fprintf(output, "%x %s %s %s %s\n", addr, label, opcode, operand, objcode);
        fscanf(intr,"%x %s %s %s",&addr,label,opcode,operand);
    }
    fprintf(object,"\nE %06x",start);
       
    fclose(intr);
    fclose(symtab);
    fclose(optab);
    fclose(object);
    fclose(output);

    puts("Pass 2 Complete!");
}