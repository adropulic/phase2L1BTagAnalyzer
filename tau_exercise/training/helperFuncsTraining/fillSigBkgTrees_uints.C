/************************************************************/
/* fillSigBkgTrees_uints.C                                  */
/* Author: Stephanie Kwan                                   */
/************************************************************/


#include <cstdlib>
#include <cstdint>
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


#ifndef FILL_SIG_BKG_TREES_UINTS_C_INCL
#define FILL_SIG_BKG_TREES_UINTS_C_INCL

/************************************************************/

int fillSigBkgTrees_uints(TTree *inputTree,
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

  ULong64_t l1Pt_u, l1Eta_u;
  /*  ULong64_t l1DM_u;
  uint64_t l1TauZ_u, l1PVDZ_u;
  uint64_t l1StripPt_u;
  uint64_t l1HoE_u, l1EoH_u, l1ChargedIso_u;*/

  // List all variables that TMVA needs
  inputTree->SetBranchAddress("genPt", &genPt);
  inputTree->SetBranchAddress("genDM", &genDM);

  inputTree->SetBranchAddress("recoPt", &recoPt);
  inputTree->SetBranchAddress("l1Pt", &l1Pt);
  
  inputTree->SetBranchAddress("l1Eta", &l1Eta);

  inputTree->SetBranchAddress("l1DM", &l1DM);
  inputTree->SetBranchAddress("l1TauZ", &l1TauZ);
  inputTree->SetBranchAddress("l1PVDZ", &l1PVDZ);
  
  inputTree->SetBranchAddress("l1StripPt", &l1StripPt);
  inputTree->SetBranchAddress("l1HoE", &l1HoE);
  inputTree->SetBranchAddress("l1EoH", &l1EoH);
  inputTree->SetBranchAddress("l1ChargedIso", &l1ChargedIso);

  // Set branch addresses in signal and background trees
  sigTree->SetBranchAddress("l1Pt_u", &l1Pt_u);
  sigTree->SetBranchAddress("l1Eta_u", &l1Eta_u);

  bkgTree->SetBranchAddress("l1Pt_u", &l1Pt_u);
  bkgTree->SetBranchAddress("l1Eta_u", &l1Eta_u);

  // Loop through taus and fill trees
  //  for (Int_t i = 0; i < inputTree->GetEntries(); i++ ) {
  for (Int_t i = 0; i < 30; i++) {
    inputTree->GetEntry(i);

    if ( (genPt > 20) && (genDM > 9) )
      {
	if (getSig)
	  {
	    l1Pt_u = ((ULong64_t) l1Pt) & 0x3FFFF;
	    l1Eta_u = ((ULong64_t) ((l1Eta + 10.00) / 0.01)) & 0x3FFFF; // need to fix this
	    printf("l1Pt is %f, l1Pt_u is %llu \n", l1Pt, l1Pt_u);
	    printf("**** l1Eta is %f, l1Eta_u is %llu \n", l1Eta, l1Eta_u);
	    sigTree->Fill();


	  }
      }
    else if ( l1Pt > 10 )
      {
	if (getBkg)
	  {

	    l1Pt_u = ((ULong64_t) l1Pt) & 0x3FFFF;
            l1Eta_u = ((ULong64_t) ((l1Eta + 10.00) / 0.01)) & 0x3FFFF; // need to fix this 
	    bkgTree->Fill();

	  }
      }
  }

  printf("sigTree has %lli entries\n", sigTree->GetEntries());
  printf("bkgTree has %lli entries\n", bkgTree->GetEntries());

  return 0;
}

/************************************************************/

#endif
