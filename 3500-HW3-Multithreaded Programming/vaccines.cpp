/*
 * Hoang Do
 * vaccines.cpp
 * 05/07/2021
 * Description: This program is to create 21 threads and each thread will read a text file to count how many vaccines
 * there are. Then once all the threads are done, print out the sum of each vaccine.
 *
 */

#include <iostream>
#include <pthread.h>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <unistd.h>

using namespace std;

#define NUM_THREADS 21

struct Vaccine{
    int v1,v2,v3;
};
void *readFile(void * threadid){

    //data initialization
    string fileName;
    long tid;
    tid = (long)threadid;
    fileName = "/home/fac/mishraa/cpsc3500/pa3a/city" + to_string(tid + 1) + ".txt";
    ifstream myFile(fileName);
    string line;

    //Check if file can be opened
    if(!myFile.is_open()){
        cout << "Failed to open the file" << endl;
    }

    //store input data
    vector<string> vaccines;
    while(getline(myFile,line)){
        vaccines.push_back(line);
    }

    Vaccine vac;
    //increment counts for each vaccines
    for(int i = 0; i < vaccines.size(); i++){
        if(vaccines[i] == "v1")
            vac.v1++;
        else if(vaccines[i] == "v2")
            vac.v2++;
        else
            vac.v3++;
    }


    Vaccine *vac_p = new Vaccine;
    *vac_p = vac;

    return (void *) vac_p;

}



int main() {

    //data initialization
    pthread_t threads[NUM_THREADS];
    int rc;

    //create threads
    for(int i = 0; i < NUM_THREADS; i++){
        rc = pthread_create(&threads[i], NULL, readFile, (void*) i);

        if(rc){
            cout << "Error: unable to create thread, " << rc << endl;
            exit(-1);

        }
    }

    Vaccine *p;
    int v1,v2,v3;
    v1 = v2 = v3 = 0;
    Vaccine **dp = &p;
    //join threads to get return values and accumulate them all
    for(int i = 0; i < NUM_THREADS; i++){
        rc = pthread_join(threads[i], (void **) dp);
        v1 += p->v1;
        v2 += p->v2;
        v3 += p->v3;
    }

    cout << "V1: " << v1 << "\t V2: " << v2 << "\t V3: " << v3 <<endl;
    pthread_exit(NULL);

}
