#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int processes;

void ReadProcesses(int allocatedMatrix[processes][3], int maxMatrix[processes][3], int needMatrix[processes][3]){
    for(int i = 0; i < processes; i++){
        printf("Process %d:\n", i+1);
        printf("Allocated Matrix:\n");
        printf(">>>");
        scanf("%d%d%d", &allocatedMatrix[i][0], &allocatedMatrix[i][1], &allocatedMatrix[i][2]);
        printf("\nMax Matrix:\n");
        printf(">>>");
        scanf("%d%d%d", &maxMatrix[i][0], &maxMatrix[i][1], &maxMatrix[i][2]);

        needMatrix[i][0] = maxMatrix[i][0] - allocatedMatrix[i][0];
        needMatrix[i][1] = maxMatrix[i][1] - allocatedMatrix[i][1];
        needMatrix[i][2] = maxMatrix[i][2] - allocatedMatrix[i][2];
    }
}

void Display(int allocatedMatrix[processes][3], int maxMatrix[processes][3], int needMatrix[processes][3]){
    printf("Process\tAllocated Matrix\tMax Matrix\tNeed Matrix\n");
    for(int i = 0; i < processes; i++){
        printf("%d\t%d %d %d\t\t\t %d %d %d\t\t %d %d %d\n", i, allocatedMatrix[i][0], allocatedMatrix[i][1],
               allocatedMatrix[i][2], maxMatrix[i][0], maxMatrix[i][1], maxMatrix[i][2], needMatrix[i][0],
               needMatrix[i][1], needMatrix[i][2]);
    }
}

int CheckNeed(int availableMatrix[1][3], int needMatrix[processes][3], int i){
    if((needMatrix[i][0] <= availableMatrix[0][0]) && (needMatrix[i][1] <= availableMatrix[0][1]) && (needMatrix[i][2]
       <= availableMatrix[0][2])){
        return 1;
       }
    else{
        return 0;
    }
}

void main(){
    printf("Enter The Total Number of Processes: ");
    scanf("%d", &processes);

    int allocatedMatrix[processes][3], maxMatrix[processes][3], availableMatrix[1][3], needMatrix[processes][3];
    ReadProcesses(allocatedMatrix, maxMatrix, needMatrix);

    printf("Enter Available Matrix: ");
    scanf("%d%d%d", &availableMatrix[0][0], &availableMatrix[0][1], &availableMatrix[0][2]);

    int safeSequence[processes];
    memset(safeSequence, 0, processes * sizeof(int));

    Display(allocatedMatrix, maxMatrix, needMatrix);

    int flag = 0, i = 0, check = 0;

    printf("\nSafe Sequence: ");
    while(flag == 0){
        if(safeSequence[i] == 0 && CheckNeed(availableMatrix, needMatrix, i) == 1){
            safeSequence[i] = 1;
            printf("P%d, ", i);
            availableMatrix[0][0] += allocatedMatrix[i][0];
            availableMatrix[0][1] += allocatedMatrix[i][1];
            availableMatrix[0][2] += allocatedMatrix[i][2];
        }
        i = (i+1) % processes;
        int flag2 = 0;
        for(int j = 0; j < processes; j++){
            if(safeSequence[j] == 0){
                flag2 = 1;
                break;
            }
        }
        if(flag2 == 1){
            if(i == 0){
                check++;
            }
            if(check == processes){
                flag = 1;
                break;
            }
        }
        else{
            flag = 1;
            break;
        }
    }
    if(check == processes)
        printf("Unsafe");
}