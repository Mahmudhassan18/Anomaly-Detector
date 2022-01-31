#include "Server.h"

string serverIO::read(){
    char c = 0;
    string s = "";
    while(c != '\n'){
        recv(clientID, &c, sizeof(char), 0);
        s += c;
    }
    return s;
}

void serverIO::write(string text){
    const char* txt = text.c_str();
    send(clientID,txt,strlen(txt),0);
}

void serverIO::write(float f){
    ostringstream ss;
    ss << f;
    string s(ss.str());
    write(s);
}

void serverIO::read(float* f){

}

Server::Server(int port)throw (const char*) {
    flag = false;
    fileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (fileDescriptor < 0)
        throw "Socket failed";

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);

    if(::bind(fileDescriptor, (struct sockaddr*)&server, sizeof(server)) < 0)
        throw "bind failed";
    if (listen(fileDescriptor, 3) < 0)
        throw "listen failed";

};

void Server::start(ClientHandler& ch) throw(const char*){
    t = new thread([&ch, this]() {
        while (!flag) {
            socklen_t clientSize = sizeof(client);
            alarm(1);
            int aClient = accept(fileDescriptor, (struct sockaddr *) &client, &clientSize);
            if (aClient < 0)
                throw "Accept failed";
            ch.handle(aClient);
            close(aClient);
            alarm(0);
        }
        close(fileDescriptor);
    });
}

void Server::stop(){
    flag = true;
	t->join(); // do not delete this!
}

Server::~Server() {

}

