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
	char time[50];//time
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
    client_ad=sizeof(client_addr);  //size of in-out parameter
    while(1>0)
    {

        if(recvfrom(sockid,d,1024+sizeof(*d),0,(struct sockaddr *) &client_addr, &client_ad)<0)
            {
                perror("receiving falied");
            }
            printf("Received\n");
            printf(" Packet size being received is  %d\n",d->rc );
            printf(" press 1 to continue... \n");
            int p=d->rc;
          
            
        int i=0;
        for(;;)
        {	
        	client_ad=sizeof(client_addr);	//size of in-out parameter
        	//printf("%n\n",recvfrom(sockid,&d,sizeof(d),0,(struct sockaddr *) &client_addr, &client_ad));
        	
        	if(recvfrom(sockid,d,p+sizeof(*d),0,(struct sockaddr *) &client_addr, &client_ad)<0)
        	{
        		perror("receiving falied");
        	}
        	printf("Received\n");
        	printf(" %d\n",d->sequence_number );
            
            printf("%d\n",d->time);
            printf("%d\n",d->rc );
            d->rc=d->rc-1;
            n= sendto(sockid,d,p+sizeof(*d),0,(struct sockaddr*) &client_addr,sizeof(client_addr));
            if(d->rc==1){i++;} 
            if(i==50){break;}
            if(n<0)
            {
                perror("There was some error writing data to socket");
                exit(1);
            }
        }
        printf("Completed %d\n",p);
        if(p>=1500){break;}
    }
}

/* 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 4 4 4 4 4 4 4 4 4 4 4 4 4 4
 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 5 5 5 5 5 5 5 5 5 5 5 5 5 5 5
 5 5 5 5 5 5 5 5 5 5 5 5 5 5 5 5 5 5 5 5 5 5 5 5 5 5 5 5 5 5 5 5 5 5 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6
 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7
 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 8 8 8 8 8 8 8 8 8 8 8 8 8 8 8 8 8 8 8 8 8 8 8
 8 8 8 8 8 8 8 8 8 8 8 8 8 8 8 8 8 8 8 8 8 8 8 8 9 9 9 9 9 9 9 9 9 9 9 9 9 9 9 9 9 9 9 9 9 9 9 9 9 9
 9 9 9 9 9 9 9 9 9 9 9 9 9 9 9 9 9 9 9 9 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10
 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11
 11 11 11 11 11 11 11 11 11 12 12 12 12 12 12 12 12 12 12 12 12 12 12 12 12 12 12 12 12 12 12 12 12 12 12 12 12 12 12 12 12 12 12 12 12 12 12 12 12 12
 12 12 12 12 12 12 12 13 13 13 13 13 13 13 13 13 13 13 13 13 13 13 13 13 13 13 13 13 13 13 13 13 13 13 13 13 13 13 13 13 13 13 13 13 13 13 13 13 13 13
 13 13 13 13 13 13 14 14 14 14 14 14 14 14 14 14 14 14 14 14 14 14 14 14 14 14 14 14 14 14 14 14 14 14 14 14 14 14 14 14 14 14 14 14 14 14 14 14 14 14
 14 14 14 14 14 14 15 15 15 15 15 15 15 15 15 15 15 15 15 15 15 15 15 15 15 15 15 15 15 15 15 15 15 15 15 15 15 15 15 15 15 15 15 15 15 15 15 15 15 15
 15 15 15 15 15 15 16 16 16 16 16 16 16 16 16 16 16 16 16 16 16 16 16 16 16 16 16 16 16 16 16 16 16 16 16 16 16 16 16 16 16 16 16 16 16 16 16 16 16 16
 16 17 17 17 17 17 17 17 17 17 17 17 17 17 17 17 17 17 17 17 17 17 17 17 17 17 17 17 17 17 17 17 17 17 17 17 17 17 17 17 17 17 17 17 17 17 17 17 18 18
 18 18 18 18 18 18 18 18 18 18 18 18 18 18 18 18 18 18 18 18 18 18 18 18 18 18 18 18 18 18 18 18 18 18 18 18 18 18 18 18 18 18 18 19 19 19 19 19 19 19





 2 2 2 2 2 2 2 2 2 2 2 3 3 3 3 3 3 3 3 3 3 3 4 4 4 4 4 4 4 4 4 4 4 5 5 5 5 5 5 5 5 5 5 5 6 6 6 6 6 6
 6 6 6 6 6 7 7 7 7 7 7 7 7 7 7 7 8 8 8 8 8 8 8 8 8 8 8 9 9 9 9 9 9 9 9 9 9 9 10 10 10 10 10 10 10 10 10 10 10 11
 11 11 11 11 11 11 11 11 11 11 12 12 12 12 12 12 12 12 12 12 12 13 13 13 13 13 13 13 13 13 13 14 14 14 14 14 14 14 14 14 14 14 15 15 15 15 15 15 15 15
 15 15 15 16 16 16 16 16 16 16 16 16 16 16 17 17 17 17 17 17 17 17 17 17 17 17 18 18 18 18 18 18 18 18 18 18 18 19 19 19 19 19 19 19 19 19 19 19 20 20
 20 20 20 20 20 20 20 20 21 21 21 21 21 21 21 21 21 21 21 22 22 22 22 22 22 22 22 22 22 22 23 23 23 23 23 23 23 23 23 23 23 24 24 24 24 24 24 24 24 24
 24 24 25 25 25 25 25 25 25 25 25 25 25 26 26 26 26 26 26 26 26 26 26 26 27 27 27 27 27 27 27 27 27 27 27 28 28 28 28 28 28 28 28 28 28 28 29 29 29 29
 29 29 29 29 29 29 30 30 30 30 30 30 30 30 30 30 30 31 31 31 31 31 31 31 31 31 31 31 32 32 32 32 32 32 32 32 32 32 33 33 33 33 33 33 33 33 33 33 33 34
 34 34 34 34 34 34 34 34 34 34 35 35 35 35 35 35 35 35 35 35 35 36 36 36 36 36 36 36 36 36 36 36 37 37 37 37 37 37 37 37 37 37 38 38 38 38 38 38 38 38
 38 38 39 39 39 39 39 39 39 39 39 39 40 40 40 40 40 40 40 40 40 40 40 41 41 41 41 41 41 41 41 41 41 41 42 42 42 42 42 42 42 42 42 42 42 43 43 43 43 43
 43 43 43 43 43 44 44 44 44 44 44 44 44 44 44 44 45 45 45 45 45 45 45 45 45 45 46 46 46 46 46 46 46 46 46 46 46 47 47 47 47 47 47 47 47 47 47 48 48 48
 48 48 48 48 48 48 48 48 49 49 49 49 49 49 49 49 49 49 49 50 50 50 50 50 50 50 50 50 50 50 51 51 51 51 51 51 51 51 51 51 51 52 52 52 52 52 52 52 52 52
 52 53 53 53 53 53 53 53 53 53 53 53 54 54 54 54 54 54 54 54 54 54 54 55 55 55 55 55 55 55 55 55 56 56 56 56 56 56 56 56 56 56 57 57 57 57 57 57 57 57
 57 57 58 58 58 58 58 58 58 58 58 58 59 59 59 59 59 59 59 59 59 59 60 60 60 60 60 60 60 60 60 60 61 61 61 61 61 61 61 61 61 61 61 62 62 62 62 62 62 62
 62 62 62 62 63 63 63 63 63 63 63 63 63 63 64 64 64 64 64 64 64 64 64 64 64 65 65 65 65 65 65 65 65 65 65 65 66 66 66 66 66 66 66 66 66 66 67 67 67 67
 67 67 67 67 67 67 67 68 68 68 68 68 68 68 68 68 68 68 69 69 69 69 69 69 69 69 69 69 69 70 70 70 70 70 70 70 70 70 70 71 71 71 71 71 71 71 71 71 71 71

 */