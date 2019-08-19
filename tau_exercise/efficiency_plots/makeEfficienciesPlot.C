/*******************************************************************/
/* makeEfficienciesPlot.C                                          */
/* ROOT macro                                                      */
/* Usage: root -l -b -q makeEfficienciesPlot.C                     */
/*******************************************************************/

#include "calculateEfficiency.C"

/*********************************************************************/

/* Plots loose/medium/tight efficiency histograms and saves to a .png.
 */

void plotHists(TGraphAsymmErrors* histLoose,
	       TGraphAsymmErrors* histMedium,
	       TGraphAsymmErrors* histTight,
	       TGraphAsymmErrors* histNoBDT,
	       TString variable, TString region,
	       TString outputDir)
{
  /*******************************************************/
  /* plotting                                            */
  /*******************************************************/
  setTDRStyle();
  TCanvas* Tcan = new TCanvas("Tcan","", 100, 20, 800, 600);
  TLegend* leg = new TLegend(0.60,0.75,0.85,0.9);
  Tcan->SetGrid();

  Tcan->cd();     /* Set current canvas */
  Tcan->SetFillColor(0);
  leg = new TLegend(0.55, 0.2, 0.90, 0.5);
  applyLegStyle(leg);

  /* Loose */
  histLoose->SetMarkerColor(9);
  histLoose->SetMarkerStyle(kFullCircle);
  histLoose->SetLineWidth(2);
  histLoose->SetLineColor(9); // kBlue+2                                                                                                             

  /* Medium */
  histMedium->SetMarkerColor(8);
  histMedium->SetMarkerStyle(kFullSquare);
  histMedium->SetLineWidth(2);
  histMedium->SetLineColor(8);

  /* Tight */
  histTight->SetMarkerColor(46);
  histTight->SetMarkerStyle(kFullTriangleUp);
  histTight->SetLineWidth(2);
  histTight->SetLineColor(46);

  /* No BDT cut */

  histNoBDT->SetMarkerColor(kBlack);
  histNoBDT->SetMarkerStyle(kFullTriangleDown);
  histNoBDT->SetLineWidth(2);
  histNoBDT->SetLineColor(kBlack);


  histLoose->Draw("");
  histMedium->Draw("SAME");
  histTight->Draw("SAME");
  histNoBDT->Draw("SAME");

  histLoose->GetXaxis()->SetTitle("#tau "+variable+" p_{T} [GeV]");
  histLoose->GetYaxis()->SetTitle("Efficiency");
  histLoose->GetXaxis()->SetTitleSize(0.06); // default is 0.03

  /* Set y-axis limits */
  //  histLoose->SetAxisRange(0.0, 1.1, "Y");

  /* Customize legend */
  leg->SetHeader("Phase 2 L1 Taus");
  leg->AddEntry(histLoose, "Loose", "l");
  
  leg->AddEntry(histMedium, "Medium", "l");
  leg->AddEntry(histTight, "Tight", "l");
  leg->AddEntry(histNoBDT, "No BDT discriminant cut", "l");

  gStyle->SetLegendFont(30);
  leg->Draw();

  Tcan->cd();
  Tcan->SaveAs(outputDir+"efficiency_vs_"+variable+"Pt_"+region+"_allDM-TEST.png");

}

/*******************************************************************/

/* Plots L1 tau efficiency as a function of BDT discriminant cuts. */

void makeEfficienciesPlot(void)
{
  gROOT->ProcessLine(".L calculateEfficiency.C");

  /* Load the TTree. */
  TString treePath = "L1TauAnalyzer/efficiencyTree";
  TString rootFileDirectory = "../ntuples/GluGluHiggsToTauTau_DYToLL_200PU.root";
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
  /* efficiency as a function of recoPt                  */
  /*******************************************************/

  nBins = 50;
  xMin = 0, xMax = 100;
  binWidth = ((xMax - xMin) / nBins);

  recoPtMin = 0;
  recoPtMax = 100;
  recoPtCut = 15;
  genPtCut = 15;
  l1PtCut = 20;
  absEtaLowerBound = 0;
  absEtaUpperBound = 2.5;

  /* Working points: 75%, 80%, and 90% efficiency (approximately) */
  wpLoose = -0.1;
  wpMedium = -0.185;
  wpTight = -0.3;  


  TString region = "barrel";

  /* RECO */
  TGraphAsymmErrors* effVsRecoPtLoose = new TGraphAsymmErrors(nBins);
  TGraphAsymmErrors* effVsRecoPtMedium = new TGraphAsymmErrors(nBins);
  TGraphAsymmErrors* effVsRecoPtTight = new TGraphAsymmErrors(nBins);
  TGraphAsymmErrors* effVsRecoPtNoBDT = new TGraphAsymmErrors(nBins);

  calculateEfficiency(treePath, rootFileDirectory, weightFileDirectory, effVsRecoPtLoose,
		      nBins, "recoPt", region, recoPtMin, recoPtMax, recoPtCut, genPtCut, l1PtCut, wpLoose);
  calculateEfficiency(treePath, rootFileDirectory, weightFileDirectory, effVsRecoPtMedium,
		      nBins, "recoPt", region, recoPtMin, recoPtMax, recoPtCut, genPtCut, l1PtCut, wpMedium);
  calculateEfficiency(treePath, rootFileDirectory, weightFileDirectory, effVsRecoPtTight,
		      nBins, "recoPt", region, recoPtMin, recoPtMax, recoPtCut, genPtCut, l1PtCut, wpTight);
  calculateEfficiency(treePath, rootFileDirectory, weightFileDirectory, effVsRecoPtNoBDT,
		      nBins, "recoPt", region, recoPtMin, recoPtMax, recoPtCut, genPtCut, l1PtCut, -99.99);


  /* GEN */
  
  TGraphAsymmErrors* effVsGenPtLoose = new TGraphAsymmErrors(nBins);
  TGraphAsymmErrors* effVsGenPtMedium = new TGraphAsymmErrors(nBins);
  TGraphAsymmErrors* effVsGenPtTight = new TGraphAsymmErrors(nBins);
  TGraphAsymmErrors* effVsGenPtNoBDT = new TGraphAsymmErrors(nBins);

  calculateEfficiency(treePath, rootFileDirectory, weightFileDirectory, effVsGenPtLoose,
		      nBins, "genPt", region, recoPtMin, recoPtMax, recoPtCut, genPtCut, l1PtCut, wpLoose);
  calculateEfficiency(treePath, rootFileDirectory, weightFileDirectory, effVsGenPtMedium,
                      nBins, "genPt", region, recoPtMin, recoPtMax, recoPtCut, genPtCut, l1PtCut, wpMedium);
  calculateEfficiency(treePath, rootFileDirectory, weightFileDirectory, effVsGenPtTight,
                      nBins, "genPt", region, recoPtMin, recoPtMax, recoPtCut, genPtCut, l1PtCut, wpTight);
  calculateEfficiency(treePath, rootFileDirectory, weightFileDirectory, effVsGenPtNoBDT,
		      nBins, "genPt", region, recoPtMin, recoPtMax, recoPtCut, genPtCut, l1PtCut, -99.99);
  
 
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
  plotHists(effVsRecoPtLoose, effVsRecoPtMedium, effVsRecoPtTight, effVsRecoPtNoBDT, "reco",
		 "barrel", outputDirectory);
  //  plotHists(effVsGenPtLoose, effVsGenPtMedium, effVsGenPtTight, effVsRecoPtNoBDT, "gen",
  //		 "barrel", outputDirectory);
}
