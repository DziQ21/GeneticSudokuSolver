#include <stdio.h>     
#include "SudokuLoader.h"
#include "ConfigLoader.h"
#include "Logger.h"
#include <iostream>
#include "Population.h"
#include "Genotype.h"
#include "Fitest.h"



int main(void)
{
  ConfigLoader configLoader("./Config.txt");
  SudokuLoader sudoku(configLoader.getConfig().getSudokuPath());

  Population<RowPermutationGenotype> population(configLoader.getConfig(),sudoku.getSudoku(),TournamentFitestFunction);
  std::cout<<sudoku.sudokuToStr()<<std::endl;
  for(int i =1; i<400; i++)
  {
    population.print(1);
    population.nextGeneration();
    printf("Generation %d\n",i);
  }
  population.print(10);

  // Population<FullPermutationGenotype> population(3,sudoku.getSudoku(),TestFitestFunction);
  // population.print(2); 
  // population.nextGeneration();
  // population.nextGeneration();
  // population.nextGeneration();
  // population.nextGeneration();
  return 0;
}
