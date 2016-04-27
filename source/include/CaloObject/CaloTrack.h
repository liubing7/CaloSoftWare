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
    CaloTrack(std::vector<caloobject::CaloCluster2D*> &vec);
    ~CaloTrack(){;}

    inline void setChi2(float chi2){_chi2=chi2;}
    inline void setTrackParameters(float *params){
      _params.clear();
      for(unsigned int i=0; i<4; i++) _params.push_back(params[i]);
    }

    inline const caloobject::CaloCluster2D* getTrackStartingCluster(){return (*clusters.begin());}
    inline const caloobject::CaloCluster2D* getTrackLastCluster(){return (*(clusters.end()-1));}
    inline std::vector<caloobject::CaloCluster2D*> &getClusters(){return clusters;} // someone may need to rearrange the vector with sort
    inline const float getChi2(){return _chi2;}
    inline const std::vector<float> getTrackParameters(){return _params;} 
    inline const void addCluster(caloobject::CaloCluster2D* cluster){clusters.push_back(cluster);}
    const CLHEP::Hep3Vector expectedTrackProjection(float z)
    {
      return CLHEP::Hep3Vector(_params[0]+_params[1]*z,
			       _params[2]+_params[3]*z,
			       z);
    }
    const float getCosTheta(){
      	CLHEP::Hep3Vector Nx(-1., 0., _params[1]);
	CLHEP::Hep3Vector Ny(0., -1., _params[3]);
	return Nx.cross(Ny).cosTheta();
    }
    inline const CLHEP::Hep3Vector orientationVector(){
      return CLHEP::Hep3Vector(-1., 0., _params[1]).cross(CLHEP::Hep3Vector(0., -1., _params[3]));
    }
  private:

    std::vector<caloobject::CaloCluster2D*> clusters;
    float _chi2;
    std::vector<float> _params;
  };
}
#endif
