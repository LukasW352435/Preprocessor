#pragma once
// command line args
// -fi <string filename input>
// -fo <string filename output>
// -d <int debug level> // 0 nothing (default), 1 error, 2 warning, 3 information, 4 verbose 
// -fd <string filename> // TODO outputs the debug messages in a file
// -s // spaces in front/back of define

// #include
// #define
// #undef

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <list>

#define DEBUG(message) stringstream ss; ss << message
#define WRITEDEBUG(level) WriteDebug(args,debugCounts,DebugLevel::level, ss.str())
#define ERROR WRITEDEBUG(Error)
#define WARNING WRITEDEBUG(Warning)
#define INFORMATION WRITEDEBUG(Information)
#define VERBOSE WRITEDEBUG(Verbose)

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
} args;

enum class DebugLevel : int8_t
{
    Error = 1,
    Warning = 2,
    Information = 3,
    Verbose = 4,
};

struct debugCount
{
    int error = 0;
    int warning = 0;
    int information = 0;
    int verbose = 0;
} debugCounts;

struct inputFile {
    inputFile(string path, int lineNumber);
    string path;
    int lineNumber;
    ifstream::pos_type pos;
};

void processArg(int argc, char** argv, arg& args);

void incI(int& i, int argc);

void replaceIf(string& line, int lineNumber, list<def>& defs, bool spaces);

void WriteDebug(arg& args, debugCount& count , DebugLevel level, string message);

void WriteDebugSum(arg& args, debugCount& count);
