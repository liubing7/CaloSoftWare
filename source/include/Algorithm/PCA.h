#ifndef PCA_HH
#define PCA_HH

#include <iostream>
#include <Eigen/Dense>
#include <vector>

typedef  std::vector< std::vector<double> > VectorList;

namespace algorithm
{

  class PCA {  
  public:  
    PCA(VectorList &vec);
    //vectorlist fill with for example 3 vectors (x,y,z) with n values each
    ~PCA(){;}

    inline Eigen::MatrixXd eigenVectors(){ return _eigenVectors; }
    inline std::vector<double> &eigenValues(){ return _eigenValues; }

  protected:
    std::vector<double> _eigenValues;
    Eigen::MatrixXd _eigenVectors;
    bool trackSuccess;
  };

}
#endif  //  PCA_HH
