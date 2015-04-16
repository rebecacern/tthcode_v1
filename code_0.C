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
  sprintf(newRootFile,"results/output_gen_nocuts.root");
  TFile f_var(newRootFile, "RECREATE");
  if(!silent){
    std::cout << "[Info:] results root file named " << newRootFile << std::endl;
  }
  
  // Histos
  char title[300];
  sprintf(title,"cuts_%s", plotName);
  TH1F* histo = new TH1F( title, "Cut Flow", 20, 0, 20 );
  histo->Sumw2();
 
  sprintf(title,"deltaR_%s", plotName);
  TH1F* histo_dr = new TH1F( title, "#Delta R between SS leptons", 100, 0, 5 );
  histo_dr->Sumw2();
 
  sprintf(title,"deltaR_qq_%s", plotName);
  TH1F* histo_dr_hwwqq = new TH1F( title, "#Delta R between HWW LF quarks", 100, 0, 5 );
  histo_dr_hwwqq->Sumw2();
 
  sprintf(title,"deltaR_lq_%s", plotName);
  TH1F* histo_dr_hwwlq = new TH1F( title, "#Delta R between HWW lepton and closest HWW quark", 100, 0, 5 );
  histo_dr_hwwlq->Sumw2();
 
  sprintf(title,"deltaR_l_q_%s", plotName);
  TH1F* histo_dr_hwwl_q = new TH1F( title, "#Delta R between HWW lepton and closest light quark", 100, 0, 5 );
  histo_dr_hwwl_q->Sumw2();
 
  sprintf(title,"deltaR_toplq_%s", plotName);
  TH1F* histo_dr_toplq = new TH1F( title, "#Delta R between top lepton and closest top light quark", 100, 0, 5 );
  histo_dr_toplq->Sumw2();
 
  sprintf(title,"deltaR_topl_q_%s", plotName);
  TH1F* histo_dr_topl_q = new TH1F( title, "#Delta R between top lepton and closest light quark", 100, 0, 5 );
  histo_dr_topl_q->Sumw2();
 
  sprintf(title,"deltaR_hwwlb_%s", plotName);
  TH1F* histo_dr_hwwlb = new TH1F( title, "#Delta R between HWW lepton and closest b quark", 100, 0, 5 );
  histo_dr_hwwlb->Sumw2();
 
  sprintf(title,"deltaR_toplb_%s", plotName);
  TH1F* histo_dr_toplb = new TH1F( title, "#Delta R between top lepton and its b quark", 100, 0, 5 );
  histo_dr_toplb->Sumw2();



  
 
  double weight = 1;
  int nused = 0;
  int nHWW = 0;
  int nHZZ = 0;
  if (!silent) cout << "[Info:] Number of raw events: " << tree->GetEntries() << endl;
  // loop over events 
  for(int iEvent = 0; iEvent < 10000; iEvent++){
    //for(int iEvent = 0; iEvent < tree->GetEntries(); iEvent++){
    Long64_t tentry = tree->LoadTree(iEvent);
    //Point to the proper entry
    b_higgs_decay->GetEntry(tentry);
    b_pruned_genParticles->GetEntry(tentry);
  
    nused++; 

    histo->Fill(0., weight);
    
    if (nsel ==0 && !higgs_decay) continue;
    histo->Fill(1., weight);
       
    int indexes[8] = { -1, -1, -1, -1, -1, -1, -1, -1}; //Main indexes for using later lH, lt, btl, btqq, q1t, q2t, q1H, q2H   
       
    // Checking the HWW decay and the top and atitop index   
    bool HWW = false; 
    int indexH = -1;
    int topindex[2] = {-1, -1};
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
    
    // Selecting semileptonic decay of the Higgs 
    int nHleptons = 0;
    ttH::GenParticle Hpar = pruned_genParticles->at(indexH);
    ttH::GenParticle W_1 = pruned_genParticles->at(Hpar.child0);
    ttH::GenParticle W_2 = pruned_genParticles->at(Hpar.child1);
    
    if (W_1.child0 != 9999 && W_1.child1 != 9999) {
      ttH::GenParticle child_1 = pruned_genParticles->at(W_1.child0);
      ttH::GenParticle child_2 = pruned_genParticles->at(W_1.child1);
      if (abs(child_1.pdgID) == 11 || abs(child_1.pdgID) == 13){ nHleptons++; indexes[0]=W_1.child0;}
      else if (abs(child_2.pdgID) == 11 || abs(child_2.pdgID) == 13){ nHleptons++; indexes[0]=W_1.child1;}
    }
    if (W_2.child0 != 9999 && W_2.child1 != 9999) {
      ttH::GenParticle child_1 = pruned_genParticles->at(W_2.child0);
      ttH::GenParticle child_2 = pruned_genParticles->at(W_2.child1);
      if (abs(child_1.pdgID) == 11 || abs(child_1.pdgID) == 13){ nHleptons++;; indexes[0]=W_2.child0;}
      else if (abs(child_2.pdgID) == 11 || abs(child_2.pdgID) == 13) {nHleptons++;indexes[0]=W_2.child1;}
    }
    
    if (nHleptons != 1) continue;
    histo->Fill(3., weight);
   
    // Checking decay products of the top quarks  
    int indext[4] = {-1, -1, -1, -1}; // tW tb atW atb
    ttH::GenParticle t_1 = pruned_genParticles->at(topindex[0]);
    ttH::GenParticle t_2 = pruned_genParticles->at(topindex[1]);
    
    if (t_1.child0 != 9999 && t_1.child1 != 9999) {
      ttH::GenParticle child_1 = pruned_genParticles->at(t_1.child0);
      ttH::GenParticle child_2 = pruned_genParticles->at(t_1.child1);
      if (abs(child_1.pdgID) == 24 && abs(child_2.pdgID) == 5){
	indext[0] =  t_1.child0;
	indext[1] =  t_1.child1;
      } else if (abs(child_2.pdgID) == 24 && abs(child_1.pdgID) == 5){
	indext[0] =  t_1.child1;
	indext[1] =  t_1.child0;
      } 
    } 
    if (t_2.child0 != 9999 && t_2.child1 != 9999) {
      ttH::GenParticle child_1 = pruned_genParticles->at(t_2.child0);
      ttH::GenParticle child_2 = pruned_genParticles->at(t_2.child1);
      if (abs(child_1.pdgID) == 24 && abs(child_2.pdgID) == 5){
	indext[2] =  t_2.child0;
	indext[3] =  t_2.child1;
      } else if (abs(child_2.pdgID) == 24 && abs(child_1.pdgID) == 5){
	indext[2] =  t_2.child1;
	indext[3] =  t_2.child0;
      }
    }
  
    bool good = true;
    for (int i=0; i<4; i++){ if (indext[i] == -1) good = false; }
    if (!good) continue;
    histo->Fill(4., weight);
    
    
    int ntopleptons = 0;
    for (int i = 0; i < pruned_genParticles->size(); i++){
      ttH::GenParticle genpar = pruned_genParticles->at(i);
      if (abs(genpar.pdgID) == 13 || genpar.status == 11){
        if (i !=  indexes[0]) {
	  if (genpar.mother != 9999){
	    ttH::GenParticle mompar = pruned_genParticles->at(genpar.mother);
	    if  (abs(mompar.pdgID) == 24){
	      if (mompar.mother != 9999){
		ttH::GenParticle granpar = pruned_genParticles->at(mompar.mother);
		if (abs(granpar.pdgID) == 6 && genpar.status == 23) {
		  ntopleptons++; 
		  indexes[1] = i;
		  indexes[2] = mompar.mother;
		}
              } 
	    }
	  }
	}
      }
    } 
    if (ntopleptons >  2) cout << "[Info:] Something fishy" << endl;
    
    if (ntopleptons != 1) continue;
    histo->Fill(5., weight);
    
    //lepton from the Higgs
    int indexl = 0;
    int counter = 0;
    for (int i = 0; i < pruned_genParticles->size(); i++){
      ttH::GenParticle genpar = pruned_genParticles->at(i);
      if (i !=  indexes[1]) {
        if (abs(genpar.pdgID) == 13 || genpar.status == 11){
	  if (genpar.mother != 9999){
	    ttH::GenParticle mompar = pruned_genParticles->at(genpar.mother);
	      if (mompar.mother != 9999){
		ttH::GenParticle granpar = pruned_genParticles->at(mompar.mother);
		cout << iEvent << ": "<<  i <<" - " << granpar.pdgID << " - " << mompar.pdgID << " - " << genpar.pdgID <<  " status: " << genpar.status << endl;
		if (abs(granpar.pdgID) == 25 && genpar.status == 1) {
		  counter ++;
		  indexl = i;
		}
              
	    }
	  }
	}
      }
    } 
    cout << counter << " ---> " << indexl << " - " << indexes[0] << endl;
    ttH::GenParticle genpar = pruned_genParticles->at(indexl);
    if (genpar.mother != 9999) {
      ttH::GenParticle mompar = pruned_genParticles->at(genpar.mother);
      ttH::GenParticle granpar = pruned_genParticles->at(mompar.mother);
      ttH::GenParticle son1 = pruned_genParticles->at(granpar.child0);
      if (indexl == granpar.child1) son1 = pruned_genParticles->at(granpar.child1);
      if (son1.child0 !=9999 && son1.child1 != 9999){
      ttH::GenParticle grandson1 = pruned_genParticles->at(son1.child0);
      ttH::GenParticle grandson2 = pruned_genParticles->at(son1.child1);
      
      cout << grandson1.pdgID  << " - " << grandson2.pdgID <<  endl;
      }
    }
    for (int i =0; i<8; i++){
    cout << indexes[i] << " " ;
    }
    cout << endl;
    
    ttH::GenParticle lep1 = pruned_genParticles->at(indexes[0]);
    ttH::GenParticle lep2 = pruned_genParticles->at(indexes[1]);
    
    if (lep1.pdgID*lep2.pdgID < 1) continue;
    histo->Fill(6., weight);
	
	cout << endl;
	/*bool index_prop = true;
	for (int i =0; i<8; i++){
	cout << indexes[i] << " " ;
	if (indexes [i] == -1) index_prop = false;}
	if (!index_prop) continue;
	cout << endl;
	histo->Fill(6., weight);
    */

    /*   
	 cout << iEvent << endl;
	 for (int i =0; i<8; i++){
	 ttH::GenParticle q = pruned_genParticles->at(indexes[i]);
	 cout << " - " << q.pdgID ;
	 }
	 cout << endl;  
    */
   
    
    /*
    //leptons
    TVector3 vlep1(lep1.tlv().Px(), lep1.tlv().Py(), lep1.tlv().Pz());
    TVector3 vlep2(lep2.tlv().Px(), lep2.tlv().Py(), lep2.tlv().Pz());
    
    //quarks
    ttH::GenParticle qw1 = pruned_genParticles->at(indexes[6]);
    ttH::GenParticle qw2 = pruned_genParticles->at(indexes[7]);
    TVector3 vqw1(qw1.tlv().Px(), qw1.tlv().Py(), qw1.tlv().Pz());
    TVector3 vqw2(qw2.tlv().Px(), qw2.tlv().Py(), qw2.tlv().Pz());
    
    ttH::GenParticle qt1 = pruned_genParticles->at(indexes[4]);
    ttH::GenParticle qt2 = pruned_genParticles->at(indexes[5]);
    TVector3 vqt1(qt1.tlv().Px(), qt1.tlv().Py(), qt1.tlv().Pz());
    TVector3 vqt2(qt2.tlv().Px(), qt2.tlv().Py(), qt2.tlv().Pz());


    float mindr = TMath::Min(vqw1.DeltaR(vlep1),vqw2.DeltaR(vlep1));
    mindr = TMath::Min(mindr,vqt1.DeltaR(vlep1));
    mindr = TMath::Min(mindr,vqt2.DeltaR(vlep1));
    
    float mindr_t = TMath::Min(vqt1.DeltaR(vlep2),vqt2.DeltaR(vlep2));
    mindr_t = TMath::Min(mindr_t,vqw1.DeltaR(vlep2));
    mindr_t = TMath::Min(mindr_t,vqw2.DeltaR(vlep2));
   
    
    //b quarks
    ttH::GenParticle qb1 = pruned_genParticles->at(indexes[2]);
    ttH::GenParticle qb2 = pruned_genParticles->at(indexes[3]);
    TVector3 vqb1(qb1.tlv().Px(), qb1.tlv().Py(), qb1.tlv().Pz());
    TVector3 vqb2(qb2.tlv().Px(), qb2.tlv().Py(), qb2.tlv().Pz());
    /*
      if (vlep1.Pt() < 10 || vlep2.Pt() < 10 || vqw1.Pt() < 10 || vqw2.Pt() < 10 || vqt1.Pt() < 10 || vqt2.Pt() < 10 || 
      vqb1.Pt() < 10 ||  vqb2.Pt() < 10) continue;
      histo->Fill(7., weight); 
    */
    
    /*
      if (vlep1.Pt() < 20 && vlep2.Pt() < 20) continue;
      histo->Fill(8., weight); 
    */	
    /*
    // Filling histos
    histo_dr->Fill(vlep1.DeltaR(vlep2), weight);
    histo_dr_hwwqq->Fill(vqw1.DeltaR(vqw2), weight);
    histo_dr_hwwlq->Fill(TMath::Min(vqw1.DeltaR(vlep1),vqw2.DeltaR(vlep1)), weight);
    histo_dr_hwwl_q->Fill(mindr, weight);
    histo_dr_toplq->Fill(TMath::Min(vqt1.DeltaR(vlep2),vqt2.DeltaR(vlep2)), weight);
    histo_dr_topl_q->Fill(mindr_t, weight);
    histo_dr_hwwlb->Fill(TMath::Min(vqb1.DeltaR(vlep1),vqb2.DeltaR(vlep1)), weight);
    histo_dr_toplb->Fill(vqb2.DeltaR(vlep2), weight);
    
    if (mindr > 0.3) continue;
    histo->Fill(9., weight); 
    */
     
    
  }
  
  

  
  if (!silent){
    cout << "------------------------------------------" << endl;
    cout << "[Results:] GEN only " << endl;
    cout << "------------------------------------------" << endl;
    for (int i = 1; i < 11; i++){
      if (i == 1) cout << " all: " << histo->GetBinContent(i) << " +/- " << histo->GetBinError(i) << endl;
      if (i == 2) cout << " higgs decay: " << histo->GetBinContent(i) << " +/- " << histo->GetBinError(i) << endl;
      if (i == 3) cout << " HWW : " << histo->GetBinContent(i) << " +/- " << histo->GetBinError(i) << endl;
      if (i == 4) cout << " Semileptonic HWW: " << histo->GetBinContent(i) << " +/- " << histo->GetBinError(i) << endl;
      if (i == 5) cout << " Two Wb decays of tt: " << histo->GetBinContent(i) << " +/- " << histo->GetBinError(i) << endl;
      if (i == 6) cout << " Semileptonic tt: " << histo->GetBinContent(i) << " +/- " << histo->GetBinError(i) << endl;
      if (i == 7) cout << " SS: " << histo->GetBinContent(i) << " +/- " << histo->GetBinError(i) << endl;
      if (i == 8) cout << " pt > 10 all : " << histo->GetBinContent(i) << " +/- " << histo->GetBinError(i) << endl;
      if (i == 9) cout << " pt > 20,10 leptons: " << histo->GetBinContent(i) << " +/- " << histo->GetBinError(i) << endl;
      if (i == 10) cout << " DR HWW and closest LF q < 0.3: " << histo->GetBinContent(i) << " +/- " << histo->GetBinError(i) << endl;
    }
    cout << "------------------------------------------" << endl;
    cout << "[Info:]" << nHWW*100/nused << "% of HWW in the events" << endl;
  



  }
  f_var.Write();
  f_var.Close();

  

}
