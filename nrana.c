/* Name - Nahushkumar Nitinbhai Rana
/* USF id - nrana
/* compile with gcc -o name server.c -lpthread 
/* Server for socket program   */



#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include <arpa/inet.h>
#include <fcntl.h> 
#include <unistd.h> 
#include<pthread.h>




char client_message[2000];      //shared memory
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;     //mutex lock
char buffer[1024];       

#define IP_ADDR "131.247.3.8" 
#define PORT 1050


// writer thread 
void * socketThread(void *arg)
{
  int newSocket = *((int *)arg);
  pthread_mutex_lock(&lock);                          //lock the mutex
  recv(newSocket , client_message , 2000 , 0);       //recieve data from writer
  char * message = malloc(sizeof(client_message)+20);
  // Send message to the client socket 
  strcpy(message,client_message);
  strcat(message,"\n");
  strcpy(buffer,message);                           //copying data in buffer    
  //printf("%s\n",message);
  free(message);
  printf("message for writer : %s\n",client_message);
  sleep(2);                                        //sleeping for 2 sec
  send(newSocket,buffer,30,0);                      //sending data to writer
  pthread_mutex_unlock(&lock);                      //unlock the mutex
  close(newSocket);
  pthread_exit(NULL);
}


int main(int argc, char const *argv[]) 
{ 
    int server_fd, new_socket; 
    struct sockaddr_in address;
    struct sockaddr_storage serverStorage;
    int opt = 1; 
    int addrlen = sizeof(address); 
    char hello[1024] = "Hello from server"; 
    socklen_t addr_size;
     

    // Creating socket file descriptor 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
       
    // Forcefully attaching socket to the port 8080 
   if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                  &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 
    
 
    address.sin_family = AF_INET; 
    //address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_addr.s_addr = inet_addr(IP_ADDR);
    address.sin_port = htons( PORT ); 
       
    // Forcefully attaching socket to the port 8080 
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
    if (listen(server_fd, 3) == -1) 
    { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    }
    
    pthread_t tid[3];
    printf("listening...\n");
    
    //start listing for writer
    for(int j = 0; j< 3;j++)
    {
        //Accept call creates a new socket for the incoming connection
        addr_size = sizeof serverStorage;
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address,  
                       (socklen_t*)&addrlen))<0) 
        { 
        perror("accept"); 
        continue;
        }
        //for each client request creates a thread and assign the client request to it to process
        //so the main thread can entertain next request
        if( pthread_create(&tid[j], NULL, socketThread, &new_socket) != 0 )
           printf("Failed to create thread\n");

        
    }
    
    //waiting for all the thread to close
     for(int j = 0; j< 3;j++){
         pthread_join(tid[j],NULL);
     }
    
    //Closing the socket
     if(shutdown(server_fd, 2 ) < 0){
         perror("Cannot close the socket");
     }
     close(server_fd);
    return 0; 
} 