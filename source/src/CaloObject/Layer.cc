#include <CaloObject/Layer.h>


namespace caloobject
{

Layer::Layer(int _id)
{
	id = _id ;

	nTracks = 0 ;
	nDetected.push_back(0) ;
	multiSum = 0.0 ;
	multiSquareSum = 0.0 ;

	efficiencies.push_back(0.0) ;
}

Layer::~Layer()
{}

std::vector<double> Layer::getEfficienciesError() const
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

double Layer::getMultiplicity() const
{
	if ( nDetected.at(0) )
		return multiSum/nDetected.at(0) ;
	else
		return 0.0 ;
}

double Layer::getMultiplicityError() const
{
	double var = multiSquareSum/nDetected.at(0) - (multiSum/nDetected.at(0))*(multiSum/nDetected.at(0)) ;
	double error = sqrt(var/nDetected.at(0)) ;
	return error ;
}

void Layer::updateEfficiencies()
{
	for ( unsigned int i = 0 ; i < efficiencies.size() ; ++i )
		efficiencies.at(i) = 1.0*nDetected.at(i)/nTracks ;
}


SDHCALLayer::SDHCALLayer(int _id, int difL, int difC, int difR)
	: Layer(_id)
{
	difID[0] = difL ;
	difID[1] = difC ;
	difID[2] = difR ;

	nDetected.push_back(0) ; //thr2
	nDetected.push_back(0) ; //thr3
	efficiencies.push_back(0.0) ; //thr2
	efficiencies.push_back(0.0) ; //thr3
}

void SDHCALLayer::buildAsics()
{
	for ( int j = 0 ; j < 3 ; j++ )
	{
		for ( int i = 1 ; i < 49 ; i++ )
		{
			SDHCALAsic* asic = new SDHCALAsic(i , difID[j]) ;
			asic->setLayer(this) ;
			CLHEP::Hep3Vector asicPos = position + 10.408*( (j*32 + jAsicTab[i-1]*8)*CLHEP::Hep3Vector(0 , 1 , 0) + iAsicTab[i-1]*8*CLHEP::Hep3Vector(1 , 0 , 0) ) ;
			asic->setPosition(asicPos) ;
			asic->buildPads() ;
			asics.insert( std::make_pair(difID[j]*100+i , asic) ) ;
		}
	}
}


void SDHCALLayer::update(const CLHEP::Hep3Vector& impactPos , CaloCluster2D* cluster)
{
	//	std::cout << "impactPos : " << impactPos << std::endl ;
	//	std::cout << "Layer : " << id << std::endl ;

	nTracks++ ;

	Asic* asic = findAsic(impactPos) ;
	asic->update(impactPos , cluster) ;

	if (cluster)
	{
		asic->update(impactPos , cluster) ;

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

Asic* SDHCALLayer::findAsic(const CLHEP::Hep3Vector& pos) const
{
	//	std::cout << "SDHCALLayer::findAsic" << pos << std::endl ;
	CLHEP::Hep3Vector posInLayer = pos - position ;

	//	std::cout << "posInLayer : " << posInLayer << std::endl ;

	int i = static_cast<int>( posInLayer.x()/(83.264) ) ; //8*10.408 = 83.264
	int j = static_cast<int>( posInLayer.y()/(83.264) ) ;
	//	std::cout << "asici : " << i << std::endl ;
	//	std::cout << "asicj : " << j << std::endl ;

	if ( j/4 > 2 )
	{
		std::cout << "Error in SDHCALLayer::findAsic : dif not possible" << std::endl ;
		std::cout << "pos : " << pos << std::endl ;
		std::cout << "posInLayer : " << posInLayer << std::endl ;
		return NULL ;
	}

	int difj = difID[ j/4 ] ;

	AsicMap::const_iterator it = asics.find( 100*difj + asicTab[i][j%4] ) ;
	if ( it == asics.end() )
	{
		std::cout << "Error in SDHCALLayer::findAsic : non existing asic : "  << std::endl ;
		std::cout << "dif : " << difj << " , asic : " << asicTab[i][difj] << std::endl ;
		return NULL ;
	}

	return it->second ;
}


const int SDHCALLayer::asicTab[12][4] =
{
	{ 4, 3, 2, 1},
	{ 5, 6, 7, 8},
	{12,11,10, 9},
	{13,14,15,16},
	{20,19,18,17},
	{21,22,23,24},
	{28,27,26,25},
	{29,30,31,32},
	{36,35,34,33},
	{37,38,39,40},
	{44,43,42,41},
	{45,46,47,48},
} ;
const int SDHCALLayer::iAsicTab[48] =
{
	0, 0, 0, 0, 1, 1, 1, 1, 2,
	2, 2, 2, 3, 3, 3, 3, 4, 4, 4,
	4, 5, 5, 5, 5, 6, 6, 6, 6, 7,
	7, 7, 7, 8, 8, 8, 8, 9, 9, 9,
	9,10,10,10,10,11,11,11,11
} ;
const int SDHCALLayer::jAsicTab[48] =
{
	3, 2, 1, 0, 0, 1, 2, 3, 3,
	2, 1, 0, 0, 1, 2, 3, 3, 2, 1,
	0, 0, 1, 2, 3, 3, 2, 1, 0, 0,
	1, 2, 3, 3, 2, 1, 0, 0, 1, 2,
	3, 3, 2, 1, 0, 0, 1, 2, 3
} ;

} //namespace caloobject
