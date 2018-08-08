# Summary
This in-development analyzer and associated plotting scripts are for analyzing b-jet properties at the Level-1 Trigger in Phase 2 of the CMS Experiment.

# Installation
Installation instructions are in `checkout.sh`.

# Running the Analyzer
To run the Analyzer:

`cd $CMSSW_BASE/src/L1Trigger/phase2L1BTagAnalyzer/test/
cmsenv
voms-proxy-init -voms cms #provide GRID password
cmsRun test-Analyzer.py #this will produce an analyzer.root`

Alternatively, you can ensure that it keeps running after logging out and write the terminal output to a logRun file:
`
nohup cmsRun test-Analyzer.py > logRun &
`

The output file should have the name `analyzer_<keyword>.root`, where <keyword> describes the type of events in the input files, e.g. ttbar.

# Plotting reconstructed jet properties
To visualize the features of the reconstructed jets, use the `plotRecoJetProperties.C` macro to generate a .pdf. Make sure to update the input file path and the output .pdf name (near the end of the script).
`
cd $CMSSW_BASE/src/L1Trigger/phase2L1BTagAnalyzer/btag_exercise/plot_jet_features/
root -l plotRecoJetProperties.C
`

# ROOT TMVA analysis
To train various TMVA classifiers (BDTs, ANNs, etc.) using the reconstructed jet properties, use the ROOT macro `TMVAAnalysis.C`. First navigate to the directory:
`
cd $CMSSW_BASE/src/L1Trigger/phase2L1BTagAnalyzer/btag_exercise/mva
`
1. Update the input file path and the keyword under the "Load data" section of `TMVAAnalysis.C`. Here it is extremely helpful if the output file from the Analyzer is of the form `analyzer_<keyword>.root`.
2. Run the macro:
`
root -l -b -q TMVAAnalysis.C
`
   - Add `> logRun` to the end to save the terminal output to `logRun`.
   - Note: A lot of "Failed filling branch:efficiencyTree" errors will appear; these seem to be harmless and are probably because ROOT thinks that it should edit the input file.
3. This will generate an output file "out_<keyword>.root" in the same directory, whose contents can be viewed and plotted with the interactive TMVA GUI. 
`
root -l -e 'TMVA::TMVAGui("out_<keyword>.root")'
`

