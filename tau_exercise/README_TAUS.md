# Tau README

Next steps:
1. Re-train BDT in ROOTTMVA with five highest-significance variables.
1. Decide on appropriate bins for these variables.
1. Generate an array in C++ with all possible combinations of the bins, evaluate the BDT for each variables' values, and save the discriminant (converted from float  to integer) into another column in the array.
1. Decide on a series of bitwise operations that can transform the variable values/ranges into x bits (decide what x is) that can be interpreted as a memory address.

## Plotting tau variables
Use `tau_exercise/plot_tau_features/runValidationPlots.C`.

## Cut-based approach efficiencies
This has all been moved to [https://github.com/skkwan/phase2L1validation](the phase2L1validation GitHub), which should be run from a SL7 machine. My current setup is on UW Wisc servers.

## Workflow
1. Obtain an n-tuple (eventually need to set up my own analyzer).
1. Plot variables using `tau_exercise/plot_tau_features/runValidationPlots.C`. 
   * Specifically, update the input/output directories in that script,
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