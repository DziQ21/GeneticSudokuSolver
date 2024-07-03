/************************************************************************************//**
* \file         main.c
* \brief        Application source file.
*
****************************************************************************************/

/****************************************************************************************
* Include files
****************************************************************************************/
#include <stdio.h>                          /* for standard input/output functions     */
#include "SudokuLoader.h"
#include "ConfigLoader.h"
#include "Logger.h"
/************************************************************************************//**
** \brief     This is the program entry point.
** \param     argc Number of program arguments.
** \param     argv Array with program arguments.
** \return    Program return code. 0 for success, error code otherwise.
**
****************************************************************************************/
int main(void)
{
  ConfigLoader configLoader("./Config.txt");
  SudokuLoader sudoku(configLoader.getConfig().getSudokuPath());
  LOGERROR<<"test log";
    // unsigned int b=a; 
  // printf("Hello World aaads%d!\n",a);
  return 0;
} /*** end of main ***/


/*********************************** end of main.c *************************************/

