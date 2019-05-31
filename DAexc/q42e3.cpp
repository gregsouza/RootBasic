#include <iostream>
#include <vector>
#include <cmath>

int main(void){

  std::vector<double> speeds = {299794000, 299791000,
				299770000, 299789000,
				299790000};
  std::vector<double> errors = {3000, 5000, 2000,
				3000, 4000};

  double Norm=0, avarage=0;
  double t=0;
  for(int i=0; i<speeds.size(); i++){
    t = 1/errors[i];
    t*=t;

    avarage+= speeds[i]*t;
    Norm+=t;
  }
  avarage /= Norm;

  std::cout<<" Avarage: \t" << avarage <<
    " pm "<< 1/std::sqrt(Norm) << "\n";
  
  
}
