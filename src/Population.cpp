
#include <vector>
#include "Genotype.h"
#include "SudokuLoader.h"
#include "Population.h"
#include <memory>
#include "SudokuLoader.h"
#include <random>
#include <iostream>
#include <chrono>


template <typename T>
Population<T>::Population(int size, const Sudoku& sudoku, std::function<Population_t(Population_t&,float)> fitestFunction) : fitestFunction(fitestFunction), sudoku(sudoku), size(size)
{
    for (int i = 0; i < size; i++)
    {
        // make unique and push
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
void Population<T>::print(size_t a)
{
    for (size_t i = 0; i < population.size(); i++)
    {
        population[i]->evaluate();
    }
    //sort vector then print a best eval value has to be lowest
    std::sort(population.begin(), population.end(), [](const std::unique_ptr<BaseGenotype> &a, const std::unique_ptr<BaseGenotype> &b) {
        return a->getEvalValue() < b->getEvalValue();
    });
    for (size_t i = 0; i < a; i++)
    {
        population[i]->print();
    }
    double totalEvalValue = std::accumulate(population.begin(), population.end(), 0.0, [](double sum, const std::unique_ptr<BaseGenotype> &genotype) {
        return sum + genotype->getEvalValue();
    });

    double averageEvalValue = totalEvalValue / population.size();

    // Print the average evaluation value
    std::cout << "Average evaluation value: " << averageEvalValue << std::endl;

}

template <typename T>
void Population<T>::nextGeneration()
{
    auto start = std::chrono::high_resolution_clock::now();
    std::cout << "nextGeneration start" << std::endl;

    for(size_t i = 0; i < population.size(); i++)
    {
        population[i]->evaluate();
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "eval finished in " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    population = fitestFunction(population, 0.3);
    end = std::chrono::high_resolution_clock::now();
    std::cout << "fitestFunction finished in " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    fillRestOfPopulation();
    end = std::chrono::high_resolution_clock::now();
    std::cout << "fillRestOfPopulation  finished in " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << std::endl;
    start = std::chrono::high_resolution_clock::now();
    for(size_t i = 0; i < population.size(); i++)
    {
        population[i]->mutate(0.01);
    }
    end = std::chrono::high_resolution_clock::now();
    std::cout << " mutate() finished in " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << std::endl;
}

template <typename T>
void Population<T>::fillRestOfPopulation()
{
    Population_t tmpPopulation;
    tmpPopulation.reserve(size);
    //heres definition of funtion fill rest BaseGenotype* SoloNumGenotype::crossover(BaseGenotype &other)
    while (tmpPopulation.size() < (std::size_t)size-population.size())
    {
        static std::random_device rd; // Obtain a random number from hardware
        static std::mt19937 eng(rd()); // Seed the generator
        std::uniform_int_distribution<> distr(0, population.size() - 1); 

        int parent1Index = distr(eng);
        int parent2Index = distr(eng);

        auto tmp = std::unique_ptr<BaseGenotype>(population[parent1Index]->crossover(*population[parent2Index]));
        tmpPopulation.push_back(std::move(tmp));
    }
    std::cout<<"to fill "<<tmpPopulation.size()<<std::endl;
    for (size_t i = 0; i < population.size(); i++)
    {
        tmpPopulation.push_back(std::move(population[i]));
    }
    population = std::move(tmpPopulation);
}

template class Population<SoloNumGenotype>;//its needed for some reason