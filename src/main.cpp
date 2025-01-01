#include <stdio.h>     
#include "SudokuLoader.h"
#include "ConfigLoader.h"
#include "Logger.h"
#include <iostream>
#include "Population.h"
#include "Genotype.h"
#include "Fitest.h"

int main(int argc, char *argv[])
{
    std::function<Population_t(Population_t&, std::size_t, bool, const Config &)> fittestTable[] = {TournamentFitestFunction, WheelFitestFunction, ExponentWheelFitestFunction};
    ConfigLoader configLoader;
    if (argc < 2) {
      // configLoaderPtr = std::make_unique<ConfigLoader>("./Config.txt");
      configLoader.loadConfigPath("./config2137.txt");
    }
    else
    {
      std::string configStr= argv[1];
      configLoader.loadConfigPath(configStr);
    }
    std::unique_ptr<BasePopulation> population;
    SudokuLoader sudoku(configLoader.getConfig().getSudokuPath());
    int lastValue = 9999;
    int lastImprovemnt = 0;
    switch (configLoader.getConfig().getSolverType())
    {
    case 0:
        population = std::make_unique<Population<BoxPermutationGenotype>>(configLoader.getConfig(), sudoku.getSudoku(), fittestTable[configLoader.getConfig().getFittest()]);
    
        break;
    case 1:
        population = std::make_unique<Population<RowPermutationGenotype>>(configLoader.getConfig(), sudoku.getSudoku(), fittestTable[configLoader.getConfig().getFittest()]);
    
        break;
    case 2:
        population = std::make_unique<Population<FullPermutationGenotype>>(configLoader.getConfig(), sudoku.getSudoku(), fittestTable[configLoader.getConfig().getFittest()]);
    
        break;
    case 3:
        population = std::make_unique<Population<SoloNumGenotype>>(configLoader.getConfig(), sudoku.getSudoku(), fittestTable[configLoader.getConfig().getFittest()]);
        break;
    default:
        break;
    }
    if(configLoader.getConfig().getLogLevel() == LogLevel::DBG)
        std::cout << sudoku.sudokuToStr() << std::endl;
    for (int i = 1; i < 10000; i++)
    {
        if(configLoader.getConfig().getLogLevel() == LogLevel::DBG)
            population->print(1);
        population->nextGeneration();
        auto stat = population->getStats();
        
        printf("Generation %d, mean: %f , best = %d\n", i, std::get<0>(stat),std::get<1>(stat));
        if(configLoader.getConfig().getLogLevel() == LogLevel::DBG)
          printf("Generation %d\n", i);
        if(std::get<1>(stat) == 0)
        {
            if(configLoader.getConfig().getLogLevel() == LogLevel::DBG)
                printf("Solution found in generation %d\n", i);
            break;
        }
        if(lastValue > std::get<1>(stat))
        {
            lastValue = std::get<1>(stat);
            lastImprovemnt = 0;
        }
        else
        {
            lastImprovemnt++;
        }
        if(lastImprovemnt > configLoader.getConfig().getHarashMutationConfig().withoutImprovment&&configLoader.getConfig().getHarashMutationConfig().withoutImprovment!=0)
        {
            lastValue = 9999;
            lastImprovemnt = 0;
            if(configLoader.getConfig().getLogLevel() == LogLevel::DBG)
                printf("Resetting population 1\n");
            population->harashMutation(configLoader.getConfig().getHarashMutationConfig().muationRate);
            lastImprovemnt = 0;
        }
        if(lastImprovemnt > configLoader.getConfig().getResetCounter())
        {
            lastValue = 9999;
            lastImprovemnt = 0;
            if(configLoader.getConfig().getLogLevel() == LogLevel::DBG)
                printf("Resetting population 2\n");
        switch (configLoader.getConfig().getSolverType())
        {
        case 0:
            population = std::make_unique<Population<BoxPermutationGenotype>>(configLoader.getConfig(), sudoku.getSudoku(), fittestTable[configLoader.getConfig().getFittest()]);
    
        break;
        case 1:
            population = std::make_unique<Population<RowPermutationGenotype>>(configLoader.getConfig(), sudoku.getSudoku(), fittestTable[configLoader.getConfig().getFittest()]);
        
            break;
        case 2:
            population = std::make_unique<Population<FullPermutationGenotype>>(configLoader.getConfig(), sudoku.getSudoku(), fittestTable[configLoader.getConfig().getFittest()]);
            break;
        case 3:
            population = std::make_unique<Population<SoloNumGenotype>>(configLoader.getConfig(), sudoku.getSudoku(), fittestTable[configLoader.getConfig().getFittest()]);
            break;
        default:
            break;
        }
             lastImprovemnt = 0;
        }
    }
    if(configLoader.getConfig().getLogLevel() == LogLevel::DBG)
        population->print(10);
    // Population<BoxPermutationGenotype> population(configLoader.getConfig(), sudoku.getSudoku(), TestFitestFunction);
    // population.print(1); 
    // population.nextGeneration();

    return 0;
}
