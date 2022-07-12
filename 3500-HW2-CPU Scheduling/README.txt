To compile mas.cpp and srtf.cpp type:

g++ -o mas mas.cpp
./mas jobs.txt

Or

g++ -o rtf srtf.cpp
./srtf jobs.txt

The mas.cpp and srtf.cpp implementations fail to address preemption. The designer attempted to include preemption conditions but both programs fail to do so. For srtf, I fail to update the ID if the remaining burst is equal even though I have an if statement intended to do so. Perhaps with more time, I would try to put more print statements and track to see where it goes wrong and put the conditions accordingly. For the mas part, time is incremented by time quantum and I have not check every ms to see if there's any incoming processes. That's something I may need to do if I have more time.