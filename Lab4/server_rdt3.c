
/********************
 * COEN 146, UDP example, server
 ********************/

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "tfv2.h"

/********************
 * main
 ********************/

int main (int argc, char *argv[]) {
	int sock, nBytes;
	struct sockaddr_in serverAddr, clientAddr;
	struct sockaddr_storage serverStorage;
	socklen_t addr_size, client_addr_size;
	int i;
  	int pak_checksum;
	int local_checksum;
	int state = 0; //state of pgrm

	//random number;
	srand (time(NULL));
	int randNum;

	FILE *outFile;
	PACKET *s_pak = malloc(sizeof(PACKET));

	/* start bracket */
	{
    if (argc != 2)
    {
        printf ("need the port number\n");
        return 1;
    }

	// init
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons ((short)atoi (argv[1]));
	serverAddr.sin_addr.s_addr = htonl (INADDR_ANY);
	memset ((char *)serverAddr.sin_zero, '\0', sizeof (serverAddr.sin_zero));
	addr_size = sizeof (serverStorage);

	// create socket
	if ((sock = socket (AF_INET, SOCK_DGRAM, 0)) < 0){
		printf ("socket error\n");
		return 1;
	}

	// bind
	if (bind (sock, (struct sockaddr *)&serverAddr, sizeof (serverAddr)) != 0){
		printf ("bind error\n");
		return 1;
	}
	}
	/* end bracket */

recv:recvfrom (sock, s_pak, sizeof(PACKET), 0, (struct sockaddr *)&serverStorage, &addr_size);
    pak_checksum = s_pak->header.checksum;
    s_pak->header.checksum = 0;
	local_checksum = calc_checksum(s_pak, sizeof(HEADER)+s_pak->header.length);

	if(local_checksum!=pak_checksum || state!=s_pak->header.seq_ack){
		printf("server inside if stmt\n");
		s_pak->header.seq_ack = (s_pak->header.seq_ack==0)? 1: 0; //flip ack to negative ack
		sendto (sock, s_pak, sizeof(PACKET), 0, (struct sockaddr *)&serverStorage, addr_size);
		printf("server hit recv\n");
		goto recv;
	}

	/* send back positive ack */
	sendto (sock, s_pak, sizeof(PACKET), 0, (struct sockaddr *)&serverStorage, addr_size);

	outFile = fopen(s_pak->data, "w");

/* while loop */
	while (1){
		state = (state==0) ? 1:0; //flips state
		// receive  datagrams
		//printf("server inside while loop\n");

recvAgain:	nBytes = recvfrom (sock, s_pak, sizeof(PACKET), 0, (struct sockaddr *)&serverStorage, &addr_size);

		randNum= rand()%100;
		if (randNum<10){
		   //10% of the time, an acknowledgement will be skipped
		  printf("%s\n","not sening ack" );
		  goto recvAgain;
		}


		printf ("received: '%s' seq num: %d and state: %d\n", s_pak->data, s_pak->header.seq_ack, state);
		//printf("server recieved datagram\n");
		pak_checksum = s_pak->header.checksum;
    	s_pak->header.checksum = 0;
		local_checksum = calc_checksum(s_pak, sizeof(HEADER)+s_pak->header.length);

		if(s_pak->header.length == 0){ // && local_checksum == pak_checksum && (state==s_pak->header.seq_ack)){
			fclose(outFile);
			printf("%s\n", "end of file");
			sendto (sock, s_pak, sizeof(PACKET), 0, (struct sockaddr *)&serverStorage, addr_size); //send ack back with empty packet
			return 0;
		}

		else if(local_checksum != pak_checksum || (state!=s_pak->header.seq_ack) ){
			printf("FAILURE: Checksum or Sequence Number\n");
			s_pak->header.checksum = 0;
			printf("Checksum: %d vs %d\n",pak_checksum, local_checksum);
			printf("Sequence: %d vs %d\n\n", s_pak->header.seq_ack, state);
			s_pak->header.seq_ack = (s_pak->header.seq_ack==0? 1: 0); //flips ack to negative ack
			sendto (sock, s_pak, sizeof(PACKET), 0, (struct sockaddr *)&serverStorage, addr_size); //send neg ack
			goto recvAgain;
		}
		else{
			printf("writing to file...\n");
			fwrite(s_pak->data, 1, s_pak->header.length, outFile);
			sendto (sock, s_pak, sizeof(PACKET), 0, (struct sockaddr *)&serverStorage, addr_size); //send ack
		}
	}
	return 0;
}
