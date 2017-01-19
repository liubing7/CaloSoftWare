#include "CaloObject/CaloLayer.h"

namespace caloobject
{

CaloLayer::CaloLayer(int id)
{
	_layerID=id;
	_multiplicity=0.;
	_efficiency=0.;
	_efficiency_energy=0.;
	_ntracks=0;
	_layerZPosition = (_layerID+1)*26.131f ;
}

void CaloLayer::update()
{
	_ntracks++ ;
}

void CaloLayer::update(int multi, float energy)
{
	_efficiency++ ;
	_multiplicity += multi ;
	_efficiency_energy += energy ;
	_ntracks++ ;
}

}
