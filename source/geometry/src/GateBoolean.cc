/*----------------------
  Copyright (C): OpenGATE Collaboration

  This software is distributed under the terms
  of the GNU Lesser General  Public Licence (LGPL)
  See LICENSE.md for further details
  ----------------------*/


#include "GateBoolean.hh"
#include "GateBooleanMessenger.hh"
#include "GateTools.hh"

#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4IntersectionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Colour.hh"
#include "G4UnitsTable.hh"
#include "G4PVPlacement.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"


//-----------------------------------------------------------------------------------------------
// Constructor
GateBoolean::GateBoolean(const G4String& itsName,const G4String& /*itsMaterialName */,
                       G4double itsRmax,
                       G4double itsRmin,
                       G4double itsSPhi, G4double itsDPhi,
                       G4double itsSTheta, G4double itsDTheta,
                       G4bool itsFlagAcceptChildren, G4int depth)
  : GateVVolume(itsName, itsFlagAcceptChildren, depth),
  m_boolean_solid(0),m_boolean_log(0),
  m_booleanRmin(itsRmin),m_booleanRmax(itsRmax),
    m_booleanSPhi(itsSPhi), m_booleanDPhi(itsDPhi),
  m_booleanSTheta(itsSTheta), m_booleanDTheta(itsDTheta),
  m_Messenger(0)
{
  m_Messenger = new GateBooleanMessenger(this);
}
//-----------------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------------
// Constructor
GateBoolean::GateBoolean(const G4String& itsName,
                       G4bool itsFlagAcceptChildren,
                       G4int depth)
  : GateVVolume(itsName, itsFlagAcceptChildren, depth),
    m_boolean_solid(0),m_boolean_log(0),
    m_Messenger(0)
{
  m_booleanRmin = 0. *cm;
  m_booleanRmax = 1. *cm;
  m_booleanSPhi = 0.;
  m_booleanDPhi = twopi;
  m_booleanSTheta = 0.;
  m_booleanDTheta = pi;

  m_Messenger = new GateBooleanMessenger(this);
}
//-----------------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------------
// Destructor
GateBoolean::~GateBoolean()
{
  delete m_Messenger;
}
//-----------------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------------
// Implementation of the pure virtual method ConstructOwnSolidAndLogical() declared by the base-class.
// Construct a new cylinder-solid and its logical volume.
// If flagUpdateOnly is set to 1, the cylinder is updated rather than rebuilt.
G4LogicalVolume* GateBoolean::ConstructOwnSolidAndLogicalVolume(G4Material* mater, G4bool flagUpdateOnly)
{
//   G4UnionSolid:: G4UnionSolid( const G4String& pName,
//                                    G4VSolid* pSolidA ,
//                                    G4VSolid* pSolidB )
//   : G4BooleanSolid(pName,pSolidA,pSolidB)
// {
//   G4ThreeVector pdelta(0.5*kCarTolerance,0.5*kCarTolerance,0.5*kCarTolerance);
//   G4ThreeVector pmin, pmax;
//   BoundingLimits(pmin, pmax);
//   fPMin = pmin - pdelta;
//   fPMax = pmax + pdelta;
// }

  if (!flagUpdateOnly || !m_boolean_solid) {
    // Build mode: build the solid, then the logical volume
    m_boolean_solid
      = new G4UnionSolid(GetSolidName(),
		     m_booleanRmin, m_booleanRmax,
		     m_booleanSPhi, m_booleanDPhi,
		     m_booleanSTheta, m_booleanDTheta);
    m_boolean_log
      = new G4LogicalVolume(m_boolean_solid, mater, GetLogicalVolumeName(), 0, 0, 0);
  }
  else {
    // Update mode: refresh the dimensions of the solid
    m_boolean_solid->SetInsideRadius( m_booleanRmin);
    m_boolean_solid->SetOuterRadius(m_booleanRmax);
    m_boolean_solid->SetStartPhiAngle(m_booleanSPhi);
    m_boolean_solid->SetDeltaPhiAngle(m_booleanDPhi);
    m_boolean_solid->SetStartThetaAngle(m_booleanSTheta);
    m_boolean_solid->SetDeltaThetaAngle(m_booleanDTheta);
  }

  // To visualisation of the boolean
  m_own_visAtt = new G4VisAttributes();
  m_own_visAtt->SetForceAuxEdgeVisible(true);
  m_boolean_log->SetVisAttributes(m_own_visAtt);

  return m_boolean_log;
}
//-----------------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------------
// Implementation of the pure virtual method DestroyOwnSolidAndVolume() declared by the base-class.
// Destroy the solid and logical volume created by ConstructOwnSolidAndLogical()
void GateBoolean::DestroyOwnSolidAndLogicalVolume()
{
  if (m_boolean_log)
    delete m_boolean_log;
  m_boolean_log = 0;

  if (m_boolean_solid)
    delete m_boolean_solid;
  m_boolean_solid = 0;

}
//-----------------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------------
/* Implementation of the virtual method DescribeMyself(), to print-out a description of the creator

   indent: the print-out indentation (cosmetic parameter)
*/
void GateBoolean::DescribeMyself(size_t level)
{
  G4cout << GateTools::Indent(level) << "Shape:              boolean\n";
  G4cout << GateTools::Indent(level) << "Internal radius:    " << G4BestUnit(m_booleanRmin,"Length") << "\n";
  G4cout << GateTools::Indent(level) << "External radius:    " << G4BestUnit(m_booleanRmax,"Length") << "\n";
  G4cout << GateTools::Indent(level) << "Start phi angle:    " << m_booleanSPhi / degree << " deg\n";
  G4cout << GateTools::Indent(level) << "Phi angular span:   " << m_booleanDPhi / degree << " deg\n";
  G4cout << GateTools::Indent(level) << "Start theta angle:  " << m_booleanSTheta / degree << " deg\n";
  G4cout << GateTools::Indent(level) << "Theta angular span: " << m_booleanDTheta / degree << " deg\n";
}
//-----------------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------------
// Overload of the dummy virtual method ComputeMyOwnVolume() defined by the base-class
// Returns the volume of the solid
G4double GateBoolean::ComputeMyOwnVolume() const
{
  return m_boolean_solid->GetCubicVolume();
}
//-----------------------------------------------------------------------------------------------
