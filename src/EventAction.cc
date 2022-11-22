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
/// \file EventAction.cc
/// \brief Implementation of the B1::EventAction class

#include "EventAction.hh"
#include "RunAction.hh"
#include "HistoManager.hh"
#include "SiDetectorHit.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"

namespace B1
{

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  EventAction::EventAction(RunAction *runAction)
      : G4UserEventAction(), fRunAction(runAction)
  {
  }

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  EventAction::~EventAction()
  {
  }

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  void EventAction::BeginOfEventAction(const G4Event *)
  {
    G4SDManager *sdManager = G4SDManager::GetSDMpointer();
    fHCID_SiDetector = sdManager->GetCollectionID("SiDetectorHitsCollection");
  }

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  void EventAction::EndOfEventAction(const G4Event *aEvent)
  {
    fHistoManager = fRunAction->GetHistoManager();

    G4HCofThisEvent *hce = aEvent->GetHCofThisEvent();
    if (!hce)
    {
      G4ExceptionDescription msg;
      msg << "No hits collection of this event found.\n";
      return;
    }

    SiDetectorHitsCollection *hcSiDetector = (SiDetectorHitsCollection *)aEvent->GetHCofThisEvent()->GetHC(fHCID_SiDetector);
    G4int nofHitsSiDetector = hcSiDetector->GetSize();
    if (nofHitsSiDetector == 0)
    {
      G4ExceptionDescription msg;
      msg << "No hits in Si detector of this event found.\n";
      return;
    }

    G4ThreeVector averagePosition(0., 0., 0.);
    G4double eDep = 0.;
    for (int i = 0; i < nofHitsSiDetector; i++)
    {
      eDep += (*hcSiDetector)[i]->GetEnergyDeposit();
      averagePosition += (*hcSiDetector)[i]->GetPosition();
    }
    averagePosition = (nofHitsSiDetector > 0) ? averagePosition / nofHitsSiDetector : G4ThreeVector(0., 0., 0.);
    G4int trackID = (*hcSiDetector)[0]->GetTrackID();
    G4String particleName = (*hcSiDetector)[0]->GetParticleName();
    G4double kineticEnergy = (*hcSiDetector)[0]->GetKineticEnergy();
    G4ThreeVector incidentMomentum = (*hcSiDetector)[0]->GetMomentum();
    fHistoManager->FillHisto(0, eDep / keV, 1.0);
    fHistoManager->FillHisto(1, averagePosition.x(), 1.0);
    fHistoManager->FillHisto(2, averagePosition.y(), 1.0);
    fHistoManager->FillNtuple(trackID, particleName, eDep / keV, kineticEnergy, incidentMomentum, averagePosition);
  }

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}
