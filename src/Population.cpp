
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
Population<T>::Population(const Config& config, const Sudoku& sudoku, std::function<Population_t(Population_t&,float)> fitestFunction) : fitestFunction(fitestFunction), sudoku(sudoku),config(config)
{
    for (int i = 0; i < (int)config.getPopulationSize(); i++)
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
    auto start = std::chrono::high_resolution_clock::now();
    if(a == 1)
    {   
        #pragma omp parallel for
        for (size_t i = 0; i < population.size(); i++)
        {
            population[i]->evaluate();
        }
        //get lowest val and print it
        auto min = std::min_element(population.begin(), population.end(), [](const std::unique_ptr<BaseGenotype> &a, const std::unique_ptr<BaseGenotype> &b) {
            return a->getEvalValue() < b->getEvalValue();
        });
        (*min)->print();
    }
    else
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
    }

    
    double totalEvalValue = std::accumulate(population.begin(), population.end(), 0.0, [](double sum, const std::unique_ptr<BaseGenotype> &genotype) {
        return sum + genotype->getEvalValue();
    });

    double averageEvalValue = totalEvalValue / population.size();

    // Print the average evaluation value
    if(config.getLogLevel() == LogLevel::DBG)
        std::cout << "Average evaluation value: " << averageEvalValue << std::endl;
    auto end = std::chrono::high_resolution_clock::now();
    if(config.getLogLevel() == LogLevel::DBG)
        std::cout << "print finished in " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << std::endl;
}

template <typename T>
void Population<T>::nextGeneration()
{
    auto start = std::chrono::high_resolution_clock::now();
    if(config.getLogLevel() == LogLevel::DBG)
        std::cout << "nextGeneration start" << std::endl;
    #pragma omp parallel for
    for(size_t i = 0; i < population.size(); i++)
    {
        population[i]->evaluate();
    }

    auto end = std::chrono::high_resolution_clock::now();
    if(config.getLogLevel() == LogLevel::DBG)
        std::cout << "eval finished in " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    population = fitestFunction(population, config.getFittestRate());
    end = std::chrono::high_resolution_clock::now();
    if(config.getLogLevel() == LogLevel::DBG)
        std::cout <<population.size()<< "fitestFunction finished in " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    fillRestOfPopulation();
    end = std::chrono::high_resolution_clock::now();
    if(config.getLogLevel() == LogLevel::DBG)
        std::cout << "fillRestOfPopulation  finished in " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << std::endl;
    start = std::chrono::high_resolution_clock::now();
    #pragma omp parallel for
    for(size_t i = 0; i < population.size(); i++)
    {
        population[i]->mutate(config.getMutationRate());
    }
    end = std::chrono::high_resolution_clock::now();
    if(config.getLogLevel() == LogLevel::DBG)
        std::cout << " mutate() finished in " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << std::endl;
}
template <typename T>
std::tuple<float,int> Population<T>::getStats()
{
    #pragma omp parallel for
    for(size_t i = 0; i < population.size(); i++)
    {
        population[i]->evaluate();
    }
    float totalEvalValue = std::accumulate(population.begin(), population.end(), 0.0, [](float sum, const std::unique_ptr<BaseGenotype> &genotype) {
        return sum + genotype->getEvalValue();
    });

    float averageEvalValue = totalEvalValue / population.size();

    auto min = std::min_element(population.begin(), population.end(), [](const std::unique_ptr<BaseGenotype> &a, const std::unique_ptr<BaseGenotype> &b) {
            return a->getEvalValue() < b->getEvalValue();
        });

    return std::make_tuple(averageEvalValue, min->get()->getEvalValue());
}

template <typename T>
void Population<T>::SoftReset()
{
    
}

template <typename T>
void Population<T>::fillRestOfPopulation()
{
    Population_t tmpPopulation;
    tmpPopulation.reserve(config.getPopulationSize());
    #pragma omp parallel
    {
        // Create a thread-local temporary population
        Population_t localTmpPopulation;
        localTmpPopulation.reserve(config.getPopulationSize());
        
        //flag if save population or only corssover
        #pragma omp for
        for (size_t i = 0; i < (std::size_t)config.getPopulationSize() - population.size(); ++i)
        {
            static std::random_device rd; // Obtain a random number from hardware
            static std::mt19937 eng(rd()); // Seed the generator
            std::uniform_int_distribution<> distr(0, population.size() - 1);

            int parent1Index = distr(eng);
            int parent2Index = distr(eng);

            auto tmp = std::unique_ptr<BaseGenotype>(population[parent1Index]->crossover(*population[parent2Index]));
            localTmpPopulation.push_back(std::move(tmp));
        }

        // Merge the thread-local temporary population into the main temporary population
        #pragma omp critical
        {
            tmpPopulation.insert(tmpPopulation.end(), std::make_move_iterator(localTmpPopulation.begin()), std::make_move_iterator(localTmpPopulation.end()));
        }
    }
    if(config.getLogLevel() == LogLevel::DBG)
        std::cout<<"to fill "<<tmpPopulation.size()<<std::endl;
    //flag if save population or only corssover
    for (size_t i = 0; i < population.size(); i++)
    {
        tmpPopulation.push_back(std::move(population[i]));
    }
    population = std::move(tmpPopulation);
}
//its needed for some reason
template class Population<SoloNumGenotype>;
template class Population<FullPermutationGenotype>;
template class Population<RowPermutationGenotype>;
template class Population<BoxPermutationGenotype>;