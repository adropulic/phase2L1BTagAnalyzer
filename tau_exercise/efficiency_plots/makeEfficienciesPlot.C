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

  wpLoose = -0.1;
  wpMedium = -0.185;
  wpTight = -0.3;  

  float x, yLoose, yMedium, yTight;
  float dyLoose;
  std::vector<float> effVecLoose;

  TH1F *effVsRecoPtLoose  = new TH1F("effVsRecoPtLoose", "Efficiency vs. recoPt (loose)", nBins, xMin, xMax);
  TH1F *effVsRecoPtMedium = new TH1F("effVsRecoPtMedium", "Efficiency vs. recoPt (medium)", nBins, xMin, xMax);
  TH1F *effVsRecoPtTight = new TH1F("effVsRecoPtTight", "Efficiency vs. recoPt (tight)", nBins, xMin, xMax);

  for (int i = 0; i < (nBins + 1); i++)
    {
      x = (xMin + i * binWidth);

      
      effVecLoose = calculateEfficiency(treePath, rootFileDirectory, weightFileDirectory, x, 20, 0, 2.5, wpLoose);
      //      yMedium = calculateEfficiency(treePath, rootFileDirectory, weightFileDirectory, x, 20, 0, 2.5, wpMedium);
      //      yTight = calculateEfficiency(treePath, rootFileDirectory, weightFileDirectory, x, 20, 0, 2.5, wpTight);
      
      //      effVsRecoPtLoose->SetBinError(x, effVecLoose.at(0), effVecLoose.at(1));
      effVsRecoPtLoose->Fill(x + (binWidth / 2), effVecLoose.at(0));
      //      effVsRecoPtMedium->Fill(x + (binWidth / 2), yMedium);
      //      effVsRecoPtTight->Fill(x + (binWidth / 2), yTight);

      effVsRecoPtLoose->SetBinError(i, effVecLoose.at(1));
      printf("bin %f: y = %f, error = %f\n", x, effVecLoose.at(0), effVecLoose.at(1));
    }

  /*******************************************************/
  /* plotting                                            */
  /*******************************************************/
  setTDRStyle();
  TCanvas* Tcan = new TCanvas("Tcan","", 100, 20, 800, 600);
  TLegend* leg = new TLegend(0.60,0.75,0.85,0.9); 
 
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
  leg = new TLegend(0.60,0.75,0.85,0.9);
  applyLegStyle(leg);

  /* Loose */
  effVsRecoPtLoose->SetMarkerColor(38);
  effVsRecoPtLoose->SetMarkerStyle(kFullCircle); 
  //  effVsRecoPtLoose->SetFillStyle(1001);
  //  effVsRecoPtLoose->SetFillColorAlpha(kBlue+2, 0.1);
  effVsRecoPtLoose->SetLineWidth(2);
  effVsRecoPtLoose->SetLineColor(38+2); // kBlue+2

  /* Medium */
  //effVsRecoPtMedium->SetMarkerColor(46);
  //effVsRecoPtMedium->SetMarkerStyle(kFullCircle);
  //  effVsRecoPtMedium->SetFillStyle(1001);
  //  effVsRecoPtMedium->SetFillColorAlpha(kGreen+2, 0.1);
  //  effVsRecoPtMedium->SetLineWidth(1);
  //  effVsRecoPtMedium->SetLineColor(kGreen+2);

  /* Tight */
  //  effVsRecoPtTight->SetMarkerColor(30);
  //effVsRecoPtTight->SetMarkerStyle(kFullCircle);
  //  effVsRecoPtTight->SetFillStyle(1001);
  //  effVsRecoPtTight->SetFillColorAlpha(kRed+2, 0.1);
  //  effVsRecoPtTight->SetLineWidth(1);
  //  effVsRecoPtTight->SetLineColor(kRed+2);

  /* Drawing, set titles */
  //  effVsRecoPtLoose->Draw("e");
  //  effVsRecoPtMedium->Draw("EP SAME");
  //  effVsRecoPtTight->Draw("EP SAME");
  effVsRecoPtLoose->GetXaxis()->SetTitle("Tau recoPt [GeV]");
  effVsRecoPtLoose->GetYaxis()->SetTitle("Efficiency");

  /* Set y-axis limits */
  effVsRecoPtLoose->SetAxisRange(0.0, 1.0, "Y");
  
  /* Customize legend */
  leg->SetHeader("Phase 2 L1 Taus");
  leg->AddEntry(effVsRecoPtLoose, "Loose", "l");
  //  leg->AddEntry(effVsRecoPtMedium, "Medium", "l");
  //  leg->AddEntry(effVsRecoPtTight, "Tight", "l");
  leg->Draw();

  effVsRecoPtLoose->Draw("ep");
  Tcan->cd();
  Tcan->Show();

  Tcan->SaveAs(outputDirectory+"efficiency_vs_recoPt_l1Pt-20GeV-testStyle.png");

  //  delete Tcan;
}
