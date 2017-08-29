#include "CaloObject/CaloCluster.h"

namespace caloobject
{

CaloCluster::CaloCluster(std::vector<caloobject::CaloHit*> &vec, bool useWeight)
	: hits(vec) ,
	  clusterPosition()
{
	for(HitVec::iterator it = hits.begin() ; it != hits.end() ; ++it)
	{
		if( useWeight )
			clusterPosition+=(*it)->getPosition()*(*it)->getEnergy() ;
		else
			clusterPosition+=(*it)->getPosition() ;

		energy+=(*it)->getEnergy() ;

		if ( (*it)->getEnergy() > maxEnergy )
			maxEnergy = (*it)->getEnergy() ;

	}

	if( useWeight )
		clusterPosition /= (hits.size()*energy) ;
	else
		clusterPosition /= hits.size() ;
}

/**********************/

CaloCluster2D::CaloCluster2D(std::vector<caloobject::CaloHit*> &vec, bool useWeight)
	: CaloCluster(vec,useWeight) ,
	  layerID( hits.at(0)->getCellID()[2] )
{

}

/**********************/

CaloCluster3D::CaloCluster3D(std::vector<caloobject::CaloHit*> &vec, bool useWeight)
	: CaloCluster(vec,useWeight) ,
	  barycenter()
{
	std::map<int,int> counter ;
	for( HitVec::iterator it = hits.begin() ; it != hits.end() ; ++it )
	{
		if( barycenter.find( (*it)->getCellID()[2] ) != barycenter.end() )
		{
			barycenter[ (*it)->getCellID()[2] ] += (*it)->getPosition() ;
			counter[ (*it)->getCellID()[2] ]++ ;
		}
		else
		{
			barycenter[ (*it)->getCellID()[2] ] = (*it)->getPosition() ;
			counter[ (*it)->getCellID()[2] ] = 1 ;
		}
	}

	for( std::map<int, CLHEP::Hep3Vector >::iterator it = barycenter.begin() ; it != barycenter.end() ; ++it)
		it->second /= counter[it->first] ;
}

CaloCluster3D::~CaloCluster3D()
{
	barycenter.clear() ;
}

void CaloCluster3D::printInfo()
{
	std::cout << "----------Cluster information----------" << std::endl;
	std::cout << "hits.size() = " << hits.size() << std::endl;
	std::cout << "energy = " << energy << std::endl;
	std::cout << "clusterPosition = " << clusterPosition << std::endl;
	for(HitVec::iterator it=hits.begin(); it!=hits.end(); ++it)
		std::cout << "hits cell id = \t"
				  << (*it)->getCellID()[0] << "\t"
				  << (*it)->getCellID()[1] << "\t"
				  << (*it)->getCellID()[2] << std::endl;
}
}
