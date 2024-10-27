#pragma once
#include "SudokuLoader.h"

enum GenotypeType
{
    SoloNum,
    FullPermutation,
    RowPermutation,
    BoxPermutation
};

class BaseGenotype
{
public:
    //make virtual functions
    BaseGenotype(const Sudoku& sudoku): sudoku(sudoku),evalSudokuValid(false){};
    virtual ~BaseGenotype(){};
    virtual void mutate(float mutationRate)=0;
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
    virtual ~SoloNumGenotype();
    BaseGenotype* crossover(BaseGenotype &other);
    virtual void mutate(float mutationRate);
    std::vector<short> getNumbers() const { return sudokunumbers; }
protected:
    void fillEvalSudoku();
    std::vector<short> sudokunumbers;
};

class FullPermutationGenotype : public BaseGenotype
{
public:
    void validateGenotype(int);
    FullPermutationGenotype(const Sudoku& sudoku);
    FullPermutationGenotype(const Sudoku&, std::vector<short>);
    virtual ~FullPermutationGenotype(){};
    BaseGenotype* crossover(BaseGenotype &other);
    virtual void mutate(float mutationRate);
    std::vector<short> getNumbers() const { return sudokunumbers; }
protected:
    void fillEvalSudoku();
    std::vector<short> sudokunumbers;
};

class RowPermutationGenotype : public BaseGenotype
{
public:
    RowPermutationGenotype(const Sudoku& sudoku);
    RowPermutationGenotype(const Sudoku&, std::array<std::vector<short>,9>);
    virtual ~RowPermutationGenotype(){};
    BaseGenotype* crossover(BaseGenotype &other);
    virtual void mutate(float mutationRate);
    const std::array<std::vector<short>,9>& getRows() const { return rows; }
protected:
    void fillEvalSudoku();
    std::array<std::vector<short>,9> rows;
};


class BoxPermutationGenotype : public BaseGenotype
{
public:
    BoxPermutationGenotype(const Sudoku& sudoku);
    BoxPermutationGenotype(const Sudoku&, std::array<std::vector<short>,9>);
    virtual ~BoxPermutationGenotype(){};
    BaseGenotype* crossover(BaseGenotype &other);
    virtual void mutate(float mutationRate);
    const std::array<std::vector<short>,9>& getRows() const { return rows; }
protected:
    void fillEvalSudoku();
    std::array<std::vector<short>,9> box;
};
