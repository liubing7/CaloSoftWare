#ifndef CALOCLUSTER_HH
#define CALOCLUSTER_HH

#include "iostream"
#include "vector"
#include <map>

#include "CLHEP/Vector/ThreeVector.h"

#include "CaloObject/CaloHit.h"

namespace caloobject
{
class CaloCluster
{
	public:
		CaloCluster(std::vector<caloobject::CaloHit*> &vec, bool useWeight=0);
		~CaloCluster(){hits.clear();}

		inline std::vector<caloobject::CaloHit*> &getHits() {return hits;}
		inline CLHEP::Hep3Vector getPosition() const {return clusterPosition;}
		inline float getEnergy() const {return energy;}
	protected:
		std::vector<caloobject::CaloHit*> hits;
		CLHEP::Hep3Vector clusterPosition;
		float energy;
};

class CaloCluster2D : public CaloCluster
{
	public:
		CaloCluster2D(std::vector<caloobject::CaloHit*> &vec, bool useWeight=0);
		~CaloCluster2D(){;}
		inline int getLayerID() const {return layerID;}

	private:
		int layerID;
};

class CaloCluster3D : public CaloCluster
{
	public:
		CaloCluster3D(std::vector<caloobject::CaloHit*> &vec, bool useWeight=0);
		~CaloCluster3D();
		inline std::map< int, CLHEP::Hep3Vector > getBarycenter() const {return barycenter;}
		void printInfo();
	private:

		std::map< int, CLHEP::Hep3Vector > barycenter;
};
}
#endif
