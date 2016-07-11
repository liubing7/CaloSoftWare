#ifndef SHOWERANALYSER_HH
#define SHOWERANALYSER_HH

#include <iostream>
#include <cmath>
#include <vector>

#include <CaloObject/Shower.h>
#include <CaloObject/CaloGeom.h>
#include <Algorithm/InteractionFinder.h>

namespace algorithm{  
   
  struct ShowerAnalyserParameterSetting{
    std::string energyCalibrationOption; //not implemented yet
    float maximumRadius; //define the radius of the most outer ring for transverse profile
    std::vector<float> energyCalibrationFactors;
    algorithm::InteractionFinderParameterSetting interactionFinderParams;
    caloobject::GeomParameterSetting geometry;
  ShowerAnalyserParameterSetting() : energyCalibrationOption(std::string("SiWEcal")) ,
      maximumRadius(1000.0f)
    {
      energyCalibrationFactors.push_back(1.0f);
    }
  };

  class ShowerAnalyser {
  public:
    ShowerAnalyser(){;}
    ~ShowerAnalyser(){;}
    void Run(caloobject::Shower *shower);
    void Run(caloobject::SDHCALShower *shower);
    
    inline void SetShowerAnalyserParameterSetting(ShowerAnalyserParameterSetting params){settings=params;}
  private:
    void FindEnergy(caloobject::Shower *shower);
    void FindEnergy(caloobject::SDHCALShower *shower);
    void SearchShowerInteraction(caloobject::Shower *shower);
    void Profile(caloobject::Shower *shower);
    void Profile(caloobject::SDHCALShower *shower);
    ShowerAnalyserParameterSetting settings;
  };
}
#endif
