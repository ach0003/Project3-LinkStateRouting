/*
 * router.h
 * Systems and Networks II
 * Project 3
 * Jeff Morton and Andrew Havard
 *
 * This file describes the functions to be implemented by the UDPclient.
 * You may also implement any helper functions you deem necessary to complete the program.
 */

/*
 * Creates a datagram socket.
 *
 * return value - the socket identifier or a negative number indicating the error for creating a socket
 */
int createSocket();

/*
 * Sends a request for service to the server. This is an asynchronous call to the server,
 * so do not wait for a reply in this function.
 *
 * sockFD     - the socket identifier
 * request    - the request to be sent encoded as a string
 * serverName - the ip address or hostname of the server given as a string
 * serverPort - the port number of the server
 *
 * return   - 0, if no error; otherwise, a negative number indicating the error
 */
int sendRequest(int sockFD, char * request, char * serverName, int serverPort);

/*
 * Receives the server's response formatted as an XML text string.
 *
 * sockfd    - the socket identifier
 * response  - the server's response as an XML formatted string to be filled in by this function into the specified string array
 * size      - the size of the buffer for storing a response from the server
 *
 * return   - 0, if no error; otherwise, a negative number indicating the error
 */
int receiveResponse(int sockFD, char * response, int size);

/*
 * Prints the response to the screen in a formatted way.
 *
 * response - the server's response as an XML formatted string
 *
 */
void printResponse(char * response);

/*
 * Closes the specified socket
 *
 * sockFD - the ID of the socket to be closed
 *
 * return - 0, if no error; otherwise, a negative number indicating the error
 */
int closeSocket(int sockFD);

