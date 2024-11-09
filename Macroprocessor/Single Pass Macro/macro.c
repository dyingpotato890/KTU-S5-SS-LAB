#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

FILE *input,*output,*deftab,*namtab,*deftabread;
char argtab[10][20];
bool expanding=false;
int argcount=0;
void processLine(char *line);
void getLine(char *line){
    fgets(line,80,input);
     
}

void defineMacro(char *line){
    char label[20],opcode[20],operand[20],arguments[20][20], *token;
    int argcount=0,level=1;
    deftab = fopen("deftab.txt","a");
    namtab = fopen("namtab.txt","a");
    sscanf(line,"%s %s %s",label,opcode,operand);
    fprintf(deftab,"\n");
    fprintf(namtab,"%s %d ",label,ftell(deftab));
    fprintf(deftab,"%s %s\n",label,operand);
    token = strtok(operand,",");
    while (token!=NULL){
        strcpy(arguments[argcount++],token);
        token = strtok(NULL,",");
    }
    getLine(line);
    sscanf(line,"%s %s %s",label,opcode,operand);
    while (level > 0){
        if (operand[0]=='&'){
            for (int i=0;i<argcount;i++){
                if (strcmp(arguments[i],operand)==0){
                    sprintf(operand,"?%d",i+1);
                }
            }
        }
        
        fprintf(deftab,"%s %s %s\n",label,opcode,operand);
        if (strcmp(opcode,"MACRO")==0){
            level++;
        }
        else if (strcmp(opcode,"MEND")==0){
            level--;
            if (level==0){
                break;
            }
        }
        getLine(line);
        sscanf(line,"%s %s %s",label,opcode,operand);
    }
    fprintf(namtab,"%d\n",ftell(deftab));
    fclose(namtab);
    fclose(deftab);
}

void expandMacro(char *line,int start, int end){
    char label[20],opcode[20],operand[20],macroname[20], finalline[100];
    int argnum;
    argcount = 0;
    sscanf(line,"%s %s %s",label, opcode,operand);
    fprintf(output,". %s %s\n",opcode,operand);
    char *token = strtok(operand,",");
    while (token!=NULL){
        strcpy(argtab[argcount],token);
        argcount++;
        token = strtok(NULL,",");
    }
    expanding = true;
    deftabread = fopen("deftab.txt","r");
    fseek(deftabread,start,SEEK_SET);
    
    
    fscanf(deftabread,"%s %s",opcode,operand);
    while (fscanf(deftabread,"%s %s %s",label,opcode,operand)!=EOF){
        if (strcmp(opcode,"MEND")==0){
            break;
        }
        if (operand[0]=='?'){
            fprintf(output,"%s %s %s\n",label,opcode,argtab[(int) operand[1]-'0'-1]);

        }
        else{
            fprintf(output,"%s %s %s\n",label,opcode,operand);
        }
    }
    
    
    fclose(deftabread);
    expanding = false;
    
}

void main(){
    input = fopen("input.txt", "r");
    output = fopen("output.txt","w");
    expanding = false;
    char line[100], label[100], opcode[100], operand[100];
    getLine(line);
    sscanf(line,"%s %s %s",label,opcode,operand);
    while (strcmp(opcode,"END")!=0){
        getLine(line);
        processLine(line);
        sscanf(line,"%s %s %s",label,opcode,operand);
    }
    fclose(input);
    fclose(output);
    printf("Successfully completed\n");

}

void processLine(char *line){
    char label[100],opcode[100],operand[100], macroname[100], namrec[100];
    int start,end;

    sscanf(line,"%s %s %s",label,opcode,operand);
    
    if (strcmp(opcode,"MACRO")==0){
        defineMacro(line);
        return;
    }
    FILE *searchNamtab = fopen("namtab.txt","r");
    
    while (fscanf(searchNamtab,"%s %d %d",macroname,&start, &end)!=EOF){
        if (strcmp(macroname,opcode)==0){
            expandMacro(line,start,end);
            return;
        }
    }
    fclose(searchNamtab);
    
    fprintf(output,"%s %s %s\n",label,opcode,operand);
}