#include<stdio.h> 
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h> 
 
int main(int argc, char **argv)
{
    if (argc != 2)
    {
        perror("Remains [PORT]");
    }
    int flag;
    int sockfd;
    char buffer[100];
    char buffer2[100];
    char server_reply[200];
    struct sockaddr_in servaddr; 

    sockfd = socket(AF_INET,SOCK_STREAM,0);
    if (sockfd == -1){
        perror("Could not create socket");
    }

    printf("Created Socket \n");
    bzero(&servaddr,sizeof (servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(atoi(argv[1]));
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if(connect(sockfd, (struct sockaddr *)&servaddr,sizeof(servaddr))==-1){
        printf("not connected");
        exit(1);
    }
    else{ 
        while (1){
            if(flag==1){
                printf("Enter INT or MAX : \n");
                scanf("%[^\n]",buffer2);
                printf("%s\n",buffer2);
                if (send(sockfd,buffer2,20,0) < 0){
                    printf("Error SEND \n");
                    return 1;
                }
                if(recv(sockfd,server_reply,200,0 ) < 0){
                    puts("Error READ ");
                    break;
                }
                printf("%s \n",server_reply );
                close(sockfd);
                return 0;
            }
            printf("Enter a pseudo: \n");
            scanf("%s\n",buffer);
            if (write(sockfd,buffer,strlen(buffer)) < 0){
                printf("Error writing \n");
                return 1;
            }
            if(read(sockfd,server_reply,200) < 0){
                puts("Error");
                break;
            }
            printf("%s \n",server_reply );
            flag++;
        }
    }

}