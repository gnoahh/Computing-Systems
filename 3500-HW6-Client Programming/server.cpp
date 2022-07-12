/*
 * Date: 06/03/2021
 * Program: server.cpp
 * Description: This file is to implement a networking server that can accept multiple clients to play the guessing
 * game. It will constantly respond to the client with appropriate message and send a leaderboard once guessed correctly
 *
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <vector>
#include <algorithm>

const int MAX_SIZE = 100;
pthread_mutex_t queueMutex;
using namespace std;

void error(const char *msg){
    perror(msg);
    exit(1);
}

struct playerInfo{
    string username;
    int turn;
    int orderNum;

    playerInfo operator=(const playerInfo rhs){ //copy assignment
        username = rhs.username;
        turn = rhs.turn;
        orderNum = rhs.orderNum;
        return *this;
    }
};

//a simple comparing helper method
//This will help order the leaderboard in an incrementing turn fashion
//In case of ties, which ever client arrives first will get the priority
bool compareTurn(playerInfo lhs, playerInfo rhs){
    if(lhs.orderNum == rhs.orderNum)
        return lhs.orderNum < rhs.orderNum;
    return lhs.turn < rhs.turn;
}
vector<playerInfo> leaderBoard; //a vector to hold the record of all number input
int order = 0;

void updateLeaderboard(string name, int turn){
    pthread_mutex_lock(&queueMutex); //only allow 1 thread to update the leaderboard at a time

    playerInfo player;
    player.username = name;
    player.turn = turn;
    player.orderNum = order;
    leaderBoard.push_back(player);
    std::sort(leaderBoard.begin(), leaderBoard.end(), &compareTurn);

    pthread_mutex_unlock(&queueMutex);
}
string printLeaderBoard(){
    string boardInfo = "";
    int size = leaderBoard.size() < 3 ? leaderBoard.size() : 3;

    //create a string that holds the leaderboard to return
    //only output the first 3 entries or the current entries
    for(int i = 0; i < size; i++){
        boardInfo += to_string((i + 1)) + ". " + leaderBoard[i].username + " " + to_string(leaderBoard[i].turn) + "\n";
    }
    return boardInfo;
}
struct ThreadArgs{
    int clientSock;

};
void processClient(int clientSock){
    int randomNum = rand() % 999;
    char buffer[MAX_SIZE];
    bzero(buffer, MAX_SIZE);
    printf("Client %i enters\n", clientSock);
    printf("The random number is: %i\n", randomNum);
    order++; //the order of which client gets in first

    //read the username from the client and print it out
    int stringBytes = MAX_SIZE;
    char *bp = buffer;
    while(stringBytes){
        int n = recv(clientSock,(void*) bp,stringBytes, 0);
        if(n < 0)
            printf("ERROR reading from socket");

        stringBytes = stringBytes - n;
        bp = bp + n;
    }
    printf("The username is: %s\n",buffer);

    //This loop is the back bone of the thread to have it repeat forever until the client enters the correct number
    //It will go out of loop if client exits early
    bool stillIncorrect = true;
    int turnCount;
    while(stillIncorrect){

        int bytesLeft = sizeof(long);
        long networkInt;

        char *np = (char *) &networkInt;
        bool exit = false;
        while(bytesLeft){
            int bytesRecv = recv(clientSock, np, bytesLeft, 0);
            if(bytesRecv <= 0){
                printf("Client %i exits \n", clientSock);
                exit = true;
                break;

            }
            bytesLeft = bytesLeft - bytesRecv;
            np = np + bytesRecv;
        }
        if(exit) //safety mechanism
            break;

        long hostInt = ntohl(networkInt);
        printf("Client %i entered number: %i\n", clientSock, hostInt);
        turnCount++; //increment the turn

        char clientRes[MAX_SIZE];
        string tempStr;
        string boardResult;
        bool success = false;
        if(hostInt == randomNum){
            tempStr = "Correct guess!"; //correct
            updateLeaderboard(buffer, turnCount);
            boardResult = printLeaderBoard();
            printf(boardResult.c_str()); //print the leaderboard to the server
            stillIncorrect = false;
            success = true;
        }
        else{
            if(hostInt < randomNum){
                tempStr = "Too low";
            }
            else{
                tempStr = "Too high";
            }
        }
        //send the corresponding response to the user number input
        strcpy(clientRes, tempStr.c_str());
        int n = send(clientSock, (void *) clientRes, MAX_SIZE, 0);
        if (n < 0){
            printf("Message sent fail!");
            exit = true;
        }

        //send the leaderboard to the client once the game is done
        if(success == true){
            char board[MAX_SIZE];
            strcpy(board, boardResult.c_str());
            int message = send(clientSock, (void *) board, MAX_SIZE, 0);
            if (message < 0){
                printf("Message sent fail!");
                exit = true;
            }
        }
        if(exit)
            break;
    }

}
void *threadMain(void *args){

    struct ThreadArgs *threadArgs = (struct ThreadArgs *) args;
    int clientSock = threadArgs ->clientSock;
    delete threadArgs;
    processClient(clientSock);

    printf("Closing the socket %i\n", clientSock); //notify the server once the client exits
    pthread_detach(pthread_self());
    close(clientSock);

    return NULL;
}
int main(int argc, char *argv[]) {
    pthread_mutex_init(&queueMutex, NULL);

    int sockfd, portno, clientSock;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;

    if (argc < 2) {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if (::bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR on binding");
    listen(sockfd,5);
    clilen = sizeof(cli_addr);
    while(true){

        struct sockaddr_in clientAddr;
        socklen_t addrLen = sizeof(clientAddr);

        listen(sockfd,5);
        clientSock = accept(sockfd, (struct sockaddr *) & clientAddr, &addrLen);

        if(clientSock < 0){
            error("ERROR on accept");
        }
        //send the client socket number to the thread for further computation
        struct ThreadArgs *threadArgs = new ThreadArgs;
        threadArgs->clientSock = clientSock;

        pthread_t threadID;
        int status = pthread_create(&threadID, NULL, threadMain, (void *) threadArgs);
        if(status != 0)
            printf("I'm out!");
    }

    close(sockfd);
    return 0;
}

