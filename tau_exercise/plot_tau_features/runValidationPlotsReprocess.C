#include "comparisonPlots.C"

void runValidationPlotsReprocess()
{
  gROOT->ProcessLine(".L comparisonPlots.C");
 
  TString treePath = "L1TauAnalyzer/efficiencyTree";
  TString inputDirectory  = "/afs/cern.ch/work/s/skkwan/public/triggerDevel/jul2019_2/CMSSW_10_6_0_pre4/src/L1Trigger/phase2L1TauAnalyzer/test/analyzer-dyll.root";
  TString outputDirectory = "/afs/cern.ch/work/s/skkwan/public/triggerDevel/apr2019/CMSSW_10_5_0_pre1/src/L1Trigger/phase2L1BTagAnalyzer/tau_exercise/plot_tau_features/validationPlots/dyll/dyll_";

  TString sigCut="(genPt>20)&&(l1Pt>0)";
  TString bkgCut="(genPt<5)&&(l1Pt>0)";
  comparisonPlots("l1Pt", sigCut, bkgCut, treePath, inputDirectory, outputDirectory, "l1Pt", 150, 0, 100);
  comparisonPlots("l1Eta", sigCut, bkgCut, treePath, inputDirectory, outputDirectory, "l1Eta", 100, -3, 3);
  comparisonPlots("l1Phi", sigCut, bkgCut, treePath, inputDirectory, outputDirectory, "l1Phi", 100, -4, 4);
  comparisonPlots("l1TauZ", sigCut, bkgCut, treePath, inputDirectory, outputDirectory, "l1TauZ", 180, -250, 310);
  comparisonPlots("l1PVDZ", sigCut, bkgCut, treePath, inputDirectory, outputDirectory, "l1PVDZ", 180, -250, 310);
  // comparisonPlots("l1Time", "genPt>20", "genPt<5", treePath, inputDirectory, outputDirectory, "l1Time", 200, 2, 20);
  // l1Iso has the tau included in the sum so it must be removed
  // comparisonPlots("l1Iso-l1Pt", "genPt>20", "genPt<5", treePath, inputDirectory, outputDirectory, "l1Iso", 200, -10, 10);
  // comparisonPlots("l1Iso_time", "genPt>20", "genPt<5", treePath, inputDirectory, outputDirectory, "l1Iso_time", 200, -10, 10);
  // Track DZs
  /*
  comparisonPlots("track12DZ", "genPt>20", "genPt<5", treePath, inputDirectory, outputDirectory, "track12DZ", 200, -11, 5);
  comparisonPlots("track13DZ", "genPt>20", "genPt<5", treePath, inputDirectory, outputDirectory, "track13DZ", 200, -11, 5);
  comparisonPlots("track1PVDZ", "genPt>20", "genPt<5", treePath, inputDirectory, outputDirectory, "l1track1PVDZ", 200, -5, 5);
  comparisonPlots("track2PVDZ","genPt>20","genPt<5",treePath, inputDirectory, outputDirectory, "l1track2PVDZ", 200, -5, 5);
  comparisonPlots("track3PVDZ", "genPt>20", "genPt<5", treePath, inputDirectory, outputDirectory, "l1track3PVDZ", 200, -5, 5); 
  */
  // Number of stubs
  /*
  comparisonPlots("track1nStubs", "genPt>20", "genPt<5", treePath, inputDirectory, outputDirectory, "track1nStubs", 20, 0, 8);
  comparisonPlots("track2nStubs", "genPt>20", "genPt<5", treePath, inputDirectory, outputDirectory, "track2nStubs", 20, 0, 8);
  comparisonPlots("track3nStubs", "genPt>20", "genPt<5", treePath, inputDirectory, outputDirectory, "Track3nStubs", 20, 0, 8);
  */
  // Track time
  /*
  comparisonPlots("track1Time", "genPt>20", "genPt<5", treePath, inputDirectory, outputDirectory, "track1Time", 200, 0, 22);
  comparisonPlots("track2Time", "genPt>20", "genPt<5", treePath, inputDirectory, outputDirectory, "track2Time", 200, 0, 22);
  comparisonPlots("track3Time", "genPt>20", "genPt<5", treePath, inputDirectory, outputDirectory, "track3Time", 200, 0, 22);
  */
  // Decay mode
  comparisonPlots("l1DM", sigCut, bkgCut, treePath, inputDirectory, outputDirectory, "l1DM", 20, -1, 12);
  // Chi Squared
  /*
  comparisonPlots("track1ChiSquared", "genPt>20", "genPt<5", treePath, inputDirectory, outputDirectory, "track1ChiSquared", 300, 0, 200);
  comparisonPlots("track2ChiSquared", "genPt>20", "genPt<5", treePath, inputDirectory, outputDirectory, "track2ChiSquared", 300, 0, 200);
  comparisonPlots("track3ChiSquared", "genPt>20", "genPt<5", treePath, inputDirectory, outputDirectory, "track3ChiSquared", 300, 0, 180);
  */
  // zVTX
  comparisonPlots("zVTX", sigCut, bkgCut, treePath, inputDirectory, outputDirectory, "zVTX", 100, -17, 17);
  // Track Zs
  /*
  comparisonPlots("track1Z", "genPt>20", "genPt<5", treePath, inputDirectory, outputDirectory, "track1Z", 200, -30, 30);
  comparisonPlots("track2Z", "genPt>20", "genPt<5", treePath, inputDirectory, outputDirectory, "track2Z", 200, -30, 30);
  comparisonPlots("track3Z", "genPt>20", "genPt<5", treePath, inputDirectory, outputDirectory, "track3Z", 200, -30, 30);
  */
  // Tau L1 Strip values
  comparisonPlots("l1StripPt", sigCut, bkgCut, treePath, inputDirectory, outputDirectory, "tauL1StripPt", 150, 0, 100);
  comparisonPlots("l1StripEta", sigCut, bkgCut, treePath, inputDirectory, outputDirectory, "tauL1StripEta", 100, -2, 2);
  comparisonPlots("l1StripPhi", sigCut, bkgCut, treePath, inputDirectory, outputDirectory, "tauL1StripPhi", 100, -2, 2);
  comparisonPlots("l1StripDR", sigCut, bkgCut, treePath, inputDirectory, outputDirectory, "tauL1StripDeltaR", 100, 0, 5);
  // pfCand
  /*
  comparisonPlots("pfCand1HoE", "genPt>20", "genPt<5", treePath, inputDirectory, outputDirectory, "pfCand1HoE", 400, -11, 20);
  comparisonPlots("pfCand2HoE", "genPt>20", "genPt<5", treePath, inputDirectory, outputDirectory, "pfCand2HoE", 400, -11, 20);
  comparisonPlots("pfCand3HoE", "genPt>20", "genPt<5", treePath, inputDirectory, outputDirectory, "pfCand3HoE", 400, -11,  20);
  */
  // EG
  /*
  comparisonPlots("tauL1nEG", "genPt>20", "genPt<5", treePath, inputDirectory, outputDirectory, "tauL1nEG", 200, 0, 25);
  comparisonPlots("tauL1EGPt", "genPt>20", "genPt<5", treePath, inputDirectory, outputDirectory, "tauL1EGPt", 200, -20, 120);
  comparisonPlots("l1TauEGTime", "genPt>20", "genPt<5", treePath, inputDirectory, outputDirectory, "l1TauEGTime", 200, 0, 20);
  */
}
