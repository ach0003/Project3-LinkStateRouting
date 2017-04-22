//  SysNets II
//  Dr. Reichherzer
//  Project 3
//  Jeff Morton and Andrew Havard

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "files.h"

routerTree * createTree(int totalNumberOfNodes, argvInfo * me)
{
    int num = totalNumberOfNodes;
    routerTree * tree = (routerTree *)malloc(1 + sizeof(tree));
    if (tree == NULL)
    {
        perror("Error in allocattion of routerTree struct!\n");
        exit(1);
    }
    tree -> iterator = 0;
    tree -> size = totalNumberOfNodes;
    tree -> routerLocation = createCharArray2(num, 30);
    tree -> routerPort = createUshortArray(num);
    tree -> routerLocation = createCharArray2(num, 30);
    tree -> distance = createIntArray(num);
    tree -> routerName = createOneDimCharArray(num, 1);
    tree -> self = me -> routerLabel;
    return tree;
}

char * createOneDimCharArray(int numberOfItems, int sizeItem)
{
    char * oneDimensionalArray = (char *)malloc((1 + (numberOfItems)) * (sizeof(char)));
    
    int i, j;
    for(i = 0; i < numberOfItems; i++)
    {
        for(j = 0; j < sizeItem; j++)
            oneDimensionalArray[i] = '\0';
    }
    return oneDimensionalArray;
}

char ** createCharArray2(int numberOfItems, int sizeItem)
{
    char ** twoDimensionalArray = (char **)malloc((1 + (numberOfItems)) * (sizeof(char *)));
    if (twoDimensionalArray == NULL)
    {
        fprintf(stderr, "Error creating initial double pointer for 2-D array!\n");
        exit(1);
    }
    
    int i, j;
    for(i = 0; i < numberOfItems; i++)
    {
        twoDimensionalArray[i] = (char *)malloc((1 + (sizeItem)) * (sizeof(char)));
        if (twoDimensionalArray[i] == NULL)
        {
            perror("Error creating space for 2D array!\n");
            fprintf(stderr, "2-D Array index: %i\n",i);
            exit(1);
        }
    }
    
    for(i = 0; i < numberOfItems; i++)
    {
        for(j = 0; j < sizeItem; j++)
        {
            twoDimensionalArray[i][j] = '\0';
        }
    }
    
    return twoDimensionalArray;
}

int * createIntArray(int numberOfItems)
{
    int * array = (int *)malloc((1 + (numberOfItems)) * (sizeof(int)));
    if (array == NULL)
    {
        perror("Error in allocation of integer array!\n");
        exit(1);
    }
    int i;
    for(i = 0; i < numberOfItems; i++)
        array[i] = 0;
    
    return array;
}

unsigned short * createUshortArray(int numberOfItems)
{
    unsigned short * array = (unsigned short *)malloc((1 + (numberOfItems)) * (sizeof(unsigned short)));
    if (array == NULL)
    {
        perror("Error in allocattion of unsigned short array!\n");
        exit(1);
    }
    int i;
    for(i = 0; i < numberOfItems; i++)
        array[i] = 0;
    return array;
}

int readIn(FILE * fin, routerTree * tree)
{
    char temp[80];
    bzero(temp, 80);
    int numOfNeighbors = 0;
    
    while(fgets(temp, 100, fin) != NULL)
    {
        numOfNeighbors++;
        parseLine(temp, tree);
        bzero(temp, 80);
    }
    return numOfNeighbors;
}

void parseLine(char * str, routerTree * tree)
{
    int iter = 0;
    char temp[80], temp1 = str[0];
    bzero(temp, 80);
    tree->iterator = (routerNumber(str[iter]));
    tree->routerName[tree->iterator] = temp1;
    iter++;
    if(tree->iterator == tree->size)
    {
        fprintf(stderr, "Error reading in the file!\n");
        exit(1);
    }
    
    while(str[iter] != ',')
        iter++;
    
    iter++;
    copyToBuffer(temp, str, &iter, 80);
    strcpy(tree -> routerLocation[tree -> iterator], &temp[1]);
    bzero(temp, 80);
    iter++;
    copyToBuffer(temp, str, &iter, 80);
    tree -> routerPort[tree -> iterator] = atoi(temp);
    bzero(temp, 80);
    iter++;
    copyToBuffer(temp, str, &iter, 80);
    tree->distance[tree -> iterator] = atoi(temp);
    
}

void copyToBuffer(char * temp, char * string, int * iterate, int size)
{
    int i = 0;
    char temp2[80];
    bzero(temp2, 80);
    while((string[(*iterate)] != ',') && ((*iterate) < size) && (string[(*iterate)] != '\n'))
    {
        temp2[i] = string[*iterate];
        *iterate = (*iterate) + 1;
        i++;
    }
    strcpy(temp, temp2);
}

int routerNumber(char aph)
{
    int i = aph - 65;
    return i;
}

void printTreeNode(routerTree * tree)
{
    int i;
    for(i = 0; i < tree->size; i++)
    {
        printf("Node %c\n", tree->routerName[i]);
        printf("Location is %s\n", tree->routerLocation[i]);
        printf("And port is %hu\n", tree->routerPort[i]);
        printf("The router distance is %d\n", tree->distance[i]);
    }
}

argvInfo * processArgv(int argc, char ** argv)
{
    if(argc == 5 || argc == 6)
    {
        argvInfo * tempInfo = malloc(sizeof(argvInfo));
        if (tempInfo == NULL)
        {
            perror("Could not allocate space to process arguments!\n");
            exit(1);
        }
        tempInfo -> port = (unsigned short)atoi(argv[1]);
        tempInfo -> routerLabel = argv[2][0];
        tempInfo -> totalNumberOfNodes = argv[3][0] - '0';
        if(tempInfo -> totalNumberOfNodes < 5)
        {
            fprintf(stderr, "%i is too small! Must have at least 5 routers to start.\n", tempInfo->totalNumberOfNodes);
            exit(-1);
        }
        tempInfo -> discoveryTextFile = malloc(sizeof(char) * 100);
        if (tempInfo -> discoveryTextFile == NULL)
        {
            perror("Error in processing arguments!\n");
            exit(1);
        }
        
        sprintf(tempInfo -> discoveryTextFile, "%s", argv[4]);
        
        if(argc == 6)
        {
            if(strcmp(argv[5],"-dynamic") == 0)
                tempInfo -> dynamic = true;
            else
            {
                fprintf(stderr, "Usage:router <portNum> <routerLabel> <totalNumberOfNodes> <discoveryTextFile> [-dynamic]\n");
                exit(-1);
            }
        }
        else
            tempInfo -> dynamic = false;
        
        return tempInfo;
    }
    else
    {
        fprintf(stderr, "Incorrect number of arguments! Required format:\nrouter <port#> <routerLabel> <totalNumberOfNodes> <discoveryTextFile> [-dynamic]\n");
        exit(-1);
    }
}
/*
 int createSocket(char * serverName, unsigned short serverPort, struct sockaddr_in * dest)
 {
 int socketCreation = 0;
 struct hostent * htptr;
 
 if((htptr = gethostbyname(serverName)) == NULL)
 {
 fprintf(stderr, "Invalid host name\n");
 return -1;
 }
 //build the destination address
 memset((void *)dest, 0, (size_t)sizeof(struct sockaddr_in));
 dest->sin_family = (short)(AF_INET);
 memcpy((void *)&dest->sin_addr, (void *) htptr -> h_addr, htptr -> h_length);
 memcpy((void *)&dest->sin_addr, (void *) htptr -> h_addr, htptr -> h_length);
 dest->sin_port = htons(serverPort);
 
 //establish a connection to a socket
 socketCreation = socket(PF_INET, SOCK_DGRAM, 0);
 return socketCreation;
 }
 */

int createListeningSocket(unsigned short portSelector)
{
    struct sockaddr_in servaddr;
    int listensockfd;
    
    // creating a datagram socket
    listensockfd = socket (AF_INET, SOCK_DGRAM, 0);
    if (listensockfd < 0) {
        fprintf(stderr, "Error creating socket\n");
        return -1;
    }
    // build local address and set port to unknown
    memset ((void *) &servaddr, 0, (size_t)sizeof(servaddr));
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(portSelector);
    
    // statical binding
    bind (listensockfd, (struct sockaddr *) &servaddr, (socklen_t)sizeof(servaddr));
    
    // port information
    struct sockaddr_in boundAddr;
    socklen_t sizeAddr = sizeof(boundAddr);
    getsockname(listensockfd, (struct sockaddr *) &boundAddr, &sizeAddr);
    fprintf(stderr, "portNum: %hu\n", ntohs(boundAddr.sin_port));
    
    return listensockfd;
}

char * createLSP(routerTree * tree, int numOfNeighbors)
{
    char * myLSP = malloc(sizeof(char) * (9 + (numOfNeighbors * 4)));
    if (myLSP == NULL)
    {
        perror("Error creating LSP!\n");
        exit(1);
    }
    myLSP[0] = tree->self;
    myLSP[1] = ';';
    myLSP[2] = '0';
    myLSP[3] = ';';
    myLSP[4] = tree->size + '0';
    myLSP[5] = ';';
    myLSP[6] = numOfNeighbors + '0';
    int i = 0;
    int j = 7;
    for(i = 0; i < tree->size; i++)
    {
        if(tree->routerName[i] != '\0')
        {
            sprintf(&myLSP[j], ";%c;%i", tree -> routerName[i], tree -> distance[i]);
            j += 4;
        }
    }
    myLSP[j] = '\0';
    fprintf(stderr, "myLSP: %s\n", myLSP);
    return myLSP;
}

char ** flood(routerTree * tree, int numOfNeighbors, int listeningSocket, int * sockets, struct sockaddr_in * addresses, char * myLSP)
{
    fprintf(stderr, "Starting flood...");
    int iteration = 0;
    int selecting = 8;
    int sendTo = 0;
    int maxfdp1;
    fd_set rset;
    struct timeval tv;
    FD_ZERO(&rset);
    FD_SET(listeningSocket, &rset);
    maxfdp1 = 100;
    char ** myLSPlist = createCharArray2(tree -> size, 30);
    sprintf(myLSPlist[routerNumber(myLSP[0])], "%s", myLSP);
    for(iteration = 0; iteration < numOfNeighbors; iteration++)
        sendTo = (int)sendto(sockets[iteration], myLSP, strlen(myLSP), 0, (const struct sockaddr *)&addresses[iteration], sizeof(addresses[iteration]));
    
    char message[256];
    bzero(message, 256);
    sendTo = 0;
    for(iteration = 0; iteration < numOfNeighbors; iteration++)
    {
        sendTo = (int)recvfrom(listeningSocket, message, 256, 0, NULL, NULL);
        if(sendTo != -1)
            sprintf(myLSPlist[routerNumber(message[0])], "%s", message);
        else
            fprintf(stderr, "Flooding error... not recieved!\n");
        bzero(message, 256);
    }
    sendTo = 0;
    for(iteration = 0; iteration < numOfNeighbors; iteration++)
        sendTo = (int)sendto(sockets[iteration], myLSP, strlen(myLSP), 0, (const struct sockaddr *)&addresses[iteration], sizeof(addresses[iteration]));
    sendTo = 0;
    for(iteration = 0; iteration < tree->size * 2; iteration++)
    {
        selecting = 0;
        tv.tv_sec = 1;
        tv.tv_usec = 0;
        selecting = select(maxfdp1, &rset, NULL, NULL, &tv);
        if (selecting != 0)
        {
            sendTo = (int)recvfrom(listeningSocket, message, 256, 0, NULL, NULL);
            bzero(message, 256);
        }
    }
    
    int done = 0;
    while(done < tree->size * 10)
    {
        selecting = 8;
        sendTo = 0;
        int l = 0;
        int m = 0;
        for(l = 0; l < tree->size; l++)
        {
            if(myLSPlist[l][0] != myLSP[0] && myLSP[4] != '0' && myLSPlist[l][0] != '0' && myLSPlist[l][0] != '\0' && myLSPlist[l][4] != '0')
            {
                bzero(message, 256);
                sprintf(message, "%s", myLSPlist[l]);
                message[2] = ((message[2] - '0') + 1) + '0';
                message[4] = ((message[4] - '0') - 1) + '0';
                for(m = 0; m < numOfNeighbors; m++)
                    sendto(sockets[m], message, strlen(message), 0, (const struct sockaddr *)&addresses[m], sizeof(addresses[m]));
            }
        }
        
        sendTo = 0;
        for(iteration = 0; iteration < numOfNeighbors; iteration++)
        {
            bzero(message, 256);
            sendTo = (int)recvfrom(listeningSocket, message, 256, 0, NULL, NULL);
            if(sendTo != -1)
            {
                if(message[2] > myLSPlist[routerNumber(message[0])][2])
                    sprintf(myLSPlist[routerNumber(message[0])], "%s", message);
            }
            else
                fprintf(stderr, "Not recieved!\n");
        }
        done++;
    }
    fprintf(stderr, " End of flood!\n");
    for(iteration = 0; iteration < tree -> size; iteration++)
        fprintf(stderr, "%s\n", myLSPlist[iteration]);
    
    return myLSPlist;
}

int ** createMatrix(char ** myLSPlist, routerTree * tree)
{
    int ** array = (int **)malloc((1 + (tree -> size)) * (sizeof(int *)));
    int i, j;
    int k = 0;
    for(i = 0; i < tree -> size; i++)
    {
        array[i] = (int *)malloc((1 + (tree -> size)) * (sizeof(int)));
    }
    
    for(i = 0; i < tree -> size; i++)
    {
        for(j = 0; j < tree -> size; j++)
        {
            array[i][j] = -1;
        }
    }
    
    int neighbors = 0;
    
    for(i = 0; i < tree -> size; i++)
    {
        k = 8;
        array[i][i] = 0;
        neighbors = myLSPlist[i][6] - '0';
        for(j = 0; j < neighbors; j++)
        {
            array[i][routerNumber(myLSPlist[i][k])] = myLSPlist[i][k + 2] - '0';
            k += 4;
        }
    }
    
    for(i = 0; i < tree -> size; i++)
    {
        for(j = 0; j < tree -> size; j++)
            fprintf(stderr, "%i ", array[i][j]);
        fprintf(stderr, "\n");
    }
    return array;
}

/*
 int main(int argc, char ** argv)
 {
 argvInfo * myInfo = processArgv(argc, argv);
 FILE * fin;
 int numOfNeighbors = 0;
 fin = fopen(myInfo -> discoveryTextFile, "r");
 if(fin == NULL)
 {
 fprintf(stderr, "Could not open file: %s\n", myInfo -> discoveryTextFile);
 return -1;
 }
 routerTree * tree = createTree(myInfo -> totalNumberOfNodes, myInfo);
 numOfNeighbors = readIn(fin, tree);
 int listeningSocket = createListeningSocket(myInfo -> port);
 //printTreeNode(tree);
 fprintf(stderr, "#ofNeighbors: %i\nrouterLabel: %c\ntotalNumOfNodes: %i\ndiscoveryFile: %s\ndynamic: %s\n", numOfNeighbors, myInfo->routerLabel, myInfo->totalNumberOfNodes, myInfo->discoveryTextFile, myInfo->dynamic ? "true" : "false");
 int sockets[numOfNeighbors];
 struct sockaddr_in addresses[numOfNeighbors];
 int i = 0;
 int j = 0;
 for(i = 0; i < tree -> size; i++)
 {
 if(tree -> routerName[i] != '\0')
 {
 sockets[j] = createSocket(tree -> routerLocation[i], tree -> routerPort[i], &addresses[j]);
 j++;
 }
 }
 char * myLSP = createLSP(tree, numOfNeighbors);
 char ** myLSPlist = flood(tree, numOfNeighbors, listeningSocket, sockets, addresses, myLSP);
 int ** myLSPmatrix = createMatrix(myLSPlist, tree);
 //myLSPmatrix = myDijkstra(myLSPmatrix);
 //void printFinalTable(myLSPmatrix);
 return 0;
 }
 */
