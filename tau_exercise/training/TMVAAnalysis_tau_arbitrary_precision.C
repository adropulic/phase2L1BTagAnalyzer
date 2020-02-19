//
// Macro for training tau TTrees using TMVA classifiers.
// TMVA is a ROOT-integrated toolkit for multivariate data analysis.
//
// Instructions:
// 1. Under the "Load data" section, make sure that dir and key together point
//    to a file that exists.
// 2. Run with
//        `root -l -b -q TMVAAnalysis_tau.C`.    (add `>logRun' after .C if desired)
// 3. This will generate an output file "out_[keyword].root". Create and view desired
//    plots in the interactive TMVA GUI:
//        `root -l -e 'TMVA::TMVAGui("out_[keyword].root")'`.
//    Important: The output .eps files in `dataset/plots` will only be generated/
//               updated by creating them afresh in the interactive TMVA GUI, which
//               can be laggy. Check the Last Edited timestamp to avoid mistaking old
//               plots for new ones.
// 
// Based on ROOT tutorials/tmva/TMVAClassification.C and TMVAMinimalClassification.C
// by Andreas Hoecker and Kim Albertsson respectively, as well as
// tutorials/tree/copytree3.C for the tree splitting.

#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include <assert.h>

#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TObjString.h"
#include "TSystem.h"
#include "TROOT.h"

#include "TMVA/Factory.h"
#include "TMVA/DataLoader.h"
#include "TMVA/Tools.h"
#include "TMVA/TMVAGui.h"

#include <math.h>

#include "helperFuncsTraining/fillSigBkgTrees_uints.C"

void TMVAAnalysis_tau_arbitrary_precision()
{
    //--------------------------------------------
    // Choose TMVA methods to use & user options
    //--------------------------------------------

    // Use key-value pairs to indicate which TMVA methods we want to use
    std::map<std::string, int> Use;

    // Neural Networks (all are feed-forward Multilayer Perceptrons)
    // Use["MLP"] = 1;  // Recommended ANN
	
    // Boosted Decision Trees
    Use["BDT"] = 1;  // uses Adaptive Boost

    //--------------------------------------------
    // Load data: TMVA requires us to declare all TFiles before declaring any TTrees
    //--------------------------------------------

    // Input file
    TString inputPath = "../ntuples/deltaZ_2cm_PF/2019_Oct15-DYToLL_200PU_Z2.root";
    TFile *inputFile  = TFile::Open(inputPath);

    // Output file
    TString outputFilename = "TMVA_training_out_deltaZ_2cm_PF_UINTS.root";
    TFile *outFile = new TFile(outputFilename, "RECREATE");

    // Input tree
    TString tree = "L1TauAnalyzer/efficiencyTree";
    TTree *inputTree = (TTree*) inputFile->Get(tree);

    // Only do this once!
    TTree* sigTree = new TTree("sigTree", "sigTree");
    
    // For uint, we need to create new branches
    ULong64_t l1Pt_u, l1Eta_u;
    /*    uint64_t l1DM_u;
    uint64_t l1TauZ_u, l1PVDZ_u;
    uint64_t l1StripPt_u;
    uint64_t l1HoE_u, l1EoH_u, l1ChargedIso_u;*/
    sigTree->Branch("l1Pt_u", &l1Pt_u, "l1Pt_u/l");
    sigTree->Branch("l1Eta_u", &l1Eta_u, "l1Eta_u/l");

    TTree* bkgTree = sigTree->CloneTree(0);

    // First input
    fillSigBkgTrees_uints(inputTree, true, true, sigTree, bkgTree);

    // Sort second input
    inputPath = "../ntuples/deltaZ_2cm_PF/2019_Oct15-ggHToTauTau_200PU_Z2.root";
    inputFile = TFile::Open(inputPath);
    tree = "L1TauAnalyzer/efficiencyTree";
    inputTree = (TTree*) inputFile->Get(tree);
    fillSigBkgTrees_uints(inputTree, true, true, sigTree, bkgTree);
    
    // Entirely-background TTree: note that the first Boolean ("getSig") is set to False
    inputPath = "../ntuples/deltaZ_2cm_PF/SingleNeutrino-200PU-2019_Oct15_Z2.root";
    inputFile = TFile::Open(inputPath);
    tree = "L1TauAnalyzerRates/efficiencyTree";
    inputTree = (TTree*) inputFile->Get(tree);
    fillSigBkgTrees_uints(inputTree, false, true, sigTree, bkgTree);

    //--------------------------------------------
    // Set up TMVA
    //--------------------------------------------	
    
    // Create the factory object. Later we can choose the methods whose
    // performance we'd like to investigate. The factory is the only 
    // TMVA object we have to interact with.
    //
    // The first argument is the base of the name of all the weightfiles
    // in the directory weight/
    // 
    // The second argument is the output file for the training results.
    TString factoryOptions = "AnalysisType=Classification";
    
    TMVA::Factory *factory = new TMVA::Factory("TMVAClassification",
					       outFile,
					       factoryOptions);
    
    TMVA::DataLoader *dataloader = new TMVA::DataLoader("dataset");
    
    // Define the input variables that will be used for the MVA training
    // Note that you may also use variable expressions, such as: 
    // "3*var1/var2*abs(var3)". [All types of expressions that can also be
    // parsed by TTree::Draw( "expression" )]
    
    dataloader->AddVariable("l1Pt_u", 'I');
    dataloader->AddVariable("l1Eta_u", 'I');
    /*    dataloader->AddVariable("l1StripPt", 'I');
    dataloader->AddVariable("l1DM", 'I');
    dataloader->AddVariable("l1PVDZ", 'I');
    dataloader->AddVariable("l1HoE", 'I');
    dataloader->AddVariable("l1EoH", 'I');
    dataloader->AddVariable("l1TauZ", 'I');
    dataloader->AddVariable("l1ChargedIso", 'I');
    */
    // You can add an arbitrary number of signal or background trees
    // Here we set the global event weights per tree to 1.0
    // It is possible to set event-wise weights (see tutorial)

    dataloader->AddSignalTree(sigTree, 1.0);
    dataloader->AddBackgroundTree(bkgTree, 1.0);
    printf("Done adding signal and background trees.\n");

    // End of tree registration
    
    // Apply additional cuts on the signal and background samples
    // e.g. TCut mycuts = "abs(var1)<0.5 && abs(var2-0.5)<1";

    // Removing l1Pt > 0 requirement for now.
    // Removing l1StripPt < 400 reqiurement for uint version
    /*    TCut signalCut     = "!isinf(l1TauZ) && !isnan(l1TauZ) &&	\
                          !isinf(l1EoH) && !isinf(l1HoE) && !isnan(l1EoH) && !isnan(l1HoE) &&\
                          !isinf(l1Pt) && !isinf(l1Eta) && !isinf(l1StripPt) && !isinf(l1DM) && !isinf(l1PVDZ) &&\
                          !isnan(l1Pt) && !isnan(l1Eta) && !isnan(l1StripPt) && !isnan(l1DM) && !isnan(l1PVDZ) &&\
                          !isinf(l1ChargedIso) && !isnan(l1ChargedIso)";
    */
    TCut signalCut     = "!isinf(l1Pt_u) && !isnan(l1Pt_u) && \
                          !isinf(l1Eta_u) && !isnan(l1Eta_u)";
    TCut backgroundCut = signalCut;
   
    TString datasetOptions = "SplitMode=Random";
    dataloader->PrepareTrainingAndTestTree(signalCut, backgroundCut, datasetOptions);
    
    // Method specification
    TString methodOptions = "";
    //  Adaptive Boost
    if (Use["BDT"])
      factory->BookMethod(dataloader, TMVA::Types::kBDT, "BDT", methodOptions);
    //  TMVA ANN: MLP (recommended ANN) -- all ANNs in TMVA are Multilayer Perceptrons
    if (Use["MLP"])
      factory->BookMethod(dataloader, TMVA::Types::kMLP, "MLP", methodOptions); 
    
    // Cross-validation
    TMVA::CrossValidation cv(dataloader);
    cv.BookMethod(TMVA::Types::kBDT, "BDT", "!H:!V:");
    cv.Evaluate();
    auto results = cv.GetResults();
    for (auto r : results)
      r.Print();
    
    // Training and Evaluation
    factory->TrainAllMethods();
    factory->TestAllMethods();
    factory->EvaluateAllMethods();
    
    // Clean up
    outFile->Close();

    delete outFile;
}

