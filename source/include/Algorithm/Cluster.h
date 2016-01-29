#ifndef CLUSTER_HH
#define CLUSTER_HH

#include "CaloObject/CaloHit.h"
#include "CaloObject/CaloCluster.h"
#include <cmath>
#include <algorithm>

namespace algorithm
{ 

  struct clusterParameterSetting{
    int maxLongitudinal;
    int maxTransversal;
    bool useDistanceInsteadCellID;
    float maxLongitudinalDistance;
    float maxTransversalDistance;
  clusterParameterSetting() : maxLongitudinal(0) ,
                              maxTransversal(1) ,
                              useDistanceInsteadCellID(false),
                              maxLongitudinalDistance(0.0),
                              maxTransversalDistance(11.0){;}
  };

  class Cluster
  {
  public:
    Cluster(){;}
    ~Cluster(){;}
    
    void Run(std::vector<caloobject::CaloHit*> &hits, std::vector<caloobject::CaloCluster*> &outClusterVec);
    inline void SetClusterParameterSetting(clusterParameterSetting val){_settings=val;}

  private:
    void BuildCluster(std::vector<caloobject::CaloHit*> &temp,
		      std::vector<caloobject::CaloHit*> &calohit,
		      caloobject::CaloHit* &hit);

    std::vector<caloobject::CaloHit*> _clusterHitList;
    
    clusterParameterSetting _settings;
  };

  /* class orderClustersWithLayer */
  /* { */
  /* public: */
  /*   orderClustersWithLayer(){;} */
  /*   bool operator()(caloobject::CaloCluster* a, caloobject::CaloCluster* b) */
  /*   {    */
  /*     return a->getPosition().z() < b->getPosition().z(); */
  /*   }    */
  /* }; */

  /* class isolatedCluster */
  /* { */
  /* public: */
  /*   isolatedCluster(){;}// : cluster(cl), clusters(list), depth(dp), transversalDistance(td) {} */
  /*   bool operator() (caloobject::CaloCluster* cl, std::vector<caloobject::CaloCluster*>& list, float dp, float td) const//const{    */
  /*   {      //std::cout << cluster << " " << clusters.size() << " " << depth << " " << transversalDistance << std::endl;       */
  /*     for(std::vector<caloobject::CaloCluster*>::const_iterator it=list.begin(); it!=list.end(); ++it){ */
  /* 	if( cl==(*it) ) continue; */
  /* 	if( fabs(cl->getPosition().z()-(*it)->getPosition().z())<dp && */
  /* 	    fabs(cl->getPosition().y()-(*it)->getPosition().y())<td && */
  /* 	    fabs(cl->getPosition().x()-(*it)->getPosition().x())<td ){ */
  /* 	  return false; */
  /* 	} */
  /*     } */
  /*     return true; */
  /*   } */
  /*  /\* private: *\/ */
  /*  /\*  caloobject::CaloCluster* cluster; *\/ */
  /*  /\*  std::vector<caloobject::CaloCluster*> clusters; *\/ */
  /*  /\*  float depth; *\/ */
  /*  /\*  float transversalDistance; *\/ */
  /* }; */
}
#endif
