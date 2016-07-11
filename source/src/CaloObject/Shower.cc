#include "CaloObject/Shower.h"

namespace caloobject
{
  Shower::Shower(std::vector<caloobject::CaloHit*> &vec)
  {
    hits=vec;
    energy = 0.0;
    nlayer = 0;
    reconstructedCosTheta = 0.0;
    transverseRatio = 0.0;
    firstIntCluster=NULL;
  }
  
  Shower::Shower(std::vector<caloobject::CaloCluster2D*> &vec)
  {
    clusters=vec;
    for( std::vector<caloobject::CaloCluster2D*>::iterator it=vec.begin(); it!=vec.end(); ++it ){
      clustersEnergy.push_back( (*it)->getEnergy() );
      for( std::vector<caloobject::CaloHit*>::iterator jt=(*it)->getHits().begin(); jt!=(*it)->getHits().end(); ++jt )
      hits.push_back(*jt);
    }
    energy = 0.0;
    nlayer = 0;
    reconstructedCosTheta = 0.0;
    transverseRatio = 0.0;
    firstIntCluster=NULL;
  }

  Shower::Shower( caloobject::CaloCluster3D* cluster )
  {
    hits=cluster->getHits();
    energy = 0.0;
    nlayer = 0;
    reconstructedCosTheta = 0.0;
    transverseRatio = 0.0;
    firstIntCluster=NULL;
  }

  SDHCALShower::SDHCALShower(std::vector<caloobject::CaloCluster2D*> &vec) : Shower(vec)
  {
    sdnhit[0] = 0;
    sdnhit[1] = 0;
    sdnhit[2] = 0;
    nhitAtMax = 0;
  }

}
