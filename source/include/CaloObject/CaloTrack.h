#ifndef CALOTRACK_HH
#define CALOTRACK_HH

#include <iostream>
#include <vector>
#include "CLHEP/Vector/ThreeVector.h"
#include "CaloObject/CaloCluster.h"

namespace caloobject
{ 

  class CaloTrack
  {
  public:
    CaloTrack(std::vector<caloobject::CaloCluster*> &vec);
    ~CaloTrack(){;}

    inline void setChi2(float chi2){_chi2=chi2;}
    inline void setTrackParameters(float *params){
      for(unsigned int i=0; i<4; i++) _params[i]=params[i];
    }

    inline const caloobject::CaloCluster* getTrackStartingCluster(){return (*clusters.begin());}
    inline const caloobject::CaloCluster* getTrackLastCluster(){return (*(clusters.end()-1));}
    inline std::vector<caloobject::CaloCluster*> &getClusters(){return clusters;} // someone may need to rearrange the vector with sort
    inline const float getChi2(){return _chi2;}
    inline const float *getTrackParameters(){return _params;} 
    inline const void addCluster(caloobject::CaloCluster* cluster){clusters.push_back(cluster);}
    const CLHEP::Hep3Vector expectedTrackProjection(float z)
    {
      return CLHEP::Hep3Vector(_params[0]+_params[1]*z,
			       _params[2]+_params[3]*z,
			       z);
    }
  private:

    std::vector<caloobject::CaloCluster*> clusters;
    float _chi2;
    float _params[4];
  };
}
#endif
