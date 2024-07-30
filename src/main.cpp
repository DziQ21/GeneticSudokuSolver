#include <stdio.h>     
#include "SudokuLoader.h"
#include "ConfigLoader.h"
#include "Logger.h"
#include <iostream>
#include "Population.h"



int main(void)
{
  ConfigLoader configLoader("./Config.txt");
  SudokuLoader sudoku(configLoader.getConfig().getSudokuPath());

  Population population(100,sudoku.getSudoku());
  std::cout<<sudoku.sudokuToStr();
  // LOGERROR<<"test log";
    // unsigned int b=a; 
  // printf("Hello World aaads%d!\n",a);
  return 0;
} /*** end of main ***/
