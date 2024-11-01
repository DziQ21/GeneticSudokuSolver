#pragma once
#include <string>
#include <map>
#include <memory> 
#include <functional>
#include "Logger.h"

class ConfigEntry
{
    
    public:
    ConfigEntry(const std::string& name) : name(name) {}
    virtual void parseValue(const std::string& value) = 0;
    std::string name;
    ConfigEntry()=delete;
    virtual ~ConfigEntry(){};
};

template<class T>
class ConfigEntryImpl:public ConfigEntry
{
    public:
    ConfigEntryImpl()=delete;
    ConfigEntryImpl(std::string name,std::function<T(std::string)> parserFunction,T& paramRef): ConfigEntry(name), parser(parserFunction),value(paramRef){};
    void parseValue(const std::string& value) override {this->value = parser(value);};
    private:
    std::function<T(std::string)> parser;
    T& value;
    virtual ~ConfigEntryImpl(){};

};

class Config
{
private:
    std::vector<std::unique_ptr<ConfigEntry>> entries;
    LogLevel logLevel;
    std::string sudokuPath;
    unsigned long populationSize;
    float mutationRate;
    /* data */
public:
    //getters
    LogLevel getLogLevel() const {return logLevel;};
    unsigned long getPopulationSize() const {return populationSize;};
    void ParseEntry(std::string key,std::string value);
    const std::string& getSudokuPath() const {return sudokuPath;};
    Config();
    ~Config(){};
    //prevent copy 
    Config(const Config&) = delete;
};

class ConfigLoader
{
private:
    Config config;
public:
    ConfigLoader(std::string configDir);
    ConfigLoader()=delete;
    ~ConfigLoader();
    void loadConfig();
    const Config& getConfig() const
    {
        return config;
    }
};

LogLevel parseLog(std::string param);


