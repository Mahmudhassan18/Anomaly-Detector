
#include "HybridAnomalyDetector.h"

HybridAnomalyDetector::HybridAnomalyDetector() {
    // TODO
}

HybridAnomalyDetector::~HybridAnomalyDetector() {
	// TODO Auto-generated destructor stub
}

void HybridAnomalyDetector::correlationCheck(const TimeSeries &ts, float pearson, string firstCur, string secCur,
                                             Point **points) {
    SimpleAnomalyDetector::correlationCheck(ts, pearson, firstCur, secCur, points);
    if (pearson > 0.5 && pearson < threshold){
        correlatedFeatures c;
        Circle minCirc = findMinCircle(points, ts.getNumberOfRows());
        c.feature1 = firstCur;
        c.feature2 = secCur;
        c.corrlation = pearson;
        c.threshold = minCirc.radius * 1.1;
        c.x = minCirc.center.x;
        c.y = minCirc.center.y;
        cf.push_back(c);
    }

}

bool HybridAnomalyDetector::isAnomalous(float x, float y, correlatedFeatures c) {
    return (c.corrlation >= threshold && SimpleAnomalyDetector::isAnomalous(x, y, c) ||
    c.corrlation > 0.5 && c.corrlation < threshold && twoPointsDis(Point(c.x, c.y), Point(x, y)) > c.threshold);
}

