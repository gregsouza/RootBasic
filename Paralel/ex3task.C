

int Fibonacci(int n){

  if(n<5)
    return 1;
  else{
    int x[5];
    ROOT::Experimental::TTaskGroup tg;

    
    tg.Run([&] {x[0]=Fibonacci(n-1);});
    tg.Run([&] {x[1]=Fibonacci(n-2);});
    tg.Run([&] {x[2]=Fibonacci(n-3);});
    tg.Run([&] {x[3]=Fibonacci(n-4);});
    tg.Run([&] {x[4]=Fibonacci(n-5);});
    tg.Wait();

    return x[0]+x[1]+x[2]+x[3]+x[4];
    
  }
  
}

void ex3task(){

  ROOT::EnableImplicitMT(8);
  int fib=30;
  std::cout << fib <<"\t" <<Fibonacci(fib) << "\n";
  
  // const Int_t nWorkers(10);
  // std::vector<Int_t> fibs;
  // for(int i=0; i<nWorkers; i++) fibs.push_back(40+i);

  // auto workItem =[](Int_t fib){
    

    
  //   return 0;
  // };

  // ROOT::TProcessExecutor workers(nWorkers);

  // workers.Map(workItem, fibs);
  
}
