#ifndef SHOWERANALYSER_HH
#define SHOWERANALYSER_HH

#include <iostream>
#include <cmath>
#include <vector>

#include <CaloObject/Shower.h>
#include <CaloObject/CaloGeom.h>
#include <Algorithm/InteractionFinder.h>

namespace algorithm
{

struct ShowerAnalyserParameterSetting
{
		std::string energyCalibrationOption = "SiWEcal" ; //not implemented yet
		float maximumRadius = 1000.0f ; //define the radius of the most outer ring for transverse profile
		std::vector<float> energyCalibrationFactors ;
		algorithm::InteractionFinderParameterSetting interactionFinderParams ;
		caloobject::GeomParameterSetting geometry ;
		ShowerAnalyserParameterSetting()
			: energyCalibrationFactors( {1.0f} ) , interactionFinderParams() , geometry()
		{}
} ;

class ShowerAnalyser
{
	public :
		ShowerAnalyser()
			: settings()
		{;}
		~ShowerAnalyser(){;}
		void Run(caloobject::Shower *shower) ;
		void Run(caloobject::SDHCALShower *shower) ;

		inline void SetShowerAnalyserParameterSetting(ShowerAnalyserParameterSetting params){ settings = params ; }

	private :
		void FindEnergy(caloobject::Shower *shower) ;
		void FindEnergy(caloobject::SDHCALShower *shower) ;
		void SearchShowerInteraction(caloobject::Shower *shower) ;
		void Profile(caloobject::Shower *shower) ;
		void Profile(caloobject::SDHCALShower *shower) ;
		ShowerAnalyserParameterSetting settings ;
} ;

} //namespace algorithm

#endif
