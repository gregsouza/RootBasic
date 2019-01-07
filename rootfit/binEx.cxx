#include "/usr/local/root/include/TFile.h"
#include "/usr/local/root/include/TH1F.h"

void createHist(){
  TFile* f =  new TFile("data.root", "recreate");
  TH1F* hh = new TH1F("aHist", "This is a Hist!", 200, -10,10);

  TRandom3 random;
  Double_t r;
  for(int i=0; i<10000; i++){
    r=random.Gaus();
    hh->Fill(10*r);    
  }

  f->Write();

  
}

void binEx(){

  TFile* f = new TFile("data.root", "read");
  TH1F* hh = (TH1F*) gDirectory->Get("aHist");

  RooRealVar x("x","x",-10,10);
  RooDataHist data("data", "dataset with x", x, hh);

  RooRealVar mean("mean", "mean", 0, -10, 10);
  RooRealVar sigma("sigma", "Width", 3, -10,10);

  RooGaussian gauss("gauss", "gauss(x| mean, sigma)", x, mean, sigma);

  gauss.fitTo(data);
  
  RooPlot* xframe = x.frame();
  data.plotOn(xframe);
  gauss.plotOn(xframe);
  xframe->Draw();

  mean.Print();
  sigma.Print();
  
}
