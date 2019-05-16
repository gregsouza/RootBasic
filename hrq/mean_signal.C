void mean_signal(){
    
    TFile *f = new TFile("0_wave0_48mV5_40ADC_Ch0_amplificador_copy.root","READ");
    TTree *t1 = (TTree*)f->Get("t1");
    
    Int_t nentries = t1->GetEntries();
    Int_t linhasEvento = 6000;
    
    Double_t peak;
    
    t1->SetBranchAddress("peak",&peak);
    
    Double_t y_mean[linhasEvento];
    Double_t x_mean[linhasEvento];
    
    std::fill(y_mean,y_mean+linhasEvento,0); // set array with 0 in every entry
    std::fill(x_mean,x_mean+linhasEvento,0);
    
    Int_t aux_mean = 0;
    Bool_t checkAmplitude = true;
    Bool_t getmean = true;
    Double_t maxamplitude = 0;
    Int_t contador = 0;
    for(Int_t i = 0; i<nentries; i++){
        
        if(checkAmplitude){
            checkAmplitude = false;
            maxamplitude = 0;
            for(Int_t j = i + 2000/4; j<=i+3500/4; j++){
                t1->GetEntry(j);
                if(peak>=maxamplitude){
                    maxamplitude = peak;
                }
            }
            if(maxamplitude>=100){
                getmean = true;
            }
            else{
                getmean = false;
            }
        }
            
        t1->GetEntry(i);
        
        if(getmean){
            contador++;
            y_mean[aux_mean] += peak;
            x_mean[aux_mean] = 4*aux_mean;
        }
        aux_mean++;
        if(aux_mean==(linhasEvento)){
            checkAmplitude = true;
            aux_mean=0;
        }
    }
//     cout << contador << endl;
    Double_t maxvalue = *std::max_element(y_mean,y_mean+linhasEvento);
    for(Int_t i = 0; i<linhasEvento; i++){
        y_mean[i] = y_mean[i]/maxvalue;
    }
    
    f->Close();

    TFile *fout = new TFile("mean_signal.root","RECREATE");
    
    TCanvas *c2 = new TCanvas("c2","pulso medio");
    c2->SetGrid();
    c2->SetTicks();
    
    TGraph *g = new TGraph(linhasEvento,x_mean,y_mean);
    g->Draw("ALP");
    fout->WriteObject(c2,"mean pulse");
    fout->Close();
    
}
