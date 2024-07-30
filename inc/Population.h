#pragma once

#include <vector>
#include "Genotype.h"

class Population
{
    public: 
    Population(int size, const Sudoku& sudoku);
    ~Population();
    void evolve();
    void print();
    private:
    std::vector<BaseGenotype> population;
    const Sudoku& sudoku;
};
