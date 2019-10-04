/*******************************************************************/
/* calculateRates.C                                                */
/* Helper function                                                 */
/* Based on comparisonplots.C by Isobel Ojalvo                     */
/* Author: Stephanie Kwan                                          */
/*******************************************************************/

#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TH1.h"
#include "TF1.h"
#include <math.h>
#include <TEfficiency.h>
#include <TMath.h>
#include <iostream>
#include <string>
#include <vector>

#include <cmath>

#include "../base_code_for_plots/tdrstyle.C"
#include "../base_code_for_plots/CMS_lumi.h"
#include "../base_code_for_plots/helperFunctions.C"

#ifndef CALCULATE_RATES_INCL
#define CALCULATE_RATES_INCL

/*******************************************************************/

/* Calculates and returns the rates
   of a BDT discriminant cut at
   the value bdtCut for Level 1 taus, using an TTree (ntuple) of L1
   taus pointed to by treePath and rootFileDirectory. 
   The BDT weight file is read from weightFileDirectory.
   pTCut and l1PtCut are lower bounds on the gen/reco and l1 pT
   respectively, and absEta(Lower/Upper)Bound is for gen/reco eta.
   (Whether gen/reco is used depends on whether L1 matches with gen/
   reco).
*/
   
TH1F* calculateRates(TString isoHistPath,
		     TString allHistPath,
		     TString rootFileDirectory)
{
  /* Load file */
  TFile *file = new TFile(rootFileDirectory);
  if (!file->IsOpen() || file==0 )
    {
      std::cout<<"ERROR FILE "<< rootFileDirectory <<" NOT FOUND; EXITING"<<std::endl;
      return 0;
    }
  
  TH1F* allHist = (TH1F*)file->Get(allHistPath);
  if (allHist == 0)
    {
      std::cout<<"ERROR: "<< allHistPath << " NOT FOUND; EXITING"<<std::endl;
      return 0;
    }

  TH1F* isoHist = (TH1F*)file->Get(isoHistPath);
  if (isoHist == 0)
    {
      std::cout << "ERROR: " << isoHistPath << " not found; EXITING"<<std::endl;
      return 0;
    }


  int nBins = isoHist->GetSize();

  float xMin = isoHist->GetBinLowEdge(0);
  float xMax = (isoHist->GetBinLowEdge(nBins) + isoHist->GetBinWidth(nBins));

  //  printf("nBins = %i, xMin = %f, xMax = %f\n", nBins, xMin, xMax);

  TH1F* ratesHist = new TH1F("Rates", "Rates", nBins, xMin, xMax);
  ratesHist->Sumw2();

  /* Loop through bins in the Rates histogram, in reverse order. */
  int Sum;

  for (int i = nBins; i > 0; i--)
    {
      Sum += isoHist->GetBinContent(i);
      ratesHist->SetBinContent(i, Sum);
    }

  /* Calculate (# of all events passing the BDT) / (# all events) */
  double nPass = isoHist->GetEntries();
  double nEvents = getEvents(rootFileDirectory, "L1TauAnalyzerRates");

  /* Convert each bin to a fraction of total events. */
  float firstBin = ratesHist->GetBinContent(1);
  ratesHist->Scale((double) 1.00 / firstBin);
  
  //  ratesHist->Scale(40.0 * 1000000.0);

  return ratesHist;
}

#endif

