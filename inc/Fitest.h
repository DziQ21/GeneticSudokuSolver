#pragma once

#include "Population.h" //for Population_t

Population_t TestFitestFunction(Population_t &pop, std::size_t resultPopulation, bool preserveselection, const Config &config);
Population_t TournamentFitestFunction(Population_t &pop, std::size_t resultPopulation, bool preserveselection, const Config &config);
Population_t WheelFitestFunction(Population_t &pop, std::size_t resultPopulation, bool preserveselection, const Config &config);
Population_t ExponentWheelFitestFunction(Population_t &pop, std::size_t resultPopulation, bool preserveselection, const Config &config);