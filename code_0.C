#include "TFile.h"
#include "TTree.h"
#include "TLorentzVector.h"
#include "objectClasses.h"


void code_0(int nsel=0, bool silent=0){


  
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

  
  vector<ttH::GenParticle> *pruned_genParticles= 0;
  
  
  TBranch *b_mcwgt;   //!
  TBranch *b_wgt;   //!
  TBranch *b_wallTimePerEvent;   //!
  TBranch *b_eventnum;   //!
  TBranch *b_lumiBlock;   //!
  TBranch *b_runNumber;   //!
  TBranch *b_higgs_decay;   //!
 
  
  TBranch *b_pruned_genParticles = 0;
 
  tree->SetBranchAddress("mcwgt", &mcwgt, &b_mcwgt);
  tree->SetBranchAddress("wgt", &wgt, &b_wgt);
  tree->SetBranchAddress("wallTimePerEvent", &wallTimePerEvent, &b_wallTimePerEvent);
  tree->SetBranchAddress("eventnum", &eventnum, &b_eventnum);
  tree->SetBranchAddress("lumiBlock", &lumiBlock, &b_lumiBlock);
  tree->SetBranchAddress("runNumber", &runNumber, &b_runNumber);
  tree->SetBranchAddress("higgs_decay", &higgs_decay, &b_higgs_decay);
  tree->SetBranchAddress("pruned_genParticles", &pruned_genParticles, &b_pruned_genParticles);
  ////
  
  
  cout <<"[Info:] You are running GEN code over " << plotName << endl;
  char newRootFile[300];
  sprintf(newRootFile,"results/out_gen_100k.root");
  TFile f_var(newRootFile, "RECREATE");
  if(!silent){
    std::cout << "[Info:] results root file named " << newRootFile << std::endl;
  }
  
  // Histos
  char title[300];
  sprintf(title,"cuts_%s", plotName);
  TH1F* histo = new TH1F( title, "Cut Flow", 20, 0, 20 );
  histo->Sumw2();
 
  sprintf(title,"deltaR_qq_%s", plotName);
  TH1F* histo_dr_hwwqq = new TH1F( title, "#Delta R between HWW LF quarks", 100, 0, 5 );
  histo_dr_hwwqq->Sumw2();
  
  sprintf(title,"deltaR_qqs_%s", plotName);
  TH1F* histo_dr_hwwqqs = new TH1F( title, "#Delta R between HWW lepton and quarks system", 100, 0, 5 );
  histo_dr_hwwqqs->Sumw2();
 
  sprintf(title,"deltaR_lq_%s", plotName);
  TH1F* histo_dr_hwwlq = new TH1F( title, "#Delta R between HWW lepton and closest HWW quark", 100, 0, 5 );
  histo_dr_hwwlq->Sumw2();
 
  sprintf(title,"lepton_pt_%s", plotName);
  TH1F* histo_lepton_pt = new TH1F( title, "P_{T} of the lepton", 200, 0, 200);
  histo_lepton_pt->Sumw2();
 
  sprintf(title,"Higgs_pt_%s", plotName);
  TH1F* histo_higgs_pt = new TH1F( title, "P_{T} of the Higgs", 500, 0, 500);
  histo_higgs_pt->Sumw2();
 
  sprintf(title,"deltaR_Hpt_%s", plotName);
  TH2F* histo_dr_higgs_pt = new TH2F( title, "#DeltaR Vs P_{T} of the Higgs", 100, 0, 5, 500, 0, 500);
  histo_dr_higgs_pt->Sumw2();
 
  sprintf(title,"deltaR_lpt_%s", plotName);
  TH2F* histo_dr_lep_pt = new TH2F( title, "#DeltaR Vs P_{T} of the lepton", 100, 0, 5, 500, 0, 500);
  histo_dr_lep_pt->Sumw2();
 
  


  
 
  double weight = 1;
  int nused = 0;
  int nHWW = 0;
  int nHZZ = 0;
  int dRcuts[4] = {0, 0, 0, 0};
  
  if (!silent) cout << "[Info:] Number of raw events: " << tree->GetEntries() << endl;
  // loop over events 
  for(int iEvent = 0; iEvent < 100000; iEvent++){
   // for(int iEvent = 0; iEvent < tree->GetEntries(); iEvent++){
    Long64_t tentry = tree->LoadTree(iEvent);
    
    //Point to the proper entry
    b_higgs_decay->GetEntry(tentry);
    b_pruned_genParticles->GetEntry(tentry);
  
    nused++; 

    histo->Fill(0., weight);
    
    if (nsel ==0 && !higgs_decay) continue;
    histo->Fill(1., weight);
       
       
    int indexes[8] = { -1, -1, -1, -1, -1, -1, -1, -1}; //Main indexes for using later lH, lt, btl, btqq, q1t, q2t, q1H, q2H   
       
    bool HWW = false; 
    int nH = 0;
    int indexH = -1;
    int topindex[2] = {-1, -1}; //top, antitop
    for (int i = 0; i < pruned_genParticles->size(); i++){
      ttH::GenParticle genpar = pruned_genParticles->at(i);
      if (genpar.pdgID == 25 && genpar.status == 62){
        indexH = i;
        if (genpar.child0 != 9999 && genpar.child1 != 9999){
          ttH::GenParticle child_1 = pruned_genParticles->at(genpar.child0);
	  ttH::GenParticle child_2 = pruned_genParticles->at(genpar.child1);
	  if (abs(child_1.pdgID) == abs(child_2.pdgID) && abs(child_1.pdgID) == 24) HWW = true;
	}
      } 
      else if (genpar.pdgID == 6 && genpar.status == 62) topindex[0] = i;
      else if (genpar.pdgID == -6 && genpar.status == 62) topindex[1] = i;
    } 
    if (nsel ==0 && !HWW) continue;
    histo->Fill(2., weight);
    nHWW++;
    
    //selecting full legacy completed kids0
    if ((pruned_genParticles->at(indexH)).child0 == 9999) continue;
    if ((pruned_genParticles->at(indexH)).child1 == 9999) continue;
    histo->Fill(3., weight);
    
    //selecting full legacy completed grandkids
    if ((pruned_genParticles->at((pruned_genParticles->at(indexH)).child0)).child0 == 9999) continue;   
    if ((pruned_genParticles->at((pruned_genParticles->at(indexH)).child0)).child1 == 9999) continue;
    if ((pruned_genParticles->at((pruned_genParticles->at(indexH)).child1)).child0 == 9999) continue;
    if ((pruned_genParticles->at((pruned_genParticles->at(indexH)).child1)).child1 == 9999) continue;
    histo->Fill(4., weight);
    
 
    //selecting semileptonic HWW
    int nleptons = 0;
    int kid0 = (pruned_genParticles->at((pruned_genParticles->at(indexH)).child0)).child0;
    int kid1 = (pruned_genParticles->at((pruned_genParticles->at(indexH)).child0)).child1;
    int kid2 = (pruned_genParticles->at((pruned_genParticles->at(indexH)).child1)).child0;
    int kid3 = (pruned_genParticles->at((pruned_genParticles->at(indexH)).child1)).child1;
    if (abs((pruned_genParticles->at(kid0)).pdgID) == 11 || abs((pruned_genParticles->at(kid0)).pdgID) == 13) nleptons++; 
    if (abs((pruned_genParticles->at(kid1)).pdgID) == 11 || abs((pruned_genParticles->at(kid1)).pdgID) == 13) nleptons++; 
    if (abs((pruned_genParticles->at(kid2)).pdgID) == 11 || abs((pruned_genParticles->at(kid2)).pdgID) == 13) nleptons++; 
    if (abs((pruned_genParticles->at(kid3)).pdgID) == 11 || abs((pruned_genParticles->at(kid3)).pdgID) == 13) nleptons++; 
    
    if (nleptons != 1) continue;
   
    
    int indexlepton = -1; 
    if (abs((pruned_genParticles->at(kid0)).pdgID) == 11 || abs((pruned_genParticles->at(kid0)).pdgID) == 13){indexlepton=kid0;}
    if (abs((pruned_genParticles->at(kid1)).pdgID) == 11 || abs((pruned_genParticles->at(kid1)).pdgID) == 13){indexlepton=kid1;}
    if (abs((pruned_genParticles->at(kid2)).pdgID) == 11 || abs((pruned_genParticles->at(kid2)).pdgID) == 13){indexlepton=kid2;}
    if (abs((pruned_genParticles->at(kid3)).pdgID) == 11 || abs((pruned_genParticles->at(kid3)).pdgID) == 13){indexlepton=kid3;}
    int indexq[2] = {-1, -1}; 
    if (abs((pruned_genParticles->at(kid0)).pdgID) < 5 && abs((pruned_genParticles->at(kid1)).pdgID) < 5){indexq[0]=kid0; indexq[1]=kid1;}
    if (abs((pruned_genParticles->at(kid2)).pdgID) < 5 && abs((pruned_genParticles->at(kid3)).pdgID) < 5){indexq[0]=kid2; indexq[1]=kid3;}
    
    if (abs((pruned_genParticles->at(kid0)).pdgID) == 15 || abs((pruned_genParticles->at(kid0)).pdgID) == 17) continue; 
    if (abs((pruned_genParticles->at(kid1)).pdgID) == 15 || abs((pruned_genParticles->at(kid1)).pdgID) == 17) continue;
    if (abs((pruned_genParticles->at(kid2)).pdgID) == 15 || abs((pruned_genParticles->at(kid2)).pdgID) == 17) continue;
    if (abs((pruned_genParticles->at(kid3)).pdgID) == 15 || abs((pruned_genParticles->at(kid3)).pdgID) == 17) continue;

    if (indexq[0] == -1 || indexq[1] == -1) continue; 
    
    histo->Fill(5., weight);
    ttH::GenParticle lep1 = pruned_genParticles->at(indexlepton);
    ttH::GenParticle qw1 = pruned_genParticles->at(indexq[0]);
    ttH::GenParticle qw2 = pruned_genParticles->at(indexq[1]);
    TVector3 vlep1(lep1.tlv().Px(), lep1.tlv().Py(), lep1.tlv().Pz());
    TVector3 vqw1(qw1.tlv().Px(), qw1.tlv().Py(), qw1.tlv().Pz());
    TVector3 vqw2(qw2.tlv().Px(), qw2.tlv().Py(), qw2.tlv().Pz());
    TVector3 vqq = vqw1+vqw2;
    
    float mindr = TMath::Min(vqw1.DeltaR(vlep1),vqw2.DeltaR(vlep1));
   
    // Filling histos
    histo_dr_hwwqq->Fill(vqw1.DeltaR(vqw2), weight);
    histo_dr_hwwlq->Fill(TMath::Min(vqw1.DeltaR(vlep1),vqw2.DeltaR(vlep1)), weight);
    histo_lepton_pt->Fill(vlep1.Pt(), weight);
    histo_higgs_pt->Fill((pruned_genParticles->at(indexH)).tlv().Pt(), weight);
    histo_dr_higgs_pt->Fill(mindr,(pruned_genParticles->at(indexH)).tlv().Pt(), weight);
    histo_dr_lep_pt->Fill(mindr,vlep1.Pt(), weight);
    histo_dr_hwwqqs->Fill(vqq.DeltaR(vlep1), weight); 


    if (mindr <= 0.3) dRcuts[0]=dRcuts[0]+weight;
    
    if (vlep1.Pt() < 10) continue;
    histo->Fill(6., weight); 
    if (mindr <= 0.3) dRcuts[1]=dRcuts[1]+weight;
    
    if (vlep1.Pt() < 20 ) continue;
    histo->Fill(7., weight); 

    if (mindr <= 0.3) dRcuts[2]=dRcuts[2]+weight;
     
   // cout << iEvent << " done" << endl;
    
  }
  
  

  
  if (!silent){
    cout << "------------------------------------------" << endl;
    cout << "[Results:] GEN only " << endl;
    cout << "------------------------------------------" << endl;
    for (int i = 1; i < 12; i++){
      if (i == 1) cout << " all: " << histo->GetBinContent(i) << " +/- " << histo->GetBinError(i) << endl;
      if (i == 2) cout << " higgs decay: " << histo->GetBinContent(i) << " +/- " << histo->GetBinError(i) << endl;
      if (i == 3) cout << " HWW : " << histo->GetBinContent(i) << " +/- " << histo->GetBinError(i) << endl;
      if (i == 4) cout << " H children present: " << histo->GetBinContent(i) << " +/- " << histo->GetBinError(i) << endl;
      if (i == 5) cout << " H grandchildren: " << histo->GetBinContent(i) << " +/- " << histo->GetBinError(i) << endl;
      if (i == 6) cout << " HWW semileptonic: " << histo->GetBinContent(i) << " +/- " << histo->GetBinError(i) << endl;
      if (i == 6) cout << " ----> from which " << dRcuts[0] << "  (" << dRcuts[0]*100/histo->GetBinContent(i) << "%) have DR <= 0.3 " << endl;
      if (i == 7) cout << " Pt lepton > 10: " << histo->GetBinContent(i) << " +/- " << histo->GetBinError(i) << endl;
      if (i == 7) cout << " ----> from which " << dRcuts[1] << "  (" << dRcuts[1]*100/histo->GetBinContent(i) << "%) have DR <= 0.3 " << endl;
      if (i == 8) cout << " Pt lepton > 20:" << histo->GetBinContent(i) << " +/- " << histo->GetBinError(i) << endl;
      if (i == 8) cout << " ----> from which " << dRcuts[2] << "  (" << dRcuts[2]*100/histo->GetBinContent(i) << "%) have DR <= 0.3 " << endl;
	 
    }
    cout << "------------------------------------------" << endl;
    cout << "[Info:]" << nHWW*100/nused << "% of HWW in the events" << endl;
  



  }
  f_var.Write();
  f_var.Close();

  

}
