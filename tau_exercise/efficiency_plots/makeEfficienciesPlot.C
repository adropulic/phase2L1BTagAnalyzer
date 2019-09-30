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

  /* RECO PT */
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
	    "Reco #tau_{H} p_{T} [GeV]",
	    "Phase 2 L1 Taus (All DM) Barrel",
	    "effVsRecoPt_"+region+"_allDM_l1TracksPt10_TESTINGscriptRewrite.png",
	    outputDirectory);
}


/*********************************************************************/
