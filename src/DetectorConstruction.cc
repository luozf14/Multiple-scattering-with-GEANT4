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
/// \file DetectorConstruction.cc
/// \brief Implementation of the B1::DetectorConstruction class

#include "DetectorConstruction.hh"
#include "SiDetectorSD.hh"

#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

namespace B1
{

    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

    DetectorConstruction::DetectorConstruction()
    {
    }

    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

    DetectorConstruction::~DetectorConstruction()
    {
    }

    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

    G4VPhysicalVolume *DetectorConstruction::Construct()
    {
        // Get nist material manager
        G4NistManager *nist = G4NistManager::Instance();

        // Option to switch on/off checking of volumes overlaps
        //
        G4bool checkOverlaps = false;
        G4double inch = 2.54 * cm;
        // G4double atm = 101325. * pascal;
        //
        // World
        //
        G4double world_sizeXY = 100. * cm;
        G4double world_sizeZ = 20. * inch;
        G4Material *world_mat = nist->FindOrBuildMaterial("G4_Galactic");

        G4Box *solidWorld =
            new G4Box("World",                                                    // its name
                      0.5 * world_sizeXY, 0.5 * world_sizeXY, 0.5 * world_sizeZ); // its size

        G4LogicalVolume *logicWorld =
            new G4LogicalVolume(solidWorld, // its solid
                                world_mat,  // its material
                                "World");   // its name

        G4VPhysicalVolume *physWorld =
            new G4PVPlacement(0,               // no rotation
                              G4ThreeVector(), // at (0,0,0)
                              logicWorld,      // its logical volume
                              "World",         // its name
                              0,               // its mother  volume
                              false,           // no boolean operation
                              0,               // copy number
                              checkOverlaps);  // overlaps checking

        //
        // 304 stainless steel
        //
        std::vector<G4String> element304{"Fe", "Cr", "Ni", "Mn"};
        std::vector<G4double> weight304{0.72, 0.18, 0.08, 0.02};
        G4double density304 = 8.0 * g / cm3;
        G4Material *stainlessSteel = nist->ConstructNewMaterial("304StainlessSteel", element304, weight304, density304);
        G4cout << "304StainlessSteel radiation length: " << G4BestUnit(stainlessSteel->GetRadlen(), "Length") << G4endl;

        //
        // beta collimator
        //
        G4double tubCollimatorDz = 0.525 * inch;
        G4double tubCollimatorInRadii = 0.;
        G4double tubCollimatorOutRadii = 5. * cm;
        G4double tubCollimatorSphi = 0. * deg;
        G4double tubCollimatorDphi = 360. * deg;
        G4Tubs *solidCollimatorTemp = new G4Tubs("CollimatorTemp",
                                                 tubCollimatorInRadii,
                                                 tubCollimatorOutRadii,
                                                 0.5 * tubCollimatorDz,
                                                 tubCollimatorSphi,
                                                 tubCollimatorDphi);

        G4double tanTrapezAngle = 2. * cm / (4.5 * inch + 0.525 * inch + 5. * mm);
        G4double trapezDx1 = 4.5 * inch * tanTrapezAngle;
        G4double trapezDx2 = (4.5 * inch + 0.525 * inch) * tanTrapezAngle;
        G4double trapezDy1 = 4.5 * inch * tanTrapezAngle;
        G4double trapezDy2 = (4.5 * inch + 0.525 * inch) * tanTrapezAngle;
        G4double trapezDz = 0.525 * inch;
        G4Trd *trapezTemp = new G4Trd("TrapezTemp",
                                      trapezDx1,
                                      trapezDx2,
                                      trapezDy1,
                                      trapezDy2,
                                      trapezDz);

        G4SubtractionSolid *solidCollimator = new G4SubtractionSolid("Collimator", solidCollimatorTemp, trapezTemp);

        G4LogicalVolume *logicCollimator = new G4LogicalVolume(solidCollimator, // its solid
                                                               stainlessSteel,  // its material
                                                               "Collimator");   // its name

        new G4PVPlacement(0,                                                    // no rotation
                          G4ThreeVector(0, 0, 4.5 * inch + 0.525 * 0.5 * inch), // at position
                          logicCollimator,                                      // its logical volume
                          "Collimator",                                         // its name
                          logicWorld,                                           // its mother  volume
                          false,                                                // no boolean operation
                          0,                                                    // copy number
                          checkOverlaps);                                       // overlaps checking

        //
        // Foil
        //
        G4String foilMaterialString = "304StainlessStel";
        G4Material *foilMaterial = (foilMaterialString == "304StainlessSteel") ? stainlessSteel : nist->FindOrBuildMaterial("G4_Be");

        G4double boxFoilX = 5. * cm;
        G4double boxFoilY = 5. * cm;
        G4double boxFoilZ = (foilMaterialString == "304StainlessSteel") ? 0.0254 * mm : 0.127 * mm;

        G4Box *solidFoil =
            new G4Box("Foil",                                          // its name
                      0.5 * boxFoilX, 0.5 * boxFoilY, 0.5 * boxFoilZ); // its size

        G4LogicalVolume *logicFoil =
            new G4LogicalVolume(solidFoil,    // its solid
                                foilMaterial, // its material
                                "Foil");      // its name

        new G4PVPlacement(0,                                                                    // no rotation
                          G4ThreeVector(0, 0, 4.5 * inch + 0.525 * inch + boxFoilZ * 0.5 * mm), // at position
                          logicFoil,                                                            // its logical volume
                          "Foil",                                                               // its name
                          logicWorld,                                                           // its mother  volume
                          false,                                                                // no boolean operation
                          0,                                                                    // copy number
                          checkOverlaps);                                                       // overlaps checking

        //
        // Dry air
        //
        G4Material *dryAir = nist->BuildMaterialWithNewDensity("dryAir", nist->FindOrBuildMaterial("G4_AIR")->GetName(), nist->FindOrBuildMaterial("G4_AIR")->GetDensity());

        G4double boxDryAirX = 5. * cm;
        G4double boxDryAirY = 5. * cm;
        G4double boxDryAirZ = 5. * mm;

        G4Box *solidDryAir =
            new G4Box("DryAir",                                              // its name
                      0.5 * boxDryAirX, 0.5 * boxDryAirY, 0.5 * boxDryAirZ); // its size

        G4LogicalVolume *logicDryAir =
            new G4LogicalVolume(solidDryAir, // its solid
                                dryAir,      // its material
                                "DryAir");   // its name

        new G4PVPlacement(0,                                                              // no rotation
                          G4ThreeVector(0, 0, 4.5 * inch + 0.525 * inch + 5. * 0.5 * mm), // at position
                          logicDryAir,                                                    // its logical volume
                          "DryAir",                                                       // its name
                          logicWorld,                                                     // its mother  volume
                          false,                                                          // no boolean operation
                          0,                                                              // copy number
                          checkOverlaps);                                                 // overlaps checking

        //
        // Si detector
        //
        G4Material *silicon = nist->FindOrBuildMaterial("G4_Si");

        G4double boxSiDetectorX = 4. * cm;
        G4double boxSiDetectorY = 4. * cm;
        G4double boxSiDetectorZ = 0.13 * mm;

        G4Box *solidSiDetector =
            new G4Box("SiDetector",                                                      // its name
                      0.5 * boxSiDetectorX, 0.5 * boxSiDetectorY, 0.5 * boxSiDetectorZ); // its size

        G4LogicalVolume *logicSiDetector =
            new G4LogicalVolume(solidSiDetector, // its solid
                                silicon,         // its material
                                "SiDetector");   // its name

        new G4PVPlacement(0,                                                                          // no rotation
                          G4ThreeVector(0, 0, 4.5 * inch + 0.525 * inch + 5. * mm + 0.13 * 0.5 * mm), // at position
                          logicSiDetector,                                                            // its logical volume
                          "SiDetector",                                                               // its name
                          logicWorld,                                                                 // its mother  volume
                          false,                                                                      // no boolean operation
                          0,                                                                          // copy number
                          checkOverlaps);                                                             // overlaps checking

        //
        // always return the physical World
        //
        return physWorld;
    }

    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

    void DetectorConstruction::ConstructSDandField()
    {
        // Sensitive detector
        SiDetectorSD* aSiDetectorSD = new SiDetectorSD("/SiDetectorSD","SiDetectorHitsCollection");
        G4SDManager::GetSDMpointer()->AddNewDetector(aSiDetectorSD);
        SetSensitiveDetector("SiDetector", aSiDetectorSD, true);
    }

}
