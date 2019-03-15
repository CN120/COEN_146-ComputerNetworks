 /**************************
	socket example, client
	Winter 2019
 **************************/

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

int main (int, char *[]);


/********************
 *	main
 ********************/
int main (int argc, char *argv[])
{
	int i;
	int sockfd = 0, n = 0;
	char buff[10];
	char *p;
	FILE *inFile;
	int bytes_Read;
	struct sockaddr_in serv_addr;

	if (argc != 5)
	{
		printf ("Usage: %s <ip of server> \n",argv[0]);
		return 1;
	}

	// set up
	memset (buff, '0', sizeof (buff));
	memset (&serv_addr, '0', sizeof (serv_addr));

	// open socket
	if ((sockfd = socket (AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf ("Error : Could not create socket \n");
		return 1;
	}

	// set address
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons (atoi(argv[1]));
	//argv[2] is ip address
	if (inet_pton (AF_INET, argv[2], &serv_addr.sin_addr) <= 0)
	{
		printf ("inet_pton error occured\n");
		return 1;
	}

	// connect
	if (connect (sockfd, (struct sockaddr *)&serv_addr, sizeof (serv_addr)) < 0)
	{
		printf ("Error : Connect Failed \n");
		return 1;
	}

	write(sockfd, argv[4], strlen(argv[4])+1);
	read(sockfd, buff, sizeof (buff));
	if(buff[0]=='\0'){
		printf ("good acknowledgement\n");
	}
	else{
		printf ("bad acknowledgement\n");
	}

	//argv[3] is input file
	inFile = fopen(argv[3], "r");

	while(!feof(inFile)){ /* this */
		bytes_Read = fread(buff, 1, sizeof (buff), inFile); /* this */
		write(sockfd, buff, bytes_Read);
	}
	fclose(inFile);


	close (sockfd);

	return 0;

}
