#ifndef CLUSTER3D_HH
#define CLUSTER3D_HH

#include "CaloObject/CaloHit.h"
#include "CaloObject/CaloCluster.h"
#include "CaloObject/CaloGeom.h"
#include <cmath>
#include <algorithm>

namespace algorithm
{ 

  struct cluster3DParameterSetting{
    int maxLongitudinal;
    float maxTransverseDistance;
    bool useEnergyToWeightPosition;
    bool printDebug;
    caloobject::GeomParameterSetting geometry;
  cluster3DParameterSetting() : maxLongitudinal(3) ,
      maxTransverseDistance(30.0),
      useEnergyToWeightPosition(false),
      printDebug(false)
    {;}
  };

  class Cluster3D
  {
  public:
    Cluster3D(){;}
    ~Cluster3D(){;}

    void Run(std::vector<caloobject::CaloHit*> &hits, std::vector<caloobject::CaloCluster3D*> &outClusterVec);
    void Run(std::vector<caloobject::CaloHit*> &hits, std::vector<caloobject::CaloCluster3D*> &outClusterVec, CLHEP::Hep3Vector seed, CLHEP::Hep3Vector direction);
    inline void SetCluster3DParameterSetting(cluster3DParameterSetting val){settings=val;}

  private:
    void BuildCluster(std::vector<caloobject::CaloHit*> &temp,
		      std::vector<caloobject::CaloHit*> &calohit,
		      caloobject::CaloHit* &hit,
		      CLHEP::Hep3Vector seed,
		      CLHEP::Hep3Vector direction);

    std::vector<caloobject::CaloHit*> _clusterHitList;
    
    cluster3DParameterSetting settings;
  };

}
#endif
