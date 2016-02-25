#include "CaloObject/Shower.h"

namespace caloobject
{
  Shower::Shower(std::vector<caloobject::CaloCluster*> &vec)
  {
    clusters=vec;
    startingPosition = CLHEP::Hep3Vector (0.0,0.0,0.0);
    energy = 0.0;
    nhit = 0;
    nlayer = 0;
    std::memset(thrust,0.0,4*sizeof(float));
    reconstructedCosTheta = 0.0;
    transverseRatio = 0.0;
  }
  
}
