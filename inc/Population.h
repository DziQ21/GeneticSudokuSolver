#pragma once

#include <vector>
#include "Genotype.h"
#include <memory>

template <typename T>
class Population
{
    public: 
    Population(int size, const Sudoku& sudoku);
    ~Population(){};
    void evolve();
    void print();
    private:
    std::vector<std::unique_ptr<BaseGenotype>> population;
    const Sudoku& sudoku;
};
