#include "Algorithm/AsicKeyFinder.h"

namespace algorithm
{
  int AsicKeyFinder::BuildAsicKey(int i, int j, int k)
  {
    if( settings.keyFactor.size()!=3 ){
      std::cout << "Asic key factor vector has a wrong size : it should be 3 for AsicKeyFinder::BuildAsicKey(int i, int j, int k) >> THROW" << std::endl;
      throw;
    }
    return i*settings.keyFactor.at(0) + j*settings.keyFactor.at(1) + k*settings.keyFactor.at(2);
  }
  
  int AsicKeyFinder::FindAsicKey(CLHEP::Hep3Vector vec)
  {
    float I=round( vec.x()/settings.padSize );
    float J=round( vec.y()/settings.padSize );
    float K=round( vec.z()/settings.layerGap );
    if(I>settings.nPadX || J>settings.nPadY || I<0 || J<0){
      if( settings.printDebug )
	std::cout << "Impossible to find the Asic key with AsicKeyFinder::FindAsicKey(CLHEP::Hep3Vector vec) with vec = " << vec << "\t return -1" << std::endl;
      return -1;
    }
    return 
      K*settings.keyFactor.at(2) + 
      ((J-1)/settings.asicNPad)*settings.keyFactor.at(1) + 
      (I-1)/settings.asicNPad*settings.keyFactor.at(0);
  }
}
