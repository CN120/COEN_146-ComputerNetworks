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
#include <stdbool.h>
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


/* Global Variables */
int cost_matrix[numNodes][numNodes];
Machine linux_machines[numNodes];
int myID;
pthread_mutex_t lock;

/* network globals */
int sock;
struct sockaddr_in listen_addr;
socklen_t addr_size;

/* Link State Thread - Thread 3 */
void* linkState(void* args){
    int srcNode;
    int leastDistanceArray[numNodes];
    bool visited[numNodes];
    while (true){
        for(srcNode=0; srcNode < numNodes; ++srcNode){
            memset(visited, false, sizeof(visited)); //should work because bools are ints in 'C'
            leastDistanceArray = {cost_matrix[srcNode][0], cost_matrix[srcNode][1], cost_matrix[srcNode][2], cost_matrix[srcNode][3]}
        }
    }
    sleep((rand()%10+10));

}

/* Recieve Info Thread - Thread 2 */
void* recieveInfo(void* b){

}

void* test1(void* c){
    printf("test 1\n");
}
void* test2(void* d){
    printf("test2\n");
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
/* start collapse */{
    srand(time(NULL));  //seed random
    myID = atoi(argv[1]); //sets the program number of the program being run1
    /* setup threads */
    pthread_t thr2, thr3;
    pthread_create(&thr3, NULL, test1, NULL);
    pthread_create(&thr2, NULL, test2, NULL);
    /* end thread setup */



    /* Fill cost_matrix */
    FILE* fp = fopen(argv[3], "r"); //open cost file for reading

    int i;
    for(i=0; i<atoi(argv[2]); ++i){
        fscanf(fp, "%d %d %d %d", &cost_matrix[i][0], &cost_matrix[i][1], &cost_matrix[i][2], &cost_matrix[i][3]);
    }
    fclose(fp);

    printTable();

    /* Fill Machine structs */
    fp = fopen(argv[4], "r");   //open machines file for reading
    for(i=0; i<numNodes; ++i){
        fscanf(fp, "%s %s %d", linux_machines[i].name, l  inux_machines[i].ip, &linux_machines[i].port);
    }
    } /* end collapse */

/* Network setup */{}
    listen_addr.sin_family = AF_INET;
    listen_addr.sin_port = linux_machines[myID].port;   //port this node will listen on
    listen_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    memset((char*)listen_addr.sin_zero, '\0', sizeof(listen_addr));
    addr_size = sizeof(listen_addr);
    int e1,e2;
    e1 = sock = socket(AF_INET, SOCK_DGRAM, 0);
    e2 = bind(sock, (struct sockaddr*)&listen_addr, sizeof(listen_addr));
    if(e1==-1 || e2==-1){
        perror("creating socket or binding had an issue");
    }



}
