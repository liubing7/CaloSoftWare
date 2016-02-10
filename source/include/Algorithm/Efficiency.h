#ifndef EFFICIENCY_HH
#define EFFICIENCY_HH

#include <iostream>
#include <cmath>
#include <vector>

#include "CaloObject/CaloLayer.h"
#include "CaloObject/CaloCluster.h"
#include "Algorithm/Tracking.h"
#include "Algorithm/Distance.h"

namespace algorithm{

 
  
  struct EfficiencyParameterSetting{
    float maxRadius;
    bool semiDigitalReadout;
    bool printDebug;
    int maxAsicKey;
    TrackingParameterSetting trackingParams;
  EfficiencyParameterSetting() : maxRadius(25.0) ,
                                 semiDigitalReadout(true) ,
                                 printDebug(false),
                                 maxAsicKey(48000)
                                 {;}
  };


  class Efficiency {
  public:
    Efficiency(){;}
    ~Efficiency(){;}
    
    inline void SetEfficiencyParameterSetting(EfficiencyParameterSetting params){settings=params;}
    inline int getAsicKey(){return asicKey;}
    inline CLHEP::Hep3Vector &getExpectedPosition(){return expectedPos;}
    void Run(caloobject::CaloLayer *layer, std::vector<caloobject::CaloCluster*> &clusters);

  private:
    EfficiencyParameterSetting settings;
    int asicKey;
    CLHEP::Hep3Vector expectedPos;
    int findAsicKey(CLHEP::Hep3Vector vec, float padSize, int nPadX, int nPadY);
  };
}
#endif
