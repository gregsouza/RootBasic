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


class my_DoubleExp: public RooAbsAnaConvPdf{

private:
  RooRealProxy _t;
  RooRealProxy _Afast, _Aslow, _Tfast, _Tslow;
    Int_t _basis;
  
public:
  my_DoubleExp(const char *name, const char *title, RooRealVar& t,
	        RooAbsReal& Afast, RooAbsReal& Aslow,
	       RooAbsReal& Tfast, RooAbsReal& Tslow,
	       const RooResolutionModel& model):
    RooAbsAnaConvPdf(name, title, model, t),
    _t("t","time",this, t),
    _Afast("Afast", "Fast", this, Afast),
    _Aslow("Aslow", "Slow Component", this, Aslow),
    _Tfast("Tfast", "Fast Tau", this, Tfast),
    _Tslow("Tslow", "Slow Tau", this, Tslow)
  {
    _basis = declareBasis("(@1/@3)*exp(-@0/@3)+(@2/@4)*exp(-@0/@4)", RooArgList(Afast, Aslow, Tfast, Tslow));
    genContext(t);
  }

  //Copy Constructor
  my_DoubleExp(const my_DoubleExp& other, const char* name):
    RooAbsAnaConvPdf(other, name),
    _t("t","time",this, other._t),
    _Afast("Afast", "Fast", this, other._Afast),
    _Aslow("Aslow", "Slow Component", this, other._Aslow),
    _Tfast("Tfast", "Fast Tau", this, other._Tfast),
    _Tslow("Tslow", "Slow Tau", this, other._Tslow) {}

  //Destructor
  ~my_DoubleExp() {}

  virtual TObject* clone(const char* newname) const{
    return new my_DoubleExp(*this, newname);
  }

  Double_t coefficient(Int_t basisIndex) const{
    switch(basisIndex)
      {
      case 1: return _Afast;
      case 2: return _Aslow;
      case 3: return _Tfast;
      case 4: return _Aslow;
      default: return 0;
      }
    
  }  
};


void genericDecay(){

  //Observable variable
  RooRealVar t("t", "t", 0., 5000.);

  //Gaussian Distribution as resolution model
  RooRealVar mean("mean","mean", 100.);
  RooRealVar sigma("sigma", "gaussian spread", 10.);
  RooGaussModel gaussm("gauss", "guass(t ;mean,sigma)", t, mean, sigma);

  //Generic PDF for analitical convolution
  //basis variables
  RooRealVar Afast("Afast", "Afast", 0.5);
  RooRealVar Aslow("Aslow", "Aslow", 0.5);
  RooRealVar Tfast("Tfast", "Tfast", 120.);
  RooRealVar Tslow("Tslow", "Tslow", 1200.);

  my_DoubleExp cintModel("cint", "cint", t, Afast, Aslow, Tfast, Tslow, gaussm );
  
  RooPlot* frame = t.frame();
  cintModel.plotOn(frame);
  
  frame->Draw();
}




