
#include <vector>
#include "Genotype.h"
#include "SudokuLoader.h"
#include "Population.h"
#include <memory>
#include "SudokuLoader.h"


template <typename T>
Population<T>::Population(int size, const Sudoku& sudoku) : sudoku(sudoku)
{
    // Initialize the population with random genotypes
    for (int i = 0; i < size; i++)
    {
        // make unique and push
        //population deff from class bellow
        // std::vector<BaseGenotype> population;
        population.push_back(std::make_unique<T>(T(sudoku)));
    }
}

template <typename T>
void Population<T>::evolve()
{
    // Implement the evolution process
    // (e.g., selection, crossover, mutation)
}
template <typename T>
void Population<T>::print()
{
    // Print the current population
    // for (const auto& genotype : population)
    // {
    // }
    for (size_t i = 0; i < population.size(); i++)
    {
        population[i]->print();
    }
}

template class Population<SoloNumGenotype>;//its needed for some reason