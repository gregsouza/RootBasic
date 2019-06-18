#define _USE_MATH_DEFINES

#include <iostream>
#include <random>
#include <ctime>
#include <cmath>

namespace MyRandom{
  
  std::mt19937 mersenne(static_cast<std::mt19937::result_type>(std::time(nullptr)));
  
  double uniform(double min, double max){
    std::uniform_real_distribution<double> u(min, max);
    return u(MyRandom::mersenne);
  }
  
}

double estimatePI(int64_t Nevents);

int main(void){
  // std::cout << M_PI <<"\n";
}

void MCpi(){

  TFile *f = new TFile("MCpi.root", "RECREATE");

  TTree *Pies = new TTree("Pies", "A Tree");
  int64_t Ntries(0);
  double pi(0);
  double error(0);
  Pies->Branch("mcpi", &pi, "mcpi/D");
  Pies->Branch("Ntries", &Ntries, "Ntries/I");
  Pies->Branch("error", &error, "error/D");

  for(int i=1; i<10; i ++){
    Ntries = std::pow(10.,i);
    pi = estimatePI(Ntries);
    error = std::fabs( (pi-M_PI)/M_PI);
    std::cout<< pi << "\n";
    Pies->Fill();
  }

  Pies->Scan();

  f->Write();
  
  return;
  
}

double estimatePI(int64_t Nevents){
  
  int64_t accepted(0);
  double x(0), y(0);
  for(int i=0; i< Nevents; i++){
    x = MyRandom::uniform(0, 1);
    y = MyRandom::uniform(0, 1);

    if( x*x+y*y < 1)
      accepted++;
  }


  if(accepted){
    double temp;
    temp = (double) accepted/Nevents ;
    return (double) 4*temp;
  }
  
  return -1;
  
}
