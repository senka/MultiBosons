#ifndef VgKit_PhysicsHistograms_h
#define VgKit_PhysicsHistograms_h

//------------------------------------------------------------------------
//!  \class PhysicsHistograms
//!  \brief Object to manage and fill various physics histograms
//!
//!  The order how the operations must be executed.
//!
//!  1. we first build our own default histogram groups (electrons, muons, etc)
//!
//!  2. the user-defined histogram groups are added by add*HistoGroup() methods.
//!
//!  3. configure starts:
//!     all PhysVarHisto pointers are collected in one big flat array for
//!     easy access and speedy processing.
//!
//!  4. various histograms are disabled.
//!
//!  5. various histograms are enabled.  configure ends.
//!
//!  At this point we're good to go and ready to see the events.
//------------------------------------------------------------------------



// system include files
#include <memory>
#include <fstream>

// user include files
#include "ElectroWeakAnalysis/MultiBosons/interface/HistoMuon.h"
#include "ElectroWeakAnalysis/MultiBosons/interface/HistoElectron.h"
#include "ElectroWeakAnalysis/MultiBosons/interface/HistoTau.h"
#include "ElectroWeakAnalysis/MultiBosons/interface/HistoJet.h"
#include "ElectroWeakAnalysis/MultiBosons/interface/HistoMET.h"
#include "ElectroWeakAnalysis/MultiBosons/interface/HistoPhoton.h"
#include "ElectroWeakAnalysis/MultiBosons/interface/HistoTrack.h"
#include "ElectroWeakAnalysis/MultiBosons/interface/HistoGenParticle.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "DataFormats/Common/interface/View.h"

#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "CommonTools/Utils/interface/TFileDirectory.h"

//
//--- Class declaration.
//


// Function to print out candidates
std::ostream & operator<<( std::ostream & out, const reco::Candidate & cand );

class PhysicsHistograms  {
public:

  struct KinAxisLimits {
  
    double pt1, pt2, m1, m2;

    KinAxisLimits( double apt1=0, double apt2=0, double am1=0, double am2=0 ) :
      pt1(apt1), pt2(apt2), m1(am1), m2(am2)
    {
    }
    ~KinAxisLimits() {}
  };


  explicit PhysicsHistograms ( KinAxisLimits const & muonAxis, 
			       KinAxisLimits const & electronAxis, 
			       KinAxisLimits const & tauAxis, 
			       KinAxisLimits const & jetAxis, 
			       KinAxisLimits const & METAxis, 
			       KinAxisLimits const & photonAxis,
			       KinAxisLimits const & trackAxis,
			       KinAxisLimits const & genParticlesAxis
			       );
  virtual ~PhysicsHistograms();

  //--- Standard methods used in the event processing, called either by ED analyzer
  //    (from the methods of the same name), or by the FWLite macro which does the
  //    event loop).
  //
  virtual void beginJob();  //!<  initialize before seeing any events
  virtual void   endJob();  //!<  do whatever is needed after seeing all events


  //--- Configuration.
  virtual void configure( std::string & histos_to_disable,   // comma separated list of names
			  std::string & histos_to_enable );  // comma separated list of names


  //--- Selection of a subset of PhysVarHistos.
  virtual void select( std::string  vars_to_select,   // comma separated list of names
		       std::vector< pat::PhysVarHisto * > & selectedHistos );

  //--- Clear cache vector for PhysVarHisto
  virtual void clearVec();

  //--- Specific actions for the event.
  // &&& Design note: we could have used overloaded fill() everywhere, but
  // &&&              the novices may find it confusing.

  //--- Process a whole collection of Muons...
  //
  inline void fillCollection( const edm::View<pat::Muon> & coll, double w = 1.0 )
    { muonHistograms_->fillCollection(coll,w); }

  //--- ... or Electrons...
  //
  inline void fillCollection( const edm::View<pat::Electron> & coll, double w = 1.0 )
    { electronHistograms_->fillCollection(coll,w); }

  //--- ... or Taus...
  //
  inline void fillCollection( const edm::View<pat::Tau> & coll, double w = 1.0 )
    { tauHistograms_->fillCollection(coll,w); }

  //--- ... or Jets...
  //
  inline void fillCollection( const edm::View<pat::Jet> & coll, double w = 1.0 )
    { jetHistograms_->fillCollection(coll,w); }

  //--- ... or MET.
  //
  inline void fillCollection( const edm::View<pat::MET> & coll, double w = 1.0 )
    { metHistograms_->fillCollection(coll,w); }

  //--- ... or Photon.
  //
  inline void fillCollection( const edm::View<pat::Photon> & coll, double w = 1.0 )
    { photonHistograms_->fillCollection(coll,w); }

  //--- ... or Track.
  //
  inline void fillCollection( const std::vector<reco::RecoChargedCandidate> & coll, double w = 1.0 )
    { trackHistograms_->fillCollection(coll,w); }

  //--- ... or GenParticle.
  //
  inline void fillCollection( const std::vector<reco::GenParticle> & coll, double w = 1.0 )
    { genParticleHistograms_->fillCollection(coll,w); }



  // &&& Design note: again, let's be explicit.  This could be compressed into
  // &&&              fewer functions, but at the expense of more complicated
  // &&&              code under the hood, and also an interface which is a teeny
  // &&&              harder to master (and we are trying to avoid that; the
  // &&&              interface should be as dumb as possible).

  //--- Add one histo to muon group, or a whole group of muon histograms
  //
  inline void addMuonHisto ( pat::PhysVarHisto * h )
    { muonHistograms_->addHisto(h); }
  inline void addMuonHistoGroup( pat::HistoMuon * hgr )
    { muonHistograms_->addHistoGroup(hgr); }

  //--- Add one histo to electron group, or a whole group of electron histograms
  //
  inline void addElectronHisto ( pat::PhysVarHisto * h )
    { electronHistograms_->addHisto(h); }
  inline void addElectronHistoGroup( pat::HistoElectron * hgr )
    { electronHistograms_->addHistoGroup(hgr); }

  //--- Add one histo to tau group, or a whole group of tau histograms
  //
  inline void addTauHisto ( pat::PhysVarHisto * h )
    { tauHistograms_->addHisto(h); }
  inline void addTauHistoGroup( pat::HistoTau * hgr )
    { tauHistograms_->addHistoGroup(hgr); }

  //--- Add one histo to jet group, or a whole group of jet histograms
  //
  inline void addJetHisto ( pat::PhysVarHisto * h )
    { jetHistograms_->addHisto(h); }
  inline void addJetHistoGroup( pat::HistoJet * hgr )
    { jetHistograms_->addHistoGroup(hgr); }

  //--- Add one histo to MET group, or a whole group of MET histograms
  //
  inline void addMetHisto ( pat::PhysVarHisto * h )
    { metHistograms_->addHisto(h); }
  inline void addMetHistoGroup( pat::HistoMET * hgr )
    { metHistograms_->addHistoGroup(hgr); }

  //--- Add one histo to photon group, or a whole group of photon histograms
  //
  inline void addPhotonHisto ( pat::PhysVarHisto * h )
    { photonHistograms_->addHisto(h); }
  inline void addPhotonHistoGroup( pat::HistoPhoton * hgr )
    { photonHistograms_->addHistoGroup(hgr); }


  //--- Add one histo to track group, or a whole group of track histograms
  //
  inline void addTrackHisto ( pat::PhysVarHisto * h )
    { trackHistograms_->addHisto(h); }
  inline void addTrackHistoGroup( pat::HistoTrack * hgr )
    { trackHistograms_->addHistoGroup(hgr); }

  //--- Add one histo to genParticle group, or a whole group of genParticle histograms
  //
  inline void addGenParticleHisto ( pat::PhysVarHisto * h )
    { genParticleHistograms_->addHisto(h); }
  inline void addGenParticleHistoGroup( pat::HistoGenParticle * hgr )
    { genParticleHistograms_->addHistoGroup(hgr); }

  //--- Add one generic histo to list
  inline void addHisto( pat::PhysVarHisto * h )
    { allVarHistos_.push_back( h ); }



private:

  // Parameters for running
  std::string     outputTextName_;

  // Histogram server
  edm::Service<TFileService> fs;

  // Histogram objects that make "standard" plots for each object
  pat::HistoMuon           * muonHistograms_;
  pat::HistoElectron       * electronHistograms_;
  pat::HistoTau            * tauHistograms_;
  pat::HistoMET            * metHistograms_;
  pat::HistoJet            * jetHistograms_;
  pat::HistoPhoton         * photonHistograms_;
  pat::HistoTrack          * trackHistograms_;
  pat::HistoGenParticle    * genParticleHistograms_;

  //--- The summary of all PhysVarHistos.
  // &&& Is this still needed?
  std::vector< pat::PhysVarHisto* > allVarHistos_ ;
  std::vector< pat::PhysVarHisto* > enabledVarHistos_ ;

  //--- This is a nice feature but let's not worry about it for now. &&&
  ofstream        outputFile_;
};

#endif
