#include "Algorithm/Cluster3D.h"
#include "Algorithm/Distance.h"

namespace algorithm
{
void Cluster3D::Run(std::vector<caloobject::CaloHit*> &hits,
					std::vector<caloobject::CaloCluster3D*> &outClusterVec)
{
	// seedless option
}

void Cluster3D::Run(std::vector<caloobject::CaloHit*> &hits,
					std::vector<caloobject::CaloCluster3D*> &outClusterVec,
					CLHEP::Hep3Vector seed,
					CLHEP::Hep3Vector direction)
{
	direction/=direction.mag();
	std::vector<caloobject::CaloHit*> temp;
	Distance<caloobject::CaloHit,CLHEP::Hep3Vector> dist;
	for( std::vector<caloobject::CaloHit*>::iterator it=hits.begin(); it!=hits.end(); ++it){
		if( std::find(temp.begin(),temp.end(),(*it))!=temp.end() ) continue;

		float coeff= ( (*it)->getPosition()-seed ).z()/direction.z();
		CLHEP::Hep3Vector seedProj=seed+coeff*direction;

		if( dist.getDistance( (*it),seedProj ) > settings.maxTransverseDistance )
			continue;
		_clusterHitList.push_back(*it);
		temp.push_back(*it);
		BuildCluster(temp,hits,(*it),seed,direction);
		caloobject::CaloCluster3D* calocluster=new caloobject::CaloCluster3D(_clusterHitList, settings.useEnergyToWeightPosition );
		outClusterVec.push_back(calocluster);
		//calocluster->printInfo();
		_clusterHitList.clear();
	}
	temp.clear();
}

void Cluster3D::BuildCluster(std::vector<caloobject::CaloHit*> &temp,


							 std::vector<caloobject::CaloHit*> &hits,
							 caloobject::CaloHit* &hit,
							 CLHEP::Hep3Vector seed,
							 CLHEP::Hep3Vector direction)
{
	Distance<caloobject::CaloHit,CLHEP::Hep3Vector> dist;
	for(std::vector<caloobject::CaloHit*>::iterator it=hits.begin(); it!=hits.end(); ++it){
		if(std::find(temp.begin(), temp.end(), (*it) )!=temp.end() )continue;
		if( std::fabs( hit->getCellID()[2]-(*it)->getCellID()[2] ) > settings.maxLongitudinal )
			continue;
		float coeff= ( (*it)->getPosition()-seed ).z()/direction.z();
		CLHEP::Hep3Vector seedProj=seed+coeff*direction;
		if( dist.getDistance( (*it),seedProj ) > settings.maxTransverseDistance )
			continue;
		_clusterHitList.push_back(*it);
		temp.push_back(*it);
		this->BuildCluster(temp,hits,*it,seed,direction);
	}
}

}
