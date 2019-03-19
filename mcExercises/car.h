#define ROAD_SIZE 1000
#define Prob (0.333)
#include <cstdlib>

double getRand(){
  return ((float)rand())/RAND_MAX;
}

class Car{
private:
  int m_position;
  int m_speed;
  int m_distance;
public:
  
  Car(int position=0, int speed=0):
    m_position(position), m_speed(speed), m_distance(0) {}

  //Getters
  int getPosition() {return m_position;}
  int getSpeed() {return m_speed;}
  int getDistance() {return m_distance;}
  //hance

  void changePosition(int p){ m_position=p;}
  void changeSpeed(int s) {m_speed=s; }

  void move(){
    m_distance+=m_speed;

    if(m_position+m_speed<ROAD_SIZE)
      m_position+=m_speed;
    else
      m_position+=m_speed-ROAD_SIZE;
    
  }

  void speedUp(int maxSpeed){
    if(m_speed<maxSpeed){
      m_speed++;
    }
  }

  void checkAhead(std::vector<Car> &cars){
    
    for(int d=1; d<=m_speed; d++){

      int checkPosition=m_position+d;
      if(checkPosition> ROAD_SIZE)
	checkPosition-=ROAD_SIZE;

      for(Car c: cars){
	if(c.getPosition()==checkPosition){
	  m_speed=d-1;
	  return;
	} //if positions
      }  //for cars
    } //for d

    return;
  } //function

  void breakChance(){
    if(m_speed>0){
      if(getRand()>Prob)
	m_speed--;
      
    }
  }
  
};

