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

int main(int argc, char *argv[])
{
  int socket_fd;
  struct sockaddr_in  dest;
  struct hostent *hostptr;
  struct { char head; u_long body; char tail; } msgbuf;

  socket_fd = socket (AF_INET, SOCK_DGRAM, 0); // creat socket with IPv4 and type UDP and the 0 is default
  bzero((char *) &dest, sizeof(dest)); /* They say you must do this */    //zero the rest of the dest socket
  hostptr = gethostbyname(argv[1]);  // get the host info 
  dest.sin_family = (short) AF_INET; // host byte order (IPv4) 
  bcopy(hostptr->h_addr, (char *)&dest.sin_addr,hostptr->h_length);  //copy the dest address  
  
  dest.sin_port = htons((u_short)0x3333);  // short, network byte order

  msgbuf.head = '<'; 
  msgbuf.body = htonl(getpid()); /* IMPORTANT! */   // host to network long 
  msgbuf.tail = '>';

  sendto(socket_fd,&msgbuf,sizeof(msgbuf),0,(struct sockaddr *)&dest,sizeof(dest));  // send the msg 
  return 0;
}

