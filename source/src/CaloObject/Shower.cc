#include "CaloObject/Shower.h"

namespace caloobject
{
  Shower::Shower(std::vector<caloobject::CaloHit*> &vec)
  {
    hits=vec;
    startingPosition = CLHEP::Hep3Vector (0.0,0.0,0.0);
    energy = 0.0;
    nlayer = 0;
    reconstructedCosTheta = 0.0;
    transverseRatio = 0.0;
  }
  
}
