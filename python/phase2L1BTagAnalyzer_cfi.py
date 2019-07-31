import FWCore.ParameterSet.Config as cms

L1BTagAnalyzer = cms.EDAnalyzer('phase2L1BTagAnalyzer',
                                tracks = cms.untracked.InputTag('ctfWithMaterialTracks'),
                                recoJets      = cms.InputTag("recoJets", "", "RECO"),
                                #slimmedJets      = cms.InputTag("slimmedJets", "", "RECO"),
                                packedCandidates = cms.InputTag("packedPFCandidates","","RECO"),
                                L1TrackInputTag  = cms.InputTag("TTTracksFromTracklet", "Level1TTTracks"),
                                )

