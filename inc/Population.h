#pragma once

#include <vector>
#include "Genotype.h"
#include <memory>
#include <functional>
#include "ConfigLoader.h"

typedef std::vector<std::unique_ptr<BaseGenotype>> Population_t;

template <typename T>
class Population
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
private:
    std::function<Population_t(Population_t&, std::size_t, bool, const Config &)> fitestFunction;
    const Sudoku& sudoku;
    Population_t population;
    const Config& config;
};
