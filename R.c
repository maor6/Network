#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
void printsin(struct sockaddr_in *s, char *str1, char *str2) {
  printf("%s\n", str1);
  printf("%s ip=%s, port=%d", str2, inet_ntoa(s->sin_addr), s->sin_port);
  //-- port: sin->sin_port (host integer type) 
  //-- IP: sin->sin_addr (IP in dotted-decimal notation) 
  printf("\n");
}
*/


int main(int argc, char *argv[])
{
  
  int socket_fd, socket_fd2, cc, fsize;

  struct sockaddr_in  s_in , s_in2, from, router; //creat socketaddr : s to client ,  s2 to server , from to the recv, router  me
 
  
  double X = 0.20;
  char data[256];

  unsigned short server_port, client_port, router_port;
  server_port = (u_short)0x5555;
  client_port = (u_short)0x3333;
  router_port   = (u_short)0x4444;
  
  socket_fd = socket (AF_INET, SOCK_DGRAM, 0); // open socket IPV4 and protocol UDP

  
  bzero((char *) &s_in, sizeof(s_in));    //zero all the rest of sockets
  bzero((char *) &s_in2, sizeof(s_in2));  
  bzero((char *) &router, sizeof(router));  
  
   
  router.sin_family = (short)AF_INET; //IPV4

  // converting from HOST to NETWORK [long] of ANY ADDRESS, 
  router.sin_addr.s_addr = htonl(INADDR_ANY);    


  
  router.sin_port = htons(router_port); // router port change byte to network

  
  s_in.sin_family = (short)AF_INET; //IPV4 

  // converting from HOST to NETWORK [long] of ANY ADDRESS, 
  s_in.sin_addr.s_addr = htonl(INADDR_ANY);

  
  s_in.sin_port = htons(client_port); // client port change byte to network

  
  s_in2.sin_family = (short)AF_INET; //IPV4 

  // converting from HOST to NETWORK [long] of ANY ADDRESS, 
  s_in2.sin_addr.s_addr = htonl(INADDR_ANY);    

  
  s_in2.sin_port = htons(server_port); // Server port change byte to network

  
  fflush(stdout);

  
  bind(socket_fd, (struct sockaddr *)&router, sizeof(router)); // connect address and the port with socket to listen

  
  for(;;) {  // to litsen all the time to reqeust
    bzero(&data,sizeof(data)); // to reset the data

    // size of struct (sockaddr_in)
    fsize = sizeof(from);
    
    
    
    cc = recvfrom(socket_fd,&data,sizeof(data),0,(struct sockaddr *)&from,&fsize); // the packet we recv

    
    printsin( &from, "Router: ", "Packet from:"); // printsin from where we get the msg

     
    printf("Got data : %s\n",data); // print the data we get
    
    if(from.sin_port == server_port) // send to client
    {
      printf("[SERVER]\n"); 
      fsize = sizeof(s_in);
      sendto(socket_fd,&data,sizeof(data),0,(struct sockaddr *)&s_in,fsize);
    } 
    else
    {
      
      double random = (double)rand() / (double)RAND_MAX; // send to server
      if(random < X) {   // check the number with x if small send ERROR
        printf(" X is: %lf, random is: %lf\n", X, random);
        
        strcpy(data,"ERROR Packet dont send");
         fsize = sizeof(s_in);
        sendto(socket_fd,&data,sizeof(data),0,(struct sockaddr *)&s_in,fsize); //send the ERROR
        continue;
      }
        
        
      printf("[CLIENT]\n");
      fsize = sizeof(s_in2);
      sendto(socket_fd,&data,sizeof(data),0,(struct sockaddr *)&s_in2,fsize);
      
      
    }    
   
  }
  
  return 0;
}

