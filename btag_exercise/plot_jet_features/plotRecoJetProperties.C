/************************************************************************/
/* Plot reconstructed jet properties from a TTree                       */
/* file created by the phase2L1BTagTrigger analyzer.                    */
/*                                                                      */
/* Example usage (note escape backslashes for strings "")               */
/* % root -l "plotRecoJetProperties_cmdlineargs.C(\"path/to/            */
/*            input/file.root\", \"outputname.pdf\")"                   */
/*                                                                      */
/* Based on mlpHiggs.C in official ROOT tutorials.                      */
/************************************************************************/


/************************************************************************/

/* Plot reco-level jet properties, using .ROOT file fileName and writing
   the output plots to outputName. */

void plotRecoJetProperties(const char *fileName, const char *outputName)
{
  /* Load file with checks on whether it exists */
  TFile *input = 0;
  if (!gSystem->AccessPathName(fileName))
    input = TFile::Open(fileName);
  else {
    printf("Attempting to access file at %s", fileName);
    input = TFile::Open(Form("%s", fileName));
  }
  if (!input) return;
  
  /* Read in data  */
  TTree *raw = (TTree *) input->Get("L1BTagAnalyzer/efficiencyTree");
  
  /* New TTrees for sig and background */
  TTree *sig = new TTree("sig", "hadronFlavor == 5");
  TTree *bkg = new TTree("bkg", "hadronFlavor != 5");

  /* Declare variables to read from TTree */
  Double_t recoPt, recoEta, recoPhi, recoTk1IP, recoTk2IP, recoTk3IP,
           recoTk4IP, l1Pt, l1Eta, l1Phi;
  Int_t hadronFlavor;
  
  /* Set branch addresses  */
  raw->SetBranchAddress("recoPt",  &recoPt);
  raw->SetBranchAddress("recoEta", &recoEta);
  raw->SetBranchAddress("recoPhi", &recoPhi);
  raw->SetBranchAddress("hadronFlavor", &hadronFlavor);
  raw->SetBranchAddress("recoTk1IP", &recoTk1IP);
  raw->SetBranchAddress("recoTk2IP", &recoTk2IP);
  raw->SetBranchAddress("recoTk3IP", &recoTk3IP);
  raw->SetBranchAddress("recoTk4IP", &recoTk4IP);


  /* Set up histograms
     note the x-axis range is manually set: best to check binning and
     ranges in TBrowser / interactively in ROOT. */

  /* GStyle options (see TStyle documentation for all options) */
  gStyle->SetOptStat(0); 
  TPaveLabel *title = new TPaveLabel(0.11, 0.95, 0.35, 0.99,
				     "Reco Jet Properties", "br");
  /* Label size is percent of pad height, defaults to 0.04 and x-axis. */
  gStyle->SetLabelSize(0.08, "x"); 
  gStyle->SetLabelSize(0.08, "y");  
  /* Title size defaults to 0.02 and axis defaults to x. */
  gStyle->SetTitleSize(0.24); 	 

  /* Set opacity for signal and background histograms */
  Float_t sigAlpha = 0.7;
  Float_t bkgAlpha = 0.7;

  /* Signal histograms */
  gStyle->SetHistLineColor(kRed); 
  gStyle->SetHistFillStyle(3001);	 
  TH1D *sig_recoPt = new TH1D("sig_recoPt", "recoPt", 100, 0, 175);
  TH1D *sig_recoEta = new TH1D("sig_recoEta", "recoEta", 100, -2.7, 2.7);
  TH1D *sig_recoPhi = new TH1D("sig_recoPhi", "recoPhi", 100, -3.1, 3.1);
  TH1D *sig_recoTk1IP = new TH1D("sig_recoTk1IP", "recoTk1IP", 70, -0.1, 0.1);
  TH1D *sig_recoTk2IP = new TH1D("sig_recoTk2IP", "recoTk2IP", 70, -0.5, 0.5);
  TH1D *sig_recoTk3IP = new TH1D("sig_recoTk3IP", "recoTk3IP", 700, -0.5, 0.5);
  TH1D *sig_recoTk4IP = new TH1D("sig_recoTk4IP", "recoTk4IP", 70, -0.5, 0.5);
  /* Need to set color opacity separately for each histogram: */
  sig_recoPt->SetFillColorAlpha(kRed, sigAlpha);
  sig_recoEta->SetFillColorAlpha(kRed, sigAlpha);
  sig_recoPhi->SetFillColorAlpha(kRed, sigAlpha);
  sig_recoTk1IP->SetFillColorAlpha(kRed, sigAlpha);
  sig_recoTk2IP->SetFillColorAlpha(kRed, sigAlpha);
  sig_recoTk3IP->SetFillColorAlpha(kRed, sigAlpha);
  sig_recoTk4IP->SetFillColorAlpha(kRed, sigAlpha);
  
  /* Likewise, background histograms */
  gStyle->SetHistLineColor(kBlue); 
  gStyle->SetHistFillStyle(3001);
  TH1D *bkg_recoPt = new TH1D("bkg_recoPt", "recoPt", 100, 0, 175);
  TH1D *bkg_recoEta = new TH1D("bkg_recoEta", "recoEta", 100, -2.7, 2.7);
  TH1D *bkg_recoPhi = new TH1D("bkg_recoPhi", "recoPhi", 100, -3.1, 3.1);
  TH1D *bkg_recoTk1IP = new TH1D("bkg_recoTk1IP", "recoTk1IP", 70, -0.1, 0.1);
  TH1D *bkg_recoTk2IP = new TH1D("bkg_recoTk2IP", "recoTk2IP", 70, -0.5, 0.5);
  TH1D *bkg_recoTk3IP = new TH1D("bkg_recoTk3IP", "recoTk3IP", 70, -0.5, 0.5);
  TH1D *bkg_recoTk4IP = new TH1D("bkg_recoTk4IP", "recoTk4IP", 70, -0.5, 0.5);
  /* Likewise, need to set color opacity separately for each histogram: */
  bkg_recoPt->SetFillColorAlpha(kBlue, bkgAlpha);
  bkg_recoEta->SetFillColorAlpha(kBlue, bkgAlpha);
  bkg_recoPhi->SetFillColorAlpha(kBlue, bkgAlpha);
  bkg_recoTk1IP->SetFillColorAlpha(kBlue, bkgAlpha);
  bkg_recoTk2IP->SetFillColorAlpha(kBlue, bkgAlpha);
  bkg_recoTk3IP->SetFillColorAlpha(kBlue, bkgAlpha);
  bkg_recoTk4IP->SetFillColorAlpha(kBlue, bkgAlpha);

  /* Loop through jets and use hadronFlavor to distinguish signal and background */
  Int_t i;
  for (i = 0; i < raw->GetEntries(); i++) {
    raw->GetEntry(i);
    if (hadronFlavor == 5) { // Signal
      sig_recoPt->Fill(recoPt);
      sig_recoEta->Fill(recoEta);
      sig_recoPhi->Fill(recoPhi);
      sig_recoTk1IP->Fill(recoTk1IP);
      sig_recoTk2IP->Fill(recoTk2IP);
      sig_recoTk3IP->Fill(recoTk3IP);
      sig_recoTk4IP->Fill(recoTk4IP);
    }
    else {	// background
      bkg_recoPt ->Fill(recoPt);
      bkg_recoEta->Fill(recoEta);
      bkg_recoPhi->Fill(recoPhi);
      bkg_recoTk1IP->Fill(recoTk1IP);
      bkg_recoTk2IP->Fill(recoTk2IP);
      bkg_recoTk3IP->Fill(recoTk3IP);
      bkg_recoTk4IP->Fill(recoTk4IP);
    }
  }

  /* Plotting */

  /* Create and set up canvas for plotting sig vs. bkg variables */
  TCanvas* sb_canvas = new TCanvas("sb_canvas", "Reconstructed jet variables");
  sb_canvas->Divide(2, 4);
	
  sb_canvas->cd(1); 
  bkg_recoPt->DrawNormalized();  sig_recoPt->DrawNormalized("same");
  
  sb_canvas->cd(2); 
  bkg_recoEta->DrawNormalized(); sig_recoEta->DrawNormalized("same"); 
  
  sb_canvas->cd(3); 
  bkg_recoPhi->DrawNormalized(); sig_recoPhi->DrawNormalized("same"); 

  sb_canvas->cd(4); 
  bkg_recoTk1IP->DrawNormalized(); sig_recoTk1IP->DrawNormalized("same"); 

  sb_canvas->cd(5);
  bkg_recoTk2IP->DrawNormalized(); sig_recoTk2IP->DrawNormalized("same"); 

  sb_canvas->cd(6); 
  bkg_recoTk3IP->DrawNormalized(); sig_recoTk3IP->DrawNormalized("same"); 

  sb_canvas->cd(7); 
  bkg_recoTk4IP->DrawNormalized(); sig_recoTk4IP->DrawNormalized("same"); 

  /* Create legend and title (defined above) */
  sb_canvas->cd(8);
  TLegend *legend = new TLegend(0.2, 0.2, 0.8, 0.8, "Events");
  legend->AddEntry(sig_recoPt, "Signal (hadronFlavor == 5)");
  legend->AddEntry(bkg_recoPt, "Background (hadronFlavor != 5)");
  legend->Draw();
  
  sb_canvas->SaveAs(outputName);
  // Close file
  input->Close();
}

/************************************************************************/

/* Reads 2 arguments from the command line: a full path to a .root file
   generated by the phase2L1BTagAnalyzer and a name for the output pdf.
   Returns 0. */

int plotRecoJetProperties_cmdlineargs(const char *filePath, 
				      const char *outputName)
{
  printf("Using file at: %s \n", filePath);

  plotRecoJetProperties(filePath, outputName);
  return 0;
}
