#ifndef Layer_h
#define Layer_h

#include <iostream>
#include <limits>

#include <CaloObject/Asic.h>

namespace caloobject
{

class Layer
{
	public :
		Layer(int _id) ;
		virtual ~Layer() ;

		void setPosition(CLHEP::Hep3Vector pos) { position = pos ; }

		int getNTrack() const { return nTracks ; }
		double getEfficiency() const { return efficiencies.at(0) ; }
		virtual const std::vector<double>& getEfficiencies() const { return efficiencies ; }
		virtual std::vector<double> getEfficienciesError() const ;

		double getMultiplicity() const ;
		double getMultiplicityError() const ;

		virtual Asic* findAsic(const CLHEP::Hep3Vector& pos) const = 0 ;


		inline int getID() const { return id ; }
		inline const CLHEP::Hep3Vector& getPosition() const { return position ; }
		inline const AsicMap& getAsics() const { return asics ; }


		virtual void update(const CLHEP::Hep3Vector& impactPos , CaloCluster2D* cluster = NULL) = 0 ;

		virtual void buildAsics() = 0 ;

	protected :

		virtual void updateEfficiencies() ;

		int nTracks ;
		std::vector<int> nDetected ;

		double multiSum ;
		double multiSquareSum ;

		std::vector<double> efficiencies ;

		CLHEP::Hep3Vector position ; //top left corner position

		int id ;

		AsicMap asics ;

} ;

class SDHCALLayer : public Layer
{
	public :
		SDHCALLayer(int _id , int difL , int difC , int difR) ;
		virtual ~SDHCALLayer() { ; }

		virtual void update(const CLHEP::Hep3Vector& impactPos , CaloCluster2D* cluster = NULL) ;


		static const int asicTab[12][4] ;
		static const int iAsicTab[48] ;
		static const int jAsicTab[48] ;

		virtual Asic* findAsic(const CLHEP::Hep3Vector& pos) const ;

		virtual void buildAsics() ;

	protected :

		int difID[3] ;
} ;

} //namespace caloobject


#endif //Layer_h
