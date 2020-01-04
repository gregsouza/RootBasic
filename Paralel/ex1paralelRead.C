void ex1paralelRead(){
   // First enable implicit multi-threading globally, so that the implicit parallelisation is on.
   // The parameter of the call specifies the number of threads to use.
   int nthreads = 8;
   ROOT::EnableImplicitMT(nthreads);
   // Open the file containing the tree
   auto file = TFile::Open("http://root.cern.ch/files/h1/dstarmb.root");
   // Get the tree
   TTree *tree = nullptr;
   file->GetObject<TTree>("h42", tree);
   const auto nEntries = tree->GetEntries();
   std::cout << nEntries <<"\n";
   // Read the branches in parallel.
   // Note that the interface does not change, the parallelisation is internal
   TStopwatch t1;
   t1.Start();
   for (auto i : ROOT::TSeqUL(nEntries)) {
      tree->GetEntry(i); // parallel read
   }
   t1.Stop();
   std::cout << "Parallel time counter: \n";
   t1.Print();
   // IMT parallelisation can be disabled for a specific tree
   tree->SetImplicitMT(false);
   // If now GetEntry is invoked on the tree, the reading is sequential
   t1.Reset();
   t1.Start();
   for (auto i : ROOT::TSeqUL(nEntries)) {
      tree->GetEntry(i); // sequential read
   }
   t1.Stop();
   std::cout << " \n Linear time counter: \n";
   t1.Print();
   // Parallel reading can be re-enabled
   tree->SetImplicitMT(true);
   // IMT can be also disabled globally.
   //   As a result, no tree will run GetEntry in parallel
   //   ROOT::DisableImplicitMT();
   // This is still sequential: the global flag is disabled, even if the
   // flag for this particular tree is enabled
   t1.Reset();
   t1.Start();
   for (auto i : ROOT::TSeqUL(nEntries)) {
     tree->GetEntry(i); // sequential read
   }
   
   t1.Stop();
   std::cout << " \n Full Linear time counter: \n";
   t1.Print();
}
