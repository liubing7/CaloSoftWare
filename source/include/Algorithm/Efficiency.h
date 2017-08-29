#ifndef EFFICIENCY_HH
#define EFFICIENCY_HH

#include <iostream>
#include <cmath>
#include <vector>

#include "CaloObject/CaloTrack.h"
#include "CaloObject/Layer.h"
#include "CaloObject/CaloCluster.h"
#include "CaloObject/CaloGeom.h"
#include "Algorithm/Tracking.h"
#include "Algorithm/Distance.h"

namespace algorithm
{

struct EfficiencyParameterSetting
{
		float maxRadius = 25.0f ;
		bool semiDigitalReadout = true ;
		bool printDebug = false ;
		caloobject::GeomParameterSetting geometry ;
		algorithm::TrackingParameterSetting trackingParams ;

		EfficiencyParameterSetting()
			: geometry() ,
			  trackingParams()
		{;}
} ;

class Efficiency
{
	public :
		Efficiency() ;
		~Efficiency() ;

		inline void SetEfficiencyParameterSetting(EfficiencyParameterSetting params) { settings = params ; }
		inline CLHEP::Hep3Vector &getExpectedPosition() { return expectedPos ; }
		void Run(caloobject::Layer* layer, std::vector<caloobject::CaloCluster2D*> &clusters) ;

		inline caloobject::CaloCluster2D* getGoodCluster() const { return goodCluster ; }
		inline bool isTrack() const { return _isTrack ; }
		inline caloobject::CaloTrack* getTrack() const { return track ; }

		Efficiency(const Efficiency&) = delete ;
		void operator=(const Efficiency&) = delete ;

	protected :
		EfficiencyParameterSetting settings ;
		CLHEP::Hep3Vector expectedPos ;

		caloobject::CaloCluster2D* goodCluster = nullptr ;
		bool _isTrack = false ;
		caloobject::CaloTrack* track = nullptr ;


} ;

} //namespace algorithm

#endif
