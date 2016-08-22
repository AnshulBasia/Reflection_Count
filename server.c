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
	char time[1024];//time
	int rc;  //an even non neg. integer
};

int main( int argc, char *argv[] ) 
{
	int sockid,port_no,newsockid;
	//data d;
	socklen_t client;
	struct sockaddr_in serv_addr,client_addr;
	int n;
	struct data* d=(struct data*)malloc(sizeof(struct data));

	sockid = socket(AF_INET, SOCK_DGRAM, 0); //Creating a socket point of datagram type

	if(sockid<0)
	{
		perror("Error while opening socket");
		exit(1);
	}

	bzero((char*) &serv_addr,sizeof(serv_addr));
	printf("Enter port number ");
	char port[1000];
	gets(port);
	int portno=atoi(port);
	serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    char seq[1024];

    //ATTEMPT TO BIND THE HOST
    if(bind(sockid,(struct sockaddr *) &serv_addr,sizeof(serv_addr))<0)
    {
    	perror("There's some error while binding the port provided");
    	exit(1);
    }
    //Wait for incoming connections i.e. start listening to clients

    socklen_t client_ad;
    
    for(;;)
    {	printf("hey\n");
    	client_ad=sizeof(client_addr);	//size of in-out parameter
    	//printf("%n\n",recvfrom(sockid,&d,sizeof(d),0,(struct sockaddr *) &client_addr, &client_ad));
    	
    	if(recvfrom(sockid,d,1024+sizeof(*d),0,(struct sockaddr *) &client_addr, &client_ad)<0)
    	{
    		perror("receiving falied");
    	}
    	
    	printf("Received: %d\n",d->sequence_number );
        
        printf("%d\n",d->time);
        printf("%d\n",d->rc );
        d->rc=d->rc-1;
        n= sendto(sockid,d,1024+sizeof(*d),0,(struct sockaddr*) &client_addr,sizeof(client_addr));
    if(n<0)
    {
        perror("There was some error writing data to socket");
        exit(1);
    }
    }
}