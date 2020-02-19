/*********************************************************************/
/* extractROC.C                                                      */
/* Author: Stephanie Kwan                                            */
/* Usage: root -l extractROC.C                                       */
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

void plotROC(TH1D* hist, TString outputDirectory);

/*********************************************************************/

/* This macro requires the output file of a ROOT TMVA training with
  the BDT method. It saves the ROC curve to an output file. */

int extractROC()
{
  TString inputDirectory = "../training/TMVA_training_taus_out_combined_DYToLL-noPU_GluGluHiggsToTauTau-200PU.root";
  TFile *file = new TFile(inputDirectory);
  TString outputDirectory = "plots/exampleROC.png";

  if(!file->IsOpen() || file==0)
    {
      std::cout<<"ERROR: FILE "<< inputDirectory <<" NOT FOUND; EXITING"<<std::endl;
      return 0;
    }

  TH1D* rejBvsS = (TH1D*)file->Get("dataset/Method_BDT/BDT/MVA_BDT_rejBvsS");
  int nBins = rejBvsS->GetNbinsX();

  TH1D* roc = new TH1D("ROC", "ROC curve (TPR vs. FPR)", nBins, 0.0, 1.0);

  /* rejBvsS (what ROOT automatically outputs) is a plot of background rejection
     (1 - background efficiency) on the y-axis, vs. signal efficiency on the
     x-axis. 

     A ROC curve is a plot of TPR (True Positive Rate) vs. FPR (False Positive
     Rate). TPR is another name for the signal efficiency, and FPR is
     (1 - True Negative Rate) = (1 - Background efficiency). 

     So we loop over each bin in the rejBvsS histogram, and fill the ROC curve
     histogram with the x-axis inverted. */

  for (int i = 0; i < nBins; i++)
    {
      /* Get the background rejection */
      double oldX = rejBvsS->GetBinCenter(i);

      /* Get the signal efficiency */
      double oldY = rejBvsS->GetBinContent(i);

      /* False Positive Rate (fpr) = 1 - True Negative Rate (background efficiency) */
      double newX = 1.0 - (double) oldX;
      
      /* Get the new bin number */
      int newXbin = roc->GetXaxis()->FindBin(newX);

      /* Fill the ROC histogram */
      roc->SetBinContent(newXbin, oldY);

    }

  plotROC(roc, outputDirectory);

  return 0;
}

/*********************************************************************/

/* Plot a ROC curve and save it to a file. */

void plotROC(TH1D* hist, TString outputDirectory)
{
  setTDRStyle();

  TCanvas* Tcan = new TCanvas("Tcan","", 100, 20, 800, 800);
  TLegend* leg = new TLegend(0.55, 0.2, 0.9, 0.5);
  TLine* diagonal = new TLine(0.0, 0.0, 1.0, 1.0);

  Tcan->SetGrid(); // Draw a grid
  
  Tcan->cd();     // Set current canvas
  Tcan->SetFillColor(0);
  applyLegStyle(leg);

  hist->Draw("");

  hist->GetXaxis()->SetTitle("False Positive Rate");
  hist->GetYaxis()->SetTitle("True Positive Rate");
  hist->GetXaxis()->SetTitleSize(0.06);
  hist->GetYaxis()->SetTitleSize(0.06);

  hist->SetAxisRange(0.0, 1.0, "Y"); 

  hist->SetLineWidth(2);

  /* Customize legend */
  leg->SetHeader("Phase 2 L1 Taus");
  leg->AddEntry(hist, "BDT", "l");
  gStyle->SetLegendFont(60);
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
