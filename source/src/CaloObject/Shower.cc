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
  
  Shower::Shower(std::vector<caloobject::CaloCluster*> &vec)
  {
    clusters=vec;
    for( std::vector<caloobject::CaloCluster*>::iterator it=vec.begin(); it!=vec.end(); ++it )
      for( std::vector<caloobject::CaloHit*>::iterator jt=(*it)->getHits().begin(); jt!=(*it)->getHits().end(); ++jt )
      hits.push_back(*jt);
    energy = 0.0;
    nlayer = 0;
    reconstructedCosTheta = 0.0;
    transverseRatio = 0.0;
    firstIntCluster=NULL;
  }
}
