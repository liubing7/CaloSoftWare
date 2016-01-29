#include "Algorithm/Efficiency.h"

namespace algorithm
{
  void Efficiency::Run(caloobject::CaloLayer *layer, std::vector<caloobject::CaloCluster*> &clusters)
  {
    std::vector<caloobject::CaloCluster*> clusterVec;
    std::vector<caloobject::CaloCluster*> clustersInLayer;
    for(std::vector<caloobject::CaloCluster*>::iterator it=clusters.begin(); it!=clusters.end(); ++it){
      if( (*it)->getLayerID()==layer->getLayerID() ) 
	clustersInLayer.push_back(*it);
      else clusterVec.push_back(*it);
    }
    
    algorithm::Tracking* algo_tracking=new algorithm::Tracking();
    algo_tracking->SetTrackingParameterSetting(settings.trackingParams);
    caloobject::CaloTrack* track=NULL;
    algo_tracking->Run(clusterVec,track);
    delete algo_tracking;
    if(track!=NULL){
      double xExpected=track->getTrackParameters()[1]+track->getTrackParameters()[0];
      double yExpected=track->getTrackParameters()[3]+track->getTrackParameters()[2];
      if(xExpected>settings.edgeX_max ||
	 xExpected<settings.edgeX_min ||
	 yExpected>settings.edgeY_max ||
	 yExpected<settings.edgeY_min){
	delete algo_tracking;
	delete track;
	if( settings.printDebug )
	  std::cout << "expected track impact outside layer " << layer->getLayerID() << ":\t" 
		    << "xExpected = " << xExpected << "\t"
		    << "yExpected = " << yExpected << std::endl;
	return;
      }
      if(clustersInLayer.empty()){
	if( settings.printDebug ) std::cout << "find one empty layer = " << layer->getLayerID() << std::endl;
	layer->update();
	delete algo_tracking;
	delete track;
	return;
      }
      Distance<caloobject::CaloCluster,caloobject::CaloTrack> dist;
      std::vector<caloobject::CaloCluster*>::iterator closestIt=clustersInLayer.begin();
      float old_dist=dist.getDistance( (*closestIt),track );
      float new_dist=0.0;
      for(std::vector<caloobject::CaloCluster*>::iterator it=clustersInLayer.begin()+1; it!=clustersInLayer.end(); ++it){
	if( (*it)->getLayerID() != layer->getLayerID() ) { 
	  std::cout << "Efficiency algorithm problem in Run(caloobject::CaloLayer *layer, std::vector<caloobject::CaloCluster*> &clusters)" << std::endl;
	  throw;
	}
	new_dist=dist.getDistance((*it),track);
	if( new_dist<old_dist ){
	  closestIt=it;
	  old_dist=new_dist;
	}
      }
      Distance<caloobject::CaloHit,caloobject::CaloTrack> distHit;
      bool efficientLayer=false;
      float energy=0.;
      for(std::vector<caloobject::CaloHit*>::iterator it=(*closestIt)->getHits().begin(); it!=(*closestIt)->getHits().end(); ++it){
	if( distHit.getDistance( (*it),track ) < settings.maxRadius ) efficientLayer=true;
      
	if( efficientLayer==true ){
	  if( settings.semiDigitalReadout == true ){
	    if( energy<(*it)->getEnergy() ) energy=(*it)->getEnergy();
	  }
	  else
	    energy+=(*it)->getEnergy();
	}
      }
      if( efficientLayer==true ){
	layer->update((*closestIt)->getHits().size(),energy);
      }
      else 
	layer->update();

      delete track;
    }
  }

}
