#include <iostream>
#include <vector>

void TreeW(){
  //Outside data
  std::vector<int> clas = {22,48,76,10,22,4 ,68,44,10,76,14,56};
  std::vector<int> quan = {63,39,61,30,51,44,74,78,55,58,41,69};

  TFile *f = new TFile("grades.root", "RECREATE");
  TTree *grades = new TTree("Grades", "Grades Tree");
  int c,q;
  grades->Branch("Classical", &c, "Classical/I");
  grades->Branch("Quantum", &q, "Quantum/I");

  int len = clas.size();
  for(int i =0; i<len; i++){
    c = clas[i];
    q = quan[i];

    grades->Fill();
    
  }

  std::cout<<"Writen Grades: \n";
  grades->Print();
  
  f->Write();  
}

void TreeR(){

  TFile *f = new TFile("grades.root");

  //Setting 2D hist
  TH2I* histGrandes =
    new TH2I("hGrades", "Grades Hist",
  	     /*x-Clas*/ 100, 1., 100.,
  	     /*y-Quan*/ 100, 1., 100.);
  
  
  //Setting the Tree
  TTree *grades = (TTree*) f->Get("Grades");
  int c,q;
  grades->SetBranchAddress("Classical", &c);
  grades->SetBranchAddress("Quantum", &q);

  
  Long64_t nentries = grades->GetEntries();
  for(int i=0; i<nentries;i++){
    grades->GetEntry(i);
    std::cout << c <<" \t" << q <<"\n";
    histGrandes->Fill(c,q);
  }

  double clasMean, clasSD;
  clasMean = histGrandes->ProjectionX()->GetMean();
  clasSD = histGrandes->ProjectionX()->GetStdDev();
  
  double quanMean, quanSD;
  quanMean = histGrandes->ProjectionY()->GetMean();
  quanSD = histGrandes -> ProjectionY()-> GetStdDev();

  double covariance, correlation;
  correlation = histGrandes->GetCorrelationFactor();
  covariance = histGrandes->GetCovariance();
  

  std::cout<<"Class \t Mean \t StdDev \n";
  std::cout<<"Clas \t" << clasMean <<" \t" << clasSD <<"\n";
  std::cout<<"Quan \t" << quanMean <<" \t" << quanSD << "\n";
  std::cout<<"Covariance: " << covariance <<
    "\t Correlation: " << correlation << "\n";
}


void q2p4(){
  TreeR();
}
