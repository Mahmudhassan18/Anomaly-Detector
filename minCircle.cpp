#include "minCircle.h"
#include "anomaly_detection_util.h"


using namespace std;

float twoPointsDis(const Point& p1, const Point& p2){
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

vector<Point> arrayToVector(Point** points, size_t size){
    vector<Point> pointsVec;
    for (int i = 0; i < size; ++i) {
        pointsVec.push_back(*points[i]);
    }
    return pointsVec;
}

bool isCircleValid(const vector<Point>& points, const Circle& c1){
    for (int i = 0; i < points.size(); ++i) {
        if (twoPointsDis(points[i], c1.center) > c1.radius){
            return false;
        }
    }
    return true;
}

Point circleCenter(const Point& p1, const Point& p2){
    float x = pow(p1.x, 2) + pow(p1.y, 2);
    float y = pow(p2.x, 2) + pow(p2.y, 2);
    float d = (p1.x * p2.y) - (p1.y * p2.x);
    return Point((p2.y * x - p1.y * y) / (2 * d), (p1.x * y - p2.x * x) / (2 * d));

}

Circle circleFrom(const Point& p1, const Point& p2){
    Point center((p1.x + p2.x) / 2.0, (p1.y + p2.y) / 2.0);
    return Circle(center, twoPointsDis(p1, p2) / 2.0);
}

Circle circleFrom(const Point& p1, const Point& p2, const Point& p3){
    Point center = circleCenter(Point(p2.x - p1.x, p2.y - p1.y), Point(p3.x - p1.x, p3.y - p1.y));
    center.x += p1.x;
    center.y += p1.y;
    return Circle(center, twoPointsDis(p1, center));
}

Circle minCircleCreator(const vector<Point>& boardPoints){
    if (boardPoints.empty()){
        return Circle(Point(0, 0), 0);
    }
    else if (boardPoints.size() == 1){
        return Circle(boardPoints[0], 0);
    }
    else if (boardPoints.size() == 2){
        return circleFrom(boardPoints[0], boardPoints[1]);
    }

    for (int i = 0; i < 3; ++i) {
        for (int j = i+1; j < 3; ++j) {
            Circle c = circleFrom(boardPoints[i], boardPoints[j]);
            if(isCircleValid(boardPoints, c)){
                return c;
            }
        }
    }
    return circleFrom(boardPoints[0], boardPoints[1], boardPoints[2]);

}

Circle minCircWelzelRec(vector<Point>& pointsVec, vector<Point> boardPoints, size_t size){
    if (size == 0 || boardPoints.size() == 3){
        return minCircleCreator(boardPoints);
    }
    int randIndex = rand() % size;
    Point randPoint = pointsVec[randIndex];
    swap(pointsVec[randIndex], pointsVec[size-1]);

    Circle minCirc = minCircWelzelRec(pointsVec, boardPoints, size-1);

    if(twoPointsDis(randPoint, minCirc.center) <= minCirc.radius){
        return minCirc;
    }

    boardPoints.push_back(randPoint);
    return minCircWelzelRec(pointsVec, boardPoints, size-1);
}

Circle minCircWelzel(vector<Point> pointsVec){
    vector<Point> pointsVec_copy = pointsVec;
    return minCircWelzelRec(pointsVec_copy, {}, pointsVec_copy.size());
}

Circle findMinCircle(Point** points,size_t size){
    vector<Point> pointsVec = arrayToVector(points, size);
    Circle minRadCirc = minCircWelzel(pointsVec);
    return minRadCirc;
}