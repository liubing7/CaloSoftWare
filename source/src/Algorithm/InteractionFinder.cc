#include "Algorithm/InteractionFinder.h"

namespace algorithm
{
  void InteractionFinder::Run(std::vector<caloobject::CaloCluster*> &clusters, const std::vector<float> params)
  {
    findInteraction=false;
    firstCluster=NULL;
    Distance<caloobject::CaloCluster, CLHEP::Hep3Vector> dist;
    CLHEP::Hep3Vector vec;
    for(std::vector<caloobject::CaloCluster*>::iterator it=clusters.begin(); it!=clusters.end(); ++it){

      vec = CLHEP::Hep3Vector(params[0]+params[1]*(*it)->getPosition().z(),
			      params[2]+params[3]*(*it)->getPosition().z(),
			      (*it)->getPosition().z());
      if( settings.printDebug ) std::cout << "size = " << (*it)->getHits().size() << "\t"
					  << "layer = " << (*it)->getLayerID() << "\t"
					  << "distance = " << dist.getDistance( (*it),vec ) << "\t"
					  << "energy = " << (*it)->getEnergy()*1000 << std::endl;
      
      if( settings.useAnalogEnergy==false ){
	if( (*it)->getHits().size()<(unsigned int)settings.minSize || 
	    dist.getDistance( (*it),vec ) > settings.maxRadius )
	  continue;
      }
      else{
	if( (*it)->getEnergy()<settings.minEnergy || 
	    dist.getDistance( (*it),vec ) > settings.maxRadius )
	  continue;
      }
      int counter=0;
      
      for(std::vector<caloobject::CaloCluster*>::iterator jt=it+1; jt!=clusters.end(); ++jt){
	vec = CLHEP::Hep3Vector(params[0]+params[1]*(*jt)->getPosition().z(),
				params[2]+params[3]*(*jt)->getPosition().z(),
				(*jt)->getPosition().z());
	
	if( settings.useAnalogEnergy==false ){
	  if( (*jt)->getHits().size() >= (unsigned int)settings.minSize && 
	      (*jt)->getPosition().z()-(*it)->getPosition().z() > std::numeric_limits<float>::epsilon() &&
	      (*jt)->getLayerID()-(*it)->getLayerID() < settings.maxDepth&&
	      dist.getDistance( (*jt),vec ) < settings.maxRadius )
	    counter++;
	}
	else{
	  if( (*jt)->getEnergy() >= settings.minEnergy && 
	      (*jt)->getPosition().z()-(*it)->getPosition().z() > std::numeric_limits<float>::epsilon() &&
	      (*jt)->getLayerID()-(*it)->getLayerID() < settings.maxDepth&&
	      dist.getDistance( (*jt),vec ) < settings.maxRadius )
	    counter++;
	}

	if( settings.printDebug ) std::cout << "\t size = " << (*jt)->getHits().size() << "\t"
					    << "\t layer = " << (*jt)->getLayerID() << "\t"
					    << "\t distance = " << dist.getDistance( (*jt),vec ) << "\t"
					    << "\t energy = " << (*jt)->getEnergy()*1000 << "\t"
					    << "\t counter = " << counter << std::endl;
      }
      if(counter>=settings.minNumberOfCluster){
	firstCluster = (*it);
	findInteraction = true;
	break;
      }
    }
  }
}
