#include <iostream>

double binnedMean(const TH1I* hist);
double binnedMedian(const TH1I* hist);

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

void treeH(){
  TFile *f = new TFile("dat26.root", "UPDATE");

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

  f->Write();
  
  
}

void treeM(){
  //It'll find Mean mode and median, binned and unbinned

  TFile *f = new TFile("dat26.root");

  //BINNED STUFF
  TH1I *hist = (TH1I*) f->Get("h");

  double bMode;
  int NbinMode, bModeHeight;
  bModeHeight = hist->GetMaximum();
  NbinMode = hist->GetMaximumBin();
  bMode = hist->GetBinCenter(NbinMode);

  double bMean;
  bMean = binnedMean(hist);

  double bMedian;
  bMedian= binnedMedian(hist);

  double mean;
  mean = hist->GetMean();

  double median(0), a;
  a=0.5;
  hist ->GetQuantiles(1,&median, &a);

  double mode(0), binMode(0),modeHeight(0);
  //rebinn
  hist->SetBins(68,22,90);
  modeHeight = hist->GetMaximum();
  binMode = hist->GetMaximumBin();
  mode = hist->GetBinCenter(NbinMode);
  

  
  std::cout <<"\n bin Median:  "  << bMedian <<
    "\t Median  "<< median << "\n";

  
  std::cout<<"\n Bin Mean:  " << bMean <<
    "\t Mean:  " << mean <<"\n";

  std::cout <<"\nMode \t Bin N \t Bin Value \t X-Value \n";
  std::cout << "Binned \t " << NbinMode << "\t     " <<
    bModeHeight<< "\t \t"<< bMode <<"\n";
  std::cout << "Binned \t " << binMode << "\t     " <<
    modeHeight<< "\t \t"<< mode <<"\n";
  
  std::cout << NbinMode;
  
 
  
  
}

double binnedMean(const TH1I* hist){
  double bMean(0);
  int Nbins;
  double bCenter, bHeight, bNorm=0;
  Nbins = hist-> GetNbinsX();
  for(int i=0; i<Nbins; i++){
    bCenter = hist->GetBinCenter(i);
    bHeight = hist->GetBinContent(i);
    bMean+= bCenter*bHeight;
    bNorm+= bHeight;
  }
  bMean = bMean/bNorm;
  return bMean;
}

double binnedMedian(const TH1I* hist){
  double bMedian;
  int Nbins;
  Nbins = hist->GetNbinsX();
  double integrated(0);
  int nentries;
  nentries = hist->GetEntries();
  int i=0;
  while(integrated<nentries/2.0){
    integrated+=hist->GetBinContent(i);
    i++;
  }
  bMedian = hist->GetBinCenter(i);
  return bMedian;
}
void q2p6(){

  treeM();
  return;
  
}
