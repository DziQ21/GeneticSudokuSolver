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
    Population(int size, const Sudoku& sudoku,std::function<Population_t(Population_t)>);
    ~Population(){};
    void evolve();
    void print();
    private:
    std::function<Population_t(Population_t)> fitestFunction;
    const Sudoku& sudoku;
    Population_t population;
};
