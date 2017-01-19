#ifndef CALOLAYER_HH
#define CALOLAYER_HH

#include <iostream>
#include <limits>

namespace caloobject
{   
  class CaloLayer
  {
    //friend class algorithm::LayerProperties;
  public :
	CaloLayer(int id) ;
	~CaloLayer(){ ; }

	void update() ;
	void update(int multi, float energy) ;
	void Reset()
	{
      _multiplicity=0.;
      _efficiency=0.;
      _efficiency_energy=0.;
      _ntracks=0;
    }

    inline void setLayerZPosition(float z){_layerZPosition=z;}
    //    inline void setLayerParameterSetting(LayerParameterSetting params){_settings=params;}
    
	inline int getNTracks() const {return _ntracks;}
	inline float getEfficiency() const {return (float)_efficiency/_ntracks;} //could be used after each event or at the end of the analysis or whenever you want
	inline float getMultiplicity() const {return _efficiency > std::numeric_limits<float>::epsilon() ? (float)_multiplicity/_efficiency : 0.0;} //could be used after each event or at the end of the analysis or whenever you want
	inline float getEfficiencyEnergy() const {return _efficiency_energy;} //this should be called for each event (if needed)
	inline int getLayerID() const {return _layerID;}
	inline float getLayerZPosition() const {return _layerZPosition;}
    
    //inline const LayerParameterSetting getSettings(){return _settings;}

  private :
    int _layerID;
    //LayerParameterSetting _settings;
    float _layerZPosition;
    float _multiplicity;
    float _efficiency;
    float _efficiency_energy; //max cell energy in the cluster (semi digital) or cluster energy used to say that the layer is efficient
    int _ntracks;
  } ;

}
#endif
