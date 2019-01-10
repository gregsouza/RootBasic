#include "includes.h"

using namespace RooFit;

void tst(const char *fname){
  
  TFile* f = new TFile(fname, "read");
  
  RooDataSet* fdata = (RooDataSet*) gDirectory->Get("exgData");

  RooRealVar x("x", "x", 0., 10.);
  RooDataSet data("data", "dataset", fdata, x);

  RooPlot* frame = x.frame();
  data.plotOn(frame);
  frame->Draw();
}
