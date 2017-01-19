#include "CaloObject/Asic2.h"

/*
namespace caloobject
{

Asic::Asic(int theKey)
{
	key = theKey ;
	ntrack = 0.0 ;
	efficiency = 0.0 ;
	multi = 0.0 ;
	multisq = 0.0 ;
	rms_multi = 0.0 ;
	dif_num = 0 ;
	asic_num = 0 ;
	positions = CLHEP::Hep3Vector(0.0,0.0,0.0) ;
}

void Asic::Update(caloobject::CaloLayer* layer)
{
	ntrack += 1 ;
	efficiency += layer->getEfficiency() ;
	multi += layer->getMultiplicity() ;
	multisq += layer->getMultiplicity()*layer->getMultiplicity() ;
	rms_multi = multi*multi ;
}


SDHCAL_Asic::SDHCAL_Asic(int theKey) : Asic(theKey) , efficiency2(0.0) , efficiency3(0.0)
{
	positions=CLHEP::Hep3Vector( theKey%1000%12 ,
								 theKey%1000/12 ,
								 theKey/1000 );
}

void SDHCAL_Asic::Update(caloobject::CaloLayer* layer)
{
	ntrack += 1 ;
	efficiency += layer->getEfficiency() ;
	multi += layer->getMultiplicity() ;
	multisq += layer->getMultiplicity()*layer->getMultiplicity() ;
	rms_multi = multi*multi ;
	if( layer->getEfficiencyEnergy() == 3 )
	{
		efficiency3 += layer->getEfficiency() ;
		efficiency2 += layer->getEfficiency() ;
	}
	else if( layer->getEfficiencyEnergy() == 2 )
	{
		efficiency2 += layer->getEfficiency() ;
	}
}



SDHCAL_AsicCustom::SDHCAL_AsicCustom(int theKey) : Asic(theKey)
{
	positions=CLHEP::Hep3Vector( theKey%1000%12 ,
								 theKey%1000/12 ,
								 theKey/1000 ) ;
}

void SDHCAL_AsicCustom::Update(caloobject::CaloLayer* layer)
{
	ntrack++ ;
	efficiency += layer->getEfficiency() ;
	multi += layer->getMultiplicity() ;
	multisq += layer->getMultiplicity()*layer->getMultiplicity() ;
	rms_multi = multi*multi ;

	double energy = layer->getEfficiencyEnergy() ;

	for ( unsigned int i = 0 ; i < thresholds.size() ; ++i )
	{
		if ( energy >= thresholds.at(i) )
			efficiencies.at(i)++ ;
	}
}

std::vector<double> SDHCAL_AsicCustom::getEfficiencies() const
{
	std::vector<double> toReturn ;

	for ( unsigned int i = 0 ; i < thresholds.size() ; ++i )
		toReturn.push_back( static_cast<double>( efficiencies.at(i) )/ntrack ) ;

	return toReturn ;
}

std::vector<double> SDHCAL_AsicCustom::getEfficienciesError() const
{
	std::vector<double> toReturn ;

	for ( unsigned int i = 0 ; i < thresholds.size() ; ++i )
	{
		double eff , effErr ;

		if ( efficiencies.at(i) == ntrack )
		{
			eff = 1.0 ;
			double falseEff = (ntrack-1.0)/ntrack ;
			effErr = std::sqrt(falseEff*(1-falseEff)/ntrack) ;
		}
		else if ( efficiencies.at(i) == 0 )
		{
			eff = 0.0 ;
			double falseEff = 1.0/ntrack ;
			effErr = std::sqrt(falseEff*(1-falseEff)/ntrack) ;
		}
		else
		{
			eff = static_cast<double>( efficiencies.at(i) )/ntrack ;
			effErr = sqrt( eff*(1-eff)/ntrack ) ;
		}

		toReturn.push_back(effErr) ;
	}

	return toReturn ;
}


}
*/
