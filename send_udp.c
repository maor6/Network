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
  int socket_fd,fsize;
  struct sockaddr_in  dest;
  struct hostent *hostptr;
  struct { char head; char *body; char tail; } msgbuf;
  char g[256];
  struct sockaddr_in  s_in, from;
  int cc,len;

  

  socket_fd = socket (AF_INET, SOCK_DGRAM, 0); // creat socket with IPv4 and type UDP and the 0 is default
  bzero((char *) &dest, sizeof(dest)); /* They say you must do this */    //zero the rest of the dest socket
  hostptr = gethostbyname(argv[1]);  // get the host info 
  dest.sin_family = (short) AF_INET; // host byte order (IPv4) 
  bcopy(hostptr->h_addr, (char *)&dest.sin_addr,hostptr->h_length);  //copy the dest address
  
  dest.sin_port = htons((u_short)0x3333);  // short, network byte order
  char m[] = "maor";
/*  
msgbuf.head = '<'; 
  msgbuf.body = htonl(m);  IMPORTANT!   // host to network long 
  msgbuf.tail = '>';
*/

  sendto(socket_fd,m,sizeof(m),0,(struct sockaddr *)&dest,sizeof(dest));  // send the data

    fsize = sizeof(from); 
    cc = recvfrom(socket_fd,&g,sizeof(g),0,(struct sockaddr *)&dest,&len); //from where we recived the socket
    
    printf("Got data ::%s\n",g);
    
 
  
  return 0;
}













