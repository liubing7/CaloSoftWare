#include "CaloObject/CaloHit.h"

namespace caloobject
{


  CaloHit::CaloHit(int* cellID, CLHEP::Hep3Vector vec, float energy, float time, CLHEP::Hep3Vector posShift)
  {
    for(int i=0; i<3; i++)
      _cellID[i]=cellID[i];
    _hitPosition=vec-posShift;
    _energy=energy;
    _time=time;
  }

}
