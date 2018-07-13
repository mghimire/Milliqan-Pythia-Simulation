Pythia mCP Simulation
=====================
This is a Pythia simulation of the hadronic contribution to mCP estimated by modifying the muon mass in Pythia to stand in for mCP and adjusting the hadron branching ratios to muons. The new branching ratio is estimated by fitting a quadratic to the branching ratios of e, mu, and a zero branching ratio when the mCP mass reaches half the decaying hadron mass. The simulation is of pp collisions at 13 TeV. The main program, `py_sim`, outputs a root file containing a TTree of muon (mCP) pairs from hadron decay along with their pT, pseudorapidity, and other misc vars. The TTree weight is scaled by the cross section (in mb) and total number of events generated.

Configuration
-------------
To configure the simulation pass commandline arguments as follows or edit the [`py_sim.cc`](../master/py_sim.cc) file defaults before compilation. User-configurable options are at the start of `main`.

The following commandline flags are available at runtime.  
`-n`: Number of events to generate. (default: 1000)  
`-m`: mCP mass in GeV. (default: 0.05)  
`-p`: Jet pT cut (pTHat) in GeV. (default: 50)  
`-f`: Output ROOT file name. (default: "out.root")

For example, to manually configure with the default options run the program after compilation with `./py_sim -n 1000 -m 0.05 -p 50 -f out.root`.

Compilation and Running
-----------------------
1. Download and compile [Pythia 8](http://home.thep.lu.se/~torbjorn/Pythia.html). If you have appropriate dependencies, this should be as simple as running `make` in the extracted directory. Alternatively, the code can be run in CernVM in which case you can use a version of Pythia on cvmfs.
2. Modify the [`Makefile`](../master/Makefile) so that `PREFIX_LIB` and `PREFIX_INCLUDE` point to the `/lib` and `/include` directories of your compiled Pythia 8 installation. Modify [`environ.sh`](../master/environ.sh) so that `PYTHIA8DATA` points to the `/share/Pythia8/xmldoc` folder in your compiled Pythia 8 installation. Note there is included commented out code to point these variables to compiled versions that work for a recent CernVM version.
3. Ensure that [ROOT](https://root.cern.ch) is installed and the `root-config` utility is available in the commandline. This can be ensured by sourcing `thisroot.sh` in your ROOT installation's `bin` directory.
4. Compile `py_sim` by running `make all` in the project directory.
5. Run `source environ.sh` to set up the `xmldoc` directory so you can run the compiled binary. The compiled binary should now run as expected with `./py_sim`.

Analysis of Data
----------------
A ROOT macro, [`ptcut.cc`](../master/ptcut.cc), is included to help calculate the number of mCP that pass a pT cut and would be seen by milliQan. To use the macro, run `root -b -q -l 'ptcut.cc(0.0,"out.root")'` with the first argument being the desired mCP pT cut in GeV and the second argument being the `py_sim` root output file you want to run on. The calculation for mCP seen is done assuming mCP charge of 1e-3, milliQan acceptance of 1e-4, and 300 fb^-1 of data. This can be easily changed by modifying the macro.
