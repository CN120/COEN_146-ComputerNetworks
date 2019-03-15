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
pthread_mutex_t myMutex;
pthread_mutex_init(&myMutex, NULL);

/* network globals */
int sock;
struct sockaddr_in listen_addr;
struct sockaddr_in recAddr;
socklen_t addr_size;
/********************/
/* FUNCTIONS START */
/*******************/
/* Link State Thread - Thread 3 */
void* linkState(void* args){
    int srcNode;
    int leastDistanceArray[numNodes];
    memset(leastDistanceArray, INT_MAX, sizeof(leastDistanceArray));
    bool visited[numNodes];
    while (true){
        for(srcNode=0; srcNode < numNodes; ++srcNode){
            memset(visited, false, sizeof(visited)); //should work because bools are ints in 'C'
            memcpy(leastDistanceArray, cost_matrix[srcNode], sizeof(cost_matrix[srcNode]));
            visited[srcNode]=true;
            for(int a=1; a<numNodes-1; ++a){
                int minIndex;
                int minCost = INT_MAX;
                for(int b=0; b<numNodes; ++b){
                    if(!visited[b] && cost_matrix[srcNode][b] < minCost){
                        minCost = cost_matrix[srcNode][b];
                        minIndex = b;
                    }
                }
                
            }

        }
    }
    sleep((rand()%11+10));

}

/* Recieve Info Thread - Thread 2 */
void* recieveInfo(void* b){
    int pack2[3];
    recvfrom(sock,pack2, sizeof(pack2),0, NULL, NULL);
    pthread_lock(&myMutex); //lock
    cost_matrix[pack2[0]][pack2[1]] = pack2[2];
    cost_matrix[pack2[1]][pack2[0]] = pack[2];
    pthread_unlock(&myMutex); //unlock
}

/* print out table */
void printTable(void){
    pthread_lock(&myMutex); //lock
    int i;
    for(i=0; i<4; ++i){
        printf("%d %d %d %d\n", cost_matrix[i][0], cost_matrix[i][1], cost_matrix[i][2], cost_matrix[i][3]);
    }
    pthread_unlock(myMutex);    //unlock
}
/* Main Function */
int main(int argc, char *argv[]){
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
        fscanf(fp, "%s %s %d", linux_machines[i].name, linux_machines[i].ip, &linux_machines[i].port);
    }
    fclose(fp);
/*----------------------------------------------------------------------------------------*/

/* Network setup */
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

    srand(time(NULL));  //seed random
    myID = atoi(argv[1]); //sets the program number of the program being run1
    /* setup threads */
    pthread_t thr2, thr3;
    pthread_create(&thr3, NULL, linkState , NULL);
    pthread_create(&thr2, NULL, recieveInfo, NULL);

/*------------------------------------------------------------------------------------*/
    
    //input: change cost table from command line
    printf("please enter an update in the form:\nmachine1 machine2 new_cost")
    pack[3]; //update array packet
    pthread_lock(&myMutex); //lock
    fscan("%d %d %d",pack[0], pack[1], pack[2])
    
    cost_matrix[pack[0]][pack[1]] = pack[2];
    cost_matrix[pack[1]][pack[0]] = pack[2];
    pthread_unlock(&myMutex); //unlock
    //end input
    recAddr.sin_family = AF_INET;
    //send updates
    for(int i=0; i<numNodes; ++1){
        if(i == myID){
            continue;
        }
        
        recAddr.sin_port = htons (linux_machines[i].port);
        inet_pton (AF_INET, linux_machines[i].ip, &recAddr.sin_addr.s_addr);
        memset (recAddr.sin_zero, '\0', sizeof (recAddr.sin_zero));
        addr_size = sizeof recAddr;
        sendto (sock, pack, sizeof(pack), 0, (struct sockaddr *)&recAddr, addr_size); //
    }
    
    


}
