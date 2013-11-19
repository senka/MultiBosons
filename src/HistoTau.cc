#include "ElectroWeakAnalysis/MultiBosons/interface/HistoTau.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/TrackReco/interface/Track.h"

#include <iostream>
#include <sstream>

using pat::HistoTau;
using namespace std;

// Constructor:


HistoTau::HistoTau(std::string dir, std::string group,std::string pre,
		   double pt1, double pt2, double m1, double m2,
		   TFileDirectory * parentDir)
  : HistoGroup<Tau>( dir, group, pre, pt1, pt2, m1, m2, parentDir)
{

  histoLeadingTrack_ = new HistoTrack( dir, group + "TauLeadingTrack", pre + "tauLeadingTrack" );
  histoSignalTrack_ = new HistoTrack( dir, group + "TauSignalTracks", pre + "tauSignalTracks" );
  histoIsolationTrack_ = new HistoTrack( dir, group + "TauIsolationTracks", pre + "tauIsolationTracks" );

  addHisto( h_emEnergyFraction_ =
	    new PhysVarHisto( pre + "EmEnergyFraction", "Tau EM Energy Fraction", 20, 0, 10, currDir_, "", "vD" )
	   );

  addHisto( h_eOverP_  =
	    new PhysVarHisto( pre + "EOverP",  "Tau E over P",  20, 0, 10, currDir_, "", "vD" )
	    );


}



void HistoTau::fill( const Tau *tau, uint iTau, double weight )
{

  // First fill common 4-vector histograms from shallow clone
  HistoGroup<Tau>::fill( tau, iTau, weight);

  // fill relevant tau histograms

  const double M_PION = 0.13957018;

  if ( tau->leadTrack().isNonnull() ) {
    const reco::Track & trk = *( tau->leadTrack() );
    ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> > p4;
    p4.SetPt( trk.pt() );
    p4.SetEta( trk.eta() );
    p4.SetPhi( trk.phi() );
    p4.SetM( M_PION );
    reco::Particle::LorentzVector p4_2( p4.x(), p4.y(), p4.z(), p4.t() );
    reco::RecoChargedCandidate trk_p4( trk.charge(), p4_2 );
    trk_p4.setTrack( tau->leadTrack() );
    histoLeadingTrack_->fill( &trk_p4, 1, weight );
  }
  
//   for ( unsigned int isignal = 0; isignal < tau->signalTracks().size(); isignal++ ) {
//     cout << "1" << endl;
//     if ( tau->signalTracks().at(isignal).isNonnull() ) {
//       cout << "2" << endl;
//       const reco::Track & trk = *( tau->signalTracks().at(isignal) );
//       ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> > p4;
//       p4.SetPt( trk.pt() );
//       p4.SetEta( trk.eta() );
//       p4.SetPhi( trk.phi() );
//       p4.SetM( M_PION );
//       cout << "3" << endl;
//       reco::Particle::LorentzVector p4_2( p4.x(), p4.y(), p4.z(), p4.t() );
//       reco::RecoChargedCandidate trk_p4( trk.charge(), p4_2 );
//       cout << "4" << endl;
//       trk_p4.setTrack( tau->signalTracks().at(isignal) );
//       cout << "5" << endl;
//       histoSignalTrack_->fill( &trk_p4, isignal, weight );
//     }
//   }
//   for ( unsigned int iisolation = 0; iisolation < tau->isolationTracks().size(); iisolation++ ) {
//   cout << "6" << endl;
//     const reco::Track & trk = *( tau->isolationTracks().at(iisolation) );
//     ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> > p4;
//     p4.SetPt( trk.pt() );
//     p4.SetEta( trk.eta() );
//     p4.SetPhi( trk.phi() );
//     p4.SetM( M_PION );
//     reco::Particle::LorentzVector p4_2( p4.x(), p4.y(), p4.z(), p4.t() );
//     reco::RecoChargedCandidate trk_p4( trk.charge(), p4_2 );
//     trk_p4.setTrack( tau->isolationTracks().at(iisolation) );
//     histoIsolationTrack_->fill( &trk_p4, iisolation, weight );
//   }
//   cout << "7" << endl;

//   h_emEnergyFraction_->fill( tau->emEnergyFraction(), iTau, weight );
//   h_eOverP_ ->fill( tau->eOverP() , iTau, weight );
}

void HistoTau::fill( const reco::ShallowClonePtrCandidate * pshallow, uint iTau, double weight )
{

  // Get the underlying object that the shallow clone represents
  const pat::Tau * tau = dynamic_cast<const pat::Tau*>(&*(pshallow->masterClonePtr()));

  if ( tau == 0 ) {
    cout << "Error! Was passed a shallow clone that is not at heart a tau" << endl;
    return;
  }


  // First fill common 4-vector histograms from shallow clone
  HistoGroup<Tau>::fill( pshallow, iTau, weight);

  // fill relevant tau histograms

  const double M_PION = 0.13957018;


  if ( tau->leadTrack().isNonnull() ) {
  
    const reco::Track & trk = *( tau->leadTrack() );
    ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> > p4;
    p4.SetPt( trk.pt() );
    p4.SetEta( trk.eta() );
    p4.SetPhi( trk.phi() );
    p4.SetM( M_PION );
    reco::Particle::LorentzVector p4_2( p4.x(), p4.y(), p4.z(), p4.t() );
    reco::RecoChargedCandidate trk_p4( trk.charge(), p4_2 );
    trk_p4.setTrack( tau->leadTrack() );
    histoLeadingTrack_->fill( &trk_p4, 1, weight );
  }
  
//   for ( unsigned int isignal = 0; isignal < tau->signalTracks().size(); isignal++ ) {
//     const reco::Track & trk = *( tau->signalTracks().at(isignal) );
//     ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> > p4;
//     p4.SetPt( trk.pt() );
//     p4.SetEta( trk.eta() );
//     p4.SetPhi( trk.phi() );
//     p4.SetM( M_PION );
//     reco::Particle::LorentzVector p4_2( p4.x(), p4.y(), p4.z(), p4.t() );
//     reco::RecoChargedCandidate trk_p4( trk.charge(), p4_2 );
//     trk_p4.setTrack( tau->leadTrack() );
//     histoSignalTrack_->fill( &trk_p4, isignal, weight );
//   }
//   for ( unsigned int iisolation = 0; iisolation < tau->isolationTracks().size(); iisolation++ ) {
//     const reco::Track & trk = *( tau->isolationTracks().at(iisolation) );
//     ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> > p4;
//     p4.SetPt( trk.pt() );
//     p4.SetEta( trk.eta() );
//     p4.SetPhi( trk.phi() );
//     p4.SetM( M_PION );
//     reco::Particle::LorentzVector p4_2( p4.x(), p4.y(), p4.z(), p4.t() );
//     reco::RecoChargedCandidate trk_p4( trk.charge(), p4_2 );
//     trk_p4.setTrack( tau->leadTrack() );
//     histoIsolationTrack_->fill( &trk_p4, iisolation , weight);
//   }

//   h_emEnergyFraction_->fill( tau->emEnergyFraction(), iTau, weight );
//   h_eOverP_ ->fill( tau->eOverP() , iTau, weight );
}


void HistoTau::fillCollection( const edm::View<Tau> & coll, double weight ) 
{

  h_size_->fill( coll.size(), 1, weight );     //! Save the size of the collection.

  edm::View<Tau>::const_iterator
    iobj = coll.begin(),
    iend = coll.end();

  uint i = 1;              //! Fortran-style indexing
  for ( ; iobj != iend; ++iobj, ++i ) {
    fill( &*iobj, i, weight);      //! &*iobj dereferences to the pointer to a PHYS_OBJ*
  } 
}


void HistoTau::clearVec()
{
  HistoGroup<Tau>::clearVec();

  h_emEnergyFraction_->clearVec();
  h_eOverP_->clearVec();
}
