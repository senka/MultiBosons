#ifndef ElectroWeakAnalysis_MultiBosons_plugins_Cleaner_h
#define ElectroWeakAnalysis_MultiBosons_plugins_Cleaner_h
//
// $Id: Cleaner.h,v 1.1 2010/10/06 12:57:09 veverka Exp $
//

/**
  \class    pat::Cleaner Cleaner.h "ElectroWeakAnalysis/MultiBosons/plugins/Cleaner.h"
  \brief    Cleaner module for generic objects - adapted from the PAT Cleaner
            
            The same module is used for all collections.

  \author   Giovanni Petrucciani
  \version  $Id: Cleaner.h,v 1.1 2010/10/06 12:57:09 veverka Exp $
*/


#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "CommonTools/Utils/interface/StringObjectFunction.h"
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"

#include "PhysicsTools/PatAlgos/interface/OverlapTest.h"
#include <boost/ptr_container/ptr_vector.hpp>

namespace vgamma {

  template<class ObjType>
  class Cleaner : public edm::EDProducer {
    public:
      explicit Cleaner(const edm::ParameterSet & iConfig);
      virtual ~Cleaner() {}

      virtual void produce(edm::Event & iEvent, const edm::EventSetup & iSetup);

    private:
      typedef StringCutObjectSelector<ObjType> Selector;

      edm::InputTag src_;
      bool doPreselection_, doFinalCut_;  
      Selector preselectionCut_;
      Selector finalCut_;

      typedef pat::helper::OverlapTest OverlapTest;
      // make a list of overlap tests (ptr_vector instead of std::vector because they're polymorphic)
      boost::ptr_vector<OverlapTest> overlapTests_;
  };

} // namespace

template <class ObjType>
vgamma::Cleaner<ObjType>::Cleaner(const edm::ParameterSet & iConfig) :
    src_(iConfig.getParameter<edm::InputTag>("src")),
    preselectionCut_(iConfig.getParameter<std::string>("preselection")),
    finalCut_(iConfig.getParameter<std::string>("finalCut"))
{
    // pick parameter set for overlaps
    edm::ParameterSet overlapPSet = iConfig.getParameter<edm::ParameterSet>("checkOverlaps");
    // get all the names of the tests (all nested PSets in this PSet)
    std::vector<std::string> overlapNames = overlapPSet.getParameterNamesForType<edm::ParameterSet>();
    // loop on them
    for (std::vector<std::string>::const_iterator itn = overlapNames.begin(); itn != overlapNames.end(); ++itn) {
        // retrieve configuration
        edm::ParameterSet cfg = overlapPSet.getParameter<edm::ParameterSet>(*itn);
        // skip empty parameter sets
        if (cfg.empty()) continue; 
        // get the name of the algorithm to use
        std::string algorithm = cfg.getParameter<std::string>("algorithm");
        // create the appropriate OverlapTest
        if (algorithm == "byDeltaR") {
            overlapTests_.push_back(new pat::helper::BasicOverlapTest(*itn, cfg));
        } else if (algorithm == "bySuperClusterSeed") {
            overlapTests_.push_back(new pat::helper::OverlapBySuperClusterSeed(*itn, cfg));
        } else {
            throw cms::Exception("Configuration") << "Cleaner for " << src_ << ": unsupported algorithm '" << algorithm << "'\n";
        }
    }
        

    produces<std::vector<ObjType> >();
}

template <class ObjType>
void 
vgamma::Cleaner<ObjType>::produce(edm::Event & iEvent, const edm::EventSetup & iSetup) {
  using namespace edm;

  // Read the input. We use View<> in case the input happes to be something different than a std::vector<>
  Handle<View<ObjType> > candidates;
  iEvent.getByLabel(src_, candidates);

  // Prepare a collection for the output
  std::auto_ptr< std::vector<ObjType> > output(new std::vector<ObjType>());

  // initialize the overlap tests
  for (boost::ptr_vector<OverlapTest>::iterator itov = overlapTests_.begin(), edov = overlapTests_.end(); itov != edov; ++itov) {
    itov->readInput(iEvent,iSetup);
  }

  for (typename View<ObjType>::const_iterator it = candidates->begin(), ed = candidates->end(); it != ed; ++it) {
      // Apply a preselection to the inputs and copy them in the output
      if (!preselectionCut_(*it)) continue; 

      // Add it to the list and take a reference to it, so it can be modified (e.g. to set the overlaps)
      // If at some point I'll decide to drop this item, I'll use pop_back to remove it
      output->push_back(*it);
      ObjType &obj = output->back();

      // Look for overlaps
      bool badForOverlap = false;
      for (boost::ptr_vector<OverlapTest>::iterator itov = overlapTests_.begin(), edov = overlapTests_.end(); itov != edov; ++itov) {
        reco::CandidatePtrVector overlaps;
        bool hasOverlap = itov->fillOverlapsForItem(obj, overlaps);
        if (hasOverlap && itov->requireNoOverlaps()) { 
            badForOverlap = true; // mark for discarding
            break; // no point in checking the others, as this item will be discarded
        }
//         obj.setOverlaps(itov->name(), overlaps);  // works for PATObjects only!
      }
      if (badForOverlap) { output->pop_back(); continue; }

      // Apply one final selection cut
      if (!finalCut_(obj)) output->pop_back();
  }

  iEvent.put(output);
}


#endif
