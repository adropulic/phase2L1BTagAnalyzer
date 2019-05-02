//
// Macro for training Phase2L1BTagAnalyzer TTree using TMVA classifiers.
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
	TString dir = "/afs/cern.ch/work/s/skkwan/public/triggerDevel/feb2019/CMSSW_10_5_0_pre1/src/L1Trigger/phase2L1BTagAnalyzer/tau_exercise/files/";
	TString file = "dyll";
	TString inputFilename = dir + file + ".root";

	// Get input file and declare output file where TMVA will store ntuples, hists, etc.
	TFile *inputFile = new TFile(inputFilename.Data());
	TString outputFilename = "TMVA_training_taus_out_" + file + ".root";
	TFile *outFile = new TFile(outputFilename, "RECREATE");
	
	// Get input tree
	TTree *inputTree = (TTree*) inputFile->Get("L1MTDAnalyzer/L1TauTree");

	// Split the signal and background into two trees
	TTree *sigTree = inputTree->CloneTree(0);    // Create a clone of oldtree and copy 0 entries
	TTree *bkgTree = inputTree->CloneTree(0);

	// Declare variables to read from TTree
	Double_t l1Pt, l1Eta, l1Phi, l1Time, l1Iso, l1Iso_time;
        Double_t track12DZ, track13DZ, track1PVDZ, track2PVDZ, track3PVDZ, track1nStubs, track2nStubs;
	Double_t track1Time, track2Time, track3Time, l1DecayMode;
        Double_t track1ChiSquared, track2ChiSquared, track3ChiSquared;
	Double_t zVTX, track1Z, track2Z, track3Z, tauL1StripPt, tauL1StripEta, tauL1StripPhi, tauL1StripDR;
	Double_t pfCand1HoE, pfCand2HoE, pfCand3HoE, tauL1nEG, tauL1EGPt, l1TauEGTime;
	Double_t genPt;

	// Set branch addresses
	inputTree->SetBranchAddress("genPt", &genPt);

	inputTree->SetBranchAddress("l1Pt", &l1Pt);
	inputTree->SetBranchAddress("l1Eta", &l1Eta);
	inputTree->SetBranchAddress("l1Phi", &l1Phi);
	inputTree->SetBranchAddress("l1Time", &l1Time);
	inputTree->SetBranchAddress("l1Iso", &l1Iso);
	inputTree->SetBranchAddress("l1Iso_time", &l1Iso_time);
	inputTree->SetBranchAddress("track12DZ", &track12DZ);
	inputTree->SetBranchAddress("track13DZ", &track13DZ);
	inputTree->SetBranchAddress("track1PVDZ", &track1PVDZ);
	inputTree->SetBranchAddress("track2PVDZ", &track2PVDZ);
	inputTree->SetBranchAddress("track3PVDZ", &track3PVDZ);
	inputTree->SetBranchAddress("track1nStubs", &track1nStubs);
	inputTree->SetBranchAddress("track2nStubs", &track2nStubs);
	inputTree->SetBranchAddress("track1Time", &track1Time);
	inputTree->SetBranchAddress("track2Time", &track2Time);
	inputTree->SetBranchAddress("track3Time", &track3Time);
	inputTree->SetBranchAddress("l1DecayMode", &l1DecayMode);
	inputTree->SetBranchAddress("track1ChiSquared", &track1ChiSquared);
	inputTree->SetBranchAddress("track2ChiSquared", &track2ChiSquared);
	inputTree->SetBranchAddress("track3ChiSquared", &track3ChiSquared);
	inputTree->SetBranchAddress("zVTX", &zVTX);
	inputTree->SetBranchAddress("track1Z", &track1Z);
	inputTree->SetBranchAddress("track2Z", &track2Z);
	inputTree->SetBranchAddress("track3Z", &track3Z);
	inputTree->SetBranchAddress("tauL1StripPt", &tauL1StripPt);
	inputTree->SetBranchAddress("tauL1StripEta", &tauL1StripEta);
	inputTree->SetBranchAddress("tauL1StripPhi", &tauL1StripPhi);
	inputTree->SetBranchAddress("tauL1StripDR", &tauL1StripDR);
	inputTree->SetBranchAddress("pfCand1HoE", &pfCand1HoE);
	inputTree->SetBranchAddress("pfCand2HoE", &pfCand2HoE);
	inputTree->SetBranchAddress("pfCand3HoE", &pfCand3HoE);
	inputTree->SetBranchAddress("tauL1nEG", &tauL1nEG);
	inputTree->SetBranchAddress("tauL1EGPt", &tauL1EGPt);
	inputTree->SetBranchAddress("l1TauEGTime", &l1TauEGTime);

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

	dataloader->AddVariable("l1Pt", 'D');
	dataloader->AddVariable("l1Eta", 'D');
	dataloader->AddVariable("l1Phi", 'D');
	dataloader->AddVariable("l1Time", 'D');
	dataloader->AddVariable("l1Iso", 'D');
	dataloader->AddVariable("l1Iso_time", 'D');
	dataloader->AddVariable("track12DZ", 'D');
	dataloader->AddVariable("track13DZ", 'D');
	dataloader->AddVariable("track1PVDZ", 'D');
	dataloader->AddVariable("track2PVDZ", 'D');
	dataloader->AddVariable("track3PVDZ", 'D');
	dataloader->AddVariable("track1nStubs", 'D');
	dataloader->AddVariable("track2nStubs", 'D');
	dataloader->AddVariable("track1Time", 'D');
	dataloader->AddVariable("track2Time", 'D');
	dataloader->AddVariable("track3Time", 'D');
	dataloader->AddVariable("l1DecayMode", 'D');
	dataloader->AddVariable("track1ChiSquared", 'D');
	dataloader->AddVariable("track2ChiSquared", 'D');
	dataloader->AddVariable("track3ChiSquared", 'D');
	dataloader->AddVariable("zVTX", 'D');
	dataloader->AddVariable("track1Z", 'D');
	dataloader->AddVariable("track2Z", 'D');
	dataloader->AddVariable("track3Z", 'D');
	dataloader->AddVariable("tauL1StripPt", 'D');
	dataloader->AddVariable("tauL1StripEta", 'D');
	dataloader->AddVariable("tauL1StripPhi", 'D');
	dataloader->AddVariable("tauL1StripDR", 'D');
	dataloader->AddVariable("pfCand1HoE", 'D');
	dataloader->AddVariable("pfCand2HoE", 'D');
	dataloader->AddVariable("pfCand3HoE", 'D');
	dataloader->AddVariable("tauL1nEG", 'D');
	dataloader->AddVariable("tauL1EGPt", 'D');
	dataloader->AddVariable("l1TauEGTime", 'D');

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
        
	TCut signalCut     = "pfCand2HoE < 20 && pfCand3HoE < 20 && l1TauEGTime < 30 ";
	TCut backgroundCut = "pfCand2HoE < 20 && pfCand3HoE < 20 && l1TauEGTime < 30";

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

