#ifndef MY_LOGGER_HPP
#define MY_LOGGER_HPP

// ===============

#define DEBUG 1     // 1 - switch on logger; 0 - switch off

// ===============


#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

enum LogError{INFO, WARNING, ERROR};

class MyLogger {
    private:
        string str;
        int error_lvl;
        static string tab;     // Indent line
        static vector<string> curr_func;

        string LogError() {
            switch (error_lvl) {
                case INFO:
                    return "INFO";

                case WARNING:
                    return "WARNING";
                
                case ERROR:
                    return "ERROR";
            }

            return "INFO";
        }

        void outputFunc() {
            if(output == "stdout") 
                cout << str;

            if(output == "file") 
                fileLog();

            if (output == "mix") {
                cout << str;
                fileLog();
            }
        }
        
        void fileLog() {
            ofstream file(logname, ios_base::app);
            file << str << endl;
            file.close();
        }

        void makeAString(string filename, string funcName, int lineNumber, int error_lvl, string message) {
            if(curr_func.back().compare(curr_func.at(curr_func.size() - 2))) 
                tab.append("    ");

            str = tab + LogError() + ' ' + filename + ":" + funcName + "():" + to_string(lineNumber) + "  ";
            
            if(timestamp) {
                str += __DATE__;
                str += ' ';
                str += __TIME__;
            }
            
            str += ' ' + message;
        }

    public:
        static string logname; // File name where logger write 
        static string output;  // stdout - to console; file - to logfile; mix - both ways.
        static bool timestamp;  

        MyLogger() {
            curr_func.push_back("main");
        }

        ~MyLogger() {}

        void log(string filename, string funcName, int lineNumber, int error_lvl, string message) {
            this->error_lvl = error_lvl;
            curr_func.push_back(funcName);

            makeAString(filename, funcName, lineNumber, error_lvl, message);
            outputFunc();
        }
};

string MyLogger::logname = "log.log";
string MyLogger::output = "file";
bool MyLogger::timestamp = true;
string MyLogger::tab;
vector<string> MyLogger::curr_func;

// ========================================================

#if DEBUG == 1
    MyLogger logger; 

    #define LOG(error_lvl, msg) \
        logger.log(__FILE__, __FUNCTION__, __LINE__, error_lvl, msg); 

    #define PRINT(var) \
        /* Print variable with value */ \
        cout << #var << ": " << var << endl;

    #define LOG_CONFIG_FILENAME(_filename) \
        /* stdout - to console; file - to logfile; mix - both ways. */ \
        MyLogger::logname = _filename; 

    #define LOG_CONFIG_TIMESTAMP(check) \
        /* If true - log will be with date and time */ \
        if(check) \
            MyLogger::timestamp = true; \
        else \
            MyLogger::timestamp = false;
#else
    #define LOG(x, y)
    #define PRINT(var)
    #define LOG_CONFIG_FILENAME(x)
    #define LOG_CONFIG_TIMESTAMP(check)

#endif

#endif
