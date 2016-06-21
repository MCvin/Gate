/*----------------------
  Copyright (C): OpenGATE Collaboration

  This software is distributed under the terms
  of the GNU Lesser General  Public Licence (LGPL)
  See GATE/LICENSE.txt for further details
  ----------------------*/


/*
  \brief Class GateLETActor :
  \brief
*/

// gate
#include "GateLETActor.hh"
#include "GateMiscFunctions.hh"

// g4
#include <G4EmCalculator.hh>
#include <G4VoxelLimits.hh>
#include <G4NistManager.hh>
#include <G4PhysicalConstants.hh>

//-----------------------------------------------------------------------------
GateLETActor::GateLETActor(G4String name, G4int depth):
  GateVImageActor(name,depth) {
  GateDebugMessageInc("Actor",4,"GateLETActor() -- begin\n");


  mIsTrackAverageDEDX=false;
  mIsTrackAverageEdepDX=false;
  mIsDoseAverageDEDX=false;
  mIsDoseAverageEdepDX=false;
  mIsAverageKinEnergy=false;
  
  mIsLETSecondMomentImageEnabled = false;
  mIsLETtoWaterEnabled = false;
  mIsParallelCalculationEnabled = false;
  mAveragingType = "DoseAverage";
  pMessenger = new GateLETActorMessenger(this);
  GateDebugMessageDec("Actor",4,"GateLETActor() -- end\n");
  emcalc = new G4EmCalculator;
}
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/// Destructor
GateLETActor::~GateLETActor()  {
  delete pMessenger;
}
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/// Construct
void GateLETActor::Construct() {
  GateDebugMessageInc("Actor", 4, "GateLETActor -- Construct - begin\n");
  GateVImageActor::Construct();

  // Find G4_WATER. This it needed here because we will used this
  // material for dedx computation for LETtoWater.
  G4NistManager::Instance()->FindOrBuildMaterial("G4_WATER");

  // Enable callbacks
  EnableBeginOfRunAction(true);
  EnableBeginOfEventAction(true);
  EnablePreUserTrackingAction(true);
  EnableUserSteppingAction(true);


  if (mAveragingType == "DoseAveraged" || mAveragingType == "DoseAverage" || mAveragingType == "doseaverage" || mAveragingType == "dose"){mIsDoseAverageDEDX = true;}
  else if (mAveragingType == "DoseAveragedEdep" || mAveragingType == "DoseAverageEdep" ){mIsDoseAverageEdepDX = true;}
  else if (mAveragingType == "TrackAveraged" || mAveragingType == "TrackAverage" || mAveragingType == "Track" || mAveragingType == "track" || mAveragingType == "TrackAveragedDXAveraged"){mIsTrackAverageDEDX = true;}
  else if (mAveragingType == "TrackAveragedEdep" || mAveragingType == "TrackAverageEdep" ){mIsTrackAverageEdepDX = true;}
  else if (mAveragingType == "AverageKinEnergy"){mIsAverageKinEnergy = true;}
  else {GateError("The LET averaging Type" << GetObjectName()
                  << " is not valid ...\n Please select 'DoseAveraged' or 'TrackAveraged')");}

  // Output Filename
  mLETFilename = mSaveFilename;
  if (mIsDoseAverageDEDX)
    {
      mLETFilename= removeExtension(mSaveFilename) + "-doseAveraged."+ getExtension(mSaveFilename);
    }
  else if (mIsTrackAverageDEDX)
    {
      mLETFilename= removeExtension(mSaveFilename) + "-trackAveraged."+ getExtension(mSaveFilename);
    }
  if (mIsLETtoWaterEnabled){
    mLETFilename= removeExtension(mLETFilename) + "-letToWater."+ getExtension(mLETFilename);
  }
  if (mIsParallelCalculationEnabled)
    {
      numeratorFileName= removeExtension(mLETFilename) + "-numerator."+ getExtension(mLETFilename);
      denominatorFileName= removeExtension(mLETFilename) + "-denominator."+ getExtension(mLETFilename);
    }

  // Set origin, transform, flag
  SetOriginTransformAndFlagToImage(mWeightedLETImage);
  SetOriginTransformAndFlagToImage(mNormalizationLETImage);
  SetOriginTransformAndFlagToImage(mDoseTrackAverageLETImage);
  SetOriginTransformAndFlagToImage(mLETSecondMomentImage);
  SetOriginTransformAndFlagToImage(mLETSigmaFinalImage);
  
  SetOriginTransformAndFlagToImage(mLETTempImage);

  // Resize and allocate images
  mWeightedLETImage.SetResolutionAndHalfSize(mResolution, mHalfSize, mPosition);
  mWeightedLETImage.Allocate();
  mNormalizationLETImage.SetResolutionAndHalfSize(mResolution, mHalfSize, mPosition);
  mNormalizationLETImage.Allocate();
  mDoseTrackAverageLETImage.SetResolutionAndHalfSize(mResolution, mHalfSize, mPosition);
  mDoseTrackAverageLETImage.Allocate();

 
  if (mIsLETSecondMomentImageEnabled) {
    mLETSecondMomentImage.SetResolutionAndHalfSize(mResolution, mHalfSize, mPosition);
    mLETSecondMomentImage.Allocate();
    mLETSigmaFinalImage.SetResolutionAndHalfSize(mResolution, mHalfSize, mPosition);
    mLETSigmaFinalImage.Allocate();
  }

  // Warning: for the moment we force to PostStepHitType. This is ok
  // (slightly faster) if voxel sizes are the same between the
  // let-actor and the attached voxelized volume. But wrong if not.
  mStepHitType = PostStepHitType;// RandomStepHitType; // Warning

  // Print information
  GateMessage("Actor", 1,
              "\tLET Actor      = '" << GetObjectName() << Gateendl <<
              "\tLET image      = " << mLETFilename << Gateendl <<
              "\tResolution     = " << mResolution << Gateendl <<
              "\tHalfSize       = " << mHalfSize << Gateendl <<
              "\tPosition       = " << mPosition << Gateendl);

  ResetData();
  GateMessageDec("Actor", 4, "GateLETActor -- Construct - end\n");
}
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/// Save datamDeltaRestricted
void GateLETActor::SaveData() {
  GateVActor::SaveData();


  if (mIsParallelCalculationEnabled) {
    mWeightedLETImage.Write(numeratorFileName);
    mNormalizationLETImage.Write(denominatorFileName);
    if (mIsLETSecondMomentImageEnabled) {
      mLETSecondMomentImage.Write(removeExtension(mLETFilename) + "-variance-unnormalizedSecondMoment."+ getExtension(mLETFilename));
    }
  }
  else
    {
      GateImageDouble::const_iterator iter_LET = mWeightedLETImage.begin();
      GateImageDouble::const_iterator iter_Edep = mNormalizationLETImage.begin();
      GateImageDouble::iterator iter_Final = mDoseTrackAverageLETImage.begin();
      for(iter_LET = mWeightedLETImage.begin(); iter_LET != mWeightedLETImage.end(); iter_LET++) {
        if (*iter_Edep == 0.0) *iter_Final = 0.0; // do not divide by zero
        else *iter_Final = (*iter_LET)/(*iter_Edep);
        iter_Edep++;
        iter_Final++;
      }
      mDoseTrackAverageLETImage.Write(mLETFilename);

      sigmaFilename = removeExtension(mLETFilename) + "-variance."+ getExtension(mLETFilename);
      if (mIsLETSecondMomentImageEnabled)
        {
	  GateImageDouble::const_iterator iter_LET_u = mWeightedLETImage.begin();
	  GateImageDouble::const_iterator iter_LET_secMoment = mLETSecondMomentImage.begin();
	  GateImageDouble::const_iterator iter_Edep_u = mNormalizationLETImage.begin();
	  GateImageDouble::iterator iter_Final_uncert = mLETSigmaFinalImage.begin();
	  for(iter_LET_u = mWeightedLETImage.begin(); iter_LET_u != mWeightedLETImage.end(); iter_LET_u++) {
            if (*iter_Edep_u == 0.0) *iter_Final_uncert = 0.0; // do not divide by zero
            else *iter_Final_uncert = (*iter_LET_secMoment)/(*iter_Edep_u) - (*iter_LET_u)*(*iter_LET_u)/(*iter_Edep_u)/(*iter_Edep_u);
            G4cout<< "X2: " << *iter_LET_secMoment<<G4endl;
            G4cout<< "mu: " << *iter_LET_u<<G4endl;
            
            G4cout<< "N: " << *iter_Edep_u<<G4endl;
            G4cout<< "s2: " << *iter_Final_uncert<<G4endl;
            iter_Edep_u++;
            iter_LET_secMoment++;
            iter_Final_uncert++;
	  }
	  mLETSigmaFinalImage.Write(sigmaFilename);
        }
    }
}
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
void GateLETActor::ResetData() {
  mWeightedLETImage.Fill(0.0);
  mNormalizationLETImage.Fill(0.0);

  if (mIsLETSecondMomentImageEnabled) {
    mLETSecondMomentImage.Fill(0.0);
  }



}
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
void GateLETActor::BeginOfRunAction(const G4Run * r) {
  GateVActor::BeginOfRunAction(r);
  GateDebugMessage("Actor", 3, "GateLETActor -- Begin of Run\n");
  // ResetData(); // Do no reset here !! (when multiple run);
}
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Callback at each event
void GateLETActor::BeginOfEventAction(const G4Event * e) {
  GateVActor::BeginOfEventAction(e);
  mCurrentEvent++;
  GateDebugMessage("Actor", 3, "GateLETActor -- Begin of Event: " << mCurrentEvent << Gateendl);
}
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
void GateLETActor::UserSteppingActionInVoxel(const int index, const G4Step* step) {
  GateDebugMessageInc("Actor", 4, "GateLETActor -- UserSteppingActionInVoxel - begin\n");
  GateDebugMessageInc("Actor", 4, "enedepo = " << step->GetTotalEnergyDeposit() << Gateendl);
  GateDebugMessageInc("Actor", 4, "weight = " <<  step->GetTrack()->GetWeight() << Gateendl);
//	G4cout << "In LET actor: " << step->GetTrack()->GetDefinition()->GetAtomicNumber() << G4endl;

  // Get edep and current particle weight
  const double weight = step->GetTrack()->GetWeight();

  // A.Resch tested calculation method:
  const double edep = step->GetTotalEnergyDeposit()*weight;

  double steplength = step->GetStepLength();

  //if no energy is deposited or energy is deposited outside image => do nothing
  if (edep == 0) {
    GateDebugMessage("Actor", 5, "GateLETActor edep == 0 : do nothing\n");
    return;
  }
  if (index < 0) {
    GateDebugMessage("Actor", 5, "GateLETActor pixel index < 0 : do nothing\n");
    return;
  }

  const G4Material* material = step->GetPreStepPoint()->GetMaterial();//->GetName();
  double energy1 = step->GetPreStepPoint()->GetKineticEnergy();
  double energy2 = step->GetPostStepPoint()->GetKineticEnergy();
  double energy=(energy1+energy2)/2;
  const G4ParticleDefinition* partname = step->GetTrack()->GetDefinition();//->GetParticleName();

  // Compute the dedx for the current particle in the current material
  double weightedLET =0;
  G4double dedx = emcalc->ComputeElectronicDEDX(energy, partname, material);


  double normalizationVal = 0;
  if (mIsDoseAverageDEDX) {
    weightedLET=edep*dedx; // /(density/(g/cm3));
    normalizationVal = edep;
  }
  else if (mIsTrackAverageDEDX) {
    weightedLET=dedx*steplength;
    normalizationVal = steplength;
  }


  else if (mIsTrackAverageEdepDX) {
    weightedLET=edep;
    normalizationVal = steplength;
  }
  else if (mIsDoseAverageEdepDX) {
    weightedLET=edep*edep/steplength;
    normalizationVal = edep;
  }
  else if (mIsAverageKinEnergy) {
	weightedLET=energy*weight;
    normalizationVal = weight;
  }
  
  if (mIsLETtoWaterEnabled){
    weightedLET = (weightedLET/dedx)*	emcalc->ComputeTotalDEDX(energy, partname->GetParticleName(), "G4_WATER") ;
  }

  if (mIsLETSecondMomentImageEnabled) {
    double secondMomentLET = 0;
    if (mIsDoseAverageDEDX){
      secondMomentLET = edep*dedx*dedx;
    }
    else if (mIsTrackAverageDEDX) { secondMomentLET = steplength*dedx*dedx;}
    else if (mIsTrackAverageEdepDX) { secondMomentLET = edep*edep/steplength;}
    else if (mIsDoseAverageEdepDX) { secondMomentLET = edep*edep/steplength;}
    else if (mIsAverageKinEnergy) {secondMomentLET = energy*energy*weight*weight;};
    mLETSecondMomentImage.AddValue(index, secondMomentLET);
  }




    mWeightedLETImage.AddValue(index, weightedLET);
    mNormalizationLETImage.AddValue(index, normalizationVal);


  GateDebugMessageDec("Actor", 4, "GateLETActor -- UserSteppingActionInVoxel -- end\n");
}
//-----------------------------------------------------------------------------
