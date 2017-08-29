#ifndef SHOWER_HH
#define SHOWER_HH

#include <iostream>
#include <vector>
#include <cstring>

#include <CaloObject/CaloHit.h>
#include <CaloObject/CaloCluster.h>
#include <CLHEP/Vector/ThreeVector.h>

namespace algorithm 
{ 

class ShowerAnalyser ;

}  //namespace algorithm

namespace caloobject
{

class Shower
{
		friend class algorithm::ShowerAnalyser ;

	public:
		Shower(std::vector<caloobject::CaloHit*>& vec);
		Shower(std::vector<caloobject::CaloCluster2D*>& vec);
		Shower(caloobject::CaloCluster3D* cluster);
		~Shower(){;}

		inline const std::vector<caloobject::CaloHit*> &getHits() const { return hits ; }
		inline const std::vector<caloobject::CaloCluster2D*> &getClusters() const { return clusters ; }

		inline caloobject::CaloCluster2D* getFirstIntCluster() const { return firstIntCluster ; }
		inline CLHEP::Hep3Vector getStartingPosition() const {return startingPosition ; }
		inline double getLastClusterPosition() const { return lastClusterPosition ; }
		inline unsigned int getNhit() const { return static_cast<unsigned int>( hits.size() ) ; }
		inline float getEnergy(){return energy;}
		inline float getEdep(){return edep;}
		inline float getMeanEdep(){return meanEdep;}
		inline float getRMSEdep(){return rmsEdep;}
		inline int getNlayer(){return nlayer;}
		inline std::vector<float> getThrust() const {return thrust ; }
		inline float getReconstructedCosTheta(){return reconstructedCosTheta;}
		inline float getTransverseRatio(){return transverseRatio;}
		inline float getEta(){return eta;}
		inline float getPhi(){return phi;}

		inline float getShowerMax(){return showerMax;}
		inline float getEdepAtMax(){return edepAtMax;}
		inline std::vector<double> &getEdepPerCell(){return edepPerCell;}
		inline bool findInteraction(){return findInteraction_;}
		inline std::vector<double> &getLongitudinal(){return longitudinal;}
		inline std::vector<double> &getTransverse(){return transverse;}
		inline std::vector<double> &getDistancesToAxis(){return distanceToAxis;}
		inline std::vector<double> &getClustersEnergy(){return clustersEnergy;}
		inline std::vector<double> &getHitTimes(){return hitTimes;}

		Shower(const Shower &toCopy) = delete ;
		void operator=(const Shower &toCopy) = delete ;

	protected :

		std::vector<caloobject::CaloHit*> hits ;
		std::vector<caloobject::CaloCluster2D*> clusters ;
		caloobject::CaloCluster2D* firstIntCluster = nullptr ;
		CLHEP::Hep3Vector startingPosition = CLHEP::Hep3Vector(0,0,0) ;

		double lastClusterPosition = 0 ;

		float energy = 0.0f ;
		float edep = 0.0f ;
		std::vector<double> edepPerCell ;
		std::vector<double> hitTimes ;
		int nlayer = 0 ;
		//shower thrust ; x = thrust[0] + thrust[1]*z ; y = thrust[2] + thrust[3]*z ;
		std::vector<float> thrust ;
		float reconstructedCosTheta = 0.0f ;
		float transverseRatio = 0.0f ;
		float eta = 0.0f ;
		float phi = 0.0f ;

		float meanEdep = 0.0f ;
		float rmsEdep = 0.0f ;
		float showerMax = 0.0f ; //x0 unit
		float edepAtMax = 0.0f ;

		bool findInteraction_ = false ;

		std::vector<double> longitudinal ;
		std::vector<double> transverse ;
		std::vector<double> distanceToAxis ;
		std::vector<double> clustersEnergy ;
} ;

class SDHCALShower : public Shower
{
		friend class algorithm::ShowerAnalyser ;

	public :
		SDHCALShower( std::vector<caloobject::CaloCluster2D*> &vec ) ;
		~SDHCALShower() { ; }
		int* getSDNHits() { return sdnhit ; }
		int getNHitsAtMax() { return nhitAtMax ; }

	protected :
		int sdnhit[3] = {0,0,0} ;
		int nhitAtMax = 0 ;
} ;

} //namespace caloobject

#endif
