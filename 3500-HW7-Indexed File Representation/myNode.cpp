#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

struct directBlock{
    int blockNum;
    string data = "";
    string nameFile = "";
};

struct singleIndirect{
    directBlock singleArr[100];
    string outerName;
};

struct doubleIndirect{
    singleIndirect doubleArr[100];
    string outerName;
    string doubleStr = "doubleIndirect";
};
struct Node{
    int currentCount;
    directBlock firstTwelve[12];
    singleIndirect singleBlock;
    doubleIndirect doubleBlock;
};

const string twelve = "twelve";
const string thirdteen = "thirteen";
const int maxSize = 10112;
int main(int argc, char* argv[]) {
    Node myNode;
    int blockCount = 0;
    if(argc != 3){
        cerr << "Please enter the input file and trace file:" << endl;
        exit(3);
    }
    string inputFile = argv[1];
    string traceFile = argv[2];
    ifstream my_inputFile(inputFile);
    ifstream my_traceFile(traceFile);

    if(!my_inputFile.is_open()){
        cout << "Failed to open the input file" << endl;
        return 0;
    }
    string blockNumber, data, line, fileName;
    int outerIndex = 0;
    int fileNum = 0;
    int secondFilenum = 0;
    int thirdOuter = 0;
    getline(my_inputFile, line);
    int lineCount = 0;
    while(getline(my_inputFile, line)){
        lineCount++;
        if(lineCount > maxSize){
            cout << "This input file is larger than the file system!";
            return 0;
        }
        stringstream sep(line);
        getline(sep, blockNumber, ',');
        getline(sep, data);
        if(blockCount <= 11){
            myNode.firstTwelve[blockCount].blockNum = blockCount;
            myNode.firstTwelve[blockCount].data = data;
            myNode.currentCount = blockCount;
            if(blockCount == 0){
                fileName = "zero.txt";
            }
            else if(blockCount == 1){
                fileName = "one.txt";
            }
            else if(blockCount == 2){
                fileName = "two.txt";
            }
            else if(blockCount == 3){
                fileName = "three.txt";
            }
            else if(blockCount == 4){
                fileName = "four.txt";
            }
            else if(blockCount == 5){
                fileName = "five.txt";
            }
            else if(blockCount == 6){
                fileName = "six.txt";
            }
            else if(blockCount == 7){
                fileName = "seven.txt";
            }
            else if(blockCount == 8){
                fileName = "eight.txt";
            }
            else if(blockCount == 9){
                fileName = "nine.txt";
            }
            else if(blockCount == 10){
                fileName = "ten.txt";
            }
            else{
                fileName = "eleven.txt";
            }
            myNode.firstTwelve[blockCount].nameFile = fileName;

        }
        else if(blockCount >= 12 && blockCount < 112){
            fileName = twelve + "_" + to_string(fileNum) + ".txt";
            myNode.singleBlock.outerName = twelve + ".txt";
            myNode.singleBlock.singleArr[fileNum].blockNum = blockCount;
            myNode.singleBlock.singleArr[fileNum].data = data;
            myNode.singleBlock.singleArr[fileNum].nameFile = fileName;
            myNode.currentCount = blockCount;
            fileNum++;
        }
        else{
            fileName = thirdteen + "_" + to_string(outerIndex) + "_" + to_string(secondFilenum) + ".txt";
            myNode.doubleBlock.outerName = thirdteen + ".txt";
            myNode.doubleBlock.doubleArr[outerIndex].singleArr[secondFilenum].blockNum = blockCount;
            myNode.doubleBlock.doubleArr[outerIndex].singleArr[secondFilenum].data = data;
            myNode.doubleBlock.doubleArr[outerIndex].singleArr[secondFilenum].nameFile = fileName;
            myNode.doubleBlock.doubleArr[outerIndex].outerName = thirdteen + "_" +  to_string(outerIndex) + ".txt";
            myNode.currentCount = blockCount;
            secondFilenum++;
            if(secondFilenum == 100){
                outerIndex++;
                secondFilenum = 0;
            }

        }
        myNode.currentCount++;
        blockCount++;
    }

    if(!my_traceFile.is_open()){
        cout << "Failed to open the trace file" << endl;
        return 0;
    }
    string request, editContent, line1;
    while(getline(my_traceFile, line1)) {
        stringstream sep(line1);
        getline(sep, request, ',');
        getline(sep, blockNumber);
        getline(sep, editContent, ',');
//        getline(sep, editContent);
        cout << "Request: " << request;
        cout << "Block: " << stoi(blockNumber) << endl;
        cout << "Content: " << editContent;
    }

//    my_inputFile.close();
//    for(int i = 0; i < 12; i ++){
//        cout << "Block number: " << myNode.firstTwelve[i].blockNum;
//        cout << " Data: " << myNode.firstTwelve[i].data << endl;
//        cout << "Name file: " << myNode.firstTwelve[i].nameFile << endl;
//    }
//
//    cout << "Now it comes second" << endl;
//    for(int i = 0; i < 100; i++){
//        cout << "Block number: " << myNode.singleBlock.singleArr[i].blockNum;
//        cout << " Data: " << myNode.singleBlock.singleArr[i].data <<endl;
//        cout << "Name file: " << myNode.singleBlock.singleArr[i].nameFile << endl;
//    }
//
//    cout << "Now it comes third" << endl;
//    for(int i = 0; i < 100; i++){
//        cout << "Block number: " << myNode.doubleBlock.doubleArr[0].singleArr[i].blockNum;
//        cout << " Data: " << myNode.doubleBlock.doubleArr[0].singleArr[i].data <<endl;
//        cout << " Data: " << myNode.doubleBlock.doubleArr[0].singleArr[i].nameFile <<endl;
//    }
//    cout << "Now it comes third one" << endl;
//    for(int i = 0; i < 10; i++){
//        cout << "Block number: " << myNode.doubleBlock.doubleArr[1].singleArr[i].blockNum;
//        cout << " Data: " << myNode.doubleBlock.doubleArr[1].singleArr[i].data <<endl;
//        cout << " Data: " << myNode.doubleBlock.doubleArr[1].singleArr[i].nameFile <<endl;
//    }
//    cout << "Level: " << myNode.doubleBlock.doubleArr[1].outerName;
    return 0;
}
