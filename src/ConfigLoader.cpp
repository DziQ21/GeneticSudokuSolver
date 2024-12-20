#include "ConfigLoader.h"
#include "Logger.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

ConfigLoader::ConfigLoader(std::string configDir)
{
    std::ifstream configFile(configDir);
    if (!configFile.is_open()) {
        std::cerr << "Could not open config file: " << configDir << std::endl;
        return;
    }

    std::string line;
    while (std::getline(configFile, line)) {
        std::istringstream iss(line);
        std::string key;
        
        // Extract the key
        if (!(iss >> key)) {
            std::cerr << "Error parsing line: " << line << std::endl;
            continue;
        }
        
        // Extract the rest of the line as the value
        std::string value;
        std::getline(iss, value);
        
        // Remove leading spaces from the value
        size_t start = value.find_first_not_of(" \t");
        if (start != std::string::npos) {
            value = value.substr(start);
        } else {
            value.clear(); // No valid value found, clear value
        }
        
        config.ParseEntry(key, value);
    }
}

void ConfigLoader::loadConfigPath(std::string configDir)
{
    std::ifstream configFile(configDir);
    if (!configFile.is_open()) {
        std::cerr << "Could not open config file: " << configDir << std::endl;
        return;
    }

    std::string line;
    while (std::getline(configFile, line)) {
        std::istringstream iss(line);
        std::string key;
        
        // Extract the key
        if (!(iss >> key)) {
            std::cerr << "Error parsing line: " << line << std::endl;
            continue;
        }
        
        // Extract the rest of the line as the value
        std::string value;
        std::getline(iss, value);
        
        // Remove leading spaces from the value
        size_t start = value.find_first_not_of(" \t");
        if (start != std::string::npos) {
            value = value.substr(start);
        } else {
            value.clear(); // No valid value found, clear value
        }
        
        config.ParseEntry(key, value);
    }
}
void ConfigLoader::loadConfigStr(std::string configStr)
{
    std::istringstream iss(configStr);
    std::string line;
    while (std::getline(iss, line)) {
        std::istringstream iss(line);
        std::string key;
        
        // Extract the key
        if (!(iss >> key)) {
            std::cerr << "Error parsing line: " << line << std::endl;
            continue;
        }
        
        // Extract the rest of the line as the value
        std::string value;
        std::getline(iss, value);
        
        // Remove leading spaces from the value
        size_t start = value.find_first_not_of(" \t");
        if (start != std::string::npos) {
            value = value.substr(start);
        } else {
            value.clear(); // No valid value found, clear value
        }
        
        config.ParseEntry(key, value);
    }
}


unsigned long parseLong(std::string param)
{
    return std::stoul(param);
}
int parseInt(std::string param)
{
    return std::stoi(param);
}

float parseFloat(std::string param)
{
    return std::stof(param);
}

ConfigLoader::~ConfigLoader()
{

}

bool parseBool(std::string param)
{
    if (param == "true") {
        return true;
    } else if (param == "false") {
        return false;
    }
    std::cout<<"Wrong Parameter using standard false"<<std::endl;
    return true;
}

MultiMutationConfig parseMultiMutation(std::string param)
{
    MultiMutationConfig result;
    std::istringstream iss(param);
    std::string subparam;
    if (!(iss >> subparam)) {
        std::cerr << "Error parsing line: " << param << std::endl;
        return result;
    }
    result.multiMutation = std::stoi(subparam);
    if (!(iss >> subparam)) {
        std::cerr << "Error parsing line: " << param << std::endl;
        return result;
    }
    result.multiMutationCoeff = std::stof(subparam);
    return result;
}

HarashMutationConfig parseHarash(std::string param)
{
    HarashMutationConfig result;
    std::istringstream iss(param);
    std::string subparam;
    if (!(iss >> subparam)) {
        std::cerr << "Error parsing line: " << param << std::endl;
        return result;
    }
    
    // printf("string\"%s\" \"%s\"  HarashMutationConfig %d %f\n", param.c_str(),subparam.c_str(),result.withoutImprovment,result.muationRate);
    result.withoutImprovment = std::stoi(subparam);
    if (!(iss >> subparam)) {
        std::cerr << "Error parsing line: " << param << std::endl;
        return result;
    }
    // printf("string\"%s\" \"%s\" HarashMutationConfig %d %f\n", param.c_str(),subparam.c_str(),result.withoutImprovment,result.muationRate);
    result.muationRate = std::stof(subparam);
    return result;
}

Config::Config()
{
    ResetCounter = 2000;
    logLevel = ERR;
    entries.push_back(std::unique_ptr<ConfigEntry>(new ConfigEntryImpl<LogLevel>("LogLevel", parseLog, logLevel)));
    auto bypass = [](std::string input) -> std::string {return input;};
    entries.push_back(std::unique_ptr<ConfigEntry>(new ConfigEntryImpl<std::string>("SudokuPath", bypass, sudokuPath)));
    entries.push_back(std::unique_ptr<ConfigEntry>(new ConfigEntryImpl<unsigned long>("PopulationSize",parseLong, populationSize)));
    entries.push_back(std::unique_ptr<ConfigEntry>(new ConfigEntryImpl<float>("MutationRate",parseFloat, mutationRate)));
    entries.push_back(std::unique_ptr<ConfigEntry>(new ConfigEntryImpl<float>("FittestRate",parseFloat, fittestRate)));
    entries.push_back(std::unique_ptr<ConfigEntry>(new ConfigEntryImpl<int>("ResetCounter",parseInt, ResetCounter)));
    entries.push_back(std::unique_ptr<ConfigEntry>(new ConfigEntryImpl<MultiMutationConfig>("MultiMutationCoeff",parseMultiMutation, multiMutationConfig)));
    entries.push_back(std::unique_ptr<ConfigEntry>(new ConfigEntryImpl<bool>("PreserveSelection",parseBool, preserveSelection)));
    entries.push_back(std::unique_ptr<ConfigEntry>(new ConfigEntryImpl<HarashMutationConfig>("MutationReset",parseHarash, mutationResetCfg)));
    entries.push_back(std::unique_ptr<ConfigEntry>(new ConfigEntryImpl<int>("Fittest",parseInt, fitestSelection)));
    entries.push_back(std::unique_ptr<ConfigEntry>(new ConfigEntryImpl<bool>("CrossMutation",parseBool, crossMutation)));
    //config ostrej mutacji
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
    // if(logLevel == DBG)
        std::cout<<"calling parser:"<<key<<" "<<value<<std::endl;
    for (auto& e : entries) {
        if (e->name == key) {
            e->parseValue(value);
            return;
        }
    }
    std::cout << "No entry found for key: " << key << std::endl;
}

