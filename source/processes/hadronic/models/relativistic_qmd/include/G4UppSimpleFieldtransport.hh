
#ifndef G4UPPSIMPLEFIELDTRANSPORT_H
#define G4UPPSIMPLEFIELDTRANSPORT_H


#include "G4VUppFieldtransport.hh"
#include "G4UppTrackVector.hh"


class G4UppSimpleFieldtransport : public G4VUppFieldtransport
{
public:

  void propagate(G4UppTrackVector& allTracks, const G4double dTime) const;

};


#endif // G4UPPSIMPLEFIELDTRANSPORT_H
