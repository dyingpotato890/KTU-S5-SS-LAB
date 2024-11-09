#include <stdio.h>
#include <string.h>
#include <stdlib.h>
char pgname[6],inpname[6],address[8];
int i,j;
FILE *inptr,*outptr;
char line[100];
void genhex(int index){
  if(index >= 0){
    char hex = address[index];
    if(hex == '9'){
      address[index] = 'A';
    }
    else if(hex + 1 > 'F'){
      address[index] = '0';
      genhex(index-1);
    }
    else address[index] = hex+1;
  }
}
int main(){
  printf("Enter the name of the program: "); 
  scanf("%s",inpname);
  //opening files
  inptr = fopen("input.txt","r");
  outptr = fopen("output.txt","w");

  //verifying header name
  fscanf(inptr,"%s",line);
  for(i = 2, j = 0; line[i] != '^';i++,j++)
    pgname[j] = line[i];
  printf("name of program: %s\n",pgname);


  if(strcmp(pgname,inpname) == 0){
    //starting reading the text records
    do{
      fscanf(inptr,"%s",line);
      if(line[0] == 'T'){
        for(i = 2, j = 0;line[i] != '^';i++,j++){
          address[j] = line[i];
        }
        address[j] = '\0';
        i++;
        while(line[i] != '\0'){
          if(line[i] != '^'){
            fprintf(outptr,"%s\t %c%c\n",address,line[i],line[i+1]);
            genhex(5);
            i += 2;
          }
          else i++;
        }
      }
      else if(line[0] == 'E'){

        printf("program successfully loaded into memory!\n");
        fclose(inptr);
        exit(0);
      }
    }
    while(!feof(inptr));
  }
  else printf("the name of the program doesn't match..\n");
}
