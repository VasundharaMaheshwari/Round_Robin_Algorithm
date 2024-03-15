#include <stdio.h>

#define SIZE 6
#define ATTRIBUTES 5

int ReadyQ[SIZE];
int front = -1;
int rear = -1;

int executed[SIZE] = {0};
int comp_time[SIZE]; // Removed initialization

int Process[SIZE][ATTRIBUTES]; // 0 AT 1 BT 2 WT 3 TAT 4 Remainder Time
int TQ, CLK;

int isFull() {
    if ((rear + 1) % SIZE == front)
        return 1;
    return 0;
}

int isEmpty() {
    if (front == -1)
        return 1;
    return 0;
}

void enQ(int process_no) {
    if (!isFull() || rear == -1) {
        rear = (rear + 1) % SIZE;
        ReadyQ[rear] = process_no;
        if (front == -1)
            front = 0; // Update front if the queue was empty
    }
}

int deQ() {
    if (!isEmpty() && front != -1) {
        int pid = ReadyQ[front];
        ReadyQ[front] = -1;
        if (front == rear) {
            front = -1;
            rear = -1;
        } else {
            front = (front + 1) % SIZE;
        }
        return pid;
    }
    return -1;
}

void printQ() {
    int i;
    printf("\nReadyQ\n");
    for (i = 0; i < SIZE; i++) {
        printf("%d ", ReadyQ[i]);
    }
    printf("\n");
}

void initStuff() {
    int i, j;
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < ATTRIBUTES - 3; j++) {
            printf("Enter %s for %d - ", j==0?"Arrival Time":"Burst Time", i);
            scanf("%d", &Process[i][j]);
        }
    }
    for (i = 0; i < SIZE; i++) {
        Process[i][4] = Process[i][1];
        Process[i][2] = 0;
        Process[i][3] = 0;
    }
    printf("Enter Time Quantum - ");
    scanf("%d", &TQ);

    printf("\nInitialized - \n\n");
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < ATTRIBUTES; j++) {
            printf("%5d ", Process[i][j]);
        }
        printf("\n");
    }
}

void loader(int current_time) {
    int i;
    for (i = 0; i < SIZE; i++) {
        if (Process[i][0] == current_time) {
            enQ(i);
            // printQ();
            // return;
        }
    }
}

void roundRobin() {
    CLK = 0;
    int i, j;
    int track = 0;
    int tq_track = 0;
    int index = -1;
    do {
        loader(CLK);
        if (CLK == 0 || index == -1) {
            index = deQ();
        }
        if (index == -1) {
            CLK++;
            continue;
        }
        printQ();
        printf("\nStarted process %d \n", index);
        for (i = 0; i < SIZE; i++) {
            if (i != index && !executed[i] && Process[i][0] <= CLK) { // Corrected condition
                Process[i][2]++;
            }
        }
        Process[index][4]--; // Update remaining time for executing process
        tq_track++;
        CLK++;
        if (tq_track == TQ || Process[index][4] <= 0) {
            tq_track = 0;
            if (Process[index][4] > 0) {
                enQ(index);
            } else {
                printf("\nFinished process %d \n", index);
                Process[index][4] = 0;
                executed[index] = 1;
                comp_time[index] = CLK; // Record completion time
                track++;
            }
            index = deQ();
        }
    } while (track < SIZE);
    for (i = 0; i < SIZE; i++) {
        printf("\nCompleted %d in %d time \n", i, comp_time[i]);
        printf("\nProcess %d waited for %d time \n", i, Process[i][2]);
        Process[i][3] = comp_time[i] - Process[i][0]; // Calculate turnaround time
        Process[i][2] = Process[i][3] - Process[i][1]; // Calculate wait time
    }
    printf("\nFinal CLK - %d ", CLK);
    printf("\n");
    printf("\nFinal - \n\n");
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < ATTRIBUTES; j++) {
            printf("%5d ", Process[i][j]);
        }
        printf("\n");
    }
}

int main() {
    initStuff();
    roundRobin();
    return 0;
}
