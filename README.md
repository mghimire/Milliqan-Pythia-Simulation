Pythia mCP Simulation
=====================
This is a Pythia simulation of the hadronic contribution to mCP estimated by modifying the muon mass in Pythia to stand in for mCP and adjusting the hadron branching ratios to muons. The new branching ratio is estimated by fitting a quadratic to the branching ratios of e, mu, and a zero branching ratio when the mCP mass reaches half the decaying hadron mass. Further, the pi0 branching ratio to e+e- is modified with a linear approximation and the e from pi0 are also chosen to stand in for mCP. The simulation is of pp collisions at 13 TeV, but can be modified to different collision energies. The main program, `py_sim`, outputs a root file containing a TTree of mCP pairs from hadron decay along with their pT, pseudorapidity, and other miscellaneous variables. The TTree weight is scaled by the cross section (in mb) and total event weight. Each event has a weight stored in the `weight` variable.

Setup and Compilation
---------------------
1. If running on CernVM or lxplus skip to step 2. Otherwise, download and compile [Pythia 8](http://home.thep.lu.se/~torbjorn/Pythia.html). If you have appropriate dependencies, this should be as simple as running `make` in the extracted directory.
2. Clone the git repository to a local working directory and modify the [`Makefile`](../master/Makefile) so that `PREFIX_LIB` and `PREFIX_INCLUDE` point to the `/lib` and `/include` directories of your compiled Pythia 8 installation. Modify [`environ.sh`](../master/environ.sh) so that `PYTHIA8DATA` points to the `/share/Pythia8/xmldoc` folder in your compiled Pythia 8 installation. Note there is included commented out code to point these variables to compiled cvmfs versions that work for CernVM and lxplus. To compile on lxplus or an slc6 based version of CernVM you will also need to set an appropriate gcc version by running `source /cvmfs/sft.cern.ch/lcg/external/gcc/4.8.4/x86_64-slc6-gcc48-opt/setup.sh`
3. Ensure that [ROOT](https://root.cern.ch) is installed and the `root-config` utility is available in the commandline. If not, this can be ensured by sourcing `thisroot.sh` in your ROOT installation's `bin` directory.
4. Compile `py_sim` by running `make all` in the project directory.
5. Run `source environ.sh` to set up the `xmldoc` directory so you can run the compiled binary. The compiled binary should now run as expected with `./py_sim`.

Configuration and Running
-------------------------
To run the simulation, pass commandline arguments as follows, or edit the [`py_sim.cc`](../master/py_sim.cc) file defaults before compilation. User-configurable options are at the start of `main`.

The following commandline flags are available at runtime.  
`-n NUM_EVENTS`: Number of events to generate. (default: 100)  
`-m MASS`: mCP mass in GeV. (default: 0.05)  
`-p PT_CUT`: QCD Jet pT cut (pTHat) in GeV. (default: 4 or twice the mCP mass, whichever is greater)  
`-t TYPE`: Type of process to simulate (0 for QCD, 1 for Onia, and 2 for gamma*/Z). (default: 0)  
`-f FILENAME`: Output ROOT file name. (default: "out.root")  
`-v`: Be verbose and output found mCP information

For example, to manually configure with the default options run the program after compilation with `./py_sim -n 100 -m 0.05 -p 4 -t 0 -f out.root`.

There is a bash script, [`run_analysis.sh`](../master/run_analysis.sh), that loops `py_sim` over a range of masses from 0.01 to 200 GeV on a logarithmic (base 10) scale. It also sets the number of events based on the mass it is running in order to maintain a maximum of 10% error. It saves the simulation TTree files into `./data*` directories and Pythia log files into a `./pythialogfiles` directory.

Analysis of Data
----------------
A ROOT macro, [`ptcut.cc`](../master/ptcut.cc), is included to help calculate the number of mCP that pass a pT cut as determined by the charge (due to magnetic bending and rock damping effects) and would be seen by milliQan. To use the macro, run `root -b -q -l 'ptcut.cc(0.001,{"out.root"})'` with the first argument being the desired mCP charge in units of e and the second argument being a comma separated list of the `py_sim` root output files you want to run on. The macro will combine all the listed files for calculation. The calculation for mCP seen is done assuming 300 fb^-1 of data. This can be easily changed by modifying the macro. There is also an option accepted as a third argument (boolean that defaults to 0) that creates a set of histograms of the mother ID, pT, pTHat, and eta of the mCPs that pass the pT cut.

There is also a bash script, [`show_hadron_BRs.sh`](../master/show_hadron_BRs.sh), which reads the Pythia log files and outputs the Branching Ratios of the Z and different hadrons into the mCP based on its mass.

Generating Plots
----------------
Included are also plotting ROOT macros [`plotgraph.cc`](../master/plotgraph.cc), [`filleff.cc`](../master/filleff.cc), and [`limitplot.cc`](../master/limitplot.cc).

[`plotgraph.cc`](../master/plotgraph.cc) is called as `root -b -q -l 'plotgraph.cc("gammaZ")'` with the input string representing the type of process (qcd, onia, or gammaZ) that we want to plot. It plots heatgraphs of the number of mCP seen vs mass vs charge as well as the corresponding uncertainty. This is made
to work in conjunction with [`run_analysis.sh`](../master/run_analysis.sh).

[`filleff.cc`](../master/filleff.cc) is called as `root -b -q -l 'filleff.cc(1)'` with the input integer representing the number of layers in the milliQan detector (defaults to 1). It plots the detection efficiency of the milliQan detector vs mass vs charge. While the efficiency is independent of mass, we include it for the sake of ease in manipulating the histograms.

[`limitplot.cc`](../master/limitplot.cc) is called as `root -b -q -l 'limitplot.cc(0,0,"")'` with the inputs representing QCD on or off, the type of background, and an extra string that identifies the input files, respectively. It combines the heatplots, multiplies by the efficiency plot, and produces a contour plot of 3 sigma limits on the detection of mCP vs mass vs charge. It also outputs the final healtplot that was used to generate the contour plot.

All three of these are made to work in conjunction with [`plotgraphs.sh`](../master/run_analysis.sh), which calls upon all three macros sequentially and generates all the aforementioned plots. All generated plots are saved in a `./plots` directory.

Summary
-------
A full run of this simulation is done via installing Pythia and ROOT, compiling [`py_sim.cc`](../master/py_sim.cc), and running [`run_analysis.sh`](../master/run_analysis.sh). The plots are then generated by running [`plotgraphs.sh`](../master/run_analysis.sh).

Documentation
-------------
[`Overleaf`](https://www.overleaf.com/read/mwxgnfbhmjyb)
