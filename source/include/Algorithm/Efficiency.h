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
    float edgeX_min;
    float edgeX_max;
    float edgeY_min;
    float edgeY_max;
    float maxRadius;
    bool semiDigitalReadout;
    bool printDebug;
    TrackingParameterSetting trackingParams;
  EfficiencyParameterSetting() : edgeX_min(-500.0) ,
                                 edgeX_max(500.0) ,
                                 edgeY_min(-500.0) ,
                                 edgeY_max(500.0) ,
                                 maxRadius(25.0) ,
                                 semiDigitalReadout(true) ,
                                 printDebug(false)
                                 {;}
  };


  class Efficiency {
  public:
    Efficiency(){;}
    ~Efficiency(){;}
    
    inline void SetEfficiencyParameterSetting(EfficiencyParameterSetting params){settings=params;}
    void Run(caloobject::CaloLayer *layer, std::vector<caloobject::CaloCluster*> &clusters);

  private:
    EfficiencyParameterSetting settings;

  };
}
#endif
