#pragma once

#include <vector>
#include "Genotype.h"
#include <memory>
#include <functional>
#include "ConfigLoader.h"

typedef std::vector<std::unique_ptr<BaseGenotype>> Population_t;

class BasePopulation {
public:
    virtual ~BasePopulation() = default;
    virtual void evolve() = 0;
    virtual void print(size_t a) = 0;
    virtual void nextGeneration() = 0;
    virtual void fillRestOfPopulation() = 0;
    virtual void harashMutation(float) = 0;
    virtual std::tuple<float,int> getStats()=0;
    virtual void performMultiMutation(size_t resultPop)=0;
};

template <typename T>
class Population:public BasePopulation
{
public: 
    Population(const Config& config, const Sudoku& sudoku,std::function<Population_t(Population_t&, std::size_t, bool, const Config &)>);
    ~Population(){};
    void evolve();
    void print(size_t a);
    void nextGeneration();
    void fillRestOfPopulation();
    void harashMutation(float);
    std::tuple<float,int> getStats();
    void performMultiMutation(size_t resultPop);
private:
    std::function<Population_t(Population_t&, std::size_t, bool, const Config &)> fitestFunction;
    const Sudoku& sudoku;
    Population_t population;
    const Config& config;
};
