#include "main.h"

int processArg(int argc, char** argv, arg& args)
{
    // show args
    cout << "Argc: " << argc << endl;
    vector<string> arg(argv, argv + argc);
    cout << "Argv:" << endl;
    for (int i = 0; i < argc; i++)
        cout << i << ". " << arg[i] << endl;

    // set args
    for (int i = 1; i < argc; i++)
    {
        if (arg[i].compare("-fi") == 0)
        {
            // file input
            incI(i, argc);
            args.fileInput = arg[i];
        }
        else if (arg[i].compare("-fo") == 0)
        {
            // file output
            incI(i, argc);
            args.fileOutput = arg[i];
        }
        else if (arg[i].compare("-d") == 0) {
            // debug level
            incI(i, argc);
            try {
                args.debugLevel = stoi(arg[i]);
                if (args.debugLevel < 0 || args.debugLevel > 4) {
                    throw invalid_argument("Debug level can only be 0-4!");
                }
            }
            catch (exception e) {
                cout << e.what() << endl;
            }
        }
        else
        {
            cout << "Unknown argv: " << arg[i] << endl;
        }
    }

    // print args
    if (!args.fileInput.empty())
        cout << "File input: " << args.fileInput << endl;
    else
        throw invalid_argument("No file input!");
    if (args.fileOutput.empty())
        args.fileOutput = "out_" + args.fileInput;
    cout << "File output: " << args.fileOutput << endl;
}

void incI(int& i, int argc)
{
    i++;
    if (i == argc)
    {
        throw invalid_argument("Not enough arguments!");
    }
}

void replaceIf(string& line, int lineNumber, list<def>& defs)
{
    // process line and do replaces with defs
    for (list<def>::iterator it = defs.begin(); it != defs.end(); it++)
    {
        size_t lineIndex = 0;
        while (lineIndex < line.size())
        {
            lineIndex = line.find(it->key, lineIndex);
            if (lineIndex != string::npos)
            {
                // check for spaces
                char s = line[lineIndex + it->key.size()];
                if (!(lineIndex == 0 || line[lineIndex - 1] == ' '))
                {
                    lineIndex++;
                    continue;
                }
                if (!(s == ' ' || s == '\n' || s == '\0'))
                {
                    lineIndex++;
                    continue;
                }

                // do the replace
                line.replace(lineIndex, it->key.size(), it->value);
                cout << lineNumber << ". Replace lineIndex: " << lineIndex << " key: " << it->key << " value: " << it->value << endl;
            }
            else {
                break;
            }
        }
    }
}

int main(int argc, char** argv)
{
    arg args;
    processArg(argc, argv, args);

    // open files
    ifstream fileIn(args.fileInput);
    ofstream fileOut(args.fileOutput);

    list<def> defs;
    string line;
    int lineNumber = 1;

    while (getline(fileIn, line))
    {
        replaceIf(line, lineNumber, defs);

        // check for definitions in the line
        if (line.size() > 8 && line.compare(0, 8, "#define ") == 0)
        {
            int afterKey = line.find(' ', 8);
            if (afterKey == -1)
            {
                cout << lineNumber << ". ERROR no valid key found after #define!" << endl;
                fileOut << line;
                if (fileIn.eof())
                    break;
                fileOut << endl;
            }
            else
            {
                string key = line.substr(8, afterKey - 8);
                string value = line.substr(afterKey + 1);
                cout << lineNumber << ". #define key: " << key << " value: " << value << endl;
                def d;
                d.key = key;
                d.value = value;
                defs.push_back(d);
            }
        }
        else if (line.size() > 7 && line.compare(0, 7, "#undef ") == 0)
        {
            cout << lineNumber << ". #undef" << endl;
        }
        else
        {
            fileOut << line;
            if (fileIn.eof())
                break;
            fileOut << endl;
        }
        lineNumber++;
    }

    // close files
    fileIn.close();
    fileOut.close();

    return 0;
}