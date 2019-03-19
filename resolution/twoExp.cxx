#include "/usr/local/root/include/RooRealVar.h"

using namespace RooFit;

void twoExps(){

  RooRealVar t("t", "t", 0., 6000.);

  //Resolution Gaussian Model
  RooRealVar mean("mean". "mean", 0);
  RooRealVar sigma("sigma", "sigma", 50.);
  RooGaussModel gaussm("gaussm","gaussm", t, mean, sigma);

  //Fast
  RooRealVar 
  
  
  
}
