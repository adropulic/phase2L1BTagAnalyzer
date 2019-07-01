/*********************************************************/
/* makeTable.C                                           */
/* Author: Stephanie Kwan                                */
/*********************************************************/

/* Usage:
   root .x makeTable.C */
   
#include <iostream>
#include <array>

/* Global variables */
int numVariables = 2;

int dBins[] = {4, 4};
double dL1Pt[] = {0.0, 20.0, 100.0, 500.0};
double dL1Eta[] = {-2.25, -0.75, 0.75, 2.25};
double *pBins[] = {dL1Pt, dL1Eta};

/* Print the contents of a 2D array. */
void printTable(double *array[], int numVars)
{

  for (int i = 0; i < numVars; i++)
    {
      int width = dBins[i];

      for (int j = 0; j < width; j++)
	{
	  std::cout << array[i][j] << ' ';
	}
      std::cout << std::endl;
    }
  
}

/* Main function. */
void makeTable(void)
{
  printTable(pBins, numVariables);
}
