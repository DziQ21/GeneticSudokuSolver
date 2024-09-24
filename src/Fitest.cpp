#include "Fitest.h"
#include "Population.h"
#include <algorithm> 
#include <random>  

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
    size_t tournamentSize = std::max(2, int(pop.size() * removeRate));
    size_t stepSize = std::max(1, int(pop.size() / tournamentSize)); 

    for (size_t i = 0; i < pop.size() && result.size() < pop.size(); i += stepSize) {
        Population_t tournamentParticipants;
        for (size_t j = i; j < i + tournamentSize && j < pop.size(); ++j) {
            tournamentParticipants.push_back(std::move(pop[j]));
        }

        auto fittest = std::max_element(tournamentParticipants.begin(), tournamentParticipants.end(),
                                         [](const std::unique_ptr<BaseGenotype> &a, const std::unique_ptr<BaseGenotype> &b) {
                                             return a->getEvalValue() < b->getEvalValue();
                                         });
                                         //typedef std::vector<std::unique_ptr<BaseGenotype>> Population_t;
        result.push_back(std::move(*fittest));
    }

    return result;
}


