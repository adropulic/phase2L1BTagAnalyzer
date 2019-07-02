/**************************************************************/
/* makeTable.C                                                */
/* Author: Stephanie Kwan                                     */
/**************************************************************/

/* Usage: 
   root .x makeTable.C */

#include <vector>

#include <iostream>
#include <array>
#include <stdio.h>

#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TFrame.h"
#include "TH1F.h"

/**************************************************************/

/* Number of variables */
int NumVars = 2;

/* Number of bins for each variable */
int NumBins[] = {4, 4};

/* Declare values for each variable (must be in the same order
   as iNumBins). */ 
double ValsL1Pt[] = {10., 35., 75., 300.};
double ValsL1Eta[] = {-2.25, -0.75, 0.75, 2.25};
double *Vals[] = {ValsL1Pt, ValsL1Eta};


/**************************************************************/

/* Print the contents of pdArray, which is a list of iNumVars
   pointers, which point to lists of doubles. These lists can
   have variable lengths, which are specified in piBins. */

void printTable(double **pdArray, Int_t numRows, Int_t numCols)
{
  for (int i = 0; i < numRows; i++)
    {
      for (int j = 0; j < numCols; j++)
	  printf("%f ", pdArray[i][j]);
      printf("\n");
    }
}

/**************************************************************/

/* Returns the number of rows, where numVars is the number of
   variables and numBins[] is an array of the number of bins. */
Int_t getNumRows(int numVars, int numBins[])
{
  assert(numVars > 0);
  assert(piBins != NULL);

  Int_t nRows = 1;
  for (Int_t i = 0; i < numVars; i++)
    nRows *= numBins[i];

  return nRows;
}

/**************************************************************/

/* Prepare 2D array of doubles, with an extra column for the
   discriminant value. iNumVars is the number of variables, 
   piBins lists the number of bins. */

double** fillTableWithPermutedValues(int numVars,
				     int numBins[])
{
  /* Number of rows. */
  Int_t nRows = getNumRows(numVars, numBins);

  /* Allocate memory for the new table, adding an extra 
     column for the discriminant value. */
  double** table = new double*[nRows];
  for (Int_t r = 0; r < nRows; r++)
    table[r] = new double[numVars + 1];

  /* Fill the table with permutations of the variables' possible
     values. */
  
  /* For each variable: */
  for (Int_t iVar = 0; iVar < numVars; iVar++)
    {
      printf("iVar = %d\n", iVar);

      /* Start at the top of a column: */
      Int_t iRow = 0;
      
      /* Calculate the product of the remaining variables' number
	 of bins. This is the "block size" that we need to fill
         in one step. */
      Int_t blockSize = 1;
      for (Int_t i = (iVar + 1); i < numVars; i++)
	blockSize *= numBins[i];
      
      printf("Blocksize: %d\n", blockSize);
      
      while (iRow < nRows)
	{
	  /* "bin" is the number of bins for this variable. */
	  for (Int_t iBin = 0; iBin < numBins[iVar]; iBin++)
	    {
	      printf("numBins[iBin] is %d\n", numBins[iBin]);
	      printf("iBin is %d\n", iBin);
	      /* Index r counts which row we are on. */
	      for (Int_t r = iRow; r < (iRow + blockSize); r++)
		{
		  /* Fill the entry. */
		  printf("Row number: %d\n", r);
		  table[r][iVar] = Vals[iVar][iBin];
		}
	      iRow += blockSize;
	    }
	}
    }


  return table;
}

/**************************************************************/

/* Main function. */
int makeTable(void)
{

  /* Create the file, the tree, and the branches. */
  /*  TFile f("table.root", "RECREATE");
  TTree t1("t1", "a Tree with five variables");

  Float_t l1Pt, l1Eta;
  Float_T track1ChiSquared, tauL1StripPt, l1DecayMode;
  Double_t discr;

  t1.Branch("l1Pt", &l1Pt, "l1Pt/F");
  t1.Branch("l1Eta", &l1Eta, "l1Eta/F");
  t1.Branch("track1ChiSquared", &track1ChiSquared,
	    "track1ChiSquared/F");
  t1.Branch("tauL1StripPt", &tauL1StripPt, "tauL1StripPt/F");
  t1.Branch("discr", &discr, "discr/D");
  */
  // Fill the tree.
  
  double **table = fillTableWithPermutedValues(NumVars, NumBins);

	     
  printTable(table, getNumRows(NumVars, NumBins), NumVars + 1);

  delete(table);
  
  return 0;
}
