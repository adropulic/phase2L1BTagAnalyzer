/****************************************************
 applyWeightFile_taus.C                                
 Stephanie Kwan + based on                        
 https://root.cern.ch/doc/v610/TMVAClassificationApplication_8C.html
 
 Usage:       
 ****************************************************/


#include <cstdlib>
#include <vector>
#include <iostream>
#include <map>
#include <string>
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TH1F.h"
#include "TStopwatch.h"
#if not defined(__CINT__) || defined(__MAKECINT__)
#include "TMVA/Tools.h"
#include "TMVA/Reader.h"
#include "TMVA/MethodCuts.h"
#endif

using namespace TMVA;

// two of category methods are implemented --> kTRUE
Bool_t UseOffsetMethod = kFALSE;

/* Use trained classifiers within an analysis module. Does not take
    an input. */
void applyWeightFile_taus()
{   
   /* Load the library */
   TMVA::Tools::Instance();

   /* Variable for declaring MVA methods to be tested */
   std::map<std::string,int> Use;
   
   Use["BDT"] = 1;
   // Use["MLP"] = 1;

   /* Create the Reader object. */
   Reader *reader = new Reader("!Color:!Silent");

   /* Create a set of variables and spectators and declare them
      to the reader. The variable names MUST corresponds in
      name and type to those given in the weight file(s)
      used.
   */

   /*   Double_t l1Pt_d, l1Eta_d, l1Phi_d, l1Time_d, l1Iso_d, l1Iso_time_d;
   Double_t track12DZ_d, track13DZ_d, track1PVDZ_d, track2PVDZ_d, track3PVDZ_d, track1nStubs_d, track2nStubs_d;
   Double_t track1Time_d, track2Time_d, track3Time_d, l1DecayMode_d;
   Double_t track1ChiSquared_d, track2ChiSquared_d, track3ChiSquared_d;
   Double_t zVTX_d, track1Z_d, track2Z_d, track3Z_d, tauL1StripPt_d, tauL1StripEta_d, tauL1StripPhi_d, tauL1StripDR_d;
   Double_t pfCand1HoE_d, pfCand2HoE_d, pfCand3HoE_d, tauL1nEG_d, tauL1EGPt_d, l1TauEGTime_d;
   Double_t genPt_d;*/
   
   Double_t l1Pt, l1Eta, l1Phi, l1Time, l1Iso, l1Iso_time;
   Double_t track12DZ, track13DZ, track1PVDZ, track2PVDZ, track3PVDZ, track1nStubs, track2nStubs;
   Double_t track1Time, track2Time, track3Time, l1DecayMode;
   Double_t track1ChiSquared, track2ChiSquared, track3ChiSquared;
   Double_t zVTX, track1Z, track2Z, track3Z, tauL1StripPt, tauL1StripEta, tauL1StripPhi, tauL1StripDR;
   Double_t pfCand1HoE, pfCand2HoE, pfCand3HoE, tauL1nEG, tauL1EGPt, l1TauEGTime;
   Double_t genPt;

   Float_t l1Pt_f, l1Eta_f, l1Phi_f, l1Time_f, l1Iso_f, l1Iso_time_f;
   Float_t track12DZ_f, track13DZ_f, track1PVDZ_f, track2PVDZ_f, track3PVDZ_f, track1nStubs_f, track2nStubs_f;
   Float_t track1Time_f, track2Time_f, track3Time_f, l1DecayMode_f;
   Float_t track1ChiSquared_f, track2ChiSquared_f, track3ChiSquared_f;
   Float_t zVTX_f, track1Z_f, track2Z_f, track3Z_f, tauL1StripPt_f, tauL1StripEta_f, tauL1StripPhi_f, tauL1StripDR_f;
   Float_t pfCand1HoE_f, pfCand2HoE_f, pfCand3HoE_f, tauL1nEG_f, tauL1EGPt_f, l1TauEGTime_f;
   Float_t genPt_f;

   reader->AddVariable("l1Pt", &l1Pt_f);   
   reader->AddVariable("l1Eta", &l1Eta_f);
   reader->AddVariable("tauL1StripPt", &tauL1StripPt_f);
   reader->AddVariable("track1ChiSquared", &track1ChiSquared_f);
   reader->AddVariable("l1DecayMode", &l1DecayMode_f);
   
   /*
   reader->AddVariable("l1Pt", &l1Pt_f);
   reader->AddVariable("l1Eta", &l1Eta_f);
   reader->AddVariable("l1Phi", &l1Phi_f);
   reader->AddVariable("l1Time", &l1Time_f);
   reader->AddVariable("l1Iso", &l1Iso_f);
   reader->AddVariable("l1Iso_time", &l1Iso_time_f);
   reader->AddVariable("track12DZ", &track12DZ_f);
   reader->AddVariable("track13DZ", &track13DZ_f);
   reader->AddVariable("track1PVDZ", &track1PVDZ_f);
   reader->AddVariable("track2PVDZ", &track2PVDZ_f);
   reader->AddVariable("track3PVDZ", &track3PVDZ_f);
   reader->AddVariable("track1nStubs", &track1nStubs_f);
   reader->AddVariable("track2nStubs", &track2nStubs_f);
   reader->AddVariable("track1Time", &track1Time_f);
   reader->AddVariable("track2Time", &track2Time_f);
   reader->AddVariable("track3Time", &track3Time_f);
   reader->AddVariable("l1DecayMode", &l1DecayMode_f);
   reader->AddVariable("track1ChiSquared", &track1ChiSquared_f);
   reader->AddVariable("track2ChiSquared", &track2ChiSquared_f);
   reader->AddVariable("track3ChiSquared", &track3ChiSquared_f);
   reader->AddVariable("zVTX", &zVTX_f);
   reader->AddVariable("track1Z", &track1Z_f);
   reader->AddVariable("track2Z", &track2Z_f);
   reader->AddVariable("track3Z", &track3Z_f);
   reader->AddVariable("tauL1StripPt", &tauL1StripPt_f);
   reader->AddVariable("tauL1StripEta", &tauL1StripEta_f);
   reader->AddVariable("tauL1StripPhi", &tauL1StripPhi_f);
   reader->AddVariable("tauL1StripDR", &tauL1StripDR_f);
   reader->AddVariable("pfCand1HoE", &pfCand1HoE_f);
   reader->AddVariable("pfCand2HoE", &pfCand2HoE_f);
   reader->AddVariable("pfCand3HoE", &pfCand3HoE_f);
   reader->AddVariable("tauL1nEG", &tauL1nEG_f);
   reader->AddVariable("tauL1EGPt", &tauL1EGPt_f);
   reader->AddVariable("l1TauEGTime", &l1TauEGTime_f);
   */

   /* Book the MVA methods. */
   for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) {
      if (it->second)
      {  TString methodName = it->first + " method";
	/* Customize the directory */
         TString weightfile = "../dataset/weights/TMVAClassification_" + TString(it->first) + ".weights.xml";
         reader->BookMVA( methodName, weightfile );
      }
   }
   
   /* Book output histograms. */
   UInt_t nbin = 100;
   std::map<std::string,TH1*> hist;
	
   hist["BDT"] = new TH1F("MVA_BDT", "MVA_BDT", nbin, -1., 1.); 
   // hist["MLP"] = new TH1F("MVA_MLP", "MVA_MLP", nbin, -1.25, 1.25);

   /* Prepare input tree. */
   TString fname = TString("../inputs/dyll-pilot.root");
   std::cout << "--- TMVAClassificationApp    : Accessing " << fname << "!" << std::endl;
   TFile *input = TFile::Open(fname);
   if (!input)
   {  std::cout << "ERROR: could not open data file: " << fname << std::endl;
      exit(1);
   }

   /********************************************/
   /* Event loop: prepare the tree             */
   /********************************************/

   TTree* inputTree = (TTree*) input->Get("L1MTDAnalyzer/L1TauTree");
   std::cout << "--- Use signal sample for evalution" << std::endl;

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
   
   std::cout << "--- Processing: " << inputTree->GetEntries() << " events" << std::endl;
   TStopwatch sw;
   sw.Start();

   for (Long64_t ievt=0; ievt < inputTree->GetEntries(); ievt++) {
      if (ievt%1000 == 0) std::cout << "--- ... Processing event: " << ievt << std::endl;
      inputTree->GetEntry(ievt);
      
      // Manually convert to floats
      l1Pt_f = float(l1Pt);
      l1Eta_f = float(l1Eta);
      l1Phi_f = float(l1Phi);
      l1Time_f = float(l1Time);
      l1Iso_f = float(l1Iso);
      l1Iso_time_f = float(l1Iso_time);
      track12DZ_f = float(track12DZ);
      track13DZ_f = float(track13DZ);
      track1PVDZ_f = float(track1PVDZ);
      track2PVDZ_f = float(track2PVDZ);
      track3PVDZ_f = float(track3PVDZ);
      track1nStubs_f = float(track1nStubs);
      track2nStubs_f = float(track2nStubs);
      track1Time_f = float(track1Time);
      track2Time_f = float(track2Time);
      track3Time_f = float(track3Time);
      l1DecayMode_f = float(l1DecayMode);
      track1ChiSquared_f = float(track1ChiSquared);
      track2ChiSquared_f = float(track2ChiSquared);
      track3ChiSquared_f = float(track3ChiSquared);
      zVTX_f = float(zVTX);
      track1Z_f = float(track1Z);
      track2Z_f = float(track2Z);
      track3Z_f = float(track3Z);
      tauL1StripPt_f = float(tauL1StripPt);
      tauL1StripEta_f = float(tauL1StripEta);
      tauL1StripPhi_f = float(tauL1StripPhi);
      tauL1StripDR_f = float(tauL1StripDR);
      pfCand1HoE_f = float(pfCand1HoE);
      pfCand2HoE_f = float(pfCand2HoE);
      pfCand3HoE_f = float(pfCand3HoE);
      tauL1nEG_f = float(tauL1nEG);
      tauL1EGPt_f = float(tauL1EGPt);
      l1TauEGTime_f = float(l1TauEGTime);
      genPt_f = float(genPt);
      

      // Return the MVA outputs and fill into histograms
      for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) {
         if (!it->second) continue;
	 // TEMPORARY: only keep genPt > 20 to see if discriminant distribution is higher
	 // if (genPt_f <= 20.0) continue;
         TString methodName = it->first + " method";
         hist[it->first]->Fill( reader->EvaluateMVA( methodName ) );
      }
   }
   sw.Stop();
   std::cout << "--- End of event loop: "; sw.Print();

   /* Write histograms */
   TFile *target  = new TFile( "outputs/appliedWeightFile_dyll-pilot.root", "RECREATE" );

   for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++)
      if (it->second) hist[it->first]->Write();

   target->Close();
   std::cout << "--- Created root file containing the MVA output histograms" << std::endl;

   delete reader;
   std::cout << "==> TMVAClassificationApplication is done!" << std::endl << std::endl;

}

int main( int argc, char** argv )
{
   applyWeightFile_taus();
   return 0;
}

