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
  InteractionFinderParameterSetting() : minSize(4) ,
                                        maxRadius(50.0) ,
                                        maxDepth(4) ,
                                        minNumberOfCluster(3)
                                        {;}
    // minSize : minimum cluster size to have an interaction
    // maxRadius : maximum transversal distance from event thrust to find interaction point
    // maxDepth : maximum depth (in layer unit) to search clusters to find an interaction
    // minNumberOfCluster : minimum number of clusters to find after a candidate interaction to define a real interaction
  };

  class InteractionFinder
  {
  public:
    InteractionFinder(){;}
    ~InteractionFinder(){;}
    bool Run(std::vector<caloobject::CaloCluster*> &clusters, const float *params);
    void SetInteractionFinderParameterSetting(InteractionFinderParameterSetting params){settings=params;}
  private:
    InteractionFinderParameterSetting settings;
  };
}
#endif
