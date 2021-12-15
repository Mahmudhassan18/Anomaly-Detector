//
// Created by Mahmud Hassan 208247825.
//
#include <math.h>
#include "anomaly_detection_util.h"

float avg(float* x, int size){
    float sum = 0;
    for (int i = 0; i < size; ++i) {
        sum += x[i];
    }
    return sum / float(size);
}

// returns the variance of X and Y
float var(float* x, int size){
    float sum = 0;
    for (int i = 0; i < size; ++i) {
        sum += x[i]*x[i];
    }
    return (sum/ float(size)) - (avg(x, size) * avg(x, size));
}

// returns the covariance of X and Y
float cov(float* x, float* y, int size){
    float multip = 0;
    for (int i = 0; i < size; ++i) {
        multip += x[i]*y[i];
    }
    return (multip/float(size)) - (avg(x ,size) * avg(y, size));
}


// returns the Pearson correlation coefficient of X and Y
float pearson(float* x, float* y, int size){
    return (cov(x, y ,size) / (sqrt(var(x, size)) * sqrt(var(y, size))));
}

// performs a linear regression and returns the line equation
Line linear_reg(Point** points, int size){
    float x[size], y[size];
    for (int i = 0; i < size; ++i) {
        x[i] = points[i]->x;
        y[i] = points[i]->y;
    }
    float a = cov(x, y, size) / var(x, size);
    float b = avg(y, size) - (avg(x, size)*a);
    return Line(a,b);
}

// returns the deviation between point p and the line equation of the points
float dev(Point p,Point** points, int size){
    Line line = linear_reg(points, size);
    return dev(p, line);
}

// returns the deviation between point p and the line
float dev(Point p,Line l){
    float a = l.a;
    float b = l.b;
    return fabs(((a*p.x) + b) - p.y);
}
