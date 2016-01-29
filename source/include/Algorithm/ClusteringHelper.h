#ifndef CLUSTERINGHELPER_HH
#define CLUSTERINGHELPER_HH

#include "CaloObject/CaloCluster.h"
#include <cmath>
#include <algorithm>

namespace algorithm
{
  struct ClusteringHelperParameterSetting{
    float longitudinalDistance;
    float transversalDistance;
  ClusteringHelperParameterSetting() : longitudinalDistance(100) , 
                                       transversalDistance(200) 
                                       {;}
  };

  class ClusteringHelper
  {
  public:
    ClusteringHelper(){;}
    ~ClusteringHelper(){;}
    inline void SetClusteringHelperParameterSetting(ClusteringHelperParameterSetting params){settings=params;}
    static bool SortClusterByLayer(caloobject::CaloCluster* a, caloobject::CaloCluster* b){return a->getPosition().z() < b->getPosition().z();}
    inline bool IsIsolatedCluster(caloobject::CaloCluster* cl, std::vector<caloobject::CaloCluster*>& list)
    {
      for(std::vector<caloobject::CaloCluster*>::iterator it=list.begin(); it!=list.end(); ++it){
	if( cl==(*it) ) continue;
	if( fabs(cl->getPosition().z()-(*it)->getPosition().z())<settings.longitudinalDistance &&
	    fabs(cl->getPosition().y()-(*it)->getPosition().y())<settings.transversalDistance &&
	    fabs(cl->getPosition().x()-(*it)->getPosition().x())<settings.transversalDistance ){
	  return false;
	}
      }
      return true;
    }
    ClusteringHelperParameterSetting settings;
  };
}
#endif
