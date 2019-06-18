#include <iostream>
#include <random>
#include <ctime>
/*

 */

namespace MyRandom{
  
  std::mt19937 mersenne(static_cast<std::mt19937::result_type>(std::time(nullptr)));
  
}

double uniform(double min, double max){
  std::uniform_real_distribution<double> u(min, max);
  return u(MyRandom::mersenne);
};

double f(double t);

int main(void){

  double tmin(0), tmax(5);
  double fmax;

  fmax = f(tmax)+0.2;
  
  int N=10;
  double u(0), t(0);
  for(int i=0; i<N; i++){
    t = uniform(tmin, tmax);
    u = uniform(0, fmax);

    if(u<f(t)){
      std::cout<< t << "\t" << f(t) <<"\n";
    }
    
  }
  
}


double f(double t){
  return 2*t+1;
}
