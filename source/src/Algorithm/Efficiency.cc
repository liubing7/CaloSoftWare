#include "Algorithm/Efficiency.h"

namespace algorithm
{
  void Efficiency::Run(caloobject::CaloLayer *layer, std::vector<caloobject::CaloCluster*> &clusters)
  {
    //std::cout << layer->getLayerID() << " " << layer->getLayerZPosition() << std::endl;
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
      //missing layerZPosition
      float xExpected=track->getTrackParameters()[1]*layer->getLayerZPosition()+track->getTrackParameters()[0];
      float yExpected=track->getTrackParameters()[3]*layer->getLayerZPosition()+track->getTrackParameters()[2];
      if(xExpected>layer->getSettings().edgeX_max ||
    	 xExpected<layer->getSettings().edgeX_min ||
    	 yExpected>layer->getSettings().edgeY_max ||
    	 yExpected<layer->getSettings().edgeY_min){
	delete track;
    	if( settings.printDebug )
    	  std::cout << "expected track impact outside layer " << layer->getLayerID() << ":\t" 
    		    << "xExpected = " << xExpected << "\t"
    		    << "yExpected = " << yExpected << std::endl;
    	return;
      }
      asicKey=findAsicKey( CLHEP::Hep3Vector(xExpected,yExpected,layer->getLayerZPosition()), 
			   layer->getSettings().padSize,
			   layer->getSettings().nPadX,
			   layer->getSettings().nPadY);
      if(clustersInLayer.empty()){
    	if( settings.printDebug ) std::cout << "find one empty layer = " << layer->getLayerID() << std::endl;
    	layer->update();
	delete track;
    	return;
      }
      Distance<caloobject::CaloCluster,caloobject::CaloTrack> dist;
      std::vector<caloobject::CaloCluster*>::iterator closestIt=clustersInLayer.begin();
      float old_dist=dist.getDistanceInLayer( (*closestIt),track );
      float new_dist=0.0;
      for(std::vector<caloobject::CaloCluster*>::iterator it=clustersInLayer.begin()+1; it!=clustersInLayer.end(); ++it){
    	if( (*it)->getLayerID() != layer->getLayerID() ) { 
    	  std::cout << "Efficiency algorithm problem in Run(caloobject::CaloLayer *layer, std::vector<caloobject::CaloCluster*> &clusters)" << std::endl;
    	  throw;
    	}
    	new_dist=dist.getDistanceInLayer((*it),track);
    	if( new_dist<old_dist ){
    	  closestIt=it;
    	  old_dist=new_dist;
    	}
      }
      Distance<caloobject::CaloHit,caloobject::CaloTrack> distHit;
      bool efficientLayer=false;
      float energy=0.;
      for(std::vector<caloobject::CaloHit*>::iterator it=(*closestIt)->getHits().begin(); it!=(*closestIt)->getHits().end(); ++it){
    	if( distHit.getDistanceInLayer( (*it),track ) < settings.maxRadius ) efficientLayer=true;
      
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

  int Efficiency::findAsicKey(CLHEP::Hep3Vector vec, float padSize, int nPadX, int nPadY)
  {
    float I=round( vec.x()/padSize );
    float J=round( vec.x()/padSize );
    if(I>nPadX || J>nPadY || I<0 || J<0) return -1;
    int jnum=(J-1)/8;
    int inum=(I-1)/8;
    int num=inum*12+jnum;
    return vec.z()*1000+num;
  }

}
