//  SysNets II
//  Dr. Reichherzer
//  Project 3
//  Jeff Morton and Andrew Havard

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>

#include "router.h"


int createSocket()
{
    int sockfd;
    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("Error creating a socket");
        return -1 * errno;
    }
    
    return sockfd;
}

int sendRequest(int sockFD, char * request, char * serverName, int serverPort)
{
    struct hostent *hostptr;
    struct sockaddr_in servaddr;
    
    memset((char *) &servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(serverPort);
    
    hostptr = gethostbyname(serverName);
    if(!hostptr) {
        fprintf(stderr, "sendRequest: Error getting IP of %s\n", serverName);
        return -1;
    }
    
    memcpy((void *) &servaddr.sin_addr, hostptr->h_addr_list[0], hostptr->h_length);
    
    if(sendto(sockFD, request, strlen(request), 0, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        perror("Error sending");
        return -1 * errno;
    }
    
    return 0;
}

int receiveResponse(int sockFD, char * response, int size)
{
    int error;
    if((error = (int)recvfrom(sockFD, response, (size-1), 0, NULL, NULL)) == -1) {
        perror("Error receiving");
        return -1 * errno;
    }
    
    response[error] = '\0';
    return 0;
}

int closeSocket(int sockFD)
{
    int error;
    if((error = close(sockFD)) == -1) {
        perror("Error receiving");
        return -1 * errno;
    }
    
    return error;
}

void printResponse(char * response)
{
    fprintf(stdout, "%s\n", response);
}
