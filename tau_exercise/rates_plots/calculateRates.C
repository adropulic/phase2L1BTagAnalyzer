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

/* isoHistPath: Path to histogram containing the objects passing the
                desired trigger working point. e.g. 
		"L1TauAnalyzerRates/l1TauIsoTight_pt" contains the
                pTs of the taus passing the Tight isolation MVA
		requirement.
   allHistPath: Path to histogram containing all the objects regardless
                of whether they pass the trigger.
                e.g. "L1TauAnalyzer/l1Tau_pt" 
   rootFileDirectory: Directory to the ROOT file. 
   Returns a TH1F of the rate assuming the HL-LHC 40 MHz event rate. */
   
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

  TH1F* ratesHist = new TH1F("Rates", "Rates", nBins+2, xMin, xMax);
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
  
  /* kHz */
  ratesHist->Scale(40.0 * 1000000.0 / 1000.0);

  return ratesHist;
}

#endif

