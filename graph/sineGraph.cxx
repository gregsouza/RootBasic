#include <iostream>
#include <cmath>

void sineGraph(){
  
  TGraph* gr = new TGraph();
  int n = 20;
  double dx=0.2, x0=0;
  double y(0), x(0);
  
  for(int i=0; i<n; i++){
    x = x0+i*dx;
    y = std::sin(x);

    gr->SetPoint(i, x,y);
    
  }

  gr->Draw("AC*");

}
