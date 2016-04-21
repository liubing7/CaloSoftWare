#ifndef INTERACTIONFINDER_HH
#define INTERACTIONFINDER_HH

#include <limits>
#include "CaloObject/CaloCluster.h"
#include "Algorithm/Distance.h"
#include <CLHEP/Vector/ThreeVector.h>

namespace algorithm
{ 
  struct InteractionFinderParameterSetting{
    int minSize; 
    float maxRadius;
    int maxDepth;
    int minNumberOfCluster;
    bool useAnalogEnergy;
    float minEnergy;
    bool printDebug;
  InteractionFinderParameterSetting() : minSize(4) ,
                                        maxRadius(50.0) ,
                                        maxDepth(4) ,
                                        minNumberOfCluster(3),
                                        useAnalogEnergy(false),
                                        minEnergy(0.001),
                                        printDebug(false)
                                        {;}
    // minSize : minimum cluster size to have an interaction
    // maxRadius : maximum transversal distance from event thrust to find interaction point
    // maxDepth : maximum depth (in layer unit) to search clusters to find an interaction
    // minNumberOfCluster : minimum number of clusters to find after a candidate interaction to define a real interaction
    // useAnalogEnergy : set true to use cluster energy rather than topology (siwcal), 
    // minEnergy : minimum cluster energy to define interaction, default value corresponds to 1 MeV
  };

  class InteractionFinder
  {
  public:
    InteractionFinder(){;}
    ~InteractionFinder(){;}
    void Run(std::vector<caloobject::CaloCluster*> &clusters, const std::vector<float> params);
    void SetInteractionFinderParameterSetting(InteractionFinderParameterSetting params){settings=params;}
    bool FindInteraction(){return findInteraction;}
    caloobject::CaloCluster* getFirstInteractionCluster(){return firstCluster;}
  private:
    InteractionFinderParameterSetting settings;
    bool findInteraction;
    caloobject::CaloCluster* firstCluster;
  };
}
#endif
