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
    bool useEnergyToWeightPosition;
  clusterParameterSetting() : maxLongitudinal(0) ,
                              maxTransversal(1) ,
                              useDistanceInsteadCellID(false),
                              maxLongitudinalDistance(0.0),
                              maxTransversalDistance(11.0),
                              useEnergyToWeightPosition(false){;}
  };

  class Cluster
  {
  public:
    Cluster(){;}
    ~Cluster(){;}
    
    void Run(std::vector<caloobject::CaloHit*> &hits, std::vector<caloobject::CaloCluster2D*> &outClusterVec);
    inline void SetClusterParameterSetting(clusterParameterSetting val){settings=val;}

  private:
    void BuildCluster(std::vector<caloobject::CaloHit*> &temp,
		      std::vector<caloobject::CaloHit*> &calohit,
		      caloobject::CaloHit* &hit);

    std::vector<caloobject::CaloHit*> _clusterHitList;
    
    clusterParameterSetting settings;
  };
}
#endif
