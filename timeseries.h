#ifndef TIMESERIES_H_
#define TIMESERIES_H_

#include <iostream>
#include <string.h>
#include <fstream>
#include<map>
#include <vector>
#include <string.h>
#include <algorithm>
#include <sstream>

using namespace std;

class TimeSeries{

    map<string, vector<float>> dataMap;
    vector<string> columnsNames;
    int numberOfRows;

public:
    TimeSeries(const char* CSVfileName){
        ifstream csvFile(CSVfileName);
        string header, lineSplit;
        csvFile>>header;//Puts first line from csvFile into Header
        stringstream headerSS(header);//creates a string stream to use in getLine
        //while line is splittable by ,loop just to get columns names as map keys
        while(getline(headerSS, lineSplit, ',')){
            vector<float> v;//Creates a new vector to insert in map vectors
            dataMap[lineSplit] = v;
            columnsNames.push_back(lineSplit);//saves columns names for easy use again
        }
        //while loop to get other lines into map data(vector)
        while(!csvFile.eof()){
            string line, lineSplit2;
            int i=0;
            csvFile>>line;
            stringstream linesSS(line);
            while(getline(linesSS, lineSplit2, ',')){
                //pushes line split in vector that is in the map
                dataMap[columnsNames[i]].push_back(stof(lineSplit2));
                i++;
            }
        }
        numberOfRows = dataMap[columnsNames[0]].size();
        csvFile.close();
    }

    //Returns vector from map with column name
    const vector<float>& getVectorFromMap(string name) const{
        return dataMap.at(name);
    }

    //Returns the vector with all columns names
    const vector<string>& getColumnsNames() const{
        return columnsNames;
    }

    //Returns number of rows in the map
    int getNumberOfRows() const{
        return numberOfRows;
    }



};
#endif /* TIMESERIES_H_ */
