#include <vector>
#include <random>
#include "SudokuLoader.h"
#include "Genotype.h"
#include <unordered_set>
#include <iostream>

SoloNumGenotype::SoloNumGenotype(const Sudoku& sudoku):BaseGenotype(sudoku)
{
    
    int count = 0;
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (sudoku[i][j] == -1) {
                count++;
            }
        }
    }
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 9);
    sudokunumbers.reserve(count);
    for(int i = 0; i < count; i++)
    {
        sudokunumbers.push_back(dis(gen));
    }
}

SoloNumGenotype::~SoloNumGenotype()
{
    // Destructor implementation
}

BaseGenotype* SoloNumGenotype::crossover(BaseGenotype &other)
{
    // Crossover implementation
    return 0;
}

void BaseGenotype::print()
{
    // Print implementation
    std::cout << getPrintStr() << std::endl;
}
std::string BaseGenotype::getPrintStr()
{
    if (!evalSudokuValid) {
        fillEvalSudoku();
    }
    evaluate();
    std::string str;
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            str += std::to_string(evalSudoku[i][j]);
            str += " ";
        }
        str += "\n";
    }
    str+= "Eval value: " + std::to_string(evalValue)+"\n";
    std::string rowStr = "Row collisions: ", colStr = "Column collisions: ", boxStr = "Box collisions: ";
    for (int i = 0; i < 9; i++) {
        rowStr += std::to_string(i) + ":" + std::to_string(rowcount[i]) + ", ";
        colStr += std::to_string(i) + ":" + std::to_string(colcount[i]) + ", ";
        boxStr += std::to_string(i) + ":" + std::to_string(boxcount[i]) + ", ";
    }
    str += rowStr + "\n" + colStr + "\n" + boxStr + "\n";
    
    return str;
}

void SoloNumGenotype::fillEvalSudoku()
{
    int count = 0;
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (sudoku[i][j] == -1) {
                evalSudoku[i][j] = sudokunumbers[count++];
            }
            else
            {
                evalSudoku[i][j] = sudoku[i][j];
            }
        }
    }
    evalSudokuValid = true;
}

void BaseGenotype::evaluate() {
    if (!evalSudokuValid) {
        fillEvalSudoku();
    }
    evalValue = 0;
    for (int i = 0; i < 9; i++) {
        rowcount[i] = 0;
        colcount[i] = 0;
        boxcount[i] = 0;
    }

    // Check for row and column collisions
    for (int i = 0; i < 9; i++) {
        std::unordered_set<int> rowSet, colSet;
        for (int j = 0; j < 9; j++) {
            // Row check
            if (rowSet.find(evalSudoku[i][j]) != rowSet.end()) {
                evalValue++;
                rowcount[i]++;
            } else {
                rowSet.insert(evalSudoku[i][j]);
            }

            // Column check
            if (colSet.find(evalSudoku[j][i]) != colSet.end()) {
                evalValue++;
                colcount[i]++;
            } else {
                colSet.insert(evalSudoku[j][i]);
            }
        }
    }
    int box = 0;
    // Check for 3x3 subgrid collisions
    for (int row = 0; row < 9; row += 3) {
        for (int col = 0; col < 9; col += 3) {
            box++;
            std::unordered_set<int> gridSet;
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    int val = evalSudoku[row + i][col + j];
                    if (gridSet.find(val) != gridSet.end()) {
                        evalValue++;
                        boxcount[box-1]++;
                    } else {
                        gridSet.insert(val);
                    }
                }
            }
        }
    }
}
