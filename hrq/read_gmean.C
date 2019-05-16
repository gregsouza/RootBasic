void read_gmean(){
    
    TFile *f = new TFile("0_wave0_47mV5_40ADC_Ch0_amplificador_copy.root","READ");
    TCanvas *c1 = (TCanvas*)f->Get("mean pulse");
    
    TGraph *g = (TGraph*)c1->GetPrimitive("Graph");
    
    Int_t n = g->GetN();
    cout << n << endl;
    Double_t x[n];
    Double_t y[n];
    
    ofstream myfile;
    myfile.open ("data_mean_waveform.txt");
    myfile << "time (ns) ADC Channels\n";
    
    for(Int_t i = 0; i<n; i++){
        x[i] = *(g->GetX()+i);
        y[i] = *(g->GetY()+i);
        myfile << x[i] << " " << y[i] << "\n";
    }
    
}