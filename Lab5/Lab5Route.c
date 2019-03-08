/*
Lab5Route.c
Created by: Chris Nelson on Friday, March 1, 2019

Compile by: gcc -o lab5 Lab5Route.c -lpthread -std=C99
Run by: ./lab5 {machine ID (0-3)} {num of machines} {cost file} {machines file}
ex: ./lab5 0 4 cost machines

1)router ID = argv[1]⟶ tells what node the program is being run
2)num of nodes = argv[2]
3)cost = argv[3]
4)machine = argv[4]
*/

#define numNodes 4
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> //sleep
#include <time.h> //random
#include <pthread.h> //threads library

/* define Machine struct */
typedef struct{
    char name[50];
    char ip[50];
    int port;
}Machine;


/* Global Variable */
int cost_matrix[numNodes][numNodes];
Machine linux_machines[numNodes];
int srcID;
int sock;
pthread_mutex_t lock;



/* Link State Thread - Thread 3 */
void* linkState(void* args){
    int srcNode;
    int leastDistanceArray[numNodes];
    bool visited[numNodes];
    while true{
        for(srcNode=0; srcNode<numNode; ++srcNode){
            leastDistanceArray[sourceNode] =
        }
    }
    sleep((rand()%10+10));

}

/* Recieve Info Thread - Thread 2 */
void* recieveInfo(void* b){

}

/* print out table */
void printTable(void){
    int i;
    for(i=0; i<4; ++i){
        printf("%d %d %d %d\n", cost_matrix[i][0], cost_matrix[i][1], cost_matrix[i][2], cost_matrix[i][3]);
    }
}
/* Main Function */
int main(int argc, char *argv[]){
    /* setup threads */
    pthread_t thr2, thr3;
    pthread_create(&thr3, NULL, linkState, NULL);
    pthread_create(&thr2, NULL, recieveInfo, NULL);
    /* end thread setup */

    srand(time(NULL));  //seed random
    srcID = atoi(argv[1]);
    /* Fill cost_matrix */{
    FILE* fp = fopen(argv[3], "r"); //open cost file for reading

    int i;
    for(i=0; i<argv[2]; ++i){
        fscanf(fp, "%d %d %d %d", &cost_matrix[i][0], &cost_matrix[i][1], &cost_matrix[i][2], &cost_matrix[i][3]);
    }
    fclose(fp);
    }
    printTable();

    /* Fill Machine structs */{
    fp = fopen(argv[4], "r");   //open machines file for reading
    for(i=0; i<numNodes; ++i){
        fscanf(fp, "%s %s %d", linux_machines[i].name, linux_machines[i].ip, linux_machines[i].port)
    }
    }




}
