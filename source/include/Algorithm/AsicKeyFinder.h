#ifndef ASICKEYFINDER_HH
#define ASICKEYFINDER_HH

#include <iostream>
#include <vector>

#include <CLHEP/Vector/ThreeVector.h>

namespace algorithm{
  
  struct AsicKeyFinderParameterSetting{
    std::vector<int> keyFactor;
    int nPadX;// number of pads in x direction per layer
    int nPadY;// number of pads in y direction per layer
    int asicNPad;// number of pads in x or y direction per asic (assuming a square)
    float layerGap;// gap between 2 layers
    float padSize;
    bool printDebug;
  AsicKeyFinderParameterSetting() : keyFactor(0) ,
                                    nPadX(96),
                                    nPadY(96),
                                    asicNPad(8),
                                    layerGap(26.131),
                                    padSize(10.408),
                                    printDebug(false)
                                    {;}
  };


  class AsicKeyFinder {
  public:
    AsicKeyFinder(){;}
    ~AsicKeyFinder(){;}
    
    inline void SetAsicKeyFinderParameterSetting(AsicKeyFinderParameterSetting params){settings=params;}
    int BuildAsicKey(int i, int j, int k);//{return i*settings.keyFactor.at(0);}
    int FindAsicKey(CLHEP::Hep3Vector vec);

  private:
    AsicKeyFinderParameterSetting settings;
  };
}
#endif
