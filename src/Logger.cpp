// #include "Logger.h"



// void Logger::setLogDirectory(const std::string& directory) {
//     std::lock_guard<std::mutex> lock(mtx);
//     logDirectory = directory;
//     if (logFile.is_open()) {
//         logFile.close();
//     }
//     logFile.open(logDirectory + "/log.txt", std::ios::app);
//     if (!logFile.is_open()) {
//         std::cerr << "Failed to open log file in directory: " << logDirectory << std::endl;
//     }
// }

// void Logger::setLogLevel(LogLevel level) {
//     currentLogLevel = level;
// }

// template<typename T>
// Logger& Logger::operator<<(const T& value) {
//     std::lock_guard<std::mutex> lock(mtx);
//     if (logFile.is_open()) {
//         logFile << value;
//     } else {
//         std::cerr << "Log file is not open." << std::endl;
//     }
//     return *this;
// }

// std::string Logger::getLogLevelString(LogLevel level) {
//     switch (level) {
//         case DBG: return "DEBUG";
//         case INF: return "INFO";
//         case ERR: return "ERROR";
//         case VIP: return "VIP";
//         default: return "UNKNOWN";
//     }
// }

// Logger& operator<<(Logger& logger, const LogManip& manip) {
//     logger.setLogLevel(manip.logLevel);
//     logger << logger.getLogLevelString(manip.logLevel) << ": ";
//     return logger;
// }
// Logger::Logger() : currentLogLevel(DBG) {
//     logDirectory = "./logs.txt";
//     logFile.open(logDirectory + "/log.txt", std::ios::app);
//     if (!logFile.is_open()) {
//         std::cerr << "Failed to open log file in directory: " << logDirectory << std::endl;
//     }
// }

// Logger::~Logger() {
//     if (logFile.is_open()) {
//         logFile.close();
//     }
// }
