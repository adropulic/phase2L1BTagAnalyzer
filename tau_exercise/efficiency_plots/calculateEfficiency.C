/*******************************************************************/
/* calculateEfficiency.C                                           */
/* Helper function                                                 */
/* Based on comparisonplots.C by Isobel Ojalvo                     */
/* Author: Stephanie Kwan                                          */
/*******************************************************************/

#include "TCanvas.h"
#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TH1.h"
#include "TF1.h"
#include <math.h>
#include <TEfficiency.h>
#include <TMath.h>
#include <TFormula.h>
#include <iostream>
#include <string>
#include <vector>


#include <iostream>
#include <cmath>
#include "TLegend.h"
#include "TPad.h"
#include "TH1.h"
#include "TH1F.h"
#include "TH2.h"
#include "TF1.h"
#include "THStack.h"
#include "TStyle.h"
#include "TAxis.h"
#include "TGaxis.h"
#include "TTree.h"
#include "TPaveText.h"
#include "tdrstyle.C"
#include "TStyle.h"
#include "TAxis.h"
#include "TGaxis.h"
#include "CMS_lumi.h"

#include "DataFormats/Math/interface/deltaR.h"

/* TMVA */
#include "TMVA/Tools.h"
#include "TMVA/Reader.h"
#include "TMVA/MethodCuts.h"

#ifndef CALCULATE_EFFICIENCY_INCL
#define CALCULATE_EFFICIENCY_INCL

/*******************************************************************/

/* Applies template style to a TPad* pad1. */
void applyPadStyle(TPad* pad1)
{
  pad1->SetFillColor(0);
  pad1->Draw();
  pad1->cd();
  pad1->SetLeftMargin(0.2);  pad1->SetBottomMargin(0.13); pad1->SetRightMargin(0.1);
  //pad1->SetGrid(); 
  pad1->SetGrid(10,10); 
}

/*******************************************************************/

/* Apply legend style to a TLegend *leg. */

void applyLegStyle(TLegend *leg){
  leg->SetFillColor(0);
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);
  leg->SetHeader("");
  leg->SetShadowColor(0);
}

/*******************************************************************/

/* Returns the uncertainty of the fraction x/y, given
   x and y and their respectively uncertainties deltaX and deltaY. */

float uncertaintyDivXY(float x, float deltaX, 
		       float y, float deltaY)
{
  float z, deltaZ;

  z = x/y;
  deltaZ = abs(z) * sqrt(pow(deltaX/x, 2) + pow(deltaY/y, 2));
  return deltaZ;
}

/*******************************************************************/

/* Calculates and returns the efficiency and statistical uncertainty
   of a BDT discriminant cut at
   the value bdtCut for Level 1 taus, using an TTree (ntuple) of L1
   taus pointed to by treePath and rootFileDirectory. 
   The BDT weight file is read from weightFileDirectory.
   pTCut and l1PtCut are lower bounds on the gen/reco and l1 pT
   respectively, and absEta(Lower/Upper)Bound is for gen/reco eta.
   (Whether gen/reco is used depends on whether L1 matches with gen/
   reco).
*/
   
std::vector<float> calculateEfficiency(TString treePath, TString rootFileDirectory,
			   TString weightFileDirectory,
			   double pTCut,
			   double l1PtCut,
			   double absEtaLowerBound,
			   double absEtaUpperBound,
			   double bdtCut)
{
  std::vector<float> effVec;
  
  /* Load file */
  TFile *file = new TFile(rootFileDirectory);
  if (!file->IsOpen() || file==0 )
    {
      std::cout<<"ERROR FILE "<< rootFileDirectory <<" NOT FOUND; EXITING"<<std::endl;
      return effVec;
    }
  
  TTree* tree = (TTree*)file->Get(treePath);
  if (tree == 0)
    {
      std::cout<<"ERROR Tau Tree is "<< tree<<" NOT FOUND; EXITING"<<std::endl;
      return effVec;
    }

  int numerator = 0;
  int denominator = 0;
  
  /* Declare variables to read in */
  double recoPt, recoEta, recoPhi;
  double genPt, genEta, genPhi;
  double l1Pt, l1Eta, l1Phi;
  double l1DM;
  double l1PVDZ;
  double l1StripPt;

  /* Set branch addresses */
  tree->SetBranchAddress("recoPt",  &recoPt);
  tree->SetBranchAddress("recoEta", &recoEta);
  tree->SetBranchAddress("recoPhi", &recoPhi);
  tree->SetBranchAddress("genPt",  &genPt);
  tree->SetBranchAddress("genEta", &genEta);
  tree->SetBranchAddress("genPhi", &genPhi);
  tree->SetBranchAddress("l1Pt",  &l1Pt);
  tree->SetBranchAddress("l1Eta", &l1Eta);
  tree->SetBranchAddress("l1Phi", &l1Phi);

  tree->SetBranchAddress("l1DM", &l1DM);
  tree->SetBranchAddress("l1PVDZ", &l1PVDZ);
  tree->SetBranchAddress("l1StripPt", &l1StripPt);

  /* Load the TMVA library. */
  TMVA::Tools::Instance();

  /* Create the Reader object. */
  TMVA::Reader *reader = new TMVA::Reader("!Color:Silent");

  /* Add variables to the Reader (must be the same name and type as the variables in the weight file(s) used. */
  Float_t l1Pt_f      = 0;
  Float_t l1Eta_f     = -10;
  Float_t l1StripPt_f = 0;
  Float_t l1DM_f      = -10;
  Float_t l1PVDZ_f    = 0;
  reader->TMVA::Reader::AddVariable("l1Pt", &l1Pt_f);
  reader->TMVA::Reader::AddVariable("l1Eta", &l1Eta_f);
  reader->TMVA::Reader::AddVariable("l1StripPt", &l1StripPt_f);
  reader->TMVA::Reader::AddVariable("l1DM", &l1DM_f);
  reader->TMVA::Reader::AddVariable("l1PVDZ", &l1PVDZ_f);

  /* Book the MVA methods. */
  TString methodName = "BDT method";
  reader->TMVA::Reader::BookMVA(methodName, weightFileDirectory);


  /* Loop through TTree. */
  for (int i = 0; i < tree->GetEntries(); i++)
    {
      tree->GetEntry(i);

      bool isGenMatched  = (abs(reco::deltaR(l1Eta, l1Phi, genEta, genPhi)) < 0.5);
      bool isRecoMatched = (abs(reco::deltaR(l1Eta, l1Phi, recoEta, recoPhi)) < 0.5);

      if ( (isGenMatched && (genPt > pTCut)   && (abs(genEta) > absEtaLowerBound)  && (abs(genEta) < absEtaUpperBound))
	   ||
	   (isRecoMatched && (recoPt > pTCut) && (abs(recoEta) > absEtaLowerBound) && (abs(recoEta) < absEtaUpperBound )) )
	{
	  denominator++;

	  
	  float bdtDiscriminant = 0.0;
	  
	  /* Evaluate the BDT. */
	  std::vector<float> event;
	  event.push_back(l1Pt); 
	  event.push_back(l1Eta);
	  event.push_back(l1StripPt);
	  event.push_back(l1DM);
	  event.push_back(l1PVDZ);
	  bdtDiscriminant = reader->EvaluateMVA(event, "BDT method");

	  if ((l1Pt > l1PtCut) && (bdtDiscriminant > bdtCut))
	    {
	      numerator++;
	    }
	}
	  

    } /* end of loop over TTree */

  /* float uncertaintyDivXY(float x, float deltaX,
                            float y, float deltaY) */
  if ((numerator > 0) && (denominator > 0))
    {
      effVec.push_back((float) numerator/denominator);
      effVec.push_back(uncertaintyDivXY(numerator, sqrt(numerator),
					denominator, sqrt(denominator)));
    }
  else
    {
      effVec.push_back(0);
      effVec.push_back(0);
    }
    
  return effVec;
}

#endif
