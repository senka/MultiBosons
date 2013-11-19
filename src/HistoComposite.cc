#include "ElectroWeakAnalysis/MultiBosons/interface/HistoComposite.h"

#include <iostream>

using namespace std;
using namespace reco;
using namespace pat;

HistoComposite::
HistoComposite( std::string dir, std::string candTitle, std::string candName,
		double pt1, double pt2, double m1, double m2, TFileDirectory * parentDir )
  :
  HistoGroup<reco::CompositeCandidate>( dir, candTitle, candName, pt1, pt2, m1, m2, parentDir ),
  candName_(candName)
{
}

HistoComposite::~HistoComposite()
{
  if ( histoMuon_.map.size() > 0 ) {
    HistoMap<HistoMuon>::map_type::iterator begin = histoMuon_.map.begin();
    HistoMap<HistoMuon>::map_type::iterator end = histoMuon_.map.end();
    for ( HistoMap<HistoMuon>::map_type::iterator i = begin;
	  i != end; ++i ) if ( i->second ) delete i->second ;
  }

  if ( histoElectron_.map.size() > 0 ) {
    HistoMap<HistoElectron>::map_type::iterator begin = histoElectron_.map.begin();
    HistoMap<HistoElectron>::map_type::iterator end = histoElectron_.map.end();
    for ( HistoMap<HistoElectron>::map_type::iterator i = begin;
	  i != end; ++i ) if ( i->second ) delete i->second ;
  }

  if ( histoTau_.map.size() > 0 ) {
    HistoMap<HistoTau>::map_type::iterator begin = histoTau_.map.begin();
    HistoMap<HistoTau>::map_type::iterator end = histoTau_.map.end();
    for ( HistoMap<HistoTau>::map_type::iterator i = begin;
	  i != end; ++i ) if ( i->second ) delete i->second ;
  }

  if ( histoJet_.map.size() > 0 ) {
    HistoMap<HistoJet>::map_type::iterator begin = histoJet_.map.begin();
    HistoMap<HistoJet>::map_type::iterator end = histoJet_.map.end();
    for ( HistoMap<HistoJet>::map_type::iterator i = begin;
	  i != end; ++i ) if ( i->second ) delete i->second ;
  }

  if ( histoMET_.map.size() > 0 ) {
    HistoMap<HistoMET>::map_type::iterator begin = histoMET_.map.begin();
    HistoMap<HistoMET>::map_type::iterator end = histoMET_.map.end();
    for ( HistoMap<HistoMET>::map_type::iterator i = begin;
	  i != end; ++i ) if ( i->second ) delete i->second ;
  }

  if ( histoPhoton_.map.size() > 0 ) {
    HistoMap<HistoPhoton>::map_type::iterator begin = histoPhoton_.map.begin();
    HistoMap<HistoPhoton>::map_type::iterator end = histoPhoton_.map.end();
    for ( HistoMap<HistoPhoton>::map_type::iterator i = begin;
	  i != end; ++i ) if ( i->second ) delete i->second ;
  }

  if ( histoTrack_.map.size() > 0 ) {
    HistoMap<HistoTrack>::map_type::iterator begin = histoTrack_.map.begin();
    HistoMap<HistoTrack>::map_type::iterator end = histoTrack_.map.end();
    for ( HistoMap<HistoTrack>::map_type::iterator i = begin;
	  i != end; ++i ) if ( i->second ) delete i->second ;
  }

  if ( histoComposite_.map.size() > 0 ) {
    HistoMap<HistoComposite>::map_type::iterator begin = histoComposite_.map.begin();
    HistoMap<HistoComposite>::map_type::iterator end = histoComposite_.map.end();
    for ( HistoMap<HistoComposite>::map_type::iterator i = begin;
	  i != end; ++i ) if ( i->second ) delete i->second ;
  }
}

void HistoComposite::fill( const reco::CompositeCandidate * cand, double weight )
{


  // Fill 4-vector information for candidate
  HistoGroup<reco::CompositeCandidate>::fill( cand, 1, weight );

  const vector<string> & roles = cand->roles();

  if ( roles.size() != cand->numberOfDaughters() ) {
    cout << "HistoComposite::fill: Error: Nroles should match Ndaughters" << endl;
    return;
  }


  // Now fill information for daughters
  for (unsigned int i = 0; i < cand->numberOfDaughters(); ++i ) {
//      cout << "-------------processing component " << i << endl;
    const reco::Candidate * c = cand->daughter(i);
    string role = roles[i];

//     cout << "Role = " << roles[i] << endl;
//     cout << "pdgid = " << c->pdgId() << endl;
//     cout << "pt = " << c->pt() << endl;

    // Figure out what the candidate is based on type
    const Muon *       pcmuon  = dynamic_cast<const Muon*>( c );
    const Electron *   pcelectron = dynamic_cast<const Electron*>( c );
    const Tau *        pctau = dynamic_cast<const Tau*>( c );
    const Jet *        pcjet = dynamic_cast<const Jet*>( c );
    const MET *        pcmet = dynamic_cast<const MET*>( c );
    const Photon *     pcphoton = dynamic_cast<const Photon *>( c );
    const reco::RecoChargedCandidate *    pctrack = dynamic_cast<const reco::RecoChargedCandidate*>(c);
    const reco::CompositeCandidate * pccomposite = dynamic_cast<const reco::CompositeCandidate*>(c);

    // The pointers might be in shallow clones, so check for that too
    const reco::ShallowClonePtrCandidate * pshallow = dynamic_cast<const reco::ShallowClonePtrCandidate *>(c);

    if ( pcmuon == 0 && c->hasMasterClonePtr() )  pcmuon = dynamic_cast<const pat::Muon*>( &*(c->masterClonePtr()) );
    if ( pcelectron == 0 && c->hasMasterClonePtr() )  pcelectron = dynamic_cast<const pat::Electron*>( &*(c->masterClonePtr()) );
    if ( pctau == 0 && c->hasMasterClonePtr() )  pctau = dynamic_cast<const pat::Tau*>( &*(c->masterClonePtr()) );
    if ( pcjet == 0 && c->hasMasterClonePtr() )  pcjet = dynamic_cast<const pat::Jet*>( &*(c->masterClonePtr()) );
    if ( pcmet == 0 && c->hasMasterClonePtr() )  pcmet = dynamic_cast<const pat::MET*>( &*(c->masterClonePtr()) );
    if ( pcphoton == 0 && c->hasMasterClonePtr() )  pcphoton = dynamic_cast<const pat::Photon*>( &*(c->masterClonePtr()) );

    if ( pctrack == 0 && c->hasMasterClonePtr() )  pctrack = dynamic_cast<const reco::RecoChargedCandidate*>( &*(c->masterClonePtr()) );
    if ( pccomposite == 0 && c->hasMasterClonePtr() )  pccomposite = dynamic_cast<const reco::CompositeCandidate*>( &*(c->masterClonePtr()) );

    // ------------------------------------------------------
    // Fill histograms if the candidate is a muon
    // ------------------------------------------------------
    if      ( pcmuon != 0 ) {
//         cout << "Filling muon" << endl;
       // Here is where we do not yet have a histogram for this role
      if ( histoMuon_.map.find( role ) == histoMuon_.map.end() ) {
	histoMuon_.map[role] = new HistoMuon( role, role, role,
					      pt1_, pt2_, m1_, m2_,
					      currDir_ ) ;
      }
      // Here is if the candidate is a shallow clone, we need to
      // fill kinematic information from the shallow clone and 
      // detector information from the base object
      if ( c->hasMasterClonePtr() ) {
	histoMuon_.map[role]    ->fill( pshallow, 1, weight );
      }
      // Here is if the candidate is a straightforward pointer
      else {
	histoMuon_.map[role]    ->fill( pcmuon, 1, weight );
      }
    }

    // ------------------------------------------------------
    // Fill histograms if the candidate is a electron
    // ------------------------------------------------------
    if      ( pcelectron != 0 ) {
//         cout << "Filling electron" << endl;
       // Here is where we do not yet have a histogram for this role
      if ( histoElectron_.map.find( role ) == histoElectron_.map.end() ) {
	histoElectron_.map[role] = new HistoElectron( role, role, role,
					      pt1_, pt2_, m1_, m2_,
					      currDir_  ) ;
      }
      // Here is if the candidate is a shallow clone, we need to
      // fill kinematic information from the shallow clone and 
      // detector information from the base object
      if ( c->hasMasterClonePtr() ) {
	histoElectron_.map[role]    ->fill( pshallow, 1, weight );
      }
      // Here is if the candidate is a straightforward pointer
      else {
	histoElectron_.map[role]    ->fill( pcelectron, 1, weight );
      }
    }


    // ------------------------------------------------------
    // Fill histograms if the candidate is a tau
    // ------------------------------------------------------
    if      ( pctau != 0 ) {
//         cout << "Filling tau" << endl;
       // Here is where we do not yet have a histogram for this role
      if ( histoTau_.map.find( role ) == histoTau_.map.end() ) {
	histoTau_.map[role] = new HistoTau( role, role, role,
					      pt1_, pt2_, m1_, m2_,
					      currDir_  ) ;
      }
      // Here is if the candidate is a shallow clone, we need to
      // fill kinematic information from the shallow clone and 
      // detector information from the base object
      if ( c->hasMasterClonePtr() ) {
	histoTau_.map[role]    ->fill( pshallow, 1, weight );
      }
      // Here is if the candidate is a straightforward pointer
      else {
	histoTau_.map[role]    ->fill( pctau, 1, weight );
      }
    }


    // ------------------------------------------------------
    // Fill histograms if the candidate is a jet
    // ------------------------------------------------------
    if      ( pcjet != 0 ) {
//         cout << "Filling jet" << endl;
       // Here is where we do not yet have a histogram for this role
      if ( histoJet_.map.find( role ) == histoJet_.map.end() ) {
	histoJet_.map[role] = new HistoJet( role, role, role,
					      pt1_, pt2_, m1_, m2_,
					      currDir_  ) ;
      }
      // Here is if the candidate is a shallow clone, we need to
      // fill kinematic information from the shallow clone and 
      // detector information from the base object
      if ( c->hasMasterClonePtr() ) {
	histoJet_.map[role]    ->fill( pshallow, 1, weight );
      }
      // Here is if the candidate is a straightforward pointer
      else {
	histoJet_.map[role]    ->fill( pcjet, 1, weight );
      }
    }


    // ------------------------------------------------------
    // Fill histograms if the candidate is a met
    // ------------------------------------------------------
    if      ( pcmet != 0 ) {
//         cout << "Filling met" << endl;
       // Here is where we do not yet have a histogram for this role
      if ( histoMET_.map.find( role ) == histoMET_.map.end() ) {
	histoMET_.map[role] = new HistoMET( role, role, role,
					      pt1_, pt2_, m1_, m2_,
					      currDir_  ) ;
      }
      // Here is if the candidate is a shallow clone, we need to
      // fill kinematic information from the shallow clone and 
      // detector information from the base object
      if ( c->hasMasterClonePtr() ) {
	histoMET_.map[role]    ->fill( pshallow, 1, weight );
      }
      // Here is if the candidate is a straightforward pointer
      else {
	histoMET_.map[role]    ->fill( pcmet, 1, weight );
      }
    }



    // ------------------------------------------------------
    // Fill histograms if the candidate is a photon
    // ------------------------------------------------------
    if      ( pcphoton != 0 ) {
//         cout << "Filling photon" << endl;
       // Here is where we do not yet have a histogram for this role
      if ( histoPhoton_.map.find( role ) == histoPhoton_.map.end() ) {
	histoPhoton_.map[role] = new HistoPhoton( role, role, role,
					      pt1_, pt2_, m1_, m2_,
					      currDir_  ) ;
      }
      // Here is if the candidate is a shallow clone, we need to
      // fill kinematic information from the shallow clone and 
      // detector information from the base object
      if ( c->hasMasterClonePtr() ) {
	histoPhoton_.map[role]    ->fill( pshallow, 1, weight );
      }
      // Here is if the candidate is a straightforward pointer
      else {
	histoPhoton_.map[role]    ->fill( pcphoton, 1, weight );
      }
    }

    // ------------------------------------------------------
    // Fill histograms if the candidate is a track
    // ------------------------------------------------------
    if      ( pctrack != 0 ) {
//         cout << "Filling track" << endl;
       // Here is where we do not yet have a histogram for this role
      if ( histoTrack_.map.find( role ) == histoTrack_.map.end() ) {
	histoTrack_.map[role] = new HistoTrack( role, role, role,
					      pt1_, pt2_, m1_, m2_,
					      currDir_  ) ;
      }
      // Here is if the candidate is a shallow clone, we need to
      // fill kinematic information from the shallow clone and 
      // detector information from the base object
      if ( c->hasMasterClonePtr() ) {
	histoTrack_.map[role]    ->fill( pshallow, 1, weight );
      }
      // Here is if the candidate is a straightforward pointer
      else {
	histoTrack_.map[role]    ->fill( pctrack, 1, weight );
      }
    }

    // ------------------------------------------------------
    // Fill histograms if the candidate is a composite
    // ------------------------------------------------------
    if      ( pccomposite != 0 ) {
//       cout << "Filling composite with role " << role << endl;
       // Here is where we do not yet have a histogram for this role
      if ( histoComposite_.map.find( role ) == histoComposite_.map.end() ) {
	histoComposite_.map[role] = new HistoComposite( role, role, role,
							pt1_, pt2_, m1_, m2_,
							currDir_ ) ;
      }
      // Here is if the candidate is a shallow clone, we need to
      // fill kinematic information from the shallow clone and 
      // detector information from the base object
      if ( c->hasMasterClonePtr() ) {
	histoComposite_.map[role]    ->fill( pshallow, weight );
      }
      // Here is if the candidate is a straightforward pointer
      else {
	histoComposite_.map[role]    ->fill( pccomposite, weight );
      }
    }


  }
}




void HistoComposite::fill( const reco::ShallowClonePtrCandidate * pshallow, double weight )
{

  const reco::CompositeCandidate * cand = dynamic_cast<reco::CompositeCandidate const *>(pshallow);

  if ( cand == 0 ) {
    cout << "Error! Was passed a shallow clone that is not at heart a composite candidate" << endl;
    return;
  }

  // Fill 4-vector information for candidate
  HistoGroup<reco::CompositeCandidate>::fill( pshallow, 1, weight );

  const vector<string> & roles = cand->roles();

  if ( roles.size() != cand->numberOfDaughters() ) {
    cout << "HistoComposite::fill: Error: Nroles should match Ndaughters" << endl;
    return;
  }


  // Now fill information for daughters
  for (unsigned int i = 0; i < cand->numberOfDaughters(); ++i ) {
//      cout << "-------------processing component " << i << endl;
    const reco::Candidate * c = cand->daughter(i);
    string role = roles[i];

//     cout << "Role = " << roles[i] << endl;
//     cout << "pdgid = " << c->pdgId() << endl;
//     cout << "pt = " << c->pt() << endl;

    // Figure out what the candidate is based on type
    const Muon *       pcmuon  = dynamic_cast<const Muon*>( c );
    const Electron *   pcelectron = dynamic_cast<const Electron*>( c );
    const Tau *        pctau = dynamic_cast<const Tau*>( c );
    const Jet *        pcjet = dynamic_cast<const Jet*>( c );
    const MET *        pcmet = dynamic_cast<const MET*>( c );
    const Photon *     pcphoton = dynamic_cast<const Photon *>( c );
    const reco::RecoChargedCandidate *    pctrack = dynamic_cast<const reco::RecoChargedCandidate*>(c);
    const reco::CompositeCandidate * pccomposite = dynamic_cast<const reco::CompositeCandidate*>(c);

    // The pointers might be in shallow clones, so check for that too
    const reco::ShallowClonePtrCandidate * pshallow_da = dynamic_cast<const reco::ShallowClonePtrCandidate *>(c);

    if ( pcmuon == 0 && c->hasMasterClonePtr() )  pcmuon = dynamic_cast<const pat::Muon*>( &*(c->masterClonePtr()) );
    if ( pcelectron == 0 && c->hasMasterClonePtr() )  pcelectron = dynamic_cast<const pat::Electron*>( &*(c->masterClonePtr()) );
    if ( pctau == 0 && c->hasMasterClonePtr() )  pctau = dynamic_cast<const pat::Tau*>( &*(c->masterClonePtr()) );
    if ( pcjet == 0 && c->hasMasterClonePtr() )  pcjet = dynamic_cast<const pat::Jet*>( &*(c->masterClonePtr()) );
    if ( pcmet == 0 && c->hasMasterClonePtr() )  pcmet = dynamic_cast<const pat::MET*>( &*(c->masterClonePtr()) );
    if ( pcphoton == 0 && c->hasMasterClonePtr() )  pcphoton = dynamic_cast<const pat::Photon*>( &*(c->masterClonePtr()) );

    if ( pctrack == 0 && c->hasMasterClonePtr() )  pctrack = dynamic_cast<const reco::RecoChargedCandidate*>( &*(c->masterClonePtr()) );
    if ( pccomposite == 0 && c->hasMasterClonePtr() )  pccomposite = dynamic_cast<const reco::CompositeCandidate*>( &*(c->masterClonePtr()) );

    // ------------------------------------------------------
    // Fill histograms if the candidate is a muon
    // ------------------------------------------------------
    if      ( pcmuon != 0 ) {
//         cout << "Filling muon" << endl;
       // Here is where we do not yet have a histogram for this role
      if ( histoMuon_.map.find( role ) == histoMuon_.map.end() ) {
	histoMuon_.map[role] = new HistoMuon( role, role, role,
					      pt1_, pt2_, m1_, m2_,
					      currDir_ ) ;
      }
      // Here is if the candidate is a shallow clone, we need to
      // fill kinematic information from the shallow clone and 
      // detector information from the base object
      if ( c->hasMasterClonePtr() ) {
	histoMuon_.map[role]    ->fill( pshallow_da, 1, weight );
      }
      // Here is if the candidate is a straightforward pointer
      else {
	histoMuon_.map[role]    ->fill( pcmuon, 1, weight );
      }
    }

    // ------------------------------------------------------
    // Fill histograms if the candidate is a electron
    // ------------------------------------------------------
    if      ( pcelectron != 0 ) {
//         cout << "Filling electron" << endl;
       // Here is where we do not yet have a histogram for this role
      if ( histoElectron_.map.find( role ) == histoElectron_.map.end() ) {
	histoElectron_.map[role] = new HistoElectron( role, role, role,
					      pt1_, pt2_, m1_, m2_,
					      currDir_  ) ;
      }
      // Here is if the candidate is a shallow clone, we need to
      // fill kinematic information from the shallow clone and 
      // detector information from the base object
      if ( c->hasMasterClonePtr() ) {
	histoElectron_.map[role]    ->fill( pshallow_da, 1, weight );
      }
      // Here is if the candidate is a straightforward pointer
      else {
	histoElectron_.map[role]    ->fill( pcelectron, 1, weight );
      }
    }


    // ------------------------------------------------------
    // Fill histograms if the candidate is a tau
    // ------------------------------------------------------
    if      ( pctau != 0 ) {
//         cout << "Filling tau" << endl;
       // Here is where we do not yet have a histogram for this role
      if ( histoTau_.map.find( role ) == histoTau_.map.end() ) {
	histoTau_.map[role] = new HistoTau( role, role, role,
					      pt1_, pt2_, m1_, m2_,
					      currDir_  ) ;
      }
      // Here is if the candidate is a shallow clone, we need to
      // fill kinematic information from the shallow clone and 
      // detector information from the base object
      if ( c->hasMasterClonePtr() ) {
	histoTau_.map[role]    ->fill( pshallow_da, 1, weight );
      }
      // Here is if the candidate is a straightforward pointer
      else {
	histoTau_.map[role]    ->fill( pctau, 1, weight );
      }
    }


    // ------------------------------------------------------
    // Fill histograms if the candidate is a jet
    // ------------------------------------------------------
    if      ( pcjet != 0 ) {
//         cout << "Filling jet" << endl;
       // Here is where we do not yet have a histogram for this role
      if ( histoJet_.map.find( role ) == histoJet_.map.end() ) {
	histoJet_.map[role] = new HistoJet( role, role, role,
					      pt1_, pt2_, m1_, m2_,
					      currDir_  ) ;
      }
      // Here is if the candidate is a shallow clone, we need to
      // fill kinematic information from the shallow clone and 
      // detector information from the base object
      if ( c->hasMasterClonePtr() ) {
	histoJet_.map[role]    ->fill( pshallow_da, 1, weight );
      }
      // Here is if the candidate is a straightforward pointer
      else {
	histoJet_.map[role]    ->fill( pcjet, 1, weight );
      }
    }


    // ------------------------------------------------------
    // Fill histograms if the candidate is a met
    // ------------------------------------------------------
    if      ( pcmet != 0 ) {
//         cout << "Filling met" << endl;
       // Here is where we do not yet have a histogram for this role
      if ( histoMET_.map.find( role ) == histoMET_.map.end() ) {
	histoMET_.map[role] = new HistoMET( role, role, role,
					      pt1_, pt2_, m1_, m2_,
					      currDir_  ) ;
      }
      // Here is if the candidate is a shallow clone, we need to
      // fill kinematic information from the shallow clone and 
      // detector information from the base object
      if ( c->hasMasterClonePtr() ) {
	histoMET_.map[role]    ->fill( pshallow_da, 1, weight );
      }
      // Here is if the candidate is a straightforward pointer
      else {
	histoMET_.map[role]    ->fill( pcmet, 1, weight );
      }
    }



    // ------------------------------------------------------
    // Fill histograms if the candidate is a photon
    // ------------------------------------------------------
    if      ( pcphoton != 0 ) {
//         cout << "Filling photon" << endl;
       // Here is where we do not yet have a histogram for this role
      if ( histoPhoton_.map.find( role ) == histoPhoton_.map.end() ) {
	histoPhoton_.map[role] = new HistoPhoton( role, role, role,
					      pt1_, pt2_, m1_, m2_,
					      currDir_  ) ;
      }
      // Here is if the candidate is a shallow clone, we need to
      // fill kinematic information from the shallow clone and 
      // detector information from the base object
      if ( c->hasMasterClonePtr() ) {
	histoPhoton_.map[role]    ->fill( pshallow_da, 1, weight );
      }
      // Here is if the candidate is a straightforward pointer
      else {
	histoPhoton_.map[role]    ->fill( pcphoton, 1, weight );
      }
    }

    // ------------------------------------------------------
    // Fill histograms if the candidate is a track
    // ------------------------------------------------------
    if      ( pctrack != 0 ) {
//         cout << "Filling track" << endl;
       // Here is where we do not yet have a histogram for this role
      if ( histoTrack_.map.find( role ) == histoTrack_.map.end() ) {
	histoTrack_.map[role] = new HistoTrack( role, role, role,
					      pt1_, pt2_, m1_, m2_,
					      currDir_  ) ;
      }
      // Here is if the candidate is a shallow clone, we need to
      // fill kinematic information from the shallow clone and 
      // detector information from the base object
      if ( c->hasMasterClonePtr() ) {
	histoTrack_.map[role]    ->fill( pshallow_da, 1, weight );
      }
      // Here is if the candidate is a straightforward pointer
      else {
	histoTrack_.map[role]    ->fill( pctrack, 1, weight );
      }
    }

    // ------------------------------------------------------
    // Fill histograms if the candidate is a composite
    // ------------------------------------------------------
    if      ( pccomposite != 0 ) {
//       cout << "Filling composite with role " << role << endl;
       // Here is where we do not yet have a histogram for this role
      if ( histoComposite_.map.find( role ) == histoComposite_.map.end() ) {
	histoComposite_.map[role] = new HistoComposite( role, role, role,
							pt1_, pt2_, m1_, m2_,
							currDir_ ) ;
      }
      // Here is if the candidate is a shallow clone, we need to
      // fill kinematic information from the shallow clone and 
      // detector information from the base object
      if ( c->hasMasterClonePtr() ) {
	histoComposite_.map[role]    ->fill( pshallow_da, weight );
      }
      // Here is if the candidate is a straightforward pointer
      else {
	histoComposite_.map[role]    ->fill( pccomposite, weight );
      }
    }


  }
}
