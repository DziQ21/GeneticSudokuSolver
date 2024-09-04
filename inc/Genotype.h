#pragma once
#include "SudokuLoader.h"

enum GenotypeType
{
    SoloNum
};

class BaseGenotype
{
public:
    //make virtual functions
    BaseGenotype(const Sudoku& sudoku): sudoku(sudoku),evalSudokuValid(false){};
    virtual ~BaseGenotype(){};
    virtual void mutate()=0;
    BaseGenotype* crossover(BaseGenotype);
    void evaluate();
    void print();
    std::string getPrintStr();
    int getEvalValue() const { return evalValue; }
    GenotypeType getType() const { return type; }
protected:
    GenotypeType type;
    virtual void fillEvalSudoku() = 0;
    const Sudoku& sudoku;
    Sudoku evalSudoku;
    int evalValue;
    bool evalSudokuValid;
    int rowcount[9];
    int colcount[9];
    int boxcount[9];
};

class SoloNumGenotype : public BaseGenotype
{
public:
    SoloNumGenotype(const Sudoku& sudoku);
    ~SoloNumGenotype();
    BaseGenotype* crossover(BaseGenotype &other);
    virtual void mutate(){evalSudokuValid = false;};
protected:
    void fillEvalSudoku();
    std::vector<short> sudokunumbers;
};