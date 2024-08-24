#pragma once
#include "SudokuLoader.h"

class BaseGenotype
{
public:
    //make virtual functions
    BaseGenotype(const Sudoku& sudoku): sudoku(sudoku),evalSudokuValid(false){};
    virtual ~BaseGenotype(){};
    virtual void mutate()=0;
    void crossover();
    void evaluate();
    void print();
    std::string getPrintStr();
    int getEvalValue() const { return evalValue; }
protected:
    virtual void fillEvalSudoku() = 0;
    const Sudoku& sudoku;
    Sudoku evalSudoku;
    int evalValue;
    bool evalSudokuValid;
};

class SoloNumGenotype : public BaseGenotype
{
public:
    SoloNumGenotype(const Sudoku& sudoku);
    ~SoloNumGenotype();
    void crossover();
    virtual void mutate(){evalSudokuValid = false;};
protected:
    void fillEvalSudoku();
    std::vector<short> sudokunumbers;
};