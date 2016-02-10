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
      if( settings.printDebug ) std::cout << "_transverseRatio = " << _transverseRatio << std::endl;
      return;
    }
    algorithm::LinearFit3D *fit=new algorithm::LinearFit3D(clusterPos,clusterSize);
    if( fit->getChi2() > settings.chiSquareLimit ){
      if( settings.printDebug ) std::cout << "chiSquare = " << fit->getChi2() << std::endl;
      delete fit;
      return;
    }
    track=new caloobject::CaloTrack(vec);
    track->setChi2(fit->getChi2());
    track->setTrackParameters(fit->getFitParameters());
    delete fit;
  }
}
