#include "SimpleAnomalyDetector.h"

#include <utility>
//Sets the threshold
SimpleAnomalyDetector::SimpleAnomalyDetector() {
    threshold = 0.9;

}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {
    // TODO Auto-generated destructor stub
}
//Calculates the threshold and returns it
float SimpleAnomalyDetector::calculateThreshold(Line line, Point** points, int len){
    float maxThreshold = 0;
    for (int i = 0; i < len; ++i) {
        float curThreshold = abs(points[i]->y - line.f(points[i]->x));
        if (curThreshold > maxThreshold){
            maxThreshold = curThreshold;
        }
    }
    return maxThreshold;
}
//Returns a points created from two vectors
Point** SimpleAnomalyDetector::fromVectorToPoints(vector<float> x, vector<float> y){
    auto** points = new Point*[x.size()];
    for (int i = 0; i < x.size(); ++i) {
        points[i] = new Point(x[i], y[i]);
    }
    return points;
}

void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts){
    vector<string> dataVector=ts.getColumnsNames();
    size_t numberOfRows=ts.getNumberOfRows();

    float dataArray[dataVector.size()][numberOfRows];
    //Loop for inserting the data from ts into a 2d array
    for(size_t i=0;i<dataVector.size();i++){
        for(size_t j=0;j<ts.getNumberOfRows();j++){
            dataArray[i][j]=ts.getVectorFromMap(dataVector[i])[j];
        }
    }

    for(size_t i=0;i<dataVector.size();i++){
        float m = 0, p;
        int c = 0;
        for(size_t j=i+1;j<dataVector.size();j++){
            p=abs(pearson(dataArray[i], dataArray[j], numberOfRows));
            if(p > m){
                m = p;
                c = j;
            }
        }
        //Gets names of the columns (map keys)
        string firstCor=dataVector[i];
        string secCor=dataVector[c];
        Point** points = fromVectorToPoints(ts.getVectorFromMap(firstCor), ts.getVectorFromMap(secCor));

        correlationCheck(ts, m, firstCor, secCor, points);
    }
}
//If pearson bigger than threshold creates a correlatedFeature and inserts data into it
void SimpleAnomalyDetector::correlationCheck(const TimeSeries& ts, float pearson, string firstCur, string secCur, Point** points){
    if(pearson > threshold){
        size_t len=ts.getNumberOfRows();
        correlatedFeatures c;
        c.feature1 = firstCur;
        c.feature2 = secCur;
        c.corrlation=pearson;
        c.lin_reg=linear_reg(points, len);
        c.threshold = calculateThreshold(c.lin_reg, points, len) * 1.1;
        cf.push_back(c);
    }
}

//To detect any alarms and pushes them in anomaly report
vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts){
    vector<AnomalyReport> anomalyReportVec;
    //for loop overs cf vector
    for_each(cf.begin(), cf.end(), [&anomalyReportVec, &ts](correlatedFeatures c){
        vector<float> x = ts.getVectorFromMap(c.feature1);
        vector<float> y = ts.getVectorFromMap(c.feature2);
        for(int i=0; i < x.size(); i++) {
            if (isAnomalous(x[i], y[i], c)) {
                string d = c.feature1 + "-" + c.feature2;
                anomalyReportVec.push_back(AnomalyReport(d, (i + 1)));
            }
        }
    });
            return anomalyReportVec;
}
//Checks if anomalous and returns bool
bool SimpleAnomalyDetector::isAnomalous(float x, float y,correlatedFeatures c){
    return (abs(y - c.lin_reg.f(x)) > c.threshold);
}


