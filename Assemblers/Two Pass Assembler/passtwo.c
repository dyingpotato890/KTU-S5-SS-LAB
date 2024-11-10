#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *intermediate, *symtab, *length, *optab, *object, *output;

int findSymbolAddress(char *symbol)
{
    char search[10];
    int address;
    fseek(symtab, 0, SEEK_SET);
    while (fscanf(symtab, "%s %x", search, &address) != EOF)
    {
        if (strcmp(symbol, search) == 0)
        {
            return address;
        }
    }
    return -1;
}

int findOpcode(char *mnemonic)
{
    char search[10];
    int opcode;
    fseek(optab, 0, SEEK_SET);
    while (fscanf(optab, "%s %x", search, &opcode) != EOF)
    {
        if (strcmp(mnemonic, search) == 0)
        {
            return opcode;
        }
    }
    return -1;
}

void main()
{
    intermediate = fopen("intermediate.txt", "r");
    symtab = fopen("symtab.txt", "r");
    length = fopen("length.txt", "r");
    optab = fopen("optab.txt", "r");
    object = fopen("object.txt", "w");
    output = fopen("output.txt", "w");

    int finadr, mnemonic, addr, start, prevaddr, lenloc, currentloc, len, symaddr;
    fscanf(length, "%x", &finadr);
    fclose(length);

    char label[10], opcode[10], operand[10];
    fscanf(intermediate, "%s %s %s", label, opcode, operand);

    if (strcmp(opcode, "START") == 0)
    {
        fprintf(output, "%s\t%s\t%s\n", label, opcode, operand);
        sscanf(operand,"%x",&start);
        fprintf(object, "H %s %6x %x\n", label, start, finadr - start);
    }
    fscanf(intermediate, "%x %s %s %s", &addr, label, opcode, operand);
    fprintf(object, "T %06x ", addr);
    lenloc = ftell(object);
    fprintf(object, "00 ");
    int remaining = 60;
    while (strcmp(opcode, "END") != 0)
    {
        printf("%s\n",opcode);
        char objcode[20] = "";
        if ((mnemonic = findOpcode(opcode)) != -1)
        {
            if ((symaddr = findSymbolAddress(operand)) != -1)
            {
                sprintf(objcode, "%02x%04x ", mnemonic, symaddr);
            }
            else
            {
                sprintf(objcode, "%02x%04x ", mnemonic, 0);
            }
        }
        else if (strcmp(opcode, "BYTE") == 0)
        {
            if (operand[0] == 'C')
                sprintf(objcode, "%02x ", (int)operand[2]);
            else
            {
                sprintf(objcode, "%02x ", atoi(operand));
            }
        }
        else if (strcmp(opcode, "WORD") == 0)
        {
            sprintf(objcode, "%06x ", atoi(operand));
        }
        else if (strcmp(opcode, "RESB") == 0)
        {
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

        else if (strcmp(opcode, "RESW") == 0)
        {
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

        if (strlen(objcode) <= remaining)
        {
            fprintf(object, "%s", objcode);
            remaining -= strlen(objcode);
        }
        else
        {
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
        fscanf(intermediate,"%x %s %s %s",&addr,label,opcode,operand);
    }
    fprintf(object,"\nE %06x",start);

       
    fclose(intermediate);
    fclose(symtab);
    fclose(optab);
    fclose(object);
    fclose(output);

}