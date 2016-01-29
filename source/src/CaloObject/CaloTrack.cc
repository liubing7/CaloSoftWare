#include "CaloObject/CaloTrack.h"

namespace caloobject
{

  CaloTrack::CaloTrack(std::vector<caloobject::CaloCluster*> &vec)
  {
    _clusters=vec;
    Update();
  }

  void CaloTrack::Update()
  {
  }

}
