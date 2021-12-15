

#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

class HybridAnomalyDetector:public SimpleAnomalyDetector {
public:
	HybridAnomalyDetector();
	virtual ~HybridAnomalyDetector();

	virtual void correlationCheck(const TimeSeries &ts, float pearson, string firstCur, string secCur, Point **points);
	virtual bool isAnomalous(float x,float y,correlatedFeatures c);

};

#endif /* HYBRIDANOMALYDETECTOR_H_ */

