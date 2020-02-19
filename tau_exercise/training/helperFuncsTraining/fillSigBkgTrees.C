/************************************************************/
/* fillSigBkgTrees.C                                        */
/* Author: Stephanie Kwan                                   */
/************************************************************/


#include <cstdlib>
#include <iostream>
#include <map>
#include <string>

#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TObjString.h"
#include "TSystem.h"
#include "TROOT.h"

#include "TMVA/Factory.h"
#include "TMVA/DataLoader.h"
#include "TMVA/Tools.h"
#include "TMVA/TMVAGui.h"

#include <math.h>


#ifndef MAKE_SIGNAL_TREE_C_INCL
#define MAKE_SIGNAL_TREE_C_INCL

/************************************************************/

int fillSigBkgTrees(TTree *inputTree,
		    bool getSig, bool getBkg,
		    TTree* sigTree, TTree *bkgTree)
{
  // Declare variables to read from TTree
  Double_t l1Pt, l1Eta, l1Phi;
  Double_t l1DM;
  Double_t zVTX, l1TauZ, l1PVDZ;
  Double_t l1StripPt, l1StripEta, l1StripPhi, l1StripDR;
  Double_t genPt, recoPt;
  Int_t genDM;
  Double_t l1HoE, l1EoH;
  Double_t l1ChargedIso;

  // List all variables that TMVA needs
  inputTree->SetBranchAddress("genPt", &genPt);
  inputTree->SetBranchAddress("genDM", &genDM);

  inputTree->SetBranchAddress("recoPt", &recoPt);
  inputTree->SetBranchAddress("l1Pt", &l1Pt);

  inputTree->SetBranchAddress("l1DM", &l1DM);
  inputTree->SetBranchAddress("l1TauZ", &l1TauZ);
  inputTree->SetBranchAddress("l1PVDZ", &l1PVDZ);
  
  inputTree->SetBranchAddress("l1StripPt", &l1StripPt);
  inputTree->SetBranchAddress("l1HoE", &l1HoE);
  inputTree->SetBranchAddress("l1EoH", &l1EoH);
  inputTree->SetBranchAddress("l1ChargedIso", &l1ChargedIso);

  // Loop through taus and fill sigTree // inputTree->GetEntries()
  for (Int_t i = 0; i < inputTree->GetEntries(); i++ ) {

    inputTree->GetEntry(i);

    if ( (genPt > 20) && (genDM > 9) )
      {
	if (getSig)
	  sigTree->Fill();
      }
    else if ( l1Pt > 10 )
      {
	if (getBkg)
	  bkgTree->Fill();
      }
  }

  printf("sigTree has %lli entries\n", sigTree->GetEntries());
  printf("bkgTree has %lli entries\n", bkgTree->GetEntries());

  return 0;
}

/************************************************************/

#endif
