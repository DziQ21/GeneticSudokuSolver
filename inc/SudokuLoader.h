#pragma once
#include <string>
#include <array>
class SudokuLoader
{
private:
    //-1 is empty field
    std::array<std::array<short,9>,9> sudoku;
    std::array<std::array<short,9>,9> solution;
    void parseFile(const std::string& file);
public:
    SudokuLoader(const std::string& file);
    ~SudokuLoader();
};


