#include <stdio.h>
#include <stdlib.h>

#include <netdb.h>
#include <netinet/in.h>

#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <inttypes.h>
#include <sys/socket.h>
struct data
{
	int sequence_number;  //identifier
	char time[50];//time
	int rc;  //an even non neg. integer
};
int main(int argc, char *argv[])
{
	int sockid,portno;		//Socket Descriptor like a file handle and port number to be used
	 socklen_t server_ad;
	struct sockaddr_in serv_addr,client_addr;
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
		fprintf(stderr,"To use type->  %s hostname port_number packetsize (Should be from 100 to 1300) \n",argv[0]);
		exit(0);
	}

	portno=atoi(argv[2]);  //From String to integer

	sockid = socket(AF_INET, SOCK_DGRAM, 0); //Creating a socket point of datagram type
	if(sockid<0) //checking validity of sockid i.e. socket descriptor
	{
		perror("There's some error in opening socket");
		exit(1);
	}

    int p=atoi(argv[3]);

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
   
    struct data d;
    struct data *ptr;
    ptr=&d;
    char seq[1024];
    printf("Enter the identifier for message (less than 1000 characters)\n");
    
    gets(seq);
    int n;
 
    d.sequence_number=atoi(seq);
   	
    time_t now=time(NULL);
     
    server_ad=sizeof(serv_addr);	//size of in-out parameter
    
    strftime(seq, 20, "%Y-%m-%d %H:%M:%S", localtime(&now));
    //seq=(string)time(0);
    
    strcpy(d.time,seq);
    printf("%s\n",d.time );
   
	double total=0;
    printf("Enter the reflection count\n");
    gets(seq);
    d.rc=atoi(seq);
    int fix=d.rc;

    //sending p
     
     struct timeval tv1,tv2,stime,rtime;
     gettimeofday(&tv1,NULL);
     uint64_t a=tv1.tv_sec*(uint64_t)1000000+tv1.tv_usec;
     uint64_t b,c;
     fd_set rfds;
    struct timeval tv;
    int retval;
    tv.tv_sec=5;
    tv.tv_usec=0;

     for(int p=100;p<=1500;)
     {
        d.rc=p;
        printf("sending %d",p );
        printf(" bytes\n");
        n= sendto(sockid,(struct data *)&d,1024+sizeof(d),0,(struct sockaddr*) &serv_addr,sizeof(serv_addr));
        if(n<0)
        {
            perror("There was some error writing data to socket");
            exit(1);
        }
                FD_ZERO(&rfds);
                FD_SET (sockid,&rfds);
                tv.tv_sec=5;
                tv.tv_usec=0;
                retval=select(sockid+1,&rfds,NULL,NULL,&tv);
  
                if(retval==-1)
                {
                    perror("error in select\n");

                }
                if(retval==0)
                {
                    printf("TIMEOUT\n");
                }
                else
                {
                    if(recvfrom(sockid,(struct data *)&d,p+sizeof(d),0,(struct sockaddr *) &serv_addr, &server_ad)<0)
                    {
                        perror("receiving falied");
                    }
                        
                    if(d.rc!=p){continue;}
                }



        //	printf("%d\n",d.rc );
       	//printf("%d\n",d.time);
        
        for(int i=0;i<50;i++)
        { 
            d.rc=fix;
            while(d.rc>0)
            {
                
                n= sendto(sockid,(struct data *)&d,p+sizeof(d),0,(struct sockaddr*) &serv_addr,sizeof(serv_addr));
                if(n<0)
                {
                	perror("There was some error writing data to socket");
                	exit(1);
                }
                
                    
                FD_ZERO(&rfds);
                FD_SET (sockid,&rfds);
                tv.tv_sec=5;
                tv.tv_usec=0;
                retval=select(sockid+1,&rfds,NULL,NULL,&tv);
  
                if(retval==-1)
                {
                    perror("error in select\n");

                }
                if(retval==0)
                {
                    printf("TIMEOUT\n");
                }
                else
                {
                    if(recvfrom(sockid,(struct data *)&d,p+sizeof(d),0,(struct sockaddr *) &serv_addr, &server_ad)<0)
                    {
                    	perror("receiving falied");
                    }
                        
                    d.rc=d.rc-1;
                }
            }

            

        }

        gettimeofday(&tv2,NULL);
        b=tv2.tv_sec*(uint64_t)1000000+tv2.tv_usec;
        printf("RTT= %" PRId64 "\n", b-a);
        gettimeofday(&tv1,NULL);
        a=tv1.tv_sec*(uint64_t)1000000+tv1.tv_usec;
        now=time(NULL);
        total=0;
        p+=100;
    }

    
}