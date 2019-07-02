/**************************************************************/
/* makeTable.C                                                */
/* Author: Stephanie Kwan                                     */
/**************************************************************/

/* Usage: specify the variables in the makeTable function.
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
/*
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
*/
/**************************************************************/

/* Prepare 2D array of doubles, with an extra column for the
   discriminant value. iNumVars is the number of variables, 
   piBins is a list of ints that are the (variable ok) number 
   of bins. */

double** fillTableWithPermutedValues(int numVars,
				     int piBins[])
{
  assert(numVars > 0);
  assert(piBins != NULL);

  /* Total number of rows. Initialize to 1 and calculate it. */
  Int_t nRows = 1;
  for (Int_t i = 0; i < numVars; i++)
      nRows *= piBins[i];

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
      /* Start at the top of a column: */
      Int_t iRow = 0;
      
      /* Calculate the product of the remaining variables' number
	 of bins. This is the "block size" that we need to fill
         in one step. */
      Int_t blockSize = 1;
      for (Int_t i = iVar; i < numVars; i++)
	blockSize *= piBins[i];
      
      while (iRow < nRows)
	{
	  /* Index iBin counts which bin we are on. */
	  for (Int_t iBin = 0; iBin < piBins[iBin]; iBin++)
	    {
	      /* Index r counts which row we are on. */
	      for (Int_t r = iRow; r < (iRow + blockSize); r++)
		{
		  /* Fill the entry. */
		  printf("%d\n", r);
		  //	  table[r][iVar] = Vals[iVar][iBin];
		}
	      iRow += blockSize;
	    }
	}
    }


  return table;
}

/**************************************************************/




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

  delete(table);
  
  return 0;
}
