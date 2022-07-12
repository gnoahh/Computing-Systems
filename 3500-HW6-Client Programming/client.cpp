/*
 * Name: Hoang Do
 * Date: 06/03/2021
 * Program: client.cpp
 * Description: This file is to implement a networking client that can connect to a server to play a guessing game.
 * The user need to enter a number in 3 digit format in a range of 0 to 999. Once they guess correctly, the server
 * will send a leaderboard to see the top 3.
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <iostream>


const int MAX_SIZE = 100;
using namespace std;

//error response function
void error(const char* msg){
    perror(msg);
    exit(0);
}

//a helper method to check if the input is a valid number
bool checkNumber(string input){
    char dot = '.';
    for(int i = 0; i < input.length(); i++){
        if(!isdigit(input[i]) && input[i] != dot)
            return false;
    }
    return true;
}
int main(int argc, char * argv[]){

    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    if(argc < 3){
        fprintf(stderr, "usage %s hostname port", argv[0]);
        exit(0);
    }

    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if(sockfd < 0)
        error("ERROR opening socket");

    server = gethostbyname(argv[1]);

    if(server == NULL){
        fprintf(stderr, "ERROR, no such host");
        exit(0);
    }

    struct hostent *gethostbyname(char *name);
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *) & serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

    if(connect(sockfd, reinterpret_cast<const sockaddr *>(&serv_addr), sizeof(serv_addr)) < 0)
        error("ERROR connecting");

    string msgStr;
    char username[MAX_SIZE];
    printf("Welcome to Number Guessing Game! \n");
    printf("Enter your username:  ");
    cin >> username; //capture the username

    //send the username to the server
    n = send(sockfd, (void *) username, MAX_SIZE, 0);
    if (n < 0)
        error("ERROR writing to socket");
    long guessNum;
    std::string tempInput;
    bool stillIncorrect = true;
    int turnCount = 1;

    while(stillIncorrect){
        while(true){ //force the user to input a correct formatted number
            printf("\n\nTurn: %i", turnCount);
            printf("\nEnter your guess: ");
            cin >> tempInput;

            //check if the input is in the range of 0 - 999 and have at least a length of 3
            if(checkNumber(tempInput) && stoi(tempInput) >= 0 && stoi(tempInput) <= 999 && tempInput.size() >= 3){
                guessNum = stoi(tempInput);
                turnCount++;
                break;
            }
            else{
                printf("Invalid input! Please enter a number between 0 and 999 in 3 digit format!");
            }
        }
        //send the input number to the server
        long networkInt = htonl(guessNum);
        int bytesSent = send(sockfd, (void *) &networkInt, sizeof(long), 0);
        if(bytesSent != sizeof(long))
            exit(-1);

        //receive the response back from the server regarding the input
        int stringBytes = MAX_SIZE;
        char buffer[MAX_SIZE];
        char *bp = buffer;
        while(stringBytes){
            int n = recv(sockfd,(void*) bp,stringBytes, 0);
            if(n < 0)
                error("ERROR reading from socket\n");

            stringBytes = stringBytes - n;
            bp = bp + n;
        }
        printf("Result of guess: ");
        printf(buffer);

        //only output the congratulation message once the number is guess correctly
        char congrats[MAX_SIZE];
        string msg = "Correct guess!";
        strcpy(congrats, msg.c_str());
        if(strcmp(buffer,congrats) == 0){
            string temp = "\nCongratulations! It took %i turn(s) to guess the number!\n";
            printf(temp.c_str(), turnCount - 1);
            break;
        }


    }
    //print out the leaderboard
    char board[MAX_SIZE];
    char *boardp = board;
    int bytes = MAX_SIZE;
    while(bytes){
        int n = recv(sockfd,(void*) boardp,bytes, 0);
        if(n < 0)
            error("ERROR reading from socket");

        bytes = bytes - n;
        boardp = boardp + n;
    }
    printf("\nLeader Board:\n");
    printf(board);


    return 0;
}

