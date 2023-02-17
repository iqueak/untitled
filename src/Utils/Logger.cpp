//
// Created by bohdan.yurov on 16.02.2023.
//
#include "Logger.h"
#include "iostream"
#include "fstream"
#include "raylib.h"

using namespace std;

namespace Logger {


    static const char *path = "Log.txt";

    void Init() {
        /*** to clear the exist text in log file ***/
        fstream f(path,std::ofstream::out | std::ofstream::trunc);
        f.close();
    }

    /*** double log in console and in file ***/
    void Log(const std::string &header, const std::string &message) {
        ConsoleLog(header,message);
        LogToFile(header,message);
    }

    void Logger::ConsoleLog(const std::string& header,const std::string& message) {
        std::string log = "[" + header + "]: " + message + "!\n";
        printf("%s", log.c_str());
    }


    void LogToFile(const std::string &header, const std::string &message) {
        /***
         *
         * open file each call of log to file can slowed perfomance
         *
         * TODO is too bad for prefomance: TEST1:128853 (with open in each call) | TEST2:2933 (without open in each call)
         *
         ***/
        fstream f(path,std::ios_base::out | std::ios_base::app);

        if (!f.is_open()) {
            ConsoleLog("ERROR","Problem with opening log file.");
            return;
        }
        std::string log = "[" + header + "]: " + message + "!\n";
        f << log << "\n";
        f.close();
    }
}
