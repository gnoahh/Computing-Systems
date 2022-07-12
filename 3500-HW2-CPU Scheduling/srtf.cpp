//
// Created by Hoang Do on 4/21/21.
//
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
using namespace std;

struct process{
    int id, burst, arrival, rmBurst, termTime, turnTime, waitTime;
    process(int _id, int _burst, int _arrival) {
        id = _id;
        burst = rmBurst = _burst;
        arrival = _arrival;
    }

    void calculateTurnTime(){
        turnTime = termTime - arrival;
    }

    void calulateWaitTime(){
        waitTime = turnTime - burst;
    }

    void printContent(){
        cout << "ID is: " << id << " Remaining burst is: " << rmBurst << "Arrival time: " << arrival << "Termination time: " << termTime << endl;
    }
};

bool compareArrival(process p1, process p2){
    return (p1.arrival < p2.arrival);
}


int main(int argc, char* argv[]){
    vector<process> myVector;
    vector<process> completedProcess;
    vector<process> orderProcess;
    ifstream file(argv[1]);
    string line, sep, id, burst, arrival;
    if(argc != 2){
        cerr << "Usage: " << argv[0] << "File name: " << endl;
        cerr << "Please enter the program name and file name!" << endl;
        exit(3);
    }
    if (!file.is_open()) {
        cout << "Failed to open the file" << endl;
        return 0;
    }
    while(getline(file, line)){
        stringstream sep(line);
        getline(sep, id, ',');
        getline(sep, burst, ',');
        getline(sep, arrival, ',');
        myVector.push_back(process(stoi(id), stoi(burst), stoi(arrival)));
    }

    sort(myVector.begin(), myVector.end(), compareArrival);
    int minBurst, index, completed, time;
    bool check = false;
    completed = 0;
    time = 0;
    index = 0;
    minBurst = 1000;
    while(completed != myVector.size()){
        for(int i = 0; i < myVector.size(); i++){
            if(myVector.at(i).arrival <= time && myVector.at(i).rmBurst < minBurst && myVector.at(i).rmBurst > 0){
//                if(myVector.at(i).rmBurst == myVector.at(index).rmBurst){
//                    cout << "ID is : " << myVector.at(i).id;
//                    index = myVector.at(i).id < myVector.at(index).id ? myVector.at(i).id : myVector.at(index).id;
//                    minBurst = myVector.at(i).rmBurst;
//                    check = true;
//                }else{
                index = i;
                minBurst = myVector.at(i).rmBurst;
                check = true;
//                }


            }
        }

        if(check == false){
            time++;
            continue;
        }

        myVector.at(index).rmBurst--;

        minBurst = myVector.at(index).rmBurst;


        if(minBurst == 0){
            minBurst = 1000;
        }

        if(myVector.at(index).rmBurst ==0){
            completed++;
            myVector.at(index).termTime = time + 1;
            orderProcess.push_back(myVector.at(index));
            check = false;


        }
        time++;
    }

    double sumTurnaround;
    double sumWait;
    for(int i = 0; i < orderProcess.size(); i++){
        orderProcess.at(i).calculateTurnTime();
        orderProcess.at(i).calulateWaitTime();
        sumTurnaround += orderProcess.at(i).turnTime;
        sumWait += orderProcess.at(i).waitTime;
    }

    cout << "Average turn around for all the jobs is: " << sumTurnaround/orderProcess.size() << endl;
    cout << "Average waiting time for all the jobs is: " << sumWait/orderProcess.size() << endl;
//    for(int i = 0; i < myVector.size(); i++){
//        myVector.at(i).printContent();
//    }
    for(int i = 0; i < orderProcess.size(); i++){
        cout << orderProcess.at(i).id << "(" << orderProcess.at(i).termTime << "), ";
        if(i == orderProcess.size() - 1){
            cout << orderProcess.at(i).id << "(" << orderProcess.at(i).termTime << ")" << endl;
        }

    }

}