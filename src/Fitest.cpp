#include "Fitest.h"
#include "Population.h"
#include <algorithm> 
#include <random>  
#include <iostream>

//since it modifies population we must remove it later because some of the pointers will be invaild

Population_t TestFitestFunction(Population_t &pop,float removeRate)
{
    Population_t result;
    for (size_t i = 0; i < pop.size()/2; i++)
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
    
    std::cout<<"TournamentFitestFunction5"<<tournamentSize<<std::endl;
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


