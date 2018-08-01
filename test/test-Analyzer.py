import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras

process = cms.Process('L1',eras.Phase2_trigger)

#from PhysicsTools.PatAlgos.patTemplate_cfg import *
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.Geometry.GeometryExtended2023D17Reco_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.SimL1Emulator_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('L1Trigger.TrackFindingTracklet.L1TrackletTracks_cff')

process.load("RecoBTag.Configuration.RecoBTag_cff") # this loads all available b-taggers

## Load the MessageLogger
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1

## Events to process
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

## Options and Output Report
process.options   = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)

## Input files
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
       #'file:mini.root'
       #### Uncomment the following file for Z->TauTau 
       #'root://cmsxrootd.fnal.gov///store/relval/CMSSW_9_3_7/RelValZTT_14TeV/MINIAODSIM/93X_upgrade2023_realistic_v5_2023D17noPU-v2/10000/C033607A-8E2C-E811-B4EF-0CC47A78A478.root'
       #### Uncomment the following file for Z->TTbar
        '/store/relval/CMSSW_9_3_7/RelValTTbar_14TeV/MINIAODSIM/93X_upgrade2023_realistic_v5_2023D17noPU-v2/10000/8CB1DA1F-A22C-E811-9917-0025905B85FE.root'
    ),
    secondaryFileNames = cms.untracked.vstring(
       # 'file:raw.root'
       # 'file:reco.root'
        # #### Z->TauTau samples
        # 'root://cmsxrootd.fnal.gov///store/relval/CMSSW_9_3_7/RelValZTT_14TeV/GEN-SIM-DIGI-RAW/93X_upgrade2023_realistic_v5_2023D17noPU-v2/10000/02306B8E-6D2C-E811-9625-0025905B858C.root',
        # 'root://cmsxrootd.fnal.gov///store/relval/CMSSW_9_3_7/RelValZTT_14TeV/GEN-SIM-DIGI-RAW/93X_upgrade2023_realistic_v5_2023D17noPU-v2/10000/163A5C8E-6D2C-E811-B990-0025905B85B2.root',
        # 'root://cmsxrootd.fnal.gov///store/relval/CMSSW_9_3_7/RelValZTT_14TeV/GEN-SIM-DIGI-RAW/93X_upgrade2023_realistic_v5_2023D17noPU-v2/10000/1A7E6D8F-6D2C-E811-8DFB-0025905B85BE.root',
        # 'root://cmsxrootd.fnal.gov///store/relval/CMSSW_9_3_7/RelValZTT_14TeV/GEN-SIM-DIGI-RAW/93X_upgrade2023_realistic_v5_2023D17noPU-v2/10000/5864268A-6D2C-E811-90F5-0025905A60EE.root',
        # 'root://cmsxrootd.fnal.gov///store/relval/CMSSW_9_3_7/RelValZTT_14TeV/GEN-SIM-DIGI-RAW/93X_upgrade2023_realistic_v5_2023D17noPU-v2/10000/627E087F-6D2C-E811-8F81-0CC47A4C8E34.root',
        # 'root://cmsxrootd.fnal.gov///store/relval/CMSSW_9_3_7/RelValZTT_14TeV/GEN-SIM-DIGI-RAW/93X_upgrade2023_realistic_v5_2023D17noPU-v2/10000/64AE2B7F-6D2C-E811-800E-0CC47A4D76A0.root',
        # 'root://cmsxrootd.fnal.gov///store/relval/CMSSW_9_3_7/RelValZTT_14TeV/GEN-SIM-DIGI-RAW/93X_upgrade2023_realistic_v5_2023D17noPU-v2/10000/820C0278-6D2C-E811-B95B-0CC47A4C8F0C.root',
        # 'root://cmsxrootd.fnal.gov///store/relval/CMSSW_9_3_7/RelValZTT_14TeV/GEN-SIM-DIGI-RAW/93X_upgrade2023_realistic_v5_2023D17noPU-v2/10000/BE6A3C80-6D2C-E811-9143-0CC47A4D7668.root',
        # 'root://cmsxrootd.fnal.gov///store/relval/CMSSW_9_3_7/RelValZTT_14TeV/GEN-SIM-DIGI-RAW/93X_upgrade2023_realistic_v5_2023D17noPU-v2/10000/DA7C408A-6D2C-E811-9BFF-0025905B858A.root'
        # #### Z->T-ttbar samples
        '/store/relval/CMSSW_9_3_7/relvalTTbar_14TeV/GEN-SIM-DIGI-RAW/93X_upgrade2023_realistic_v5_2023D17noPU-v2/10000/EEFED097-752C-E811-B621-0CC47A4C8E22.root',
        '/store/relval/CMSSW_9_3_7/relvalTTbar_14TeV/GEN-SIM-DIGI-RAW/93X_upgrade2023_realistic_v5_2023D17noPU-v2/10000/D86D2DA6-752C-E811-8E90-0025905B856C.root',
        '/store/relval/CMSSW_9_3_7/relvalTTbar_14TeV/GEN-SIM-DIGI-RAW/93X_upgrade2023_realistic_v5_2023D17noPU-v2/10000/CE1930F1-762C-E811-B9F4-0CC47A78A468.root',
        '/store/relval/CMSSW_9_3_7/relvalTTbar_14TeV/GEN-SIM-DIGI-RAW/93X_upgrade2023_realistic_v5_2023D17noPU-v2/10000/B0D7BD76-782C-E811-AED2-0CC47A7C35F4.root',
        '/store/relval/CMSSW_9_3_7/relvalTTbar_14TeV/GEN-SIM-DIGI-RAW/93X_upgrade2023_realistic_v5_2023D17noPU-v2/10000/AC64F3F2-8D2C-E811-AEA8-0025905A611E.root',
        '/store/relval/CMSSW_9_3_7/relvalTTbar_14TeV/GEN-SIM-DIGI-RAW/93X_upgrade2023_realistic_v5_2023D17noPU-v2/10000/A05308F4-762C-E811-ACC0-0CC47A78A360.root',
        '/store/relval/CMSSW_9_3_7/relvalTTbar_14TeV/GEN-SIM-DIGI-RAW/93X_upgrade2023_realistic_v5_2023D17noPU-v2/10000/886F551E-7E2C-E811-9D10-0025905A608C.root',
        '/store/relval/CMSSW_9_3_7/relvalTTbar_14TeV/GEN-SIM-DIGI-RAW/93X_upgrade2023_realistic_v5_2023D17noPU-v2/10000/7EC7DD7F-782C-E811-B469-0CC47A4D76A0.root',
        '/store/relval/CMSSW_9_3_7/relvalTTbar_14TeV/GEN-SIM-DIGI-RAW/93X_upgrade2023_realistic_v5_2023D17noPU-v2/10000/7EB68679-782C-E811-A2C9-0CC47A4D7690.root',
        '/store/relval/CMSSW_9_3_7/relvalTTbar_14TeV/GEN-SIM-DIGI-RAW/93X_upgrade2023_realistic_v5_2023D17noPU-v2/10000/0E1785F0-762C-E811-9192-0CC47A78A4BA.root',
        ),
    inputCommands = cms.untracked.vstring("keep *", 
        "drop l1tHGCalTowerMapBXVector_hgcalTriggerPrimitiveDigiProducer_towerMap_HLT",
        "drop l1tEMTFHit2016Extras_simEmtfDigis_CSC_HLT",
        "drop l1tEMTFHit2016Extras_simEmtfDigis_RPC_HLT",
        "drop l1tEMTFHit2016s_simEmtfDigis__HLT",
        "drop l1tEMTFTrack2016Extras_simEmtfDigis__HLT",
        "drop l1tEMTFTrack2016s_simEmtfDigis__HLT")
     #skipEvents = cms.untracked.uint32(80)
)

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '100X_upgrade2023_realistic_v1', '')


#################################################
## Remake jets
#################################################

## Select charged hadron subtracted packed PF candidates
process.pfCHS = cms.EDFilter("CandPtrSelector", src = cms.InputTag("packedPFCandidates"), cut = cms.string("fromPV"))
from RecoJets.JetProducers.ak4PFJets_cfi import ak4PFJets
## Define PFJetsCHS
process.ak4PFJetsCHS = ak4PFJets.clone(src = 'pfCHS')

#################################################

process.load('SimCalorimetry.HcalTrigPrimProducers.hcaltpdigi_cff')
process.load('CalibCalorimetry.CaloTPG.CaloTPGTranscoder_cfi')

process.load('L1Trigger.L1THGCal.hgcalTriggerPrimitives_cff')
process.hgcl1tpg_step = cms.Path(process.hgcalTriggerPrimitives)

process.load('SimCalorimetry.EcalEBTrigPrimProducers.ecalEBTriggerPrimitiveDigis_cff')
process.EcalEBtp_step = cms.Path(process.simEcalEBTriggerPrimitiveDigis)

process.load('L1Trigger.TrackFindingTracklet.L1TrackletTracks_cff')
process.L1TrackTrigger_step = cms.Path(process.L1TrackletTracksWithAssociators)

process.VertexProducer.l1TracksInputTag = cms.InputTag("TTTracksFromTracklet", "Level1TTTracks")

process.L1simulation_step = cms.Path(process.SimL1Emulator)

## Output file
#process.out = cms.OutputModule("PoolOutputModule",
#    outputCommands = cms.untracked.vstring('keep *'), 
#    fileName = cms.untracked.string('outfile.root')
#)

process.endjob_step = cms.EndPath(process.endOfProcess)

#################################################
## Remake jets
#################################################

## Select charged hadron subtracted packed PF candidates
process.pfCHS = cms.EDFilter("CandPtrSelector", src = cms.InputTag("packedPFCandidates"), cut = cms.string("fromPV"))
from RecoJets.JetProducers.ak4PFJets_cfi import ak4PFJets
## Define PFJetsCHS
process.ak4PFJetsCHS = ak4PFJets.clone(src = 'pfCHS')

#################################################

### Output file
#process.out = cms.OutputModule("PoolOutputModule",
#    outputCommands = cms.untracked.vstring('keep *'), 
#    fileName = cms.untracked.string('outfile-2.root')
#)

# load the standard PAT config
process.load("PhysicsTools.PatAlgos.patSequences_cff")

# load the coreTools of PAT
from PhysicsTools.PatAlgos.tools.jetTools import *
addJetCollection(
                 process,
                 labelName = 'NewSlimmedJets',
                 jetSource = cms.InputTag('ak4PFJetsCHS'),
                 pfCandidates = cms.InputTag("packedPFCandidates"),
                 explicitJTA = False,
                 svClustering = False,
                 #jetCorrections = ('AK4PFchs', cms.vstring(['L1FastJet', 'L2Relative', 'L3Absolute']), 'Type-2'),
                 btagInfos = ['impactParameterTagInfos','secondaryVertexTagInfos','softPFMuonsTagInfos'],#,'softPFElectronsTagInfos'
                 btagDiscriminators=['simpleSecondaryVertexHighEffBJetTags','simpleSecondaryVertexHighPurBJetTags','softPFMuonBJetTags']#,'softPFElectronBJetTags'
                 )

process.unpackTV  = cms.EDProducer('PATTrackAndVertexUnpacker',
                                   slimmedVertices  = cms.InputTag("offlineSlimmedPrimaryVertices"),
                                   additionalTracks = cms.InputTag("lostTracks"),
                                   packedCandidates = cms.InputTag("packedPFCandidates"),
                                   slimmedSecondaryVertices = cms.InputTag("slimmedSecondaryVertices")
                                   )

process.patJetPartonsLegacy = cms.EDProducer("PartonSelector",
                                             withLeptons = cms.bool(False),
                                             src = cms.InputTag("genParticles")
)

process.patJetPartonAssociationLegacy = cms.EDProducer("JetPartonMatcher",
                                                       jets    = cms.InputTag("ak4PFJetsCHS"),
                                                       partons = cms.InputTag("patJetPartonsLegacy"),
                                                       coneSizeToAssociate = cms.double(0.3),
                                                       )

process.patJetFlavourAssociationLegacy = cms.EDProducer("JetFlavourIdentifier",
                                                        srcByReference    = cms.InputTag("patJetPartonAssociationLegacy"),
                                                        physicsDefinition = cms.bool(False)
                                                        )

process.patJetPartons = cms.EDProducer('HadronAndPartonSelector',
                                       src        = cms.InputTag("generator"),
                                       particles  = cms.InputTag("genParticles"),
                                       partonMode = cms.string("Auto"),
                                       fullChainPhysPartons = cms.bool(True)
                                       )

process.jetTracksAssociatorAtVertexNewSlimmedJets.jets = cms.InputTag("ak4PFJetsCHS")
process.jetTracksAssociatorAtVertexNewSlimmedJets.tracks = cms.InputTag("unpackTV")
process.impactParameterTagInfosNewSlimmedJets.primaryVertex = cms.InputTag("unpackTV")
process.softPFMuonsTagInfosNewSlimmedJets.primaryVertex = cms.InputTag("unpackTV")
process.softPFMuonBJetTagsNewSlimmedJets.tagInfos = cms.VInputTag(cms.InputTag("softPFMuonsTagInfosNewSlimmedJets"))

## the key to adding the 
process.patJetsNewSlimmedJets.addTagInfos = cms.bool(True)

process.load("L1Trigger.phase2L1BTagAnalyzer.phase2L1BTagAnalyzer_cfi")
process.L1BTagAnalyzer.slimmedJets = cms.InputTag("patJetsNewSlimmedJets")

process.TFileService = cms.Service("TFileService", 
                                   fileName = cms.string("analyzer_ZTTbar.root")
                                   )

## Define a Path
process.btaggingPath = cms.Path(
    process.pfCHS
    * process.ak4PFJetsCHS
    * process.unpackTV
    * process.patJetPartonsLegacy
    * process.patJetPartonAssociationLegacy
    * process.patJetFlavourAssociationLegacy
    * process.patJetPartons
    * process.softPFMuonsTagInfosNewSlimmedJets
    * process.softPFMuonBJetTagsNewSlimmedJets
    * process.patJetsNewSlimmedJets
    * process.L1BTagAnalyzer
)

process.schedule = cms.Schedule(process.EcalEBtp_step,
                                process.L1TrackTrigger_step,
                                process.L1simulation_step,
                                process.btaggingPath,
                                process.endjob_step) 

## Define the EndPath, if needed
#process.output = cms.EndPath(
#    process.out
#)

from PhysicsTools.PatAlgos.tools.helpers import associatePatAlgosToolsTask
associatePatAlgosToolsTask(process)

from Configuration.StandardSequences.earlyDeleteSettings_cff import customiseEarlyDelete
process = customiseEarlyDelete(process)

dump_file = open('dump.py','w')
dump_file.write(process.dumpPython())
