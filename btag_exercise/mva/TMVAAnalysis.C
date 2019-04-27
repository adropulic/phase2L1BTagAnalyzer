//
// Macro for training Phase2L1BTagAnalyzer TTree using TMVA classifiers.
// TMVA is a ROOT-integrated toolkit for multivariate data analysis.
//
// Instructions:
// 1. Under the "Load data" section, make sure that dir and key together point
//    to a file that exists. The name of the .root file produced by the
//    Phase2L1BTagAnalyzer should be of the format "output_[keyword].root",
//    E.g. [keyword] is ZTT or ttbar or 1, 2, 3, ...
// 2. Run with
//        `root -l -b -q TMVAAnalysis.C`.    (add `>logRun' after .C if desired)
// 3. This will generate an output file "TMVA_output[keyword].root". Create and view desired
//    plots in the interactive TMVA GUI:
//        `root -l -e 'TMVA::TMVAGui("TMVA_output[keyword].root")'`.
//    Important: The output .eps files in `dataset/plots` will only be generated/
//               updated by creating them afresh in the interactive TMVA GUI, which
//               can be laggy. Check the Last Edited timestamp to avoid mistaking old
//               plots for new ones.
//
// Note: During Step 2, a lot of "Failed filling branch:efficiencyTree.[..]" errors
//       will pop up: these seem to be innocuous.
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

void TMVAAnalysis()
{
    //--------------------------------------------
    // Choose TMVA methods to use
    //--------------------------------------------

    // Use key-value pairs to indicate which TMVA methods we want to use
    std::map<std::string, int> Use;

    // Neural Networks (all are feed-forward Multilayer Perceptrons)
    //    Use["MLP"] = 1;  // Recommended ANN
	
	// Boosted Decision Trees
	Use["BDT"] = 1;  // uses Adaptive Boost

	//--------------------------------------------
	// Load data
	//--------------------------------------------
	TString dir = "/afs/cern.ch/work/s/skkwan/public/triggerDevel/apr2019/CMSSW_10_5_0_pre1/src/L1Trigger/phase2L1BTagAnalyzer/test/outputs/";
	TString key = "5";
	TString inputFilename = dir + "analyzer_" + key + ".root";

	// Get input file and declare output file where TMVA will store ntuples, hists, etc.
	TFile *inputFile = new TFile(inputFilename.Data());
	TString outputFilename = "TMVA_output" + key + ".root";
	TFile *outFile = new TFile(outputFilename, "RECREATE");
	
	// Get input tree
	TTree *inputTree = (TTree*) inputFile->Get("L1BTagAnalyzer/efficiencyTree");

	// Split the signal and background into two trees
	TTree *sigTree = inputTree->CloneTree(0);    // Create a clone of oldtree and copy 0 entries
	TTree *bkgTree = inputTree->CloneTree(0);

	// Declare variables to read from TTree
	Double_t recoPt, recoEta, recoPhi, recoTk1IP, recoTk2IP, recoTk3IP, recoTk4IP, l1Pt, l1Eta, l1Phi;
	Int_t hadronFlavor;
	UShort_t recoTk1IP_uint, recoTk2IP_uint, recoTk3IP_uint, recoTk4IP_uint;
	UShort_t muPt_uint, muEta_uint, muSIP2D_uint;

	// Set branch addresses
        /*
        inputTree->SetBranchAddress("recoPt",  &recoPt);
	inputTree->SetBranchAddress("recoEta", &recoEta);
	inputTree->SetBranchAddress("recoPhi", &recoPhi);
	inputTree->SetBranchAddress("hadronFlavor", &hadronFlavor);
	inputTree->SetBranchAddress("recoTk1IP", &recoTk1IP);
	inputTree->SetBranchAddress("recoTk2IP", &recoTk2IP);
	inputTree->SetBranchAddress("recoTk3IP", &recoTk3IP);
	inputTree->SetBranchAddress("recoTk4IP", &recoTk4IP);
	*/

	inputTree->SetBranchAddress("hadronFlavor", &hadronFlavor);
	inputTree->SetBranchAddress("recoTk1IP_uint", &recoTk1IP_uint);
	inputTree->SetBranchAddress("recoTk2IP_uint", &recoTk2IP_uint);
	inputTree->SetBranchAddress("recoTk3IP_uint", &recoTk3IP_uint);
	inputTree->SetBranchAddress("recoTk4IP_uint", &recoTk4IP_uint);
	inputTree->SetBranchAddress("muPt_uint",    &muPt_uint);
	inputTree->SetBranchAddress("muEta_uint",   &muEta_uint);
	inputTree->SetBranchAddress("muSIP2D_uint", &muSIP2D_uint);

	// Loop through jets and fill sigTree and bkgTree
	Int_t i;
	for ( i = 0; i < inputTree->GetEntries(); i++ ) {
	        inputTree->GetEntry(i);
		if ( hadronFlavor == 5 ) {
			sigTree->Fill();
		}
		else {
			bkgTree->Fill();
		}
	} // end jet loop

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

	dataloader->AddVariable("recoTk1IP_uint", 'I');
	dataloader->AddVariable("recoTk2IP_uint", 'I');
	dataloader->AddVariable("recoTk3IP_uint", 'I');
	dataloader->AddVariable("recoTk4IP_uint", 'I');
	dataloader->AddVariable("muPt_uint",    'I');
	dataloader->AddVariable("muEta_uint",   'I');
	dataloader->AddVariable("muSIP2D_uint", 'I');

	// You can add an arbitrary number of signal or background trees
	// Here we set the global event weights per tree to 1.0
	// It is possible to set event-wise weights (see tutorial)
	dataloader->AddSignalTree(sigTree, 1.0);
	dataloader->AddBackgroundTree(bkgTree, 1.0);

	// End of tree registration

	// Apply additional cuts on the signal and background samples
	// e.g. TCut mycuts = "abs(var1)<0.5 && abs(var2-0.5)<1";
	
	/*
	TCut signalCut     = "recoTk1IP > -99 && recoTk2IP > -99 && recoTk3IP > -99  && recoTk4IP > -99";   
	TCut backgroundCut = "recoTk1IP > -99 && recoTk2IP > -99 && recoTk3IP > -99  && recoTk4IP > -99"; 
	*/

	/*	TCut signalCut     = "recoTk1IP_uint > 0 && recoTk2IP_uint > 0 && recoTk3IP_uint > 0 && recoTk4IP_uint > 0 && muPt_uint > 0 && muEta_uint > 0 && muSIP2D_uint > 0";
	TCut backgroundCut = "recoTk1IP_uint > 0 && recoTk2IP_uint > 0 && recoTk3IP_uint > 0 && recoTk4IP_uint > 0 && muPt_uint > 0 && muEta_uint > 0 && muSIP2D_uint > 0";
	*/

	TCut signalCut = "";
	TCut backgroundCut = "";

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

	// Training and Evaluation
	factory->TrainAllMethods();
	factory->TestAllMethods();
	factory->EvaluateAllMethods();

	// Clean up
	outFile->Close();

	delete outFile;
}

