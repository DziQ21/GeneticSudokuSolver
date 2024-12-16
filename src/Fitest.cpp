#include "Fitest.h"
#include "Population.h"
#include <algorithm> 
#include <random>  
#include <iostream>

//since it modifies population we must remove it later because some of the pointers will be invaild

Population_t TestFitestFunction(Population_t &pop,float removeRate)
{
    Population_t result;
    size_t elementsToKeep = std::ceil(pop.size() * (1 - removeRate));

    for (size_t i = 0; i < elementsToKeep; i++)
    {
        result.push_back(std::move(pop[i]));
    }
    return result;
} 

Population_t TournamentFitestFunction(Population_t &pop, std::size_t resultPopulation, bool preserveselection, const Config &config) {
    if (pop.empty()) return {};

    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(pop.begin(), pop.end(), gen);
    auto removeRate = config.getFittestRate();
    Population_t savedpop;
    Population_t result;
    result.reserve(resultPopulation);
    size_t tournamentSize = std::max(2, int(1 / removeRate));
    int tobreed;
    while (!pop.empty()) 
    {
        Population_t tournamentParticipants;
        for (size_t j=0; j < tournamentSize && !pop.empty(); ++j) {
            tournamentParticipants.push_back(std::move(pop.back()));
            pop.pop_back();
        }
        auto fittest = std::min_element(tournamentParticipants.begin(), tournamentParticipants.end(),
                                         [](const std::unique_ptr<BaseGenotype> &a, const std::unique_ptr<BaseGenotype> &b) {
                                             return a->getEvalValue() < b->getEvalValue();
                                         });
                                         //typedef std::vector<std::unique_ptr<BaseGenotype>> Population_t;
        savedpop.push_back(std::move(*fittest));
    }
    if (preserveselection) {
        if(resultPopulation < savedpop.size())
        {
            //NO CROSSOVER !!! may be just bad results
            std::shuffle(savedpop.begin(), savedpop.end(), gen);
            savedpop.erase(savedpop.begin() + resultPopulation, savedpop.end());
            return savedpop;
        }
        else
        {
            tobreed = resultPopulation - savedpop.size();
            for (int i = 0; i < tobreed; i++)
            {
                result.push_back(savedpop[i]->clone());
            }
        }
    }
    else
    {
        tobreed = resultPopulation;
    }
    std::uniform_int_distribution<> dist(0, savedpop.size() - 1);
    for (int i = 0; i < tobreed; i++) {
        auto parent1 = savedpop[dist(gen)].get();
        auto parent2 = savedpop[dist(gen)].get();
        auto tmp = std::unique_ptr<BaseGenotype>(parent1->crossover(*parent2));
        result.push_back(std::move(tmp));
    }
    return result;
}

Population_t WheelFitestFunction(Population_t &pop, std::size_t resultPopulation, bool preserveselection, const Config &config) {
    Population_t result;
    double sum = 0.0;
    std::vector<double> wheel;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::size_t elementsToKeep = std::ceil(pop.size() * (config.getFittestRate()));
    std::size_t toBreed = preserveselection?resultPopulation-elementsToKeep:resultPopulation;
    if(resultPopulation<elementsToKeep)
    {
        elementsToKeep = resultPopulation;
    }
    // Calculate the inverse of the evaluation values and the sum of the inverses
    for (size_t i = 0; i < pop.size(); i++) {
        double inverseEval = 1.0 / (static_cast<double>(pop[i]->getEvalValue())+1);
        wheel.push_back(inverseEval);
        sum += inverseEval;
    }
    std::size_t parentIdx[2] = {0, 0};
    // Perform the selection based on the inverse values
    while (toBreed > 0||elementsToKeep>0) {
        double random = std::uniform_real_distribution<double>(0.0, sum)(gen);
        for (size_t i = 0; i < pop.size(); i++) {
            random -= wheel[i];
            if (random <= 0) {
                parentIdx[0] = i;
                break; 
            }
        }
        random = std::uniform_real_distribution<double>(0.0, sum)(gen);
        for (size_t i = 0; i < pop.size(); i++) {
            random -= wheel[i];
            if (random <= 0) {
                parentIdx[1] = i;
                break;
            }
        }
        if(elementsToKeep>0)
        {
            elementsToKeep--;
            result.push_back(pop[parentIdx[0]].get()->clone());
        }
        if(toBreed>0)
        {
            //some experimental crossover????
            toBreed--;
            auto tmp = std::unique_ptr<BaseGenotype>(pop[parentIdx[0]].get()->crossover(*pop[parentIdx[1]].get()));
            result.push_back(std::move(tmp));
        }
        
    }

    return result;
}
