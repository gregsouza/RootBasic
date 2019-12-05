#include <vector>
#include <iostream>
#include "TTree.h"
#include "TFile.h"


void treeOrderW(){

  TFile* fout = new TFile("tree1.root", "RECREATE");
  
  TTree* t1 = new TTree("t1", "Test Tree for Order");
  Double_t tx,ty,tz;
  t1->Branch("x", &tx, "x/F");
  t1->Branch("y", &ty, "y/F");
  t1->Branch("z", &tz, "z/F");

  for(int i=0; i<100; i++){
    tx = (Double_t) i+0.1;
    ty = (Double_t) i+0.1;
    tz = (Double_t) i+0.1;

    t1->Fill();
    
  }
    
}


void treeOrderR(){

  TFile* fout = new TFile("tree1.root");

  TTree* t1 = (TTree*) fout->Get("t1");
  Double_t x,y,z;
  t1->SetBranchAddress("x", &x);

  for(int i=0; i<100; i++){
    t1->GetEntry();
    std::cout<< x << "\n";
    
  }
  
  
  
}

void treeOrder(){

  treeOrderW();
  treeOrderR();
  
  
}
