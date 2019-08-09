#include "comparisonPlots.C"

void runValidationPlotsReprocess()
{
  gROOT->ProcessLine(".L comparisonPlots.C");
 
  TString treePath = "L1TauAnalyzer/efficiencyTree";

  /**** Method 1 for booking input/output directory ****/
  // TString inputDirectory  = "inputs/2019_Jul30-GluGluHiggsToTauTau-200PU-baby.root";
  // TString outputDirectory = "validationPlots/baby_2019_Jul30_GluGluHTauTau/baby_2019_Jul30_GluGluHTauTau_";
  /**** End of Method 1 ***/

  /**** Method 2 for booking input/output directory (see below for the assumed file/naming convention ****/
  TString description = "GluGluHiggsToTauTau";
  TString inputDirectory = ("../ntuples/" + description + ".root");
  /* Make the output directory */ 
  gSystem->Exec("mkdir validationPlots/" + description);
  TString outputDirectory = ("validationPlots/" + description + "/" + description + "_");
  /**** End of Method 2 ***/

  TString sigCut="(genPt>20)&&(l1Pt>0)";
  TString bkgCut="(genPt<5)&&(l1Pt>0)";
  comparisonPlots("l1Pt", sigCut, bkgCut, treePath, inputDirectory, outputDirectory, "l1Pt", 80, 0, 100);
  comparisonPlots("l1Eta", sigCut, bkgCut, treePath, inputDirectory, outputDirectory, "l1Eta", 50, -3, 3);
  comparisonPlots("l1Phi", sigCut, bkgCut, treePath, inputDirectory, outputDirectory, "l1Phi", 50, -4, 4);
  comparisonPlots("l1TauZ", sigCut, bkgCut, treePath, inputDirectory, outputDirectory, "l1TauZ", 80, -250, 310);
  comparisonPlots("l1PVDZ", sigCut, bkgCut, treePath, inputDirectory, outputDirectory, "l1PVDZ", 80, -250, 310);

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
  comparisonPlots("l1DM", sigCut, bkgCut, treePath, inputDirectory, outputDirectory, "l1DM", 15, -1, 12);
  // Chi Squared
  /*
  comparisonPlots("track1ChiSquared", "genPt>20", "genPt<5", treePath, inputDirectory, outputDirectory, "track1ChiSquared", 300, 0, 200);
  comparisonPlots("track2ChiSquared", "genPt>20", "genPt<5", treePath, inputDirectory, outputDirectory, "track2ChiSquared", 300, 0, 200);
  comparisonPlots("track3ChiSquared", "genPt>20", "genPt<5", treePath, inputDirectory, outputDirectory, "track3ChiSquared", 300, 0, 180);
  */
  // zVTX
  comparisonPlots("zVTX", sigCut, bkgCut, treePath, inputDirectory, outputDirectory, "zVTX", 50, -17, 17);
  // Track Zs
  /*
  comparisonPlots("track1Z", "genPt>20", "genPt<5", treePath, inputDirectory, outputDirectory, "track1Z", 200, -30, 30);
  comparisonPlots("track2Z", "genPt>20", "genPt<5", treePath, inputDirectory, outputDirectory, "track2Z", 200, -30, 30);
  comparisonPlots("track3Z", "genPt>20", "genPt<5", treePath, inputDirectory, outputDirectory, "track3Z", 200, -30, 30);
  */
  // Tau L1 Strip values
  
  comparisonPlots("l1StripPt", sigCut, bkgCut, treePath, inputDirectory, outputDirectory, "l1StripPt", 80, 0, 100);
  comparisonPlots("l1StripEta", sigCut, bkgCut, treePath, inputDirectory, outputDirectory, "l1StripEta", 60, -2, 2);
  comparisonPlots("l1StripPhi", sigCut, bkgCut, treePath, inputDirectory, outputDirectory, "l1StripPhi", 60, -2, 2);
  comparisonPlots("l1StripDR", sigCut, bkgCut, treePath, inputDirectory, outputDirectory, "l1StripDeltaR", 60, 0, 5);
  
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
  // Reco
  comparisonPlots("recoPt", sigCut, bkgCut, treePath, inputDirectory, outputDirectory, "recoPt", 80, 0, 100);
}
