#ifndef CALOCLUSTER_HH
#define CALOCLUSTER_HH

#include "CaloObject/CaloHit.h"
#include "iostream"
#include "vector"
#include "CLHEP/Vector/ThreeVector.h"

namespace caloobject
{ 

  class CaloCluster2D
  {
  public:
    CaloCluster2D(std::vector<caloobject::CaloHit*> vec);
    ~CaloCluster2D(){_hits.clear();}

    inline std::vector<caloobject::CaloHit*> &getHits() {return _hits;}
    inline CLHEP::Hep3Vector getPosition() const {return _clusterPosition;}
    inline float getEnergy() const {return _energy;}
    inline int getLayerID() const {return _layerID;}

  private:
    std::vector<caloobject::CaloHit*> _hits;
    CLHEP::Hep3Vector _clusterPosition;
    float _energy;
    int _layerID;
 };

}
#endif
