#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h> //socket lib
#include <netinet/in.h> //Internet Protocol family lib = IPPROTO_IP, IPPROTO_ICMP, IPPROTO_TCP, IPPROTO_UDP


void errorHandling(const char *msg){
    perror(msg);
    exit(1);
}

int main(){       

    printf("Init client-side\n");

    in_port_t servPort = 5000;

    /* SOCKET */
    int clientSkt = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if(clientSkt < 0)
        errorHandling("Socket error\n");

    printf("Socket created\n");

    /* Build server address structure */
    struct sockaddr_in servAddr;//Local address      
    memset(&servAddr, 0, sizeof(servAddr)); //Zero out structure
    servAddr.sin_family = AF_INET;//IPv4 address family
    servAddr.sin_addr.s_addr = inet_addr("192.168.1.11");//Any incoming interface (server side IP)
    servAddr.sin_port = htons(servPort); //Local port

    /* CONNECT */
    if(connect(clientSkt,(struct sockaddr *) &servAddr, sizeof(servAddr)))
        errorHandling("Connect error\n");

    printf("Connect ok\n");

    /* Get the message*/
    char msg[100];
    printf("Write the message: ");
    fgets(msg, 100+1, stdin);

    /* SEND */
    if(send(clientSkt,msg,sizeof(msg),0)<0)
        errorHandling("Send error\n");
    
    printf("Send ok\n");
    
    /* CLOSE */
    if(close(clientSkt) < 0)
        errorHandling("Close error\n");

    printf("Close ok");
}