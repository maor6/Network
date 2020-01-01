#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
  int socket_fd;
  struct sockaddr_in  dest,router;
  struct hostent *hostptr;
  struct { char head; char *body; char tail; } msgbuf;
  
  struct sockaddr_in  s_in;
  int cc;

  

  socket_fd = socket (AF_INET, SOCK_DGRAM, 0); // open socket with IPv4 and type UDP and the 0 is default
  bzero((char *) &dest, sizeof(dest)); /* They say you must do this */    //zero the rest of the dest socket
  hostptr = gethostbyname(argv[1]);  // get the host info 
   
  //open socket to listen
  dest.sin_family = (short) AF_INET; // host byte order (IPv4) 
  bcopy(hostptr->h_addr, (char *)&dest.sin_addr,hostptr->h_length);  //copy the dest address
  
  dest.sin_port = htons((u_short)0x3333);  // short, network byte order

 //open new socket to send
  router.sin_family = (short) AF_INET; // host byte order (IPv4) 
  bcopy(hostptr->h_addr, (char *)&router.sin_addr,hostptr->h_length);  //copy the dest address
  
  router.sin_port = htons((u_short)0x4444);  // short, network byte order

  char m[256]; //to send
  char g[256]; // to save

/*  
msgbuf.head = '<'; 
  msgbuf.body = htonl(m);  IMPORTANT!   // host to network long 
  msgbuf.tail = '>';
*/
 
 bind(socket_fd, (struct sockaddr *)&dest, sizeof(dest));
 
 int scan = 1;
 while(scan !=0 && strcmp(m,"exit")!=0){
    printf("enter a msg to send :");
    scanf("%s",m);
    sendto(socket_fd,m,sizeof(m),0,(struct sockaddr *)&router,sizeof(router));  // send the data      
    cc = recvfrom(socket_fd,&g,sizeof(g),0,(struct sockaddr *)&dest,sizeof(dest)); //from where we recived the socket
    
    printf("Got data :%s\n",g);
   
    bzero(g, sizeof(g)); //clear what we recv
 }
 

  return 0;
}




