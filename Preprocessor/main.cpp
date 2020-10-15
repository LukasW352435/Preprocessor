#include "main.h"

void processArg(int argc, char** argv, arg& args)
{
    vector<string> arg(argv, argv + argc);

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
        else if (arg[i].compare("-s") == 0) {
            // spaces for #define
            args.spacesBeforeAfterDefine = true;
        }
        else
        {
            cout << "Unknown argv: " << arg[i] << endl;
        }
    }

    // print args
    if (!args.fileInput.empty()) {
        DEBUG( "File input: " + args.fileInput); INFORMATION
    }
    else
    {
        DEBUG("No file input!"); ERROR
        throw invalid_argument("No file input!");
    }
    if (args.fileOutput.empty())
        args.fileOutput = "out_" + args.fileInput;
    DEBUG("File output: " + args.fileOutput); INFORMATION
}

void incI(int& i, int argc)
{
    i++;
    if (i == argc)
    {
        throw invalid_argument("Not enough arguments!");
    }
}

void replaceIf(string& line, int lineNumber, list<def>& defs, bool spaces = false)
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
                if (spaces) {
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
                }

                // do the replace
                line.replace(lineIndex, it->key.size(), it->value);

                DEBUG(lineNumber << ". Replace lineIndex: " << lineIndex << " key: " << it->key << " value: " << it->value); VERBOSE
            }
            else {
                break;
            }
        }
    }
}

void WriteDebug(arg& args, debugCount& count, DebugLevel level, string message){
    if (args.debugLevel >= (int8_t)level) {
        switch (level)
        {
        case DebugLevel::Error:
            cout << "ERROR\t\t";
            count.error++;
            break;
        case DebugLevel::Warning:
            cout << "WARNING\t\t";
            count.warning++;
            break;
        case DebugLevel::Information:
            cout << "INFORMATION\t";
            count.information++;
            break;
        case DebugLevel::Verbose:
            cout << "VERBOSE\t\t";
            count.verbose++;
            break;
        default:
            break;
        }
        cout <<  message << endl;
    }
}

void WriteDebugSum(arg& args, debugCount& count) {
    if (args.debugLevel >= (int8_t)DebugLevel::Error) {
        cout << endl << "Debug Summary" << endl;
        cout << "ERROR:\t\t" << count.error << endl;
    }
    if (args.debugLevel >= (int8_t)DebugLevel::Warning) {
        cout << "WARNING:\t" << count.warning << endl;
    }
    if (args.debugLevel >= (int8_t)DebugLevel::Information) {
        cout << "INFORMATION:\t" << count.information << endl;
    }
    if (args.debugLevel >= (int8_t)DebugLevel::Verbose) {
        cout << "VERBOSE:\t" << count.verbose << endl;
    }
}


int main(int argc, char** argv)
{

    processArg(argc, argv, args);

    // open files
    ifstream fileIn(args.fileInput);
    ofstream fileOut(args.fileOutput);

    list<def> defs;
    string line;
    int lineNumber = 1;

    while (getline(fileIn, line))
    {
        replaceIf(line, lineNumber, defs, args.spacesBeforeAfterDefine);

        // check for definitions in the line
        if (line.size() > 8 && line.compare(0, 8, "#define ") == 0)
        {
            int afterKey = line.find(' ', 8);
            if (afterKey == -1)
            {
                DEBUG(lineNumber << ". No valid key found after #define!"); ERROR
                fileOut << line;
                if (fileIn.eof())
                    break;
                fileOut << endl;
            }
            else
            {
                string key = line.substr(8, afterKey - 8);
                string value = line.substr(afterKey + 1);
                DEBUG(lineNumber << ". #define key: " << key << " value: " << value); VERBOSE
                def d;
                d.key = key;
                d.value = value;
                defs.push_back(d);
            }
        }
        else if (line.size() > 7 && line.compare(0, 7, "#undef ") == 0)
        {
            int afterKey = line.find(' ', 7);
            if (afterKey == -1) {
                DEBUG(lineNumber << ". No valid key found after #undef!"); ERROR
                fileOut << line;
                if (fileIn.eof())
                    break;
                fileOut << endl;
            }
            else
            {
                string key = line.substr(7, afterKey - 7);
                DEBUG(lineNumber << ". #undef key: " << key); VERBOSE
            }
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
    WriteDebugSum(args, debugCounts);
    system("pause");
    return 0;
}