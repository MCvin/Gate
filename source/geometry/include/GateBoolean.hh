/*----------------------
  Copyright (C): OpenGATE Collaboration

  This software is distributed under the terms
  of the GNU Lesser General  Public Licence (LGPL)
  See LICENSE.md for further details
  ----------------------*/

#ifndef GateBoolean_h
#define GateBoolean_h 1

#include "globals.hh"
#include "G4Boolean.hh"

#include "GateVVolume.hh"
#include "GateVolumeManager.hh"

class G4VisAttributes;

class GateBooleanMessenger;

/*! \class  GateBoolean
  \brief  The GateBoolean creates a solid and a logical volume for a boolean

  - GateBoolean - by Daniel.Strul@iphe.unil.ch

*/
class GateBoolean  : public GateVVolume
{
public:
  //! Constructor

  GateBoolean(const G4String& itsName,
             G4bool acceptsChildren=true,
             G4int depth=0);


  GateBoolean(const G4String& itsName, const G4String& itsMaterialName,
             G4double itsRmax,
             G4double itsRmin=0.,
             G4double itsSPhi=0., G4double itsDPhi=twopi,
             G4double itsSTheta=0., G4double itsDTheta=pi,
             G4bool acceptsChildren=true,
             G4int depth=0);
  //! Destructor
  virtual ~GateBoolean();

  FCT_FOR_AUTO_CREATOR_VOLUME(GateBoolean)

  //! \name Implementations of pure virtual methods declared by the base-class
  //@{

  //! Implementation of the pure virtual method ConstructOwnSolidAndLogical() declared by the base-class.
  //! Construct a new boolean-solid and its logical volume.
  //! If flagUpdateOnly is set to 1, the boolean is updated rather than rebuilt.
  virtual G4LogicalVolume*   ConstructOwnSolidAndLogicalVolume(G4Material*, G4bool);
  //     virtual G4VPhysicalVolume* ConstructOwnPhysicalVolume();

  //! Implementation of the pure virtual method DestroyOwnSolidAndVolume() declared by the base-class.
  //! Destroy the solid and logical volume created by ConstructOwnSolidAndLogical()
  virtual void DestroyOwnSolidAndLogicalVolume();

  /*! \brief Implementation of the virtual method DescribeMyself(), to print-out
    \brief a description of the creator

    \param indent: the print-out indentation (cosmetic parameter)
  */
  virtual void DescribeMyself(size_t level);

  //! Implementation of the pure virtual method GetHalfDimension() declared by the base-class
  //! Must return the half-size of the boolean along an axis (X=0, Y=1, Z=2)
  //! Returns the radius: accurate only for full booleans
  inline G4double GetHalfDimension(size_t )
  {return m_booleanRmax;};

  //! Overload of the dummy virtual method ComputeMyOwnVolume() defined by the base-class
  //! Returns the volume of the solid
  G4double ComputeMyOwnVolume()  const;

  //@}

  //! \name getters and setters
  //@{

  //! Get the internal diameter
  inline G4double GetBooleanRmin()     {return m_booleanRmin;};
  //! Get the external diameter
  inline G4double GetBooleanRmax()     {return m_booleanRmax;};
  //! Get the start phi angle
  inline G4double GetBooleanSPhi()     {return m_booleanSPhi;};
  //! Get the angular span for the phi angle
  inline G4double GetBooleanDPhi()     {return m_booleanDPhi;};
  //! Get the start theta angle
  inline G4double GetBooleanSTheta()     {return m_booleanSTheta;};
  //! Get the angular span for the theta angle
  inline G4double GetBooleanDTheta()     {return m_booleanDTheta;};

  //! Set the internal diameter
  void SetBooleanRmin  (G4double val)
  {  m_booleanRmin = val; /*ComputeParameters();*/ }
  //! Set the external diameter
  void SetBooleanRmax  (G4double val)
  {  m_booleanRmax = val; /*ComputeParameters();*/ }
  //! Set the start phi angle
  void SetBooleanSPhi  (G4double val)
  {  m_booleanSPhi = val; /*ComputeParameters();*/ }
  //! Set the angular span for the phi angle
  void SetBooleanDPhi  (G4double val)
  {  m_booleanDPhi = val; /*ComputeParameters();*/ }
  //! Set the start theta angle
  void SetBooleanSTheta  (G4double val)
  { m_booleanSTheta = val; /*ComputeParameters();*/ }
  //! Set the angular span for the theta angle
  void SetBooleanDTheta  (G4double val)
  {  m_booleanDTheta = val; /*ComputeParameters();*/ }

  //@}

private:
  //! \name own geometry
  //@{
  G4Boolean*          m_boolean_solid;       	    //!< Solid pointer
  G4LogicalVolume*   m_boolean_log; 	      	    //!< logical volume pointer
  //@}

  //! \name parameters
  //@{
  G4double m_booleanRmin;   	      	      	    //!< internal diameter
  G4double m_booleanRmax;   	      	      	    //!< external diameter
  G4double m_booleanSPhi;   	      	      	    //!< start phi angle
  G4double m_booleanDPhi;   	      	      	    //!< angular span for the phi angle
  G4double m_booleanSTheta;   	      	    //!< start theta angle
  G4double m_booleanDTheta;   	      	    //!< angular span for the theta angle
  //@}

  //! Object visualisation attribute object.
  //! It is passed to the logical volume each time the logical volume is created
  G4VisAttributes *m_own_visAtt;

  //! Messenger
  GateBooleanMessenger* m_Messenger;

};

MAKE_AUTO_CREATOR_VOLUME(boolean,GateBoolean)

#endif
