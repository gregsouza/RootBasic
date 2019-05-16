Int_t n_peaks = 7;

Double_t myfunction(Double_t *x, Double_t *par){
    Double_t f;
    Double_t xx = x[0];
    f  = par[0]*exp(-0.5*TMath::Power((xx-par[1])/par[2],2)); // first argument
    f = f+par[3]*exp(-0.5*TMath::Power((xx-par[4])/par[5],2));
    for(Int_t i = 0; i<n_peaks; i++){
        f = f+ abs(par[i+6])*exp(-0.5*TMath::Power((xx-(i+2)*par[4])/(TMath::Power((i+2),0.5)*par[5]),2));
    }
    return f;
}
// "[0]*exp(-0.5*TMath::Power((x-[1])/[2],2))
// +[3]*exp(-0.5*TMath::Power((x-[4])/[5],2))
// 
// +[6]*exp(-0.5*TMath::Power((x-2*[4])/(TMath::Power(2,0.5)*[5]),2))
// +[7]*exp(-0.5*TMath::Power((x-3*[4])/(TMath::Power(3,0.5)*[5]),2))
// +[8]*exp(-0.5*TMath::Power((x-4*[4])/(TMath::Power(4,0.5)*[5]),2))
// +[9]*exp(-0.5*TMath::Power((x-5*[4])/(TMath::Power(5,0.5)*[5]),2))"
string startingPump(){
    string f = "gaus(0) + gaus(3)";
    Int_t aux = 0;
    for(Int_t i = 0; i<n_peaks; i++){
        f = f + " + gaus(" + to_string(i+6+aux) + ")";
        aux = aux+2;
    }
    return f;
//         TF1 *func = new TF1("func","gaus(0)+gaus(3)+gaus(6)+gaus(9)+gaus(12)+gaus(15)",-2000,5000);

}




void getMyParameters(Double_t peaks[],Double_t stdpeaks[],TF1 *func){
    for(Int_t i=0; i<n_peaks; i++){
        peaks[i] = (i+2)*(func->GetParameter(4));
        stdpeaks[i] = TMath::Power((i+2),0.5)*func->GetParameter(5);
//         cout << peaks[i] << endl;
    }
}

void fit_sphe(){
    TFile *f2 = new TFile("phe_output_1.root","READ");
    TH1D *hcharge = (TH1D*)f2->Get("hcharge");
    
    TCanvas *c1 = new TCanvas("c1","Carga");
    c1->SetLogy();
    
    //First function, will almost fit freely
    TF1 *func = new TF1("func",startingPump().c_str(),-2000,10000);
    TF1 *fu[2+n_peaks];
    string funame;
    for(Int_t i = 0; i<(2+n_peaks); i++){
        funame = "fu[" + to_string(i) + "]";
        fu[i] = new TF1(funame.c_str(),"gaus(0)",-2000,10000);
    }
        

    
    Double_t firstPeak;
    Double_t firstStd;
    
    Double_t peaks[n_peaks];
    Double_t stdpeaks[n_peaks];
    
    
    firstPeak = 700;
    firstStd = 300;
    
    func->SetParameters(0.004,50,67,0.004,firstPeak,firstStd); // this values can change
    
    Double_t startpoint = 0.001;
    Int_t aux = 0;
    for(Int_t i = 0; i<n_peaks; i++){
        func->SetParameter((i+6+aux),startpoint);
        aux++;
        func->SetParameter((i+6+aux),(i+2)*firstPeak);
        aux++;
        func->SetParameter((i+6+aux),(i+2)*firstStd);
        startpoint = startpoint/5;
    }
    func->SetParName(4,"#mu");
    func->SetParName(5,"#sigma");
    
    func->SetNpx(1000);
    
    getMyParameters(peaks,stdpeaks,func);
    
    // limits as a try to not get strange values
//     func->SetParLimits(7,secondPeak-10,secondPeak+10);
//     func->SetParLimits(8,secondStd,3*secondStd);
//     func->SetParLimits(10,thirdPeak-10,thirdPeak+10);
//     func->SetParLimits(11,thirdStd,5*thirdStd);
//     func->SetParLimits(13,fourthPeak-10,fourthPeak+10);
//     func->SetParLimits(14,3*fourthStd,5*fourthStd);
//     func->SetParLimits(16,fifthPeak-10,fifthPeak+10);
//     func->SetParLimits(17,3*fifthStd,5*fifthStd);
    
    Double_t scale = 1/(hcharge->Integral());
    hcharge->Scale(scale);
    hcharge->Draw("hist");
    hcharge->Fit("func","R0Q");
    
    // recovery parameters
    getMyParameters(peaks,stdpeaks,func);
    aux=0;
    // use as fixed
    for(Int_t i = 0; i<n_peaks; i++){
        
        func->FixParameter((i+7+aux),peaks[i]);
        aux++;
        func->FixParameter((i+7+aux),stdpeaks[i]);
        aux++;
    }
        
//     func->FixParameter(7,secondPeak);
//     func->FixParameter(8,secondStd);
//     func->FixParameter(10,thirdPeak);
//     func->FixParameter(11,thirdStd);
//     func->FixParameter(13,fourthPeak);
//     func->FixParameter(14,fourthStd);
//     func->FixParameter(16,fifthPeak);
//     func->FixParameter(17,fifthStd);
    
    hcharge->Fit("func","R0Q");
    
    // set a new function, now fixed for real
    TF1 *lastOne = new TF1("lastOne",myfunction,-2000,10000,6+n_peaks);
    
    
    lastOne->SetParameter(0,func->GetParameter(0));
    lastOne->SetParameter(1,func->GetParameter(1));
    lastOne->SetParameter(2,func->GetParameter(2));
    
    lastOne->SetParameter(3,func->GetParameter(3));
    lastOne->SetParameter(4,func->GetParameter(4));
    lastOne->SetParameter(5,func->GetParameter(5));
    aux = 0;
    for(Int_t i = 0; i<n_peaks; i++){
        lastOne->SetParameter((i+6),func->GetParameter(i+6+aux));
        aux=aux+2;
    }
    
//     lastOne->SetParameter(6,func->GetParameter(6));
//     lastOne->SetParameter(7,func->GetParameter(9));
//     lastOne->SetParameter(8,func->GetParameter(12));
//     lastOne->SetParameter(9,func->GetParameter(15));
    
    lastOne->SetParName(4,"#mu");
    lastOne->SetParName(5,"#sigma");
    
    hcharge->Fit("lastOne","R0");
    
    
    
    
    getMyParameters(peaks,stdpeaks,lastOne);
    
    fu[0]->SetParameter(0,lastOne->GetParameter(0));
    fu[0]->SetParameter(1,lastOne->GetParameter(1));
    fu[0]->SetParameter(2,lastOne->GetParameter(2));
    
    fu[1]->SetParameter(0,lastOne->GetParameter(3));
    fu[1]->SetParameter(1,lastOne->GetParameter(4));
    fu[1]->SetParameter(2,lastOne->GetParameter(5));
    
   for(Int_t i = 0; i<n_peaks; i++){
       fu[i+2]->SetParameter(0,abs(lastOne->GetParameter(i+6)));
       fu[i+2]->SetParameter(1,peaks[i]);
       fu[i+2]->SetParameter(2,stdpeaks[i]);
    }
    //     fu4->SetParameter(0,lastOne->GetParameter(7));
    //     fu4->SetParameter(1,thirdPeak);
    //     fu4->SetParameter(2,thirdStd);
    //     
    //     fu5->SetParameter(0,lastOne->GetParameter(8));
    //     fu5->SetParameter(1,fourthPeak);
    //     fu5->SetParameter(2,fourthStd);
    //     
    //     fu6->SetParameter(0,lastOne->GetParameter(9));
    //     fu6->SetParameter(1,fifthPeak);
    //     fu6->SetParameter(2,fifthStd);
    for(Int_t i = 0; i<(2+n_peaks); i++){
        fu[i]->SetLineColor(kGray+1);
        fu[i]->SetNpx(1000);
        fu[i]->Draw("SAME");
    }
    
    lastOne->SetNpx(1000);
    
    
    
    lastOne->SetRange(-2000,10000);
    lastOne->Draw("SAME");
    
    
}
