/**************************************************************/
/* makeTable.C                                                */
/* Author: Stephanie Kwan                                     */
/**************************************************************/

/* Usage: 
   root .x makeTable.C 

   Specify the number of values for each variable, and the values
   themselves in the global variables after the #include statements. 

   You also need to change the variables declared to the Reader,
   in the function fillTableWithTMVAdiscriminant.

   N.B.: L1StripPt is set to 0 for tau candidates with 1-prong
         or 3-prong (DM 1 or 10) decay modes.

*/

#include <vector>
#include <iostream>
#include <array>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <fstream>

#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TSystem.h"
#include "TFrame.h"
#include "TH1F.h"

#if not defined(__CINT__) || defined(__MAKECINT__)
#include "TMVA/Tools.h"
#include "TMVA/Reader.h"
#include "TMVA/MethodCuts.h"
#endif

/**************************************************************/

/* Number of bins for each variable */
Int_t NumBins[] = {4, 3, 4, 4, 5};

/* Declare values for each variable (must be in the same order
   as NumBins, and the same order as the ones in the TMVA
   weight file). */ 
Float_t ValsL1Pt[]      = {10.0, 35.0, 75.0, 300.0};
Float_t ValsL1Eta[]     = {-1.99, 0.0, 1.99};
Float_t ValsL1StripPt[] = {0.0, 10.5, 40.0, 80.0};
Float_t ValsL1DM[]      = {0.0, 1.0, 10.0}; 
Float_t ValsL1PVDZ[]    = {-200.0, -75.0, 0.0, 75.0, 200.0};
Float_t *Vals[] = {ValsL1Pt,
		   ValsL1Eta,
		   ValsL1StripPt,
		   ValsL1DM,
		   ValsL1PVDZ};

/**************************************************************/

/* Print a std::vector vec of Float_t's to stdout. */

void printVec(std::vector<Float_t> vec)
{
  for (Int_t i = 0; i < vec.size(); i++)
    {
      printf("%f ", vec.at(i));
    }
  printf("\n");
}

/**************************************************************/

/* Search: Returns the index of array-of-floats target in the
   array-of-arrays-of-floats list, which has length size. If 
   target is not found, return -1. */

Int_t getIndexOf(Float_t* target, Float_t** list, Int_t size)
{
  for (Int_t i = 0; i < size; i++)
    {
      if (list[i] == target)
	return i; 
    } 
  return -1;
}

/**************************************************************/

/* Print the contents of table t (dimensions nRows x nCols)
   to stdout. */

void printTable(Float_t **t, Int_t nRows, Int_t nCols)
{
  for (int i = 0; i < nRows; i++)
    {
      for (int j = 0; j < nCols; j++)
	  printf("%f ", t[i][j]);
      printf("\n");
    }
}

/**************************************************************/

/* Write the contents of table t (dimensions nRows x nCols)
   to a file fName. */

void writeTable(Float_t **t, Int_t nRows, Int_t nCols, 
		const char* fName)
{
  FILE *fp;
  
  fp = fopen(fName, "w");
  
  for (int i = 0; i < nRows; i++)
    {
      for (int j = 0; j < nCols; j++)
	{
	  fprintf(fp, "%8f", t[i][j]);
	
	  /* Put a space between each column except after the
	     last column. */
	  if (j < (nCols - 1))
	    fprintf(fp, " ");
	}
      fprintf(fp, "\n");
    }

  fclose(fp);
}

/**************************************************************/

/* Returns the number of rows, where numVars is the number of
   variables and numBins[] is an array of the number of bins. */

Int_t getNumRows(Int_t numVars, Int_t numBins[])
{
  assert(numVars > 0);
  assert(piBins != NULL);

  Int_t nRows = 1;
  for (Int_t i = 0; i < numVars; i++)
    nRows *= numBins[i];

  return nRows;
}

/**************************************************************/

/* Creates and fills in 2D array containing all possible
   permutations(* see note below) of numVars variables with
   values as listed in the global variables, plus one extra
   column for the TMVA discriminant (initialized to a dummy
   value).

   Takes two arguments: numVars, the number of variables, and
   numBins[], a list of ints which is the number of bins. 

   *Note: There is an additional loop which sets L1StripPt to 0
          for events with DM 0 (1 prong) and DM 10 (3-prong), 
          because non-zero L1StripPt's are only measured for 
          DM 1 (1 prong + Pi0). */

Float_t** fillTableWithPermutedValues(Int_t numVars,
				       Int_t numBins[])
{
  /* Number of rows. */
  Int_t nRows = getNumRows(numVars, numBins);

  /* Allocate memory for the new table, adding an extra 
     column for the discriminant value. */
  Float_t** table = new Float_t*[nRows];
  for (Int_t r = 0; r < nRows; r++)
    table[r] = new Float_t[numVars + 1];

  /* Fill the table with permutations of the variables' possible
     values. */
  
  /* For each variable: */
  for (Int_t iVar = 0; iVar < numVars; iVar++)
    {
      // printf("iVar = %d\n", iVar);
      
      /* Calculate the product of the remaining variables' number
	 of bins. This is the "block size" that we need to fill
         in one step. */
      Int_t blockSize = 1;
      for (Int_t i = (iVar + 1); i < numVars; i++)
	blockSize *= numBins[i];
      
      // printf("Blocksize: %d\n", blockSize);
  
      /* Start at the top of a column: */
      Int_t iRow = 0;
      while (iRow < nRows)
	{
	  /* "bin" is the number of bins for this variable. */
	  for (Int_t iBin = 0; iBin < numBins[iVar]; iBin++)
	    {
	      // printf("numBins[iBin] is %d\n", numBins[iBin]);
	      // printf("iBin is %d\n", iBin);
	      /* Index r counts which row we are on. */
	      for (Int_t r = iRow; r < (iRow + blockSize); r++)
		{
		  /* Fill the entry. */
		  // printf("Row number: %d\n", r);
		  table[r][iVar] = Vals[iVar][iBin];
		  table[r][numVars] = -99.99; /* super inefficient, sorry! */
		}
	      iRow += blockSize;
	    }
	}

    }  /* end of loop over variables */

  /* Set L1StripPt = 0 for Decay Modes 0 or 10. */
  Int_t locStripPt = getIndexOf(ValsL1StripPt, Vals, numVars);
  Int_t locDM = getIndexOf(ValsL1DM, Vals, numVars + 1);
  //  printf("L1StripPt's index is %d\n", locStripPt);
  //  printf("L1DecayMode's index is %d\n", locDM);
  if ((locStripPt != -1) && (locDM != -1))
    {
      for (Int_t r = 0; r < nRows; r++)
	{
	  if (table[r][locDM] != 1.0)
	    table[r][locStripPt] = 0;
	}
    }

  return table;
}

/**************************************************************/

/* Takes table, a 2D array of doubles, applies TMVA's
   classification application function, and returns the 
   filled table. */

Float_t **fillTableWithTMVAdiscriminant(Float_t **table,
					Int_t numVars,
					Int_t numBins[])
{
  
  /* Default MVA methods to be applied: */
  std::map<std::string, int> Use;
  Use["BDT"] = 1;

  /* Create the Reader objct. */
  TMVA::Reader *reader = new TMVA::Reader("!Color:Silent");

  /* Create variables and declare them to the Reader. The
     variable names must correspond in name and type to those
     given in the weight file(s) used. */
  Float_t l1Pt, l1Eta, l1StripPt, l1DM, l1PVDZ;

  reader->AddVariable("l1Pt", &l1Pt);
  reader->AddVariable("l1Eta", &l1Eta);
  reader->AddVariable("l1StripPt", &l1StripPt);
  reader->AddVariable("l1DM", &l1DM);
  reader->AddVariable("l1PVDZ", &l1PVDZ);


  /* Get the number of rows in table. */
  Int_t nRows = getNumRows(numVars, numBins);
  
  /* Book the MVA methods. */
  for (std::map<std::string,int>::iterator it = Use.begin();
       it != Use.end(); it++)
    {
      if (it->second)
	{  TString methodName = it->first + " method";
	  /* Customize the directory */
	  TString weightfile = "../training/dataset/weights/TMVAClassification_"
	    + TString(it->first) + ".weights.xml";
	  reader->BookMVA(methodName, weightfile);
	}
    }
  
  for (std::map<std::string,int>::iterator it = Use.begin();
       it != Use.end(); it++)
    {
      if (!it->second) continue;
      TString methodName = it->first + " method";

      /* "Event" loop: Loop through the rows in table: */

      for (Int_t r = 0; r < nRows; r++)
	{
	  /* Put the BDT input variables in a vector. */
	  std::vector<Float_t> event;

	  for (Int_t iVar = 0; iVar < numVars; iVar++)
	    {
	      event.push_back(table[r][iVar]);
	    }
	  
	  /* Call the EvaluateMVA function. */
	  Float_t discr = -99.99;   /* placeholder value */
	  discr = reader->EvaluateMVA(event, methodName);
	  table[r][numVars] = discr;
	}
    }

  delete reader;

  return table;
}


/**************************************************************/

/* Main function. Prints a table with all possible permutations
   of input variables, along with a column containing the TMVA
   MVA output for each set of input variables.

   Returns 0 if successful. */

int makeTable(void)
{
  /* Calculate number of variables. */
  Int_t nVars = sizeof(Vals)/sizeof(Vals[0]);

  /* Create the table with permutations of variable values. */
  Float_t **table = fillTableWithPermutedValues(nVars, NumBins);
  
  /* Calculate the discriminant. */
  table = fillTableWithTMVAdiscriminant(table, nVars, NumBins);
	     
  //  printTable(table, getNumRows(nVars, NumBins), nVars + 1);

  writeTable(table, getNumRows(nVars, NumBins), nVars + 1,
	     "table.txt");

  delete(table);
  
  return 0;
}
