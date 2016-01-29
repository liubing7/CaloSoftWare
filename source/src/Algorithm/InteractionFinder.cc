#include "Algorithm/InteractionFinder.h"

namespace algorithm
{
  bool InteractionFinder::Run(std::vector<caloobject::CaloCluster*> &clusters, float *params)
  {
    Distance<caloobject::CaloCluster, CLHEP::Hep3Vector> dist;
    CLHEP::Hep3Vector vec;    
    for(std::vector<caloobject::CaloCluster*>::iterator it=clusters.begin(); it!=clusters.end(); ++it){

      vec = CLHEP::Hep3Vector(params[0]+params[1]*(*it)->getPosition().z(),
			      params[2]+params[3]*(*it)->getPosition().z(),
			      (*it)->getPosition().z());
      if( (*it)->getHits().size()<settings.minSize || 
	  dist.getDistance( (*it),vec ) < settings.maxRadius )
	continue;

      int counter=0;
      
      for(std::vector<caloobject::CaloCluster*>::iterator jt=it+1; jt!=clusters.end(); ++jt){
	vec = CLHEP::Hep3Vector(params[0]+params[1]*(*jt)->getPosition().z(),
				params[2]+params[3]*(*jt)->getPosition().z(),
				(*jt)->getPosition().z());
	if( (*jt)->getHits().size() >= settings.minSize && 
	    (*jt)->getPosition().z()-(*it)->getPosition().z() > std::numeric_limits<float>::epsilon() &&
	    (*jt)->getLayerID()-(*it)->getLayerID() < settings.maxDepth&&
	    dist.getDistance( (*jt),vec ) < settings.maxRadius )
	  counter++;
      }
      if(counter>=settings.minNumberOfCluster)
	return true;
    }
    return false;
  }

}
