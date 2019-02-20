/*
Chris Nelson
COEN 146L - Lab 1
Fri 2:15-5:00

Description:
Program reads from input file src.txt and copies it to an output file dest.txt
Utilizes fopen(),fwrite(), fread(),feof(), and fclose()
*/


#include <stdio.h>
#include <stdlib.h>
#define BUFSIZE 10

int main(int argc, char *argv[]){
	FILE * inFile;
	FILE * outFile;
	char buffer[10];
	int inBytes;

	inFile = fopen(argv[1], "r");
	outFile=fopen(argv[2],"w");	
	while(!feof(inFile)){
	  inBytes= fread(buffer, 1, BUFSIZE, inFile);
	  //reads in 10 bytes from src file to buffer[]
		/*
		if(outBytes!=BUFSIZE){
			printf("%s \n", "Error reading bytes");
		}
		*/
	    
	    fwrite (buffer , 1, inBytes, outFile);
	}
	fclose(inFile);
	fclose(outFile);
}
