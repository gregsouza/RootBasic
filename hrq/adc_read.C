// This program will save event by event registered by the ADC. 
// Just copy the name (right click > Rename > ctrl+c) and replace at "wave_5ADC_example_only_1". 
// Multiple files can be read this way.
// Change the number registred lines of each event (Record Length: ## at the data file)

// Created by Henrique Souza, Augusth 2018

void readData(string name, Int_t linhas);
void getMyBaseline(string rootfile, string rootfilecopy);
void fillMyTree(string rootfile, Double_t *myBaseline, Double_t *Fprompt_v);
void smoothWaveform(Double_t y[], Int_t interactions);

    //Number of lines in each event (RECORDO_LENGTH: ##)
    //Be aware, this is a global variable :)
    Int_t *linhasEvento = new Int_t; 

void adc_read(){
    //Insert the file name here. DONT NEED .txt !
  Int_t linhas = 6000;
    readData(
	     "20181129_x_arapuca/run16_48mV5_40ADC_Ch0_amplificador/0_wave0_48mV5_40ADC_Ch0_amplificador",linhas); //use it like this
    //readData("0_wave1_48mV5_40ADC_Ch0_amplificador",linhas); //use it like this
    //     readData("myfile", 100); //or like this
    return;
   
}


// This function will read your data and create a root file with the same name
void readData(string name, Int_t linhas){
    
    *linhasEvento = linhas;
    
    string filename = name + ".txt";
    string rootfile = name + ".root";
    string rootfilecopy = name + "_copy.root";
    
    TFile *f1 = new TFile(rootfilecopy.c_str(),"RECREATE");
    TTree *t1 = new TTree("t1","ADC data");
    
    TFile *f2 = new TFile(rootfile.c_str(),"RECREATE");
    TTree *t2 = new TTree("t1","Amplitude spectrum");    
    
    
    //     Double_t peak = 0;
    Double_t adcpeak = 0;
    Double_t adccharge = 0;
  
    Double_t adc = 0;
    Double_t data = 0;
    Double_t event = 0;
    
    Double_t time = 0; // time in ns
    Double_t dtime = 4; // steps (ADC's MS/s, 250 MS/s = 4 ns steps)
    Double_t chargeTime = 5000; // time limit to start looking for baseline
    
    Double_t adc_s;
    Double_t adc_b;
    Double_t ys[linhas];
    Double_t yb[linhas];
    
  
    
    
    
    TBranch *b_adc = t1->Branch("adc",&adc,"adc/D");
    TBranch *b_adc_s = t1->Branch("adc_s",&adc_s,"adc_s/D");
    TBranch *b_adc_b = t1->Branch("adc_b",&adc_b,"adc_b/D");
    TBranch *b_time = t1->Branch("time",&time,"time/D");
    TBranch *b_event = t1->Branch("event",&event,"event/D");
    
    t2->Branch("adcpeak",&adcpeak,"adcpeak/D");
    t2->Branch("adccharge",&adccharge,"adccharge/D");
    
    TH2D *sample = new TH2D("sample","peak vs time",6000,0,24000,4096,0,4096);

    
    Int_t numeroLinhas=0;
    
    
    ifstream file;
    file.open(filename.c_str(),ios::in);
    
    // here i check if the data file is open (or exist), if not, I remove the .root files and close the program. 
    if(file.good() && file.is_open()){ // Ok
        cout << "Lendo o arquivo " << filename << " ... " << endl;
    }
    else{ 
        cout << "O arquivo " << filename << " não abriu!!" << endl;
        string erase1 = "rm " + rootfile;
        string erase2 = "rm " + rootfilecopy;
        system(erase1.c_str());
        system(erase2.c_str());
        return;
        
    }
    
    // to jump headers
    string headers;
    
    Bool_t OnlyOneEvent = false; //Set here as true if you want to read only one event (6000 lines)
    
    
    Bool_t closeMyWhile = false;
//     Bool_t TwoEvents = true;
    // While for read the entire file
    while(!file.fail() && closeMyWhile == false){
//         for(int i = 0; i < 7; i++) //jump headers
//         {
//             getline(file,headers);
//             numeroLinhas++;
//             //             cout << headers << endl;
//         }
        adcpeak = 0;
        adccharge = 0;
        time = 0;
        for(int j = 0; j < *linhasEvento; j++)
        {
            file >> data;
            if(file.bad() || file.fail()){
                break;
            }
            if(adcpeak==0){
                adcpeak = data;
            }
            else{
                if(adcpeak<data)adcpeak=data;           
            }
            if(time<=chargeTime){
                adccharge = adccharge + (data);
            }

            //             cout << data << endl;
            adc = data; // keep as ADC channels but trade sign
            ys[j] = data;
            yb[j] = data;
            
            numeroLinhas++;
            sample->Fill(time,adc);
            t1->SetEntries(6000*(event+1)); //this is needed to fill each branch separeted
            b_adc->Fill();
            b_time->Fill();
            b_event->Fill();
//             time.Fill()
            time = time + dtime;
            if(OnlyOneEvent == true){
                closeMyWhile=true;
            }
        }
        smoothWaveform(ys,75);
        smoothWaveform(yb,225);
        for(Int_t i = 0; i< *linhasEvento; i++){
            adc_s = ys[i];
            adc_b = yb[i];
            b_adc_s->Fill();
            b_adc_b->Fill();
        }
        adcpeak = adcpeak;
        adccharge = adccharge*4;
//         getline(file,headers);
// 	if(file.bad() || file.fail()){
// 	    //             cout << "saindo" << endl;
// 	    break;
// 	}
	//         adcpeak = adcpeak;
	t2->Fill();
	event++;
    }
    numeroLinhas = numeroLinhas; //will run the first for no matter what

    cout << event << " eventos em " << numeroLinhas << " linhas" << endl;
    f1->WriteObject(t1,"t1","TObject::kOverwrite");
    f2->WriteObject(t2,"t1","TObject::kOverwrite");
    
    TCanvas *c1 = new TCanvas("c1","Exemplo de evento");
    c1->SetGrid();
    c1->SetTicks();
    sample->Draw("COLZ");
    f1->WriteObject(c1,"persistence");
    
    

    c1->Close();
   
    f1->Close();
    f2->Close();
    
    getMyBaseline(rootfile,rootfilecopy);
}



void getMyBaseline(string rootfile, string rootfilecopy){
    
    TFile *f1 = new TFile(rootfilecopy.c_str(),"UPDATE");
    TTree *t1 = (TTree*)f1->Get("t1");
    
    Int_t nentries = t1->GetEntries(); //total numer of entries
    Double_t adc, adc_s, adc_b, time, event;
    Double_t baselineTime = 6000;
    Double_t adcpeak;
    Int_t totalev = nentries/ *linhasEvento;
    
    // this baseline will be used in the non copy file
    Double_t myBaseline2[totalev];
    Double_t Fprompt[totalev];
    Double_t intofast = 0;
    Double_t intoslow = 0;
    
    Double_t y_mean[*linhasEvento];
    Double_t x_mean[*linhasEvento];
    
    std::fill(y_mean,y_mean+*linhasEvento,0);
    std::fill(x_mean,x_mean+*linhasEvento,0);

    //setting previous branchs
    t1->SetBranchAddress("adc",&adc);
    t1->SetBranchAddress("adc_s",&adc_s);
    t1->SetBranchAddress("adc_b",&adc_b);
    t1->SetBranchAddress("time",&time);
    t1->SetBranchAddress("event",&event);
    
   
    Double_t peak,peak_s,peak_b; //new branch
    
    TBranch *newBranch = t1->Branch("peak",&peak,"peak/D");
    TBranch *newBranch2 = t1->Branch("peak_s",&peak_s,"peak_s/D");
    TBranch *newBranch3 = t1->Branch("peak_b",&peak_b,"peak_b/D");
    
    TH1D *hbase = new TH1D("hbase","finding baseline",160,400,416);
    TH1D *hbase2 = new TH1D("hbase2","baseline found",76,-38,38);
    Double_t sdev = 0;
    Double_t mean = 0;
    
    Int_t group = *linhasEvento; // group off data to be analized - X points per event, Y events 
    
    // supose we have 200 events, each one with 100 points. That give us 20000 points. 
    // We separete the events in groups of 100 events for the baseline: group 1 (events from 1 to 100) and group 2 (101 - 200)
    // So, each group of baseline will have 10000 (100*100) points and we will have 2 groups (20000/10000)
    Int_t stepsForBaseline = static_cast<Int_t>(nentries/group);
    
    //If we had 1001 events, we will have residual of 1 event
    Int_t residual = nentries%group; // not using right now, check it time to time. 

    Int_t stepsB = 0;
    Bool_t lastGroup = false; //For the last group we will include the residual, so we need this frag
    
    Double_t myBaseline[stepsForBaseline];
    Double_t baseline = 0;

    TF1 *func = new TF1("func","gaus",0,2055);
    

    Int_t aux = 0;
    for(Long_t i = 0; i < nentries; i++){
        t1->GetEntry(i);
        
        if(time>baselineTime){
            hbase->Fill(adc);
        }
        //Finish before the last one
        //One example is i=9999 this is event number 99. i = 10000 is already event number 100
        if((i+1)%(group)==0 && lastGroup!=true && i!= 0){ 
//             cout << i << " " << group << " " << hbase->GetEntries() << " " << time << endl;
//             return;
            myBaseline[stepsB] = (hbase->GetMean());
//             if(myBaseline[stepsB]!=2048){
//                 cout << "i = " << i << ", myBaseline["<<stepsB<<"] = " << myBaseline[stepsB] << endl;
//             }
            
            stepsB++;

            if(stepsB==(stepsForBaseline)){ //if it's the last group, we insert the residual part
                lastGroup = true;
//                 cout << "passou aqui " << hbase->GetEntries() << " i = " << i << endl;
            }
            else{
//                 f1->WriteObject(hbase,(std::to_string(event+1)).c_str());
                hbase->Reset(); // reset unless is the last one
            }            
        }
        else if(lastGroup==true){ // when it is the last one
            if(i==nentries-1){
                myBaseline[stepsB] = (hbase->GetMean());
//                 cout << hbase->GetEntries() << " i = "<< i <<  endl;
//                 cout << "ULTIMO!!! -> i = " << i << ", myBaseline["<<stepsB<<"] = " << myBaseline[stepsB] << endl;
//                 cout << stepsB << endl;
                
            }
        }

        if(stepsB==(stepsForBaseline)){
            lastGroup = true;
        }
        
    }
    lastGroup =false;
//     cout << "baseline = " << baseline << endl;
    stepsB=0;
    Int_t aux_mean=0;
    
    for(Long_t i = 0; i < nentries; i++){
        t1->GetEntry(i);
        peak = adc-myBaseline[stepsB];
        peak_s = adc_s-myBaseline[stepsB];
        peak_b = adc_b-myBaseline[stepsB];
        
        y_mean[aux_mean] += peak;
        x_mean[aux_mean] = 4*aux_mean;
        aux_mean++;
        if(aux_mean==(*linhasEvento)){
            aux_mean=0;
        }
        
        if(time<2500){
            intofast = intofast + peak;
        }
        if(time<10000){
            intoslow = intoslow + peak;
        }
        
        

//         cout << "i = " << i << " peak = " << peak << ", myBaseline["<<stepsB<<"] = " << myBaseline[stepsB] << endl;

        if((i+1)%(group)==0 && lastGroup!=true && i!= 0){
            
            stepsB++;
            Fprompt[stepsB] = intofast/intoslow;
            intofast = 0;
            intoslow = 0;
            
//             cout << stepsB << endl;
        }
        if(stepsB==(stepsForBaseline)){
            lastGroup = true;
        }
        
        newBranch->Fill();
        newBranch2->Fill();
        newBranch3->Fill();
    }
    stepsB = 0;
    for(Int_t i = 0; i < totalev; i++){
        myBaseline2[i] = myBaseline[stepsB];
        stepsB++;
        
    }
//     cout << stepsB << endl;
    Double_t maxvalue = *std::max_element(y_mean,y_mean+*linhasEvento);
    for(Int_t i = 0; i<*linhasEvento; i++){
        y_mean[i] = y_mean[i]/maxvalue;
    }
    
    hbase->Write("teste");
    
    TCanvas *c2 = new TCanvas("c2","pulso medio");
    c2->SetGrid();
    c2->SetTicks();
    TGraph *mean_pulse = new TGraph(*linhasEvento,x_mean,y_mean);
    mean_pulse->Draw("ALP");
    f1->WriteObject(c2,"mean pulse");
    f1->WriteObject(t1,"t1","TObject::kOverwrite"); // save only the new version of the tree
    f1->Close();
    
    fillMyTree(rootfile,myBaseline2,Fprompt);
    
}

void fillMyTree(string rootfile, Double_t *myBaseline, Double_t *Fprompt_v){
    
    TFile *f1 = new TFile(rootfile.c_str(),"UPDATE");
    TTree *t1 = (TTree*)f1->Get("t1");
    
    Int_t nentries = t1->GetEntries();
    Double_t adcpeak, peak;
    Double_t adccharge, charge;
    Double_t chargeTime = 5000;
    Double_t dtime = 4;
    Int_t ncharges = (chargeTime/dtime+1);
    
    Double_t Fprompt=0;
    
    //     cout << ncharges << endl;
    
    t1->SetBranchAddress("adcpeak",&adcpeak);
    t1->SetBranchAddress("adccharge",&adccharge);
    TBranch *newBranch = t1->Branch("peak",&peak,"peak/D");
    TBranch *newBranch2 = t1->Branch("charge",&charge,"charge/D");
    TBranch *newBranch3 = t1->Branch("Fprompt",&Fprompt,"Fprompt/D");
    
    for(Long_t i = 0; i < nentries; i++){
        t1->GetEntry(i);
        peak = adcpeak-myBaseline[i];
        charge = adccharge-4*myBaseline[i]*ncharges;
        Fprompt = Fprompt_v[i];
//         if(charge>80 && charge<90){
//             cout << charge << " = " << adccharge << " = " << myBaseline[i] << "/" << ncharges << " i = " << i << endl;
//             
//         }
        newBranch->Fill();
        newBranch2->Fill();
        newBranch3->Fill();
    }
    
    f1->WriteObject(t1,"t1","TObject::kOverwrite"); // save only the new version of the tree
    f1->Close();
}

void smoothWaveform(Double_t y[], Int_t interactions){
    Double_t sum=0;
    Int_t midpoint = (interactions-1)/2;
    Double_t y_smooth[*linhasEvento];
    for(Int_t i = midpoint; i<(*linhasEvento-midpoint); i++){
        for(Int_t j = (i-midpoint); j<=(i+midpoint); j++){
            sum = sum+y[j];
        }
        y_smooth[i] = sum/interactions;
        sum=0;
        
    }
    for(Int_t i = midpoint; i<(*linhasEvento-midpoint); i++){
       y[i] = y_smooth[i];
        
    }
}
