

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <string.h>

#include <fstream>
#include <vector>
#include "HybridAnomalyDetector.h"

using namespace std;

class DefaultIO{
public:
	virtual string read()=0;
	virtual void write(string text)=0;
	virtual void write(float f)=0;
	virtual void read(float* f)=0;
	virtual ~DefaultIO(){}

	void readAndSave(string fileName){
        ofstream out(fileName);
        string s="";
        while((s=read())!="done"){
            out<<s<<endl;
        }
        out.close();
    }
};

// you may add here helper classes
struct AnomalyShared{
    float threshold;
    vector<AnomalyReport> report;
    AnomalyShared(){
        threshold = 0.9;
    };
};

// you may edit this class
class Command{
protected:
	DefaultIO* dio;
public:
    const string description;
	Command(DefaultIO* dio, const string description):dio(dio),description(description){}
	virtual void execute(AnomalyShared* anomalyShared)=0;
	virtual ~Command(){}
};

class UploadFile: public Command{
public:
    UploadFile(DefaultIO* dio):Command(dio, "1. upload a time series csv file\n"){}
    virtual void execute(AnomalyShared* anomalyShared) {
        dio->write("Please upload your local train CSV file.\n");
        dio->readAndSave("anomalyTrain.csv");
        dio->write("Upload complete.\n");
        dio->write("Please upload your local test CSV file.\n");
        dio->readAndSave("anomalyTest.csv");
        dio->write("Upload complete.\n");
    }
};

class AlgorithmSettings: public Command{
public:
    AlgorithmSettings(DefaultIO* dio): Command(dio, "2. algorithm settings\n"){}
    virtual void execute(AnomalyShared* anomalyShared) {
        bool flag = false;
        while(!flag) {
            dio->write("The current correlation threshold is ");
            dio->write(anomalyShared->threshold);
            dio->write("\nType a new threshold\n");
            float newThreshold;
            dio->read(&newThreshold);
            if (newThreshold >= 0 && newThreshold <= 1){
                flag = true;
                anomalyShared->threshold = newThreshold;
            }
            else{
                dio->write("please choose a value between 0 and 1.\n");
            }
        }
    }
};

class Detect: public Command{
public:
    Detect(DefaultIO* dio): Command(dio, "3. detect anomalies\n"){}
    virtual void execute(AnomalyShared* anomalyShared){
        TimeSeries trainTS("anomalyTrain.csv");
        TimeSeries testTS("anomalyTest.csv");
        HybridAnomalyDetector hybridDetector;
        hybridDetector.setCorrelationThreshold(anomalyShared->threshold);
        hybridDetector.learnNormal(trainTS);
        anomalyShared->report = hybridDetector.detect(testTS);
        dio->write("anomaly detection complete.\n");
    }
};

class Results: public Command{
public:
    Results(DefaultIO* dio): Command(dio, "4. display results\n"){}
    virtual void execute(AnomalyShared* anomalyShared){
        for_each(anomalyShared->report.begin(), anomalyShared->report.end(), [this](AnomalyReport& report){
          dio->write(report.timeStep);
          dio->write("\t" + report.description + "\n");
        });
        dio->write("Done.\n");
    }
};


#endif /* COMMANDS_H_ */
