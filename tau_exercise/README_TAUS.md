# Tau README

## Getting tau n-tuples
Generate n-tuples using [the phase2L1TauAnalyzer](https://github.com/skkwan/phase2L1TauAnalyzer).

## (Aside) Plotting efficiencies for cut-based approach
This has all been moved to [the phase2L1validation GitHub](https://github.com/skkwan/phase2L1validation] which should be run from a SL7 machine. My current setup is on UWisc servers.

## Training a BDT in TMVA and recording it in a .txt table
1. Obtain an n-tuple (eventually need to set up my own analyzer).
1. Plot variables using `tau_exercise/plot_tau_features/runValidationPlots.C`. 
   * Specifically, update the input/output directories in that script, and then run
   * `root -l -b -q runValidationPlots.C`.
1. Choose which variables to train on in the MVA.
1. Train the MVA using `tau_exercise/training/TMVAAnalysis_tau_multifile.C`.
   * `root -l -b -q TMVAAnalysis_tau_multifile.C`.
1. (Optional:) To plot the distributions of the input variables (overlaying signal vs. background) in the training events, run `plot_tau_features/runTMVAInputVarsPlots.C`
1. (Outdated:) To plot a histogram of the resultant discriminant values, use `/tau_exercise/training/applyWeightFiles/applyWeightFile_taus.C`.
1. Choose binning/ which values to use for the input variables.
1. Declare the binning/values in `tau_exercise/tables/makeTable.C` and generate a text file with the table:
   * `root .x makeTable.C > log`.

## Making efficiency plots
1. In the folder `efficiency_plots`, `makeEfficienciesPlot.C` calls a few macros (`calculateEfficiency.C` to calculate eff. histograms and
   `efficienciesHist.C` for plotting) to generate plots. 
   * `root -l makeEfficienciesPlot.C` or `root -l -b -q makeEfficienciesPlot.C`

## Making ROC curves
1. We can use the ROOT file output by the TMVA training process to make ROC curves.
   In the folder `roc_curves`, the macro `extractROC.C` makes a ROC curve (True Positive Rate vs. False Positive Rate)
   and the macro `bkgRejVsSigEff.C` reproduces the TMVA background rejection vs. signal efficiency plot.
   * `root -l bkgRejVsSigEff.C` or `root -l -b -q bkgRejVsSigEff.C`

## Making rates plots
1. In the folder `rates_plots`, run the macro `makeRatesPlot.C`
   * `root -l -b -q makeRatesPlots.C`