#include <stdio.h>

char memory[25];
int front = -1, rear = -1;

void EnQueue(char page, int n){
    if(rear == -1){
        front = rear = 0;
        memory[rear] = page;
    }
    else if((rear + 1) % n != front){
        rear = (rear + 1) % n;
        memory[rear] = page;
    }
}

int isInMemory(char page, int n){
    for(int i = 0; i < n; i++){
        if(page == memory[i]){
            return 1;
        }
    }
    return 0;
}

void Execute(char pages[], int n){
    int hitCount = 0, missCount = 0;
    for(int i = 0; i < n; i++){
        memory[i] = ' ';
    }
    for(int i = 0; pages[i] != '\0'; i++){
        if(isInMemory(pages[i], n) == 0){
            if((rear + 1) % n != front){
                EnQueue(pages[i], n);
            }
            else{
                front++;
                EnQueue(pages[i], n);
            }
            missCount++;
        }
        else{
            hitCount++;
        }
        for(int j = 0; j < n; j++){
            printf("%c ", memory[j]);
        }
        printf("\n");
    }
    printf("\nHit Count = %d", hitCount);
    printf("\nMiss Count = %d", missCount);
    printf("\nHit Ratio = %f", (float)hitCount / (float)(hitCount + missCount));
    printf("\nMiss Ratio = %f", (float)missCount / (float)(hitCount + missCount));
}

void main(){
    printf("FIFO Page Replacement\n");
    printf("---------------------\n\n");
    printf("Enter the page string: ");
    char pages[25];
    scanf("%s", pages);
    printf("Enter the number of frames: ");
    int n;
    scanf("%d", &n);

    Execute(pages, n);
}
