#ifndef CALOLAYER_HH
#define CALOLAYER_HH

#include <iostream>

namespace caloobject
{ 
  
  enum EfficiencyLayerTag{
    unknown,
    efficient,
    unefficient
  };
  
  class CaloLayer
  {
    //friend class algorithm::LayerProperties;
  public:
    CaloLayer(int id);
    ~CaloLayer(){;}

    void update();
    void update(int multi, float energy);

    inline const float getEfficiency(){return (float)_efficiency/_ntracks;} //could be used after each event or at the end of the analysis or whenever you want
    inline const float getMultiplicity(){return (float)_multiplicity/_efficiency;} //could be used after each event or at the end of the analysis or whenever you want

    inline const float getEfficiencyEnergy(){return _efficiency_energy;} //this should be called for each event (if needed)

    inline const int getLayerID(){return _layerID;}
    
  private:
    int _layerID;
    float _multiplicity;
    float _efficiency;
    float _efficiency_energy; //max cell energy in the cluster (semi digital) or cluster energy used to say that the layer is efficient
    int _ntracks;
  };

}
#endif
