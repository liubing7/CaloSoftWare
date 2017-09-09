#include "CaloObject/Shower.h"

namespace caloobject
{

Shower::Shower(std::vector<caloobject::CaloHit*>& vec)
	: hits(vec) , clusters() ,
	  edepPerCell() ,
	  hitTimes() ,
	  thrust() ,
	  longitudinal() ,
	  transverse() ,
	  distanceToAxis() ,
	  clustersEnergy()
{

}

Shower::Shower(std::vector<caloobject::CaloCluster2D*>& vec)
	: hits() , clusters(vec) ,
	  edepPerCell() ,
	  hitTimes() ,
	  thrust() ,
	  longitudinal() ,
	  transverse() ,
	  distanceToAxis() ,
	  clustersEnergy()
{
	for( std::vector<caloobject::CaloCluster2D*>::iterator it = vec.begin() ; it != vec.end() ; ++it )
	{
		clustersEnergy.push_back( (*it)->getEnergy() ) ;
		for( HitVec::const_iterator jt = (*it)->getHits().begin() ; jt != (*it)->getHits().end() ; ++jt )
			hits.push_back(*jt) ;
	}

}

Shower::Shower( caloobject::CaloCluster3D* cluster )
	: hits( cluster->getHits() ) , clusters() ,
	  edepPerCell() ,
	  hitTimes() ,
	  thrust() ,
	  longitudinal() ,
	  transverse() ,
	  distanceToAxis() ,
	  clustersEnergy()
{

}

SDHCALShower::SDHCALShower(std::vector<caloobject::CaloCluster2D*>& vec)
	: Shower(vec)
{
}

} //namespace caloobject
