#include "CaloObject/Asic.h"

namespace caloobject
{
  Asic::Asic(int theKey)
  {
    key=theKey;
    ntrack=0.0;
    efficiency=0.0;
    multi=0.0;
    rms_multi=0.0;
    dif_num=0;
    asic_num=0;
    positions=CLHEP::Hep3Vector(0.0,0.0,0.0);
  }
  
  void Asic::Update(caloobject::CaloLayer* layer)
  {
    ntrack+=1;
    efficiency+=layer->getEfficiency();
    multi+=layer->getMultiplicity();
    rms_multi=multi*multi;
  }

  /*------------------------------------------------------------------------------------*/
  /*------------------------------------------------------------------------------------*/

  SDHCAL_Asic::SDHCAL_Asic(int theKey) : Asic(theKey) , efficiency2(0.0) , efficiency3(0.0) 
  {
    positions=CLHEP::Hep3Vector( theKey%1000/12 ,
				 theKey%1000%12 ,
				 theKey/1000 );
  }

  void SDHCAL_Asic::Update(caloobject::CaloLayer* layer)
  {
    ntrack+=1;
    efficiency+=layer->getEfficiency();
    multi+=layer->getMultiplicity();
    rms_multi=multi*multi;
    if( layer->getEfficiencyEnergy()==3 ){
      efficiency3+=layer->getEfficiency();
      efficiency2+=layer->getEfficiency();
    }
    else if( layer->getEfficiencyEnergy()==2 ){
      efficiency2+=layer->getEfficiency();
    }
  }
}
