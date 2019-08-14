/*******************************************************************/
/* makeEfficienciesPlot.C                                          */
/* ROOT macro                                                      */
/*******************************************************************/

#include "calculateEfficiency.C"

/*******************************************************************/

/* Plots L1 tau efficiency as a function of BDT discriminant cuts. */

void makeEfficienciesPlot(void)
{
  gROOT->ProcessLine(".L calculateEfficiency.C");

  /* Load the TTree. */
  TString treePath = "L1TauAnalyzer/efficiencyTree";
  TString rootFileDirectory = "../ntuples/GluGluHiggsToTauTau.root";
  TString weightFileDirectory = "../training/dataset/weights/TMVAClassification_BDT.weights.xml";
  TString outputDirectory = "";

  int nBins;
  float xMin, xMax, binWidth;
  float recoPtMin, recoPtMax, genPtCut, l1PtCut, absEtaLowerBound, absEtaUpperBound;
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

  nBins = 20;
  xMin = 0, xMax = 100;
  binWidth = ((xMax - xMin) / nBins);

  recoPtMin = 0;
  recoPtMax = 100;
  genPtCut = 15;
  l1PtCut = 15;
  absEtaLowerBound = 0;
  absEtaUpperBound = 2.5;

  /* Working points: 75%, 80%, and 90% efficiency (approximately) */
  wpLoose = -0.1;
  wpMedium = -0.185;
  wpTight = -0.3;  

  TH1F *effVsRecoPtLoose  = new TH1F("effVsRecoPtLoose", "Efficiency vs. recoPt (loose)", nBins, xMin, xMax);
  TH1F *effVsRecoPtMedium = new TH1F("effVsRecoPtMedium", "Efficiency vs. recoPt (medium)", nBins, xMin, xMax);
  TH1F *effVsRecoPtTight = new TH1F("effVsRecoPtTight", "Efficiency vs. recoPt (tight)", nBins, xMin, xMax);


  calculateEfficiency(treePath, rootFileDirectory, weightFileDirectory, effVsRecoPtLoose,
		      nBins, recoPtMin, recoPtMax, genPtCut, l1PtCut, absEtaLowerBound, absEtaUpperBound,
		      wpLoose);
  calculateEfficiency(treePath, rootFileDirectory, weightFileDirectory, effVsRecoPtMedium,
		      nBins, recoPtMin, recoPtMax, genPtCut, l1PtCut, absEtaLowerBound, absEtaUpperBound,
		      wpMedium);
  calculateEfficiency(treePath, rootFileDirectory, weightFileDirectory,effVsRecoPtTight,
		      nBins, recoPtMin, recoPtMax, genPtCut, l1PtCut, absEtaLowerBound, absEtaUpperBound,
		      wpTight);

  /*******************************************************/
  /* plotting                                            */
  /*******************************************************/
  setTDRStyle();
  TCanvas* Tcan = new TCanvas("Tcan","", 100, 20, 800, 600);
  TLegend* leg = new TLegend(0.60,0.75,0.85,0.9); 
  Tcan->SetGrid();
 
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
  /********************************/
  /* plotting eff vs. recoPt      */
  /********************************/
  /* Reset canvas */
  Tcan->cd();     /* Set current canvas */
  Tcan->SetFillColor(0);
  leg = new TLegend(0.55, 0.2, 0.90, 0.5);
  applyLegStyle(leg);

  /* Loose */
  effVsRecoPtLoose->SetMarkerColor(9);
  effVsRecoPtLoose->SetMarkerStyle(kFullCircle); 
  effVsRecoPtLoose->SetLineWidth(2);
  effVsRecoPtLoose->SetLineColor(9); // kBlue+2

  /* Medium */
  effVsRecoPtMedium->SetMarkerColor(8);
  effVsRecoPtMedium->SetMarkerStyle(kFullSquare);
  effVsRecoPtMedium->SetLineWidth(2);
  effVsRecoPtMedium->SetLineColor(8);

  /* Tight */
  effVsRecoPtTight->SetMarkerColor(46);
  effVsRecoPtTight->SetMarkerStyle(kFullTriangleUp);
  effVsRecoPtTight->SetLineWidth(2);
  effVsRecoPtTight->SetLineColor(46);

  effVsRecoPtLoose->Draw("E1 P");
  effVsRecoPtMedium->Draw("E1 P SAME");
  effVsRecoPtTight->Draw("E1 P SAME");

  effVsRecoPtLoose->GetXaxis()->SetTitle("#tau reco p_{T} [GeV]");
  effVsRecoPtLoose->GetYaxis()->SetTitle("Efficiency");
  effVsRecoPtLoose->GetXaxis()->SetTitleSize(0.06); // default is 0.03
  effVsRecoPtLoose->GetYaxis()->SetTitleSize(0.06); // default is 0.03

  /* Set y-axis limits */
  effVsRecoPtLoose->SetAxisRange(0.0, 1.2, "Y");
  
  /* Customize legend */
  leg->SetHeader("Phase 2 L1 Taus");
  leg->AddEntry(effVsRecoPtLoose, "Loose", "l");
  leg->AddEntry(effVsRecoPtMedium, "Medium", "l");
  leg->AddEntry(effVsRecoPtTight, "Tight", "l");
  leg->Draw();

  Tcan->Show();

  Tcan->SaveAs(outputDirectory+"efficiency_vs_recoPt_l1Pt-20GeV-testStyle.png");

  //  delete Tcan;
}
