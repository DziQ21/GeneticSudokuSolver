#pragma once
#include "SudokuLoader.h"

class BaseGenotype
{
public:
    //make virtual functions
    BaseGenotype(const Sudoku& sudoku);
    virtual ~BaseGenotype();
    void evaluate();
    void print();
private:
    const Sudoku& sudoku;
};

class Phenotype : public BaseGenotype
{
public:
    Phenotype();
    ~Phenotype();
    void mutate();
    void crossover();
    void evaluate();
    void print();
    void setFitness();
    void getFitness();
    void setGenotype();
    void getGenotype();
    void setPhenotype();
    void getPhenotype();
    void setGenome();
    void getGenome();
    void setGenomeSize();
    void getGenomeSize();
    void setGenomeType();
    void getGenomeType();
};