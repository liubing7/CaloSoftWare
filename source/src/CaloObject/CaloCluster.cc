#include "CaloObject/CaloCluster.h"

namespace caloobject
{

  CaloCluster::CaloCluster(std::vector<caloobject::CaloHit*> &vec) : hits(vec), energy(0.0)  
  {
    for(std::vector<caloobject::CaloHit*>::iterator it=hits.begin(); it!=hits.end(); ++it){
      clusterPosition+=(*it)->getPosition();
      energy+=(*it)->getEnergy();
    }
    clusterPosition/=hits.size();
  }
  
  /**********************/

  CaloCluster2D::CaloCluster2D(std::vector<caloobject::CaloHit*> &vec) : CaloCluster(vec)
  {
    layerID=hits.at(0)->getCellID()[2];
  }

  /**********************/

  CaloCluster3D::CaloCluster3D(std::vector<caloobject::CaloHit*> &vec) : CaloCluster(vec)
  {
    std::map<int,int> counter;
    for( std::vector<caloobject::CaloHit*>::iterator it=hits.begin(); it!=hits.end(); ++it ){
      if( barycenter.find( (*it)->getCellID()[2] )!=barycenter.end() ){
	barycenter[ (*it)->getCellID()[2] ] += (*it)->getPosition();
	counter[ (*it)->getCellID()[2] ] ++;
      }
      else{
	barycenter[ (*it)->getCellID()[2] ] = (*it)->getPosition();
	counter[ (*it)->getCellID()[2] ] = 1;
      }
    }
    for( std::map<int, CLHEP::Hep3Vector >::iterator it=barycenter.begin(); it!=barycenter.end(); ++it)
      it->second/=counter[it->first];
  }

  CaloCluster3D::~CaloCluster3D()
  {
    barycenter.clear();
  }

  void CaloCluster3D::printInfo()
  {
    std::cout << "----------Cluster information----------" << std::endl;
    std::cout << "hits.size() = " << hits.size() << std::endl;
    std::cout << "energy = " << energy << std::endl;
    std::cout << "clusterPosition = " << clusterPosition << std::endl;
    for(std::vector<caloobject::CaloHit*>::iterator it=hits.begin(); it!=hits.end(); ++it)
      std::cout << "hits cell id = \t"
		<< (*it)->getCellID()[0] << "\t"
		<< (*it)->getCellID()[1] << "\t"
		<< (*it)->getCellID()[2] << std::endl;
  }
}
