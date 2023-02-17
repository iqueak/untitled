//
// Created by bohdan.yurov on 13.02.2023.
//

#include "string"
#ifndef UNTITLED_LOGGER_H
#define UNTITLED_LOGGER_H

#endif //UNTITLED_LOGGER_H

namespace Logger {

    void Init();

    void Log(const std::string& header,const std::string& message);

    void ConsoleLog(const std::string& header,const std::string& message);
    void LogToFile(const std::string& header,const std::string& message);

}

