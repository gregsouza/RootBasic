

void readTst(){

  TFile* f = new TFile("convData.root", "read");
  RooRealVar x("x", "x", 5,0., 10.);
  RooDataSet* data = (RooDataSet*) gDirectory->Get("exgData");

  RooPlot* frame = x.frame();
  data->plotOn(frame);
  frame->Draw();
  
}
