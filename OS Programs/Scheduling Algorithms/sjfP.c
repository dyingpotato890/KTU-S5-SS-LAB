#include <stdio.h>
#include <stdlib.h>

struct node{
    int id, at, bt, tempbt, ct, tat, wt, isCompleted;
    struct node* next;
};

struct node* head = NULL;
int processes, pointer = -1;

void SortByID(){
    struct node* temp = head;
    while(temp != NULL){
        struct node* smallNode = temp;
        struct node* temp2 = temp -> next;
        while(temp2 != NULL){
            if((temp2 -> id) < (smallNode -> id))
                smallNode = temp2;
            temp2 = temp2 -> next;
        }

        if(temp != smallNode){
            int id = smallNode -> id;
            int at = smallNode -> at;
            int bt = smallNode -> bt;
            int ct = smallNode -> ct;
            int tat = smallNode -> tat;
            int wt = smallNode -> wt;
            smallNode -> id = temp -> id;
            smallNode -> at = temp -> at;
            smallNode -> bt = temp -> bt;
            smallNode -> ct = temp -> ct;
            smallNode -> tat = temp -> tat;
            smallNode -> wt = temp -> wt;
            temp -> id = id;
            temp -> at = at;
            temp -> bt = bt;
            temp -> ct = ct;
            temp -> tat = tat;
            temp -> wt = wt;
        }
        temp = temp -> next;
    }
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
        newNode -> tempbt = newNode -> bt;
        newNode -> isCompleted = 0;

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

void Sort(){
    struct node* temp = head;
    while(temp != NULL){
        struct node* smallNode = temp;
        struct node* temp2 = temp -> next;
        while(temp2 != NULL){
            if((temp2 -> at) < (smallNode -> at))
                smallNode = temp2;
            temp2 = temp2 -> next;
        }

        if(temp != smallNode){
            int id = smallNode -> id;
            int at = smallNode -> at;
            int bt = smallNode -> bt;
            int tempbt = smallNode -> tempbt;
            smallNode -> id = temp -> id;
            smallNode -> at = temp -> at;
            smallNode -> bt = temp -> bt;
            smallNode -> tempbt = temp -> tempbt;
            temp -> id = id;
            temp -> at = at;
            temp -> bt = bt;
            temp -> tempbt = tempbt;
        }
        temp = temp -> next;
    }
}

void CheckArrival(struct node* array[], int currentTime){
    struct node* temp = head;
    while(temp != NULL){
        if(temp -> at <= currentTime && temp -> isCompleted == 0){
            pointer++;
            array[pointer] = temp;
        }
        temp = temp -> next;
    }
}

struct node* ReturnSmallest(struct node* array[]){
    struct node* smallNode = array[0];
    for(int i = 1; i <= pointer; i++){
        if(array[i] -> tempbt < smallNode -> tempbt)
            smallNode = array[i];
    }
    return smallNode;
}

int AllExecuted(){
    struct node* temp = head;
    while(temp != NULL){
        if(temp -> isCompleted == 0)
            return 0;
        temp = temp -> next;
    }
    return 1;
}

void ExecuteSJF(){
    Sort();
    int currentTime = head -> at;
    struct node* array[processes];
    while(AllExecuted() == 0){
        CheckArrival(array, currentTime);
        struct node* process = ReturnSmallest(array);
        currentTime++;
        if(process -> tempbt != 1)
            process -> tempbt -= 1;
        else{
            process -> tempbt = 0;
            process -> isCompleted = 1;
            process -> ct = currentTime;
            process -> tat = (process -> ct) - (process -> at);
            process -> wt = (process -> tat) - (process -> bt);
        }
        pointer = -1;
    }
    display(head);
}

void main(){
    printf("Shortest Job First - Preemptive\n");
    printf("-------------------------------\n\n");
    printf("Enter The Total Number of Processes: ");
    scanf("%d", &processes);

    Create();
    ExecuteSJF();
}