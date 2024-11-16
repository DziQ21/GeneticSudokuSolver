#pragma once

#include "Population.h" //for Population_t

Population_t TestFitestFunction(Population_t &pop,float removeRate);
Population_t TournamentFitestFunction(Population_t &pop,float removeRate );
Population_t WheelFitestFunction(Population_t &pop,float removeRate );