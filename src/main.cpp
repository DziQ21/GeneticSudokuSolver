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
  std::unique_ptr<ConfigLoader> configLoaderPtr;
  if (argc < 2) {
      configLoaderPtr = std::make_unique<ConfigLoader>("./Config.txt");
  }
  else
  {
    std::string configFilePath = argv[1];
    configLoaderPtr = std::make_unique<ConfigLoader>(configFilePath);
  }

  
  SudokuLoader sudoku(configLoaderPtr->getConfig().getSudokuPath());

  Population<BoxPermutationGenotype> population(configLoaderPtr->getConfig(), sudoku.getSudoku(), TournamentFitestFunction);
  std::cout << sudoku.sudokuToStr() << std::endl;
  for (int i = 1; i < 600; i++)
  {
    population.print(1);
    population.nextGeneration();
    printf("Generation %d\n", i);
  }
  population.print(10);
  // Population<BoxPermutationGenotype> population(configLoader.getConfig(), sudoku.getSudoku(), TestFitestFunction);
  // population.print(1); 
  // population.nextGeneration();

  return 0;
}
