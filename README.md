# Multiple scattering with GEANT4
A Geant4 program which simulates electron scattering on different materials and finally being detected by a Si-strip detector. The code is based on Geant4' s example ``basic/B1``. Detailed description can be found in ProblemDescription.pdf.

## Prerequisites
- Geant4 v11 (v10 does not work!)
- C++17
- CMake>=3.16

## How to use
### Compile
Make a build directory and enter it.
```
$ mkdir build && cd build
```

cmake & make 
```
$ cmake ../ && make
```

### Configuration
This program takes JSON file as configuration (it is mandatory!). The example config file is ``config/config.json``. Here below shows the content of this JSON file.
```
{
    "GUI": true,
    "RunMac": "run1.mac",
    "Threads":10,
    "Foil": 2,
    "BeamEnergy": 0.5
}
```
- GUI: false - batch mode, true - interactive mode.
- RunMac: The .mac file defines how many particles you want to fire. Only valid for batch mode.
- Threads: 0 - serial mode, N - multithread mode with N threads (N>0).
- Foil: 0 - no foil, 1 - 304 stainless steel, 2 - Be.
- BeamEnergy: any non negative value.

### Run
```
$ ./exampleB1 ../config/config.json
```

### Analysis
The output data file is ``Data.root``. A simple analysis ROOT macro is provided as ``plotHisto.C``. 

To change the foil for vacuum, one needs to modify ``src/DetectorConstruction.cc``.
