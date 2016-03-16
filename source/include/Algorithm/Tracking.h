#ifndef TRACKING_HH
#define TRACKING_HH

#include <cmath>
#include <algorithm>

#include "CLHEP/Vector/ThreeVector.h"

#include "CaloObject/CaloCluster.h"
#include "CaloObject/CaloTrack.h"
#include "Algorithm/PCA.h"
#include "Algorithm/LinearFit3D.h"

namespace algorithm
{ 
  struct TrackingParameterSetting{
    float maxTransverseRatio;
    float chiSquareLimit;
    float cosThetaLimit;
    bool printDebug;
  TrackingParameterSetting() : maxTransverseRatio(0.05) , 
                               chiSquareLimit(100),
                               cosThetaLimit(0.0),
                               printDebug(false)
                               {;}
  };

  class Tracking
  {
  public:
    Tracking(){;}
    ~Tracking(){;}
  
    void Run(std::vector<caloobject::CaloCluster*> &vec, caloobject::CaloTrack* &track);
    void TryToAddAClusterInTrack(caloobject::CaloCluster* cluster, caloobject::CaloTrack* &track);
    
    inline void SetTrackingParameterSetting(TrackingParameterSetting params){settings=params;}
    inline float getTransverseRatio(){return _transverseRatio;}

  private:
    TrackingParameterSetting settings;
    float _transverseRatio;
    
  };

}
#endif
