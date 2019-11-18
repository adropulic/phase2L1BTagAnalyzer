/*******************************************************************/
/* makeEfficienciesPlot.C                                          */
/* ROOT macro                                                      */
/* Usage: root -l -b -q makeEfficienciesPlot.C                     */
/*******************************************************************/

#include "efficiencyHist.C"
#include "calculateEfficiency.C"
#include "calculateL1TrackEfficiency.C"
#include "calculatePFCandEfficiency.C"

#include <string>
/*********************************************************************/

/* Plots L1 tau efficiency for different BDT discriminant working
   points. */

void makeEfficienciesPlot(void)
{
  gROOT->ProcessLine(".L calculateEfficiency.C");

  /* Load the TTree. */
  TString treePath = "L1TauAnalyzer/efficiencyTree";
  TString rootFileDirectory = "../ntuples/newTauZL1PtCut10/ggH.root";
  TString outputDirectory = "plots/PF_200PU_deltaZ5cm_sandeepCheck/";

  float xMin, xMax;
  TString recoCut, l1Cut;

  /*******************************************************/
  /* efficiency as a function of recoPt                  */
  /*******************************************************/

  xMin = 0;
  xMax = 100;
  recoCut = "recoPt>10 && genPt>10 && l1Track_pt>10 && ((genDM>9) || (genDM==3) || (genDM==4))";
  l1Cut   = "recoPt>10 && genPt>10 && l1Track_pt>10 && ((genDM>9) || (genDM==3) || (genDM==4)) && l1Pt>0";
  
  TGraphAsymmErrors* recoPtVLoose = calculateEfficiency("recoPt", treePath, rootFileDirectory,
						     l1Cut + "&& l1IsoVLoose",
						     recoCut, xMin, xMax, true);
  
  TGraphAsymmErrors* recoPtLoose = calculateEfficiency("recoPt", treePath, rootFileDirectory,
						       l1Cut + "&& l1IsoLoose",
						       recoCut, xMin, xMax, true);
  
  TGraphAsymmErrors* recoPtMedium = calculateEfficiency("recoPt", treePath, rootFileDirectory,
							l1Cut + "&& l1IsoMedium",
							recoCut, xMin, xMax, true);
  
  TGraphAsymmErrors* recoPtTight = calculateEfficiency("recoPt", treePath, rootFileDirectory,
						       l1Cut + "&& l1IsoTight",
						       recoCut, xMin, xMax, true);


  TGraphAsymmErrors* recoPtNoBDT = calculateEfficiency("recoPt", treePath, rootFileDirectory,
						       l1Cut,
						       recoCut, xMin, xMax, true);



  plotFiveHists(
		recoPtNoBDT, "allDM + leptonic", kAzure+1,
		recoPtTight, "allDM + leptonic, Tight", kBlue-3,
		recoPtMedium, "allDM + leptonic, Medium", kViolet-5,
		recoPtLoose, "allDM + leptonic, Loose", kPink+8,
		recoPtVLoose, "allDM + leptonic, VLoose", kPink+6,
		"Reco #tau_{H} p_{T} [GeV]",
		"Phase 2 L1 Taus",
		"eff_recoPt_PF_deltaZ5_WITH_LeptonDM.png",
		outputDirectory);

  /*******************************************************/
  /* efficiency as a function of recoEta                 */
  /*******************************************************/
  /*  xMin = -3;
  xMax = 3;
  
  recoCut = "recoPt>30 && genPt>20 && l1Track_pt>10 && genDM>9";
  l1Cut   = "recoPt>30 && genPt>20 && l1Track_pt>10 && genDM>9 && l1Pt>10";

  TGraphAsymmErrors* effVsRecoEta90 = calculateEfficiency("recoEta", treePath, rootFileDirectory,
							  l1Cut + "&& (l1BDTDisriminant > -0.0435867)",
							  recoCut, xMin, xMax, false);
  TGraphAsymmErrors* effVsRecoEta95 = calculateEfficiency("recoEta", treePath, rootFileDirectory,
                                                          l1Cut + "&& (l1BDTDisriminant > -0.0486184)",
							  recoCut, xMin, xMax, false);
  TGraphAsymmErrors* effVsRecoEtaNoBDT = calculateEfficiency("recoEta", treePath, rootFileDirectory, 
							     l1Cut,
							     recoCut, xMin, xMax, false);

  plotThreeHists(
		 effVsRecoEta90, "BDT Loose",
		 effVsRecoEta95, "BDT VLoose",
		 effVsRecoEtaNoBDT, "No BDT",
		 "Reco #tau_{H} #eta",
		 "Phase 2 L1 Taus (all #tau_{H} decay modes)",
		 "effVsRecoEta_allDM_l1TracksPt10.png",
		 outputDirectory);
  */
}


/*********************************************************************/
