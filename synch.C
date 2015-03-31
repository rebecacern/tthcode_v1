#include "TFile.h"
#include "TTree.h"
#include "TLorentzVector.h"
#include "objectClasses.h"


void synch(int nsel=0, int mode=0, bool silent=0){


  TFile *fin = new TFile("../tuples/synch_full_second.root");
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

  vector<ttH::Electron> *cutBased_electrons = 0;
  vector<ttH::Muon> *cutBased_muons = 0;
  vector<ttH::Lepton> *cutBased_leptons = 0;
  
  
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
  
  TBranch *b_cutBased_electrons = 0;
  TBranch *b_cutBased_muons = 0;
  TBranch *b_cutBased_leptons = 0;
 
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
  tree->SetBranchAddress("cutBased_electrons", &cutBased_electrons, &b_cutBased_electrons);
  tree->SetBranchAddress("cutBased_muons", &cutBased_muons, &b_cutBased_muons);
  tree->SetBranchAddress("cutBased_leptons", &cutBased_leptons, &b_cutBased_leptons);
  ////
  
  if (mode != 0 && mode !=1 && mode !=2) mode = 0;
  if (!silent){
    cout << "[Info:]" ;
    if (mode == 0) cout << " emu channel, " ;
    else if (mode == 1) cout << " mumu channel, " ;
    else if (mode == 2) cout << " ee channel, " ;
  }
  char newRootFile[300];
  sprintf(newRootFile,"results/synch_%d.root", mode);
  TFile f_var(newRootFile, "RECREATE");
  if(!silent){
    std::cout << "results root file named " << newRootFile << std::endl;
  }

  // Histos
  char title[300];
  sprintf(title,"cuts");
  TH1F* histo = new TH1F( title, " ", 20, 0, 20 );
  histo->Sumw2();

 
  double weight = 1;
  int nused = 0;
  if (!silent) cout << "[Info:] Number of raw events: " << tree->GetEntries() << endl;
  // loop over events 
  for(int iEvent = 0; iEvent < tree->GetEntries(); iEvent++){
    Long64_t tentry = tree->LoadTree(iEvent);
    //Point to the proper entry
    b_preselected_electrons->GetEntry(tentry);
    b_preselected_muons->GetEntry(tentry);
    b_preselected_leptons->GetEntry(tentry);
    b_higgs_decay->GetEntry(tentry);
    b_cutBased_electrons->GetEntry(tentry);
    b_cutBased_muons->GetEntry(tentry);
    b_cutBased_leptons->GetEntry(tentry);
     
    //do stuff 
    histo->Fill(0., weight);
    
    if (!higgs_decay) continue;
    histo->Fill(1., weight);
    
    if (preselected_leptons->size() < 2) continue;
    histo->Fill(2., weight);
 
    ttH::Lepton lepton1 = preselected_leptons->at(0);
    ttH::Lepton lepton2 = preselected_leptons->at(1);
    
    if (mode == 0 && abs(lepton1.pdgID) ==  abs(lepton2.pdgID)) continue;
    if (mode == 1 && (abs(lepton1.pdgID) != 13 ||  abs(lepton2.pdgID) != 13)) continue;
    if (mode == 2 && (abs(lepton1.pdgID) != 11 ||  abs(lepton2.pdgID) != 11)) continue;
    histo->Fill(3., weight);
   

    if (lepton1.charge!=lepton2.charge) continue;
    histo->Fill(4., weight);
	      
    if (lepton1.tlv().Pt() <= 20) continue;
    if (lepton2.tlv().Pt() <= 20)  continue;
   
    histo->Fill(5., weight);
    

    if (cutBased_leptons->size() != 2) continue;
  
    ttH::Lepton tlepton1 = cutBased_leptons->at(0);
    ttH::Lepton tlepton2 = cutBased_leptons->at(1);
    
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
      //if (i == 2) cout << " higgs decay: " << histo->GetBinContent(i) << " +/- " << histo->GetBinError(i) << endl;
      if (i == 3) cout << " 2+ preselected leptons: " << histo->GetBinContent(i) << " +/- " << histo->GetBinError(i) << endl;
      //if (i == 4) cout << " decay to " << label << ": " << histo->GetBinContent(i) << " +/- " << histo->GetBinError(i) << endl;
      if (i == 5) cout << " SS: " << histo->GetBinContent(i) << " +/- " << histo->GetBinError(i) << endl;
      if (i == 6) cout << " pt > 20,20: " << histo->GetBinContent(i) << " +/- " << histo->GetBinError(i) << endl;
      if (i == 7) cout << " 2 tight: " << histo->GetBinContent(i) << " +/- " << histo->GetBinError(i) << endl;
      //if (i == 8) cout << " 2 tight corresponding with the presel: " << histo->GetBinContent(i) << " +/- " << histo->GetBinError(i) << endl;
   }
    cout << "------------------------------------------" << endl;



  }
  f_var.Write();
  f_var.Close();

  

}
