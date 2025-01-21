#include "Fitest.h"
#include "Population.h"
#include <algorithm> 
#include <random>  
#include <iostream>
#include <cmath>

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

    #pragma omp parallel
    {
        Population_t thread_savedpop;
        #pragma omp for nowait
        for (size_t i = 0; i < pop.size(); i += tournamentSize) {
            Population_t tournamentParticipants;
            for (size_t j = 0; j < tournamentSize && i + j < pop.size(); ++j) {
                tournamentParticipants.push_back(std::move(pop[i + j]));
            }
            auto fittest = std::min_element(tournamentParticipants.begin(), tournamentParticipants.end(),
                                            [](const std::unique_ptr<BaseGenotype> &a, const std::unique_ptr<BaseGenotype> &b) {
                                                return a->getEvalValue() < b->getEvalValue();
                                            });
            if (fittest != tournamentParticipants.end()) {
                thread_savedpop.push_back(std::move(*fittest));
            }
        }

        #pragma omp critical
        {
            savedpop.insert(savedpop.end(), std::make_move_iterator(thread_savedpop.begin()), std::make_move_iterator(thread_savedpop.end()));
        }
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
            for (std::size_t i = 0; i < savedpop.size(); i++)
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
        if (tobreed-i>2&&config.getMultiCrossover())
        {
            std::array<BaseGenotype*,2> tmp = parent1->multiCrossover(*parent2);
            result.push_back(std::unique_ptr<BaseGenotype>(tmp[0]));
            result.push_back(std::unique_ptr<BaseGenotype>(tmp[1]));
            i++;
        }
        else{
            auto tmp = std::unique_ptr<BaseGenotype>(parent1->crossover(*parent2));
            result.push_back(std::move(tmp));
        }
    }
    return result;
}

Population_t WheelFitestFunction(Population_t &pop, std::size_t resultPopulation, bool preserveselection, const Config &config) {
    
    Population_t result;
    double sum = 0.0;
    std::vector<double> wheel;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::size_t elementsToKeep = std::min((std::size_t)std::ceil(pop.size() * (config.getFittestRate())), resultPopulation);
    // std::cout << "elementsToKeep: " <<" "<< elementsToKeep<<" " << resultPopulation <<" "<< preserveselection << std::endl;
    std::size_t toBreed = preserveselection?resultPopulation-elementsToKeep:resultPopulation;
    if(resultPopulation<elementsToKeep)
    {
        elementsToKeep = resultPopulation;
    }
    std::sort(pop.begin(), pop.end(), [](const std::unique_ptr<BaseGenotype> &a, const std::unique_ptr<BaseGenotype> &b) {
        return a->getEvalValue() < b->getEvalValue();
    });
    result.reserve(resultPopulation);
    //copy n best elems
    for (size_t i = 0; i < elementsToKeep; i++) {
        result.push_back(std::move(pop[i]->clone()));
    }
    // Calculate the inverse of the evaluation values and the sum of the inverses
    for (size_t i = 0; i < pop.size(); i++) {
        double inverseEval = 200-(static_cast<double>(pop[i]->getEvalValue()));
        wheel.push_back(inverseEval);
        sum += inverseEval;
    }
    std::size_t parentIdx[2] = {0, 0};

    // Perform the selection based on the inverse values
    while (toBreed > 0) {
        // printf("WheelFitestFunction 23 %lld %lld\n",toBreed,elementsToKeep);
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
        if(toBreed>0)
        {
            //some experimental crossover????
            if (toBreed>1&&config.getMultiCrossover())
            {
                toBreed-=2;
                std::array<BaseGenotype*,2> tmp = pop[parentIdx[0]].get()->multiCrossover(*pop[parentIdx[1]].get());
                result.push_back(std::unique_ptr<BaseGenotype>(tmp[0]));
                result.push_back(std::unique_ptr<BaseGenotype>(tmp[1]));
            }
            else
            {
                toBreed--;
                auto tmp = std::unique_ptr<BaseGenotype>(pop[parentIdx[0]].get()->crossover(*pop[parentIdx[1]].get()));
                result.push_back(std::move(tmp));
            }
            
        }
        
    }
    return result;
}


Population_t ExponentWheelFitestFunction(Population_t &pop, std::size_t resultPopulation, bool preserveselection, const Config &config) {
    Population_t result;
    double sum = 0.0;
    std::vector<double> wheel;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::size_t elementsToKeep = std::min((std::size_t)std::ceil(pop.size() * (config.getFittestRate())), resultPopulation);
    std::size_t toBreed = preserveselection?resultPopulation-elementsToKeep:resultPopulation;
    if(resultPopulation<elementsToKeep)
    {
        elementsToKeep = resultPopulation;
    }
    std::sort(pop.begin(), pop.end(), [](const std::unique_ptr<BaseGenotype> &a, const std::unique_ptr<BaseGenotype> &b) {
        return a->getEvalValue() < b->getEvalValue();
    });
    result.reserve(resultPopulation);
    //copy n best elems
    for (size_t i = 0; i < elementsToKeep; i++) {
        result.push_back(std::move(pop[i]->clone()));
    }
    // Calculate the inverse of the evaluation values and the sum of the inverses
    for (size_t i = 0; i < pop.size(); i++) {
        //calculate exponent of the evaluation value
        double inverseEval = pow(2,-(static_cast<double>(pop[i]->getEvalValue())));
        wheel.push_back(inverseEval);
        sum += inverseEval;
    }
    std::size_t parentIdx[2] = {0, 0};
    // Perform the selection based on the inverse values
    while (toBreed > 0) {
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
        if(toBreed>0)
        {
            if (toBreed>1&&config.getMultiCrossover())
            {
                toBreed=toBreed-2;
                std::array<BaseGenotype*,2> tmp = pop[parentIdx[0]].get()->multiCrossover(*pop[parentIdx[1]].get());
                result.push_back(std::unique_ptr<BaseGenotype>(tmp[0]));
                result.push_back(std::unique_ptr<BaseGenotype>(tmp[1]));
            }
            else
            {
                toBreed--;
                auto tmp = std::unique_ptr<BaseGenotype>(pop[parentIdx[0]].get()->crossover(*pop[parentIdx[1]].get()));
                result.push_back(std::move(tmp));
            }
        }
        
    }

    return result;
}
