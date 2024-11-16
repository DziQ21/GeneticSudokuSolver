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

Population_t TournamentFitestFunction(Population_t &pop,float removeRate)
{
    if (pop.empty()) return {};
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(pop.begin(), pop.end(), gen);
    Population_t result;
    size_t tournamentSize = std::max(2, int(1/removeRate));
    
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
        result.push_back(std::move(*fittest));
    }
    return result;
}

Population_t WheelFitestFunction(Population_t &pop, float removeRate) {
    Population_t result;
    double sum = 0.0;
    std::vector<double> wheel;
    std::random_device rd;
    std::mt19937 gen(rd());
    size_t elementsToKeep = static_cast<size_t>(pop.size() * (removeRate));
    // Calculate the inverse of the evaluation values and the sum of the inverses
    for (size_t i = 0; i < pop.size(); i++) {
        double inverseEval = 1.0 / (static_cast<double>(pop[i]->getEvalValue())+1);
        wheel.push_back(inverseEval);
        sum += inverseEval;
    }

    // Perform the selection based on the inverse values
    while (result.size() < elementsToKeep) {
        double random = std::uniform_real_distribution<double>(0.0, sum)(gen);
        for (size_t i = 0; i < pop.size(); i++) {
            random -= wheel[i];
            if (random <= 0) {
                result.push_back(pop[i]->clone());
                break; // Exit the loop once an element is added
            }
        }
    }

    return result;
}
