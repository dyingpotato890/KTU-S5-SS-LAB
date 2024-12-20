#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *input, *output;

void readByte(char* byte, char* record, int start){
    byte[0] = record[start];
    byte[1] = record[start + 1];
    byte[2] = '\0';
}

void main(){
    input = fopen("input.txt", "r");
    output = fopen("output.txt", "w");

    char recordType[3], pn[10], record[100], byte[3];
    int length, finadr, locctr = 0, offset;

    fscanf(input, "%s", recordType);

    while(strcmp(recordType, "E") != 0){
        if(strcmp(recordType, "H") == 0){
            fscanf(input, "%s", pn);
            fscanf(input, "%x", &locctr);
            fscanf(input, "%x", &finadr);
        }
        else if(strcmp(recordType, "T") == 0){
            fscanf(input, "%x", &locctr);
            fscanf(input, "%x", &length);
            fscanf(input, "%s", record);

            for (offset = 0; offset < strlen(record); offset += 2){
                readByte(byte, record, offset);
                fprintf(output, "%x\t%s\n", locctr, byte);
                locctr++;
            }
        }
        fscanf(input, "%s", recordType);
    }

    fclose(input);
    fclose(output);
}