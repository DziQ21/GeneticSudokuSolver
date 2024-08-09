#pragma once
#include "SudokuLoader.h"

class BaseGenotype
{
public:
    //make virtual functions
    BaseGenotype(const Sudoku& sudoku);
    virtual ~BaseGenotype();
    void mutate();
    void crossover();
    void evaluate();
    void print();
protected:
    virtual void fillEvalSudoku() = 0;
    const Sudoku& sudoku;
    Sudoku EvalSudoku;
};

class SoloNumGenotype : public BaseGenotype
{
public:
    SoloNumGenotype();
    ~SoloNumGenotype();
    void crossover();
    void print();
protected:
    void fillEvalSudoku();
    std::vector<short> sudokunumbers;
};