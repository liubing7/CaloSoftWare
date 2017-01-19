#include "Algorithm/Efficiency.h"

namespace algorithm
{

void Efficiency::Run(caloobject::CaloLayer *layer, std::vector<caloobject::CaloCluster2D*> &clusters)
{
	std::vector<caloobject::CaloCluster2D*> clusterVec;
	std::vector<caloobject::CaloCluster2D*> clustersInLayer;
	for(std::vector<caloobject::CaloCluster2D*>::iterator it=clusters.begin(); it!=clusters.end(); ++it)
	{
		if( (*it)->getLayerID()==layer->getLayerID() )
			clustersInLayer.push_back(*it) ;
		else
			clusterVec.push_back(*it) ;
	}

	algorithm::Tracking* algo_tracking = new algorithm::Tracking() ;
	algo_tracking->SetTrackingParameterSetting(settings.trackingParams) ;
	caloobject::CaloTrack* track = NULL ;
	algo_tracking->Run(clusterVec,track) ;
	delete algo_tracking ;
	if (track != NULL)
	{
		expectedPos = CLHEP::Hep3Vector(track->getTrackParameters()[1]*layer->getLayerZPosition()+track->getTrackParameters()[0] ,
				track->getTrackParameters()[3]*layer->getLayerZPosition()+track->getTrackParameters()[2] ,
				layer->getLayerZPosition() ) ;
		if(expectedPos.x()>settings.geometry.xmax ||
		   expectedPos.x()<settings.geometry.xmin ||
		   expectedPos.y()>settings.geometry.ymax ||
		   expectedPos.y()<settings.geometry.ymin)
		{
			delete track ;
			if( settings.printDebug )
				std::cout << "expected track impact outside layer " << layer->getLayerID() << ":\t"
						  << "expectedPos.x() = " << expectedPos.x() << "\t"
						  << "expectedPos.y() = " << expectedPos.y() << std::endl ;
			return ;
		}
		if ( clustersInLayer.empty() )
		{
			if( settings.printDebug )
				std::cout << "find one empty layer = " << layer->getLayerID() << std::endl ;
			layer->update() ;
			delete track ;
			return ;
		}
		Distance<caloobject::CaloCluster2D,caloobject::CaloTrack> dist;
		std::vector<caloobject::CaloCluster2D*>::iterator closestIt=clustersInLayer.begin();
		float old_dist=dist.getDistanceInLayer( (*closestIt),track );
		float new_dist=0.0;
		for(std::vector<caloobject::CaloCluster2D*>::iterator it=clustersInLayer.begin()+1; it!=clustersInLayer.end(); ++it)
		{
			if( (*it)->getLayerID() != layer->getLayerID() )
			{
				std::cout << "Efficiency algorithm problem in Run(caloobject::CaloLayer *layer, std::vector<caloobject::CaloCluster2D*> &clusters)" << std::endl;
				throw;
			}
			new_dist=dist.getDistanceInLayer((*it),track);
			if( new_dist<old_dist )
			{
				closestIt=it;
				old_dist=new_dist;
			}
		}
		Distance<caloobject::CaloHit,caloobject::CaloTrack> distHit;
		bool efficientLayer=false;
		float energy=0.;
		for(std::vector<caloobject::CaloHit*>::iterator it=(*closestIt)->getHits().begin(); it!=(*closestIt)->getHits().end(); ++it)
		{
			if( distHit.getDistanceInLayer( (*it),track ) < settings.maxRadius )
				efficientLayer=true;

			if( efficientLayer==true )
			{
				if( settings.semiDigitalReadout == true )
				{
					if( energy<(*it)->getEnergy() ) energy=(*it)->getEnergy();
				}
				else
					energy+=(*it)->getEnergy();
			}
		}
		if( efficientLayer==true )
		{
			layer->update((*closestIt)->getHits().size(),energy);
		}
		else
			layer->update();

		delete track;
	}
}

Efficiency2::Efficiency2()
{
	goodCluster = NULL ;
	_isTrack = false ;
}

Efficiency2::~Efficiency2()
{}

void Efficiency2::Run(caloobject::Layer *layer, std::vector<caloobject::CaloCluster2D*> &clusters)
{
	_isTrack = false ;
	goodCluster = NULL ;

	std::vector<caloobject::CaloCluster2D*> clusterVec ;
	std::vector<caloobject::CaloCluster2D*> clustersInLayer ;
	for( std::vector<caloobject::CaloCluster2D*>::iterator it=clusters.begin() ; it!=clusters.end() ; ++it )
	{
		if( (*it)->getLayerID() == layer->getID() )
			clustersInLayer.push_back(*it) ;
		else
			clusterVec.push_back(*it) ;
	}

	algorithm::Tracking* algo_tracking = new algorithm::Tracking() ;
	algo_tracking->SetTrackingParameterSetting(settings.trackingParams) ;
	caloobject::CaloTrack* track = NULL ;
	algo_tracking->Run(clusterVec , track) ;
	delete algo_tracking ;
	if (track != NULL)
	{
		_isTrack = true ;
		expectedPos = CLHEP::Hep3Vector(track->getTrackParameters()[1]*layer->getPosition().z() + track->getTrackParameters()[0] ,
										track->getTrackParameters()[3]*layer->getPosition().z() + track->getTrackParameters()[2] ,
										layer->getPosition().z() ) ;

//		std::cout << "expectedPos : " << expectedPos << std::endl ;
		if(expectedPos.x()>settings.geometry.xmax ||
		   expectedPos.x()<settings.geometry.xmin ||
		   expectedPos.y()>settings.geometry.ymax ||
		   expectedPos.y()<settings.geometry.ymin)
		{
			_isTrack = false ;
			delete track ;
			if( settings.printDebug )
				std::cout << "expected track impact outside layer " << layer->getID() << ":\t"
						  << "expectedPos.x() = " << expectedPos.x() << "\t"
						  << "expectedPos.y() = " << expectedPos.y() << std::endl ;
			return ;
		}

		if ( clustersInLayer.empty() )
		{
			if( settings.printDebug )
				std::cout << "find one empty layer = " << layer->getID() << std::endl ;
			delete track ;
			return ;
		}

		Distance<caloobject::CaloCluster2D,caloobject::CaloTrack> dist ;
		std::vector<caloobject::CaloCluster2D*>::iterator closestIt = clustersInLayer.begin() ;
		float old_dist = dist.getDistanceInLayer( (*closestIt) , track ) ;
		float new_dist = 0.0 ;

		for( std::vector<caloobject::CaloCluster2D*>::iterator it = clustersInLayer.begin()+1 ; it != clustersInLayer.end() ; ++it )
		{
			if( (*it)->getLayerID() != layer->getID() )
			{
				std::cout << "Efficiency algorithm problem in Run(caloobject::Layer *layer, std::vector<caloobject::CaloCluster2D*> &clusters)" << std::endl ;
				throw ;
			}
			new_dist = dist.getDistanceInLayer( (*it) , track) ;
			if( new_dist < old_dist )
			{
				closestIt = it ;
				old_dist = new_dist ;
			}
		}

		Distance<caloobject::CaloHit,caloobject::CaloTrack> distHit ;
		for(std::vector<caloobject::CaloHit*>::iterator it = (*closestIt)->getHits().begin() ; it != (*closestIt)->getHits().end() ; ++it)
		{
			if( distHit.getDistanceInLayer( (*it),track ) < settings.maxRadius )
				goodCluster = (*closestIt) ;
		}

		delete track ;
	}
}






} //namespace algorithm
