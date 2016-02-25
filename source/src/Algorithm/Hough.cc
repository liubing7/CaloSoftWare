#include <Algorithm/Hough.h>

const float PI = 3.1415927;

namespace algorithm{

  void Hough::createHoughObjects(std::vector<caloobject::CaloCluster*> &clusters)
  {
    Distance<caloobject::CaloCluster,caloobject::CaloCluster> dist;
    for( std::vector<caloobject::CaloCluster*>::iterator it=clusters.begin(); it!=clusters.end(); ++it ){
      if( (*it)->getHits().size() > settings.maximumClusterSizeForMip ) continue;
      unsigned int nNeighbours=0;
      unsigned int nCoreNeighbours=0;
      for( std::vector<caloobject::CaloCluster*>::iterator jt=clusters.begin(); jt!=clusters.end(); ++jt ){
	if( (*it)==(*jt) || (*it)->getPosition().z()-(*jt)->getPosition().z() > std::numeric_limits<float>::epsilon() ) continue;
	if( dist.getDistance( (*it),(*jt) ) < settings.transversalDistance ){ 
	  nNeighbours++;
	  if( (*jt)->getHits().size() > settings.maximumClusterSizeForMip ) nCoreNeighbours++;
	}
      }
      if( nNeighbours < settings.maximumNumberOfNeighboursForMip || nCoreNeighbours < settings.maximumNumberOfCoreNeighboursForMip ){
	HoughObject obj;
	obj.cluster=(*it);
	obj.tag=fMip;
	for( unsigned int theta=0; theta<settings.thetaSteps; theta++ ){
	  obj.rhoXVec.push_back( round( (*it)->getPosition().z()*std::cos(-PI/2+theta*PI/settings.thetaSteps)/settings.padSize + 
					(*it)->getPosition().x()*std::sin(-PI/2+theta*PI/settings.thetaSteps)/settings.padSize ) );
	  obj.rhoXVec.push_back( round( (*it)->getPosition().z()*std::cos(-PI/2+theta*PI/settings.thetaSteps)/settings.padSize + 
					(*it)->getPosition().y()*std::sin(-PI/2+theta*PI/settings.thetaSteps)/settings.padSize ) );
	}
	houghObjects.push_back(obj);
      }
    }
  }

  std::vector< HoughBin > Hough::getHoughBins()
  {
    std::vector< HoughBin > houghBins;
    for( std::vector< HoughObject  >::iterator it=houghObjects.begin(); it!=houghObjects.end(); ++it ){
      for( unsigned int theta_x=0; theta_x<settings.thetaSteps; theta_x++ ){
	for( unsigned int theta_y=0; theta_y<settings.thetaSteps; theta_y++ ){
	  std::vector< HoughBin >::iterator jt;
	  for( jt=houghBins.begin(); jt!=houghBins.end(); ++jt ){
	    if( (*it).thetas.at(theta_x)==(*jt).theta_x &&
		(*it).thetas.at(theta_y)==(*jt).theta_y && 
		(*it).rhoXVec.at(theta_x)==(*jt).rho_x &&
		(*it).rhoYVec.at(theta_y)==(*jt).rho_y ){
	      (*jt).houghObjects.push_back(*it);
	      break;
	    }
	  }
	  if( jt!=houghBins.end() ) continue;
	  else{
	    HoughBin bin;
	    bin.theta_x=theta_x;
	    bin.theta_y=theta_y;
	    bin.rho_x=(*it).rhoXVec.at(theta_x);
	    bin.rho_y=(*it).rhoYVec.at(theta_y);
	    houghBins.push_back(bin);
	  }
	}
      }
    }
    for( std::vector< HoughBin >::iterator it=houghBins.begin(); it!=houghBins.end(); ++it){
      if( TestHoughBinSize(*it) ){
    	houghBins.erase(it);
    	--it;
      }
    }
    //houghBins.erase(std::remove_if(hgVec.begin(), hgVec.end(), HoughBinFunction::TrackInLayer),hgVec.end());
    std::sort(houghBins.begin(), houghBins.end(),SortHoughBinBySize::Sort);
    return houghBins;
  }

  void Hough::RemoveIsolatedClusterInHoughBin(HoughBin a)
  {
    Distance<caloobject::CaloCluster,caloobject::CaloCluster> dist;
    for( std::vector<HoughObject>::iterator it=a.houghObjects.begin(); it!=a.houghObjects.end(); ++it ){
      std::vector<HoughObject>::iterator jt;
      for( jt=a.houghObjects.begin(); jt!=a.houghObjects.end(); ++jt ){
	if( it!=jt && dist.getDistance( (*it).cluster,(*jt).cluster ) < settings.globalDistance )
	  break;
      }
      if( jt==a.houghObjects.end() ){
	if( settings.printDebug )
	  std::cout << "Hough::RemoveIsolatedClusterInHoughBin << DEBUG : Find one isloated cluster at : " << (*it).cluster->getPosition() << std::endl;
	houghObjects.erase(it);
	--it;
      }
    }
  }

  void Hough::RemoveTrackedObjects(std::vector<HoughBin> &houghBins)
  {
    for(std::vector<HoughBin>::iterator it=houghBins.begin(); it!=houghBins.end(); ++it){
      for(std::vector<HoughObject>::iterator jt=(*it).houghObjects.begin(); jt!=(*it).houghObjects.end(); ++jt){
	if( (*jt).tag==fTrack ){
	  (*it).houghObjects.erase(jt);
	  --jt;
	}
      }
    }
  }
  
  void Hough::runHough(std::vector<caloobject::CaloCluster*> &clusters, std::vector<caloobject::CaloTrack*> &tracks, algorithm::Tracking *algo_Tracking)
  {
    if( algo_Tracking==NULL ){
      std::cout << "Hough::runHough << ERROR : algo_Tracking is NULL whill it should already be initialised" << std::endl;
      throw;
    }
    createHoughObjects(clusters); 
    std::vector< HoughBin > houghBins = getHoughBins();
    for( std::vector< HoughBin >::iterator it=houghBins.begin(); it!=houghBins.end(); ++it ){
      RemoveIsolatedClusterInHoughBin( *it );
      if( !TestHoughBinSize(*it) ) {
	if( settings.printDebug ) 
	  std::cout << "Hough::runHough << DEBUG : Hough bin size is no longer big enough to create a track after remoing isloated clusters" << std::endl;
	continue;
      }

      caloobject::CaloTrack* track=NULL;
      std::vector<caloobject::CaloCluster*> temp;
      for( std::vector<HoughObject>::iterator jt=(*it).houghObjects.begin(); jt!=(*it).houghObjects.end(); ++jt)
	temp.push_back( (*jt).cluster );

      algo_Tracking->Run( temp,track );
      if( track!=NULL )
	tracks.push_back(track);
    }
  }
}
