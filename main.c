//  SysNets II
//  Dr. Reichherzer
//  Project 3
//  Jeff Morton and Andrew Havard

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>
#include "router.h"

#define RCVBUFSIZE 256   /* Size of receive buffer */

//Prototype(s)
void zeroBuf(char *, int);

/*
 * A test program to start a client and connect it to a specified server.
 * Usage: client <hostname> <portnum>
 *    client is this client program
 *    <hostname> IP address or name of a host that runs the server
 *    <portnum> the numeric port number on which the server listens
 */
int main(int argc, char** argv)
{
    int                sockfd;
    struct sockaddr_in servaddr;
    char               response[256];
    char               message[256];
    
    zeroBuf(response, RCVBUFSIZE);
    zeroBuf(message, RCVBUFSIZE);
    
    printf("Runs here 1\n");
    fflush(stdout);
    argc = 3;
    argv[1] = "cs-ssh3.cs.uwf.edu";
    argv[2] = "60009";
    if (argc != 3) {
        fprintf (stderr, "Usage: client <hostname> <portnum>\n");
        exit (1);
    }
    printf("Runs here 2\n");
    fflush(stdout);
    // parse input parameter for port information
    int portNum = atoi (argv[2]);
    printf("Runs here 3\n");
    fflush(stdout);
    // create a streaming socket
    sockfd = createSocket(argv[1], portNum, &servaddr);
    if (sockfd < 0) {
        exit (1);
    }
    printf("Runs here 4\n");
    fflush(stdout);
    printf ("Enter a message: ");
    fgets (message, 256, stdin);
    // replace new line with null character
    message[strlen(message)-1] = '\0';
    
    // send request to server
    //if (sendRequest (sockfd, "<echo>Hello, World!</echo>", &servaddr) < 0) {
    if (sendRequest (sockfd, message, argv[1], portNum) < 0) {
        close (sockfd);
        exit (1);
    }
    
    if (receiveResponse(sockfd, response, 256) < 0) {
        close (sockfd);
        exit (1);
    }
    close (sockfd);
    
    // display response from server
    printResponse(response);
    
    exit(0);
}


/* zeroBuf
 *  in place of bzero function, nulls out a char
 *  array (pointer), given the size
 */
void zeroBuf(char * arry, int sizeArry)
{
    int i;
    for(i = 0; i < sizeArry; i++)
    {
        arry[i] = '\0';
    }	
}

