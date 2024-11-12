#include<stdio.h>
#include<stdlib.h>
#include<string.h>
FILE *input,*output;


void readByte(char *byte, char *line,int start){
    byte[0]=line[start];
    byte[1]=line[start+1];
    byte[2]='\0';
}

void main(){
    input = fopen("input.txt","r");
    output = fopen("output.txt","w");
    
    
    int locctr,finadr,length,offset;
    char record[100],line[80],pgname[10],recordtype[3],byte[3],*token;

    fscanf(input,"%s",recordtype);
    while(strcmp(recordtype,"E")!=0){
        if(strcmp(recordtype,"H")==0){
            fscanf(input,"%s",pgname);
            fscanf(input,"%x",&locctr);
            fscanf(input,"%x",&finadr);
        }
        else if(strcmp(recordtype,"T")==0){
            fscanf(input,"%x",&locctr);
            fscanf(input,"%x",&length);
            fscanf(input,"%s",line);
            for(offset=0;offset<strlen(line);offset+=2){
                readByte(byte,line,offset);
                fprintf(output,"%x\t%s\n",locctr,byte);
                locctr++;
            }
        }
        fscanf(input,"%s",recordtype);
    }

    fclose(input);
    fclose(output);

}