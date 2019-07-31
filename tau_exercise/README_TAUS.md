# Tau README

## Inputs
Currently using a Drell-Yan MC file located at `/afs/cern.ch/work/s/skkwan/public/triggerDevel/apr2019/CMSSW_10_5_0_pre1/src/L1Trigger/phase2L1BTagAnalyzer/tau_exercise/training/inputs/dyll.root` for training. Still working on setting up Isobel's analyzer for generating tau n-tuples.

## (Aside) Plotting efficiencies for cut-based approach
This has all been moved to [https://github.com/skkwan/phase2L1validation](the phase2L1validation GitHub), which should be run from a SL7 machine. My current setup is on UW Wisc servers.

## Training a BDT in TMVA and recording it in a LUT
1. Obtain an n-tuple (eventually need to set up my own analyzer).
1. Plot variables using `tau_exercise/plot_tau_features/runValidationPlots.C`. 
   * Specifically, update the input/output directories in that script, and then run
   * `root -l -b -q runValidationPlots.C`.
1. Choose which variables to train on in the MVA.
1. Train the MVA using `tau_exercise/training/TMVAAnalysis_tau.C`.
   * Specifically, update the input/output file names there and specify which
     variables to train on,
   * `root -l -b -q TMVAAnalysis_tau.C`.
1. (Optional) to plot a histogram of the resultant discriminant values, use `/tau_exercise/training/applyWeightFiles/applyWeightFile_taus.C`.
1. Choose binning/ which values to use for the input variables.
1. Declare the binning/values in `tau_exercise/tables/makeTable.C` and generate a text file of the LUT (sort of):
   * `root .x makeTable.C > log`.