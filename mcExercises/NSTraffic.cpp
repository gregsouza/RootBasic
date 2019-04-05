#include <iostream>
#include <array>
#include <cstdlib>
#include <vector>
#include "car.h"

/* TODO

Teste Move, SpeedUp, CheckAhead

*/

void setInitial(std::vector<Car> &cars, int initialSpeed=0);
int oneRun(const int NCars);
int trafficSteps(std::vector<Car> &cars, int maxSpeed, int B=1000);
void burn(std::vector<Car> &cars, int maxSpeed, int B=1000);

void NSTraffic(){
  
  struct runData{
    
    Int_t size;
    Int_t distance;
    
  };

  srand (time(NULL));
  
  runData data;

  TFile *f = new TFile("mc.root", "RECREATE");
  TTree *tree = new TTree("NSTraffic", "Data from NS moonte calor traffic simulations ");

  tree->Branch("data", &data.size, "size:distance");
  for(int i=50; i<501; i+=5){
    data.size=i;
    data.distance=oneRun(data.size);
    tree->Fill();
    std::cout<< data.distance << "\n";
  }
  
  tree->Write();

  
}

int main(){

 
}


int oneRun(const int NCars){
    std::vector<Car> cars(NCars);
  int maxSpeed = 5;
  setInitial(cars,0);

  burn(cars, maxSpeed);
  
  return trafficSteps(cars, maxSpeed);
  
 }

int trafficSteps(std::vector<Car> &cars, int maxSpeed, int B){
  int k=cars.size();
  int totalDistance(0);
  //  std::cout<<"#step\t  #speed  #position  #distance";
  
  for(int step=0; step<B; step++) {
    for( int i=0; i<k; i++){
      cars[i].speedUp(maxSpeed);
      cars[i].checkAhead(cars);
      cars[i].breakChance();
      cars[i].move();

      totalDistance+=cars[i].getSpeed();
    }
    
  }

  return totalDistance;
}


void burn(std::vector<Car> &cars, int maxSpeed, int B){


  int k=cars.size();
  
  for(int step=0; step<B; step++) {
    for( int i=0; i<k; i++){
      cars[i].speedUp(maxSpeed);
      cars[i].checkAhead(cars);
      cars[i].breakChance();
      cars[i].move();
            
    }
    
  }  
}

void setInitial(std::vector<Car> &cars, int initialSpeed){

  int k=cars.size();
  int spaceStep = ROAD_SIZE/k;

  for(int i=0; i<k; i++){

    int position= (i+1)*spaceStep;
    if(position>ROAD_SIZE)
      position-=ROAD_SIZE;

    cars[i].changePosition(position);
    cars[i].changeSpeed(initialSpeed);
    
  }
  
}


