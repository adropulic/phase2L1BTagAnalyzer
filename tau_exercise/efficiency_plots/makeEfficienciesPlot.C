/*******************************************************************/
/* makeEfficienciesPlot.C                                          */
/* ROOT macro                                                      */
/*******************************************************************/

#include "calculateEfficiency.C"



/*******************************************************************/

void makeEfficienciesPlot(void)
{
  gROOT->ProcessLine(".L calculateEfficiency.C");

  /* Load the TTree. */
  TString treePath = "L1TauAnalyzer/efficiencyTree";
  TString inputDirectory  = "../ntuples/GluGluHiggsToTauTau.root";
  TString outputDirectory = "";

  float eff = calculateEfficiency(treePath, inputDirectory, 20, 20, 20, 2.5, 0.2);
  printf("efficiency: %f", eff);


}
