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
    virtual BaseGenotype* crossover(BaseGenotype& other) = 0;
    void evaluate();
    void print();
    std::string getPrintStr();
    int getEvalValue() const { return evalValue; }
    GenotypeType getGenoType() const { return genoType; }
protected:
    GenotypeType genoType;
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
    SoloNumGenotype(const Sudoku&, std::vector<short>);
    ~SoloNumGenotype();
    BaseGenotype* crossover(BaseGenotype &other);
    virtual void mutate(){evalSudokuValid = false;};
    std::vector<short> getNumbers() const { return sudokunumbers; }
protected:
    void fillEvalSudoku();
    std::vector<short> sudokunumbers;
};