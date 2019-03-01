/*
Lab5Route.c
Created by: Chris Nelson on Friday, March 1, 2019

Compile by: gcc -o lab5 Lab5Route -lpthread -std=C99
Run by: ./lab5 {machine ID (0-3)} {num of machines} machines
ex: ./lab5 0 4 machines
*/

#define numNodes 5;

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

/* define Machine struct */
struct 


/* Global Variable */
int n;
int* matrix;
Machine linux_machines[numNodes];
int sock;
pthread_mutex_t lock;



/* Link State Thread */
void* linkState(void* a){

}

/* Recieve Info Thread */
void* recieveInfo(void* b){

}

/* print out table */
void printTable(void){

}
/* Main Function */
int main(int argc, char *argv[]){

}
