#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define MAX_NAME 10

void *client_work(void *sock);
void sendINT(int sock);
void sendMAX(int sock);
void hello(int sock,char* pseudo);


uint16_t max ;
int ip;
char name[MAX_NAME];
char tmpname[MAX_NAME];
uint16_t num ;

pthread_mutex_t verrou = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        perror("Remains [PORT]");
    }

    int sockfd = socket(PF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        perror("Can't create socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in adress;
    adress.sin_port = htons(atoi(argv[1]));
    adress.sin_family = AF_INET;
    if (inet_aton("127.0.0.1", &adress.sin_addr) == 0)
    {
        perror("Error during assign localhost (INVALID IP)");
        exit(EXIT_FAILURE);
    }

    int bin = bind(sockfd, (struct sockaddr *)&adress, sizeof(struct sockaddr));
    if (bin == -1)
    {
        perror("binding error");
        exit(1);
    }

    int lis = listen(sockfd, 0);
    if (lis == -1)
    {
        perror("listening error");
        exit(1);
    }

    max = 0 ;
    struct sockaddr_in caller;
    socklen_t size = sizeof(caller);
    int *clients;
    while (1)
    {
        clients = (int *)malloc(sizeof(int));
        if (clients == NULL)
        {
            perror("Error malloc clients");
            exit(EXIT_FAILURE);
        }
        *clients = accept(sockfd, (struct sockaddr *)&caller, &size);
        if (*clients == -1)
        {
            perror("Error accepting client");
            exit(EXIT_FAILURE);
        }

        pthread_t th1;
        pthread_create(&th1, NULL, client_work, clients);
    }

    close(sockfd);
    close(*clients);
    pthread_exit(clients);
    free(clients);
    return 0;
}
    void *client_work(void *sock){
        int sock2 = *((int*)sock);
        char pseudo[MAX_NAME];
        hello(sock2, pseudo);
        char buff[3];
        read(sock2,buff,3);
        if(strcmp(buff,"INT")){
            sendINT(sock2);
        }
        if(strcmp(buff,"MAX")){
            sendMAX(sock2);
        }
        else{
            perror("invalid argument");
        }
        close(sock2);
        free(sock);
        return NULL;
    }

    void hello(int sock,char* pseudo){
        read(sock,pseudo,MAX_NAME);
        strcpy(tmpname,pseudo);
        char responsehello[MAX_NAME+6+1];
        sprintf(responsehello,"%s %s", "HELLO",pseudo);
        write(sock,responsehello,strlen(responsehello));
    }


    void sendINT(int sock){

       char buff[20];
       read(sock,buff,20);
       uint16_t val;
       memmove(&val,buff, sizeof(uint16_t));
        pthread_mutex_lock(&verrou);
        if(val > max){
            printf("c'est le plus grand max");
            max = val;
            strcpy(name,tmpname);
        }
        pthread_mutex_unlock(&verrou);
        char responseINT[100];
        sprintf(responseINT,"%s", "INTOK");
        if(write(sock,responseINT,100)==-1){
            perror("error sending int ");
            exit(1);
        }
        
    }

    void sendMAX(int sock){
        if(max > 0){
            size_t size = sizeof("REP")+sizeof(MAX_NAME)+sizeof(int);
            char reponseclient[size];
            memmove(reponseclient,"REP",3);
            memmove(reponseclient,name,sizeof(MAX_NAME));
            memmove(reponseclient,&max,sizeof(uint16_t));
            int count = write(sock,reponseclient,size);
            if(count!=size){
                write(sock,"NOP",3);
            }
        }
        else{
            perror("max = 0");
            exit(1);
        }
    }
