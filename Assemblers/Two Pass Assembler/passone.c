#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *input, *intermediate, *symtab, *optab;

void addToSymtab(char *label, int address)
{
    fprintf(symtab,"%s %x\n", label, address);
}

int findOpcode(char *mnemonic)
{
    fseek(optab, 0, SEEK_SET);
    char string[10];
    int opcode;
    while (fscanf(optab, "%s %x", string, &opcode) != EOF)
    {
        if (strcmp(mnemonic, string) == 0)
        {
            return opcode;
        }
    }
    return -1;
}

void main()
{
    input = fopen("input.txt", "r");
    intermediate = fopen("intermediate.txt", "w");
    symtab = fopen("symtab.txt", "w");
    optab = fopen("optab.txt", "r");

    char label[10], opcode[10], operand[10];
    int locctr;

    fscanf(input, "%s %s %s", label, opcode, operand);

    if (strcmp(opcode, "START") == 0)
    {
        sscanf(operand, "%x", &locctr);
        printf("set locctr to %x %s",locctr,operand);
        fprintf(intermediate, "%s\t%s\t%s\n", label, opcode, operand);
    }
    else
    {
        locctr = 0;
    }

    fscanf(input, "%s %s %s", label, opcode, operand);
    while (strcmp(opcode, "END") != 0)
    {
        fprintf(intermediate,"%x\t%s\t%s\t%s\n",locctr,label,opcode,operand);
        if (strcmp(label, "**") != 0)
        {
            addToSymtab(label, locctr);
        }
        if (findOpcode(opcode) != -1)
        {
            locctr += 3;
        }
        else if (strcmp(opcode, "WORD") == 0)
        {
            locctr += 3;
        }
        else if (strcmp(opcode, "BYTE") == 0)
        {
            locctr += 1;
        }
        else if (strcmp(opcode, "RESW")==0){
            locctr += 3 * atoi(operand);
        }
        else if (strcmp(opcode,"RESB")==0){
            locctr += atoi(operand);
        }
        fscanf(input,"%s %s %s",label,opcode,operand);
    }
    fprintf(intermediate,"%x\t%s\t%s\t%s\n",locctr,label,opcode,operand);

    FILE *length = fopen("length.txt","w");
    fprintf(length,"%x",locctr);

    fclose(input);
    fclose(intermediate);
    fclose(optab);
    fclose(symtab);
    fclose(length);
}