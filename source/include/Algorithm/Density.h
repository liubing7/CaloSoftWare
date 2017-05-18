#ifndef Density_h
#define Density_h

#include "CaloObject/CaloHit.h"

#include <map>
#include <cmath>
#include <algorithm>

namespace algorithm
{

class DensityDistanceFunction
{
	public :
		DensityDistanceFunction() ;
		virtual ~DensityDistanceFunction() ;

		virtual double eval(caloobject::CaloHit* hitA , caloobject::CaloHit* hitB) const = 0 ;

	protected :

} ;

class DensityDistanceFunction3By3 : public DensityDistanceFunction
{
	public :
		DensityDistanceFunction3By3() ;
		virtual ~DensityDistanceFunction3By3() ;

		virtual double eval(caloobject::CaloHit* hitA , caloobject::CaloHit* hitB) const ;

	protected :

} ;


class Density
{
	public :
		Density() ;
		~Density() ;

		double compute(HitVec& hitVec) ;


	protected :
		std::map<caloobject::CaloHit* , double> densityPerHit ;

		DensityDistanceFunction* densityFunc ;

} ;


} //namespace algorithm

#endif //Density_h
