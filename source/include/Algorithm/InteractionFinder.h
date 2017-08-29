#ifndef INTERACTIONFINDER_HH
#define INTERACTIONFINDER_HH

#include <limits>
#include "CaloObject/CaloCluster.h"
#include "Algorithm/Distance.h"
#include <CLHEP/Vector/ThreeVector.h>

namespace algorithm
{ 

struct InteractionFinderParameterSetting
{
		int minSize = 4 ;
		float maxRadius = 50.0f ;
		int maxDepth = 4 ;
		int minNumberOfCluster = 3 ;
		bool useAnalogEnergy = false ;
		float minEnergy = 0.001f ;
		bool printDebug = false ;

		// minSize : minimum cluster size to have an interaction
		// maxRadius : maximum transversal distance from event thrust to find interaction point
		// maxDepth : maximum depth (in layer unit) to search clusters to find an interaction
		// minNumberOfCluster : minimum number of clusters to find after a candidate interaction to define a real interaction
		// useAnalogEnergy : set true to use cluster energy rather than topology (siwcal),
		// minEnergy : minimum cluster energy to define interaction, default value corresponds to 1 MeV
} ;

class InteractionFinder
{
	public :
		InteractionFinder()
			: settings()
		{;}
		~InteractionFinder(){;}
		void Run(std::vector<caloobject::CaloCluster2D*> &clusters, const std::vector<float> params) ;
		void SetInteractionFinderParameterSetting(InteractionFinderParameterSetting params){ settings = params ; }
		bool FindInteraction(){ return findInteraction ; }
		caloobject::CaloCluster2D* getFirstInteractionCluster(){ return firstCluster ; }

		InteractionFinder(const InteractionFinder&) = delete ;
		void operator=(const InteractionFinder&) = delete ;

	private :
		InteractionFinderParameterSetting settings ;
		bool findInteraction = false ;
		caloobject::CaloCluster2D* firstCluster = nullptr ;
} ;

} //namespace algorithm

#endif
