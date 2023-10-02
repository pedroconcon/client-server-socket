#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h> //socket lib
#include <netinet/in.h> //Internet Protocol family lib = IPPROTO_IP, IPPROTO_ICMP, IPPROTO_TCP, IPPROTO_UDP | Structs that define port, IP Address...

static const int MAXPENDING = 5;

void errorHandling(const char *msg){
    perror(msg);
    exit(1);
}

void main(){

    printf("Init server-side\n");

    in_port_t servPort = 5000;

    /* SOCKET */
    int servSkt = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if(servSkt < 0)
        errorHandling("Socket error\n");

    printf("Socket created\n");

     /* Build local address structure */
    struct sockaddr_in servAddr;//Local address      
    memset(&servAddr, 0, sizeof(servAddr)); //Zero out structure
    servAddr.sin_family = AF_INET;//IPv4 address family
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);//Any incoming interface
    servAddr.sin_port = htons(servPort); //Local port

    /* BIND */
    if (bind(servSkt, (struct sockaddr*) &servAddr, sizeof(servAddr)) < 0)
        errorHandling("Bind error\n");

    printf("Bind ok\n");

     /* LISTEN */
    if(listen(servSkt,MAXPENDING)< 0)
        errorHandling("Listen error\n");    

    printf("Listen ok\n");

    while(1){
        printf("Wait Accept\n");
        /* ACCEPT */
        struct sockaddr_in clntAddr; //client address
        socklen_t clntAddrLen = sizeof(clntAddr);

        int clientSocket = accept(servSkt,(struct sockaddr *) &clntAddr, &clntAddrLen);
        
        if(clientSocket < 0)
            errorHandling("Accept error\n");

        printf("Accept ok\n");

        /* RECEIVE*/
        char recv_msg[100];
        int msg_size = 0;

        if((msg_size = recv(clientSocket, recv_msg, sizeof(recv_msg),0)) < 0)
            errorHandling("Receive error\n");
        
        printf("Receive ok\n");    

        printf("Msg: %s", recv_msg);
        
        /* CLOSE */
        if((close(clientSocket)) < 0)
            errorHandling("Close error\n");

        printf("Close ok\n");
    }

}