#ifndef HOUGH_HH
#define HOUGH_HH

#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <limits>
#include <algorithm>

#include "CLHEP/Vector/ThreeVector.h"

#include "CaloObject/CaloCluster.h"
#include "CaloObject/CaloTrack.h"
#include "Algorithm/Distance.h"
#include "Algorithm/Tracking.h"

namespace algorithm
{

  struct HoughParameterSetting{
    unsigned int thetaSteps;
    unsigned int minimumNBins;
    unsigned int maximumClusterSizeForMip;
    unsigned int maximumNumberOfNeighboursForMip;
    unsigned int maximumNumberOfCoreNeighboursForMip;
    float transversalDistance;
    float globalDistance;
    float padSize;
    bool printDebug;
  HoughParameterSetting() : thetaSteps(100) ,
      minimumNBins(10) ,
      maximumClusterSizeForMip(4),
      maximumNumberOfNeighboursForMip(2),
      maximumNumberOfCoreNeighboursForMip(0),
      transversalDistance(50.0),
      globalDistance(100.0),
      padSize(10.408),
      printDebug(false)
    {;}
  };

  enum HoughTag{
    fMip,
    fTrack,
    fIsolated
  };

  struct HoughObject{
    std::vector<int> thetas;
    std::vector<int> rhoXVec;
    std::vector<int> rhoYVec;
    HoughTag tag;
    caloobject::CaloCluster* cluster;
  };

  struct HoughBin{
    int theta_x, theta_y;
    int rho_x, rho_y;
    std::vector<HoughObject> houghObjects;
  };

  class Hough
  {
  public : 
    Hough(){;}
    ~Hough(){;}
    
    inline void SetHoughParameterSetting(HoughParameterSetting params){settings=params;}
    void runHough(std::vector<caloobject::CaloCluster*> &clusters, std::vector<caloobject::CaloTrack*> &tracks, algorithm::Tracking* algo_Tracking);

  private: 
    void createHoughObjects( std::vector<caloobject::CaloCluster*> &clusters );
    std::vector< HoughBin > getHoughBins();
    inline bool TestHoughBinSize( HoughBin a ){ return a.houghObjects.size() < settings.minimumNBins ; }
    void RemoveIsolatedClusterInHoughBin(HoughBin a);
    void RemoveTrackedObjects(std::vector<HoughBin> &houghBins);

    HoughParameterSetting settings;
    std::vector< HoughObject  > houghObjects;
  };

  class SortHoughBinBySize
  { 
  public: 
    SortHoughBinBySize(){;} 
    static bool Sort(HoughBin a, HoughBin b){return a.houghObjects.size() > b.houghObjects.size() ;} 
  };

}

#endif
