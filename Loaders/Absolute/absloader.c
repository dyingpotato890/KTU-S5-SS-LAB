#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *INPUT, *OUTPUT;

void readByte(char* byte, char* record, int start){
    byte[0] = record[start];
    byte[1] = record[start + 1];
    byte[2] = '\0';
}

void main(){
    INPUT = fopen("input.txt", "r");
    OUTPUT = fopen("output.txt", "w");

    char recordType[3], pn[10], record[100], byte[3];
    int length, finadr, locctr = 0, offset;

    fscanf(INPUT, "%s", recordType);

    while(strcmp(recordType, "E") != 0){
        if(strcmp(recordType, "H") == 0){
            fscanf(INPUT, "%s", pn);
            fscanf(INPUT, "%x", &locctr);
            fscanf(INPUT, "%x", &finadr);
        }
        else if(strcmp(recordType, "T") == 0){
            fscanf(INPUT, "%x", &locctr);
            fscanf(INPUT, "%x", &length);
            fscanf(INPUT, "%s", record);

            for (offset = 0; offset < strlen(record); offset += 2){
                readByte(byte, record, offset);
                fprintf(OUTPUT, "%x\t%s\n", locctr, byte);
                locctr++;
            }
        }
        fscanf(INPUT, "%s", recordType);
    }

    fclose(INPUT);
    fclose(OUTPUT);
}