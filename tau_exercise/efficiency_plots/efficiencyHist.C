/*******************************************************************/
/* efficiencyHist.C                                                */
/* Author: Stephanie Kwan                                          */
/*******************************************************************/

#include "TGraphAsymmErrors.h"
#include "TH1.h"
#include "TH1F.h"

#include "TAxis.h"
#include "TChain.h"

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


#include "../base_code_for_plots/CMS_lumi.h"
#include "../base_code_for_plots/tdrstyle.C"

#ifndef EFFICIENCY_HIST_INCL
#define EFFICIENCY_HIST_INCL

/*******************************************************************/

/* Sets the maximum and minimum error of an efficiency
   TGraphAsymmErrors to be 1.0 and 0.0 respectively. */

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
}

/*******************************************************************/

/* Plots loose/medium/tight efficiency histograms and saves to a .png. */

void plotHists(TGraphAsymmErrors* histLoose,
               TGraphAsymmErrors* histMedium,
               TGraphAsymmErrors* histMedium2,
               TGraphAsymmErrors* histTight,
               TGraphAsymmErrors* histTight2,
               TGraphAsymmErrors* histNoBDT,
	       TString xAxisLabel,
	       TString legendName,
	       TString outputName,
	       TString outputDir
	       )
{
  /*******************************************************/
  /* plotting                                            */
  /*******************************************************/
  
  setTDRStyle();
  TCanvas* Tcan = new TCanvas("Tcan","", 100, 20, 1000, 800);
  TLegend* leg = new TLegend(0.60,0.75,0.85,0.9);
  Tcan->SetGrid();

  Tcan->cd();     /* Set current canvas */
  Tcan->SetFillColor(0);
  leg = new TLegend(0.55, 0.2, 0.90, 0.5);
  applyLegStyle(leg);

  /* Loose */
  histLoose->SetMarkerColor(kViolet+9);
  histLoose->SetMarkerStyle(kFullCircle);
  histLoose->SetLineWidth(2);
  histLoose->SetLineColor(kViolet+9);

  /* Medium */
  histMedium->SetMarkerColor(kViolet-5);
  histMedium->SetMarkerStyle(kFullCircle);
  histMedium->SetLineWidth(2);
  histMedium->SetLineColor(kViolet-5);

  /* Medium 2*/
  histMedium2->SetMarkerColor(kPink+5);
  histMedium2->SetMarkerStyle(kFullCircle);
  histMedium2->SetLineWidth(2);
  histMedium2->SetLineColor(kPink+5);

  /* Tight */
  histTight->SetMarkerColor(kOrange+2);
  histTight->SetMarkerStyle(kFullCircle);
  histTight->SetLineWidth(2);
  histTight->SetLineColor(kOrange+2);

  /* Tight2  */
  histTight2->SetMarkerColor(kRed-4);
  histTight2->SetMarkerStyle(kFullCircle);
  histTight2->SetLineWidth(2);
  histTight2->SetLineColor(kRed-4);

  /* No BDT */
  histNoBDT->SetMarkerColor(kGray+1);
  histNoBDT->SetMarkerStyle(kFullCircle);
  histNoBDT->SetLineWidth(2);
  histNoBDT->SetLineColor(kGray+1);

  /* Set y-axis limits */
  histLoose->GetYaxis()->SetRangeUser(0.0, 1.1);

  histLoose->Draw("");
  histMedium->Draw("SAME");
  histMedium2->Draw("SAME");
  histTight->Draw("SAME");
  histTight2->Draw("SAME");
  histNoBDT->Draw("SAME");

  histLoose->GetXaxis()->SetTitle(xAxisLabel);
  histLoose->GetYaxis()->SetTitle("L1 Efficiency");
  histLoose->GetXaxis()->SetTitleSize(0.06); // default is 0.03                                                                                                     
  /* Customize legend */
  leg->SetHeader(legendName);
  leg->AddEntry(histLoose, "60%", "l");
  leg->AddEntry(histMedium, "70%", "l");
  leg->AddEntry(histMedium2, "80%", "l");
  leg->AddEntry(histTight, "90%", "l");
  leg->AddEntry(histTight2, "95%", "l");
  leg->AddEntry(histNoBDT, "No BDT", "l");

  gStyle->SetLegendFont(30);
  leg->Draw();

  Tcan->cd();
  Tcan->SaveAs(outputDir+outputName);


}



/*******************************************************************/

#endif 
