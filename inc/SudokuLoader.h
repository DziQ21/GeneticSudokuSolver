#pragma once
#include <string>
#include <array>

typedef std::array<std::array<short,9>,9> Sudoku;
class SudokuLoader
{
private:
    //-1 is empty field
    Sudoku sudoku;
    Sudoku solution;
    void parseFile(const std::string& file);
public:
    std::string sudokuToStr();
    SudokuLoader(const std::string& file);
    const Sudoku& getSudoku() const {return sudoku;};
    const Sudoku& getSolution() const {return solution;};
    ~SudokuLoader();
};


