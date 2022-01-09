#include "CLI.h"

CLI::CLI(DefaultIO* dio) {
    this->dio = dio;
    commands.push_back(new UploadFile(dio));
    commands.push_back(new AlgorithmSettings(dio));
    commands.push_back(new Detect(dio));
    commands.push_back(new Results(dio));
    commands.push_back(new AnomalyUpload(dio));
    commands.push_back(new Exit(dio));
}

void CLI::start(){
    AnomalyShared anomalyShared;
    int index=-1;
    while(index!=5){
        dio->write("Welcome to the Anomaly Detection Server.\n");
        dio->write("Please choose an option:\n");
        for(size_t i=0;i<commands.size();i++){
            dio->write(commands[i]->description);
        }
        string input = dio->read();
        index=input[0]-'0'-1;
        if(index>=0 && index<=6)
            commands[index]->execute(&anomalyShared);
    }
}

CLI::~CLI() {
}

