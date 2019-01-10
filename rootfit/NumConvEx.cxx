#include "/usr/local/root/include/RooPlot.h"
#include "/usr/local/root/include/RooRealVar.h"
#include "/usr/local/root/include/RooGenericPdf.h"
#include "/usr/local/root/include/RooGaussian.h"
#include "/usr/local/root/include/RooFFTConvPdf.h"

using namespace RooFit;



void NumConvEx(){

  //Sample Variable
  RooRealVar x("x", "x",0., 10.);

  //Double Exponential Distribution
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

  //FFT convolution of PDFs
  x.setBins( 10000, "fft");

  RooNumConvPdf exg("exg", "exp (x) gauss", x, e, gauss);

  //RooDataSet* data = exg.generate(x,1000);
  
  RooPlot* frame = x.frame();
  //data->plotOn(frame);
  e.plotOn(frame,LineColor(kBlue), LineStyle(kDashed));
  gauss.plotOn(frame, LineColor(kBlack), LineStyle(kDashed));
  exg.plotOn(frame, LineColor(kRed));
  
  frame->Draw();
  
}
