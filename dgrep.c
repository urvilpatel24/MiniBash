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
#include <fcntl.h>

//default buffer size
#define SIZE 1024

void recvResult(int sockfd)
{
    int n;
    char bf[SIZE];
    while(1) {
        n = recv(sockfd, bf, SIZE, 0);
        if(n<=0) {
            break;
            return;
        }
        printf("%s", bf);
        bzero(bf, SIZE);
    }
    return;
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
}

int main(int argc, char **argv)
{
    //char *ipaddress = "127.0.0.1";
    char *ipaddress = "137.207.82.51";
    int port = 7333;
    int e, sockfd;
    FILE *fp, *fp2;
    char *ptrn = argv[1];
    char *fname1 = argv[2];
    char *fname = argv[3];
    char cmd[255], input_ptrn[255], input_file[255];
    struct sockaddr_in server_address;
    
	//check if file exists
    fp = fopen(fname, "r");
    fp2 = fopen(fname1, "r");
    if(fp == NULL || fp2 == NULL) {
        perror("Issue in reading file ...");
        exit(1);
    }
	// check if pattern is calid or not
	if(ptrn == NULL) {
        perror("Issue in pattern ...");
        exit(1);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd<0) {
        perror("Issue in socket ...");
        exit(1);
    }
    server_address.sin_family = AF_INET;
    server_address.sin_port = port;
    server_address.sin_addr.s_addr = inet_addr(ipaddress);

    e = connect(sockfd, (struct sockaddr*)&server_address, sizeof(server_address));
    if(e == -1) {
        perror("Issue in connection ...");
        exit(1);
    }

    sendFile(fp,sockfd);
    sprintf(input_ptrn, "%s#%s\n", ptrn, fname);

    int bc = send(sockfd,  input_ptrn,  strlen(input_ptrn),  0);
    shutdown(sockfd, SHUT_WR);
    sprintf(cmd, "grep  -RwH %s %s --color",ptrn, fname1);
    system(cmd);
    
    recvResult(sockfd);
    close(sockfd);
    printf("\nDisconnected from the server ...\n");
    return 0;
}
