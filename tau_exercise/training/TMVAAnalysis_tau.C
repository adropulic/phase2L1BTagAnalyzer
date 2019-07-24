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

void TMVAAnalysis_tau()
{
    //--------------------------------------------
    // Choose TMVA methods to use
    //--------------------------------------------

    // Use key-value pairs to indicate which TMVA methods we want to use
    std::map<std::string, int> Use;

    // Neural Networks (all are feed-forward Multilayer Perceptrons)
    // Use["MLP"] = 1;  // Recommended ANN
	
    // Boosted Decision Trees
    Use["BDT"] = 1;  // uses Adaptive Boost

    //--------------------------------------------
    // Load data
    //--------------------------------------------
    TString dir = "inputs/";
    TString file = "analyzer-dyll-4FEVT";
    TString inputFilename = dir + file + ".root";

	// Get input file and declare output file where TMVA will store ntuples, hists, etc.
	TFile *inputFile = new TFile(inputFilename.Data());
	TString outputFilename = "TMVA_training_taus_out_4FEVT" + file + ".root";
	TFile *outFile = new TFile(outputFilename, "RECREATE");
	
	// Get input tree
	TTree *inputTree = (TTree*) inputFile->Get("L1TauAnalyzer/efficiencyTree");

	// Split the signal and background into two trees
	TTree *sigTree = inputTree->CloneTree(0);    // Create a clone of oldtree and copy 0 entries
	TTree *bkgTree = inputTree->CloneTree(0);

	// Declare variables to read from TTree
	Double_t l1Pt, l1Eta, l1Phi;
	Double_t l1DM;
	Double_t zVTX, l1TauZ, l1PVDZ;
	Double_t l1StripPt, l1StripEta, l1StripPhi, l1StripDR;
	Double_t genPt;

	// Set branch addresses
	inputTree->SetBranchAddress("genPt", &genPt);

	inputTree->SetBranchAddress("l1Pt", &l1Pt);
	inputTree->SetBranchAddress("l1Eta", &l1Eta);
	inputTree->SetBranchAddress("l1Phi", &l1Phi);
	inputTree->SetBranchAddress("l1DM", &l1DM);
	inputTree->SetBranchAddress("zVTX", &zVTX);
	inputTree->SetBranchAddress("l1TauZ", &l1TauZ);
	inputTree->SetBranchAddress("l1PVDZ", &l1PVDZ);

	inputTree->SetBranchAddress("l1StripPt", &l1StripPt);
	inputTree->SetBranchAddress("l1StripEta", &l1StripEta);
	inputTree->SetBranchAddress("l1StripPhi", &l1StripPhi);
	inputTree->SetBranchAddress("l1StripDR", &l1StripDR);

	inputTree->SetBranchAddress("genPt", &genPt);

	// Loop through taus and fill sigTree and bkgTree
	Int_t i;
	for ( i = 0; i < inputTree->GetEntries(); i++ ) {
	        inputTree->GetEntry(i);
		
		if ( genPt > 20 ) {
			sigTree->Fill();
		}
		else {
			bkgTree->Fill();
		}
	} 

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

	// Top five variables are:
	dataloader->AddVariable("l1Pt", 'D');
	dataloader->AddVariable("l1Eta", 'D');
	dataloader->AddVariable("l1StripPt", 'D');
	dataloader->AddVariable("l1DM", 'D');
	dataloader->AddVariable("l1PVDZ", 'D');

	// You can add an arbitrary number of signal or background trees
	// Here we set the global event weights per tree to 1.0
	// It is possible to set event-wise weights (see tutorial)
	dataloader->AddSignalTree(sigTree, 1.0);
	dataloader->AddBackgroundTree(bkgTree, 1.0);

	// End of tree registration

	// Apply additional cuts on the signal and background samples
	// e.g. TCut mycuts = "abs(var1)<0.5 && abs(var2-0.5)<1";
	// Commenting out the below, which were cuts used for b jets. 

	TCut signalCut     = "(l1Pt > 0) && \
                              !isinf(l1Pt) && !isinf(l1Eta) && !isinf(l1StripPt) && !isinf(l1DM) && !isinf(l1PVDZ) &&\
                              !isnan(l1Pt) && !isnan(l1Eta) && !isnan(l1StripPt) && !isnan(l1DM) && !isnan(l1PVDZ)";
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

