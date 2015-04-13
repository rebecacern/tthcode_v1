#include "TFile.h"
#include "TTree.h"
#include "TLorentzVector.h"
#include "objectClasses.h"


void code_0(int nsel=0, int mode=0, bool silent=0){


  
  char plotName[300];
  sprintf(plotName,"test");
  if (nsel == 0) {sprintf(plotName,"ttH125");}
  if (nsel == 1) {sprintf(plotName,"TTJets");}
  
  
  char myRootFile[300];
  sprintf(myRootFile,"../tuples/%s.root", plotName);

  TFile *fin = new TFile(myRootFile);

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

  vector<ttH::Electron> *tightMvaBased_electrons = 0;
  vector<ttH::Muon> *tightMvaBased_muons = 0;
  vector<ttH::Lepton> *tightMvaBased_leptons = 0;
  vector<ttH::Jet> *tight_bJets= 0;
  
  vector<ttH::MET> *met = 0;
  
  vector<ttH::GenParticle> *pruned_genParticles= 0;
  
  
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
  
  TBranch *b_tightMvaBased_electrons = 0;
  TBranch *b_tightMvaBased_muons = 0;
  TBranch *b_tightMvaBased_leptons = 0;
  TBranch *b_tight_bJets= 0;
  TBranch *b_met = 0;
  
  TBranch *b_pruned_genParticles = 0;
 
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
  tree->SetBranchAddress("tightMvaBased_electrons", &tightMvaBased_electrons, &b_tightMvaBased_electrons);
  tree->SetBranchAddress("tightMvaBased_muons", &tightMvaBased_muons, &b_tightMvaBased_muons);
  tree->SetBranchAddress("tightMvaBased_leptons", &tightMvaBased_leptons, &b_tightMvaBased_leptons);
  tree->SetBranchAddress("tight_bJets", &tight_bJets, &b_tight_bJets);
  tree->SetBranchAddress("met", &met, &b_met);
  tree->SetBranchAddress("pruned_genParticles", &pruned_genParticles, &b_pruned_genParticles);
  ////
  
  
  cout <<"[Info:] You are running GEN code over " << plotName << endl;
  if (mode != 0 && mode !=1 && mode !=2) mode = 0;
  char newRootFile[300];
  sprintf(newRootFile,"results/output_gen.root");
  TFile f_var(newRootFile, "RECREATE");
  if(!silent){
    std::cout << "results root file named " << newRootFile << std::endl;
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
  TH1F* histo_nbjets = new TH1F( title, "Number of b-jets pt > 20", 10, 0, 10 );
  histo_nbjets->Sumw2();
  
  sprintf(title,"nleptons_%s", plotName);;
  TH1F* histo_nleptons = new TH1F( title, "Number of leptons pt > 10", 20, 0, 20 );
  histo_nleptons->Sumw2();
 
  sprintf(title,"met_%s", plotName);
  TH1F* histo_met = new TH1F( title, "Missing ET", 100, 0, 200 );
  histo_met->Sumw2();
 
  sprintf(title,"deltaR_%s", plotName);
  TH1F* histo_dr = new TH1F( title, "#Delta R between SS leptons", 100, -5, 5 );
  histo_dr->Sumw2();
 
  sprintf(title,"deltaR_qq_%s", plotName);
  TH1F* histo_dr_hwwqq = new TH1F( title, "#Delta R between HWW LF quarks", 100, -5, 5 );
  histo_dr_hwwqq->Sumw2();
 
  sprintf(title,"deltaR_lq_%s", plotName);
  TH1F* histo_dr_hwwlq = new TH1F( title, "#Delta R between HWW lepton and closest quark", 100, -5, 5 );
  histo_dr_hwwlq->Sumw2();

  
 
  double weight = 1;
  int nused = 0;
  int nHWW = 0;
  int nHZZ = 0;
  if (!silent) cout << "[Info:] Number of raw events: " << tree->GetEntries() << endl;
  // loop over events 
  //for(int iEvent = 0; iEvent < 1000; iEvent++){
  for(int iEvent = 0; iEvent < tree->GetEntries(); iEvent++){
    Long64_t tentry = tree->LoadTree(iEvent);
    //Point to the proper entry
    b_higgs_decay->GetEntry(tentry);
    b_pruned_genParticles->GetEntry(tentry);
     
     
    /*  b_preselected_electrons->GetEntry(tentry);
	b_preselected_muons->GetEntry(tentry);
	b_preselected_leptons->GetEntry(tentry);
	b_preselected_jets->GetEntry(tentry);
   
	b_tightMvaBased_electrons->GetEntry(tentry);
	b_tightMvaBased_muons->GetEntry(tentry);
	b_tightMvaBased_leptons->GetEntry(tentry);
	b_tight_bJets->GetEntry(tentry);
	b_met->GetEntry(tentry);*/
   
    
    
    nused++; 

    histo->Fill(0., weight);
    
    if (nsel ==0 && !higgs_decay) continue;
    histo->Fill(1., weight);
    
    bool HWW = false;
    bool HZZ = false;
    int n_W_plus = 0;
    int n_W_minus = 0;
    int n_Z = 0;
    bool HW_plus_lepton = false;
    bool HW_minus_lepton = false; 
    int n_tW_plus = 0;
    int n_tW_minus = 0;
    bool tW_plus_lepton = false;
    bool tW_minus_lepton = false;
    int indexes[8] = { -1, -1, -1, -1, -1, -1, -1, -1}; //lH, lt, btl, btqq, q1t, q2t, q1H, q2H
   
    for (int i = 0; i < pruned_genParticles->size(); i++){
      ttH::GenParticle genpar = pruned_genParticles->at(i);
      if (genpar.pdgID == 24){
        if (genpar.mother !=9999){
	  ttH::GenParticle mamapar = pruned_genParticles->at(genpar.mother);
	  if (mamapar.pdgID == 25){
	    n_W_plus++;  
	    if (genpar.child0 != 9999 && genpar.child1 != 9999) {
	      ttH::GenParticle child_1 = pruned_genParticles->at(genpar.child0);
	      ttH::GenParticle child_2 = pruned_genParticles->at(genpar.child1);
	      if (abs(child_1.pdgID) == 11 || abs(child_1.pdgID) == 13) { HW_plus_lepton = true; indexes[0] = genpar.child0;}
	      else if (abs(child_2.pdgID) == 11 || abs(child_2.pdgID) == 13){ HW_plus_lepton = true; indexes[0] = genpar.child1;}
	      else {indexes[6] = genpar.child0; indexes[7] = genpar.child1;}
	    }
	  } else if (mamapar.pdgID == 6){
	    n_tW_plus++;  
	    if (genpar.child0 != 9999 && genpar.child1 != 9999) {
	      ttH::GenParticle child_1 = pruned_genParticles->at(genpar.child0);
	      ttH::GenParticle child_2 = pruned_genParticles->at(genpar.child1);
	      if (abs(child_1.pdgID) == 11 || abs(child_1.pdgID) == 13) {tW_plus_lepton = true; indexes[1] = genpar.child0; indexes[2]=genpar.mother;}
	      else if (abs(child_2.pdgID) == 11 || abs(child_2.pdgID) == 13) {tW_plus_lepton = true; indexes[1] = genpar.child1; indexes[2]=genpar.mother;}
	      else {indexes[4] = genpar.child0; indexes[5] = genpar.child1; indexes[3]=genpar.mother;}
	    }
	  } 
	}
      }
      
      if (genpar.pdgID == -24){
        if (genpar.mother !=9999){
	  ttH::GenParticle mamapar = pruned_genParticles->at(genpar.mother);
	  if (mamapar.pdgID == 25){
	    n_W_minus++; 
	    if (genpar.child0 != 9999 && genpar.child1 != 9999) {
	      ttH::GenParticle child_1 = pruned_genParticles->at(genpar.child0);
	      ttH::GenParticle child_2 = pruned_genParticles->at(genpar.child1);
	      if (abs(child_1.pdgID) == 11 || abs(child_1.pdgID) == 13) {HW_minus_lepton = true; indexes[0] = genpar.child0;}
	      if (abs(child_2.pdgID) == 11 || abs(child_2.pdgID) == 13) {HW_minus_lepton = true; indexes[0] = genpar.child1;}
	      else {indexes[6] = genpar.child0; indexes[7] = genpar.child1;}
	    }
	  } else if (mamapar.pdgID == -6){
	    n_tW_minus++; 
	    if (genpar.child0 != 9999 && genpar.child1 != 9999) {
	      ttH::GenParticle child_1 = pruned_genParticles->at(genpar.child0);
	      ttH::GenParticle child_2 = pruned_genParticles->at(genpar.child1);
	      if (abs(child_1.pdgID) == 11 || abs(child_1.pdgID) == 13) {tW_minus_lepton = true; indexes[1] = genpar.child0; indexes[2]=genpar.mother;}
	      else if (abs(child_2.pdgID) == 11 || abs(child_2.pdgID) == 13) {tW_minus_lepton = true; indexes[1] = genpar.child1; indexes[2]=genpar.mother;}
	      else {indexes[4] = genpar.child0; indexes[5] = genpar.child1; indexes[3]=genpar.mother;}
	    }
	  }
	}
      }
      if (genpar.pdgID == 23){
        if (genpar.mother !=9999){
	  ttH::GenParticle mamapar = pruned_genParticles->at(genpar.mother);
	  if (mamapar.pdgID == 25)  n_Z++; 
	}
      }
      
    }
    if (nsel ==0 && n_W_plus == 1 && n_W_minus == 1) HWW = true;
    else if (nsel ==0 && n_Z == 2) HZZ = true;  
   
    if (nsel ==0 && HWW && HZZ) cout << "[Info:] You are doing something wrong, you cannot have HWW and HZZ in the same event" << endl;
    
    
    if (nsel ==0 && HWW) nHWW++;
    else if (nsel ==0 && HZZ) nHZZ++;

    if (nsel ==0 && !HWW) continue;
    histo->Fill(2., weight);
    if (nsel == 0 && ((!HW_minus_lepton && !HW_plus_lepton) || (HW_minus_lepton && HW_plus_lepton))) continue;
    histo->Fill(3., weight);
    
    if ((!tW_minus_lepton && !tW_plus_lepton) || (tW_minus_lepton && tW_plus_lepton)) continue;
    
    histo->Fill(4., weight);
   
    ttH::GenParticle lep1 = pruned_genParticles->at(indexes[0]);
    ttH::GenParticle lep2 = pruned_genParticles->at(indexes[1]);

    if (lep1.pdgID*lep2.pdgID < 1) continue;
    histo->Fill(5., weight);
   
    bool index_prop = true;
    for (int i =0; i<8; i++){
      if (indexes [i] == -1) index_prop = false;}
    if (!index_prop) continue;
   
    histo->Fill(6., weight);

    TVector3 vlep1(lep1.tlv().Px(), lep1.tlv().Py(), lep1.tlv().Pz());
    TVector3 vlep2(lep2.tlv().Px(), lep2.tlv().Py(), lep2.tlv().Pz());
    ttH::GenParticle q1 = pruned_genParticles->at(indexes[6]);
    ttH::GenParticle q2 = pruned_genParticles->at(indexes[7]);
    TVector3 vq1(q1.tlv().Px(), q1.tlv().Py(), q1.tlv().Pz());
    TVector3 vq2(q2.tlv().Px(), q2.tlv().Py(), q2.tlv().Pz());

    histo_dr->Fill(vlep1.DeltaR(vlep2), weight);
    histo_dr_hwwqq->Fill(vq1.DeltaR(vq2), weight);
    histo_dr_hwwlq->Fill(TMath::Min(vq1.DeltaR(vlep1),vq2.DeltaR(vlep1)), weight);
    
    /*
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
    */
  
    
  }
  
  
  
  char label[300];
  sprintf(label, "emu");
  if (mode == 1) sprintf(label,"mumu");
  if (mode == 2) sprintf(label,"ee");
  
 
 
  
  if (!silent){
    cout << "------------------------------------------" << endl;
    cout << "[Results:] GEN only " << label << endl;
    cout << "------------------------------------------" << endl;
    for (int i = 1; i < 9; i++){
      if (i == 1) cout << " all: " << histo->GetBinContent(i) << " +/- " << histo->GetBinError(i) << endl;
      if (i == 2) cout << " higgs decay: " << histo->GetBinContent(i) << " +/- " << histo->GetBinError(i) << endl;
      if (i == 3) cout << " HWW : " << histo->GetBinContent(i) << " +/- " << histo->GetBinError(i) << endl;
      if (i == 4) cout << " Semileptonic HWW: " << label << ": " << histo->GetBinContent(i) << " +/- " << histo->GetBinError(i) << endl;
      if (i == 5) cout << " Semileptonic tt: " << histo->GetBinContent(i) << " +/- " << histo->GetBinError(i) << endl;
      if (i == 6) cout << " SS: " << histo->GetBinContent(i) << " +/- " << histo->GetBinError(i) << endl;
      if (i == 7) cout << " proper index: " << histo->GetBinContent(i) << " +/- " << histo->GetBinError(i) << endl;
      if (i == 8) cout << " 2 tight corresponding with the presel: " << histo->GetBinContent(i) << " +/- " << histo->GetBinError(i) << endl;
    }
    cout << nHWW*100/nused << "% of HWW, " << nHZZ*100/nused << "% of HZZ in the events" << endl;
    cout << "------------------------------------------" << endl;



  }
  f_var.Write();
  f_var.Close();

  

}