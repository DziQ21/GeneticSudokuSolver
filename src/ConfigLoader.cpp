#include "ConfigLoader.h"
#include "Logger.h"
#include <iostream>
#include <fstream>
#include <sstream>

ConfigLoader::ConfigLoader(std::string configDir)
{
    std::ifstream configFile(configDir);
    if (!configFile.is_open()) {
        std::cout << "Could not open config file: " << configDir << std::endl;
        return;
    }

    std::string line;
    while (std::getline(configFile, line)) {
        std::istringstream iss(line);
        std::string key, value;
        if (!(iss >> key >> value)) {
            std::cerr << "Error parsing line: " << line << std::endl;
            continue;
        }
        config.ParseEntry(key, value);
    }
}


ConfigLoader::~ConfigLoader()
{

}
Config::Config()
{
    entries.push_back(std::unique_ptr<ConfigEntry>(new ConfigEntryImpl<LogLevel>("LogLevel", parseLog, logLevel)));
    auto bypass = [](std::string input) -> std::string {return input;};
    entries.push_back(std::unique_ptr<ConfigEntry>(new ConfigEntryImpl<std::string>("SudokuPath", bypass, sudokuPath)));
}

LogLevel parseLog(std::string param)
{
    if (param == "DBG") {
        return DBG;
    } else if (param == "INF") {
        return INF;
    } else if (param == "ERR") {
        return ERR;
    } else if (param == "VIP") {
        return VIP;
    }
    std::cout<<"Wrong Parameter using standard INF"<<std::endl;
    return INF;
}

void Config::ParseEntry(std::string key, std::string value) {
    std::cout<<"calling parser:"<<key<<" "<<value<<std::endl;
    for (auto& e : entries) {
        if (e->name == key) {
            e->parseValue(value);
            return;
        }
    }
    std::cout << "No entry found for key: " << key << std::endl;
}

