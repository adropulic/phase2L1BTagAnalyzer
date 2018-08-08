// -*- C++ -*-
//
// Package:    L1Trigger/phase2L1BTagAnalyzer
// Class:      phase2L1BTagAnalyzer
//
/**\class phase2L1BTagAnalyzer phase2L1BTagAnalyzer.cc L1Trigger/phase2L1BTagAnalyzer/plugins/phase2L1BTagAnalyzer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Isobel Ojalvo
//         Created:  Wed, 18 Jul 2018 09:40:15 GMT
//
//


// system include files
#include <memory>

// Math Include
#include "TH1.h"
#include "TH2.h"
#include "TTree.h"
#include <TLorentzVector.h>
#include <memory>
#include <math.h>
#include <vector>
#include <list>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/Ref.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/L1CaloTrigger/interface/L1CaloCollections.h"

//Vertex and gen particle
#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"
#include "Geometry/CaloEventSetup/interface/CaloTopologyRecord.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/EcalAlgo/interface/EcalBarrelGeometry.h"
#include "Geometry/EcalAlgo/interface/EcalEndcapGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"

#include "DataFormats/Math/interface/deltaR.h"

#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"

#include "DataFormats/L1TrackTrigger/interface/TTTypes.h"
#include "DataFormats/L1TrackTrigger/interface/TTCluster.h"
#include "DataFormats/L1TrackTrigger/interface/TTStub.h"
#include "DataFormats/L1TrackTrigger/interface/TTTrack.h"

#include "DataFormats/BTauReco/interface/TrackIPTagInfo.h"
#include "DataFormats/BTauReco/interface/IPTagInfo.h"

// Transient Track and IP
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "TrackingTools/IPTools/interface/IPTools.h"
#include "DataFormats/GeometryVector/interface/GlobalVector.h"
#include <cmath>

//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<>
// This will improve performance in multithreaded jobs.

//using namespace l1t;
using std::vector;
using reco::TrackCollection;

class phase2L1BTagAnalyzer : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit phase2L1BTagAnalyzer(const edm::ParameterSet&);
      ~phase2L1BTagAnalyzer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

  
      // ----------member data ---------------------------
      edm::EDGetTokenT<TrackCollection> tracksToken_;  //used to select what s to read from configuration file
      edm::EDGetTokenT< std::vector< TTTrack< Ref_Phase2TrackerDigi_ > > > ttTrackToken_;
      edm::EDGetTokenT< vector<pat::Jet>  > MiniJetsToken_;
      edm::EDGetTokenT< vector<pat::Muon>  > MiniMuonsToken_;
      edm::EDGetTokenT<std::vector<pat::PackedCandidate> > PackedCands_;
      edm::EDGetTokenT<EcalEBTrigPrimDigiCollection> ecalTPGBToken_;

      edm::EDGetTokenT<reco::VertexCollection> primaryVertexToken_ ;
      const  reco::Vertex  *pv;
  //const reco::IPTagInfo * toIPTagInfo(const pat::Jet & jet, const std::string & tagInfos);
  //const reco::SVTagInfo * toSVTagInfo(const pat::Jet & jet, const std::string & tagInfos);

      edm::InputTag L1TrackInputTag;

      TTree* efficiencyTree;

      int hadronFlavor;
      double recoPt, recoEta, recoPhi; 
      double recoMuPt;
      double recoTk1IP, recoTk2IP, recoTk3IP, recoTk4IP; 
      double recoTk1IP3D;
      double muPt, muEta, muPhi, muPtRel, muDeltaR, muSIP2D, muSIP3D;
      double muRatio, muRatioRel, muSIP2Dsig, muSIP3Dsig;
      int recoFlavor;
      int run, lumi, event;
      double l1Pt, l1Eta, l1Phi;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//

/* Get as input
 * miniaod jets
 * level 1 jets
 * level 1 muons
 */
phase2L1BTagAnalyzer::phase2L1BTagAnalyzer(const edm::ParameterSet& cfg):
  tracksToken_(consumes<TrackCollection>(cfg.getUntrackedParameter<edm::InputTag>("tracks"))),
  MiniJetsToken_(   consumes< vector<pat::Jet>  >(cfg.getParameter<edm::InputTag>("slimmedJets"))),
  MiniMuonsToken_(  consumes< vector<pat::Muon> >(cfg.getParameter<edm::InputTag>("slimmedMuons"))),
  PackedCands_(     consumes< std::vector<pat::PackedCandidate> >(cfg.getParameter<edm::InputTag>("packedCandidates"))),
  primaryVertexToken_( consumes<reco::VertexCollection>(cfg.getParameter<edm::InputTag>("primaryVertices")))
  //l1TkJets_(),  //get L1 TK Jets
  //l1TkMuons_(),  //get L1 TK Muons
  //l1TkVertex()  //get L1 Vertex
{
   //now do what ever initialization is needed
  usesResource("TFileService");
  L1TrackInputTag = cfg.getParameter<edm::InputTag>("L1TrackInputTag");
  ttTrackToken_ = consumes< std::vector< TTTrack< Ref_Phase2TrackerDigi_ > > >(L1TrackInputTag);  

  edm::Service<TFileService> fs;
  efficiencyTree = fs->make<TTree>("efficiencyTree", "Reco Jet Tree");
  efficiencyTree->Branch("run",          &run,         "run/I");
  efficiencyTree->Branch("lumi",         &lumi,        "lumi/I");
  efficiencyTree->Branch("event",        &event,       "event/I");
  
  efficiencyTree->Branch("recoPt",       &recoPt,      "recoPt/D");
  efficiencyTree->Branch("recoEta",      &recoEta,     "recoEta/D");
  efficiencyTree->Branch("recoPhi",      &recoPhi,     "recoPhi/D");

  efficiencyTree->Branch("hadronFlavor", &hadronFlavor, "hadronFlavor/I");

  efficiencyTree->Branch("recoTk1IP",     &recoTk1IP,   "recoTk1IP/D");
  efficiencyTree->Branch("recoTk2IP",     &recoTk2IP,   "recoTk2IP/D");
  efficiencyTree->Branch("recoTk3IP",     &recoTk3IP,   "recoTk3IP/D");
  efficiencyTree->Branch("recoTk4IP",     &recoTk4IP,   "recoTk4IP/D");

  efficiencyTree->Branch("l1Pt",  &l1Pt,   "l1Pt/D");
  efficiencyTree->Branch("l1Eta", &l1Eta,   "l1Eta/D");
  efficiencyTree->Branch("l1Phi", &l1Phi,   "l1Phi/D");

  efficiencyTree->Branch("muPt",  &muPt,  "muPt/D");
  efficiencyTree->Branch("muEta", &muEta, "muEta/D");
  efficiencyTree->Branch("muPhi", &muPhi, "muPhi/D");

  efficiencyTree->Branch("muPtRel",    &muPtRel,    "muPtRel/D");
  efficiencyTree->Branch("muRatio",    &muRatio,    "muRatio/D");
  efficiencyTree->Branch("muRatioRel", &muRatioRel, "muRatioRel/D");
  efficiencyTree->Branch("muDeltaR",   &muDeltaR,   "muDeltaR/D");

  efficiencyTree->Branch("muSIP2D", &muSIP2D, "muSIP2D/D");
  efficiencyTree->Branch("muSIP3D", &muSIP3D, "muSIP3D/D");
  efficiencyTree->Branch("muSIP2Dsig", &muSIP2Dsig, "muSIP2Dsig/D");
  efficiencyTree->Branch("muSIP3Dsig", &muSIP3Dsig, "muSIP3Dsig/D");
}


phase2L1BTagAnalyzer::~phase2L1BTagAnalyzer()
{

   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
phase2L1BTagAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   using namespace reco;

   // Biult TransientTrackBuilder
   edm::ESHandle<TransientTrackBuilder> theTrackBuilder;
   iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder", theTrackBuilder);
   const TransientTrackBuilder* transientTrackBuilder=theTrackBuilder.product();

   typedef typename CandIPTagInfo::input_container Tracks;
   
   edm::Handle<vector<reco::Vertex> > primaryVertex;
   iEvent.getByToken(primaryVertexToken_,primaryVertex);
   bool pvFound = (primaryVertex->size() != 0);
   if ( pvFound ) {
     pv = &(*primaryVertex->begin());
   }

  run   = iEvent.id().run();
  lumi  = iEvent.id().luminosityBlock();
  event = iEvent.id().event();

  // get L1 quantities  
  // L1 tracks  
  std::vector<TTTrack< Ref_Phase2TrackerDigi_ > > l1Tracks;
  edm::Handle< std::vector< TTTrack< Ref_Phase2TrackerDigi_ > > > l1trackHandle;
  iEvent.getByToken(ttTrackToken_, l1trackHandle);
  l1Tracks.clear();

  //Find and sort the tracks
  for(size_t track_index=0; track_index<l1trackHandle->size(); ++track_index)
    {

      edm::Ptr< TTTrack< Ref_Phase2TrackerDigi_ > > ptr(l1trackHandle, track_index);
      //double pt  = ptr->getMomentum().perp();
      //double eta = ptr->getMomentum().eta();

      //only using tracks with eta less than 1.5 and pt greater than 2.5 GeV
      //if(abs(eta)<1.5 && pt > 2.5)
      l1Tracks.push_back(l1trackHandle->at(track_index));       

    }

  //sort the tracks by transverse momentum
  std::sort(l1Tracks.begin(), l1Tracks.end(), [](TTTrack< Ref_Phase2TrackerDigi_ > i,TTTrack< Ref_Phase2TrackerDigi_ > j){return(i.getMomentum().perp() > j.getMomentum().perp());});   
   
   // get reco jets
  edm::Handle< std::vector<pat::Jet> > miniJets;
  if(!iEvent.getByToken( MiniJetsToken_, miniJets))
    std::cout<<"No miniAOD jets found"<<std::endl;

   // get reco muons
  edm::Handle< std::vector<pat::Muon> > miniMuons;
  if(!iEvent.getByToken( MiniMuonsToken_, miniMuons))
    std::cout<<"No miniAOD muons found"<<std::endl;

  for( std::vector<pat::Jet>::const_iterator jet = miniJets->begin(); jet != miniJets->end(); ++jet )
    {

      //since we are starting from miniAOD it is impossible to rerun the muontaggging as one might see here:
      //https://github.com/cms-btv-pog/RecoBTag-PerformanceMeasurements/blob/9_4_X/plugins/BTagAnalyzer.cc
      //Instead we will just find the muons in the jets
      for(unsigned int id=0, nd=jet->numberOfDaughters(); id<nd; ++id) {
	//std::cout<<"id "<<id<<std::endl;
	edm::Ptr<reco::Candidate> lepPtr = jet->daughterPtr(id);
	if(std::abs(lepPtr->pdgId())!=13) continue;
	
	for(unsigned int im=0, nm= miniMuons->size(); im<nm; ++im) { // --- Begin loop on muons
          const pat::Muon patmuon= miniMuons->at(im);
	  if(patmuon.originalObjectRef()==lepPtr) {
	    reco::TrackRef trkRef( patmuon.innerTrack() );
	    reco::TrackBaseRef trkBaseRef( trkRef );
	    // Build Transient Track
	    reco::TransientTrack transientTrack=transientTrackBuilder->build(trkRef);
	    Measurement1D ip2d    = IPTools::signedTransverseImpactParameter(transientTrack, GlobalVector(jet->px(), jet->py(), jet->pz()), *pv).second;
	    // soft lepton variables go here, see:
	    // https://github.com/cms-sw/cmssw/blob/a1a699103d53ed00ae87f2a2578dd7e4b6bd0b5b/RecoBTag/SoftLepton/plugins/SoftPFMuonTagInfoProducer.cc#L121-L135
	    std::cout<<"Found the muon ip2d: "<<ip2d.value()<<std::endl;
	    break;
	  }
	}
      }

      recoEta        = -99;
      recoPhi        = -99;
      recoPt         = -99;
      recoFlavor     = -99;
      recoMuPt       = -99;
      recoTk1IP      = -99;
      recoTk2IP      = -99;
      recoTk3IP      = -99;
      recoTk4IP      = -99;
      hadronFlavor   = -99;

      recoPt  = jet->pt();
      recoEta = jet->eta();
      recoPhi = jet->phi();

      // gen level hadron flavor
      hadronFlavor = jet->hadronFlavour();
      //Uncomment only for debugging
      //for(auto string:jet->tagInfoLabels())
      //std::cout<<string<<std::endl;

      // initialize b-tag info
      // if TagInfo present
      std::string tagInfoString = "impactParameter";
      if( jet->hasTagInfo(tagInfoString) ) 
	{
	  const TrackIPTagInfo *ipTagInfo = jet->tagInfoTrackIP(tagInfoString);
	  const edm::RefVector<std::vector<reco::Track> > selectedTracks( ipTagInfo->selectedTracks() );
	  unsigned int trackSize = ipTagInfo->selectedTracks().size();

	  //find IP definition in RecoBTag/BTagTools/src/SignedTransverseImpactParameter.cc
	  if(trackSize>0)
	    {
	      recoTk1IP    = ipTagInfo->impactParameterData()[0].ip2d.value();
	      recoTk1IP3D  = ipTagInfo->impactParameterData()[0].ip3d.value();
	      //consider adding track DXY
	      //recoTk1DXY = ipTagInfo->selectedTracks().at(0).dxy(pv->position());
	    }
	  if(trackSize>1)
	    {
	      recoTk2IP = ipTagInfo->impactParameterData()[1].ip2d.value();
	    }
	  if(trackSize>2)
	    {
	      recoTk3IP = ipTagInfo->impactParameterData()[2].ip2d.value();
	    }
	  if(trackSize>3)
	    {
	      recoTk4IP = ipTagInfo->impactParameterData()[3].ip2d.value();
	    }
	}
      else
	std::cout<<"jet does not have "<<tagInfoString<<std::endl;

      // muon variables:  https://github.com/cms-btv-pog/RecoBTag-PerformanceMeasurements/blob/9_4_X/plugins/BTagAnalyzer.cc#L2595-L2607
      muPt  = -99;
      muEta = -99;
      muPhi = -99;
      muPtRel    = -99;
      muRatio    = -99;
      muRatioRel = -99;
      muDeltaR   = -99;
      muSIP2D  = -99;
      muSIP3D  = -99;
      muSIP2Dsig = -99;
      muSIP3Dsig = -99;

      tagInfoString = "softPFMuons";
      if( jet->hasTagInfo(tagInfoString) ) 
	{
	  const reco::CandSoftLeptonTagInfo *softPFMuTagInfo = jet->tagInfoCandSoftLepton(tagInfoString);
	  int nSM = (jet->hasTagInfo(tagInfoString) ? softPFMuTagInfo->leptons() : 0);
	  if( nSM > 0 )
	    {//just to note, please indent your code! (you can delete this comment when you see it)
	      muPt  = softPFMuTagInfo->lepton(0)->pt();
	      muEta = softPFMuTagInfo->lepton(0)->eta();
	      muPhi = softPFMuTagInfo->lepton(0)->phi();
	      muPtRel = (softPFMuTagInfo->properties(0).ptRel);
	      muRatio = (softPFMuTagInfo->properties(0).ratio);
	      muRatioRel = (softPFMuTagInfo->properties(0).ratioRel);
	      muDeltaR = (softPFMuTagInfo->properties(0).deltaR);
	      
	      muSIP3D = (softPFMuTagInfo->properties(0).sip3d);
	      muSIP2D = (softPFMuTagInfo->properties(0).sip2d);
	      muSIP3Dsig = (softPFMuTagInfo->properties(0).sip3dsig);
	      muSIP2Dsig = (softPFMuTagInfo->properties(0).sip2dsig);
	    }
	  //std::cout<<"muPt "<<muPt<<std::endl;
	}
      //else
      //std::cout<<"jet does not have "<<tagInfoString<<std::endl;
	
      efficiencyTree->Fill();
   }

   //Keep this for now, will be useful if we want to compare L1Tracks to Reco Tracks
   /*  
    Handle<TrackCollection> tracks;
    iEvent.getByToken(tracksToken_, tracks);
    for(TrackCollection::const_iterator itTrack = tracks->begin();
        itTrack != tracks->end();
        ++itTrack) {
      // do something with track parameters, e.g, plot the charge.
      // int charge = itTrack->charge();
    }
   */
   
}


// ------------ method called once each job just before starting event loop  ------------
void
phase2L1BTagAnalyzer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void
phase2L1BTagAnalyzer::endJob()
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
phase2L1BTagAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);

  //Specify that only 'tracks' is allowed
  //To use, remove the default given above and uncomment below
  //ParameterSetDescription desc;
  //desc.addUntracked<edm::InputTag>("tracks","ctfWithMaterialTracks");
  //descriptions.addDefault(desc);
}


  // -------------- toIPTagInfo ----------------
  /*
template<>
const phase2L1BTagAnalyzer<reco::TrackIPTagInfo,reco::Vertex>::IPTagInfo *
phase2L1BTagAnalyzer<reco::TrackIPTagInfo,reco::Vertex>::toIPTagInfo(const pat::Jet & jet, const std::string & tagInfos)
{
  return jet.tagInfoTrackIP(tagInfos.c_str());
}

template<>
const phase2L1BTagAnalyzer<reco::CandIPTagInfo,reco::VertexCompositePtrCandidate>::IPTagInfo *
phase2L1BTagAnalyzer<reco::CandIPTagInfo,reco::VertexCompositePtrCandidate>::toIPTagInfo(const pat::Jet & jet, const std::string & tagInfos)
{
    return jet.tagInfoCandIP(tagInfos.c_str());
}
  */
//define this as a plug-in
DEFINE_FWK_MODULE(phase2L1BTagAnalyzer);
