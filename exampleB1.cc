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
/// \file exampleB1.cc
/// \brief Main program of the B1 example

#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"
#include "nlohmann/json.hpp"

#include "G4RunManagerFactory.hh"
#include "G4SteppingVerbose.hh"
#include "G4UImanager.hh"
#include "QBBC.hh"
#include "ShieldingLEND.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "Randomize.hh"

#include <fstream>
#include <iostream>

using namespace B1;
using json = nlohmann::json;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc, char **argv)
{
    // Detect interactive mode (if no arguments) and define UI session
    //
    G4UIExecutive *ui = nullptr;
    // if (argc == 1)
    // {
    //     ui = new G4UIExecutive(argc, argv);
    // }
    if (argc != 2)
    {
        G4cerr << "\n---> Usage: ./exampleB1 <config.json>" << G4endl;
        return 0;
    }

    // Parse the configure JSON file
    //
    std::string configFile = argv[1];
    std::ifstream configStream(configFile.c_str());
    json config = json::parse(configStream);
    G4int useGUI = config["GUI"];
    G4bool isInteractive = !!useGUI;
    std::string runMac = config["RunMac"];
    G4int foilID = config["Foil"];
    G4int nThreads = config["Threads"];

    // Optionally: choose a different Random engine...
    G4Random::setTheEngine(new CLHEP::MTwistEngine);
    // G4Random::setTheEngine(new CLHEP::RanecuEngine);
    // set random seed with system time
    G4long seed = time(NULL);
    CLHEP::HepRandom::setTheSeed(seed);

    // use G4SteppingVerboseWithUnits
    G4int precision = 4;
    G4SteppingVerbose::UseBestUnit(precision);

    // Construct the default run manager
    //
    G4RunManager* runManager=nullptr;
    if(nThreads==0)
    {
        runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Serial, nThreads);
    }
    else
    {
        runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::MT, nThreads);
    }
    
    // Set mandatory initialization classes
    //
    // Detector construction
    DetectorConstruction *detector = new DetectorConstruction();
    detector->SetFoilID(foilID);
    runManager->SetUserInitialization(detector);

    // Physics list
    G4VModularPhysicsList *physicsList = new QBBC;
    // G4VModularPhysicsList *physicsList = new ShieldingLEND;
    physicsList->SetVerboseLevel(1);
    runManager->SetUserInitialization(physicsList);

    // User action initialization
    runManager->SetUserInitialization(new ActionInitialization());

    // Initialize visualization
    //
    G4VisManager *visManager = new G4VisExecutive;
    // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
    // G4VisManager* visManager = new G4VisExecutive("Quiet");
    visManager->Initialize();

    // Get the pointer to the User Interface manager
    G4UImanager *UImanager = G4UImanager::GetUIpointer();

    // Process macro or start UI session
    //
    if (!isInteractive)
    {
        // batch mode
        G4String command = "/control/execute ";
        G4String fileName = runMac;
        UImanager->ApplyCommand(command + fileName);
    }
    else
    {
        // interactive mode
        ui = new G4UIExecutive(argc, argv);
        UImanager->ApplyCommand("/control/execute init_vis.mac");
        ui->SessionStart();
        delete ui;
    }

    // Job termination
    // Free the store: user actions, physics_list and detector_description are
    // owned and deleted by the run manager, so they should not be deleted
    // in the main() program !

    delete visManager;
    delete runManager;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
