#include <iostream>
#include "/usr/local/root/include/RooRealVar.h"

using namespace RooFit;

class my_Decay: public RooAbsAnaConvPdf{
protected:
  RooRealProxy _t; //time proxy
  RooRealProxy _tau; //tau proxyx
  int _PlusBasis; //basis code
  int _MinusBasis;
  
public:
  //Constrcutor================================
  my_Decay(const char *name, const char *title,
	   RooRealVar& t, RooAbsReal &tau,
	   const RooResolutionModel& model):
    RooAbsAnaConvPdf(name,title, model, t),
    _t("t","time", this, t),
    _tau("tau", "tau", this, tau)
  {


    _MinusBasis = declareBasis("exp(-@0/@1)", tau);
    _PlusBasis = declareBasis("exp(@0/@1)", tau);
    
  }

  //Copy Constructor ====================================
  my_Decay(const my_Decay& other, const char* name):
    RooAbsAnaConvPdf(other, name),
    _t("t", "time", this, other._t),
    _tau("tau", "tau", this, other._tau ),
    _PlusBasis(other._PlusBasis),
    _MinusBasis(other._MinusBasis) {}
  //Destructor ======================
  ~my_Decay() {}

  //Clone and Coefficient
  virtual TObject* clone(const char* newname) const{
    return new my_Decay(*this, newname);
  }

  Double_t coefficient(Int_t basisIndex) const{
    if(basisIndex == _PlusBasis){
      return 0.5;
    }
    if(basisIndex==_MinusBasis){
      return 0.5;
    }

    return 0;
  }

  // Int_t getGenerator(const RooArgSet& directVars,
  // 		     RooArgSet &generateVars, Bool_t ) const{
  //   if(matchArgs(directVars, generateVars, _t)) return 1;

  //   return 0;
    
  // }
  
};


void my_TwoDecay(){
  //time
  RooRealVar t("t", "t", 0., 100.);

  //gaussian model
  RooRealVar mean("mean", "mean", 10.);
  RooRealVar sigma("sigma", "sigma", 10.);
  RooGaussModel gaussm("gauss", "gauss", t, mean, sigma);
  
  //decay distribution
  RooRealVar tau("tau", "tau", 100.);

  //RooDecay d("Decay", "Decay", t, tau, gaussm, RooDecay::SingleSided);
  my_Decay d("Decay", "My Decay", t, tau, gaussm);

  RooPlot* frame = t.frame();
  d.plotOn(frame);

  frame->Draw();

  
  
}















  
