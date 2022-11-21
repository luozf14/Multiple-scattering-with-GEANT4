//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file SiDetectorHit.hh
/// \brief Definition of the B1::SiDetectorHit class

#ifndef B1SiDetectorHit_h
#define B1SiDetectorHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "tls.hh"

namespace B1
{

  /// SiDetector hit class
  ///
  /// It defines data members to store the trackID, energy deposit, kinetic energy, momentum,
  /// and position of charged particles in a selected volume:
  /// - fTrackID, fEnergyDeposit, fKineticEnergy, fMomentum, fPosition

  class SiDetectorHit : public G4VHit
  {
  public:
    SiDetectorHit();
    SiDetectorHit(const SiDetectorHit &) = default;
    ~SiDetectorHit() override;

    // operators
    SiDetectorHit &operator=(const SiDetectorHit &) = default;
    G4bool operator==(const SiDetectorHit &) const;

    inline void *operator new(size_t);
    inline void operator delete(void *);

    // methods from base class
    void Print() override;

    // Set methods
    void SetTrackID(G4int track) { fTrackID = track; };
    void SetParticleName(G4String name) { fParticleName = name; };
    void SetEnergyDeposit(G4double de) { fEnergyDeposit = de; };
    void SetKineticEnergy(G4double ke) { fKineticEnergy = ke; };
    void SetMomentum(G4ThreeVector xyz) { fMomentum = xyz; };
    void SetPosition(G4ThreeVector xyz) { fPosition = xyz; };

    // Get methods
    G4int GetTrackID() const { return fTrackID; };
    G4String GetParticleName() const { return fParticleName; };
    G4double GetEnergyDeposit() const { return fEnergyDeposit; };
    G4double GetKineticEnergy() const { return fKineticEnergy; };
    G4ThreeVector GetMomentum() const { return fMomentum; };
    G4ThreeVector GetPosition() const { return fPosition; };

  private:
    G4int fTrackID = -1;
    G4String fParticleName;
    G4double fEnergyDeposit = 0.;
    G4double fKineticEnergy = 0.;
    G4ThreeVector fMomentum;
    G4ThreeVector fPosition;

  };

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  typedef G4THitsCollection<SiDetectorHit> SiDetectorHitsCollection;

  extern G4ThreadLocal G4Allocator<SiDetectorHit> *SiDetectorHitAllocator;

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  inline void *SiDetectorHit::operator new(size_t)
  {
    if (!SiDetectorHitAllocator)
      SiDetectorHitAllocator = new G4Allocator<SiDetectorHit>;
    return (void *)SiDetectorHitAllocator->MallocSingle();
  }

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  inline void SiDetectorHit::operator delete(void *hit)
  {
    SiDetectorHitAllocator->FreeSingle((SiDetectorHit *)hit);
  }

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}

#endif
