#ifndef VgKit_HistoJet_h
#define VgKit_HistoJet_h

//------------------------------------------------------------
// Title: HistoJet.h
// Purpose: To histogram Jets
//
// Authors:
// Liz Sexton-Kennedy <sexton@fnal.gov>
// Eric Vaandering <ewv@fnal.gov >
// Petar Maksimovic <petar@jhu.edu>
// Sal Rappoccio <rappocc@fnal.gov>
//------------------------------------------------------------
//
// Interface:
//
//   HistoJet ( TFile * file );
//   Description: Constructor.
//
//   void fill( TK::Jet * );
//   Description: Fill object. Will fill relevant jet variables
//
//   void write();
//   Description: Write object to file in question.
//
//   ~HistoJet
//    Description: Destructor. Deallocates memory.
//
//------------------------------------------------------------
//
// Modification History:
//
//   -29Nov07: Sal Rappoccio: Creation of the object
//------------------------------------------------------------


// CMSSW include files
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "ElectroWeakAnalysis/MultiBosons/interface/HistoGroup.h"

#include "DataFormats/Common/interface/View.h"

// STL include files
#include <string>

// ROOT include files
#include <TH1D.h>
#include <TFile.h>

namespace pat {

  class HistoJet : public HistoGroup<Jet> {

  public:
    HistoJet( std::string dir = "jet",std::string group = "Jet",
	      std::string pre="jet",
	      double pt1=0, double pt2=200, double m1=0, double m2=200,
	      TFileDirectory * parentDir=0 );
    virtual ~HistoJet();


    // fill a plain ol' jet:
    virtual void fill( const Jet *jet, uint iPart = 1, double weight = 1.0 );
    virtual void fill( const Jet &jet, uint iPart = 1, double weight = 1.0 ) { fill(&jet, iPart,weight); }

    // fill a jet that is a shallow clone, and take kinematics from 
    // shallow clone but detector plots from the jet itself
    virtual void fill( const reco::ShallowClonePtrCandidate *jet, uint iPart = 1, double weight = 1.0 );
    virtual void fill( const reco::ShallowClonePtrCandidate &jet, uint iPart = 1, double weight = 1.0 )
    { fill(&jet, iPart, weight); }

    virtual void fillCollection( const edm::View<Jet> & coll, double weight = 1.0 );

    // Clear ntuple cache
    void clearVec();
  protected:
    PhysVarHisto *    h_jetFlavour_;
    PhysVarHisto *    h_BDiscriminant_;
    PhysVarHisto *    h_jetCharge_;
    PhysVarHisto *    h_nTrk_;

    PhysVarHisto *    jetME_;

   // Generic Jet Parameters
//    PhysVarHisto *    mEta_;
//    PhysVarHisto *    mPhi_;
    PhysVarHisto *    mE_;
    PhysVarHisto *    mP_;
//    PhysVarHisto *    mPt_;
    PhysVarHisto *    mPt_1_;
    PhysVarHisto *    mPt_2_;
    PhysVarHisto *    mPt_3_;
//    PhysVarHisto *    mMass_;
    PhysVarHisto *    mConstituents_;

 // Leading Jet Parameters
    PhysVarHisto *    mEtaFirst_;
    PhysVarHisto *    mPhiFirst_;
    PhysVarHisto *    mEFirst_;
    PhysVarHisto *    mPtFirst_;

 // CaloJet specific
    PhysVarHisto *    mMaxEInEmTowers_;
    PhysVarHisto *    mMaxEInHadTowers_;
    PhysVarHisto *    mHadEnergyInHO_;
    PhysVarHisto *    mHadEnergyInHB_;
    PhysVarHisto *    mHadEnergyInHF_;
    PhysVarHisto *    mHadEnergyInHE_;
    PhysVarHisto *    mEmEnergyInEB_;
    PhysVarHisto *    mEmEnergyInEE_;
    PhysVarHisto *    mEmEnergyInHF_;
    PhysVarHisto *    mEnergyFractionHadronic_;
    PhysVarHisto *    mEnergyFractionEm_;
    PhysVarHisto *    mN90_;
/*
  // PFlowJet specific
    PhysVarHisto *    mChargedHadronEnergy_;
    PhysVarHisto *    mNeutralHadronEnergy_;
    PhysVarHisto *    mChargedEmEnergy_;
    PhysVarHisto *    mChargedMuEnergy_;
    PhysVarHisto *    mNeutralEmEnergy_;
    PhysVarHisto *    mChargedMultiplicity_;
    PhysVarHisto *    mNeutralMultiplicity_;
    PhysVarHisto *    mMuonMultiplicity_;

    PhysVarHisto *    mNeutralFraction_;
*/

  };

}
#endif
