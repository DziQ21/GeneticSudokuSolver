#include <vector>
#include <random>
#include "SudokuLoader.h"
#include "Genotype.h"

SoloNumGenotype::SoloNumGenotype()
{
    
    int count = 0;
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (sudoku[i][j] != -1) {
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

void SoloNumGenotype::crossover()
{
    // Crossover implementation
}

void SoloNumGenotype::print()
{
    // Print implementation
}

void SoloNumGenotype::fillEvalSudoku()
{
    // fillEvalSudoku implementation
}