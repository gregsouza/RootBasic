/*

 Toy Model for the Data in the ARAPUCA test in LAr
 This is a Root Macro with:
 GenerateDataFFT: Write Data in a .root file using FFT Convolution for the model
 FitDataFFT: Fit Data with a model using FFTFit convolution


 */

#include "/usr/local/root/include/RooPlot.h"
#include "/usr/local/root/include/RooRealVar.h"
#include "/usr/local/root/include/RooGenericPdf.h"
#include "/usr/local/root/include/RooGaussian.h"
#include "/usr/local/root/include/RooFFTConvPdf.h"
#include "/usr/local/root/include/RooDataSet.h"
#include "/usr/local/root/include/TFile.h"


using namespace RooFit;



void GenerateDataFFT(){

  //Sample Variable
  RooRealVar x("x", "x",0., 4000.);

  //Double Exponential Distribution
  RooRealVar Aratio("Aratio", "Ratio of Afast/Aslow", 1.5, 0.1, 4.);
  RooRealVar Tfast("Tfast", "Fast Componente Emission Time", 1., 250., 400);
  RooRealVar Tslow("Tslow", "Slow Componente Emission Time", 1300.,1000., 2000.);
  //Distribution Definiton PDF = Norm(Aratio/tfast*exp(-t/tfast) + e(-t/tslow) / tslow)
    
  RooGenericPdf e("exp", "(Aratio/Tfast)*exp(-x/Tfast) + (1/Tslow)*exp(-x/Tslow)",
		  RooArgSet(x, Aratio, Tfast, Tslow));

  //Gaussian Distribution
  RooRealVar mean("mean", "mean", 500., 100., 3000.);
  RooRealVar sigma("sigma", "gaussian spread", .200, 100., 3000.);
  //Gaussian Distribution
  RooGaussian gauss("gauss", "gauss(x| mean, sigma)", x, mean, sigma);

  //FFT convolution of PDFs
  x.setBins( 1000000, "fft");

  RooFFTConvPdf exg("exg", "exp (x) gauss", x, e, gauss);

  TFile* f = new TFile("convData.root", "recreate");
  RooDataSet* data = exg.generate(x,10000);
  data->Write();
  
  //RooPlot* frame = x.frame();
  //data->plotOn(frame);
  //frame->Draw();
}

void FitDataFFT(){
  
  TFile* f = new TFile("convData.root", "read");
  
  RooDataSet* fdata = (RooDataSet*) gDirectory->Get("exgData");

  RooRealVar x("x", "x", 0., 4000.);
  RooDataSet data("data", "dataset", fdata, x);
 
  //Model set up


  //Double Exponential Distribution
  RooRealVar Aratio("Aratio", "Ratio of Afast/Aslow", 1.5, 0.1, 4.);
  RooRealVar Tfast("Tfast", "Fast Componente Emission Time", 1., 250., 400);
  RooRealVar Tslow("Tslow", "Slow Componente Emission Time", 1300.,1000., 2000.);
  //Distribution Definiton PDF = Norm(Aratio/tfast*exp(-t/tfast) + e(-t/tslow) / tslow)
    
  RooGenericPdf e("exp", "(Aratio/Tfast)*exp(-x/Tfast) + (1/Tslow)*exp(-x/Tslow)",
		  RooArgSet(x, Aratio, Tfast, Tslow));

  //Gaussian Distribution
  RooRealVar mean("mean", "mean", 500., 100., 3000.);
  RooRealVar sigma("sigma", "gaussian spread", .200, 100., 3000.);
  //Gaussian Distribution
  RooGaussian gauss("gauss", "gauss(x| mean, sigma)", x, mean, sigma);

  //FFT convolution of PDFs
  x.setBins( 1000000, "fft");

  RooFFTConvPdf exg("exg", "exp (x) gauss", x, e, gauss);

  exg.fitTo(data);
  
  RooPlot* frame = x.frame();
  data.plotOn(frame);
  exg.plotOn(frame);
  e.plotOn(frame,LineColor(kRed),LineStyle(kDashed));
  gauss.plotOn(frame,LineColor(kGreen),LineStyle(kDashed));
  frame->Draw();

}
