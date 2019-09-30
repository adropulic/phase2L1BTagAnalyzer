/*******************************************************************/
/* calculateEfficiency.C                                           */
/* Helper function                                                 */
/* Based on comparisonplots.C by Isobel Ojalvo                     */
/* Author: Stephanie Kwan                                          */
/*******************************************************************/

#include "../base_code_for_plots/CMS_lumi.h"
#include "../base_code_for_plots/tdrstyle.C"

#include "efficiencyHist.C"

#include "TAxis.h"
#include "TChain.h"
#include "TF1.h"
#include "TFile.h"
#include "TGaxis.h"
#include "TGraphAsymmErrors.h"
#include "TH1.h"
#include "TH1F.h"

#include "THStack.h"
#include "TLegend.h"
#include "TPad.h"
#include "TPaveText.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TTree.h"

#include <cmath>
#include <iostream>
#include <math.h>
#include <string>
#include <TMath.h>
#include <vector>

#include "DataFormats/Math/interface/deltaR.h"

/* TMVA */
#include "TMVA/Tools.h"
#include "TMVA/Reader.h"
#include "TMVA/MethodCuts.h"



#ifndef CALCULATE_EFFICIENCY_INCL
#define CALCULATE_EFFICIENCY_INCL

/*******************************************************************/

/* Helper function declarations */

void effHistToTGraph(TH1F* effHist, TGraphAsymmErrors* effGraph);
void setMaxErrorTo1(TGraphAsymmErrors *graph);

/*******************************************************************/

/* Calculates and returns the efficiency and statistical uncertainty
   (as a TGraphAsymmErrors with range [xMin, xMax]) using a BDT discriminant cut at
   the value bdtDiscriminantMin, using an TTree (ntuple) of L1
   taus pointed to by treePath and rootFileDirectory. 
   The BDT weight file is read from weightFileDirectory.
   nBins is the number of bins, variable is "genPt", "recoPt", or "recoEta"
   (x-axis), recoPtCut and genPtCut specify lower-bound cuts on recoPt and genPt,
   and l1PtCut and bdtDiscriminantMin are lower-bound cuts on the numerator.
*/
   
TGraphAsymmErrors* calculateEfficiency(TString treePath, TString rootFileDirectory,
				       TString weightFileDirectory,
				       int nBins,
				       TString variable,
				       TString region,
				       double xMin,
				       double xMax,
				       double recoPtCut,
				       double genPtCut,
				       double l1PtCut,
				       double bdtDiscriminantMin)
{
  /* Numerator and denominator histograms. */
  TH1F* numerator = new TH1F("numerator", "numerator", nBins, xMin, xMax);
  TH1F* denominator = new TH1F("denominator", "denominator", nBins, xMin, xMax);
  TH1F* efficiencyHist = new TH1F("efficiency", "efficiency", nBins, xMin, xMax);

  /* Use Sumw2 method to calculate uncertainties. */
  numerator->Sumw2();
  denominator->Sumw2();

  /* Load file */
  TFile *file = new TFile(rootFileDirectory);
  if (!file->IsOpen() || file==0 )
    {
      std::cout<<"ERROR FILE "<< rootFileDirectory <<" NOT FOUND; EXITING"<<std::endl;
      return NULL;
    }
  
  TTree* tree = (TTree*)file->Get(treePath);
  if (tree == 0)
    {
      std::cout<<"ERROR Tau Tree is "<< tree<<" NOT FOUND; EXITING"<<std::endl;
      return NULL;
    }

  if (! ((variable == "recoPt") || (variable == "genPt") || (variable == "recoEta")))
    {
      std::cout<<"ERROR: parameter 'variable' must be 'recoPt', 'genPt', or 'recoEta'; EXITING"<<std::endl;
      return NULL;
    }

  if (! ((region == "barrel") || (variable == "endcap")))
    {
      std::cout<<"ERROR: parameter 'region' must be 'barrel' or 'endcap'; EXITING"<<std::endl;
      return NULL;
    }

  /* Declare variables to read in */
  double recoPt, recoEta, recoPhi;
  int recoDM;
  int genDM;
  double genPt, genEta, genPhi;
  double l1Pt, l1Eta, l1Phi;
  double l1DM;
  double l1PVDZ;
  double l1StripPt;
  double l1HoE, l1EoH;

  double l1Track_pt;

  /* Set branch addresses */
  tree->SetBranchAddress("recoPt",  &recoPt);
  tree->SetBranchAddress("recoEta", &recoEta);
  tree->SetBranchAddress("recoPhi", &recoPhi);

  tree->SetBranchAddress("recoDM", &recoDM);

  tree->SetBranchAddress("genDM", &genDM);
  tree->SetBranchAddress("genPt",  &genPt);
  tree->SetBranchAddress("genEta", &genEta);
  tree->SetBranchAddress("genPhi", &genPhi);

  tree->SetBranchAddress("l1Pt",  &l1Pt);
  tree->SetBranchAddress("l1Eta", &l1Eta);
  tree->SetBranchAddress("l1Phi", &l1Phi);

  tree->SetBranchAddress("l1DM", &l1DM);
  tree->SetBranchAddress("l1PVDZ", &l1PVDZ);
  tree->SetBranchAddress("l1StripPt", &l1StripPt);

  tree->SetBranchAddress("l1HoE", &l1HoE);
  tree->SetBranchAddress("l1EoH", &l1EoH);

  tree->SetBranchAddress("l1Track_pt", &l1Track_pt);

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
  Float_t l1HoE_f     = 0;
  Float_t l1EoH_f     = 0;
  reader->TMVA::Reader::AddVariable("l1Pt", &l1Pt_f);
  reader->TMVA::Reader::AddVariable("l1Eta", &l1Eta_f);
  reader->TMVA::Reader::AddVariable("l1StripPt", &l1StripPt_f);
  reader->TMVA::Reader::AddVariable("l1DM", &l1DM_f);
  reader->TMVA::Reader::AddVariable("l1PVDZ", &l1PVDZ_f);
  reader->TMVA::Reader::AddVariable("l1HoE", &l1HoE_f);
  reader->TMVA::Reader::AddVariable("l1EoH", &l1EoH_f);

  /* Book the MVA methods. */
  TString methodName = "BDT method";
  reader->TMVA::Reader::BookMVA(methodName, weightFileDirectory);

  /* Loop through TTree. */
  for (int i = 0; i < tree->GetEntries(); i++)
    {
      tree->GetEntry(i);

      /* deltaR matching between L1 and Reco, and L1 and Gen, is already performed in the analyzer: not needed here */

      bool passesOverallCut;
      bool passesEta;

      if (variable == "genPt")
	{
	  if (region == "barrel")
	    passesEta = (abs(genEta) < 1.44);
	  else if (region == "endcap")
	    passesEta = (abs(genEta) > 1.44);

	  passesOverallCut = ((genPt > genPtCut)
			      && passesEta
			      && (genDM > 9)
			      && (l1Track_pt > 10)
			      // && (recoDM == 0)
			      );
	}
      else if (variable == "recoPt")
	{
	  if (region == "barrel")
            passesEta = (abs(recoEta) < 1.44);
          else if (region == "endcap")
            passesEta = (abs(recoEta) > 1.44);

	  passesOverallCut = ((recoPt > recoPtCut)
			      && (genPt > genPtCut)
			      && passesEta
			      && (genDM > 9)
			      && (l1Track_pt > 10)
			      // && (recoDM == 0)
			      );
	}
      else if (variable == "recoEta")
	{
	  passesOverallCut = ((recoPt > recoPtCut)
                              && (genPt > genPtCut)
                              && passesEta
                              && (genDM > 9)
                              && (l1Track_pt > 10)
			      );

	}

      /* Fill denominator */
      if (passesOverallCut)
	{
	  if (variable == "genPt")
	    denominator->Fill(genPt);
	  else if (variable == "recoPt")
	    denominator->Fill(recoPt);
	  else if (variable == "recoEta")
	    denominator->Fill(recoEta);

	  // Evaluate the BDT.
	  float bdtDiscriminant;

	  std::vector<float> event;
	  event.push_back(l1Pt); 
	  event.push_back(l1Eta);
	  event.push_back(l1StripPt);
	  event.push_back(l1DM);
	  event.push_back(l1PVDZ);
	  event.push_back(l1HoE);
	  event.push_back(l1EoH);

	  bdtDiscriminant = reader->EvaluateMVA(event, "BDT method");
	  //	  printf("bdtDiscriminant is %f\n", bdtDiscriminant);

	  /* Fill numerator */
	  if ((l1Pt > l1PtCut) && (bdtDiscriminant > bdtDiscriminantMin))
	    {
	      if (variable == "genPt")
		numerator->Fill(genPt);
	      else if (variable == "recoPt")
		numerator->Fill(recoPt);
	      else if (variable == "recoEta")
		numerator->Fill(recoEta);

	    }
	}
	  
    } /* end of loop over TTree */


  efficiencyHist->Divide(numerator, denominator);

  TGraphAsymmErrors* effGraph = new TGraphAsymmErrors();
  *effGraph = TGraphAsymmErrors(numerator, denominator);
  
  setMaxErrorTo1(effGraph);

  return effGraph;
}

/*******************************************************************/

#endif
