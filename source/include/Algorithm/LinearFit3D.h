#ifndef LINEAR3DFIT_HH
#define LINEAR3DFIT_HH

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <vector>

#include "CLHEP/Vector/ThreeVector.h"
/*!
 * Class LinearFit3D.
 * Fit a 3D line in 3D space. The line equation is the following one :
 *       x = a*z + b
 *       y = c*z + d
 *       z = t
 */

namespace algorithm{

  class LinearFit3D {

  protected :
    std::vector<CLHEP::Hep3Vector> _positions;
    std::vector<int> _clusterSize;
    float _chi2;
    float _params[4];
    float _paramsError[4];
    void ComputeChi2();
    CLHEP::Hep3Vector VectorFromRealLine( const CLHEP::Hep3Vector& );
    CLHEP::Hep3Vector GetNormaleProjection( const CLHEP::Hep3Vector& );

  public :
    LinearFit3D( std::vector<CLHEP::Hep3Vector> &pos , std::vector<int> &ClustersSize);
    virtual ~LinearFit3D(){;}
    inline float getChi2(){ return _chi2; }
    inline float* getFitParameters(){ return _params; }
    inline float* getFitParError(){ return _paramsError; }
  };

}
#endif
