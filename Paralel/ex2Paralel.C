

void ex2Paralel(){
  const UInt_t nNumbers=20000000U;
  const Int_t nWorkers=6;



  
  auto workItem = [](UInt_t workerID){

    TRandom3 workerRndm(workerID);
    TFile f(Form("myFile_%u.root", workerID), "RECREATE");
    TH1F h(Form("myHisto_%u", workerID), "The Histogram", 64, -4, 4);
    for(UInt_t i=0; i<nNumbers; ++i)
      h.Fill(workerRndm.Gaus());

    h.Write();
    return 0;
  };

  ROOT::TProcessExecutor workers(nWorkers);

  std::vector<Int_t> ids;
  
  for(int i=0; i<nWorkers; i++)
    ids.push_back(i);
  
  workers.Map(workItem, ids);
  
  
}
