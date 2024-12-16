#include <stdio.h>
#include <stdlib.h>

#define MAX_REQUESTS 100

int rnum, head, dir, MAX_TRACKS;

int CSCAN(int request[MAX_REQUESTS]) {
    int totalTime = 0;
    int index = 0;

    for (int i = 0; i < rnum; i++) {
        for (int j = 0; j < rnum - i - 1; j++) {
            if (request[j] > request[j + 1]) {
                int temp = request[j];
                request[j] = request[j + 1];
                request[j + 1] = temp;
            }
        }
    }

    for (int i = 0; i < rnum; i++) {
            if (request[i] >= head) {
                index = i;
                break;
            }
    }
    
    if (dir == 1) {
        for (int i = index; i < rnum; i++) {
            totalTime += abs(head - request[i]);
            head = request[i];
        }

        if (index != 0) {
            totalTime += abs(MAX_TRACKS - request[rnum - 1] - 1); // Checks for the furthest tracks and calculates the distance
            head = 0;
            totalTime += abs(MAX_TRACKS - 1);
            for (int i = 0; i < index; i++) {
                totalTime += abs(head - request[i]);
                head = request[i];
            }
        }
    }
    else {
        for (int i = index - 1; i >= 0; i--) {
            totalTime += abs(head - request[i]);
            head = request[i];
        }

        if (index != rnum - 1) {
            totalTime += abs(head - 0);
            head = MAX_TRACKS - 1;
            totalTime += abs(MAX_TRACKS - 1 - 0);
        }

        for (int i = rnum - 1; i >= index; i--) {
            totalTime += abs(head - request[i]);
            head = request[i];
        }
    }

    return totalTime;
}

int main() {
    printf("\nEnter The Maximum Number Of Tracks: ");
    scanf("%d", &MAX_TRACKS);

    printf("\nEnter The Number Of Requests: ");
    scanf("%d", &rnum);

    int requests[rnum];
    for (int i = 0; i < rnum; i++) {
        printf("Enter The Disk Number - %d: ", (i + 1));
        scanf("%d", &requests[i]);
    }

    printf("\nPosition of Head: ");
    scanf("%d", &head);

    printf("\nEnter The Direction (0/1): ");
    scanf("%d", &dir);

    printf("\nTotal Time (C-SCAN)= %d ms", CSCAN(requests));
    return 0;
}