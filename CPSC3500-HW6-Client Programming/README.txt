Run server.cpp and client.cpp on CS1

server.cpp:

g++ -o server server.cpp -std=c++1 -lpthread
./server 4500

The port number can be any 

client.cpp:

g++ -o client client.cpp
./client 10.124.72.20 4500

The port number here needs to be the same as the one above for connection