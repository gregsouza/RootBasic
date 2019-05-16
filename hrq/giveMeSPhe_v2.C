

vector<Double_t> xforcharge;
vector<Double_t> yforcharge;

Double_t tolerance= 1.0;
Double_t tolerance_tspec= 2.3;
Double_t limit = 12;

const Int_t nshow = 100;
// Double_t my_events[nshow] = {1,11,13,15,45,99,235,1009,2356,6451};
// Double_t my_events[nshow] = {1,11,13,15,37,99,399,1009,2356,6451};
// Double_t my_events[nshow] = {1,11,13,23,37,71,452,978,2356,6451};

Double_t my_events[nshow];
Int_t l = 0;
Bool_t goRandom=true;

const Int_t nacq = 6000;
const Double_t timeCut = 6000; // will only look at data after 6000 ns


Double_t meanEv = 0;
Double_t sdevEv = 0;

const Int_t dataPerEvent = ((nacq*4-4)-timeCut)/4; // 396 ns - 100 ns divide by 4 ns. = 74
Double_t peaksTotal[nacq]; // this will be the 74 data after 100 ns
Double_t timeTotal[nacq]; // this will be the 74 data after 100 ns
Double_t timePerEvent[dataPerEvent]; 
Double_t peaksPerEvent_tspec[dataPerEvent]; 
Double_t peaksPerEvent[dataPerEvent]; 
Double_t rawEvent[dataPerEvent]; 
Double_t dest[dataPerEvent]; 
Double_t eventNow = 0;

Int_t totalEvents = 0;
Int_t n_pulses = 125; // always odd please
Int_t rangeForMaximum = (n_pulses-1)/2; // here is the array range for looking at maximum 

string filename = "0_wave1_48mV5_40ADC_Ch0_amplificador"; //quicker way to change
string roofile = filename + "_copy.root";

TFile *f2 = new TFile("phe_output_1.root","RECREATE");

TH1D *baseline_tspec = new TH1D("baseline_tspec","new baseline",990,-10,1000);
TH1D *baseline = new TH1D("baseline","normal baseline",24,-8,8);

TH1D *hcharge = new TH1D("hcharge","SPhe charge 1000 pts - sigma 3",125,-1000,11000);    

TGraph *gtotal;
TGraph *gtspec;
TGraph *graw;
TGraph *gpoints;
TGraph *gintegral;

Bool_t hitEv = false;

TSpectrum *s = new TSpectrum(1000,1);

Double_t mean;
Double_t stddev;
Double_t mean_tspec;
Double_t std_tspec;

Double_t cutCase = 0;
Double_t charge=0;

void drawMySamples(Int_t nfound,Double_t fPositionX[],Double_t fPositionY[], Double_t *xpeaks);
Double_t convertToTime(Double_t value);

Bool_t calculateMyCharge(Int_t peakpos){
    Int_t timesAtNegative = 0;
    charge=0;
    for(Int_t i = (peakpos-rangeForMaximum); i<=(peakpos+rangeForMaximum); i++){
            if(peaksPerEvent[i]<-2.5){
                timesAtNegative++;
                if(peaksPerEvent[i]<-3.){
                    timesAtNegative++;
                    if(peaksPerEvent[i]<-4.){
                        timesAtNegative++;
                    }
                }
            }
            if(peaksPerEvent[i]>=limit*stddev){
                timesAtNegative=100;
            }
            
        charge = charge+peaksPerEvent[i];
    }
    charge=charge*4;
//     if(abs(charge)<100){
//         cout << eventNow << " have a charge off " << charge << " integrated at " << convertToTime(peakpos) <<  endl;
//     }
    
       
    if(timesAtNegative<=5){
        if(eventNow == my_events[l]){
            for(Int_t i = (peakpos-rangeForMaximum); i<=(peakpos+rangeForMaximum); i++){
//                 cout << peaksPerEvent[i] << " " << convertToTime(i) << " "  << timesAtNegative << endl;
                xforcharge.push_back(timePerEvent[i]);
                yforcharge.push_back(peaksPerEvent[i]);
            }
            cout << "The charge was " << charge << endl;
        }
        hcharge->Fill(charge);
        return true;
    }
    else{
        cutCase = 3;
        if(timesAtNegative>=100){
            cutCase = 100;
        }
        return false;
    }
    
}

Bool_t checkConditions(Int_t peakpos, Int_t &next_valid, Bool_t &goforcharge, Double_t stddev){
    Bool_t keepLooking = true;
    Double_t maxvalue = peaksPerEvent[peakpos];
    Int_t newposition=peakpos;
    Int_t timesHigher = 0;
    if((peakpos + rangeForMaximum)>=dataPerEvent ){ // out of array range
        //cout << "Can't integrate this :/" << endl;
        cutCase = -10;
        return false;
    }
    else{ // still in
        for(Int_t i = (peakpos-rangeForMaximum); i <= (peakpos+rangeForMaximum); i++){ // for position - 63 to position + 63
            
            if(peaksPerEvent[i]>maxvalue){
//                 cout << "keep going from " << peakpos << " to " << i << " that means " << convertToTime(i) << " ns" << endl;
                newposition = i;
                maxvalue = peaksPerEvent[i];
            }
        }
        
        if((newposition+2*rangeForMaximum)<dataPerEvent){
            next_valid = peakpos+2*rangeForMaximum;
            if(peaksPerEvent[newposition]>=(tolerance*stddev)){
                goforcharge = calculateMyCharge(newposition);   
                //                 cout << "leaving with newposition = " << newposition << " = " << convertToTime(newposition) << endl;
            }
            else{
//                 cout << "probably noise at " << newposition << " = " << convertToTime(newposition) << endl;
                goforcharge=false;
                cutCase = 2;
            }
            return true;
        }
        else{
//             cout << "it has entered the search, but we are done " << endl;
            goforcharge = false;
            cutCase = 1;
            return false;
        }
        
    }
}



void searchMyPeaks(Double_t event){ // This function will... search... my... peaks 
    
    s->SmoothMarkov(rawEvent,dataPerEvent,3); // Here i use TSpectrum to smooth the data at peaksPerEvent_tspec. 15 is basically the number of interactions.
    
    // nfound store the number of peaks found.
    Int_t nfound = s->SearchHighRes(rawEvent,    // source
                                    peaksPerEvent_tspec,                   // destination vector
                                    dataPerEvent,           // nbins
                                    3,                      // sigma tolerance
                                    10,                   // threshold in % of maximum peak: (value)*maximumPeak/100
                                    kTRUE,                  // bool (background remove)
                                    3,                      // Interactions for deconvolution
                                    kFALSE,                  // bool markov 
                                    20);                     // avarage for Markov window
    
    Double_t fPositionX[nfound]; //actually only using it at drawMySamples
    Double_t fPositionY[nfound];
    Int_t peakpos; // will store the array position of peaks
    Double_t *xpeaks = s->GetPositionX(); //pointer working as array to store positions as Double_t
    sort(xpeaks,xpeaks+nfound);
    
    Int_t next_valid=0; // for checking reasons 
    
    for(Int_t i = 0; i < dataPerEvent; i++){ //taking baselines for check standard deviation
        baseline->Fill(peaksPerEvent[i]);
        baseline_tspec->Fill(peaksPerEvent_tspec[i]);
    }
    mean = baseline->GetMean();
    stddev = baseline->GetStdDev();
    mean_tspec = baseline_tspec->GetMean();
    std_tspec = baseline_tspec->GetStdDev();
    
    Bool_t keepLooking = true;
    Bool_t goforcharge = false;
    
    xforcharge.clear();
    yforcharge.clear();
    for (Int_t i = 0; i < nfound; i++){ // here will loop inside the peaks
        peakpos=static_cast<Int_t>(xpeaks[i]);
        if(peaksPerEvent_tspec[peakpos]>=mean_tspec+tolerance_tspec*abs(std_tspec) && peakpos>=next_valid && (peakpos-2*rangeForMaximum)>0 ){  // the peak is higher then my tolerance?
            // the peak is higher then my next valid array position?
            
            keepLooking = checkConditions(peakpos,next_valid,goforcharge,stddev); // check the charge and tell me if i should keep moving 
//             cout << "checking conditions for " << peakpos << " at time = " << convertToTime(peakpos) << " next one have to be bigger then " << next_valid <<  endl;
//             cout << "is it good for charge? " << goforcharge << endl;
            
            if(goforcharge==false && eventNow==my_events[l]){
                cout << "There was a case = " << cutCase << " with charge of: " << charge << endl;
            }
            cutCase = 0;
            if(!keepLooking){ //if false
                i = nfound-1; // finish my for
//                 if((event) == my_events[l]){
//                     cout << event << " " << peakpos << " " << timePerEvent[peakpos] << endl;
//                 }
                
                
            }
        }
        
            
        //                     cout << peakpos << " " << fPositionX[i] << " " << fPositionY[i] << endl;
    }
    
    baseline->Reset();
    baseline_tspec->Reset();
    
    
    
    // this is for events sample
    if((event) == my_events[l]){
        cout << "---------->      hit for event: " << event << " " << l+1 << "/" << nshow << endl;
        drawMySamples(nfound,fPositionX,fPositionY,xpeaks);
        l++;
        if(l==nshow){
//             cout << " UEH!! " << endl;
            l=0;
        }
    }
    
}


void giveMeSPhe_v2(){
    
    gStyle->SetOptFit(); 
    gStyle->SetPadTickX(1);
    gStyle->SetPadTickY(1);
    gStyle->SetPadGridX(1);
    gStyle->SetPadGridY(1);
    
    
    TFile *f1 = new TFile(roofile.c_str(),"");
    TTree *t1 = (TTree*)f1->Get("t1");
    Int_t nentries = t1->GetEntries();
    Int_t nevents = nentries/nacq;
    
    Double_t adc,peak,time, event;
    Double_t adc_s, adc_b, peak_s, peak_b;
    
    t1->SetBranchAddress("adc",&adc);
    t1->SetBranchAddress("adc_s",&adc_s);
    t1->SetBranchAddress("adc_b",&adc_b);
    t1->SetBranchAddress("peak",&peak);
    t1->SetBranchAddress("peak_s",&peak_s);
    t1->SetBranchAddress("peak_b",&peak_b);
    t1->SetBranchAddress("time",&time);
    t1->SetBranchAddress("event",&event);
    
    
    
    TH1D *candidates = new TH1D("candidates","Candidates per event",nevents,0,nevents);
    
    hcharge->GetYaxis()->SetTitle("Normalized Counts");
    hcharge->GetXaxis()->SetTitle("Charge (ADC*nsec)");
    TRandom *rmd = new TRandom();
    
    // getting random values for sample
    if(goRandom){
        my_events[0] = 1;
        for(Int_t i = 1; i<nshow; i++){
            my_events[i] = static_cast<Int_t>(rmd->Uniform(my_events[i-1]+1,my_events[i-1]+50));
        }
        sort(my_events,my_events+nshow);
        cout << "Random sampling was: \n{";
        for(Int_t i = 0; i<nshow; i++){
            cout << my_events[i];
            if(i!=nshow-1){
                cout << ", ";
            }
        }
        cout << "}" << endl;
    }
    Int_t j = 0; // this will help me with the peaksPerEvent below
    Int_t k = 0;
    
    
     //for(Int_t i = 0; i<2700000; i++){
    for(Int_t i = 0; i<nentries; i++){
        
        t1->GetEntry(i);
        if(i%(nacq)==0 && i!=0){ //close of one event ! (the branch "event" have already been updated!
            eventNow = event-1; // just in case

//             cout << event-1 << endl;
            searchMyPeaks(eventNow); // Well :) guess what
            j=0;
            k=0;
        }
        
        peaksTotal[k] = peak; // This are been used only to plot the sample canvas! 
        timeTotal[k] = time;
        //             cout << peaksTotal[k] << " " << timeTotal[k] << " " << event << " "  << k << endl;
        
        k++;
        if(time>timeCut){ // Search peaks only after timeCut in nanoseconds
            peaksPerEvent_tspec[j] = peak; // This one will be changed by TSpectrum
            peaksPerEvent[j] = peak; // This one not
            rawEvent[j] = adc; // This one not
            timePerEvent[j] = time;
            //                 cout << peaksPerEvent_tspec[j] << " " << timePerEvent[j] << " " << j << endl;
            j++;
        }
        //             cout << i << " " <<  j << " of " << dataPerEvent << " " << time << " " << event << endl;
        
        
        //       
        
    }
    
    TCanvas *c1 = new TCanvas();
    hcharge->Draw("");
    
    f2->WriteObject(hcharge,"hcharge","TObject::kOverwrite");
    
}

void drawMySamples(Int_t nfound,Double_t fPositionX[],Double_t fPositionY[], Double_t *xpeaks){
    Int_t peakpos;
    for(Int_t i = 0; i < nfound; i++){
        peakpos=static_cast<Int_t>(xpeaks[i]);
        fPositionX[i] = timePerEvent[peakpos];
        fPositionY[i] = peaksPerEvent_tspec[peakpos];
    }
    
    
    TCanvas *c1 = new TCanvas("c1", "c1",0,25,1366,717);
    
    TLine *lmean = new TLine(timePerEvent[0],mean_tspec,timePerEvent[dataPerEvent-1],mean_tspec);
    TLine *lstd = new TLine(timePerEvent[0],0+tolerance_tspec*std_tspec,timePerEvent[dataPerEvent-1],0+tolerance_tspec*std_tspec);
    
    TLine *lmean2 = new TLine(timePerEvent[0],mean,timePerEvent[dataPerEvent-1],mean);
    TLine *lstd2 = new TLine(timePerEvent[0],mean+tolerance*stddev,timePerEvent[dataPerEvent-1],mean+tolerance*stddev);
    TLine *llimit2 = new TLine(timePerEvent[0],mean+limit*stddev,timePerEvent[dataPerEvent-1],mean+limit*stddev);
    TMultiGraph *g = new TMultiGraph();
    gtotal = new TGraph(nacq,timeTotal,peaksTotal);
    graw = new TGraph(dataPerEvent,timePerEvent,rawEvent);
    gtspec = new TGraph(dataPerEvent,timePerEvent,peaksPerEvent_tspec);
    gpoints = new TGraph(nfound,fPositionX,fPositionY);
    
    gintegral = new TGraph(xforcharge.size(),&xforcharge[0],&yforcharge[0]);
    
    
    gtotal->SetLineColor(kTeal-8);
    gpoints->SetMarkerColor(kBlack);    
    gpoints->SetMarkerStyle(21);    
    gpoints->SetMarkerSize(0.7);    
    gtspec->SetLineColor(kBlue);
    gtspec->SetLineWidth(2);
    graw->SetLineWidth(2);
    gintegral->SetMarkerColor(kRed);
    
    g->Add(gtotal);
    
    lmean->SetLineColor(kRed);
    lstd->SetLineColor(kRed);
    lmean->SetLineWidth(2);
    lstd->SetLineWidth(2);
   
    
    lmean2->SetLineColor(kGreen);
    lstd2->SetLineColor(kGreen);
    llimit2->SetLineColor(kGreen);
    lmean2->SetLineWidth(2);
    lstd2->SetLineWidth(2);
    llimit2->SetLineWidth(2);
    
    
    g->Add(gtspec,"LP");
    //g->Add(graw,"LP");
    g->Add(gpoints,"P");
    g->Add(gintegral,"P*");
    g->Draw("A");
    lmean->Draw();
    lstd->Draw();
    lmean2->Draw();
    lstd2->Draw();
    llimit2->Draw();
    
    
    f2->WriteObject(c1,(to_string(my_events[l]).c_str()),"TObject::kOverwrite");
    
    gtotal->Set(0);
    gtspec->Set(0);
    graw->Set(0);
    gpoints->Set(0);
    gintegral->Set(0);
    c1->Close();
    
}

Double_t convertToTime(Double_t value){
        return value*4+6004;
}
