/*
    COMP8567 - Adavance System Programming
    Final Project
    Team:
    1. Urvil Patel (110030912)
    2. Vraj Soni (110060926)
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define SIZE 1024

char *recvFile(int sockfd, char *filename)
{
    int n;
    FILE *fp;
    char bf[SIZE];
    fp = fopen(filename, "w");
    
    if(fp==NULL) {
        perror("Issue in creating file ...");
        exit(1);
    }
    while(1) {
        n = recv(sockfd, bf, SIZE, 0);
        if(n<=0)
            break;
        
        char* pPosition = strrchr(bf,'#');
        
        if (!pPosition)
            fprintf(fp, "%s", bf);
        else {
            static char pattern[50];
            strcpy(pattern, bf);
            fclose(fp);
            return pattern;
        }
        bzero(bf, SIZE);
    }
    fclose(fp);
}

void sendFile(FILE *fp, int sockfd)
{
    char data[SIZE] = {0};

    while(fgets(data, SIZE, fp)!=NULL) {
        if(send(sockfd, data, sizeof(data), 0)== -1) {
            perror("Issue in sending data ...");
            exit(1);
        }
        bzero(data, SIZE);
    }
    fclose(fp);
}

int main ()
{
    char bf[SIZE];
    int port = 7333, e, sockfd, new_sock;
    struct sockaddr_in server_address, new_address;
    socklen_t addr_size;
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd<0) {
        perror("Issue in socket ...");
        exit(1);
    }
    
    printf("Server socket has been created ... \n");
    server_address.sin_family = AF_INET;
    server_address.sin_port = port;
    // we hve used INADDR_ANY becuse we want to bind to all interfaces - not just "localhost"
    // used to convert an IPv4 address in host byte order to the IPv4 address in network byte order
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);

    e = bind(sockfd,(struct sockaddr*)&server_address, sizeof(server_address));

    if(e<0) {
        perror("Issue in Binding ...");
        exit(1);
    }
    printf("Binding Successfull ...\n");

    while(1){
        e = listen(sockfd, 10);
        
        if(e==0)
            printf("Listening ...\n");
        else {
            perror("Issue in Binding ...");
            exit(1);
        }
        addr_size = sizeof(new_address);
        
        new_sock = accept(sockfd,(struct sockaddr*)&new_address, &addr_size);

        char *filename = "file2.txt";
        char* a;
        a = recvFile(new_sock, filename);

        char *pattern = strtok(a, "#");
        char *outfile_tmp = strtok(NULL, "#");
        char *outfile = strtok(outfile_tmp, "\n");
        char cmd[255];
        
        sprintf(cmd, "mv %s  %s && grep -RwH %s %s --color=always > output.txt", filename, outfile, pattern, outfile);
        system(cmd);

        FILE *fp;
        fp = fopen("output.txt", "r");
        sendFile(fp,new_sock);
        shutdown(new_sock, SHUT_WR);
     }
}
