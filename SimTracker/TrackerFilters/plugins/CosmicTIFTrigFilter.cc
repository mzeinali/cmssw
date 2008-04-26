// livio.fano@cern.ch

#include "SimTracker/TrackerFilters/interface/CosmicTIFTrigFilter.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/Common/interface/Handle.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "FastSimulation/BaseParticlePropagator/interface/BaseParticlePropagator.h"
#include "FastSimulation/ParticlePropagator/interface/MagneticFieldMap.h"
#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "HepMC/GenVertex.h"
//#include "CLHEP/Vector/LorentzVector.h"
#include <map>
#include <vector>

using namespace std;
namespace cms

{
  CosmicTIFTrigFilter::CosmicTIFTrigFilter(const edm::ParameterSet& conf):    conf_(conf)
  {
    trigconf  = conf_.getParameter<int>("trig_conf");
  }
  
  bool CosmicTIFTrigFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
  {
    
    
    edm::Handle<edm::HepMCProduct>HepMCEvt;
    iEvent.getByLabel("source","",HepMCEvt);
    const HepMC::GenEvent* MCEvt = HepMCEvt->GetEvent();
    
    bool hit1=false;
    bool hit2=false;
    bool hit3=false;
    bool hit4=false;
    
    
    for(HepMC::GenEvent::particle_const_iterator i=MCEvt->particles_begin(); i != MCEvt->particles_end();++i)
      {
	int myId = (*i)->pdg_id();
	if (abs(myId)==13)
	  {
	    
	    // Get the muon position and momentum
	    HepMC::GenVertex* pv = (*i)->production_vertex();
	    HepMC::FourVector vertex = pv->position();
	    
	    HepMC::FourVector  momentum=(*i)->momentum();

	    //std::cout << "\t vertex for cut = " << vertex << std::endl; 
	    //std::cout << "\t momentum  = " << momentum << std::endl; 
	    
	    if(trigconf==1){

	      HepMC::FourVector S1(350.,1600.,500.,0.);
	      HepMC::FourVector S2(350.,-1600.,400.,0.);
	      HepMC::FourVector S3(350.,1600.,1600.,0.);


	      hit1=Sci_trig(vertex, momentum, S1);
	      hit2=Sci_trig(vertex, momentum, S2);
	      hit3=Sci_trig(vertex, momentum, S3);
	      
	      // trigger conditions
	      
	      if((hit1&&hit2) || (hit3&&hit2))
		{
		  /*
		  cout << "\tGot a trigger in configuration A " << endl; 
		  if(hit1)cout << "hit1 " << endl;
		  if(hit2)cout << "hit2 " << endl;
		  if(hit3)cout << "hit3 " << endl;
		  */
		  trig1++;
		  return true;
		}
	    }else if(trigconf ==2) {

	      HepMC::FourVector S1(350.,1600.,850.,0.);
	      HepMC::FourVector S2(0.,-1550.,-1650.,0.);
	      HepMC::FourVector S3(350.,1600.,2300.,0.);
	      
	      hit1=Sci_trig(vertex, momentum, S1);
	      hit2=Sci_trig(vertex, momentum, S2);
	      hit3=Sci_trig(vertex, momentum, S3);
	      
	      // trigger conditions
	      
	      if((hit1&&hit2) || (hit3&&hit2))
		{
		  /*
		  cout << "\tGot a trigger in configuration B " << endl; 
		  if(hit1)cout << "hit1 " << endl;
		  if(hit2)cout << "hit2 " << endl;
		  if(hit3)cout << "hit3 " << endl;
		  */
		  trig2++;
		  return true;
		}

	    }else if(trigconf ==3) {

	      HepMC::FourVector S1(350.,1600.,850.,0.);
	      HepMC::FourVector S3(350.,1600.,2300.,0.);
	      HepMC::FourVector S2(350.,-1600.,400.,0.);
	      HepMC::FourVector S4(0.,-1600.,-2000.,0.);
	      
	      hit1=Sci_trig(vertex, momentum, S1);
	      hit2=Sci_trig(vertex, momentum, S2);
	      hit3=Sci_trig(vertex, momentum, S3);
	      hit4=Sci_trig(vertex, momentum, S4);
	      
	      // trigger conditions
	      if((hit1&&hit2) || (hit3&&hit2) || (hit1&&hit4) || (hit3&&hit4))
		{
		  cout << "\tGot a trigger in configuration C " << endl; 
		  if(hit1)cout << "hit1 " << endl;
		  if(hit2)cout << "hit2 " << endl;
		  if(hit3)cout << "hit3 " << endl;
		  if(hit4)cout << "hit4 " << endl;
		  trig3++;
		  return true;
		}
	    }
	  }
      }
    
    return false;
  }
  
  bool CosmicTIFTrigFilter::Sci_trig(HepMC::FourVector vertex,  HepMC::FourVector momentum, HepMC::FourVector S)
  {
    float x0= vertex.x();
    float y0= vertex.y();
    float z0= vertex.z();
    float px0=momentum.x();
    float py0=momentum.y();
    float pz0=momentum.z();
    float Sx=S.x();
    float Sy=S.y();
    float Sz=S.z();
    
    //float ys=Sy;
    float zs=(Sy-y0)*(pz0/py0)+z0;
    //	  float xs=((Sy-y0)*(pz0/py0)-z0)*(px0/pz0)+x0;
    float xs=(Sy-y0)*(px0/py0)+x0;
    
    //    cout << Sx << " " << Sz << " " << xs << " " << zs << endl;
    //    cout << x0 << " " << z0 << " " << px0 << " " << py0 << " " << pz0 << endl;
    
    if((xs<Sx+500 && xs>Sx-500) && (zs<Sz+500 && zs>Sz-500) )
      {
	return true;
	cout << "PASSED" << endl;
      }
    else
      {
	return false;
      }
    
  }
  
}

