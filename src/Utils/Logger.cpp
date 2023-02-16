//
// Created by bohdan.yurov on 16.02.2023.
//
#include "Logger.h"
#include "raylib.h"


namespace Logger {
    static const char *path = "Log.txt";

    void Init() {

    }

    void Log(const std::string &header, const std::string &message) {
        ConsoleLog(header,message);
        LogToFile(header,message);
    }

    void Logger::ConsoleLog(const std::string& header,const std::string& message) {
        std::string log = "[" + header + "]: " + message + "!\n";
        printf("%s", log.c_str());
    }

    //TODO make log to file with iostream, open file in init and close in destroy
    void LogToFile(const std::string &header, const std::string &message) {
        std::string log = "[" + header + "]: " + message + "!\n";
        char * str = const_cast<char*>(log.c_str());;
        SaveFileText(path,str);
    }
}
