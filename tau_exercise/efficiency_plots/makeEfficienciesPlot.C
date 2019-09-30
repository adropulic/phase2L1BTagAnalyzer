/*******************************************************************/
/* makeEfficienciesPlot.C                                          */
/* ROOT macro                                                      */
/* Usage: root -l -b -q makeEfficienciesPlot.C                     */
/*******************************************************************/

#include "efficiencyHist.C"
#include "calculateEfficiency.C"
#include "calculateL1TrackEfficiency.C"
#include "calculatePFCandEfficiency.C"

void plotHists(TGraphAsymmErrors* histLoose,
               TGraphAsymmErrors* histMedium,
               TGraphAsymmErrors* histMedium2,
               TGraphAsymmErrors* histTight,
               TGraphAsymmErrors* histTight2,
	       TGraphAsymmErrors* histNoBDT,
               TString variable, TString region,
               TString outputDir);

/*********************************************************************/

/* Plots L1 tau efficiency for different BDT discriminant working
   points. */

void makeEfficienciesPlot(void)
{
  gROOT->ProcessLine(".L calculateEfficiency.C");

  /* Load the TTree. */
  TString treePath = "L1TauAnalyzer/efficiencyTree";
  TString rootFileDirectory = "../ntuples/use_charghadr_newAlgo/2019_Sep28-DYToLL_200PU_ext1-USE_CHARGED_HADRON_new_algo.root";
  //  TString rootFileDirectory = "../ntuples/l1Tracks_PFCands_noChargedHadronMatching/2019_Sep25-DYToLL_NoPU_noChargHadrMatch.root";
  TString weightFileDirectory = "../training/dataset/weights/TMVAClassification_BDT.weights.xml";
  TString outputDirectory = "plots/";

  /*******************************************************/
  /* efficiency as a function of recoPt/genPt            */
  /*******************************************************/

  int nBins = 50;
  float xMin = 0;
  float xMax = 100;

  float recoPtMin = 0;
  float recoPtMax = 80;
  float recoPtCut = 15;
  float genPtCut = 15;
  float l1PtCut = 20;

  /* Working points: 60%, 70%, 80%, 90%, 95% */
  float wp60 = 0.0540466;
  float wp70 = 0.0126322;
  float wp80 = -0.0253954;
  float wp90 = -0.0435867;
  float wp95 = -0.0486184;
  float wpNone = -99;

  TString region = "barrel";

  /* RECO */
  TGraphAsymmErrors* effVsRecoPt60 = calculateEfficiency(treePath, rootFileDirectory, weightFileDirectory,
							 nBins, "recoPt", region, recoPtMin, recoPtMax,
							 recoPtCut, genPtCut, l1PtCut, wp60);
  TGraphAsymmErrors* effVsRecoPt70 = calculateEfficiency(treePath, rootFileDirectory, weightFileDirectory,
							 nBins, "recoPt", region, recoPtMin, recoPtMax,
							 recoPtCut, genPtCut, l1PtCut, wp70);
  TGraphAsymmErrors* effVsRecoPt80 = calculateEfficiency(treePath, rootFileDirectory, weightFileDirectory,
                                                         nBins, "recoPt", region, recoPtMin, recoPtMax,
                                                         recoPtCut, genPtCut, l1PtCut, wp80);
  TGraphAsymmErrors* effVsRecoPt90 = calculateEfficiency(treePath, rootFileDirectory, weightFileDirectory,
                                                         nBins, "recoPt", region, recoPtMin, recoPtMax,
                                                         recoPtCut, genPtCut, l1PtCut, wp90);
  TGraphAsymmErrors* effVsRecoPt95 = calculateEfficiency(treePath, rootFileDirectory, weightFileDirectory,
                                                         nBins, "recoPt", region, recoPtMin, recoPtMax,
                                                         recoPtCut, genPtCut, l1PtCut, wp95);
  TGraphAsymmErrors* effVsRecoPtNoBDT = calculateEfficiency(treePath, rootFileDirectory, weightFileDirectory,
							    nBins, "recoPt", region, recoPtMin, recoPtMax,
							    recoPtCut, genPtCut, l1PtCut, wpNone);

  plotHists(effVsRecoPt60,
	    effVsRecoPt70,
	    effVsRecoPt80,
	    effVsRecoPt90,
	    effVsRecoPt95,
	    effVsRecoPtNoBDT,
	    "reco",
	    "barrel",
	    outputDirectory);
}

/*********************************************************************/

/* Plots loose/medium/tight efficiency histograms and saves to a .png. */

void plotHists(TGraphAsymmErrors* histLoose,
	       TGraphAsymmErrors* histMedium,
	       TGraphAsymmErrors* histMedium2,
	       TGraphAsymmErrors* histTight,
	       TGraphAsymmErrors* histTight2,
	       TGraphAsymmErrors* histNoBDT,
	       TString variable, TString region,
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

  histLoose->GetXaxis()->SetTitle("Reco #tau_{H} p_{T} [GeV]");
  histLoose->GetYaxis()->SetTitle("L1 Efficiency");
  histLoose->GetXaxis()->SetTitleSize(0.06); // default is 0.03

  /* Customize legend */
  leg->SetHeader("Phase 2 L1 Taus (All DM) Barrel");
  leg->AddEntry(histLoose, "60%", "l");
  leg->AddEntry(histMedium, "70%", "l");
  leg->AddEntry(histMedium2, "80%", "l");
  leg->AddEntry(histTight, "90%", "l");
  leg->AddEntry(histTight2, "95%", "l");
  leg->AddEntry(histNoBDT, "No BDT", "l");

  gStyle->SetLegendFont(30);
  leg->Draw();

  Tcan->cd();
  Tcan->SaveAs(outputDir+"efficiency_vs_"+variable+"Pt_"+region+"allDM_l1TrackPt10.png");

}

/*********************************************************************/
