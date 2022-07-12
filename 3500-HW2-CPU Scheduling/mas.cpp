#include <iostream>
#include <queue>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
using namespace std;

const int quantumT0 = 6;
const int quantumT1 = 12;
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
        cout << "ID is: " << id << " Burst is: " << burst << " Arrival time is: " << arrival << endl;
    }
};

bool compareArrival(process p1, process p2){
    return (p1.arrival < p2.arrival);
}
int main(int argc, char* argv[]) {
    if(argc != 2){

        cerr << "Usage: " << argv[0] << "File name: " << endl;
        cerr << "Please enter the program name and file name!" << endl;
        exit(3);
    }
    fstream myFile("jobs.txt");
    int myArr[3];
    string line;
    string id, burst, arrival;
    vector<process> myVector;
    vector<process> orderProcess;
    vector<process> completedProcess;
    queue<process> q, q0, q1, q2;
    if (!myFile.is_open()) {
        cout << "Failed to open the file" << endl;
        return 0;
    }

    while (getline(myFile, line)) {
        stringstream sep(line);
        getline(sep, id, ',');
        getline(sep, burst, ',');
        getline(sep, arrival, ',');
        myVector.push_back(process(stoi(id), stoi(burst), stoi(arrival)));

    }

    //sort the arrival time
    sort(myVector.begin(), myVector.end(), compareArrival);

    for (int i = 0; i < 20; i++) {
        myVector.at(i).printContent();
        q0.push(myVector.at(i));
    }

    int time = 0;
    bool completed = false;

    while(!completed){
        if(!q0.empty()) {
            if (q0.front().rmBurst > 0) {
                if (q0.front().rmBurst > quantumT0) {
                    time += 6;
                    q0.front().rmBurst -= quantumT0;
                    q1.push(q0.front());
                    orderProcess.push_back(q0.front());
                    q0.pop();
                } else {
                    time = time + q0.front().rmBurst;
                    q0.front().rmBurst = 0;
                    q0.front().termTime = time;
                    orderProcess.push_back(q0.front());
                    completedProcess.push_back(q0.front());
                    q0.pop();
                }
            }
        }
        if(q0.empty()){
            if(!q1.empty()){
                if (q1.front().rmBurst > 0) {
                    if (q1.front().rmBurst > quantumT1) {
                        time += quantumT1;
                        q1.front().rmBurst -= quantumT1;
                        q2.push(q1.front());
                        orderProcess.push_back(q1.front());
                        q1.pop();
                    } else {
                        time = time + q1.front().rmBurst;
                        q1.front().rmBurst = 0;
                        q1.front().termTime = time;
                        orderProcess.push_back(q1.front());
                        completedProcess.push_back(q1.front());
                        q1.pop();
                    }
                }
            }
            else{
                if(!q2.empty()){
                    if(q2.front().rmBurst > 0){
                        time = time + q2.front().rmBurst;
                        q2.front().rmBurst = 0;
                        q2.front().termTime = time;
                        orderProcess.push_back(q2.front());
                        completedProcess.push_back(q2.front());
                        q2.pop();
                    }
                }
                if(q2.empty()){
                    break;
                }
            }


        }

    }

    double sumTurnaround;
    double sumWait;

    for(int i = 0; i < completedProcess.size(); i++){
        completedProcess.at(i).calculateTurnTime();
        completedProcess.at(i).calulateWaitTime();
        sumTurnaround += completedProcess.at(i).turnTime;
        sumWait += completedProcess.at(i).waitTime;
    }

    cout << "Average turn around for all the jobs is: " << sumTurnaround/completedProcess.size() << endl;
    cout << "Average waiting time for all the jobs is: " << sumWait/completedProcess.size() << endl;
    for(int i = 0; i < orderProcess.size(); i++){
        for(int j = 0; j < completedProcess.size(); j++){
            if(orderProcess.at(i).id == completedProcess.at(j).id){
                orderProcess.at(i) = completedProcess.at(j);
            }
        }
    }


    for(int i = 0; i < orderProcess.size(); i++){
        cout << orderProcess.at(i).id << "(" << orderProcess.at(i).termTime << "), ";
        if(i == orderProcess.size() - 1){
            cout << orderProcess.at(i).id << "(" << orderProcess.at(i).termTime << ")";
        }

    }

}
