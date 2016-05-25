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
#include "CaloObject/CaloGeom.h"
#include "Algorithm/Distance.h"
#include "Algorithm/Tracking.h"

namespace algorithm
{

  struct HoughParameterSetting{
    int thetaSteps;
    int minimumNBins;
    int maximumClusterSizeForMip;
    int maximumNumberOfNeighboursForMip;
    int maximumNumberOfCoreNeighboursForMip;
    float transversalDistance;
    int isolationDistance;
    bool printDebug;
    bool useAnalogEnergy;
    float maxEnergy;
    caloobject::GeomParameterSetting geometry;
  HoughParameterSetting() : thetaSteps(100) ,
      minimumNBins(10) ,
      maximumClusterSizeForMip(4),
      maximumNumberOfNeighboursForMip(2),
      maximumNumberOfCoreNeighboursForMip(0),
      transversalDistance(50.0),
      isolationDistance(2),
      printDebug(false),
      useAnalogEnergy(false),
      maxEnergy(0.001) // 1 MeV (MIP arround 85 keV)
    {;}
  };

  enum HoughTag{
    fMip,
    fTrack,
    fIsolated
  };

  struct HoughObject{
    std::vector<int> thetas;
    std::vector<float> rhoXVec;
    std::vector<float> rhoYVec;
    HoughTag tag;
    caloobject::CaloCluster2D* cluster;
  };

  struct HoughBin{
    int theta;
    float rho;
    bool rmTag;
    std::vector< HoughObject* > houghObjects;
  };

  class Hough
  {
  public : 
    Hough(){;}
    ~Hough(){;}
    
    inline void SetHoughParameterSetting(HoughParameterSetting params){settings=params;}
    void runHough(std::vector<caloobject::CaloCluster2D*> &clusters, std::vector<caloobject::CaloTrack*> &tracks, algorithm::Tracking* algo_Tracking);

  private: 
    void createHoughObjects( std::vector<caloobject::CaloCluster2D*> &clusters );
    void selectNonDensePart( std::vector<caloobject::CaloCluster2D*> &clusters, std::vector<caloobject::CaloCluster2D*> &mipCandidate );
    std::vector< HoughBin > getHoughBinsFromZX();
    HoughBin getBestHoughBinFromZY( HoughBin &inputBin );
    void RemoveIsolatedClusterInHoughBin(HoughBin &a);
    void RemoveTrackedObjects(std::vector<HoughBin> &houghBins);

    HoughParameterSetting settings;
    std::vector< HoughObject*  > houghObjects;

    inline bool TestHoughBinSize( HoughBin a ){ return a.houghObjects.size() < settings.minimumNBins ; }
    static bool HasToBeDeleted( HoughBin a ){ return a.rmTag; }
  };

  class SortHoughBinBySize
  { 
  public: 
    SortHoughBinBySize(){;} 
    static bool Sort(HoughBin a, HoughBin b){return a.houghObjects.size() > b.houghObjects.size() ;} 
  };

  class RemoveTrackedObject
  { 
  public:
    RemoveTrackedObject(){;} 
    static bool rm(HoughObject* a){return a->tag==fTrack;}
  };
}

#endif
