#include "CommonTools/Utils/interface/ExpressionHisto.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/FWLite/interface/Event.h"
#include "DataFormats/FWLite/interface/ChainEvent.h"
#include "DataFormats/FWLite/interface/MultiChainEvent.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "FWCore/PythonParameterSet/interface/PythonProcessDesc.h"
#include "FWCore/ParameterSet/interface/ProcessDesc.h"

#include "ElectroWeakAnalysis/MultiBosons/interface/VGammaEventSelector.h"
#include "ElectroWeakAnalysis/MultiBosons/interface/Histogrammer.h"

#include "ElectroWeakAnalysis/MultiBosons/interface/Heartbeat.h"

#include "Math/GenVector/PxPyPzM4D.h"

#include <iostream>
#include <iomanip>
#include <cmath>      //necessary for absolute function fabs()
#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>

//Root includes
#include "TROOT.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TSystem.h"

using namespace std;

bool jsonContainsEvent( std::vector<edm::LuminosityBlockRange>*,
			const edm::EventBase& );

int main ( int argc, char ** argv )
{

  // load framework libraries
  gSystem->Load( "libFWCoreFWLite" );
  AutoLibraryLoader::enable();

  if ( argc < 2 ) {
    std::cout << "Usage : " << argv[0] << " [parameters.py]" << std::endl;
    return 0;
  }
  cout << 
    " _____________________________________________________________________________ \n"
    "|                                                                             |\n"
    "|                                  Welcome to                                 |\n"
    "|               _   _           _____  _    _  _      _  _                    |\n"
    "|              | | | | _   _   |  ___|| |  | || |    (_)| |_  ___             |\n"
    "|              | | | |/_\\_/ /  | |_   | |/\\| || |    | || __|/ _ \\            |\n"
    "|              \\ \\_/ /  \\  /   |  _|  \\  /\\  /| |___ | || |_|  __/            |\n"
    "|               \\___/   / /    |_|     \\/  \\/ |_____||_| \\__|\\___|            |\n"
    "|                      |_/                                                    |\n"
    "|                    _                   _              _                     |\n"
    "|                   / \\    _ __    __ _ | | _   _  ___ (_) ___                |\n"
    "|                  / _ \\  | '_ \\  / _` || || | | |/ __|| |/ __|               |\n"
    "|                 / ___ \\ | | | || (_| || || |_| |\\__ \\| |\\__ \\               |\n"
    "|                /_/   \\_\\|_| |_| \\__,_||_| \\__, ||___/|_||___/               |\n"
    "|                                           |___/                             |\n"
    "|                                                                             |\n"
    "|  Visit https://twiki.cern.ch/twiki/bin/view/CMS/VGammaFramework for help.   |\n"
    "|_____________________________________________________________________________|\n"
     << flush << endl;

  // Parse configuration, get names of input files, output files
  // and the configuration of the selector and histograms.
  PythonProcessDesc builder(argv[1],argc,argv);
  boost::shared_ptr<edm::ParameterSet> cfg = builder.processDesc()->getProcessPSet();
  edm::ParameterSet const& inputs =  cfg->getParameter<edm::ParameterSet>("inputs");
  edm::ParameterSet const& outputs =  cfg->getParameter<edm::ParameterSet>("outputs");
  VGammaEventSelector select(cfg->getParameter<edm::ParameterSet>("SelectorConfig"));

  fwlite::TFileService fs = fwlite::TFileService( outputs.getParameter<std::string>("outputName") );

  Histogrammer<pat::Muon>*     zgmuonHistos(NULL), *wgmuonHistos(NULL);
  Histogrammer<pat::Electron>* zgelectronHistos(NULL), *wgelectronHistos(NULL);
  Histogrammer<pat::Photon>*   zeegphotonHistos(NULL), *zmumugphotonHistos(NULL), 
    *wmunugphotonHistos(NULL),*wenugphotonHistos(NULL), *znunugphotonHistos(NULL);
  Histogrammer<pat::MET>*      znunugmetHistos(NULL), *wenugmetHistos(NULL), *wmunugmetHistos(NULL);

  Histogrammer<reco::CompositeCandidate>* diMuonHistos(NULL);
  Histogrammer<reco::CompositeCandidate>* diElectronHistos(NULL);
  Histogrammer<reco::CompositeCandidate>* muonPlusMETHistos(NULL);
  Histogrammer<reco::CompositeCandidate>* electronPlusMETHistos(NULL);
      
  Histogrammer<reco::CompositeCandidate>* ZMuMuGammaHistos(NULL);
  Histogrammer<reco::CompositeCandidate>* ZEEGammaHistos(NULL);
  Histogrammer<reco::CompositeCandidate>* WMuNuGammaHistos(NULL);
  Histogrammer<reco::CompositeCandidate>* WENuGammaHistos(NULL);
  Histogrammer<reco::CompositeCandidate>* ZNuNuGammaHistos(NULL);

  // vgamma event hypotheses
  if(cfg->existsAs<edm::ParameterSet>("ZMuMuGamma")) {
    edm::ParameterSet histConf = cfg->getParameter<edm::ParameterSet>("ZMuMuGamma");

    ZMuMuGammaHistos = new Histogrammer<reco::CompositeCandidate>(histConf.getParameter<edm::ParameterSet>("ZMuMuGammaHistos"),fs);

    if(histConf.existsAs<edm::ParameterSet>("muonHistos"))
      zgmuonHistos = new Histogrammer<pat::Muon>(histConf.getParameter<edm::ParameterSet>("muonHistos"),fs);
    if(histConf.existsAs<edm::ParameterSet>("dimuonHistos"))
      diMuonHistos = new Histogrammer<reco::CompositeCandidate>(histConf.getParameter<edm::ParameterSet>("diMuonHistos"),fs);
    if(histConf.existsAs<edm::ParameterSet>("photonHistos")) 
      zmumugphotonHistos = new Histogrammer<pat::Photon>(histConf.getParameter<edm::ParameterSet>("photonHistos"),fs);
  }
  if(cfg->existsAs<edm::ParameterSet>("ZEEGamma")) {
    edm::ParameterSet histConf = cfg->getParameter<edm::ParameterSet>("ZEEGamma");

    ZEEGammaHistos = new Histogrammer<reco::CompositeCandidate>(histConf.getParameter<edm::ParameterSet>("ZEEGammaHistos"),fs);

    if(histConf.existsAs<edm::ParameterSet>("electronHistos"))
      zgelectronHistos = new Histogrammer<pat::Electron>(histConf.getParameter<edm::ParameterSet>("electronHistos"),fs);
    if(histConf.existsAs<edm::ParameterSet>("dimuonHistos"))
      diElectronHistos = new Histogrammer<reco::CompositeCandidate>(histConf.getParameter<edm::ParameterSet>("diElectronHistos"),fs);
    if(histConf.existsAs<edm::ParameterSet>("photonHistos")) 
      zeegphotonHistos = new Histogrammer<pat::Photon>(histConf.getParameter<edm::ParameterSet>("photonHistos"),fs);    
  }
  if(cfg->existsAs<edm::ParameterSet>("WMuNuGamma")) {
    edm::ParameterSet histConf = cfg->getParameter<edm::ParameterSet>("WMuNuGamma");
    
    WMuNuGammaHistos = new Histogrammer<reco::CompositeCandidate>(histConf.getParameter<edm::ParameterSet>("WMuNuGammaHistos"),fs);

    if(histConf.existsAs<edm::ParameterSet>("muonHistos"))
      wgmuonHistos = new Histogrammer<pat::Muon>(histConf.getParameter<edm::ParameterSet>("muonHistos"),fs);
    if(histConf.existsAs<edm::ParameterSet>("metHistos"))
      wmunugmetHistos = new Histogrammer<pat::MET>(histConf.getParameter<edm::ParameterSet>("metHistos"),fs);
    if(histConf.existsAs<edm::ParameterSet>("dimuonHistos"))
      muonPlusMETHistos = new Histogrammer<reco::CompositeCandidate>(histConf.getParameter<edm::ParameterSet>("muonPlusMETHistos"),fs);
    if(histConf.existsAs<edm::ParameterSet>("photonHistos")) 
      wmunugphotonHistos = new Histogrammer<pat::Photon>(histConf.getParameter<edm::ParameterSet>("photonHistos"),fs);    
  }
  if(cfg->existsAs<edm::ParameterSet>("WENuGamma")) {
    edm::ParameterSet histConf = cfg->getParameter<edm::ParameterSet>("WENuGamma");

    WENuGammaHistos = new Histogrammer<reco::CompositeCandidate>(histConf.getParameter<edm::ParameterSet>("WENuGammaHistos"),fs);

    if(histConf.existsAs<edm::ParameterSet>("electronHistos"))
      wgelectronHistos = new Histogrammer<pat::Electron>(histConf.getParameter<edm::ParameterSet>("electronHistos"),fs);
    if(histConf.existsAs<edm::ParameterSet>("metHistos"))
      wenugmetHistos = new Histogrammer<pat::MET>(histConf.getParameter<edm::ParameterSet>("metHistos"),fs);
    if(histConf.existsAs<edm::ParameterSet>("dimuonHistos"))
      electronPlusMETHistos = 
	new Histogrammer<reco::CompositeCandidate>(histConf.getParameter<edm::ParameterSet>("electronPlusMETHistos"),fs);
    if(histConf.existsAs<edm::ParameterSet>("photonHistos")) 
      wenugphotonHistos = new Histogrammer<pat::Photon>(histConf.getParameter<edm::ParameterSet>("photonHistos"),fs);    
  }
  if(cfg->existsAs<edm::ParameterSet>("ZNuNuGamma")) {
    edm::ParameterSet histConf = cfg->getParameter<edm::ParameterSet>("ZNuNuGamma");

    ZNuNuGammaHistos = new Histogrammer<reco::CompositeCandidate>(histConf.getParameter<edm::ParameterSet>("ZNuNuGammaHistos"),fs);
    
    if(histConf.existsAs<edm::ParameterSet>("metHistos"))
      znunugmetHistos = new Histogrammer<pat::MET>(histConf.getParameter<edm::ParameterSet>("metHistos"),fs);
    if(histConf.existsAs<edm::ParameterSet>("photonHistos")) 
      znunugphotonHistos = new Histogrammer<pat::Photon>(histConf.getParameter<edm::ParameterSet>("photonHistos"),fs);    
  }

 

  cout << "Reading input file(s) ... " << flush;
  std::vector<std::string> filesToProcess;
  std::map<std::string,std::vector<edm::LuminosityBlockRange>* > filesToLumis;
  std::vector<std::vector<edm::LuminosityBlockRange>* > lumisToProcess;
  // This object 'event' is used both to get all information from the
  // event as well as to store histograms, etc.
  // Process differently depending on if we are fed a list of files or 
  // a list of files with associated LumiBlocks
  if( inputs.existsAs<std::vector<std::string> >("fileNames") ) {
    filesToProcess = inputs.getParameter<std::vector<std::string> >("fileNames");
  } else if( inputs.existsAs<std::vector<edm::ParameterSet> >("fileNames") ) {
    std::vector<edm::ParameterSet> files = inputs.getParameter<std::vector<edm::ParameterSet> >("fileNames");
      for( std::vector<edm::ParameterSet>::const_iterator c = files.begin(); c != files.end(); ++c) {		
	// make map of file names to lumiblock sets
	std::vector<edm::LuminosityBlockRange> const &templumisref = 
	  c->getParameter<std::vector<edm::LuminosityBlockRange> >("lumisToProcess");
	lumisToProcess.push_back(new std::vector<edm::LuminosityBlockRange>());
	lumisToProcess.back()->resize(templumisref.size());
	std::copy(templumisref.begin(),templumisref.end(),lumisToProcess.back()->begin());

	std::vector<std::string> fileNames = c->getParameter<std::vector<std::string> >("fileNames");
	filesToProcess.reserve(filesToProcess.size()+fileNames.size());
	filesToProcess.insert(filesToProcess.end(),fileNames.begin(),fileNames.end());	

	for ( std::vector<std::string>::const_iterator f = fileNames.begin(); f != fileNames.end(); ++f)
	  filesToLumis[*f] = lumisToProcess.back();
      }
  } else {
    throw cms::Exception("InvalidInput") << "fileNames must be of type vstring or VPSet!" << std::endl;
  }
  fwlite::ChainEvent ev ( filesToProcess );
  cout << "done." << endl << flush;
  long long maxEventsInput = -1;
  if (cfg->existsAs<edm::ParameterSet>("maxEvents")) {
    edm::ParameterSet maxEvents = cfg->getParameter<edm::ParameterSet>("maxEvents");
    maxEventsInput = maxEvents.getUntrackedParameter<long long>("input", -1);
  }

  // get heartbeat configuration (very important, I know...)
  edm::ParameterSet hbConf = cfg->getParameter<edm::ParameterSet>("heartbeat");
  unsigned long long totalEvents = ev.size();
  double theprecision = hbConf.getParameter<double>("updateEvery");
  // make the heartbeat
  Heartbeat hb(((maxEventsInput == -1 || (long long)totalEvents < maxEventsInput) ? totalEvents : maxEventsInput),
	       theprecision);

  double iEvent = 0.0;
  //loop through each event
  for (ev.toBegin(), iEvent=0; ! ev.atEnd() && (iEvent < maxEventsInput || maxEventsInput == -1); ++ev, ++iEvent) {

    hb.update(std::cout);

    // only process those events which are in the good runs list.
    if(lumisToProcess.size())
      if(!jsonContainsEvent(filesToLumis[ev.getTFile()->GetName()],ev)) {
	//std::cout << "Event in file: " << ev.getTFile()->GetName() << " not in JSON!" << std::endl;
	continue;
      }

    
        
    edm::EventBase const & event = ev;

    //edm::Handle<edm::TriggerResults> skimResults;
    //event.getByLabel(edm::InputTag("TriggerResults","","PAT"),skimResults);

    const edm::TriggerResultsByName& trbn = event.triggerResultsByName("PAT");

    // get selector bit template and fill
    pat::strbitset event_result = select.getBitTemplate();
    
    if( (select.considerCut("ZMuMuGamma")      && trbn.accept("ZMuMuGammaPath"))     ||
        (select.considerCut("ZEEGamma")        && trbn.accept("ZEEGammaPath"))       ||
	(select.considerCut("WMuNuGamma")      && trbn.accept("WMuNuGammaPath"))     ||
	(select.considerCut("WENuGamma")       && trbn.accept("WENuGammaPath"))      ||
	(select.considerCut("ZInvisibleGamma") && trbn.accept("ZInvisibleGammaPath"))  ) {
      select(event,event_result);
    } else { continue; } 
    
    if(ZMuMuGammaHistos) {
      if(zgmuonHistos)       zgmuonHistos->analyze(select.allMuons());
      if(diMuonHistos)       diMuonHistos->analyze(select.allDiMuons());
      if(zmumugphotonHistos) zmumugphotonHistos->analyze(select.allPhotons());
      
      ZMuMuGammaHistos->analyze(select.allZMuMuGammaCands());

      if(event_result[std::string("ZMuMuGamma")] &&
	 select.considerCut("ZMuMuGamma")) {

	if(zgmuonHistos)       zgmuonHistos->analyze(select.selectedZGammaMuons());
	if(diMuonHistos)       diMuonHistos->analyze(select.selectedZGammaDiMuons());
	if(zmumugphotonHistos) zmumugphotonHistos->analyze(select.selectedZGammaPhotons());
      
	ZMuMuGammaHistos->analyze(select.selectedZMuMuGammaCands());
      }
    }
    if(ZEEGammaHistos) {
      if(zgelectronHistos) zgelectronHistos->analyze(select.allElectrons());
      if(diElectronHistos) diElectronHistos->analyze(select.allDiElectrons());
      if(zeegphotonHistos) zeegphotonHistos->analyze(select.allPhotons());
	
      ZEEGammaHistos->analyze(select.allZEEGammaCands());

      if(event_result[std::string("ZEEGamma")] &&
	 select.considerCut("ZEEGamma")) {
	if(zgelectronHistos) zgelectronHistos->analyze(select.selectedZGammaElectrons());
	if(diElectronHistos) diElectronHistos->analyze(select.selectedZGammaDiElectrons());
	if(zeegphotonHistos) zeegphotonHistos->analyze(select.selectedZGammaPhotons());
	
	ZEEGammaHistos->analyze(select.selectedZEEGammaCands());
      }    
    }
    if(WMuNuGammaHistos) {
      if(wgmuonHistos)       wgmuonHistos->analyze(select.allMuons());
      if(muonPlusMETHistos)  muonPlusMETHistos->analyze(select.allMuonPlusMETs());
      if(wmunugmetHistos)    wmunugmetHistos->analyze(select.allMETs());
      if(wmunugphotonHistos) wmunugphotonHistos->analyze(select.allPhotons());
      
      WMuNuGammaHistos->analyze(select.allWMuNuGammaCands());

      if(event_result[std::string("WMuNuGamma")] &&
	 select.considerCut("WMuNuGamma")) { 
	if(wgmuonHistos)       wgmuonHistos->analyze(select.selectedWGammaMuons());
	if(muonPlusMETHistos)  muonPlusMETHistos->analyze(select.selectedWGammaMuonPlusMETs());
	if(wmunugmetHistos)    wmunugmetHistos->analyze(select.selectedWGammaMETs());
	if(wmunugphotonHistos) wmunugphotonHistos->analyze(select.selectedWGammaPhotons());
	
	WMuNuGammaHistos->analyze(select.selectedWMuNuGammaCands());
      }
    }
    if(WENuGammaHistos) {
      if(wgelectronHistos)       wgelectronHistos->analyze(select.allElectrons());
      if(electronPlusMETHistos)  electronPlusMETHistos->analyze(select.allElectronPlusMETs());
      if(wenugmetHistos)         wenugmetHistos->analyze(select.allMETs());
      if(wenugphotonHistos)      wenugphotonHistos->analyze(select.allPhotons());
      
      WENuGammaHistos->analyze(select.allWENuGammaCands());

      if(event_result[std::string("WENuGamma")] &&
	 select.considerCut("WENuGamma")) {
	if(wgelectronHistos)       wgelectronHistos->analyze(select.selectedWGammaElectrons());
	if(electronPlusMETHistos)  electronPlusMETHistos->analyze(select.selectedWGammaElectronPlusMETs());
	if(wenugmetHistos)         wenugmetHistos->analyze(select.selectedWGammaMETs());
	if(wenugphotonHistos)      wenugphotonHistos->analyze(select.selectedWGammaPhotons());
	
	WENuGammaHistos->analyze(select.selectedWENuGammaCands());
      }  
    }
    /*
    if(ZNuNuGammaHistos) {
      if(znunugmetHistos)              znunugmetHistos->analyze(select.allMETs());
      if(znunugphotonHistos)           znunugphotonHistos->analyze(select.allPhotons());
      
      ZNuNuGammaHistos->analyze(select.allZNuNuGammaCands());

      if(event_result[std::string("ZInivisibleGamma")] &&
	 select.considerCut("ZInvisibleGamma")) {
	if(znunugmetHistos)              znunugmetHistos->analyze(select.photonPlusMETSelector().selectedMETs());
	if(znunugphotonHistos)           znunugphotonHistos->analyze(select.photonPlusMETSelector().selectedPhotons());
      
	ZNuNuGammaHistos->analyze(select.selectedZNuNuGammaCands());
      }    
    }
    */
  } // end loop over events
  
  select.printSelectors(std::cout);  

  if(zgmuonHistos)       delete zgmuonHistos;
  if(wgmuonHistos)       delete wgmuonHistos;
  if(zgelectronHistos)   delete zgelectronHistos;
  if(wgelectronHistos)   delete wgelectronHistos;
  if(zmumugphotonHistos) delete zmumugphotonHistos;
  if(zeegphotonHistos)   delete zeegphotonHistos;
  if(wmunugphotonHistos) delete wmunugphotonHistos;
  if(wenugphotonHistos)  delete wenugphotonHistos;
  if(znunugphotonHistos) delete znunugphotonHistos;
  if(wenugmetHistos)     delete wenugmetHistos;
  if(wmunugmetHistos)    delete wmunugmetHistos;
  if(znunugmetHistos)    delete znunugmetHistos;
  if(diMuonHistos)       delete diMuonHistos;
  if(diElectronHistos)   delete diElectronHistos;
  if(ZMuMuGammaHistos)   delete ZMuMuGammaHistos;
  if(ZEEGammaHistos)     delete ZEEGammaHistos;
  if(ZNuNuGammaHistos)   delete ZNuNuGammaHistos;
  if(WENuGammaHistos)    delete WENuGammaHistos;
  if(WMuNuGammaHistos)   delete WMuNuGammaHistos;
  
}

bool jsonContainsEvent( std::vector<edm::LuminosityBlockRange>* j,
			const edm::EventBase& e ) {
  
  if(j->empty()) return true;

  bool (*fcn) (edm::LuminosityBlockRange const&,
	       edm::LuminosityBlockID const&) = &edm::contains;

  edm::LuminosityBlockID lumiID(e.id().run(),e.id().luminosityBlock());

  std::vector<edm::LuminosityBlockRange>::const_iterator f = 
    std::find_if(j->begin(),j->end(),
		 boost::bind(fcn,_1,lumiID));

  return f != j->end();
}
