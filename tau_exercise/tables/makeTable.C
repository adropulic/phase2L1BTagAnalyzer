/**************************************************************/
/* makeTable.C                                                */
/* Author: Stephanie Kwan                                     */
/**************************************************************/

/* Usage:
   root .x makeTable.C */
   
#include <iostream>
#include <array>

/**************************************************************/

/* Number of variables */
int NumVars = 2;

/* Number of bins for each variable */
int NumBins[] = {4, 4};

/* Declare bins for each variable (must be in the same order
   as iNumBins). */ 
double BinsL1Pt[] = {0.0, 20.0, 100.0, 500.0};
double BinsL1Eta[] = {-2.25, -0.75, 0.75, 2.25};
double *Bins[] = {BinsL1Pt, BinsL1Eta};


/**************************************************************/

/* Print the contents of pdArray, which is a list of iNumVars
   pointers, which point to lists of doubles. These lists can
   have variable lengths, which are specified in piBins. */

void printTable(double *pdArray[], int iNumVars, int piBins[])
{
  for (int i = 0; i < iNumVars; i++)
    {
      for (int j = 0; j < piBins[i]; j++)
	{
	  std::cout << pdArray[i][j] << ' ';
	}
      std::cout << std::endl;
    }
}

/**********************************************************/

/* Main function. */
void makeTable(void)
{
  printTable(Bins, NumVars, NumBins);
}
