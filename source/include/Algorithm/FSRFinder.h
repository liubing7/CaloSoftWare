#ifndef FSRFINDER_HH
#define FSRFINDER_HH

#include "CaloObject/CaloHit.h"
#include "CaloObject/CaloTrack.h"
#include "CaloObject/CaloGeom.h"
#include "CaloObject/CaloCluster.h"
#include "Algorithm/Cluster3D.h"
#include "Algorithm/ShowerAnalyser.h"

namespace algorithm
{ 

  struct FSRFinderParameterSetting{
    float minTransverseRatio;
    float minMeanEdepPerCell;
    float minRMSEdepPerCell;
    float minEdepPerX0;
    float minSurroundedEdepFraction;
    bool printDebug;
    std::vector<float> minEnergyFraction;
    caloobject::GeomParameterSetting geometry;
    algorithm::cluster3DParameterSetting clustering;
    algorithm::ShowerAnalyserParameterSetting showerAnalyser;
  FSRFinderParameterSetting() : minTransverseRatio(0.02) ,
      minMeanEdepPerCell(0.001),
      minRMSEdepPerCell(0.001),
      minEdepPerX0(0.1),
      minSurroundedEdepFraction(0.8),
      printDebug(false)
    {;}
  };

  class FSRFinder
  {
  public:
    FSRFinder(){;}
    ~FSRFinder(){;}

    void Run(std::vector<caloobject::CaloHit*> hits, caloobject::CaloTrack* track, caloobject::CaloCluster3D* gammaCluster);
    inline void SetFSRFinderParameterSetting(FSRFinderParameterSetting val){settings=val;}
    
  private:
    void rmTrackedHitsFromList(std::vector<caloobject::CaloHit*> &hits, caloobject::CaloTrack* track);
    bool isGammaCluster( caloobject::CaloCluster3D* fsrCandidate );
    FSRFinderParameterSetting settings;
  };

}
#endif
