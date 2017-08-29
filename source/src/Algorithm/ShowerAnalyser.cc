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
	std::set<int> layers ;
	std::vector<CLHEP::Hep3Vector> hitPos ;
	std::vector<int> clSize ; //=1 because it use hits and not clusters
	std::map<int,float> edep_layerMap ;
	std::vector<double> x,y,z ;

	shower->edep = 0.0f ;

	shower->rmsEdep = 0.0f ;
	for ( HitVec::const_iterator it = shower->getHits().begin() ; it != shower->getHits().end() ; ++it )
	{
		shower->edep+=(*it)->getEnergy();
		shower->edepPerCell.push_back( (*it)->getEnergy() );
		shower->rmsEdep+=(*it)->getEnergy()*(*it)->getEnergy();
		layers.insert( (*it)->getCellID()[2] );
		hitPos.push_back( (*it)->getPosition() );
		clSize.push_back( 1 );
		shower->hitTimes.push_back( (*it)->getTime() );

		if( edep_layerMap[ (*it)->getCellID()[2] ] > 0 )
			edep_layerMap[ (*it)->getCellID()[2] ] += (*it)->getEnergy() ;
		else
			edep_layerMap[ (*it)->getCellID()[2] ] = (*it)->getEnergy() ;



		x.push_back( (*it)->getPosition().x() );
		y.push_back( (*it)->getPosition().y() );
		z.push_back( (*it)->getPosition().z() );

	}
	shower->meanEdep=shower->edep/shower->getHits().size();
	shower->rmsEdep=std::sqrt( shower->rmsEdep/shower->getHits().size() - shower->meanEdep*shower->meanEdep );

	FindEnergy(shower);


	shower->showerMax = algorithm::map_max_element(edep_layerMap)->first; //x0 unit
	shower->edepAtMax = algorithm::map_max_element(edep_layerMap)->second;

	shower->nlayer = layers.size() ;

	algorithm::LinearFit3D* algo_LinearFit3D = new algorithm::LinearFit3D(hitPos,clSize) ;
	shower->thrust.clear() ;
	for(int i=0; i<4; i++)
		shower->thrust.push_back( algo_LinearFit3D->getFitParameters()[i] ) ;

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

//	int begin=0;
//	if( shower->firstIntCluster!=NULL )
//		begin=shower->firstIntCluster->getLayerID();


	Distance<caloobject::CaloHit,CLHEP::Hep3Vector> dist;
	for(std::vector<caloobject::CaloHit*>::const_iterator it=shower->getHits().begin(); it!=shower->getHits().end(); ++it){
		if( (*it)->getCellID()[2] >= settings.geometry.nLayers ){
			std::cout << "Problem in ShowerAnalyser::Profile \n\t => Hit found at " << (*it)->getCellID()[2] << " while settings.geometry.nLayers = " << settings.geometry.nLayers << std::endl;
			continue;
		}
		//if( (*it)->getCellID()[2]>=begin )
		shower->longitudinal.at( (*it)->getCellID()[2] ) +=(*it)->getEnergy();

		CLHEP::Hep3Vector vec(shower->thrust[0]+shower->thrust[1]*(*it)->getPosition().z(),
				shower->thrust[2]+shower->thrust[3]*(*it)->getPosition().z(),
				(*it)->getPosition().z());
		shower->distanceToAxis.push_back( dist.getDistance( (*it),vec ) );
		int ring=(int)(dist.getDistance( (*it),vec )/settings.geometry.pixelSize );
		if( ring>(int)(settings.maximumRadius/settings.geometry.pixelSize-1) || ring<0 )
			continue;
		else
			shower->transverse.at(ring)+=(*it)->getEnergy();
	}
}


void ShowerAnalyser::Run(caloobject::SDHCALShower* shower)
{
	std::set<int> layers;
	std::vector<CLHEP::Hep3Vector> hitPos;
	std::vector<int> clSize; //=1 because it use hits and not clusters
	std::map<int,int> nhit_layerMap;
	std::vector<double> x,y,z;


	for(std::vector<caloobject::CaloHit*>::const_iterator it=shower->getHits().begin(); it!=shower->getHits().end(); ++it)
	{
		layers.insert( (*it)->getCellID()[2] );
		hitPos.push_back( (*it)->getPosition() );
		clSize.push_back( 1 );

		if( nhit_layerMap[ (*it)->getCellID()[2] ] > 0 )
			nhit_layerMap[ (*it)->getCellID()[2] ] ++ ;
		else
			nhit_layerMap[ (*it)->getCellID()[2] ] = 1 ;


		x.push_back( (*it)->getPosition().x() );
		y.push_back( (*it)->getPosition().y() );
		z.push_back( (*it)->getPosition().z() );

		if( (int)(*it)->getEnergy()==1 )
			shower->sdnhit[0]++;
		if( (int)(*it)->getEnergy()==2 )
			shower->sdnhit[1]++;
		if( (int)(*it)->getEnergy()==3 )
			shower->sdnhit[2]++;

	}

	shower->showerMax = algorithm::map_max_element(nhit_layerMap)->first; //x0 unit
	shower->nhitAtMax = algorithm::map_max_element(nhit_layerMap)->second;

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
	shower->transverseRatio = std::sqrt( algo_PCA->eigenValues()[0]*algo_PCA->eigenValues()[0] + algo_PCA->eigenValues()[1]*algo_PCA->eigenValues()[1] )/algo_PCA->eigenValues()[2] ;
	delete algo_PCA;

	FindEnergy(shower);
	SearchShowerInteraction(shower) ;

	int lastLayer = -10 ;

//	for ( auto jt = shower->getClusters().begin() ; jt != shower->getClusters().end() ; ++jt )
	for ( auto& jt : shower->getClusters() )
	{
		if ( jt->getLayerID() > lastLayer )
			lastLayer = jt->getLayerID() ;
	}

	shower->lastClusterPosition = lastLayer ;

	Profile(shower) ;
}

void ShowerAnalyser::FindEnergy(caloobject::SDHCALShower* shower)
{
	if( settings.energyCalibrationOption==std::string("SDHCAL_Quadratic") )
	{
		int NhitTot=shower->sdnhit[0]+shower->sdnhit[1]+shower->sdnhit[2];
		if( settings.energyCalibrationFactors.size() != 9 )
			return;
		shower->energy=
				shower->sdnhit[0]*(settings.energyCalibrationFactors.at(0) + settings.energyCalibrationFactors.at(1)*NhitTot + settings.energyCalibrationFactors.at(2)*NhitTot*NhitTot)+
				shower->sdnhit[1]*(settings.energyCalibrationFactors.at(3) + settings.energyCalibrationFactors.at(4)*NhitTot + settings.energyCalibrationFactors.at(5)*NhitTot*NhitTot)+
				shower->sdnhit[2]*(settings.energyCalibrationFactors.at(6) + settings.energyCalibrationFactors.at(7)*NhitTot + settings.energyCalibrationFactors.at(8)*NhitTot*NhitTot);
	}
}

void ShowerAnalyser::Profile(caloobject::SDHCALShower* shower)
{
	shower->longitudinal.clear();
	shower->transverse.clear();
	shower->distanceToAxis.clear();
	for(int i=0; i<settings.geometry.nLayers; i++)
		shower->longitudinal.push_back(0);
	for(int i=0; i<(int)(settings.maximumRadius/settings.geometry.pixelSize); i++)
		shower->transverse.push_back(0);

//	int begin=0;
//	if( shower->firstIntCluster!=NULL )
//		begin=shower->firstIntCluster->getLayerID();

	Distance<caloobject::CaloHit,CLHEP::Hep3Vector> dist;
	for(std::vector<caloobject::CaloHit*>::const_iterator it=shower->getHits().begin(); it!=shower->getHits().end(); ++it)
	{
		if( (*it)->getCellID()[2] >= settings.geometry.nLayers )
		{
			std::cout << "Problem in ShowerAnalyser::Profile \n\t => Hit found at " << (*it)->getCellID()[2] << " while settings.geometry.nLayers = " << settings.geometry.nLayers << std::endl;
			continue;
		}
		shower->longitudinal.at( (*it)->getCellID()[2] ) ++;

		CLHEP::Hep3Vector vec(shower->thrust[0]+shower->thrust[1]*(*it)->getPosition().z(),
				shower->thrust[2]+shower->thrust[3]*(*it)->getPosition().z(),
				(*it)->getPosition().z());
		shower->distanceToAxis.push_back( dist.getDistance( (*it),vec ) );
		int ring=(int)(dist.getDistance( (*it),vec )/settings.geometry.pixelSize );
		if( ring>(int)(settings.maximumRadius/settings.geometry.pixelSize-1) || ring<0 )
			continue;
		else
			shower->transverse.at(ring) ++;

	}
}
}
