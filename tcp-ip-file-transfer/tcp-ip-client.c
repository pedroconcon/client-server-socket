#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>//inet_addr
#include <unistd.h> //close()
#include <sys/socket.h> //socket lib
#include <netinet/in.h> //Internet Protocol family lib = IPPROTO_IP, IPPROTO_ICMP, IPPROTO_TCP, IPPROTO_UDP

static const int SIZE = 2048;

void upload(int clientSkt);
void download(int clientSkt);
void errorHandling(const char *msg);

int main(){       

    printf("Init client-side\n");

    in_port_t servPort = 5000;

    /* SOCKET */
    int clientSkt = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if(clientSkt < 0)
        errorHandling("Socket error");

    printf("Socket created\n");

    /* Build server address structure */
    struct sockaddr_in servAddr;//Local address      
    memset(&servAddr, 0, sizeof(servAddr)); //Zero out structure
    servAddr.sin_family = AF_INET;//IPv4 address family
    servAddr.sin_addr.s_addr = inet_addr("192.168.1.11");
    servAddr.sin_port = htons(servPort); //Local port

    /* CONNECT */
    if(connect(clientSkt,(struct sockaddr *) &servAddr, sizeof(servAddr)))
        errorHandling("Connect error");

    printf("Connect ok\n");

    /*UPLOAD OR DOWNLOAD FILE*/
    int action;
    printf("Upload or download a file?\n");
    printf("1 - UPLOAD\n");
    printf("2 - DOWNLOAD\n");
    scanf("%d", &action);

    if(send(clientSkt,&action,sizeof(action),0)<0)
        errorHandling("Send action error");

    printf("Send Action ok\n");

    if(action == 1){

        printf("Start Uploading...\n");
        upload(clientSkt);
    }
    
    if(action == 2){

        printf("Start download...\n");
        download(clientSkt);
    }

    /* CLOSE */
    if(close(clientSkt) < 0)
        errorHandling("Close error");

    printf("Close ok\n");
}

void upload(int clientSkt){

        /*Get the file*/
        FILE *f = fopen("info-to-upload.txt", "r");
        if(f == NULL){
            printf("File not found\n");     
        }
        
        char buf[SIZE];

        memset(&buf, 0, sizeof(buf)); 
        while(!feof(f)){ //final end of file
            fread(&buf, sizeof(buf), 1, f);
            
            /* SEND */
            if(send(clientSkt,buf,sizeof(buf),0)<0)//send file to client
                errorHandling("Send error");
             
            memset(&buf, 0, sizeof(buf)); 
        }

        fclose(f);
        printf("Send ok\n");
}

void download(int clientSkt){

    FILE *fileToDownload = fopen("info-downloaded.txt","w");
    if(fileToDownload == NULL){
        printf("File not found\n");     
    }

    char buf[SIZE];

    while(recv(clientSkt, buf, SIZE,0)> 0){//receive the file
        fprintf(fileToDownload, "%s", buf);
        memset(&buf,0,sizeof(buf));            
    }

    fclose(fileToDownload);
}

void errorHandling(const char *msg){
    perror(msg);
    exit(1);
}