# Multiple scattering with GEANT4
A Geant4 program which simulates electron scattering on different materials and finally being detected by a Si-strip detector. The code is based on Geant4' s example ``basic/B1``.

## Description
Detailed description can be found in ProblemDescription.pdf.

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

### Run
- GUI mode

```
$ ./exampleB1
```
- Shell
  
```
$ ./example run1.mac
```

### Analysis
The output data file is ``Data.root``. A simple analysis ROOT macro is provided as ``plotHisto.C``. 

To change the foil for vacuum, one needs to modify ``src/DetectorConstruction.cc``.