#include <iostream>
#include <vector>

void TreeW(){
  std::vector<double> ages ={19.0,18.7,19.3,19.2,18.9,19.0,20.2,
			     19.9,18.6,19.4,19.3,18.8,19.3,19.2,
			     18.7,18.5, 18.6, 19.7,19.9, 20.0,
			     19.5, 19.4, 19.6, 20.0, 18.9};

  TFile *f1 = new TFile("Tages.root", "RECREATE");

  TTree *noTeacher = new TTree("noTeacher", "data without teacher");
  double a;
  noTeacher->Branch("Ages", &a, "age/D");
  
  TTree *wTeacher = new TTree("wTeacher", "data without teacher");
  double b;
  wTeacher->Branch("Ages", &b, "age/D");

  //Student Ages
  for(int i=0; i<ages.size()-1;i++){
    std::cout<< ages[i]<<"\n";
    a = ages[i];
    b = a;
    noTeacher->Fill();
    wTeacher->Fill();
  }
  //Teacher Ages
  b=37.0;
  wTeacher->Fill();

  std::cout <<"No Teacher Tree \n";
  noTeacher->Print();
  std::cout <<"With Teacher Tree \n";
  wTeacher->Print();
  
  f1->Write();  
}

void TreeR(){

  TFile *f = new TFile("Tages.root", "UPDATE");
  
  TTree *noTeacher = (TTree*) f->Get("noTeacher");
  noTeacher->Draw("Ages>>aHist");
  TH1F *noTeacherHist = (TH1F*) gDirectory->Get("aHist");
  //Getting Data
  double ntMean, ntSD, ntSkew;
  ntMean = noTeacherHist->GetMean();
  ntSD = noTeacherHist->GetStdDev();
  ntSkew = noTeacherHist->GetSkewness();

  
  TTree *wTeacher = (TTree*) f->Get("wTeacher");
  wTeacher -> Draw("Ages>>otherHist");
  TH1F *wTeacherHist = (TH1F*) gDirectory->Get("otherHist");

  double wtMean, wtSD, wtSkew;
  wtMean = wTeacherHist->GetMean();
  wtSD = wTeacherHist->GetStdDev();
  wtSkew = wTeacherHist->GetSkewness();

  
  std::cout << "\t    Mean \t StdDev \t Skew \n";
  std::cout << "No Teacher: " <<ntMean << "\t "<< ntSD << 
    "\t" << ntSkew << "\n";
  std::cout << "W Teacher: " <<wtMean << "\t "<< wtSD << 
    "\t" << wtSkew << "\n";
  std::cout <<"Variantion: " << (wtMean-ntMean)/wtMean <<
    "\t" << (wtSD - ntSD)/wtSD << "\t" <<
    (wtSkew-ntSkew)/wtSkew << "\n";
  
  f->Write();
}


void q2to3(){
  TreeR();
}

