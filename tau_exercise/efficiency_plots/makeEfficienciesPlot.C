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
  TString rootFileDirectory = "../ntuples/use_charghadr_newAlgo/2019_Sep28-DYToLL_200PU_ext1-USE_CHARGED_HADRON_new_algo.root";
  TString weightFileDirectory = "../training/dataset/weights/TMVAClassification_BDT.weights.xml";
  TString outputDirectory = "plots/";

  float xMin, xMax;
  TString recoCut, l1Cut;

  /* Working points: 60%, 70%, 80%, 90%, 95% */
  float wp60 = 0.0540466;
  float wp70 = 0.0126322;   // Tight
  float wp80 = -0.0253954;  // Medium
  float wp90 = -0.0435867;  // Loose
  float wp95 = -0.0486184;  // VLoose
  float wpNone = -99;

  /*******************************************************/
  /* efficiency as a function of recoPt                  */
  /*******************************************************/

  xMin = 0;
  xMax = 100;
  recoCut = "recoPt>10 && genPt>10 && l1Track_pt>10 && genDM>9";
  l1Cut   = "recoPt>10 && genPt>10 && l1Track_pt>10 && genDM>9 && l1Pt>0";
  
  TGraphAsymmErrors* effVsRecoPt90 = calculateEfficiency("recoPt", treePath, rootFileDirectory,
							 l1Cut + "&& (l1BDTDisriminant > -0.0435867)",
							 recoCut, xMin, xMax, true);
  
  TGraphAsymmErrors* effVsRecoPt95 = calculateEfficiency("recoPt", treePath, rootFileDirectory,
                                                         l1Cut + "&& (l1BDTDisriminant > -0.0486184)",
							 recoCut, xMin, xMax, true);

  TGraphAsymmErrors* effVsRecoPtNoBDT = calculateEfficiency("recoPt", treePath, rootFileDirectory,
							    l1Cut,
							    recoCut, xMin, xMax, true);


  plotThreeHists(
		 effVsRecoPt90, "allDM Loose",
		 effVsRecoPt95, "allDM VLoose",
		 effVsRecoPtNoBDT, "allDM",
		 "Reco #tau_{H} p_{T} [GeV]",
		 "Phase 2 L1 Taus",
		 "effRecoPt_allDM_l1TracksPt10.png",
		 outputDirectory);

  /*******************************************************/
  /* efficiency as a function of recoEta                 */
  /*******************************************************/
  xMin = -3;
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
		 effVsRecoEta90, "allDM Loose",
		 effVsRecoEta95, "allDM VLoose",
		 effVsRecoEtaNoBDT, "allDM",
		 "Reco #tau_{H} #eta",
		 "Phase 2 L1 Taus",
		 "effVsRecoEta_allDM_l1TracksPt10.png",
		 outputDirectory);
  
}


/*********************************************************************/
