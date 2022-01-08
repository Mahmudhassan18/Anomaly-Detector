

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

struct checksReport{
    int start;
    int end;
    string description;
    bool tp;
};

// you may add here helper classes
struct AnomalyShared{
    float threshold;
    vector<AnomalyReport> report;
    vector<checksReport> checksReport;
    int testFileSize;
    AnomalyShared(){
        threshold = 0.9;
        testFileSize = 0;
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
    UploadFile(DefaultIO* dio):Command(dio, "1.upload a time series csv file\n"){}
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
    AlgorithmSettings(DefaultIO* dio): Command(dio, "2.algorithm settings\n"){}
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

class Detect:public Command{
public:
    Detect(DefaultIO* dio):Command(dio,"3.detect anomalies\n"){}
    virtual void execute(AnomalyShared* sharedState){
        TimeSeries train("anomalyTrain.csv");
        TimeSeries test("anomalyTest.csv");
        sharedState->testFileSize = test.getNumberOfRows();
        HybridAnomalyDetector ad;
        ad.setCorrelationThreshold(sharedState->threshold);
        ad.learnNormal(train);
        sharedState->report = ad.detect(test);

        checksReport checksRep;
        checksRep.start=0;
        checksRep.end=0;
        checksRep.description="";
        checksRep.tp=false;
        for_each(sharedState->report.begin(),sharedState->report.end(),[&checksRep,sharedState](AnomalyReport& anomalyRep){
            if(anomalyRep.timeStep == checksRep.end + 1 && anomalyRep.description == checksRep.description)
                checksRep.end++;
            else{
                sharedState->checksReport.push_back(checksRep);
                checksRep.start = anomalyRep.timeStep;
                checksRep.end = checksRep.start;
                checksRep.description = anomalyRep.description;
            }
        });
        sharedState->checksReport.push_back(checksRep);
        sharedState->checksReport.erase(sharedState->checksReport.begin());

        dio->write("anomaly detection complete.\n");
    }
};

class Results: public Command{
public:
    Results(DefaultIO* dio): Command(dio, "4.display results\n"){}
    virtual void execute(AnomalyShared* anomalyShared){
        for_each(anomalyShared->report.begin(), anomalyShared->report.end(), [this](AnomalyReport& report){
          dio->write(report.timeStep);
          dio->write("\t" + report.description + "\n");
        });
        dio->write("Done.\n");
    }
};

class AnomalyUpload: public Command{
public:
    AnomalyUpload(DefaultIO* dio): Command(dio, "5.upload anomalies and analyze results\n"){}

    bool CS(int as, int ae, int bs, int be){
        return (ae>=bs && be>=as);
    }

    bool isTP(int start, int end,AnomalyShared* sharedState){
        for(size_t i=0;i<sharedState->checksReport.size();i++){
            checksReport fr=sharedState->checksReport[i];
            if(CS(start, end, fr.start, fr.end)){
                sharedState->checksReport[i].tp=true;
                return true;
            }
        }
        return false;
    }

    virtual void execute(AnomalyShared* sharedState){

        for(size_t i=0;i<sharedState->checksReport.size();i++){
            sharedState->checksReport[i].tp=false;
        }

        dio->write("Please upload your local anomalies file.\n");
        string s = "";
        float truePositive=0, sum=0, positive=0;
        while((s=dio->read())!="done"){
            size_t t=0;
            for(;s[t]!=',';t++);
            string st = s.substr(0, t);
            string en = s.substr(t+1, s.length());
            int start = stoi(st);
            int end = stoi(en);
            if(isTP(start,end,sharedState))
                truePositive++;
            sum += end + 1 - start;
            positive++;
        }
        dio->write("Upload complete.\n");
        float falsePositive = 0;
        for(size_t i=0;i<sharedState->checksReport.size();i++)
            if(!sharedState->checksReport[i].tp)
                falsePositive++;
        float N=sharedState->testFileSize - sum;
        float truePositiveRate= ((int)(1000.0 * truePositive / positive)) / 1000.0f;
        float falsePositiveRate= ((int)(1000.0 * falsePositive / N)) / 1000.0f;
        dio->write("True Positive Rate: ");
        dio->write(truePositiveRate);
        dio->write("\nFalse Positive Rate: ");
        dio->write(falsePositiveRate);
        dio->write("\n");

    }
};

class Exit:public Command{
public:
    Exit(DefaultIO* dio):Command(dio,"6.exit\n"){}
    virtual void execute(AnomalyShared* sharedState){

    }
};


#endif /* COMMANDS_H_ */
