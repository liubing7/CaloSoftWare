#include "CaloObject/Asic.h"

namespace caloobject
{

Asic::Asic(int _id , int _difID)
{
	id = _id ;
	difID = _difID ;

	nTracks = 0 ;
	nDetected.push_back(0) ;
	multiSum = 0.0 ;
	multiSquareSum = 0.0 ;

	efficiencies.push_back(0.0) ;
}

Asic::~Asic()
{}

void Asic::reset()
{
	nTracks = 0 ;
	nDetected.clear() ;
	multiSum = 0.0 ;
	multiSquareSum = 0.0 ;

	thresholds.clear() ;
	efficiencies.clear() ;

	for ( PadMap::const_iterator it = pads.begin() ; it != pads.end() ; ++it )
		it->second->reset() ;

}

void Asic::setThresholds(const std::vector<double>& thr)
{
	reset() ;

	thresholds = thr ;

	nDetected = std::vector<int>(thresholds.size() , 0) ;
	efficiencies = std::vector<double>(thresholds.size() , 0.0) ;

	for ( PadMap::const_iterator it = pads.begin() ; it != pads.end() ; ++it )
		it->second->setThresholds(thresholds) ;
}

std::vector<double> Asic::getEfficienciesError() const
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

double Asic::getMultiplicity() const
{
	if ( nDetected.at(0) )
		return multiSum/nDetected.at(0) ;
	else
		return 0.0 ;
}

double Asic::getMultiplicityError() const
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

void Asic::update(const CLHEP::Hep3Vector& impactPos , CaloCluster2D* cluster)
{
	nTracks++ ;

	Pad* pad = findPad(impactPos) ;
	pad->update(cluster) ;

	if (cluster)
	{
		for ( unsigned int i = 0 ; i < thresholds.size() ; ++i )
		{
			if ( cluster->getMaxEnergy() > thresholds.at(i) )
				nDetected.at(i)++ ;
		}

		if ( cluster->getMaxEnergy() > thresholds.at(0) )
		{
			multiSum += cluster->getHits().size() ;
			multiSquareSum += cluster->getHits().size()*cluster->getHits().size() ;
		}
	}

	updateEfficiencies() ;
}

void Asic::updateEfficiencies()
{
	for ( unsigned int i = 0 ; i < efficiencies.size() ; ++i )
		efficiencies.at(i) = 1.0*nDetected.at(i)/nTracks ;
}

SDHCALAsic::SDHCALAsic(int _id, int _difID)
	: Asic(_id , _difID)
{
}

void SDHCALAsic::buildPads()
{
	for ( int i = 0 ; i < 64 ; i++ )
	{
		SDHCALPad* pad = new SDHCALPad(i) ;
		pad->setAsic(this) ;
		CLHEP::Hep3Vector padPos = position + 10.408*CLHEP::Hep3Vector(iPadTab[i] , jPadTab[i] , 0) ;
		pad->setPosition(padPos) ;
		pads.insert( std::make_pair(i , pad) ) ;
	}
}


Pad* SDHCALAsic::findPad(const CLHEP::Hep3Vector& pos) const
{
	CLHEP::Hep3Vector posInAsic = pos - position ;
	int i = static_cast<int>( posInAsic.x()/10.408 ) ;
	int j = static_cast<int>( posInAsic.y()/10.408 ) ;

	PadMap::const_iterator it = pads.find( padTab[i][j] ) ;
	if ( it == pads.end() )
	{
		std::cout << "Error in SDHCALAsic::findPad : non existing pad" << std::endl ;
		std::cout << "position.x() : " << position.x() << " , position.y() : " << position.y() << std::endl ;
		std::cout << "pos.x() : " << pos.x() << " , pos.y() : " << pos.y() << std::endl ;
		std::cout << "posInAsic.x() : " << posInAsic.x() << " , posInAsic.y() : " << posInAsic.y() << std::endl ;
		std::cout << "i : " << i << " , j : " << j <<" , pad : " << padTab[i][j] << std::endl ;
		return NULL ;
	}

	return it->second ;
}

const int SDHCALAsic::padTab[8][8] =
{
	{12,10, 8, 7, 5, 3, 1,17} ,   // \/
	{13,11, 9, 6, 4, 2, 0,18} ,   // ||
	{23,22,21,14,15,16,19,20} ,   // ||
	{24,25,26,27,28,29,30,31} ,   //  I
	{39,38,37,36,35,34,33,32} ,   // ||
	{40,41,50,49,48,47,44,42} ,   // ||
	{51,53,55,57,60,62,45,43} ,   // \/
	{52,54,56,58,59,61,63,46} ,
} ;
const int SDHCALAsic::iPadTab[64] =
{
	1, 0, 1, 0, 1, 0, 1, 0, 0, 1,
	0, 1, 0, 1, 2, 2, 2, 0, 1, 2,
	2, 2, 2, 2, 3, 3, 3, 3, 3, 3,
	3, 3, 4, 4, 4, 4, 4, 4, 4, 4,
	5, 5, 5, 6, 5, 6, 7, 5, 5, 5,
	5, 6, 7, 6, 7, 6, 7, 6, 7, 7,
	6, 7, 6, 7
} ;
const int SDHCALAsic::jPadTab[64] =
{
	6, 6, 5, 5, 4, 4, 3, 3, 2, 2,
	1, 1, 0, 0, 3, 4, 5, 7, 7, 6,
	7, 2, 1, 0, 0, 1, 2, 3, 4, 5,
	6, 7, 7, 6, 5, 4, 3, 2, 1, 0,
	0, 1, 7, 7, 6, 6, 7, 5, 4, 3,
	2, 0, 0, 1, 1, 2, 2, 3, 3, 4,
	4, 5, 5, 6
} ;

} //namespace caloobject
