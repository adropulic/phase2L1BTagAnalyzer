/*******************************************************************/
/* makeEfficienciesPlot.C                                          */
/* ROOT macro                                                      */
/* Usage: root -l -b -q makeEfficienciesPlot.C                     */
/*******************************************************************/

#include "calculateEfficiency.C"
#include "calculateL1TrackEfficiency.C"
#include "calculatePFCandEfficiency.C"

void plotHists(TGraphAsymmErrors* histLoose,
               TGraphAsymmErrors* histMedium,
               TGraphAsymmErrors* histTight,
               TGraphAsymmErrors* histNoBDT,
	       TGraphAsymmErrors* histStudy,
	       TGraphAsymmErrors* histStudy2,
               TString variable, TString region,
               TString outputDir);

/*********************************************************************/

/* Plots L1 tau efficiency as a function of BDT discriminant cuts. */

void makeEfficienciesPlot(void)
{
  gROOT->ProcessLine(".L calculateEfficiency.C");

  /* Load the TTree. */
  TString treePath = "L1TauAnalyzer/efficiencyTree";
  //TString rootFileDirectory = "../ntuples/l1Tracks_only/2019_Sep10-DYToLL_NoPU_withL1Tracks.root";
  //  TString rootFileDirectory = "../ntuples/l1Tracks_and_PFCands/2019_Sep17-DYToLL_NoPU_withL1Tracks_PFCand.root";
  TString rootFileDirectory = "../ntuples/l1Tracks_PFCands_noChargedHadronMatching/2019_Sep25-DYToLL_NoPU_noChargHadrMatch.root";
  TString weightFileDirectory = "../training/dataset/weights/TMVAClassification_BDT.weights.xml";
  TString outputDirectory = "plots/";

  int nBins;
  float xMin, xMax, binWidth;
  float recoPtMin, recoPtMax, recoPtCut, genPtCut, l1PtCut, absEtaLowerBound, absEtaUpperBound;
  float wpLoose, wpMedium, wpTight; 

  /*******************************************************/
  /* efficiency as a function of BDT cutoff              */
  /*******************************************************/
  /*
  nBins = 40;
  xMin = -0.8, xMax = 0.8;

  TH1F *effVsBDTCutoff = new TH1F("effVsBDTCutoff", "Efficiency vs. BDT cutoff", nBins, xMin, xMax);
  binWidth = ((xMax - xMin) / nBins);

  float x, y;
  for (int i = 0; i < (nBins + 1); i++)
    {
      x = (xMin + i * binWidth);
      y = calculateEfficiency(treePath, rootFileDirectory, weightFileDirectory, 15, 15, 0, 2.5, x);
      effVsBDTCutoff->Fill(x + (binWidth / 2), y);

      // printf("(x, y) = (%f, %f)\n", x, y);

    }
  */

  /*******************************************************/
  /* efficiency as a function of recoPt/genPt            */
  /*******************************************************/

  nBins = 50;
  xMin = 0, xMax = 100;
  binWidth = ((xMax - xMin) / nBins);

  recoPtMin = 0;
  recoPtMax = 80;
  recoPtCut = 15;
  genPtCut = 15;
  l1PtCut = 20;
  

  /* Working points: 75%, 80%, and 90% efficiency (approximately) */
  /*wpLoose = -0.1;
  wpMedium = -0.185;
  wpTight = -0.3;  */
  wpLoose = -0.1;
  wpMedium = -0.05;
  wpTight = 0.0;

  TString region = "barrel";

  /* RECO */
  TGraphAsymmErrors* effVsRecoPtLoose = new TGraphAsymmErrors();
  TGraphAsymmErrors* effVsRecoPtMedium = new TGraphAsymmErrors();
  TGraphAsymmErrors* effVsRecoPtTight = new TGraphAsymmErrors();
  TGraphAsymmErrors* effVsRecoPtNoBDT = new TGraphAsymmErrors();
  TGraphAsymmErrors* effL1Track = new TGraphAsymmErrors();
  TGraphAsymmErrors* effPFCand = new TGraphAsymmErrors();

  calculateEfficiency(treePath, rootFileDirectory, weightFileDirectory, effVsRecoPtLoose,
		      nBins, "recoPt", region, recoPtMin, recoPtMax, recoPtCut, genPtCut, l1PtCut, wpLoose);
  calculateEfficiency(treePath, rootFileDirectory, weightFileDirectory, effVsRecoPtMedium,
		      nBins, "recoPt", region, recoPtMin, recoPtMax, recoPtCut, genPtCut, l1PtCut, wpMedium);
  calculateEfficiency(treePath, rootFileDirectory, weightFileDirectory, effVsRecoPtTight,
		      nBins, "recoPt", region, recoPtMin, recoPtMax, recoPtCut, genPtCut, l1PtCut, wpTight);
  calculateEfficiency(treePath, rootFileDirectory, weightFileDirectory, effVsRecoPtNoBDT,
		      nBins, "recoPt", region, recoPtMin, recoPtMax, recoPtCut, genPtCut, l1PtCut, -99.99);

  calculateL1TrackEfficiency(treePath, rootFileDirectory, effL1Track,
  			     nBins, "recoPt", region, recoPtMin, recoPtMax, recoPtCut, genPtCut, l1PtCut);
  calculatePFCandEfficiency(treePath, rootFileDirectory, effPFCand,
			    nBins, "recoPt", region, recoPtMin, recoPtMax, recoPtCut, genPtCut, l1PtCut);
  

  /* GEN */
  /*
  TGraphAsymmErrors* effVsGenPtLoose = new TGraphAsymmErrors();
  TGraphAsymmErrors* effVsGenPtMedium = new TGraphAsymmErrors();
  TGraphAsymmErrors* effVsGenPtTight = new TGraphAsymmErrors();
  TGraphAsymmErrors* effVsGenPtNoBDT = new TGraphAsymmErrors();

  calculateEfficiency(treePath, rootFileDirectory, weightFileDirectory, effVsGenPtLoose,
		      nBins, "genPt", region, recoPtMin, recoPtMax, recoPtCut, genPtCut, l1PtCut, wpLoose);
  calculateEfficiency(treePath, rootFileDirectory, weightFileDirectory, effVsGenPtMedium,
                      nBins, "genPt", region, recoPtMin, recoPtMax, recoPtCut, genPtCut, l1PtCut, wpMedium);
  calculateEfficiency(treePath, rootFileDirectory, weightFileDirectory, effVsGenPtTight,
                      nBins, "genPt", region, recoPtMin, recoPtMax, recoPtCut, genPtCut, l1PtCut, wpTight);
  calculateEfficiency(treePath, rootFileDirectory, weightFileDirectory, effVsGenPtNoBDT,
		      nBins, "genPt", region, recoPtMin, recoPtMax, recoPtCut, genPtCut, l1PtCut, -99.99);
  */
 
  /********************************/
  /* plotting eff vs. BDT cutoff  */
  /********************************/
  /*
  Tcan->cd();     // Set current canvas 
  Tcan->SetFillColor(0);

  applyLegStyle(leg);


  effVsBDTCutoff->SetMarkerColor(0);
  effVsBDTCutoff->SetFillStyle(1001);
  effVsBDTCutoff->SetFillColorAlpha(kGreen+2, 0.1);
  effVsBDTCutoff->SetLineWidth(1);
  effVsBDTCutoff->SetLineColor(kGreen+2);

  effVsBDTCutoff->Draw("HIST"); 

  effVsBDTCutoff->GetXaxis()->SetTitle("BDT discriminant cut");
  effVsBDTCutoff->GetYaxis()->SetTitle("Efficiency");


  leg->SetHeader("Phase 2 L1 PF Taus");
  leg->AddEntry(effVsBDTCutoff, "efficiency", "l");
  leg->Draw();
  
  Tcan->cd();
  
  Tcan->SaveAs(outputDirectory+"efficiency_vs_bdt_cutoff.png");
  
  Tcan->Clear();
  */


  /* void plotThreeHists(TH1F *histLoose, TH1F *histMedium, TH1F *histTight,
                    TString variable, TString region,
                    TString outputDir)*/
  plotHists(effVsRecoPtLoose, effVsRecoPtMedium, effVsRecoPtTight, effVsRecoPtNoBDT, effL1Track,
	    effPFCand,
	    "reco",
	    "barrel", outputDirectory);
  //  plotHists(effVsGenPtLoose, effVsGenPtMedium, effVsGenPtTight, effVsRecoPtNoBDT, "gen",
  //	    "barrel", outputDirectory);
}

/*********************************************************************/

/* Plots loose/medium/tight efficiency histograms and saves to a .png.
 */

void plotHists(TGraphAsymmErrors* histLoose,
	       TGraphAsymmErrors* histMedium,
	       TGraphAsymmErrors* histTight,
	       TGraphAsymmErrors* histNoBDT,
	       TGraphAsymmErrors* histStudy,
	       TGraphAsymmErrors* histStudy2,
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
  histLoose->SetMarkerColor(kBlue-4);
  histLoose->SetMarkerStyle(kFullCircle);
  histLoose->SetLineWidth(2);
  histLoose->SetLineColor(kBlue-4); // kBlue+2

  /* Medium */
  histMedium->SetMarkerColor(kSpring-1);
  histMedium->SetMarkerStyle(kFullSquare);
  histMedium->SetLineWidth(2);
  histMedium->SetLineColor(kSpring-1);

  /* Tight */
  histTight->SetMarkerColor(kRed+1);
  histTight->SetMarkerStyle(kFullTriangleUp);
  histTight->SetLineWidth(2);
  histTight->SetLineColor(kRed+1);

  /* No BDT cut */
  histNoBDT->SetMarkerColor(kRed+4);
  histNoBDT->SetMarkerStyle(kFullTriangleDown);
  histNoBDT->SetLineWidth(2);
  histNoBDT->SetLineColor(kRed+4);

  /* Study */
  histStudy->SetMarkerColor(kOrange+7);
  histStudy->SetMarkerStyle(kFullCircle);
  histStudy->SetLineWidth(2);
  histStudy->SetLineColor(kOrange+7);

  /* Study 2*/
  histStudy2->SetMarkerColor(kPink+8);
  histStudy2->SetMarkerStyle(kFullCircle);
  histStudy2->SetLineWidth(2);
  histStudy2->SetLineColor(kPink+8);

  /* Set y-axis limits */
  histLoose->GetYaxis()->SetRangeUser(0.0, 1.1);

  histLoose->Draw("");
  histMedium->Draw("SAME");
  histTight->Draw("SAME");
  histNoBDT->Draw("SAME");
  //  histStudy->Draw("SAME");
  //histStudy2->Draw("SAME");

  histLoose->GetXaxis()->SetTitle("Reco #tau_{H} p_{T} [GeV]");
  histLoose->GetYaxis()->SetTitle("L1 Efficiency");
  histLoose->GetXaxis()->SetTitleSize(0.06); // default is 0.03

  /* Customize legend */
  leg->SetHeader("Phase 2 L1 Taus (1prong) Barrel");
  leg->AddEntry(histLoose, "Loose", "l");
  leg->AddEntry(histMedium, "Medium", "l");
  leg->AddEntry(histTight, "Tight", "l");
  leg->AddEntry(histNoBDT, "No BDT discriminant cut", "l");

  //  leg->AddEntry(histStudy, "L1 track matched with Reco Tau", "l");
  //leg->AddEntry(histStudy2, "PF Cand matched with Reco Tau", "l");

  gStyle->SetLegendFont(30);
  leg->Draw();

  Tcan->cd();
  Tcan->SaveAs(outputDir+"efficiency_vs_"+variable+"Pt_"+region+"_DM0_testingWPs.png");

}

/*********************************************************************/
