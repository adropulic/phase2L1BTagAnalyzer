/*********************************************************************/
/* bkgRejVsSigEff.C                                                  */
/* Author: Stephanie Kwan                                            */
/* Usage: root -l bkgRejVsSigEff.C                                   */
/*********************************************************************/

#include "../base_code_for_plots/CMS_lumi.h"
#include "../base_code_for_plots/tdrstyle.C"

#include "TAxis.h"
#include "TFile.h"
#include "TGaxis.h"
#include "TH1.h"
#include "TLine.h"

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

void plotHist(TH1D* hist, TString outputDirectory);

/*********************************************************************/

/* This macro requires the output file of a ROOT TMVA training with
  the BDT method. It saves the ROC curve to an output file. */

int bkgRejVsSigEff()
{
  TString inputDirectory = "../training/TMVA_training_out.root";
  TFile *file = new TFile(inputDirectory);
  TString outputDirectory = "plots/bkgRejVsSigEff.png";

  if(!file->IsOpen() || file==0)
    {
      std::cout<<"ERROR: FILE "<< inputDirectory <<" NOT FOUND; EXITING"<<std::endl;
      return 1;
    }

  TH1D* rejBvsS = (TH1D*)file->Get("dataset/Method_BDT/BDT/MVA_BDT_rejBvsS");

  plotHist(rejBvsS, outputDirectory);

  return 0;
}

/*********************************************************************/

/* Plot the curve and save it to a file. */

void plotHist(TH1D* hist, TString outputDirectory)
{
  setTDRStyle();

  TCanvas* Tcan = new TCanvas("Tcan","", 100, 20, 800, 800);
  TLegend* leg = new TLegend(0.20, 0.20, 0.5, 0.5);
  TLine* diagonal = new TLine(0.0, 1.0, 1.0, 0.0);

  Tcan->SetGrid(); // Draw a grid
  
  Tcan->cd();     // Set current canvas
  Tcan->SetFillColor(0);
  applyLegStyle(leg);

  hist->Draw("");

  hist->GetXaxis()->SetTitle("Signal efficiency");
  hist->GetYaxis()->SetTitle("Background rejection rate (1 - background efficiency)");
  hist->GetXaxis()->SetTitleSize(0.04);
  hist->GetYaxis()->SetTitleSize(0.04);

  hist->SetAxisRange(0.0, 1.0, "Y"); 

  hist->SetLineWidth(2);

  /* Customize legend */
  leg->SetHeader("Phase 2 L1 Taus");
  leg->AddEntry(hist, "BDT", "l");
  gStyle->SetLegendFont(100);
  leg->Draw();

  /* Customize the diagonal line */
  diagonal->SetLineColor(12); // dark grey
  diagonal->SetLineWidth(2);
  diagonal->SetLineStyle(9); // dashed
  diagonal->Draw();

  Tcan->cd();
  Tcan->SaveAs(outputDirectory);
}

/*********************************************************************/
