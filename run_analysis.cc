#include <iostream>
#include <vector>
#include <TVectorD.h>
#include <TROOT.h>

void run_analysis{
  std::vector<Double_t> masses = {0.01, 0.02, 0.04, 0.054, 0.081, 0.15, 0.219, 0.329, 0.5, 0.75, 1, 2, 4, 5, 10}
  int num_iterations = 1;
  TString events = "5000000";
  for (std::size_t m_i = 0; m_i < masses.size(); m_i++) {
    for (int i = 0; i < num_iterations; i++) {
      TString command = TString("./py_sim -p ")+Form["%f",masses[m_i]*2]
      gSystem->Exec(command);
    }
  }
}
