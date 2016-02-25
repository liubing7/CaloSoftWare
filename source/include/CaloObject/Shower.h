#ifndef SHOWER_HH
#define SHOWER_HH

#include <iostream>
#include <vector>
#include <cstring>

#include <CaloObject/CaloHit.h>
#include <CaloObject/CaloCluster.h>
#include <CLHEP/Vector/ThreeVector.h>

namespace caloobject
{
  class Shower
  {
  public:
    Shower(std::vector<caloobject::CaloCluster*> &vec);
    ~Shower(){;}
    
    inline CLHEP::Hep3Vector getStartingPosition(){return startingPosition;}
    inline float getEnergy(){return energy;}
    inline int getNhit(){return nhit;}
    inline int getNlayer(){return nlayer;}
    inline float* getThrust(){return thrust;}
    inline float getReconstructedCosTheta(){return reconstructedCosTheta;}
    inline float getTransverseRatio(){return transverseRatio;}

  protected:
    std::vector<caloobject::CaloCluster*> clusters;
    CLHEP::Hep3Vector startingPosition;
    float energy;
    int nhit;
    int nlayer;
    //shower thrust ; x = thrust[0] + thrust[1]*z ; y = thrust[2] + thrust[3]*z ; 
    float thrust[4];
    float reconstructedCosTheta;
    float transverseRatio;
  };

}
#endif
