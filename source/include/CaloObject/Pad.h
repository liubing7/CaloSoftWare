#ifndef Pad_h
#define Pad_h

#include <limits>
#include <map>

#include <CLHEP/Vector/ThreeVector.h>

#include <CaloObject/CaloCluster.h>


namespace caloobject
{

class Asic ;

class Pad
{
	public :
		Pad(int _id) ;
		virtual ~Pad() ;

		void setAsic(caloobject::Asic* _asic) { asic = _asic ; }
		void setPosition(CLHEP::Hep3Vector pos) { position = pos ; }

		int getNTrack() const { return nTracks ; }
		double getEfficiency() const { return efficiencies.at(0) ; }
		virtual const std::vector<double>& getEfficiencies() const { return efficiencies ; }
		virtual std::vector<double> getEfficienciesError() const ;

		double getMultiplicity() const ;
		double getMultiplicityError() const ;

		inline int getID() const { return id ; }
		inline const CLHEP::Hep3Vector &getPosition() const { return position ; }


		virtual void update(CaloCluster2D* cluster = NULL) = 0 ;


	protected :

		virtual void updateEfficiencies() ;

		caloobject::Asic* asic ;

		int nTracks ;
		std::vector<int> nDetected ;

		double multiSum ;
		double multiSquareSum ;

		std::vector<double> efficiencies ;

		CLHEP::Hep3Vector position ;

		int id ;

} ;

class SDHCALPad : public Pad
{
	public :
		SDHCALPad(int _id) ;
		virtual ~SDHCALPad() ;

		virtual void update(CaloCluster2D* cluster = NULL) ;

	protected :

} ;

} //namespace caloobject

typedef std::map<int , caloobject::Pad*> PadMap ;


#endif //Pad_h
