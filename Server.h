
#ifndef SERVER_H_
#define SERVER_H_

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>

#include <pthread.h>
#include <thread>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include<signal.h>
#include <sstream>

#include "CLI.h"
using namespace std;

// edit your ClientHandler interface here:
class ClientHandler{
    public:
    virtual void handle(int clientID)=0;
};


// you can add helper classes here and implement on the cpp file
class serverIO:public DefaultIO{
    int clientID;
public:

    serverIO(int clientID):clientID(clientID){}

    virtual string read();
    virtual void write(string text);

    virtual void write(float f);

    virtual void read(float* f);

};


// edit your AnomalyDetectionHandler class here
class AnomalyDetectionHandler:public ClientHandler{
	public:
    virtual void handle(int clientID){
        serverIO sio(clientID);
        CLI cli(&sio);
        cli.start();
    }
};


// implement on Server.cpp
class Server {
	thread* t{}; // the thread to run the start() method in
	int fileDescriptor;
	sockaddr_in server{};
	sockaddr_in client{};
	bool flag;
public:
	Server(int port) throw (const char*);
	virtual ~Server();
	void start(ClientHandler& ch)throw(const char*);
	void stop();
};

#endif /* SERVER_H_ */
