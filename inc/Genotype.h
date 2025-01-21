#pragma once
#include "SudokuLoader.h"
#include <memory>
#include <array>

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
    BaseGenotype(const Sudoku& sudoku): sudoku(sudoku), evalSudokuValid(false) {};
    virtual ~BaseGenotype() {};
    virtual void mutate(float mutationRate,bool) = 0;
    virtual BaseGenotype* crossover(BaseGenotype& other) = 0;
    void evaluate();
    void print();
    std::string getPrintStr();
    int getEvalValue() const { return evalValue; }
    GenotypeType getGenoType() const { return genoType; }
    virtual std::unique_ptr<BaseGenotype> clone() const = 0;
    virtual std::array<BaseGenotype*,2> multiCrossover(BaseGenotype& other){return {crossover(other),crossover(other)};};

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
    SoloNumGenotype(const SoloNumGenotype& other); // Copy constructor
    virtual ~SoloNumGenotype();
    BaseGenotype* crossover(BaseGenotype &other);
    virtual void mutate(float mutationRate, bool);
    std::vector<short> getNumbers() const { return sudokunumbers; }
    virtual std::unique_ptr<BaseGenotype> clone() const { return std::make_unique<SoloNumGenotype>(*this); }
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
    FullPermutationGenotype(const FullPermutationGenotype& other); // Copy constructor
    virtual ~FullPermutationGenotype() {};
    BaseGenotype* crossover(BaseGenotype &other);
    virtual void mutate(float mutationRate, bool);
    std::vector<short> getNumbers() const { return sudokunumbers; }
    virtual std::unique_ptr<BaseGenotype> clone() const { return std::make_unique<FullPermutationGenotype>(*this); }
protected:
    void fillEvalSudoku();
    std::vector<short> sudokunumbers;
};

class RowPermutationGenotype : public BaseGenotype
{
public:
    RowPermutationGenotype(const Sudoku& sudoku);
    RowPermutationGenotype(const Sudoku&, std::array<std::vector<short>,9>);
    RowPermutationGenotype(const RowPermutationGenotype& other); // Copy constructor
    virtual ~RowPermutationGenotype() {};
    BaseGenotype* crossover(BaseGenotype &other);
    virtual void mutate(float mutationRate, bool);
    const std::array<std::vector<short>,9>& getRows() const { return rows; }
    virtual std::unique_ptr<BaseGenotype> clone() const { return std::make_unique<RowPermutationGenotype>(*this); }
protected:
    void fillEvalSudoku();
    std::array<std::vector<short>,9> rows;
};

class BoxPermutationGenotype : public BaseGenotype
{
public:
    BoxPermutationGenotype(const Sudoku& sudoku);
    BoxPermutationGenotype(const Sudoku&, std::array<std::vector<short>,9>);
    BoxPermutationGenotype(const BoxPermutationGenotype& other); // Copy constructor
    virtual ~BoxPermutationGenotype() {};
    BaseGenotype* crossover(BaseGenotype &other);
    virtual void mutate(float mutationRate, bool);
    const std::array<std::vector<short>,9>& getBox() const { return box; }
    virtual std::unique_ptr<BaseGenotype> clone() const { return std::make_unique<BoxPermutationGenotype>(*this); }
    virtual std::array<BaseGenotype*,2> multiCrossover(BaseGenotype& other);
protected:
    void fillEvalSudoku();
    std::array<std::vector<short>,9> box;
};

