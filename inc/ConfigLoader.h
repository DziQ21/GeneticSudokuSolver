#pragma once
#include <string>
#include <map>
#include <memory> 
#include <functional>
#include "Logger.h"

struct MultiMutationConfig {
    int multiMutation;
    float multiMutationCoeff;
};

struct HarashMutationConfig {
    int withoutImprovment;
    float muationRate;
};

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
    float fittestRate;
    int ResetCounter;
    MultiMutationConfig multiMutationConfig;
    bool preserveSelection;
    HarashMutationConfig mutationResetCfg;
    int fitestSelection;
    bool crossMutation;
    int solverType;
public:
    //getters
    bool getPreserveSelection() const {return preserveSelection;};
    LogLevel getLogLevel() const {return logLevel;};
    unsigned long getPopulationSize() const {return populationSize;};
    void ParseEntry(std::string key,std::string value);
    const std::string& getSudokuPath() const {return sudokuPath;};
    float getMutationRate() const {return mutationRate;};
    float getFittestRate() const {return fittestRate;};
    int getResetCounter() const {return ResetCounter;};
    int getMultiMutation() const {return multiMutationConfig.multiMutation;};
    float getMultiMutationCoeff() const {return multiMutationConfig.multiMutationCoeff;};
    const HarashMutationConfig& getHarashMutationConfig() const {return mutationResetCfg;};
    int getFittest() const {return fitestSelection;};
    bool getCrossMutation() const {return crossMutation;};
    int getSolverType() const {return solverType;};
    Config();
    ~Config(){};
};

class ConfigLoader
{
private:
    Config config;
public:
    ConfigLoader(std::string configDir);
    ConfigLoader(){};
    ~ConfigLoader();
    void loadConfigPath(std::string configDir);
    void loadConfigStr(std::string configStr);
    const Config& getConfig() const
    {
        return config;
    }
};

LogLevel parseLog(std::string param);


