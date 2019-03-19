#include "/usr/local/root/include/RooRealVar.h"


using namespace RooFit;

void decayResolution(){

  //Observable Variable
  RooRealVar t("t", "t",0.,5000.);

  //Gaussian Resolution Model
  RooRealVar mean("mean", "mean", 100.);
  RooRealVar sigma("sigma","sigma", 5.);
  RooGaussModel gaussm("gaussm", "gauss model",t,mean,sigma);

  //RooDecay with gaussian resoltion
  RooRealVar tau("tau", "lifetime", 1200.);
  RooDecay model("model", "decay (x) gauss", t, tau, gaussm, RooDecay::DoubleSided);

  RooPlot* frame = t.frame();
  model.plotOn(frame);

  frame->Draw();
  
}


void regularDecay(){

  //Observable variable
  RooRealVar t("t","t", 0., 5000.);

  
  //exp pdf
  RooRealVar c("c","c", -1/50.0);
  RooExponential exp("exp", "exp", t, c);

  //Gaussian Distribution
  RooRealVar mean("mean", "mean", 100.);
  RooRealVar sigma("sigma", "gaussian spread", .50);
  //Gaussian Distribution
  RooGaussian gauss("gauss", "gauss(x| mean, sigma)", t, mean, sigma);

  t.setBins(1000000, "fft");

  RooFFTConvPdf exg("exg", "exg", t, exp, gauss);
  
  RooPlot* frame = t.frame();
  // exp.plotOn(frame);
  exg.plotOn(frame);

  

  frame->Draw();
  
}


class ArgonCint: public RooAbsAnaConvPdf;
//See RooDecay implementation
//Implemet Afast/Tfast exp(fast) + Aslow/Tslow exp(slow)
//Create RooArapuca.h

void genericDecay(){

  //Observable variable
  RooRealVar t("t", "t", 0., 5000.);

  //Gaussian Distribution as resolution model
  RooRealVar mean("mean","mean", 100.);
  RooRealVar sigma("sigma", "gaussian spread", 10.);
  RooGaussian gaussm("gauss", "guass(t ;mean,sigma)", t, mean, sigma);

  //Generic PDF for analitical convolution
  //basis variables
  RooRealVar Afast("Afast", "Afast", 0.5);
  RooRealVar Aslow("Aslow", "Aslow", 0.5);
  RooRealVar Tfast("Tfast", "Tfast", 120.);
  RooRealVar Tslow("Tslow", "Tslow", 1200.);
  
  //RooAbsAnaConvPdf cintModel("model", "model",
		   gaussm, t);

  //cintModel.declareBasis("Afast*exp(-t/Tfast) + Aslow*exp(-t/Tslow)",
  //	 RooArgSet(t, Afast,Tfast, Aslow, Tslow));


  
    
  
  RooPlot* frame = t.frame();
  cintModel.plotOn(frame);
  
  frame->Draw();
}




