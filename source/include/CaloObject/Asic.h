#ifndef Asic_h
#define Asic_h

#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <algorithm>
#include <CLHEP/Vector/ThreeVector.h>

#include <CaloObject/Pad.h>

namespace caloobject
{

class Layer ;

class Asic
{
	public :
		Asic(int _id , int _difID = -1) ;
		virtual ~Asic() ;

		void setLayer(caloobject::Layer* _layer) { layer = _layer ; }
		void setPosition(CLHEP::Hep3Vector pos) { position = pos ; }

		int getNTrack() const { return nTracks ; }
		double getEfficiency() const { return efficiencies.at(0) ; }
		virtual const std::vector<double>& getEfficiencies() const { return efficiencies ; }
		virtual std::vector<double> getEfficienciesError() const ;

		double getMultiplicity() const ;
		double getMultiplicityError() const ;


		virtual Pad* findPad(const CLHEP::Hep3Vector& pos) const = 0 ;

		inline int getID() const { return id ; }
		inline int getDifID() const { return difID ; }
		inline const CLHEP::Hep3Vector &getPosition() const { return position ; }
		inline const PadMap& getPads() const { return pads ; }

		virtual void update(const CLHEP::Hep3Vector& impactPos , CaloCluster2D* cluster = NULL) = 0 ;

		virtual void buildPads() = 0 ;


	protected :

		virtual void updateEfficiencies() ;

		caloobject::Layer* layer ;

		int nTracks ;
		std::vector<int> nDetected ;

		double multiSum ;
		double multiSquareSum ;

		std::vector<double> efficiencies ;

		CLHEP::Hep3Vector position ; //top left corner position

		int id ;
		int difID ;


		PadMap pads ;
} ;

class SDHCALAsic : public Asic
{
	public :
		SDHCALAsic(int _id , int _difID) ;
		virtual ~SDHCALAsic() { ; }

		virtual void update(const CLHEP::Hep3Vector& impactPos , CaloCluster2D* cluster = NULL) ;


		static const int padTab[8][8] ;
		static const int iPadTab[64] ;
		static const int jPadTab[64] ;

		virtual Pad* findPad(const CLHEP::Hep3Vector& pos) const ;

		virtual void buildPads() ;


	protected :

} ;


/*
class SDHCALAsicCustom : public Asic  //with analog hits for polya studies
{
	public :
		SDHCALAsicCustom(int _id) ;
		virtual ~SDHCALAsicCustom(){ ; }

		void setPoints( std::vector<double> _thresholds) { thresholds = _thresholds ; efficiencies = std::vector<int>(thresholds.size() , 0) ; }

		inline std::vector<double> getThresholds() const { return thresholds ; }
//		std::vector<double> getEfficiencies() const ;
//		std::vector<double> getEfficienciesError() const ;

		virtual void update(CLHEP::Hep3Vector impactPos , CaloCluster2D* cluster = NULL) ;

	private :
		std::vector<double> thresholds ;
		std::vector<int> efficiencies ;
} ;
*/


} //namespace caloobject

typedef std::map<int , caloobject::Asic*> AsicMap ;

#endif //Asic_h
