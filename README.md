# Summary
This in-development analyzer and associated plotting scripts are for developing tau and b-jet Level-1 Triggers for the CMS Experiment's Run 3 and Phase 2. 

# Installation
Installation instructions are in `checkout.sh`.

## B-tagging
### Running the analyzer
To run the Analyzer:

```
cd $CMSSW_BASE/src/L1Trigger/phase2L1BTagAnalyzer/test/
cmsenv
voms-proxy-init --rfc --voms cms  # provide GRID password
cmsRun test-Analyzer.py #this will produce an analyzer.root
```

Alternatively, you can have it run as a background process so it conntinues after logging out, and redirect the terminal output to a logRun file:
```
nohup cmsRun test-Analyzer.py > logRun &
```

The .root file is written to the same directory (usually called `analyzer_X.root`). Rename it and save it to a folder (e.g. `outputs/`) and update a `README` in the folder that describes which miniAOD/FEVT files were used to generate it. 

### Plotting reconstructed jet properties
To plot histograms of the reco-level jet pT, Eta, Phi, and (up to 4) track impact parameters, with b-jets in red and light-flavor jets in blue, use `plotRecoJetProperties.C` macro with command line arguments to make a .pdf.
```
cd $CMSSW_BASE/src/L1Trigger/phase2L1BTagAnalyzer/btag_exercise/plot_jet_features/
root -l "plotRecoJetProperties.C(\"path/to/input/file.root\", \"outputname.pdf\")"	

```

### Training ROOT TMVA classifiers
To train TMVA classifiers (BDTs, ANNs, etc.) using the reconstructed jet properties, use the ROOT macro `TMVAAnalysis.C`. First navigate to the directory:
```
cd $CMSSW_BASE/src/L1Trigger/phase2L1BTagAnalyzer/btag_exercise/mva
```
1. Update the input file path in the "Load data" section of `TMVAAnalysis.C`.
2. Run the macro:
```
root -l -b -q TMVAAnalysis.C
```
   - Add `> logRun` to the end to pipe the terminal output to `logRun`.
3. This will generate an output file "TMVA_output<keyword>.root" in the same directory, whose contents can be viewed and plotted with the interactive TMVA GUI. 
```
root -l -e 'TMVA::TMVAGui("TMVA_output<keyword>.root")'
```
   
### Applying the TMVA classifiers to other files
To apply the standalone classifiers to other .ROOT samples, set the input and output file paths in `applyWeightFile.C` and run:
```
root -l -b -q applyWeightFile.C
```

## Tau tagging
### Workflow
This repository does not include the tau analyzer, so the workflow for training TMVA classifiers on tau ttrees is:
1. In `tau_exercise`, set input and output file paths in `TMVAAnalysis_tau.C`. Run the macro to generate an output file (replace X with whatever identifier the input file has):
```
root -l -b -q TMVAAnalysis_tau.C > files/trainingOutputs/TMVA_taus_logX 
```
Create and view desired plots in the interactive TMVA GUI:
```
root -l -e 'TMVA::TMVAGui("out_[keyword].root")
```
The output .eps files in `dataset/plots` will only be generated/ updated by creating them afresh in the interactive TMV GUI, which can be laggy. Check the Last Edited timestamp to avoid mistaking old plots for new ones. 
 
2. In `/applyWeightFiles`, set the input tree path and output file name.
```
root -l -b -q applyWeightFile_taus.C
```
