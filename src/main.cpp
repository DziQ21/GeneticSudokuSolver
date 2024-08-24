#include <stdio.h>     
#include "SudokuLoader.h"
#include "ConfigLoader.h"
#include "Logger.h"
#include <iostream>
#include "Population.h"
#include "Genotype.h"



int main(void)
{
  ConfigLoader configLoader("../Config.txt");
  SudokuLoader sudoku(configLoader.getConfig().getSudokuPath());

  Population<SoloNumGenotype> population((int)2,sudoku.getSudoku());
  std::cout<<sudoku.sudokuToStr()<<std::endl;
  population.print(); 
  // LOGERROR<<"test log";
    // unsigned int b=a; 
  // printf("Hello World aaads%d!\n",a);
  return 0;
} /*** end of main ***/
