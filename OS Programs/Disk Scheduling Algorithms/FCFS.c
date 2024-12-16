#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int cylinders, noOfTracks, head;

int performFCFS(int requests[]){
    int totalTime = 0;

    for(int i = 0; i < noOfTracks; i++){
        totalTime += abs(head - requests[i]);
        head = requests[i];
    }

    return totalTime;
}

void main(){
    printf("Enter The Total Number of Cylinders: ");
    scanf("%d", &cylinders);
    printf("Enter The Total Number of Tracks: ");
    scanf("%d", &noOfTracks);

    int requests[noOfTracks];
    printf("\nEnter The Track Numbers:\n\n");
    for(int i = 0; i < noOfTracks; i++){
        printf(">>> ");
        scanf("%d", &requests[i]);
    }

    printf("\nEnter The Current Track Pointer: ");
    scanf("%d", &head);

    printf("\nTotal Seek Time (FCFS)= %d ms", performFCFS(requests));
}