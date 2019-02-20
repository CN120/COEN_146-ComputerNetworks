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


/***********
 *  main
 ***********/

int main (int argc, char *argv[]) {
    /* argv
     1 - port number
     2 - IP address
     3 - input filename
     4 - output filename
     */
	int sock, portNum, nBytes;
	char buffer[10];
	struct sockaddr_in serverAddr;
	socklen_t addr_size;
	int state = 0; //state of pgrm
	int lastSend=0;
    /* start add */
	FILE *inFile; //input file
	srand(time(0));

    PACKET *pak1 = malloc(sizeof(PACKET));
    PACKET *pak2 = malloc(sizeof(PACKET));

	/* end add */

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

    /* start add */
    pak1->header.seq_ack=state;
    pak1->header.length=(sizeof(argv[4])+1);
    memcpy (pak1->data, argv[4], pak1->header.length);
    pak1->header.checksum = 0;
    pak1->header.checksum=calc_checksum(pak1, sizeof(pak1->header)+pak1->header.length);



    int resendCount=0;
    //send name of file output argv[4]
sndpckt: sendto (sock, pak1, sizeof(PACKET), 0, (struct sockaddr *)&serverAddr, addr_size);
    //printf("I didn't get to point #0\n");
    recvfrom (sock, pak2, pak1->header.length, 0,(struct sockaddr *)&serverAddr, &addr_size);
	printf("client received response\n");
    if(pak2->header.seq_ack != pak1->header.seq_ack){
        if(resendCount<3){
            resendCount++;
            printf("I didn't get to point #1\n");
            goto sndpckt;
        }
        else{
            exit(EXIT_FAILURE);
        }
    }
    state = state==0 ? 1:0; //flips state

    printf("I got to point #1\n");


	//argv[3] is input file
	inFile = fopen(argv[3], "r");
    /* end add */
	while (1) {

	    fread(pak1->data, 1, sizeof(pak1->data), inFile);
	    pak1->header.seq_ack=state;
	    pak1->header.length=(sizeof(pak1->data));
		pak1->header.checksum=0;
	    pak1->header.checksum=calc_checksum(pak1, sizeof(pak1->header)+pak1->header.length);
		resendCount=0;
		if(!feof(inFile)) {
			printf ("Sending: %s with seq num: %d\n", pak1->data, pak1->header.seq_ack);

send:		sendto (sock, pak1, sizeof(PACKET), 0, (struct sockaddr *)&serverAddr, addr_size); //send bytes from file

		    /*  receive  */
			recvfrom (sock, pak2, sizeof(PACKET), 0, (struct sockaddr *)&serverAddr, &addr_size);
			if(pak2->header.seq_ack != pak1->header.seq_ack){
			  if(resendCount<3){
			      resendCount++;
			      goto send;
			  }
			  else{
			      exit(EXIT_FAILURE);
			  }
			}
			state = (state==0 ? 1:0); //flips state
		}
		else if(lastSend==0) {
			lastSend=1;
			goto send;
		}
  		else{
	        memset(pak1->data, '\0',sizeof(pak1->data));
	        pak1->header.length=0;
			pak1->header.checksum=0;
	        pak1->header.checksum=calc_checksum(pak1, sizeof(pak1->header)+pak1->header.length);
	        sendto (sock, pak1, sizeof(PACKET), 0, (struct sockaddr *)&serverAddr, addr_size);
	        fclose(inFile);
	        return 0;
		}
	}
}
