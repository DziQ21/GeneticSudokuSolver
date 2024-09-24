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

  Population<SoloNumGenotype> population((int)configLoader.getConfig().getPopulationSize(),sudoku.getSudoku(),TestFitestFunction);
  // Population<SoloNumGenotype> population(1,sudoku.getSudoku(),TestFitestFunction);
  
  std::cout<<sudoku.sudokuToStr()<<std::endl;
  for(int i =1; i<200; i++)
  {
    population.print(1);
    population.nextGeneration();
    printf("Generation %d\n",i);
  }
  population.print(10);
  // population.print(1); 
  // LOGERROR<<"test log";
    // unsigned int b=a; 
  // printf("Hello World aaads%d!\n",a);
  return 0;
} /*** end of main ***/
