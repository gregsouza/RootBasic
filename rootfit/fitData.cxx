#include "includes.h"

using namespace RooFit;

void fitData(){
  
  TFile* f = new TFile("convData.root", "read");
  
  RooDataSet* fdata = (RooDataSet*) gDirectory->Get("exgData");

  RooRealVar x("x", "x", 0., 10.);
  RooDataSet data("data", "dataset", fdata, x);
 
  //Model set up


  //Exponential Distribution
  RooRealVar alpha("alpha", "alpha", 0.05, 0., 1.);
  RooRealVar beta("beta", "beta", 5., 0., 10.);
  RooRealVar c1("c1", "c1", 0.5, 0., 1);
  //Distribution Definiton
  RooGenericPdf e("exp", "c1*exp(-alpha*x) + (1-c1)*exp(-beta*x)",
		  RooArgSet(x, c1, alpha, beta));

  
  //Gaussian Distribution
  RooRealVar mean("mean", "mean", 2, 0., 10.);
  RooRealVar sigma("sigma", "gaussian spread", 0.7, 0.5, 3.);
  //Gaussian Distribution
  RooGaussian gauss("gauss", "gauss(x| mean, sigma)", x, mean, sigma);

  //Convolution Model
  x.setBins(10000, "fft");
  RooFFTConvPdf exg("exg", "exp (X) gauss", x, e, gauss);

  exg.fitTo(data);
  
  RooPlot* frame = x.frame();
  data.plotOn(frame);
  exg.plotOn(frame);
  e.plotOn(frame,LineColor(kRed),LineStyle(kDashed));
  gauss.plotOn(frame,LineColor(kGreen),LineStyle(kDashed));
  frame->Draw();

}
