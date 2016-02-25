#include "Algorithm/Tracking.h"

namespace algorithm
{
  void Tracking::Run(std::vector<caloobject::CaloCluster*> &vec,
		     caloobject::CaloTrack* &track)
  {
    track=NULL;
    if( vec.size()==0 ) return;
    _transverseRatio=0.0;
    std::vector<double> x;
    std::vector<double> y;
    std::vector<double> z;
    std::vector<CLHEP::Hep3Vector> clusterPos;
    std::vector<int> clusterSize;
    for(std::vector<caloobject::CaloCluster*>::iterator it=vec.begin(); it!=vec.end(); ++it){
      clusterPos.push_back( (*it)->getPosition() );
      clusterSize.push_back( (*it)->getHits().size() );
      x.push_back( (*it)->getPosition().x() );
      y.push_back( (*it)->getPosition().y() );
      z.push_back( (*it)->getPosition().z() );
    }
    std::vector< std::vector<double> > pcaVec;
    pcaVec.push_back(x);
    pcaVec.push_back(y);
    pcaVec.push_back(z);
    algorithm::PCA* pca=new algorithm::PCA(pcaVec);
    std::vector<double> eigenValues=pca->eigenValues();
    _transverseRatio = (eigenValues.at(0)+eigenValues.at(1)) / eigenValues.at(2);
    delete pca;
    if( _transverseRatio > settings.maxTransverseRatio ){
      if( settings.printDebug ) std::cout << "Tracking::Run << DEBUG : _transverseRatio = " << _transverseRatio << std::endl;
      return;
    }
    algorithm::LinearFit3D *fit=new algorithm::LinearFit3D(clusterPos,clusterSize);
    if( fit->getChi2() > settings.chiSquareLimit ){
      if( settings.printDebug ) std::cout << "Tracking::Run << DEBUG : chiSquare = " << fit->getChi2() << std::endl;
      delete fit;
      return;
    }
    if( fit->getFitParameters()[0]!=fit->getFitParameters()[0] || 
	fit->getFitParameters()[1]!=fit->getFitParameters()[1] ||
	fit->getFitParameters()[2]!=fit->getFitParameters()[2] ||
	fit->getFitParameters()[3]!=fit->getFitParameters()[3] ){
      if( settings.printDebug ) 
	std::cout << "Tracking::Run << DEBUG : Problem with fit : fit->getFitParameters() = " 
		  << fit->getFitParameters()[0] << "\t"
		  << fit->getFitParameters()[1] << "\t"
		  << fit->getFitParameters()[2] << "\t"
		  << fit->getFitParameters()[3] << std::endl;
      delete fit;
      return;
    }
    CLHEP::Hep3Vector nx(-1.,0.,fit->getFitParameters()[1]);
    CLHEP::Hep3Vector ny(0.,-1.,fit->getFitParameters()[3]);
    if( (nx.cross(ny)).cosTheta() < settings.cosThetaLimit ){
      if( settings.printDebug ) 
	std::cout << "Tracking::Run << DEBUG : Track angle is too high : cos(theta) = " 
		  << (nx.cross(ny)).cosTheta() 
		  << std::endl;
      delete fit;
      return;    
    }

    track=new caloobject::CaloTrack(vec);
    track->setChi2(fit->getChi2());
    track->setTrackParameters(fit->getFitParameters());
    delete fit;
  }

  void Tracking::TryToAddAClusterInTrack(caloobject::CaloCluster* cluster, caloobject::CaloTrack* &track)
  {
    std::vector<caloobject::CaloCluster*> clusters=track->getClusters();
    if( clusters.size()==0 ) return;
    clusters.push_back(cluster);
    _transverseRatio=0.0;
    std::vector<double> x;
    std::vector<double> y;
    std::vector<double> z;
    std::vector<CLHEP::Hep3Vector> clusterPos;
    std::vector<int> clusterSize;
    for(std::vector<caloobject::CaloCluster*>::iterator it=clusters.begin(); it!=clusters.end(); ++it){
      clusterPos.push_back( (*it)->getPosition() );
      clusterSize.push_back( (*it)->getHits().size() );
      x.push_back( (*it)->getPosition().x() );
      y.push_back( (*it)->getPosition().y() );
      z.push_back( (*it)->getPosition().z() );
    }
    std::vector< std::vector<double> > pcaVec;
    pcaVec.push_back(x);
    pcaVec.push_back(y);
    pcaVec.push_back(z);
    algorithm::PCA* pca=new algorithm::PCA(pcaVec);
    std::vector<double> eigenValues=pca->eigenValues();
    _transverseRatio = (eigenValues.at(0)+eigenValues.at(1)) / eigenValues.at(2);
    delete pca;
    if( _transverseRatio > settings.maxTransverseRatio ){
      if( settings.printDebug ) std::cout << "Tracking::Run << DEBUG : _transverseRatio = " << _transverseRatio << std::endl;
      return;
    }
    algorithm::LinearFit3D *fit=new algorithm::LinearFit3D(clusterPos,clusterSize);
    if( fit->getChi2() > settings.chiSquareLimit ){
      if( settings.printDebug ) std::cout << "Tracking::Run << DEBUG : chiSquare = " << fit->getChi2() << std::endl;
      delete fit;
      return;
    }
    if( fit->getFitParameters()[0]!=fit->getFitParameters()[0] || 
	fit->getFitParameters()[1]!=fit->getFitParameters()[1] ||
	fit->getFitParameters()[2]!=fit->getFitParameters()[2] ||
	fit->getFitParameters()[3]!=fit->getFitParameters()[3] ){
      if( settings.printDebug ) 
	std::cout << "Tracking::Run << DEBUG : Problem with fit : fit->getFitParameters() = " 
		  << fit->getFitParameters()[0] << "\t"
		  << fit->getFitParameters()[1] << "\t"
		  << fit->getFitParameters()[2] << "\t"
		  << fit->getFitParameters()[3] << std::endl;
      delete fit;
      return;
    }
    CLHEP::Hep3Vector nx(-1.,0.,fit->getFitParameters()[1]);
    CLHEP::Hep3Vector ny(0.,-1.,fit->getFitParameters()[3]);
    if( (nx.cross(ny)).cosTheta() < settings.cosThetaLimit ){
      if( settings.printDebug ) 
	std::cout << "Tracking::Run << DEBUG : Track angle is too high : cos(theta) = " 
		  << (nx.cross(ny)).cosTheta() 
		  << std::endl;
      delete fit;
      return;    
    }
    track->addCluster(cluster);
    track->setChi2(fit->getChi2());
    track->setTrackParameters(fit->getFitParameters());
    delete fit;
  }
}
