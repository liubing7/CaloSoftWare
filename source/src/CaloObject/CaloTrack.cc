#include "CaloObject/CaloTrack.h"

namespace caloobject
{

  CaloTrack::CaloTrack(std::vector<caloobject::CaloCluster*> &vec)
  {
    clusters=vec;
  }
}
