# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: repr --processName=REPR --python_filename=reprocess_test_10_5_0_pre1.py --no_exec -s L1 --datatier GEN-SIM-DIGI-RAW -n 2 --era Phase2 --eventcontent FEVTDEBUGHLT --filein root://cms-xrd-global.cern.ch//store/mc/PhaseIIMTDTDRAutumn18DR/DYToLL_M-50_14TeV_pythia8/FEVT/PU200_pilot_103X_upgrade2023_realistic_v2_ext4-v1/280000/FF5C31D5-D96E-5E48-B97F-61A0E00DF5C4.root --conditions 103X_upgrade2023_realistic_v2 --beamspot HLLHC14TeV --geometry Extended2023D28 --fileout file:step2_2ev_reprocess_slim.root
import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras

#process = cms.Process('REPR',eras.Phase2C4_trigger)
#process = cms.Process('REPR',eras.Phase2C4_timing_layer_bar)
process = cms.Process('REPR')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.Geometry.GeometryExtended2023D35Reco_cff')
process.load('Configuration.Geometry.GeometryExtended2023D35_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.SimL1Emulator_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.load("RecoBTag.Configuration.RecoBTag_cff") # this loads all available b-taggers

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

# Input source
process.source = cms.Source("PoolSource",
   fileNames = cms.untracked.vstring('/store/mc/PhaseIIMTDTDRAutumn18MiniAOD/GluGluToHHTo4B_node_SM_14TeV-madgraph/MINIAODSIM/PU200_103X_upgrade2023_realistic_v2-v1/90000/FB21FB37-11FC-D948-8922-2690CD8B5BCB.root'),
   secondaryFileNames = cms.untracked.vstring('/store/mc/PhaseIIMTDTDRAutumn18DR/GluGluToHHTo4B_node_SM_14TeV-madgraph/FEVT/PU200_103X_upgrade2023_realistic_v2-v1/90000/2B449275-7BDC-B04A-9447-A656A3DA6A36.root')

)

process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange("1:337")


process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('repr nevts:2'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Output definition

process.FEVTDEBUGHLToutput = cms.OutputModule("PoolOutputModule",
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('GEN-SIM-DIGI-RAW'),
        filterName = cms.untracked.string('')
    ),
    fileName = cms.untracked.string('file:step2_2ev_reprocess_slim.root'),
    outputCommands = process.FEVTDEBUGHLTEventContent.outputCommands,
    splitLevel = cms.untracked.int32(0)
)

# Additional output definition


# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
#process.GlobalTag = GlobalTag(process.GlobalTag, '103X_upgrade2023_realistic_v2', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase2_realistic', '')

process.GlobalTag = GlobalTag(process.GlobalTag, '103X_upgrade2023_realistic_v2', '') 

process.load('SimCalorimetry.HcalTrigPrimProducers.hcaltpdigi_cff')
process.load('CalibCalorimetry.CaloTPG.CaloTPGTranscoder_cfi')

# From test-Analyzer.py
process.load('L1Trigger.L1THGCal.hgcalTriggerPrimitives_cff')
process.hgcl1tpg_step = cms.Path(process.hgcalTriggerPrimitives)

process.load('SimCalorimetry.EcalEBTrigPrimProducers.ecalEBTriggerPrimitiveDigis_cff')
process.EcalEBtp_step = cms.Path(process.simEcalEBTriggerPrimitiveDigis)

process.load('L1Trigger.TrackFindingTracklet.L1TrackletTracks_cff')
process.L1TrackTrigger_step = cms.Path(process.L1TrackletTracksWithAssociators)

process.VertexProducer.l1TracksInputTag = cms.InputTag("TTTracksFromTracklet", "Level1TTTracks")

## Output file
#process.out = cms.OutputModule("PoolOutputModule",
#    outputCommands = cms.untracked.vstring('keep *'), 
#    fileName = cms.untracked.string('outfile.root')
#)

# Path and EndPath definitions
process.L1simulation_step = cms.Path(process.SimL1Emulator)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.FEVTDEBUGHLToutput_step = cms.EndPath(process.FEVTDEBUGHLToutput)

#################################################
## Remake jets
#################################################

## Select charged hadron subtracted packed PF candidates
process.pfCHS = cms.EDFilter("CandPtrSelector", src = cms.InputTag("packedPFCandidates"), cut = cms.string("fromPV"))
from RecoJets.JetProducers.ak4PFJets_cfi import ak4PFJets
## Define PFJetsCHS
process.ak4PFJetsCHS = ak4PFJets.clone(src = 'pfCHS')

#################################################

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
                 btagInfos = ['impactParameterTagInfos','secondaryVertexTagInfos'],#softPFElectronsTagInfos'
                 btagDiscriminators=['simpleSecondaryVertexHighEffBJetTags','simpleSecondaryVertexHighPurBJetTags']#,'softPFElectronBJetTags'
                 )

process.unpackTV  = cms.EDProducer('PATTrackAndVertexUnpacker',
                                   slimmedVertices  = cms.InputTag("offlineSlimmedPrimaryVertices","","PAT"), 
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
#process.softPFMuonsTagInfosNewSlimmedJets.primaryVertex = cms.InputTag("unpackTV")
#process.softPFMuonBJetTagsNewSlimmedJets.tagInfos = cms.VInputTag(cms.InputTag("softPFMuonsTagInfosNewSlimmedJets"))

## the key to adding the 
process.patJetsNewSlimmedJets.addTagInfos = cms.bool(True)

process.load("L1Trigger.phase2L1BTagAnalyzer.phase2L1BTagAnalyzer_cfi")
# cms.InputTag takes as input a field found in edmDumpEventContent, and lets us rename it to
# something in the analyzer:
process.L1BTagAnalyzer.primaryVertices = cms.InputTag("offlineSlimmedPrimaryVertices","","PAT")
process.L1BTagAnalyzer.slimmedJets = cms.InputTag("patJetsNewSlimmedJets")
process.L1BTagAnalyzer.slimmedMuons = cms.InputTag("slimmedMuons")

process.TFileService = cms.Service("TFileService", 
                                   fileName = cms.string("analyzer.root")
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
#    * process.softPFMuonsTagInfosNewSlimmedJets
#    * process.softPFMuonBJetTagsNewSlimmedJets
    * process.patJetsNewSlimmedJets
    * process.L1BTagAnalyzer
)


# Schedule definition
process.schedule = cms.Schedule(process.L1simulation_step,
                                process.FEVTDEBUGHLToutput_step,
                                process.EcalEBtp_step,
                                process.L1TrackTrigger_step,
                                process.btaggingPath,
                                process.endjob_step)
## Define the EndPath, if needed
#process.output = cms.EndPath(
#    process.out
#)

from PhysicsTools.PatAlgos.tools.helpers import associatePatAlgosToolsTask
associatePatAlgosToolsTask(process)


# Customisation from command line

from L1Trigger.Configuration.customiseUtils import L1TrackTriggerTracklet
process = L1TrackTriggerTracklet(process)

# Add early deletion of temporary data products to reduce peak memory need
from Configuration.StandardSequences.earlyDeleteSettings_cff import customiseEarlyDelete
process = customiseEarlyDelete(process)
# End adding early deletion
