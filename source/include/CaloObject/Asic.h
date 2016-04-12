#ifndef ASIC_HH
#define ASIC_HH

#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <CLHEP/Vector/ThreeVector.h>
#include <CaloObject/CaloLayer.h>

namespace caloobject
{
  class Asic
  {
  public:
    Asic(int theKey);
    virtual ~Asic(){;}
    virtual void Update(caloobject::CaloLayer* layer);

    inline const int getAsicKey(){return key;}
    inline const int getAsicNtrack(){return ntrack;}
    inline const float getAsicEfficiency(){return ntrack > 0 ? efficiency/ntrack : 0;}
    inline const float getAsicMultiplicity(){return efficiency > std::numeric_limits<float>::epsilon() ? multi/efficiency : 0.0;}
    inline const float getAsicRMSMultiplicity(){return efficiency > std::numeric_limits<float>::epsilon() ? std::sqrt( rms_multi/efficiency - multi*multi/efficiency/efficiency) : 0.0;}
    inline const CLHEP::Hep3Vector &getPosition(){return positions;}
    
    inline void setASIC_ID(int asicnum, int difnum){asic_num=asicnum; dif_num=difnum;}
    inline const int getAsicNum(){return asic_num;}
    inline const int getAsicDifNum(){return dif_num;}
    
  protected:
    int key;
    int ntrack;
    float efficiency;
    float multi;
    float rms_multi;
  
    int dif_num;
    int asic_num;
    CLHEP::Hep3Vector positions;
  };

  class SDHCAL_Asic : public Asic
  {
  public:
    SDHCAL_Asic(int theKey);
    ~SDHCAL_Asic(){;}
    inline const float getAsicEfficiency2(){return efficiency2/ntrack;}
    inline const float getAsicEfficiency3(){return efficiency3/ntrack;}
    void Update(caloobject::CaloLayer* layer);
  
  private:
    float efficiency2;
    float efficiency3;
  };
}
#endif
