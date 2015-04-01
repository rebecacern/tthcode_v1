#include "TFile.h"
#include "TTree.h"
#include "TLorentzVector.h"
#include "objectClasses.h"


void minimal(int nsel=0, int mode=0, bool silent=0){


  
  char plotName[300];
  sprintf(plotName,"test");
  if (nsel == 0) {sprintf(plotName,"ttH125");}
  if (nsel == 1) {sprintf(plotName,"TTJets");}
  
  
  char myRootFile[300];
  sprintf(myRootFile,"../tuples/%s.root", plotName);

  TFile *fin = new TFile(myRootFile);
  // TFile::Open("root://eoscms.cern.ch//eos/cms/store/user/gesmith/crabdir/v3/ttH125/multileptree_9_1_uB4.root"); 

  TTree *tree = (TTree*)fin->Get("OSTwoLepAna/summaryTree");

  // Tree branches
  Double_t        mcwgt;
  Double_t	  wgt;
  Double_t	  wallTimePerEvent;
  Int_t 	  eventnum;
  Int_t 	  lumiBlock;
  Int_t 	  runNumber;
  Int_t 	  higgs_decay;

  vector<ttH::Electron> *preselected_electrons = 0;
  vector<ttH::Muon> *preselected_muons = 0;
  vector<ttH::Lepton> *preselected_leptons = 0;
  vector<ttH::Jet> *preselected_jets = 0;

  vector<ttH::Electron> *tight_electrons = 0;
  vector<ttH::Muon> *tight_muons = 0;
  vector<ttH::Lepton> *tight_leptons = 0;
  vector<ttH::Jet> *tight_bJets = 0;
  
  vector<ttH::MET> *met = 0;
  
  
  TBranch *b_mcwgt;   //!
  TBranch *b_wgt;   //!
  TBranch *b_wallTimePerEvent;   //!
  TBranch *b_eventnum;   //!
  TBranch *b_lumiBlock;   //!
  TBranch *b_runNumber;   //!
  TBranch *b_higgs_decay;   //!
  
  TBranch *b_preselected_electrons = 0;
  TBranch *b_preselected_muons = 0;
  TBranch *b_preselected_leptons = 0;
  TBranch *b_preselected_jets = 0;
  
  TBranch *b_tight_electrons = 0;
  TBranch *b_tight_muons = 0;
  TBranch *b_tight_leptons = 0;
  TBranch *b_tight_bJets = 0;
  TBranch *b_met = 0;
 
  tree->SetBranchAddress("mcwgt", &mcwgt, &b_mcwgt);
  tree->SetBranchAddress("wgt", &wgt, &b_wgt);
  tree->SetBranchAddress("wallTimePerEvent", &wallTimePerEvent, &b_wallTimePerEvent);
  tree->SetBranchAddress("eventnum", &eventnum, &b_eventnum);
  tree->SetBranchAddress("lumiBlock", &lumiBlock, &b_lumiBlock);
  tree->SetBranchAddress("runNumber", &runNumber, &b_runNumber);
  tree->SetBranchAddress("higgs_decay", &higgs_decay, &b_higgs_decay);
  tree->SetBranchAddress("preselected_electrons", &preselected_electrons, &b_preselected_electrons);
  tree->SetBranchAddress("preselected_muons", &preselected_muons, &b_preselected_muons);
  tree->SetBranchAddress("preselected_leptons", &preselected_leptons, &b_preselected_leptons);
  tree->SetBranchAddress("preselected_jets", &preselected_jets, &b_preselected_jets);
  tree->SetBranchAddress("tight_electrons", &tight_electrons, &b_tight_electrons);
  tree->SetBranchAddress("tight_muons", &tight_muons, &b_tight_muons);
  tree->SetBranchAddress("tight_leptons", &tight_leptons, &b_tight_leptons);
  tree->SetBranchAddress("tight_bJets", &tight_bJets, &b_tight_bJets);
  tree->SetBranchAddress("met", &met, &b_met);
  ////
  
  
  cout <<"[Info:] You are running " << plotName << endl;
  if (mode != 0 && mode !=1 && mode !=2) mode = 0;
  if (!silent){
    cout << "[Info:]" ;
    if (mode == 0) cout << " emu channel, " ;
    else if (mode == 1) cout << " mumu channel, " ;
    else if (mode == 2) cout << " ee channel, " ;
  }
  char newRootFile[300];
  sprintf(newRootFile,"results/output_short_%d.root", mode);
  TFile f_var(newRootFile, "UPDATE");
  if(!silent){
    std::cout << "results root file named " << newRootFile << std::endl;
    std::cout << "[Info:] Everything made using preselected objects " << std::endl;
  }
  
  // Histos
  char title[300];
  sprintf(title,"cuts_%s", plotName);
  TH1F* histo = new TH1F( title, "Cut Flow", 20, 0, 20 );
  histo->Sumw2();
  
  sprintf(title,"njets_%s", plotName);
  TH1F* histo_njets = new TH1F( title, "Number of jets pt > 20", 20, 0, 20 );
  histo_njets->Sumw2();
  
  sprintf(title,"nbjets_%s", plotName);
  TH1F* histo_nbjets = new TH1F( title, "Number of b-jets pt > 20", 20, 0, 20 );
  histo_nbjets->Sumw2();
  
  sprintf(title,"nleptons_%s", plotName);;
  TH1F* histo_nleptons = new TH1F( title, "Number of leptons pt > 10", 20, 0, 20 );
  histo_nleptons->Sumw2();
  
  sprintf(title,"njets_2lep_%s", plotName);
  TH1F* histo_njets_2l = new TH1F( title, "Number of jets pt > 20", 20, 0, 20 );
  histo_njets_2l->Sumw2();
  
  sprintf(title,"nbjets_2lep_%s", plotName);
  TH1F* histo_nbjets_2l = new TH1F( title, "Number of b-jets pt > 20", 20, 0, 20 );
  histo_nbjets_2l->Sumw2();
  
  sprintf(title,"nleptons_2lep_%s", plotName);
  TH1F* histo_nleptons_2l = new TH1F( title, "Number of leptons pt > 10", 20, 0, 20 );
  histo_nleptons_2l->Sumw2();

  
  sprintf(title,"njets_ss_%s", plotName);
  TH1F* histo_njets_ss = new TH1F( title, "Number of jets pt > 20", 20, 0, 20 );
  histo_njets_ss->Sumw2();
  
  sprintf(title,"nbjets_ss_%s", plotName);
  TH1F* histo_nbjets_ss = new TH1F( title, "Number of b-jets pt > 20", 20, 0, 20 );
  histo_nbjets_ss->Sumw2();
  
  sprintf(title,"nleptons_ss_%s", plotName);
  TH1F* histo_nleptons_ss = new TH1F( title, "Number of leptons pt > 10", 20, 0, 20 );
  histo_nleptons_ss->Sumw2();

  sprintf(title,"pt_lep1_%s", plotName);
  TH1F* histo_ptlep1 = new TH1F( title, "Pt of the leading lepton", 100, 0, 200 );
  histo_ptlep1->Sumw2();

  sprintf(title,"pt_lep2_%s", plotName);
  TH1F* histo_ptlep2 = new TH1F( title, "Pt of the second lepton", 100, 0, 200 );
  histo_ptlep2->Sumw2();

  sprintf(title,"met_%s", plotName);
  TH1F* histo_met = new TH1F( title, "Missing ET", 100, 0, 200 );
  histo_met->Sumw2();



 
  double weight = 1;
  int nused = 0;
  if (!silent) cout << "[Info:] Number of raw events: " << tree->GetEntries() << endl;
  // loop over events 
  for(int iEvent = 0; iEvent < 10000; iEvent++){
  //  for(int iEvent = 0; iEvent < tree->GetEntries(); iEvent++){
    Long64_t tentry = tree->LoadTree(iEvent);
    //Point to the proper entry
    b_preselected_electrons->GetEntry(tentry);
    b_preselected_muons->GetEntry(tentry);
    b_preselected_leptons->GetEntry(tentry);
    b_preselected_jets->GetEntry(tentry);
    b_higgs_decay->GetEntry(tentry);
    b_tight_electrons->GetEntry(tentry);
    b_tight_muons->GetEntry(tentry);
    b_tight_leptons->GetEntry(tentry);
    b_tight_bJets->GetEntry(tentry);
    b_met->GetEntry(tentry);
     
    //do stuff 
    histo->Fill(0., weight);
    
    if (nsel ==0 && !higgs_decay) continue;
    histo->Fill(1., weight);
    
    int njets = 0;
    for (int i = 0; i < preselected_jets->size() ; i++){
      ttH::Jet jet = preselected_jets->at(i);
      if (jet.tlv().Pt() < 20) continue;
      njets++;
    }
    histo_njets->Fill(njets, weight);
    
    
    int nbjets = 0;
    for (int i = 0; i < tight_bJets->size() ; i++){
      ttH::Jet bjet = tight_bJets->at(i);
      if (bjet.tlv().Pt() < 20) continue;
      nbjets++;
    }
    histo_nbjets->Fill(nbjets, weight);
    
    int nleptons = 0;
    for (int i = 0; i < preselected_leptons->size() ; i++){
      ttH::Lepton lep0 = preselected_leptons->at(i);
      if (lep0.tlv().Pt() < 10) continue;
      nleptons++;
    }
    histo_nleptons->Fill(nleptons, weight);
    
    
    
    if (preselected_leptons->size() < 2) continue;
    histo->Fill(2., weight);
    
 
    ttH::Lepton lepton1 = preselected_leptons->at(0);
    ttH::Lepton lepton2 = preselected_leptons->at(1);
    
    if (mode == 0 && abs(lepton1.pdgID) ==  abs(lepton2.pdgID)) continue;
    if (mode == 1 && (abs(lepton1.pdgID) != 13 ||  abs(lepton2.pdgID) != 13)) continue;
    if (mode == 2 && (abs(lepton1.pdgID) != 11 ||  abs(lepton2.pdgID) != 11)) continue;
    histo->Fill(3., weight);
   
     int njets_2l = 0;
    for (int i = 0; i < preselected_jets->size() ; i++){
      ttH::Jet jet = preselected_jets->at(i);
      if (jet.tlv().Pt() < 20) continue;
      njets_2l++;
    }
    histo_njets_2l->Fill(njets_2l, weight);
    
    
    int nbjets_2l = 0;
    for (int i = 0; i < tight_bJets->size() ; i++){
      ttH::Jet bjet = tight_bJets->at(i);
      if (bjet.tlv().Pt() < 20) continue;
      nbjets_2l++;
    }
    histo_nbjets_2l->Fill(nbjets_2l, weight);
    
    int nleptons_2l = 0;
    for (int i = 0; i < preselected_leptons->size() ; i++){
      ttH::Lepton lep0 = preselected_leptons->at(i);
      if (lep0.tlv().Pt() < 10) continue;
      nleptons_2l++;
    }
    histo_nleptons_2l->Fill(nleptons_2l, weight);
    
    


    if (lepton1.charge!=lepton2.charge) continue;
    histo->Fill(4., weight);

         int njets_ss = 0;
    for (int i = 0; i < preselected_jets->size() ; i++){
      ttH::Jet jet = preselected_jets->at(i);
      if (jet.tlv().Pt() < 20) continue;
      njets_ss++;
    }
    histo_njets_ss->Fill(njets_ss, weight);
    
    
    int nbjets_ss = 0;
    for (int i = 0; i < tight_bJets->size() ; i++){
      ttH::Jet bjet = tight_bJets->at(i);
      if (bjet.tlv().Pt() < 20) continue;
      nbjets_ss++;
    }
    histo_nbjets_ss->Fill(nbjets_ss, weight);
    
    int nleptons_ss = 0;
    for (int i = 0; i < preselected_leptons->size() ; i++){
      ttH::Lepton lep0 = preselected_leptons->at(i);
      if (lep0.tlv().Pt() < 10) continue;
      nleptons_ss++;
    }
    histo_nleptons_ss->Fill(nleptons_ss, weight);
    
    ttH::MET evmet = met->at(0);
    histo_met->Fill(evmet.tlv().Pt(), weight);	      
    histo_ptlep1->Fill(lepton1.tlv().Pt(), weight);
    histo_ptlep2->Fill(lepton2.tlv().Pt(), weight);
        
	      
	      
    if (lepton1.tlv().Pt() <= 20) continue;
    if (lepton2.tlv().Pt() <= 20)  continue;
   
    histo->Fill(5., weight);
    

    if (tight_leptons->size() != 2) continue;
  
    ttH::Lepton tlepton1 = tight_leptons->at(0);
    ttH::Lepton tlepton2 = tight_leptons->at(1);
    
    histo->Fill(6., weight);
    
    if (lepton1.tlv().Pt()  != tlepton1.tlv().Pt()) continue;
    if (lepton2.tlv().Pt()  != tlepton2.tlv().Pt()) continue;
    
    histo->Fill(7., weight);
   
  }
  
  
  char label[300];
  sprintf(label, "emu");
  if (mode == 1) sprintf(label,"mumu");
  if (mode == 2) sprintf(label,"ee");
  
 
 
  
  if (!silent){
    cout << "------------------------------------------" << endl;
    cout << "[Results:] preselected leptons " << label << endl;
    cout << "------------------------------------------" << endl;
    for (int i = 1; i < 9; i++){
      if (i == 1) cout << " all: " << histo->GetBinContent(i) << " +/- " << histo->GetBinError(i) << endl;
      if (i == 2) cout << " higgs decay: " << histo->GetBinContent(i) << " +/- " << histo->GetBinError(i) << endl;
      if (i == 3) cout << " 2+ preselected leptons: " << histo->GetBinContent(i) << " +/- " << histo->GetBinError(i) << endl;
      if (i == 4) cout << " decay to " << label << ": " << histo->GetBinContent(i) << " +/- " << histo->GetBinError(i) << endl;
      if (i == 5) cout << " SS: " << histo->GetBinContent(i) << " +/- " << histo->GetBinError(i) << endl;
      if (i == 6) cout << " pt > 20,20: " << histo->GetBinContent(i) << " +/- " << histo->GetBinError(i) << endl;
      if (i == 7) cout << " 2 tight: " << histo->GetBinContent(i) << " +/- " << histo->GetBinError(i) << endl;
      if (i == 8) cout << " 2 tight corresponding with the presel: " << histo->GetBinContent(i) << " +/- " << histo->GetBinError(i) << endl;
   }
    cout << "------------------------------------------" << endl;



  }
  f_var.Write();
  f_var.Close();

  

}
