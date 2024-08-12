
#include <vector>
#include "Genotype.h"
#include "SudokuLoader.h"
#include "Population.h"
#include <memory>

template <typename T>
Population<T>::Population(int size, const Sudoku& sudoku) : sudoku(sudoku)
{
    // Initialize the population with random genotypes
    for (int i = 0; i < size; i++)
    {
        // make unique and push
        //population deff from class bellow
        // std::vector<BaseGenotype> population;
        population.push_back(std::make_unique<T>(new T(sudoku)));
    }
}

Population::~Population()
{
    // Clean up any dynamically allocated resources
    // (if necessary)
}

void Population::evolve()
{
    // Implement the evolution process
    // (e.g., selection, crossover, mutation)
}

void Population::print()
{
    // Print the current population
    // for (const auto& genotype : population)
    // {
    // }
}