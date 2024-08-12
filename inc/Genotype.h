#pragma once
#include "SudokuLoader.h"

class BaseGenotype
{
public:
    //make virtual functions
    BaseGenotype(const Sudoku& sudoku): sudoku(sudoku){};
    virtual ~BaseGenotype(){};
    void mutate();
    void crossover();
    void evaluate();
    void print();
    int getEvalValue() const { return evalValue; }
protected:
    virtual void fillEvalSudoku() = 0;
    const Sudoku& sudoku;
    Sudoku evalSudoku;
    int evalValue;
};

class SoloNumGenotype : public BaseGenotype
{
public:
    SoloNumGenotype(const Sudoku& sudoku);
    ~SoloNumGenotype();
    void crossover();
    void print();
protected:
    void fillEvalSudoku();
    std::vector<short> sudokunumbers;
};