/*----------------------
   Copyright (C): OpenGATE Collaboration

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/


#include "GateBooleanMessenger.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"

#include "GateBoolean.hh"

//-------------------------------------------------------------------------------------
GateBooleanMessenger::GateBooleanMessenger(GateBoolean *itsCreator)
: GateVolumeMessenger(itsCreator)
{ 

  G4String dir = GetDirectoryName() + "geometry/";
  
  G4String cmdName = dir+"setRmin";
  BooleanRminCmd = new G4UIcmdWithADoubleAndUnit(cmdName.c_str(),this);
  
  G4cout << " Rmin" << cmdName.c_str() << Gateendl;
  
  BooleanRminCmd->SetGuidance("Set internal radius of the boolean (0 for full boolean).");
  BooleanRminCmd->SetParameterName("Rmin",false);
  BooleanRminCmd->SetRange("Rmin>=0.");
  BooleanRminCmd->SetUnitCategory("Length");

  cmdName = dir+"setRmax";
  BooleanRmaxCmd = new G4UIcmdWithADoubleAndUnit(cmdName.c_str(),this);
  BooleanRmaxCmd->SetGuidance("Set external radius of the boolean.");
  BooleanRmaxCmd->SetParameterName("Rmax",false);
  BooleanRmaxCmd->SetRange("Rmax>0.");
  BooleanRmaxCmd->SetUnitCategory("Length");

  cmdName = dir+"setPhiStart";
  BooleanSPhiCmd = new G4UIcmdWithADoubleAndUnit(cmdName.c_str(),this);
  BooleanSPhiCmd->SetGuidance("Set start phi angle.");
  BooleanSPhiCmd->SetParameterName("PhiStart",false);
  BooleanSPhiCmd->SetUnitCategory("Angle");

  cmdName = dir+"setDeltaPhi";
  BooleanDPhiCmd = new G4UIcmdWithADoubleAndUnit(cmdName.c_str(),this);
  BooleanDPhiCmd->SetGuidance("Set phi angular span (2PI for full boolean).");
  BooleanDPhiCmd->SetParameterName("DeltaPhi",false);
  BooleanDPhiCmd->SetUnitCategory("Angle");

  cmdName = dir+"setThetaStart";
  BooleanSThetaCmd = new G4UIcmdWithADoubleAndUnit(cmdName.c_str(),this);
  BooleanSThetaCmd->SetGuidance("Set start theta angle.");
  BooleanSThetaCmd->SetParameterName("ThetaStart",false);
  BooleanSThetaCmd->SetUnitCategory("Angle");

  cmdName = dir+"setDeltaTheta";
  BooleanDThetaCmd = new G4UIcmdWithADoubleAndUnit(cmdName.c_str(),this);
  BooleanDThetaCmd->SetGuidance("Set theta angular span (2PI for full boolean).");
  BooleanDThetaCmd->SetParameterName("DeltaTheta",false);
  BooleanDThetaCmd->SetUnitCategory("Angle");

}
//-------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------
GateBooleanMessenger::~GateBooleanMessenger()
{
    delete BooleanRminCmd;
    delete BooleanRmaxCmd;
    delete BooleanSPhiCmd;
    delete BooleanDPhiCmd;
    delete BooleanSThetaCmd;
    delete BooleanDThetaCmd;
}
//-------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------
void GateBooleanMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{ 
  if( command == BooleanRminCmd )
    { 
    GetBooleanCreator()->SetBooleanRmin(BooleanRminCmd->GetNewDoubleValue(newValue)); /*TellGeometryToUpdate();*/}
  else if( command == BooleanRmaxCmd )
    { GetBooleanCreator()->SetBooleanRmax(BooleanRmaxCmd->GetNewDoubleValue(newValue)); /*TellGeometryToUpdate();*/}
  else if( command == BooleanSPhiCmd )
    { GetBooleanCreator()->SetBooleanSPhi(BooleanSPhiCmd->GetNewDoubleValue(newValue)); /*TellGeometryToUpdate();*/}
  else if( command == BooleanDPhiCmd )
    { GetBooleanCreator()->SetBooleanDPhi(BooleanDPhiCmd->GetNewDoubleValue(newValue)); /*TellGeometryToUpdate();*/}
  else if( command == BooleanSThetaCmd )
    { GetBooleanCreator()->SetBooleanSTheta(BooleanSThetaCmd->GetNewDoubleValue(newValue)); /*TellGeometryToUpdate();*/}
  else if( command == BooleanDThetaCmd )
    { GetBooleanCreator()->SetBooleanDTheta(BooleanDThetaCmd->GetNewDoubleValue(newValue)); /*TellGeometryToUpdate();*/}
  else
    GateVolumeMessenger::SetNewValue(command,newValue);
}
//-------------------------------------------------------------------------------------
