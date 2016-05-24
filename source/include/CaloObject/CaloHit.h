#ifndef CALOHIT_HH
#define CALOHIT_HH

#include <iostream>
#include <vector>
#include "CLHEP/Vector/ThreeVector.h"


namespace caloobject
{ 

  class CaloHit
  {
  public:
    CaloHit(int* cellID, CLHEP::Hep3Vector vec, float energy, float time, 
	    CLHEP::Hep3Vector posShift);
    // algorithms assume that the zero is located at the middle of the first layer.

    ~CaloHit(){;}

    inline const CLHEP::Hep3Vector getPosition(){return _hitPosition;}
    inline const int* getCellID(){return _cellID;}
    inline const float getEnergy(){return _energy;}
    inline const float getTime(){return _time;}


  private:
    int _cellID[3];
    CLHEP::Hep3Vector _hitPosition;
    float _energy;
    float _time;
  };

}
#endif
