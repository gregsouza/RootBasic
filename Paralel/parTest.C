

struct workInput{

  Int_t workerID;
  TH1F *h;
};

void PrintAll(std::vector<TH1*> hists){

  for(int i=0; i<hists.size(); i++){
    hists[i]->Print();
  }
  
}


void parTest(){
  const UInt_t nNumbers=20000000U;
  const Int_t nWorkers=6;

  //  TFile fout("myFile.root", "RECREATE");
  
  auto workItem = [](Int_t workerID){
    
    TRandom3 workerRndm(workerID);
    TH1F* h = new TH1F(Form("myHisto_%u", workerID), "The Histogram", 64, -4,4);
    for(UInt_t i=0; i<nNumbers; i++){
      h->Fill(workerRndm.Gaus());
    }

    return h;
  };

  
  ROOT::TProcessExecutor workers(nWorkers);

  workers.MapReduce(workItem, ROOT::TSeqI(nWorkers), PrintAll);

}
