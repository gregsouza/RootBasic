#include <iostream>
#include <cmath>

void expGraph(){

  TCanvas* c = new TCanvas("c", "canvas");

  c->SetLogy();
  c->SetLogx();
  
  TGraph* gr = new TGraph();
  
  int n(20);
  double dx=0.2, x0=1;
  double x(0), y(0);

  for(int i=0; i<n; i++){
    x=x0*std::exp(i*dx);
    y = std::exp(x);

    gr->SetPoint(i,x,y);
  }
  
  gr->Draw("AC*");
  
}
