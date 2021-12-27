#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "minCircle.h"
#include <chrono>
#include "CLI.h"
#include "commands.h"

using namespace std;
using namespace std::chrono;

class STDtest:public DefaultIO{
    ifstream in;
    ofstream out;
public:
    STDtest(string inputFile,string outputFile){
        in.open(inputFile);
        out.open(outputFile);
    }
    virtual string read(){
        string s;
        cin>>s;
        return s;
    }
    virtual void write(string text){
        cout<<text;
    }

    virtual void write(float f){
        cout<<f;
    }

    virtual void read(float* f){
        cin>>*f;
    }

    void close(){
        if(in.is_open())
            in.close();
        if(out.is_open())
            out.close();
    }
    ~STDtest(){
        close();
    }
};


int main(){
    STDtest std("input.txt","output.txt");
    CLI cli(&std);
    cli.start();
    std.close();

};

