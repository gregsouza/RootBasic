#include "/usr/local/root/include/RooRealVar.h"
#include "/usr/local/root/include/RooPlot.h"

using namespace RooFit;

void genericPdf(){

  RooRealVar x("x","x", 0, 10);
  RooRealVar alpha("alpha", "alpha", 0.5, 0., 1.);
  RooRealVar beta("beta", "beta", 2., 1., 3.);
  RooRealVar c1("c1", "c1", 0.5, 0., 1.);
  RooRealVar c2("c2", "c2", 0.5, 0., 1.);
  RooGenericPdf e("exp", "c1*exp(-alpha*x)+c2*exp(-beta*x)",
		  RooArgSet(x, c1,c2, alpha,beta));

  RooDataSet* data = e.generate(x,1000);
  
  RooPlot* frame = x.frame();
  data->plotOn(frame);
  e.plotOn(frame,LineColor(kRed));
  frame->Draw();
  
  
}
