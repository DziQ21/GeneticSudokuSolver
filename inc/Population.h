#pragma once

#include <vector>
#include "Genotype.h"
#include <memory>
#include <functional>

typedef std::vector<std::unique_ptr<BaseGenotype>> Population_t;

template <typename T>
class Population
{
public: 
    Population(int size, const Sudoku& sudoku,std::function<Population_t(Population_t&,float)>);
    ~Population(){};
    void evolve();
    void print(size_t a);
    void nextGeneration();
    void fillRestOfPopulation();
private:
    std::function<Population_t(Population_t&,float)> fitestFunction;
    const Sudoku& sudoku;
    Population_t population;
    int size;
};
