void scaleMyHist(TH1D *h1, Double_t * scale);
void configMyHist(TH1D *h1, Bool_t stat, Width_t width);


TFile *f1 = new TFile("wave_48mV5_40ADC.root","READ");
TTree *t1 = (TTree*)f1->Get("t1");

const Int_t n_bins = 250;

Double_t z[n_bins], y[n_bins],ErrorZ[n_bins];

TH1D *h0 = new TH1D("h0","Channel 0",n_bins,0,2500);
TH1D *h1 = new TH1D("h1","Channel 1",n_bins,0,2500);
TH1D *hsum = new TH1D("hsum","Sum",n_bins,0,2500);

TH1D *hq0 = new TH1D("hq0","Channel 0",n_bins,0,1200000);
TH1D *hq1 = new TH1D("hq1","Channel 1",n_bins,0,1200000);
TH1D *hqsum = new TH1D("hsum","Sum",n_bins,0,1200000);
TH1D *hteste = new TH1D("hteste","",n_bins,0,2500);


Double_t charge0, charge1;
Double_t peak0, peak1;
Bool_t coincidence;
Double_t scale0, scale1;

Int_t n = t1->GetEntries();

Double_t sphe[2] = {600,500};

void alphaSourceSpectrum(){
    
    
    
   
    Double_t totalCharge = 0;
    Double_t scaleTotal = 0;
    
    t1->SetBranchAddress("peak0",&peak0);
    t1->SetBranchAddress("peak1",&peak1);
    t1->SetBranchAddress("charge0",&charge0);
    t1->SetBranchAddress("charge1",&charge1);
    t1->SetBranchAddress("coincidence",&coincidence);
    
    
    
    
    
    
    h0->SetLineColor(kBlack);
    h1->SetLineColor(kBlue);
    hteste->SetLineColor(kRed);
    
    for(Int_t i = 0; i<n; i++){
        t1->GetEntry(i);
        if(coincidence==true && peak0>150 && peak1>150){
            h0->Fill(charge0/sphe[0]);
            h1->Fill(charge1/sphe[1]);
            totalCharge = charge0/sphe[0]+charge1/sphe[1];
            hsum->Fill(totalCharge);
            
            hq0->Fill(charge0);
            hq1->Fill(charge1);
            totalCharge = charge0+charge1;
            hqsum->Fill(totalCharge);
        }
        
    }
    scaleMyHist(h0,&scale0);
    scaleMyHist(h1,&scale1);
    scaleMyHist(hsum,&scaleTotal);
    
    configMyHist(h0,0,2);
    configMyHist(h1,0,2);
    configMyHist(hsum,1,2);
    
    hq0->Scale(scale0);
    hq1->Scale(scale1);
    hqsum->Scale(scaleTotal);
    
    configMyHist(hq0,0,2);
    configMyHist(hq1,0,2);
    configMyHist(hqsum,0,2);
    
    hq0->GetXaxis()->SetTitle("Charge (ADC*nsec)");
    hq1->GetXaxis()->SetTitle("Charge (ADC*nsec)");
    hqsum->GetXaxis()->SetTitle("Charge (ADC*nsec)");
   
    
    TCanvas *c1 = new TCanvas();
    c1->cd(1);
    
    hsum->Draw("hist");
    
    // mu2 = 4.464/4.759 mu1 = 0.938012187*[2]
    // A2 = A1
    
    // m3 = 4.187/4.759 mu1
    // A3 = 2.2/48.9 A1
    TF1 *spec = new TF1("spec","([0]/(2*[2]))*exp((x-[1])/[2]+[3]*[3]/(2*[2]*[2]))*TMath::Erfc(((x-[1])/[3]+[3]/[2])/TMath::Power(2,0.5))  +  ([0]/(2*[2]))*exp((x-0.938012187*[1])/[2]+[3]*[3]/(2*[2]*[2]))*TMath::Erfc(((x-0.938012187*[1])/[3]+[3]/[2])/TMath::Power(2,0.5))  +  ([0]*0.044989775/(2*[2]))*exp((x-0.879806682*[1])/[2]+[3]*[3]/(2*[2]*[2]))*TMath::Erfc(((x-0.879806682*[1])/[3]+[3]/[2])/TMath::Power(2,0.5))",300,2500);
    
    spec->SetParameter(0,6);
    spec->SetParameter(1,700);
    spec->SetParameter(2,1350);
    spec->SetParameter(3,71.2);
    
    spec->SetParName(0,"A_{1}");
    spec->SetParName(1,"#mu_{1}");
    spec->SetParName(2,"#tau");
    spec->SetParName(3,"#sigma");
    
    hsum->Fit("spec","R");
    spec->Draw("SAME");
    
    TCanvas *c2 = new TCanvas();
    
    h1->Draw("hist");
    h0->Draw("hist SAME");
    hsum->Draw("hist SAME");
    
    gPad->BuildLegend();
    TCanvas *c3 = new TCanvas();
    
    hq1->Draw("hist");
    hq0->Draw("hist SAME");
    hqsum->Draw("hist SAME");
    gPad->BuildLegend();

}
   

// ______________________________________________________________________________
void scaleMyHist(TH1D *h1, Double_t *scale){
    
    //     scale = h1->GetXaxis()->GetBinWidth(1)/(h1->Integral());
    *scale = 1/(h1->Integral());
    cout << "h1: scale = " << *scale << " integral = " << h1->Integral() << endl;
    h1->Scale(*scale);
    //     cout << h1->Integral() << " <---- " << h1->GetXaxis()->GetBinWidth(1) << endl;
}
// ______________________________________________________________________________


// ______________________________________________________________________________

void configMyHist(TH1D *h1, Bool_t stat, Width_t width){
    h1->GetXaxis()->SetTitle("Photon-electrons");
    //     h1->GetXaxis()->SetTitle("Charge (ADC*nsec)");
    h1->GetYaxis()->SetTitle("Normalized Counts");
    //     h1->SetTitle("Single Photon Electrons Comparison");
    h1->SetStats(stat);
    h1->SetLineWidth(width);
}
// ______________________________________________________________________________


