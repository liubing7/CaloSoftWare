#include "Algorithm/Cluster.h"

namespace algorithm
{
  void Cluster::Run(std::vector<caloobject::CaloHit*> &hits,std::vector<caloobject::CaloCluster2D*> &outClusterVec)
  {
    std::vector<caloobject::CaloHit*> temp;
    for( std::vector<caloobject::CaloHit*>::iterator it=hits.begin(); it!=hits.end(); ++it){
      if( std::find(temp.begin(),temp.end(),(*it))!=temp.end() ) continue;
      _clusterHitList.push_back(*it);
      temp.push_back(*it);
      BuildCluster(temp,hits,(*it));
      caloobject::CaloCluster2D* calocluster=new caloobject::CaloCluster2D(_clusterHitList);
      outClusterVec.push_back(calocluster);
      _clusterHitList.clear();
    }
    temp.clear();
  }
  
  void Cluster::BuildCluster(std::vector<caloobject::CaloHit*> &temp, std::vector<caloobject::CaloHit*> &hits,caloobject::CaloHit* &hit)
  {
    for(std::vector<caloobject::CaloHit*>::iterator it=hits.begin(); it!=hits.end(); ++it){
      if(std::find(temp.begin(), temp.end(), (*it) )!=temp.end() )continue;
      
      if(!settings.useDistanceInsteadCellID){
	if(
	   std::fabs( (hit)->getCellID()[0]-(*it)->getCellID()[0] )<=settings.maxTransversal &&
	   std::fabs( (hit)->getCellID()[1]-(*it)->getCellID()[1] )<=settings.maxTransversal &&
	   std::fabs( (hit)->getCellID()[2]-(*it)->getCellID()[2] )==settings.maxLongitudinal ){
	  _clusterHitList.push_back(*it);
	  temp.push_back(*it);
	  this->BuildCluster(temp,hits,*it);
	}
      }
      else{
	if(
	   std::fabs( (hit)->getPosition().x()-(*it)->getPosition().x() )<=settings.maxTransversalDistance &&
	   std::fabs( (hit)->getPosition().y()-(*it)->getPosition().y() )<=settings.maxTransversalDistance &&
	   std::fabs( (hit)->getPosition().z()-(*it)->getPosition().z() )==settings.maxLongitudinalDistance ){
	  _clusterHitList.push_back(*it);
	  temp.push_back(*it);
	  this->BuildCluster(temp,hits,*it);
	}
      }
    }
  }
 
}
