#include <iostream>

void treeW(){
  //Input File
  FILE *fp=fopen("dat26.dat","r");
  char line[3];
  //ROOT File
  TFile *f = new TFile("dat26.root", "RECREATE");
  //Create Tree
  TTree *T = new TTree("T", "A Tree");
  int n;
  T->Branch("Numbers", &n, "number/I");

  //Filling the tree
  while(fgets(line,3,fp)){
    sscanf(&line[0], "%d", &n);
    T->Fill();
  }

  f->Write();
}

void treeR(){
  TFile *f = new TFile("dat26.root");

  //Setting hist
  TH1I* hist = new TH1I("h","Hist", 8, 30, 90);

  //Setting Tree
  TTree *T = (TTree*) f->Get("T");
  int n;
  T->SetBranchAddress("Numbers", &n);

  Long64_t nentries = T->GetEntries();
  for(int i=0; i<nentries; i++){
    T->GetEntry(i);
    hist->Fill(n);
  }

  hist->Draw();
  std::cout<< hist->GetBin(2) <<"\t"<<
    hist->GetBinCenter(2) <<"\t" << hist->GetBinContent(2) << "\t"<<
    hist->GetNbinsX() << "\n";
  
  
}

void q2p6(){
  treeR();
  
}
