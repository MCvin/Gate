/*----------------------
   Copyright (C): OpenGATE Collaboration

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/


#ifndef GateBooleanMessenger_h
#define GateBooleanMessenger_h 1

#include "globals.hh"

#include "GateVolumeMessenger.hh"

class GateBoolean;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class GateBooleanMessenger: public GateVolumeMessenger
{
  public:
    GateBooleanMessenger(GateBoolean* itsCreator);
   ~GateBooleanMessenger();
    
    void SetNewValue(G4UIcommand*, G4String);
    
    virtual inline GateBoolean* GetBooleanCreator() 
      { return (GateBoolean*)GetVolumeCreator(); }

  private:
    G4UIcmdWithADoubleAndUnit* BooleanRminCmd;
    G4UIcmdWithADoubleAndUnit* BooleanRmaxCmd;
    G4UIcmdWithADoubleAndUnit* BooleanSPhiCmd;
    G4UIcmdWithADoubleAndUnit* BooleanDPhiCmd;
    G4UIcmdWithADoubleAndUnit* BooleanSThetaCmd;
    G4UIcmdWithADoubleAndUnit* BooleanDThetaCmd;
};

#endif

