#include <iostream>
using std::cin;
#include <fstream>
using std::ifstream;
#include <iterator>
using std::istream_iterator;
#include <algorithm>
using std::copy;
#include <TMath.h>
#include <TROOT.h>
#include <TTree.h>
#include <TVectorD.h>
#include <iomanip>
#include <sstream>
#include <vector>
#include "ptcut.cc"

void makehists() {
  // load up the filenames of files in data/ to a vector of TStrings
  std::vector<TString> filenames;
  ifstream myfile("filenames.txt"); 

  copy(istream_iterator<TString>(myfile),
       istream_iterator<TString>(),
       back_inserter(filenames));

  //create a vector of masses being used for future reference
  std::vector<Double_t> masses;
  for (std::size_t i = 0; i < filenames.size(); i++) {
    if (filenames[i].EndsWith("_1.root")) masses.push_back(filenames[i].Atof());
  }

  //create a vector of vectors per mass of filenames
  std::vector<vector<TString>> files;
  int f_i = 0;
  do {
      std::vector<TString> massfiles;
      TString check = filenames[f_i](0,filenames[f_i].First("_"));
      do {
          massfiles.push_back("data/" + filenames[f_i]);
          f_i++;
      } while (filenames[f_i].BeginsWith(check));
      files.push_back(massfiles);
  } while (f_i < filenames.size());

  // vector of charges we will plot
  std::vector<Double_t> charges; 
//  charges = {0.001, 0.005, 0.01, 0.05, 0.1, 0.3};
  for (int i = 0; i < 11; i++) {
    charges.push_back(0.0001*TMath::Power(10, (4*i / 10.0)));
  }
  for (std::size_t q_i = 0; q_i < charges.size(); q_i++) {
    // run analysis on each mass size with given charge
    Double_t q = charges[q_i];
    for (std::size_t i = 0; i < files.size(); i++) {
      ptcut(q, files[i], 1);
    }
  }
}
