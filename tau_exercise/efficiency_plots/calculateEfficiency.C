/*******************************************************************/
/* calculateEfficiency.C                                           */
/* Helper function                                                 */
/* Based on comparisonplots.C by Isobel Ojalvo                     */
/* Author: Stephanie Kwan                                          */
/*******************************************************************/

#include "../base_code_for_plots/CMS_lumi.h"
#include "../base_code_for_plots/tdrstyle.C"

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
   of a BDT discriminant cut at
   the value bdtCut for Level 1 taus, using an TTree (ntuple) of L1
   taus pointed to by treePath and rootFileDirectory. 
   The BDT weight file is read from weightFileDirectory.
   pTCut and l1PtCut are lower bounds on the gen/reco and l1 pT
   respectively, and absEta(Lower/Upper)Bound is for gen/reco eta.
   (Whether gen/reco is used depends on whether L1 matches with gen/
   reco).
*/
   
int calculateEfficiency(TString treePath, TString rootFileDirectory,
			TString weightFileDirectory,
			TGraphAsymmErrors* efficiencyGraph,
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

  numerator->Sumw2();
  denominator->Sumw2();

  /* Load file */
  TFile *file = new TFile(rootFileDirectory);
  if (!file->IsOpen() || file==0 )
    {
      std::cout<<"ERROR FILE "<< rootFileDirectory <<" NOT FOUND; EXITING"<<std::endl;
      return 0;
    }
  
  TTree* tree = (TTree*)file->Get(treePath);
  if (tree == 0)
    {
      std::cout<<"ERROR Tau Tree is "<< tree<<" NOT FOUND; EXITING"<<std::endl;
      return 0;
    }

  if (! ((variable == "recoPt") || (variable == "genPt")))
    {
      std::cout<<"ERROR: parameter 'variable' is not recognized; EXITING"<<std::endl;
      return 0;
    }

  if (! ((region == "barrel") || (variable == "endcap")))
    {
      std::cout<<"ERROR: parameter 'region' must be 'barrel' or 'endcap'; EXITING"<<std::endl;
      return 0;
    }

  /* Declare variables to read in */
  double recoPt, recoEta, recoPhi;
  int recoDM;
  double genPt, genEta, genPhi;
  double l1Pt, l1Eta, l1Phi;
  double l1DM;
  double l1PVDZ;
  double l1StripPt;

  /* Set branch addresses */
  tree->SetBranchAddress("recoPt",  &recoPt);
  tree->SetBranchAddress("recoEta", &recoEta);
  tree->SetBranchAddress("recoPhi", &recoPhi);

  tree->SetBranchAddress("recoDM", &recoDM);

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

      /* deltaR matching between L1 and Reco, and L1 and Gen, is already performed in the analyzer: not needed here */

      bool passesOverallCut;
      bool passesEta;

      if (region == "barrel")
	passesEta =(genEta < 0.774);
      else if (region == "endcap")
	passesEta = (genEta > 0.774);

      if (variable == "genPt")
	{
	  passesOverallCut = ((genPt > genPtCut) && 
			      passesEta
			      // && (recoDM == 10)
			      );
	}
      else if (variable == "recoPt")
	{
	  passesOverallCut = ((recoPt > recoPtCut) &&
			      (genPt > genPtCut) &&
			      passesEta
			      // && (recoDM == 10)
			      );
	}

      /* Fill denominator */
      if (passesOverallCut)
	{
	  if (variable == "genPt")
	    denominator->Fill(genPt);
	  else if (variable == "recoPt")
	    denominator->Fill(recoPt);

	  float bdtDiscriminant = -9.9;
	  // Evaluate the BDT.
	  std::vector<float> event;
	  event.push_back(l1Pt); 
	  event.push_back(l1Eta);
	  event.push_back(l1StripPt);
	  event.push_back(l1DM);
	  event.push_back(l1PVDZ);

	  bdtDiscriminant = reader->EvaluateMVA(event, "BDT method");

	  /* Fill numerator */
	  if ((l1Pt > l1PtCut) && (bdtDiscriminant > bdtDiscriminantMin))
	    {
	      if (variable == "genPt")
		numerator->Fill(genPt);
	      else if (variable == "recoPt")
		numerator->Fill(recoPt);

	    }
	}
	  
    } /* end of loop over TTree */


  efficiencyHist->Divide(numerator, denominator);

  for (int i = 0; i < nBins; i++)
    {
      printf("efficiencyHist bin %d content is %f, with error %f\n", i, efficiencyHist->GetBinContent(i),
	     efficiencyHist->GetBinError(i));
      
    }

  

  effHistToTGraph(efficiencyHist, efficiencyGraph);
  return 0;
}

/*******************************************************************/

/* Convert an efficiency TH1F to a TGraphAsymmErrors. */

void effHistToTGraph(TH1F* effHist, TGraphAsymmErrors* effGraph)
{
  /* Loop through effHist. */
  int nBins = effHist->GetNbinsX();

  /* Efficiency cannot be above 1.0 */
  float maxEff = 1.000000;
  float minEff = 0.000000;
  

  *effGraph = TGraphAsymmErrors(effHist);

  float x, y;
  float yErrOld, yErrNewLow, yErrNewHigh;

  for (int i = 1; i < nBins; i++)
    {
      x = effHist->GetBinCenter(i);
      y = effHist->GetBinContent(i);
      yErrOld = effHist->GetBinError(i);

      effGraph->SetPoint(i, x, y);
      effGraph->SetPointEXhigh(i, 0.0);
      effGraph->SetPointEXlow(i, 0.0);
    }

  setMaxErrorTo1(effGraph);
  
}


/*******************************************************************/

void setMaxErrorTo1(TGraphAsymmErrors *graph)
{
  for (int i = 1; i < graph->GetN(); i++)
    {
      Double_t errorY = graph->GetErrorY(i);
      Double_t pointX, pointY;

      if (graph->GetPoint(i, pointX, pointY) < 0)
	printf("Error getting point\n");
      
      Double_t errorUp = pointY + errorY;
      Double_t errorLow = pointY - errorY;
      
      if (errorUp > 1)
	graph->SetPointEYhigh(i, 1 - pointY);
      else if (errorLow < 0)
	graph->SetPointEYlow(i, pointY);
      
    } 

  graph->Print();  
}



/*******************************************************************/

#endif
