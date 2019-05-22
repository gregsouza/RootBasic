
void readTst(){

  FILE *fp=fopen("temp.dat","r");
  char Name[16];
  float temp;
  int year;

  char line[36];
  while(fgets(line,36, fp)){
    sscanf(&line[0], "%s %f %d", Name, &temp,&year );
    std::cout << Name << "\t" << temp << "\t" << year <<"\n";
    
  }
  
  
}
