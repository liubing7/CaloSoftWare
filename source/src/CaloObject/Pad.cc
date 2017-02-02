#include "CaloObject/Pad.h"

namespace caloobject
{

Pad::Pad(int _id)
{
	id = _id ;

	nTracks = 0 ;
	nDetected.push_back(0) ;
	multiSum = 0.0 ;
	multiSquareSum = 0.0 ;

	efficiencies.push_back(0.0) ;
}

Pad::~Pad()
{}

void Pad::reset()
{
	nTracks = 0 ;
	nDetected.clear() ;
	multiSum = 0.0 ;
	multiSquareSum = 0.0 ;

	thresholds.clear() ;
	efficiencies.clear() ;
}

void Pad::setThresholds(const std::vector<double>& thr)
{
	reset() ;

	thresholds = thr ;

	nDetected = std::vector<int>(thresholds.size() , 0) ;
	efficiencies = std::vector<double>(thresholds.size() , 0.0) ;
}

std::vector<double> Pad::getEfficienciesError() const
{
	std::vector<double> toReturn ;

	for ( unsigned int i = 0 ; i < nDetected.size() ; ++i )
	{
		double eff , effErr ;

		if ( nDetected.at(i) == nTracks )
			eff = (nTracks-1.0)/nTracks ;
		else if ( nDetected.at(i) == 0 )
			eff = 1.0/nTracks ;
		else
			eff = static_cast<double>( nDetected.at(i) )/nTracks ;


		effErr = sqrt( eff*(1-eff)/nTracks ) ;
		toReturn.push_back(effErr) ;
	}
	return toReturn ;
}

double Pad::getMultiplicity() const
{
	if ( nDetected.at(0) )
		return multiSum/nDetected.at(0) ;
	else
		return 0.0 ;
}

double Pad::getMultiplicityError() const
{
	if ( !nDetected.at(0) )
		return 0.0 ;

	double var = multiSquareSum/nDetected.at(0) - (multiSum/nDetected.at(0))*(multiSum/nDetected.at(0)) ;

	if ( var < std::numeric_limits<double>::epsilon() )
		var = 1.0/( std::sqrt(12*nDetected.at(0)) ) ;

	double error ;
	if ( nDetected.at(0) < 2 )
		error = multiSum/nDetected.at(0) ;
	else
		error = sqrt( var/(nDetected.at(0)-1.0) ) ;
	return error ;
}


void Pad::updateEfficiencies()
{
	for ( unsigned int i = 0 ; i < efficiencies.size() ; ++i )
		efficiencies.at(i) = 1.0*nDetected.at(i)/nTracks ;
}

//===============
//======SDHCALPad
//===============

SDHCALPad::SDHCALPad(int _id)
	: Pad(_id)
{
}

SDHCALPad::~SDHCALPad()
{
}

void Pad::update(CaloCluster2D* cluster)
{
	nTracks++ ;

	if (cluster)
	{
		for ( unsigned int i = 0 ; i < thresholds.size() ; ++i )
		{
			if ( cluster->getMaxEnergy() >= thresholds.at(i) )
				nDetected.at(i)++ ;
		}

		multiSum += cluster->getHits().size() ;
		multiSquareSum += cluster->getHits().size()*cluster->getHits().size() ;
	}

	updateEfficiencies() ;
}



} //namespace caloobject
