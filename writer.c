/* Name - Nahushkumar Nitinbhai Rana
/* USF id - nrana
/* compile with in linux = gcc -o writer writer.c 
/* compile with in windows = gcc -o writer writer.c -lws2_32
/* writer for socket program*/



#include <sys/types.h> 
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#if defined(_WIN32) || defined(_WIN64)
    #include <winsock2.h>
#else
    #include <sys/socket.h>
    #include <arpa/inet.h>
#endif


#define IP_ADDR "131.247.3.8"   /* netcluster's IP address, where server */
//  #define IP_ADDR "127.0.0.1"                                  
#define PORT_NUM 1050               //port number                       
/* For test #define PORT_NUM 1150 */
/* #define PORT_NUM 1050 */
//#define PORT_NUM 1044
//#define PORT_NUM 1050
//#define PORT_NUM 1159
//#define PORT_NUM 1051
//#define PORT_NUM 1050
#define BUFFLEN 100



int  main(int argc, char *argv[])
{
	int len;
	int sock;
	struct sockaddr_in  server,remote;
	int remotelength;
	int fd;
	char buf[BUFFLEN];
    
    printf("\n****** writer start *****\n");
    #if defined(_WIN32) || defined(_WIN64)    
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0) {
        printf("\nError: Windows socket subsytsem could not be initialized. Error Code: %d. Exiting..\n", WSAGetLastError());
        exit(1);
    }
    #endif

	if ( argc != 2 ) /* argc should be 2 for correct execution */
	  {
	    /* We print argv[0] assuming it is the program name */
	    printf( "usage: %s filename", argv[0] );
            exit(0);
	  }
      
      
	/* socket() call generates one socket for ipc */
	if ( (sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("Opening Stream Socket");
		exit(1);
	}
    else
    printf("Opening Stream Socket\n");



	/* address naming for the internet domain socket */
	server.sin_family = AF_INET;
      	server.sin_port = htons(PORT_NUM);
      	//server.sin_addr.s_addr = INADDR_ANY;
        server.sin_addr.s_addr = inet_addr(IP_ADDR);
        
        
        
	/* use the socket and server address to initiate connection */
	if( connect(sock, (struct sockaddr*)&server, sizeof(server))<0)	{
		perror("connecting stream socket");
		exit(1);
	}
    else
    printf("connecting stream socket\n");


        len = strlen(argv[1]);
        printf("Connected and sending out a message of len to server%d\n", len);

        //sending msg to server
		if (send(sock, argv[1], len, 0) == -1)
			{
			perror("Write to Socket Channel");
			exit(1);
			}
        else
        printf("Write to Socket Channel\n");
    
        //receving msg to server
        len = recv(sock, buf, BUFFLEN, 0);
        printf("Got back for Server: ");
        fflush(stdout);
        printf("%s", buf);

        write(0, buf, len);
        printf("\n****** writer end *****\n");
        
        
        /* close the communication socket and listenning socket */
        #if defined(_WIN32) || defined(_WIN64)  
        closesocket(sock);
        WSACleanup();
        #else
        close(sock);
        #endif
        
    return 0;
}

