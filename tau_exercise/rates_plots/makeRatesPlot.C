/*******************************************************************/
/* makeRatesPlot.C                                                 */
/* ROOT macro                                                      */
/* Usage: root -l -b -q makeRatesPlot.C                            */
/*******************************************************************/

#include "calculateRates.C"

#include "../base_code_for_plots/CMS_lumi.h"
#include "../base_code_for_plots/tdrstyle.C"

void plotHists(TH1F* h1, TString h1Label, 
	       TH1F* h2, TString h2Label,
	       TH1F* h3, TString h3Label,
	       TString filename,
               TString outputDir);

void plotSixHists(TH1F* h1, TString h1Label,
                  TH1F* h2, TString h2Label,
                  TH1F* h3, TString h3Label,
                  TH1F* h4, TString h4Label,
                  TH1F* h5, TString h5Label,
                  TH1F* h6, TString h6Label,
                  TString filename,
                  TString outputDir);

/*********************************************************************/

/* Plots rates (x-axis: L1 Tau Pt). */

void makeRatesPlot(void)
{
  gROOT->ProcessLine(".L calculateRates.C");

  /* Specify paths/directories for input files. */
  TString l1PtPath = "L1TauAnalyzerRates/l1Tau_pt";
  TString rootFileDirectory = "../ntuples/use_charghadr_newAlgo/SingleNeutrino-140PU-2019_Oct1-partial.root";
  TString outputDirectory = "plots/";

  /*******************************************************/
  /* Rates as a function of l1Pt                         */
  /*******************************************************/

  TH1F* tauRates60    = calculateRates("L1TauAnalyzerRates/l1TauIsoTightV0_pt",l1PtPath, rootFileDirectory);
  TH1F* tauRates70    = calculateRates("L1TauAnalyzerRates/l1TauIsoTight_pt",  l1PtPath, rootFileDirectory);
  TH1F* tauRates80    = calculateRates("L1TauAnalyzerRates/l1TauIsoMedium_pt", l1PtPath, rootFileDirectory);
  TH1F* tauRates90    = calculateRates("L1TauAnalyzerRates/l1TauIsoLoose_pt",  l1PtPath, rootFileDirectory);
  TH1F* tauRates95    = calculateRates("L1TauAnalyzerRates/l1TauIsoVLoose_pt", l1PtPath, rootFileDirectory);
  TH1F* tauRatesNoBDT = calculateRates("L1TauAnalyzerRates/l1Tau_pt",          l1PtPath, rootFileDirectory);

  //  TH1F* diTauRates = calculateRates("L1TauAnalyzerRates/l1DiTau_pt", treePath,
  //				    rootFileDirectory, weightFileDirectory, wpLoose);

  plotSixHists(
	    tauRates60, "#tau VTight",
	    tauRates70, "#tau Tight",
	    tauRates80, "#tau Medium",
	    tauRates90, "#tau Loose",
	    tauRates95, "#tau VLoose",
	    tauRatesNoBDT, "#tau No BDT",
	    "rates-analyzer.png",
	    "plots/");

}

/*********************************************************************/

void plotSixHists(TH1F* h1, TString h1Label,
		  TH1F* h2, TString h2Label,
		  TH1F* h3, TString h3Label,
		  TH1F* h4, TString h4Label,
		  TH1F* h5, TString h5Label,
		  TH1F* h6, TString h6Label,
		  TString filename,
		  TString outputDir)
{
  /*******************************************************/
  /* plotting                                            */
  /*******************************************************/
  setTDRStyle();
  TCanvas* Tcan = new TCanvas("Tcan","", 100, 20, 1000, 800);
  TLegend* leg = new TLegend(0.55, 0.2, 0.90, 0.5);
  Tcan->SetGrid();

  Tcan->cd();     /* Set current canvas */
  Tcan->SetFillColor(0);
  applyLegStyle(leg);

  h1->SetMarkerColor(kBlue+2);
  h1->SetLineWidth(2);
  h1->SetLineColor(kBlue+2);

  h2->SetMarkerColor(kBlue-7);
  h2->SetLineWidth(2);
  h2->SetLineColor(kBlue-7);

  h3->SetMarkerColor(kBlue-9);
  h3->SetLineWidth(2);
  h3->SetLineColor(kBlue-9);

  h4->SetMarkerColor(kOrange+4);
  h4->SetLineWidth(2);
  h4->SetLineColor(kOrange+4);

  h5->SetMarkerColor(kOrange+8);
  h5->SetLineWidth(2);
  h5->SetLineColor(kOrange+8);

  h6->SetMarkerColor(kGray);
  h6->SetLineWidth(2);
  h6->SetLineColor(kGray);

  /* Set x-axis limits */
  h2->GetXaxis()->SetRangeUser(0.0, 200.0);
  //  h1->Draw("");
  h2->Draw("");
  h3->Draw("SAME");
  h4->Draw("SAME");
  h5->Draw("SAME");
  h6->Draw("SAME");

  h1->GetXaxis()->SetTitle("L1 Tau p_{T} [GeV]");
  h1->GetYaxis()->SetTitle("Rate [MHz]");
  h1->GetXaxis()->SetTitleSize(0.06); // default is 0.03                                                                                                        
  h1->GetYaxis()->SetTitleSize(0.06);


  //  Tcan->SetLogy();

  /* Customize legend */
  leg->SetHeader("Phase 2 L1 Taus");
  //  leg->AddEntry(h1, h1Label, "l");
  leg->AddEntry(h2, h2Label, "l");
  leg->AddEntry(h3, h3Label, "l");
  leg->AddEntry(h4, h4Label, "l");
  leg->AddEntry(h5, h5Label, "l");
  leg->AddEntry(h6, h6Label, "l");

  gStyle->SetLegendFont(20);
  leg->Draw();

  Tcan->cd();
  Tcan->SaveAs(outputDir+filename);

}


/*********************************************************************/

/* Plots Hist. */

void plotHists(TH1F* h1, TString h1Label,
               TH1F* h2, TString h2Label,
               TH1F* h3, TString h3Label,
	       TString filename,
	       TString outputDir)
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

  h1->SetMarkerColor(kViolet-5);
  h1->SetLineWidth(2);
  h1->SetLineColor(kViolet-5);

  h2->SetMarkerColor(kAzure+7);
  h2->SetLineWidth(2);
  h2->SetLineColor(kAzure+7);

  h3->SetMarkerColor(kOrange+7);
  h3->SetLineWidth(2);
  h3->SetLineColor(kOrange+7);

  /* Set x-axis limits */
  h1->GetXaxis()->SetRangeUser(0.0, 90.0);
  
  h1->Draw("");
  h2->Draw("SAME");
  h3->Draw("SAME");  

  h1->GetXaxis()->SetTitle("L1 Tau p_{T} [GeV]");
  h1->GetYaxis()->SetTitle("Rate [Hz]");
  h1->GetXaxis()->SetTitleSize(0.06); // default is 0.03  
  h1->GetYaxis()->SetTitleSize(0.06);

  
  Tcan->SetLogy();

  /* Customize legend */
  leg->SetHeader("Phase 2 L1 Taus");
  leg->AddEntry(h1, h1Label, "l");
  leg->AddEntry(h2, h2Label, "l");
  leg->AddEntry(h3, h3Label, "l");

  gStyle->SetLegendFont(20);
  leg->Draw();

  Tcan->cd();
  Tcan->SaveAs(outputDir+filename);

}

/*********************************************************************/
