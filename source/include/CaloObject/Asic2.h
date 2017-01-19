#ifndef ASIC2_HH
#define ASIC2_HH

#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <algorithm>
#include <CLHEP/Vector/ThreeVector.h>
#include <CaloObject/CaloLayer.h>

#include <CaloObject/Pad.h>

/*
namespace caloobject
{

class Asic
{
	public :
		Asic(int theKey) ;
		virtual ~Asic(){ ; }
		virtual void Update(caloobject::CaloLayer* layer) ;

		inline int getAsicKey() const { return key ; }
		inline int getAsicNtrack() const { return ntrack ; }
		inline float getAsicEfficiency() const { return ntrack > 0 ? efficiency/ntrack : 0 ; }
		inline float getAsicMultiplicity() const { return efficiency > std::numeric_limits<float>::epsilon() ? multi/efficiency : 0.0 ; }
		inline float getAsicRMSMultiplicity() const { return efficiency > std::numeric_limits<float>::epsilon() ? std::sqrt( multisq/efficiency - (multi/efficiency)*(multi/efficiency) ) : 0.0 ; }
		inline float getAsicMultiplicityError() const { return efficiency > std::numeric_limits<float>::epsilon() ? getAsicRMSMultiplicity()/std::sqrt(efficiency) : 0.0 ; }
		inline const CLHEP::Hep3Vector &getPosition() { return positions ; }

		inline void setASIC_ID(int asicnum, int difnum) {asic_num = asicnum ; dif_num = difnum ; }
		inline int getAsicNum() const { return asic_num ; }
		inline int getAsicDifNum() const { return dif_num ; }

	protected :
		int key ;
		int ntrack ;
		float efficiency ;
		float multi ;
		float multisq ;
		float rms_multi ;

		int dif_num ;
		int asic_num ;
		CLHEP::Hep3Vector positions ;
} ;

class SDHCAL_Asic : public Asic
{
	public :
		SDHCAL_Asic(int theKey) ;
		~SDHCAL_Asic(){ ; }
		inline float getAsicEfficiency2() const { return efficiency2/ntrack ; }
		inline float getAsicEfficiency3() const { return efficiency3/ntrack ; }
		void Update(caloobject::CaloLayer* layer) ;

	private :
		float efficiency2 ;
		float efficiency3 ;
} ;

class SDHCAL_AsicCustom : public Asic  //with analog hits for polya studies
{
	public :
		SDHCAL_AsicCustom(int theKey) ;
		~SDHCAL_AsicCustom(){ ; }

		void setPoints( std::vector<double> _thresholds) { thresholds = _thresholds ; efficiencies = std::vector<int>(thresholds.size() , 0) ; }

		inline std::vector<double> getThresholds() const { return thresholds ; }
		std::vector<double> getEfficiencies() const ;
		std::vector<double> getEfficienciesError() const ;

		void Update(caloobject::CaloLayer* layer) ;

	private :
		std::vector<double> thresholds ;
		std::vector<int> efficiencies ;
} ;

}
*/
#endif
