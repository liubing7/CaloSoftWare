#include "Algorithm/Cluster.h"

namespace algorithm
{
  void Cluster::Run(std::vector<caloobject::CaloHit*> &hits,std::vector<caloobject::CaloCluster*> &outClusterVec)
  {
    std::vector<caloobject::CaloHit*> temp;
    for( std::vector<caloobject::CaloHit*>::iterator it=hits.begin(); it!=hits.end(); ++it){
      if( std::find(temp.begin(),temp.end(),(*it))!=temp.end() ) continue;
      _clusterHitList.push_back(*it);
      temp.push_back(*it);
      BuildCluster(temp,hits,(*it));
      caloobject::CaloCluster* calocluster=new caloobject::CaloCluster(_clusterHitList);
      outClusterVec.push_back(calocluster);
      _clusterHitList.clear();
    }
    temp.clear();
  }
  
  void Cluster::BuildCluster(std::vector<caloobject::CaloHit*> &temp, std::vector<caloobject::CaloHit*> &hits,caloobject::CaloHit* &hit)
  {
    for(std::vector<caloobject::CaloHit*>::iterator it=hits.begin(); it!=hits.end(); ++it){
      if(std::find(temp.begin(), temp.end(), (*it) )!=temp.end() )continue;
      
      if(!_settings.useDistanceInsteadCellID){
	if(
	   std::fabs( (hit)->getCellID()[0]-(*it)->getCellID()[0] )<=_settings.maxTransversal &&
	   std::fabs( (hit)->getCellID()[1]-(*it)->getCellID()[1] )<=_settings.maxTransversal &&
	   std::fabs( (hit)->getCellID()[2]-(*it)->getCellID()[2] )==_settings.maxLongitudinal ){
	  _clusterHitList.push_back(*it);
	  temp.push_back(*it);
	  this->BuildCluster(temp,hits,*it);
	}
      }
      else{
	if(
	   std::fabs( (hit)->getPosition().x()-(*it)->getPosition().x() )<=_settings.maxTransversalDistance &&
	   std::fabs( (hit)->getPosition().y()-(*it)->getPosition().y() )<=_settings.maxTransversalDistance &&
	   std::fabs( (hit)->getPosition().z()-(*it)->getPosition().z() )==_settings.maxLongitudinalDistance ){
	  _clusterHitList.push_back(*it);
	  temp.push_back(*it);
	  this->BuildCluster(temp,hits,*it);
	}
      }
    }
  }
}
