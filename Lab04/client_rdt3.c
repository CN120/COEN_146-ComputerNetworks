/*****************************
 * COEN 146, UDP, client
 *****************************/

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include "tfv2.h"
#include <time.h>
#include <sys/select.h>
#include <fcntl.h>


/***********
 *  main
 ***********/

int main (int argc, char *argv[]) {
    /* argv[]
     1 - port number
     2 - IP address
     3 - input filename
     4 - output filename
     */
	int sock, portNum, nBytes;
	struct sockaddr_in serverAddr;
	socklen_t addr_size;
	int state = 0; //state of pgrm
	int bytesRead=0;
	srand (time(NULL));
	int randNum;
	FILE *inFile; //input file
	struct timeval tv; //timer
	int rv; 	//select return value

    PACKET *pak1 = malloc(sizeof(PACKET));
    PACKET *pak2 = malloc(sizeof(PACKET));

	//timer setup
	fd_set readfds;
	fcntl(sock, F_SETFL, O_NONBLOCK);

/* start bracket */
	{
	if (argc != 5){
		printf ("need the port number, IP address, input file, and output file\n");
		return 1;
	}

	// configure address

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons (atoi (argv[1]));
	inet_pton (AF_INET, argv[2], &serverAddr.sin_addr.s_addr);
	memset (serverAddr.sin_zero, '\0', sizeof (serverAddr.sin_zero));
	addr_size = sizeof serverAddr;


	/*Create UDP socket*/
    if ((sock = socket (AF_INET, SOCK_DGRAM, 0)) < 0){
		printf ("socket error\n");
		return 1;
	}
	}
/* end bracket */


    pak1->header.seq_ack=state;
    pak1->header.length=(sizeof(argv[4])+1);
    memcpy (pak1->data, argv[4], pak1->header.length);
    pak1->header.checksum = 0;
    pak1->header.checksum=calc_checksum(pak1, sizeof(HEADER)+pak1->header.length);


    int resendCount=0;
    //send name of file output argv[4]
sndpckt: sendto (sock, pak1, sizeof(PACKET), 0, (struct sockaddr *)&serverAddr, addr_size);
/* start & check timer */
	// start before calling select
	FD_ZERO (&readfds);
	FD_SET (sock, &readfds);
	// set the timer
	tv.tv_sec = 10;
	tv.tv_usec = 0;
	//get select value
	rv = select (sock + 1, &readfds, NULL, NULL, &tv); // call select
/*--------------------------------------------------------------------------*/
/*  receive  */
	if(rv==0){
		goto sndpckt;
	}
	else if(rv==1){
		recvfrom (sock, pak2, pak1->header.length, 0,(struct sockaddr *)&serverAddr, &addr_size);
	}
    if(pak2->header.seq_ack != pak1->header.seq_ack){
        if(resendCount<3){
            resendCount++;
            goto sndpckt;
        }
        else{
            exit(EXIT_FAILURE);
        }
    }
    state = state==0 ? 1:0; //flips state


	//argv[3] is input file
	inFile = fopen(argv[3], "r");

/* while loop */
	while (1) {
		resendCount=0;
		memset(pak1->data, '\0',SIZE+1);
	    bytesRead=fread(pak1->data, 1, sizeof(pak1->data), inFile);
	    pak1->header.seq_ack=state;


		if(bytesRead!=0) {
		    pak1->header.length=(bytesRead);
			randNum= rand()%100;
			if (randNum<10){
				pak1->header.checksum=0;
				printf("sending: '%s'  with Checksum ERROR\n", pak1->data);
			}
			else if(randNum<20){goto skip;}
			else {
resend:			pak1->header.checksum=0;
				pak1->header.checksum=calc_checksum(pak1, sizeof(HEADER)+pak1->header.length);
				printf ("Sending: '%s'  seq num: %d\n", pak1->data, pak1->header.seq_ack);
			}

			sendto (sock, pak1, sizeof(PACKET), 0, (struct sockaddr *)&serverAddr, addr_size); //send bytes from file
skip:
/* start & check timer */
			// start before calling select
			FD_ZERO (&readfds);
			FD_SET (sock, &readfds);
			// set the timer
			tv.tv_sec = 10;
			tv.tv_usec = 0;
			//get select value
			rv = select (sock + 1, &readfds, NULL, NULL, &tv); // call select
/*  receive  */
			if(rv==0){
				goto resend;
			}
			else if(rv==1){
				recvfrom (sock, pak2, sizeof(PACKET), 0, (struct sockaddr *)&serverAddr, &addr_size);
			}
			if(pak2->header.seq_ack != pak1->header.seq_ack){
			  if(resendCount<3){
			      resendCount++;
			      goto resend;
			  }
			  else{
				  printf("3 consecutive send errors\n");
				  printf("TERMINATING PROGRAM\n");
				  return 1;
			  }
			}
			state = (state==0 ? 1:0); //flips state
		}
  		else{
			//send empty packet when finihed reading
	        memset(pak1->data, '\0',sizeof(pak1->data));
	        pak1->header.length=0;
	        pak1->header.checksum=0;
			resendCount=0;
resendLast: sendto (sock, pak1, sizeof(PACKET), 0, (struct sockaddr *)&serverAddr, addr_size);
	        fclose(inFile);
/* start & check timer */
			// start before calling select
			FD_ZERO (&readfds);
			FD_SET (sock, &readfds);
			// set the timer
			tv.tv_sec = 10;
			tv.tv_usec = 0;
			//get select value
			rv = select (sock + 1, &readfds, NULL, NULL, &tv); // call select
/*  receive  */
			if(rv==0){
				goto rsdCount;
			}
			else if(rv==1){
				recvfrom (sock, pak2, sizeof(PACKET), 0, (struct sockaddr *)&serverAddr, &addr_size);
			}
			if(pak2->header.seq_ack != pak1->header.seq_ack){
rsdCount: 	  if(resendCount<3){
	      		  resendCount++;
			      goto resendLast;
			  }
			  else{
				  printf("3 consecutive sendLast errors\n");
				  printf("TERMINATING PROGRAM\n");
				  return 1;
			  }
			}
	        return 0;
		}
	}
}
