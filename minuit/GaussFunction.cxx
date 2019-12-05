#include <vector>


class FCNData{

public:
  std::vector<Double_t> m_pos, m_measure, m_Merror;
  
public:

  FCNData() {}
  
  FCNData(std::vector<Double_t>& pos, std::vector<Double_t>& measure,
  	  std::vector<Double_t>& Merror  ):
    m_pos(pos), m_measure(measure), m_Merror(Merror)
  {}


  Double_t modelChiSq(double &height, double &mean, double &sigma){

    int nentries = m_pos.size();
    
    Double_t gModel, chiSq(0);
    for(int i=0; i<nentries ; i++){
      gModel = height*TMath::Gaus(m_pos[i], mean, sigma, true);
      
      chiSq+= ((gModel - m_measure[i])*(gModel - m_measure[i]))/m_Merror[i];
    }

    return chiSq;
    
  }
      
};


FCNData D;

void fcn(Int_t &npar, Double_t* gin, Double_t &f, Double_t *par, Int_t iflag){

  Double_t height(par[0]), mean(par[1]), sigma(par[2]);

  f = D.modelChiSq(height, mean, sigma);
 
}



void GaussFunction(){

  //Generating Gaussian Data
  Int_t nbins(100);
  Double_t xlow(0), xup(3);
  
  TH1D* h1 = new TH1D("h1", "My Hist", nbins, 0, 3);
  TF1* f1 = new TF1("f1", "gaus", 0,3);

  Double_t genMean(1.532), genSigma(0.734);
  f1->SetParameters(1.0, genMean, genSigma);

  h1->FillRandom("f1", 10000);

  //Fillind FCN with Data
  for(int i=0; i<nbins; i++){

    if(h1->GetBinError(i)){
    //Setting bin position
      D.m_pos.push_back(  h1->GetBinCenter(i)  );
    //bin content
      D.m_measure.push_back(  h1->GetBinContent(i)  );
    //bin error
      D.m_Merror.push_back(  h1->GetBinError(i)  );

      //  std::cout << D.m_pos.back() << "\t" << D.m_measure.back() <<
      //"\t" << D.m_Merror.back() << "\n";
    }
  }

  //setting fit up
  TMinuit *gMinuit = new TMinuit(5);
  gMinuit->SetFCN(fcn);

  //Commands arguments
  Double_t arglist[10];
  Int_t ierflg = 0;

  double hstart(300), mstart(0.), sstart(1.0);
  //Starting parameters and step
  static Double_t vstart[3] = {hstart, mstart, sstart};
  static Double_t step[3] = {5 , 0.05, 0.05};

  gMinuit->mnparm(0, "Height", vstart[0], step[0], 100, 1000, ierflg);
  gMinuit->mnparm(1, "Mean", vstart[1], step[1], 0, 3, ierflg);
  gMinuit->mnparm(2, "Sigma", vstart[2], step[2], 0.2, 3, ierflg);

  //Scan Example
  arglist[0]=2; //Param to be scanned
  arglist[1]=100; //Steps in scan
  arglist[2]=0.1; //From
  arglist[3] = 3; //to
  gMinuit->mnexcm("SCAn", arglist, 4, ierflg);

  //MIGRAD example
  arglist[0] = 1000; //Max steps
  arglist[1] = 0.001;
  gMinuit->mnexcm("MIGRAD", arglist,2, ierflg);

  //Improving strategy
  arglist[0] = 2;
  gMinuit->mnexcm("SET STR", arglist,1, ierflg);

  //Final Migrad
  arglist[0] = 1000;
  arglist[1] = 0.00001;
  gMinuit->mnexcm("MIGRAD", arglist,2, ierflg);
  
  
}
