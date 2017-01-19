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
	//	if ( nDetected.at(0) )
	//	{
	//		double rmsLike = multiSum*multiSum*(1 - 1.0/nDetected.at(0) )/nDetected.at(0) ;
	//		return std::sqrt( rmsLike/nDetected.at(0) ) ;
	//	}
	//	else
	//		return 0.0 ;


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
	nDetected.push_back(0) ; //thr2
	nDetected.push_back(0) ; //thr3
	efficiencies.push_back(0.0) ; //thr2
	efficiencies.push_back(0.0) ; //thr3
}

SDHCALPad::~SDHCALPad()
{
}

void SDHCALPad::update(CaloCluster2D* cluster)
{
	// std::cout << "Pad : " << id << " , pos : " << position << std::endl ;

	nTracks++ ;

	if (cluster)
	{
		nDetected.at(0)++ ;

		int maxThr = 0 ;
		HitVec hits = cluster->getHits() ;
		for ( HitVec::const_iterator it = hits.begin() ; it != hits.end() ; ++it )
		{
			if ( (*it)->getEnergy() > maxThr )
				maxThr = static_cast<int>( (*it)->getEnergy() ) ;
		}

		if ( maxThr >=2 )
			nDetected.at(1)++ ;
		if ( maxThr >= 3 )
			nDetected.at(2)++ ;

		multiSum += cluster->getHits().size() ;
		multiSquareSum += cluster->getHits().size()*cluster->getHits().size() ;
	}

	updateEfficiencies() ;
}





} //namespace caloobject
