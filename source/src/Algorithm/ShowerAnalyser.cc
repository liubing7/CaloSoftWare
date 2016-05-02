#include "Algorithm/ShowerAnalyser.h"
#include "Algorithm/LinearFit3D.h"
#include "Algorithm/ManipMap.h"
#include "Algorithm/PCA.h"
#include "Algorithm/Distance.h"
#include <set>
#include <map>


namespace algorithm
{
  void ShowerAnalyser::Run(caloobject::Shower* shower)
  {
    std::set<int> layers;
    std::vector<CLHEP::Hep3Vector> hitPos;
    std::vector<int> clSize; //=1 because it use hits and not clusters
    std::map<int,float> edep_layerMap;
    std::vector<double> x,y,z;

    shower->edep=0.0f;
    shower->f1=0.0f;
    shower->f2=0.0f;
    shower->f3=0.0f;
    shower->f4=0.0f;
    shower->rmsEdep=0.0f;
    for(std::vector<caloobject::CaloHit*>::const_iterator it=shower->getHits().begin(); it!=shower->getHits().end(); ++it){
      shower->edep+=(*it)->getEnergy();
      shower->edepPerCell.push_back( (*it)->getEnergy() );
      shower->rmsEdep+=(*it)->getEnergy()*(*it)->getEnergy();
      layers.insert( (*it)->getCellID()[2] );
      hitPos.push_back( (*it)->getPosition() );
      clSize.push_back( 1 );

      if( edep_layerMap[ (*it)->getCellID()[2] ] > 0 )
	edep_layerMap[ (*it)->getCellID()[2] ] += (*it)->getEnergy() ;
      else
	edep_layerMap[ (*it)->getCellID()[2] ] = (*it)->getEnergy() ;
      
      if( (*it)->getCellID()[2]<settings.geometry.firstSectionLastLayer )
	shower->f1+=(*it)->getEnergy();
      if( (*it)->getCellID()[2]<settings.geometry.secondSectionLastLayer )
	shower->f2+=(*it)->getEnergy();
      if( (*it)->getCellID()[2]<settings.geometry.thirdSectionLastLayer )
	shower->f3+=(*it)->getEnergy();
      if( (*it)->getCellID()[2]<settings.geometry.fourthSectionLastLayer )
	shower->f4+=(*it)->getEnergy();

      x.push_back( (*it)->getPosition().x() );
      y.push_back( (*it)->getPosition().y() );
      z.push_back( (*it)->getPosition().z() );
    
    }
    shower->meanEdep=shower->edep/shower->getHits().size();
    shower->rmsEdep=std::sqrt( shower->rmsEdep/shower->getHits().size() - shower->meanEdep*shower->meanEdep );

    FindEnergy(shower);
    
    shower->f1/=shower->edep;
    shower->f2/=shower->edep;
    shower->f3/=shower->edep;
    shower->f4/=shower->edep;
    
    shower->showerMax = algorithm::map_max_element(edep_layerMap)->first; //x0 unit
    shower->edepAtMax = algorithm::map_max_element(edep_layerMap)->second;

    shower->nlayer=layers.size();
    
    algorithm::LinearFit3D* algo_LinearFit3D=new algorithm::LinearFit3D(hitPos,clSize);
    shower->thrust.clear();
    for(int i=0; i<4; i++)
      shower->thrust.push_back(algo_LinearFit3D->getFitParameters()[i]);

    CLHEP::Hep3Vector orientation = CLHEP::Hep3Vector(-1,0,shower->thrust[1]).cross(CLHEP::Hep3Vector(0,-1,shower->thrust[3]));
    shower->reconstructedCosTheta = orientation.cosTheta() ;
    shower->eta = orientation.eta();
    shower->phi = orientation.phi();

    delete algo_LinearFit3D;

    VectorList vecList;
    vecList.push_back(x);
    vecList.push_back(y);
    vecList.push_back(z);
    algorithm::PCA* algo_PCA=new algorithm::PCA(vecList);
    shower->transverseRatio=std::sqrt( algo_PCA->eigenValues()[0]*algo_PCA->eigenValues()[0] +
				       algo_PCA->eigenValues()[1]*algo_PCA->eigenValues()[1] )/algo_PCA->eigenValues()[2];
    delete algo_PCA;

    SearchShowerInteraction(shower);
    Profile(shower);
  }

  void ShowerAnalyser::FindEnergy(caloobject::Shower* shower)
  {
    if( settings.energyCalibrationOption==std::string("SiWEcal") )
      shower->energy=shower->edep*settings.energyCalibrationFactors.at(0);
  }

  void ShowerAnalyser::SearchShowerInteraction(caloobject::Shower* shower)
  {
    shower->findInteraction_=false;
    algorithm::InteractionFinder* algo = new algorithm::InteractionFinder();
    algo->SetInteractionFinderParameterSetting(settings.interactionFinderParams);
    algo->Run(shower->clusters,shower->getThrust());
    if( algo->FindInteraction() ){
      shower->findInteraction_=true;
      shower->firstIntCluster=algo->getFirstInteractionCluster();
      shower->startingPosition=algo->getFirstInteractionCluster()->getPosition();
    }
  }

  void ShowerAnalyser::Profile(caloobject::Shower* shower)
  {
    shower->longitudinal.clear();
    shower->transverse.clear();
    shower->distanceToAxis.clear();
    for(int i=0; i<settings.geometry.nLayers; i++)
      shower->longitudinal.push_back(0);
    for(int i=0; i<(int)(settings.maximumRadius/settings.geometry.pixelSize); i++)
      shower->transverse.push_back(0);
    
    int begin=0;
    if( shower->firstIntCluster!=NULL )
      begin=shower->firstIntCluster->getLayerID();


    Distance<caloobject::CaloHit,CLHEP::Hep3Vector> dist;
    for(std::vector<caloobject::CaloHit*>::const_iterator it=shower->getHits().begin(); it!=shower->getHits().end(); ++it){
      //if( (*it)->getCellID()[2]>=begin )
      shower->longitudinal.at( (*it)->getCellID()[2] ) +=(*it)->getEnergy();

      CLHEP::Hep3Vector vec(shower->thrust[0]+shower->thrust[1]*(*it)->getPosition().z(),
			    shower->thrust[2]+shower->thrust[3]*(*it)->getPosition().z(),
			    (*it)->getPosition().z());
      shower->distanceToAxis.push_back( dist.getDistance( (*it),vec ) );
      int ring=(int)(dist.getDistance( (*it),vec )/settings.geometry.pixelSize );
      if( ring>(int)(settings.maximumRadius/settings.geometry.pixelSize) )
	continue;
      else
	shower->transverse.at(ring)+=(*it)->getEnergy();
    }
  }
}
