// This program will save event by event registered by the ADC. 
// Multiple files will(!) be read this way.
// Change the number registred lines of each event (Record Length: ## at the data file)

// Created by Henrique Souza, Augusth 2018

void readData(string file_ch0, string file_ch1, Int_t linhas,string rootfile,string rootfilecopy, Double_t &tEvent);
void getMyBaseline(string rootfile, string rootfilecopy);
void fillMyTree(string rootfile, Double_t *myBaseline0, Double_t *myBaseline1, Bool_t *myCoincidence);
void smoothWaveform(vector<Double_t> &y, Int_t interactions);
void readFiles(string files, Int_t linhas);
Double_t checkBigger(Double_t &adcpeak, Double_t adccharge, Double_t data, Double_t time, Double_t chargeTime);


//Number of lines in each event (RECORDO_LENGTH: ##)
//Be aware, this is a global variable :)
Int_t *linhasEvento = new Int_t;
Double_t filethreshold = 40;

Bool_t OnlyOneEvent = false; // Do you want only one event? Choose it wisely    

TH1D *hbase0 = new TH1D("hbase0","finding baseline",160,400,416);
TH1D *hbase1 = new TH1D("hbase1","finding baseline",160,400,416);

Double_t chargeTime = 10000;
Double_t baselineTime = 6000;

void adc_read_all_data(){
    //Insert the file name here. DONT NEED .txt !
    Int_t linhas = 6000;
        system("rm files.txt");
        system("ls -1 *.txt| sed -e 's/.txt$//' > files.txt");
    readFiles("files.txt",linhas); //use it like this
    //     readData("myfile", 100); //or like this
    return;
    
}

void smoothWaveform(Double_t y[], Int_t interactions){
    Double_t sum=0;
    Int_t j = 0;
    for(Int_t i = 0; i<*linhasEvento; i++){
        sum = sum+y[i];
        if(j==(interactions-1)){
            for(Int_t k = (i+1-interactions); k<i; k++){
                y[k] = sum/interactions;
                
                //                 cout << y[k] << endl;
            }
            sum=0;
            j=-1;
        }
        j++;
    }
}

void readFiles(string files, Int_t linhas){
    
    ifstream filesdata;
    filesdata.open(files.c_str(),ios::in);
    string file_ch0, file_ch1;
    
    
    TFile *f1, *f2;
    TTree *t1, *t2;
    
    string rootfile;
    string rootfilecopy;
    
    //For t1
    Double_t adc0 = 0;
    Double_t adc1 = 0;
    Double_t event = 0;
    
    //For t2
    Double_t adcpeak0 = 0;
    Double_t adcpeak1 = 0;
    Double_t adccharge = 0;
    Double_t adccharge0 = 0;
    Double_t adccharge1 = 0;
    
    
    Double_t time = 0; // time in ns
    Double_t dtime = 4; // steps (ADC's MS/s, 250 MS/s = 4 ns steps)
    
    Double_t tEvent = 0;
    
    
    
    
    
    Bool_t first_file = true;
    
    if(filesdata.good() && filesdata.is_open()){ // Ok
        cout << "Lendo o arquivo " << files << " ... " << endl;
    }
    else{ 
        cout << "O arquivo " << files << " não abriu!!" << endl;
        return;
    }
    
    // While for read the entire file
    while(!filesdata.fail()){
        filesdata >> file_ch0 >> file_ch1;
        if(filesdata.bad() || filesdata.fail()){
            break;
        }
        
        // ______________________________ Create root files at first files only __________________________________
        if(first_file){
            first_file = false;
            char *nameFragment = new char[file_ch0.length()+1]; // files will always be "2_wave0_47mV5_40ADC_Ch0_amplificador" 
            // ..........................................................................0123456789012345678
            // soo root file can be wave_47mV5_40ADC that is location 2-18 excluding 6
            
            strcpy(nameFragment,file_ch0.c_str());
            string name;
            for(Int_t pts = 2; pts <=18; pts++){
                if(pts!=6){
                    name = name + nameFragment[pts];
                }
            }
            rootfilecopy = name + "_copy.root";
            rootfile = name + ".root";
            
            string erase1 = "rm " + rootfilecopy;
            string erase2 = "rm " + rootfile;
            system(erase1.c_str());
            system(erase2.c_str());
            
            
            f1 = new TFile(rootfilecopy.c_str(),"RECREATE");
            t1 = new TTree("t1","ADC row waveform");
            
            f2 = new TFile(rootfile.c_str(),"RECREATE");
            t2 = new TTree("t1","ADC processed waveform");
            
            
            t1->Branch("adc0",&adc0,"adc0/D");
            t1->Branch("adc1",&adc1,"adc1/D");
            t1->Branch("time",&time,"time/D");
            t1->Branch("event",&event,"event/D");            
            
            
            t2->Branch("adcpeak0",&adcpeak0,"adcpeak0/D");
            t2->Branch("adcpeak1",&adcpeak1,"adcpeak1/D");
            t2->Branch("adccharge",&adccharge,"adccharge/D");
            t2->Branch("adccharge0",&adccharge0,"adccharge0/D");
            t2->Branch("adccharge1",&adccharge1,"adccharge1/D");
            
            
            f1->Write();
            f2->Write();
            
            f1->Close();
            f2->Close();
        }
        // _______________________________________________________________________________________________________
        
        readData(file_ch0,file_ch1,linhas,rootfile,rootfilecopy,tEvent);
        
    }
    getMyBaseline(rootfile,rootfilecopy);
    
}


// This function will read your data and create a root file with the same name
void readData(string file_ch0, string file_ch1, Int_t linhas,string rootfile,string rootfilecopy, Double_t &tEvent){
    
    TFile *f1 = new TFile(rootfilecopy.c_str(),"UPDATE");
    TTree *t1 = (TTree*)f1->Get("t1");
    
    TFile *f2 = new TFile(rootfile.c_str(),"UPDATE");
    TTree *t2 = (TTree*)f2->Get("t1");
    
    *linhasEvento = linhas;
    
    Double_t adcpeak0 = 0;
    Double_t adcpeak1 = 0;
    Double_t adccharge = 0;
    Double_t adccharge0 = 0;
    Double_t adccharge1 = 0;
    
    Double_t adc0 = 0;
    Double_t adc1 = 0;
    Double_t event = 0;
    Bool_t coincidence = false;
    
    Double_t time = 0; // time in ns
    Double_t dtime = 4; // steps (ADC's MS/s, 250 MS/s = 4 ns steps)
    
    
    t1->SetBranchAddress("adc0",&adc0);
    t1->SetBranchAddress("adc1",&adc1);
    t1->SetBranchAddress("time",&time);
    t1->SetBranchAddress("event",&event);
    
    t2->SetBranchAddress("adcpeak0",&adcpeak0);
    t2->SetBranchAddress("adcpeak1",&adcpeak1);
    t2->SetBranchAddress("adccharge",&adccharge);
    t2->SetBranchAddress("adccharge0",&adccharge0);
    t2->SetBranchAddress("adccharge1",&adccharge1);    
    
    
    string filename_ch0 = file_ch0 + ".txt";
    string filename_ch1 = file_ch1 + ".txt";
    
    
    Double_t data0 = 0;
    Double_t data1 = 0;
    string headers;
    
    Int_t numeroLinhas=0;
    
    
    ifstream file0, file1;
    file0.open(filename_ch0.c_str(),ios::in);
    file1.open(filename_ch1.c_str(),ios::in);
    
    // here i check if the data file is open (or exist), if not, I remove the .root files and close the program. 
    if(file0.good() && file0.is_open()){ // Ok
        cout << "Lendo o arquivo " << filename_ch0 << " ... " << endl;
    }
    else{ 
        cout << "O arquivo " << filename_ch0 << " não abriu!!" << endl;
        return;
        
    }
    
    if(file1.good() && file1.is_open()){ // Ok
        cout << "Lendo o arquivo " << filename_ch1 << " ... " << endl;
    }
    else{ 
        cout << "O arquivo " << filename_ch1 << " não abriu!!" << endl;
        return;
        
    }
    
    Bool_t closeMyWhile = false; // keep it false
    // While for read the entire file
    while(!file0.fail() && closeMyWhile == false){ // We can just look for one file, they shold have the same amount of lines anyway!
        
        /*** NO HEADER FOR MY FILES ***/
        //         for(int i = 0; i < 7; i++) //jump headers
        //         {
        //             getline(file0,headers);
        //             numeroLinhas++;
        //             //             cout << headers << endl;
        //         }
        adcpeak0 = 0;
        adcpeak1 = 0;
        adccharge = 0;
        adccharge0 = 0;
        adccharge1 = 0;
        time = 0;
        for(int j = 0; j < *linhasEvento; j++)
        {
            file0 >> data0;
            file1 >> data1;
            if(file0.bad() || file0.fail()){
                break;
            }
            adccharge0 = checkBigger(adcpeak0,adccharge0,data0,time,chargeTime);
            adccharge1 = checkBigger(adcpeak1,adccharge1,data1,time,chargeTime);
           
            adc0 = data0;
            adc1 = data1;
            
            numeroLinhas++;
            event = tEvent;

            t1->Fill();
            time = time + dtime;
            if(OnlyOneEvent == true){
                closeMyWhile=true;
            }
        }
        
        adccharge0 = adccharge0*4;
        adccharge1 = adccharge1*4;
        adccharge = adccharge0+adccharge1;
        t2->Fill();
        tEvent++;

    }
    numeroLinhas = numeroLinhas; //will run the first for no matter what
    cout << tEvent << " eventos em " << numeroLinhas << " linhas" << endl;
    
    f1->WriteObject(t1,"t1","TObject::kOverwrite");
    f2->WriteObject(t2,"t1","TObject::kOverwrite");
    
    f1->Close();
    f2->Close();
    
}



void getMyBaseline(string rootfile, string rootfilecopy){
    
    TFile *f1 = new TFile(rootfilecopy.c_str(),"UPDATE");
    TTree *t1 = (TTree*)f1->Get("t1");
    
    
    
    Int_t nentries = t1->GetEntries(); //total numer of entries
    Double_t adc0 = 0;
    Double_t adc1 = 0;
    Double_t event = 0;
    Double_t time = 0;
    Bool_t coincidence = false;
    //     Double_t baselineTime = 6000;
    
    Int_t totalev = nentries/ *linhasEvento+1;
//     cout << totalev << endl;
    
    // this baseline will be used in the non copy file
    
    //setting previous branchs
    t1->SetBranchAddress("adc0",&adc0);
    t1->SetBranchAddress("adc1",&adc1);
    t1->SetBranchAddress("time",&time);
    t1->SetBranchAddress("event",&event);
    
    
    Double_t peak0, peak1; //new branch
    
    
    TBranch *b1 = t1->Branch("peak0",&peak0,"peak0/D");
    TBranch *b2 = t1->Branch("peak1",&peak1,"peak1/D");
    TBranch *b3 = t1->Branch("coincidence",&coincidence,"coincidence/O");
    
    
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
    
    Double_t myBaseline0[stepsForBaseline];
    Double_t myBaseline1[stepsForBaseline];
    Bool_t myCoincidence[stepsForBaseline];
    Double_t biggervalue = 0;
    
    Double_t myBaseline0_2[totalev];
    Double_t myBaseline1_2[totalev];
    Bool_t myCoincidence_2[totalev];
    
    
    Double_t baseline = 0;    
    
    for(Long_t i = 0; i < nentries; i++){
        t1->GetEntry(i);
        if(i%*linhasEvento==0){
            myCoincidence[stepsB] = false;
            biggervalue=0;
        }
        if(adc1>biggervalue && time<chargeTime){
            biggervalue = adc1;
            
        }
        
        
        if(time>baselineTime){
            hbase0->Fill(adc0);
            hbase1->Fill(adc1);
        }
        
        //Finish before the last one
        //One example is i=9999 this is event number 99. i = 10000 is already event number 100
        if((i+1)%(group)==0 && lastGroup!=true && i!= 0){ 
            //             cout << i << " " << group << " " << hbase0->GetEntries() << " " << time << endl;
            //             return;
            myBaseline0[stepsB] = (hbase0->GetMean());
            myBaseline1[stepsB] = (hbase1->GetMean());
            if((biggervalue-myBaseline1[stepsB])>=filethreshold){
                myCoincidence[stepsB] = true;
            }
            //             if(myBaseline0[stepsB]!=2048){
            //                 cout << "i = " << i << ", myBaseline0["<<stepsB<<"] = " << myBaseline0[stepsB] << endl;
            //             }
            
            stepsB++;
            
            if(stepsB==(stepsForBaseline)){ //if it's the last group, we insert the residual part
                lastGroup = true;
                //                 cout << "passou aqui " << hbase0->GetEntries() << " i = " << i << endl;
            }
            else{
                //                 f1->WriteObject(hbase0,(std::to_string(event+1)).c_str());
                hbase0->Reset(); // reset unless is the last one
                hbase1->Reset(); // reset unless is the last one
            }            
        }
        else if(lastGroup==true){ // when it is the last one
            if(i==nentries-1){
                myBaseline0[stepsB] = (hbase0->GetMean());
                myBaseline1[stepsB] = (hbase1->GetMean());
                if((biggervalue-myBaseline1[stepsB])>=filethreshold){
                    myCoincidence[stepsB] = true;
//                     cout << event << " " << biggervalue << " " << biggervalue-myBaseline1[stepsB] << " " << myCoincidence[stepsB] << endl;
                }
                //                 cout << hbase0->GetEntries() << " i = "<< i <<  endl;
                //                 cout << "ULTIMO!!! -> i = " << i << ", myBaseline0["<<stepsB<<"] = " << myBaseline0[stepsB] << endl;
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
    for(Long_t i = 0; i < nentries; i++){
        t1->GetEntry(i);
        
        peak0 = adc0-myBaseline0[stepsB];
        peak1 = adc1-myBaseline1[stepsB];
        coincidence = myCoincidence[stepsB];
        
        //         cout << "i = " << i << " peak = " << peak << ", myBaseline0["<<stepsB<<"] = " << myBaseline0[stepsB] << endl;
        
        if((i+1)%(group)==0 && lastGroup!=true && i!= 0){
            stepsB++;
            //             cout << stepsB << endl;
        }
        if(stepsB==(stepsForBaseline)){
            lastGroup = true;
        }
        
        b1->Fill();
        b2->Fill();
        b3->Fill();
        
    }
    stepsB = 0;
    for(Int_t i = 0; i < totalev; i++){
        myBaseline0_2[i] = myBaseline0[stepsB];
        myBaseline1_2[i] = myBaseline1[stepsB];
        myCoincidence_2[i] = myCoincidence[stepsB];
        stepsB++;
        
    }
    //     cout << stepsB << endl;
    
    f1->WriteObject(t1,"t1","TObject::kOverwrite"); // save only the new version of the tree
    f1->Close();
    
    fillMyTree(rootfile,myBaseline0_2,myBaseline1_2,myCoincidence_2);
    
}

void fillMyTree(string rootfile, Double_t *myBaseline0, Double_t *myBaseline1, Bool_t *myCoincidence){
    
    TFile *f1 = new TFile(rootfile.c_str(),"UPDATE");
    TTree *t1 = (TTree*)f1->Get("t1");
    
    
    Int_t nentries = t1->GetEntries();
    
    Double_t peak0, peak1;
    Double_t charge, charge0, charge1;
    
    Double_t adcpeak0 = 0;
    Double_t adcpeak1 = 0;
    Double_t adccharge0 = 0;
    Double_t adccharge1 = 0;
    Bool_t coincidence = false;
    
    //     Double_t chargeTime = 5000;
    Double_t dtime = 4;
    Int_t ncharges = (chargeTime/dtime+1);
//     cout << ncharges << endl;
    
    t1->SetBranchAddress("adcpeak0",&adcpeak0);
    t1->SetBranchAddress("adcpeak1",&adcpeak1);
    t1->SetBranchAddress("adccharge0",&adccharge0);
    t1->SetBranchAddress("adccharge1",&adccharge1);
    
    TBranch *b1 = t1->Branch("peak0",&peak0,"peak0/D");
    TBranch *b2 = t1->Branch("peak1",&peak1,"peak1/D");
    TBranch *b3 = t1->Branch("charge",&charge,"charge/D");
    TBranch *b4 = t1->Branch("charge0",&charge0,"charge0/D");
    TBranch *b5 = t1->Branch("charge1",&charge1,"charge1/D");
    TBranch *b6 = t1->Branch("coincidence",&coincidence,"coincidence/O");
    
    
    
    for(Long_t i = 0; i < nentries; i++){
        t1->GetEntry(i);
        peak0 = adcpeak0-myBaseline0[i];
        peak1 = adcpeak1-myBaseline1[i];
        charge0 = adccharge0-4*myBaseline0[i]*ncharges;
        charge1 = adccharge1-4*myBaseline1[i]*ncharges;
        charge = charge0+charge1;
        coincidence = myCoincidence[i];
//         if(charge>80 && charge<90){
//             cout << charge0 << " = " << adccharge0 << " = " << myBaseline0[i] << "*" << ncharges << " i = " << i << endl;
//             
//         }
        b1->Fill();
        b2->Fill();
        b3->Fill();
        b4->Fill();
        b5->Fill();
        b6->Fill();
    }
    
    f1->WriteObject(t1,"t1","TObject::kOverwrite"); // save only the new version of the tree
    f1->Close();
}


Double_t checkBigger(Double_t &adcpeak, Double_t adccharge, Double_t data, Double_t time, Double_t chargeTime){
    if(time<=chargeTime){
        if(adcpeak==0){
            adcpeak = data;
        }
        else{
            if(adcpeak<data)adcpeak=data;
        }
        
        adccharge = adccharge + (data);
    }
    return adccharge;
}
