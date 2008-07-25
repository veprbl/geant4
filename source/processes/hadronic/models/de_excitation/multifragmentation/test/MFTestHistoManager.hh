
// Igor Pshenichnov 01.12.2006

#ifndef MFTestHistoManager_h
#define MFTestHistoManager_h 1

#include "globals.hh"

class TFile;
class TH1D;
class TH2D;

class MFTestHistoManager
{
  public:

  MFTestHistoManager();
   ~MFTestHistoManager();

#ifdef G4ANALYSIS_USE_ROOT
  TH1D* GetHisto(G4int id) {return histo[id];};
  TH2D* GetHisto2(G4int id) {return histo2[id];};
#endif
       
  void BookHisto();
  void NormalizeHisto();
  void CleanHisto();

  void fill(G4int i, G4double x, G4double y);
  void fill2(G4int i, G4double x, G4double y, G4double z);

  inline G4int GetZ()    {return Z;};
  inline G4int GetA()    {return A;};
  inline G4int GetIterations()  {return iterations;};
  inline G4double GetLowEn() {return lowLimitExEn;};
  inline G4double GetUpEn() {return upperLimitExEn;};


  
  private:

#ifdef G4ROOT_USE
  TFile* fFile;
  TH1D*  histo[20];
  TH2D*  histo2[10];   
#endif


  G4int Z;
  G4int A;
  G4int iterations; 
   
  G4String fileName;
  G4String fileType;

  G4String     fileFullName;
  G4int        compressionFactor;

  G4double lowLimitExEn;
  G4double upperLimitExEn;
  G4int    binsExEn;
  G4int    eventsPerBin;      

};

#endif
