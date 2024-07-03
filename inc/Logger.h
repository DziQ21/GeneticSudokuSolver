#pragma once

// #include <iostream>
// #include <fstream>
// #include <string>
// #include <mutex>

enum LogLevel {
    DBG,
    INF,
    ERR,
    VIP
};

// class Logger {
// public:
//     Logger();
//     ~Logger();
//     static Logger& getInstance() {
//         static Logger instance;
//         return instance;
//     }
    
//     void setLogDirectory(const std::string& directory);

//     // Delete copy constructor and assignment operator
//     Logger(const Logger&) = delete;
//     Logger& operator=(const Logger&) = delete;

//     // Overloaded operator<< for logging
//     template<typename T>
//     Logger& operator<<(const T& value);

//     // Function to set log level
//     void setLogLevel(LogLevel level);
//     std::string getLogLevelString(LogLevel level);
// private:
//     std::ofstream logFile;
//     std::string logDirectory;
//     std::mutex mtx;
//     LogLevel currentLogLevel;
// };

// // Manipulator functions
// struct LogManip {
//     explicit LogManip(LogLevel level) : logLevel(level) {}
//     LogLevel logLevel;
// };

// Logger& operator<<(Logger& logger, const LogManip& manip);

// #define LOGDEBUG Logger::getInstance() << LogManip(DBG)
// #define LOGINFO Logger::getInstance() << LogManip(INF)
// #define LOGERROR Logger::getInstance() << LogManip(ERR)
// #define LOGVIP Logger::getInstance() << LogManip(VIP)
