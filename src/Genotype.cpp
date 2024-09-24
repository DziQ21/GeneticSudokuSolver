#include <vector>
#include <random>
#include "SudokuLoader.h"
#include "Genotype.h"
#include <unordered_set>
#include <iostream>
#include <cassert>

SoloNumGenotype::SoloNumGenotype(const Sudoku& sudoku):BaseGenotype(sudoku)
{
    genoType = SoloNum;
    evalSudokuValid = false;
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

SoloNumGenotype::SoloNumGenotype(const Sudoku& sudoku, std::vector<short> numbers):BaseGenotype(sudoku)
{
    genoType = SoloNum;
    sudokunumbers = numbers;
    evalSudokuValid = false;
}

BaseGenotype* SoloNumGenotype::crossover(BaseGenotype &other)
{

    BaseGenotype* result = nullptr;
    switch (other.getGenoType())
    {
    case SoloNum:
        {
            SoloNumGenotype& otherSolo = static_cast<SoloNumGenotype&>(other);
            const std::vector<short>& otherNumbers = otherSolo.getNumbers();
            std::vector<short> newNumbers;
            newNumbers.reserve(sudokunumbers.size());

            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> distrib(0, 1); 

        for (size_t i = 0; i < sudokunumbers.size(); ++i)
        {
            if (distrib(gen) == 0)
            {
                newNumbers.push_back(sudokunumbers[i]);
            }
            else
            {
                newNumbers.push_back(otherNumbers[i]);
            }
        }
        result = new SoloNumGenotype(sudoku, newNumbers); 
        }
        break;
    
    default:
        assert("ilegal crossover"&&0);
    }

    return result;
}

void BaseGenotype::print()
{
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
void SoloNumGenotype::mutate(float mutationRate)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 1);
    std::uniform_int_distribution<> dis2(0, 8);
    for (size_t i = 0; i < sudokunumbers.size(); i++)
    {
        if (dis(gen) < mutationRate)
        {
            evalSudokuValid = false;
            sudokunumbers[i] = dis2(gen);
        }
    }
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
    // std::cout<<"BaseGenotype::evaluate()"<<std::endl;
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
        std::array<bool,9> colisionRow = {false, false, false, false, false, false, false, false, false};
        std::array<bool,9> colisionCol = {false, false, false, false, false, false, false, false, false};
        for (int j = 0; j < 9; j++) {
            // Row check
            if (colisionRow[evalSudoku[i][j]-1]) {
                evalValue++;
                rowcount[i]++;
            } else {
                colisionRow[evalSudoku[i][j]-1] = true;
            }

            // Column check
            if (colisionCol[evalSudoku[j][i]-1]) {
                evalValue++;
                colcount[i]++;
            } else {
                colisionCol[evalSudoku[j][i]-1] = true;
            }
        }
    }
    int box = 0;
    // Check for 3x3 subgrid collisions
    for (int row = 0; row < 9; row += 3) {
        for (int col = 0; col < 9; col += 3) {
            std::array<bool,9> colisionBox = {false, false, false, false, false, false, false, false, false};
            box++;
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    int val = evalSudoku[row + i][col + j];
                    if (colisionBox[val-1]) {
                        // if(box==1)
                        // std::cout<< "row col val" << box <<" "<<row<<i<<" "<<col<<j<<" "<<val<<std::endl;
                        evalValue++;
                        boxcount[box-1]++;
                    } else {
                        colisionBox[val-1] = true;
                    }
                }
            }
        }
    }
}
