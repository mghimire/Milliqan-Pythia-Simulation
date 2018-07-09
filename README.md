Pythia mCP Simulation
=====================
This is a Pythia simulation of the hadronic contribution to mCP estimated by modifying the muon mass in Pythia to stand in for mCP and adjusting the hadron branching ratios to muons. The new branching ratio is estimated by fitting a quadratic to the branching ratios of e, mu, and a zero branching ratio when the mCP mass reaches half the decaying hadron mass. The main program, `py_sim`, outputs a ROOT TTree of muons pairs from hadron decay along with their pT.

Configuration
-------------
To configure the simulation edit the [`py_sim.cc`](../master/py_sim.cc) file before compilation. User-configurable options are at the start of `int main()` and include the number of events, the mCP mass, the pT cut on jets produced, and the name of the output ROOT file.

Compilation and Running
-----------------------
1. Download and compile [Pythia 8](http://home.thep.lu.se/~torbjorn/Pythia.html). If you have appropriate dependencies, this should be as simple as running `make` in the extracted directory. Alternatively, the code can be run in CernVM in which case you can use a version of Pythia on cvmfs.
2. Modify the [`Makefile`](../master/Makefile) so that the `PREFIX_LIB` and `PREFIX_INCLUDE` point to your `/lib` and `/include` directories of your compiled Pythia 8 installation. Modify [`environ.sh`](../master/environ.sh) so that `PYTHIA8DATA` points to the `/share/Pythia8/xmldoc` folder in your compiled Pythia 8 installation. Note there is included commented out code to point these variables to compiled versions that work for a recent CernVM version.
3. Ensure that ROOT is installed and the `root-config` utility is available in the commandline. This can be ensured by sourcing `thisroot.sh` in your ROOT installation's bin directory.
4. Compile `py_sim` by running `make all` in the project directory.
5. Run `source environ.sh` to set up the `xmldoc` directory so you can run the compiled binary. The compiled binary should now run as expected with `./py_sim`.
