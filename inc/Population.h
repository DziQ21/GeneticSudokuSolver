#pragma once

#include <vector>
#include "Genotype.h"

template <typename T>
class Population
{
    public: 
    Population<T>(int size, const Sudoku& sudoku);
    ~Population();
    void evolve();
    void print();
    private:
    std::vector<std::unique_ptr<BaseGenotype>> population;
    const Sudoku& sudoku;
};
