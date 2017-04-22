//  SysNets II
//  Dr. Reichherzer
//  Project 3
// Jeff Morton and Andrew Havard

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>

#ifndef FILES_H_
#define FILES_H_

/* tree struct for the routers */
typedef struct tree
{
    int iterator;
    int size;
    char * routerName;
    char ** routerLocation;
    unsigned short * routerPort;
    int * distance;
    char self;
    
} routerTree;

/* argument struct for argument parameters */
typedef struct argvInfo
{
    char routerLabel;
    unsigned short port;
    int totalNumberOfNodes;
    char * discoveryTextFile;
    bool dynamic;
}argvInfo;

/* createTree
 *  creates a tree struct given the argument struct
 *
 */
routerTree * createTree(int, argvInfo *);

/* createCharArray
 *  creates an array of chars
 *
 */
char * createOneDimCharArray(int numberOfItems, int sizeItem);

/* createCharArray2
 *  creates a 2d char array
 *
 */
char ** createCharArray2(int numberOfItems, int sizeItem);

/* createIntArray
 *  creates integer pointer (array) given
 *  number of items
 */
int * createIntArray(int numberOfItems);

/* createUshortArray
 *  creates an unsigned long array given
 *  number of items
 */
unsigned short * createUshortArray(int numberOfItems);

/* readIn
 *  reads the file containg to our routing
 *  structure.
 *
 */
int readIn(FILE * fin, routerTree * tree);

/* parseLine
 *  parses string for tree struct
 *
 */
void parseLine(char * str, routerTree * tree);

/* copyToBuffer
 *  copies information from one buffer
 *  to the other
 */
void copyToBuffer(char * temp, char * str, int * iter, int size);

/* routerNumber
 *  according to the char passed, returns router
 *  number if uppercase character
 */
int routerNumber(char aph);

/* printTreeNode
 *  prints information contained within
 *  passed in routerTree struct
 */
void printTreeNode(routerTree * tree);

/* processArgv
 *  processes arguments and handles them
 *  accordingly
 */
argvInfo * processArgv(int argc, char ** argv);

//Creates a socket to connect to
//int createSocket(char *, unsigned short, struct sockaddr_in *);

/* createListeningSocket
 *  creates a socket in which to listen
 *  on.
 */
int createListeningSocket(unsigned short);

/* createLSP
 *  creates and returns the LSP for the unique
 *  integer
 */
char * createLSP(routerTree *, int);

/* flood
 *  simulated flooding of our data link network
 *
 */
char ** flood(routerTree *, int, int, int *, struct sockaddr_in *, char *);

/* createMatrix
 *  creates a matrix for use for the flood
 *
 */
int ** createMatrix(char **, routerTree *);

#endif /* FILES_H_ */
