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

};

class Config
{
private:
    std::vector<std::unique_ptr<ConfigEntry>> entries;
    LogLevel logLevel;
    std::string sudokuPath;
    unsigned long populationSize;
    /* data */
public:
    const std::string& getSudokuPath() const {return sudokuPath;};
    unsigned long getPopulationSize() const {return populationSize;};
    void ParseEntry(std::string key,std::string value);
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


