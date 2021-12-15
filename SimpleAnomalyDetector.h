

#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_

#include "anomaly_detection_util.h"
#include "AnomalyDetector.h"
#include <vector>
#include <algorithm>
#include <string.h>
#include <math.h>

struct correlatedFeatures{
	string feature1,feature2;  // names of the correlated features
	float corrlation;
	Line lin_reg;
	float threshold;
	float x, y;
};


class SimpleAnomalyDetector:public TimeSeriesAnomalyDetector{
protected:
	vector<correlatedFeatures> cf;
	float threshold;
public:
	SimpleAnomalyDetector();
	virtual ~SimpleAnomalyDetector();
	virtual void learnNormal(const TimeSeries& ts);
	virtual vector<AnomalyReport> detect(const TimeSeries& ts);
	vector<correlatedFeatures> getNormalModel(){
		return cf;
	}
protected:
    virtual void correlationCheck(const TimeSeries& ts, float pearson, string firstCur, string secCur, Point** points);
    virtual bool isAnomalous(float x, float y, correlatedFeatures c);
    Point** fromVectorToPoints(vector<float> x, vector<float> y);
    float calculateThreshold(Line line, Point** points, int len);
};



#endif /* SIMPLEANOMALYDETECTOR_H_ */
