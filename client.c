#include <stdio.h>
#include <stdlib.h>

#include <netdb.h>
#include <netinet/in.h>

#include <string.h>
#include <unistd.h>
#include <time.h>

struct data
{
	int sequence_number;  //identifier
	time_t time;//time
	int rc;  //an even non neg. integer
};
int main(int argc, char *argv[])
{
	int sockid,portno;		//Socket Descriptor like a file handle and port number to be used
	struct sockaddr_in serv_addr;
	/*
	struct in_addr
	 {
		unsigned long s_addr; 
	 }       
     //Internet address (32 bits) 

	struct sockaddr_in
	{
		unsigned short sin_family;   
		//Internet protocol (AF_INET) 
		unsigned short sin_port;     
		 //Address port (16 bits) 
		struct in_addr sin_addr;   
		// Internet address (32 bits) 
		char sin_zero[8];  
		 //Not used 
	}
	*/
	struct hostent *server;  //used to represent an entry n host's database

	if(argc<3)
	{
		fprintf(stderr,"To use type->  %s hostname port_number\n",argv[0]);
		exit(0);
	}

	portno=atoi(argv[2]);  //From String to integer

	sockid = socket(AF_INET, SOCK_DGRAM, 0); //Creating a socket point of datagram type
	if(sockid<0) //checking validity of sockid i.e. socket descriptor
	{
		perror("There's some error in opening socket");
		exit(1);
	}

	server=gethostbyname(argv[1]);  //From the host name provided, we get the server address using hostnet's database and gethostbyname function
	
	if(server==NULL) //checking host name
	{
		fprintf(stderr, "There's no such host which you are trying to open connected to this network\n");
	}

	bzero((char *) &serv_addr,sizeof(serv_addr));  //initializing server address with null bytes
	serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length); //to copy server adress from host given to serv_addr
    serv_addr.sin_port = htons(portno);

    //ATTEMPT TO CONNECT TO SERVER
    if (connect(sockid, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) 
    {
      perror("There's some error in connecting to server");
      exit(1);
    }

    //if connection successful->
    //ASK for the message from user which is to be sent to server
    printf("Please enter the message:\n");
    data d;
    printf("Enter the identifier for message\n");
    d.sequence_number=atoi(stdin);
    printf("Enter the reflection count\n");
    d.rc=atoi(stdin);
    d.time=time(0);

    int n=write(sockid,d,sizeof(d));

    if(n<0)
    {
    	perror("There was some error writing data to socket");
    	exit(1);
    }

    //read back from server
    data d2;
    n=read(sockid,d2,sizeof(d2));
    if(n<0)
    {
    	perror("there was error while reading back from socket we sent through");
    	exit(1);
    }

    printf("%s,%d\n",d.sequence_number,d.time );


}