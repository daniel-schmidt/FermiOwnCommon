# FermiOwnCommon
﻿Common classes and definitions to be used in projects to simulate FourFermion theories.
Provides basic classes for a lattice, fields on the lattice and building blocks for a Metropolis algorithm.
Strongly depends on the eigen3 library for linear algebra operations and uses these vectors for basic data storage.
A simple Ising model Hamiltonian is included for testing.
Tests are provided if sensible...

## building in console: 

* export CPLUS_INCLUDE_PATH="/path/to/eigen3"
* mkdir build
* cd build
* cmake ../
* make

switch build types with cmake -DCMAKE_BUILD_TYPE=Debug or -DCMAKE_BUILD_TYPE=Release

