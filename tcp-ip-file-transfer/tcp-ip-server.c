#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>//inet_addr
#include <unistd.h> //close()
#include <sys/socket.h> //socket lib
#include <netinet/in.h> //Internet Protocol family lib = IPPROTO_IP, IPPROTO_ICMP, IPPROTO_TCP, IPPROTO_UDP | Structs that define port, IP Address...

static const int MAXPENDING = 5;
static const int SIZE = 2048;

void errorHandling(const char *msg);
void upload(int clientSocket);
void download(int clientSocket);

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
        int recv_msg;//action
        int msg_size = 0;

        if((msg_size = recv(clientSocket, &recv_msg, sizeof(recv_msg),0)) < 0)
            errorHandling("Receive Action error\n");
        
        printf("Action: %d\n", recv_msg);
        printf("Receive Action ok\n");    

        if(recv_msg == 1){//UPLOAD

            printf("Start Uploading...\n");
            upload(clientSocket);
        }

        if(recv_msg == 2){//DOWNLOAD

            printf("Start download...\n");
            download(clientSocket);
        }        
        
        /* CLOSE */
        if((close(clientSocket)) < 0)
            errorHandling("Close error\n");

        printf("Close ok\n");
    }

}

void upload(int clientSocket){

    FILE *fileToUpload = fopen("info-uploaded.txt","w");
    if(fileToUpload == NULL){
        printf("File not found\n");     
    }

    char buf[SIZE];

    while(recv(clientSocket, buf, SIZE,0)> 0){//receive the file
        fprintf(fileToUpload, "%s", buf);
        memset(&buf,0,sizeof(buf));            
    }

    fclose(fileToUpload);
}

void download(int clientSocket){
    
        /*Get the file*/
        FILE *f = fopen("info-to-download.txt", "r");
        if(f == NULL){
            printf("File not found\n");     
        }
        
        char buf[SIZE];

        memset(&buf, 0, sizeof(buf)); 
        while(!feof(f)){ //final end of file
            fread(&buf, sizeof(buf), 1, f);
            
            /* SEND */
            if(send(clientSocket,buf,sizeof(buf),0)<0)//send file to client
                errorHandling("Send error");
             
            memset(&buf, 0, sizeof(buf)); 
        }

        fclose(f);
        printf("Send ok\n");
}

void errorHandling(const char *msg){
    perror(msg);
    exit(1);
}

