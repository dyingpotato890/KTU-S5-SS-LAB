#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node {
    int id, at, bt, ct, wt, tat;
    struct node* next;
};

struct node* head = NULL;
int processes, pointer = -1;

void SortByID(struct node* head) {
    struct node* temp = head;
    while (temp != NULL) {
        struct node* smallNode = temp;
        struct node* temp2 = temp->next;
        while (temp2 != NULL) {
            if ((temp2->id) < (smallNode->id))
                smallNode = temp2;
            temp2 = temp2->next;
        }
        if (temp != smallNode) {
            int id = temp->id;
            int at = temp->at;
            int bt = temp->bt;
            int ct = temp->ct;
            int tat = temp->tat;
            int wt = temp->wt;

            temp->id = smallNode->id;
            temp->at = smallNode->at;
            temp->bt = smallNode->bt;
            temp->ct = smallNode->ct;
            temp->tat = smallNode->tat;
            temp->wt = smallNode->wt;

            smallNode->id = id;
            smallNode->at = at;
            smallNode->bt = bt;
            smallNode->ct = ct;
            smallNode->tat = tat;
            smallNode->wt = wt;
        }
        temp = temp->next;
    }
}

void Create(){
    struct node* temp = NULL;
    for(int i = 0; i < processes; i++){
        printf("\nPROCESS %d\n", i+1);
        struct node* newNode = (struct node*)malloc(sizeof(struct node));
        newNode -> id = i+1;
        printf("Arrival Time: ");
        scanf("%d", &newNode -> at);
        printf("Burst Time: ");
        scanf("%d", &newNode -> bt);

        if(head == NULL){
            head = temp = newNode;
        }
        else{
            temp -> next = newNode;
            temp = newNode;
        }
        newNode -> next = NULL;
    }
}

void CheckArrival(struct node* head, struct node* array[], int isExecuted[], int currentTime) {
    struct node* temp = head;
    while (temp != NULL) {
        if (temp->at <= currentTime && isExecuted[(temp->id) - 1] == 0) {
            pointer++;
            array[pointer] = temp;
        }
        temp = temp->next;
    }
}

int AllExecuted(int isExecuted[]) {
    for (int i = 0; i < processes; i++) {
        if (isExecuted[i] == 0) {
            return 0;
        }
    }
    return 1;
}

void display(struct node* head) {
    struct node* ptr = head;
    float total_wt = 0;
    float total_tat = 0;

    printf("\n------------------- SJF -------------------");
    printf("\nPID\tAT\tBT\tCT\tWT\tTAT\n");
    printf("-------------------------------------------\n");
    while (ptr != NULL) {
        total_wt += ptr->wt;
        total_tat += ptr->tat;
        printf("%d\t%d\t%d\t%d\t%d\t%d\n", ptr->id, ptr->at, ptr->bt, ptr->ct, ptr->wt, ptr->tat);
        ptr = ptr->next;
    }
    printf("-------------------------------------------\n");

    printf("\nAverage Waiting Time: %.2f ms", total_wt / processes);
    printf("\nAverage Turn Around Time: %.2f ms", total_tat / processes);
}

struct node* ReturnSmallest(struct node* array[]) {
    struct node* smallNode = array[0];
    for (int i = 1; i <= pointer; i++) {
        if ((array[i]->bt) < (smallNode->bt)) {
            smallNode = array[i];
        }
    }
    return smallNode;
}

void executeSJF(struct node* head) {
    int currentTime = 0;
    int isExecuted[processes];
    memset(isExecuted, 0, sizeof(isExecuted));
    
    struct node* array[processes];
    while (AllExecuted(isExecuted) == 0) {
        CheckArrival(head, array, isExecuted, currentTime);
        if (pointer == -1) {
            currentTime++;
            continue;
        }
        struct node* toExecute = ReturnSmallest(array);
        currentTime = (currentTime < toExecute->at) ? toExecute->at : currentTime;
        currentTime += toExecute->bt;

        toExecute->ct = currentTime;
        toExecute->tat = (toExecute->ct) - (toExecute->at);
        toExecute->wt = (toExecute->tat) - (toExecute->bt);
        isExecuted[(toExecute->id) - 1] = 1;

        pointer = -1;
    }
    SortByID(head);
    display(head);
}

void Sort(struct node* head) {
    struct node* temp = head;
    while (temp != NULL) {
        struct node* smallNode = temp;
        struct node* temp2 = temp->next;
        while (temp2 != NULL) {
            if ((temp2->at) < (smallNode->at)) {
                smallNode = temp2;
            }
            temp2 = temp2->next;
        }
        if (temp != smallNode) {
            int id = temp->id;
            int at = temp->at;
            int bt = temp->bt;

            temp->id = smallNode->id;
            temp->at = smallNode->at;
            temp->bt = smallNode->bt;

            smallNode->id = id;
            smallNode->at = at;
            smallNode->bt = bt;
        }
        temp = temp->next;
    }
}

void main() {
    printf("Shortest Job First: Non-Preemptive\n");
    printf("----------------------------------\n\n");
    printf("Enter The Total Number of Processes: ");
    scanf("%d", &processes);

    Create();
    Sort(head);
    executeSJF(head);
}
