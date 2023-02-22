//
// Created by bohdan.yurov on 13.02.2023.
//

#include "string"
#ifndef UNTITLED_LOGGER_H
#define UNTITLED_LOGGER_H

#endif //UNTITLED_LOGGER_H

using namespace std;

namespace Logger {

    void Log(string header, string message);

    void ConsoleLog(string header,string message);
    void LogToFile(string header,string message);

}

