#ifndef CALOGEOM_HH
#define CALOGEOM_HH

#include <iostream>

namespace caloobject
{ 
  
  struct GeomParameterSetting{
    int nLayers;
    int nPixelsPerLayer;
    int nPixelsX;
    int nPixelsY;
    float xmin;
    float xmax;
    float ymin;
    float ymax;
    float pixelSize;
    float layerGap;
    float firstLayerZ;
    int firstSectionLastLayer; // assume first layer is number 0
    int secondSectionLastLayer; // assume first layer is number 0
    int thirdSectionLastLayer; // assume first layer is number 0
    int fourthSectionLastLayer; // assume first layer is number 0 (start of cms HGCAL FH?)
  GeomParameterSetting() : nLayers(48) ,
      nPixelsPerLayer(96) ,
      nPixelsX(96) ,
      nPixelsY(96) ,
      xmin(-500.0) ,
      xmax(500.0) ,
      ymin(-500.0) ,
      ymax(500.0) ,
      pixelSize(10.406) ,
      layerGap(26.131) ,
      firstLayerZ(10.0),
      firstSectionLastLayer(10),
      secondSectionLastLayer(20),
      thirdSectionLastLayer(30),
      fourthSectionLastLayer(40)
    {;}
  };
  
}
#endif
