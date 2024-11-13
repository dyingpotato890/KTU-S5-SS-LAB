#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *input, *output;

void toBinary(char *stringout, char *stringin);
void toByte(char *byte, char *stringin, int offset);

void main()
{
    char recordtype[3], bitmask[5], bits[13], pgname[10],word[10], byte[4];
    int actualStart, address, start, end, length, locctr, reclen, instruction;

    printf("Enter the actual start address: ");
    scanf("%x", &actualStart);

    input = fopen("input.txt", "r");
    output = fopen("output.txt", "w");

    fscanf(input, "%s", recordtype);
    while (strcmp(recordtype, "E") != 0)
    {
        if (strcmp(recordtype, "H") == 0)
        {
            fscanf(input, "%s", pgname);
            fscanf(input, "%x", &start);
            fscanf(input, "%x", &end);
        }
        else if (strcmp(recordtype, "T") == 0)
        {
            strcpy(bits,"");
            fscanf(input, "%x", &locctr);
            locctr += actualStart;

            fscanf(input, "%x", &reclen);
            fscanf(input, "%s", bitmask);
            toBinary(bits,bitmask);
            for (int i=0; i<reclen/3; i++){
                fscanf(input,"%x",&instruction);
                if (bits[i]=='1'){
                    instruction+=actualStart;
                }
                sprintf(word,"%x",instruction);
                for (int offset=0;offset<6;offset+=2){
                    toByte(byte,word,offset);
                    fprintf(output,"%x\t%s\n",locctr++,byte);
                }
            }

        }
        fscanf(input,"%s",recordtype);
    }
    fclose(input);
    fclose(output);

}

void toByte(char *byte, char *stringin,int offset){
    byte[0]=stringin[offset];
    byte[1]=stringin[offset+1];
    byte[2]='\0';
}
void toBinary(char *stringout, char *stringin)
{
    int l = strlen(stringin);
    for (int i = 0; i < l; i++)
    {
        switch (stringin[i])
        {
        case '0':
            strcat(stringout, "0000");
            break;
        case '1':
            strcat(stringout, "0001");
            break;
        case '2':
            strcat(stringout, "0010");
            break;
        case '3':
            strcat(stringout, "0011");
            break;
        case '4':
            strcat(stringout, "0100");
            break;
        case '5':
            strcat(stringout, "0101");
            break;
        case '6':
            strcat(stringout, "0100");
            break;
        case '7':
            strcat(stringout, "0111");
            break;
        case '8':
            strcat(stringout, "1000");
            break;
        case '9':
            strcat(stringout, "1001");
            break;
        case 'A':
            strcat(stringout, "1010");
            break;
        case 'B':
            strcat(stringout, "1011");
            break;
        case 'C':
            strcat(stringout, "1100");
            break;
        case 'D':
            strcat(stringout, "1101");
            break;
        case 'E':
            strcat(stringout, "1110");
            break;
        case 'F':
            strcat(stringout, "1111");
            break;
        default:
            break;
        }
    }
}