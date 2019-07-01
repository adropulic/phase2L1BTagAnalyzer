/**************************************************************/
/* makeTable.C                                                */
/* Author: Stephanie Kwan                                     */
/**************************************************************/

/* Usage:
   root .x makeTable.C */
   
#include <iostream>
#include <array>
#include <stdio.h>

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

/**************************************************************/

/* Prepare 2D array of doubles, with an extra column for the
   discriminant value. iNumVars is the number of variables, 
   piBins is a list of ints that are the (variable ok) number 
   of bins. */

/* double**   */
double** create2DArray(int numVars, int piBins[])
{
  /* The 2D array of doubles. */
  double **table;

  /* Total number of rows. */
  int nRows = 1;

  /* Index to loop over rows. */
  int iRow;

  /* Calculate total number of rows. */
  for (int i = 0; i < numVars; i++)
      nRows *= piBins[i];

  /* Allocate memory for the new table. */
  table = new double*[nRows];
  for (int iRow = 0; iRow < nRows; iRow++)
      table[iRow] = new double[numVars + 1];

  /* For each variable, */
  for (int i = 0; i < numVars; i++)
    {
      /* Start at the top of a column: */
      iRow = 0;
      
      

    }


  return table;
}



/**************************************************************/

/* Main function. */
void makeTable(void)
{
  double **table = create2DArray(NumVars, NumBins);

  /* Fill the array. */

  //  printTable(Bins, NumVars, NumBins);

  delete(table);
}
