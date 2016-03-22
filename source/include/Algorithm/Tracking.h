#ifndef TRACKING_HH
#define TRACKING_HH

#include <cmath>
#include <algorithm>

#include "CLHEP/Vector/ThreeVector.h"

#include "CaloObject/CaloCluster.h"
#include "CaloObject/CaloTrack.h"
#include "Algorithm/PCA.h"
#include "Algorithm/ClusteringHelper.h"
#include "Algorithm/LinearFit3D.h"
#include "Algorithm/Distance.h"

namespace algorithm
{ 
  struct TrackingParameterSetting{
    float maxTransverseRatio;
    float chiSquareLimit;
    float cosThetaLimit;
    float maxDistance;
    int maxDiffBetweenLayer;
    bool printDebug;
  TrackingParameterSetting() : maxTransverseRatio(0.05) , 
                               chiSquareLimit(100),
                               cosThetaLimit(0.0),
                               maxDistance(15.0),
                               maxDiffBetweenLayer(2),
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
    void splitTrack(caloobject::CaloTrack*);    
    inline void SetTrackingParameterSetting(TrackingParameterSetting params){settings=params;}
    inline float getTransverseRatio(){return _transverseRatio;}

  private:
    TrackingParameterSetting settings;
    float _transverseRatio;
    
  };


  class removeClusterFromTrackIfLayerBiggerThanValue
  {
  public:
  //use :
  //track->getClusters().erase( std::remove_if(track->getClusters().begin(), track->getClusters().end(), removeClusterFromTrackIfLayerBiggerThanValue(val) ) , track->getClusters().end() )
  removeClusterFromTrackIfLayerBiggerThanValue(int val) : _value(val) {}
    bool operator()(caloobject::CaloCluster* cluster) { return cluster->getLayerID() > _value;}
  private:
    int _value;
  };
  
}
#endif
