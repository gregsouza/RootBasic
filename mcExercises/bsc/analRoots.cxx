#include <iostream>
#include <cmath>



void logPlot(){

  char filename[]="polk5.root";
  char treeName[]="T";
  char xbranch[]="tries";
  char ybranch[]="error";
  
  TFile* f = new TFile(filename, "READ");
  TTree* T = (TTree*) f->Get(treeName);

  auto x(0);
  double y(0);

  T->SetBranchAddress(xbranch, &x);
  T->SetBranchAddress(ybranch, &y);

  TCanvas* c = new TCanvas("c", "canvas");
  TGraph* gr = new TGraph();
  
  int64_t nentries = T->GetEntries();
  for(int i=0; i<nentries; i++){
    T->GetEntry(i);
    gr->SetPoint(i,(double) x,(double) y);
  }

  c->SetLogx();
  c->SetLogy();

  TF1 *func = new TF1("func", "[0]*x**[1]");
  
  gr->Draw("A*");
  gr->Fit(func, "L");
  func->Draw("SAME");

}

void analRoots(){
  logPlot();
}
