#include "SudokuLoader.h"
#include <fstream>
#include <iostream>


SudokuLoader::~SudokuLoader()
{

}

SudokuLoader::SudokuLoader(const std::string& file)
{
    parseFile(file);
}

std::string SudokuLoader::sudokuToStr()
{
    std::string res = "";
    for(int i = 0; i < 9; i++)
    {
        for(int j = 0; j < 9; j++)
        {
            res += std::to_string(sudoku[i][j]) + " ";
        }
        res += "\n";
    }
    return res;
}


void SudokuLoader::parseFile(const std::string& file)
{

    std::ifstream infile(file);
    if (!infile.is_open()) {
        // std::cerr << "Failed to open file: " << file << std::endl;
        //logerror here
        return;
    }
    std::string line;
    int row = 0;

    // Read the sudoku grid
    for (; row < 9 && std::getline(infile, line); ++row) {
        for (int col = 0; col < 9; ++col) {
            sudoku[row][col] = (line[col] == 'b') ? -1 : (line[col] - '0');
        }
    }

    // Read the solution grid
    for (row = 0; row < 9 && std::getline(infile, line); ++row) {
        for (int col = 0; col < 9; ++col) {
            solution[row][col] = (line[col] - '0');
        }
    }

    infile.close();
}