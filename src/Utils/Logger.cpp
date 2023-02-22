//
// Created by bohdan.yurov on 16.02.2023.
//
#include "Logger.h"
#include "iostream"
#include "fstream"
#include "raylib.h"


#include <chrono>
using namespace std::chrono;

using namespace std;

namespace Logger {

    static const char *path = "Log.txt";



    /*** double log in console and in file ***/
    void Log(const std::string &header, const std::string &message) {
        ConsoleLog(header,message);
        LogToFile(header,message);
    }

    void Logger::ConsoleLog(string header, string message) {
        string log = "[" + header + "]: " + message + "\n";
        printf("%s", log.c_str());
    }

    void LogToFile(string header, string message) {

        static ofstream file(path, ofstream::out | ofstream::trunc);

        std::time_t now = std::time(nullptr);
        char timestamp[20];
        std::strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));

        file << "[" << /*timestamp <<*/ "] " << "[" + header + "]: " + message << endl;
    }
}
