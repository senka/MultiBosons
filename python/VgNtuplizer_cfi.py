import FWCore.ParameterSet.Config as cms
from ElectroWeakAnalysis.MultiBosons.kinAxis_cfi  import *

VgNtuplizer = cms.EDProducer("VgNtuplizer",
                             ntuplize = cms.string('none'),
                             outputTextName = cms.string('VgAnalyzerKit_output.txt'),
                             enable = cms.string(''),
                             disable = cms.string(''),
                             doMuon         = cms.bool(True),
                             doElectron     = cms.bool(True),
                             doTau          = cms.bool(True),
                             doJet          = cms.bool(True),
                             doMET          = cms.bool(True),
                             doPhoton       = cms.bool(True),
                             doTrack        = cms.bool(True),
                             doGenParticles = cms.bool(False),
                             doStoreJets    = cms.bool(True),
                             doSkim         = cms.bool(False),
                             doZmumu        = cms.bool(False),
                             electronSrc = cms.InputTag("cleanPatElectronsTriggerMatch"),
                             tauSrc = cms.InputTag("cleanPatTausTriggerMatch"),
                             muonSrc = cms.InputTag("cleanPatMuonsTriggerMatch"),
                             jetSrc = cms.InputTag("cleanPatJetsTriggerMatch"),
                             photonSrc = cms.InputTag("cleanPatPhotonsTriggerMatch"),
                             METSrc = cms.InputTag("patMETsTriggerMatch"),
                             trackSrc = cms.InputTag("allTrackCandidates"),
                             genParticleSrc = cms.InputTag("prunedGenParticles"),
                             triggerEvent = cms.InputTag("patTriggerEvent"),
                             VtxLabel = cms.InputTag("offlinePrimaryVertices"),
                             CaloTowerLabel = cms.InputTag('selectedPatJetsTriggerMatch','caloTowers'),
                             tcMETLabel = cms.InputTag("patMETsTCTriggerMatch"),
                             pfMETLabel = cms.InputTag("patMETsPFTriggerMatch"),
                             PUInfoLabel = cms.InputTag("addPileupInfo"),
                             RhoLabel= cms.InputTag("kt6PFJets", "rho"),
                             SigmaLabel= cms.InputTag("kt6PFJets", "sigma"),                             
                             LeadingElePtCut = cms.double(15),
                             LeadingMuPtCut = cms.double(15),
                             LeadingPhoPtCut = cms.double(10),
                             BeamSpotCollection = cms.InputTag("offlineBeamSpot"),
                             muonAxis     = kinAxis(0, 200, 0, 200),
                             electronAxis = kinAxis(0, 200, 0, 200),
                             tauAxis      = kinAxis(0, 200, 0, 200),
                             jetAxis      = kinAxis(0, 200, 0, 200),
                             METAxis      = kinAxis(0, 200, 0, 200),
                             photonAxis   = kinAxis(0, 200, 0, 200),
                             trackAxis    = kinAxis(0, 200, 0, 200),
                             genParticleAxis = kinAxis(0, 200, 0, 200)
                             )

