#pragma once
// command line args
// -fi <string filename input>
// -fo <string filename output>
// -d <int debug level> // 0 nothing (default), 1 error, 2 warning, 3 information, 4 verbose 
// -s // spaces in front/back of define

// #include
// #define
// #undef

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <list>

using namespace std;

struct def
{
    string key;
    string value;
};

struct arg {
    string fileInput;
    string fileOutput;
    int debugLevel = 0;
    bool spacesBeforeAfterDefine = false;
};

int processArg(int argc, char** argv, arg& args);

void incI(int& i, int argc);

void replaceIf(string& line, int lineNumber, list<def>& defs, bool spaces);
