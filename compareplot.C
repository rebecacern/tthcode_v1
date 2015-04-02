 #include "TFile.h"
#include "TTree.h"
#include "TLorentzVector.h"

//#include "setTDRStyle.C"
 
void compareplot(int mode = 2){
  
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  gStyle->SetErrorX(0);
  
  gStyle->SetOptStat(0);
  gStyle->SetPalette(1);
  gROOT->SetBatch(1);
    
  gStyle->SetCanvasBorderMode(0);
  gStyle->SetCanvasColor(kWhite);
  gStyle->SetCanvasDefH(600);
  gStyle->SetCanvasDefW(600);
  gStyle->SetLabelFont(18,"");
  
  TString finalState[3] = { "emu","mumu", "ee"};
  TString sampleLabel[2] = { "ttH125",	"TTJets"};
  TString properLabel[2] ={"ttH signal", "t#bar{t} background" };
 
  char newRootFile[300];
  sprintf(newRootFile,"results/output_short_%d.root",mode);
  
  TFile *_file0 = TFile::Open(newRootFile);
 

  const int nPlots = 18;
  TString cutLabel[nPlots] = { "cuts", "njets", "nbjets", "nleptons",
  				"njets_2lep", "nbjets_2lep", "nleptons_2lep", 
				"njets_ss", "nbjets_ss", "nleptons_ss", 
				"pt_lep1", "pt_lep2", "met",
				"njets_final", "nbjets_final",  
				"pt_lep1_final", "pt_lep2_final", "met_final"};
  TString cutTitle[nPlots] = { "Cut Flow", "Number of jets Pt > 20", "Number of b-jets Pt > 20", "Number of leptons Pt > 10",
  				"Number of jets Pt > 20", "Number of b-jets Pt > 20", "Number of leptons Pt > 10",
  				"Number of jets Pt > 20", "Number of b-jets Pt > 20", "Number of leptons Pt > 10",
  				"Pt of the leading lepton", "Pt of the second lepton", "MET",
  				"Number of jets Pt > 20", "Number of b-jets Pt > 20", 
  				"Pt of the leading lepton", "Pt of the second lepton", "MET" }; 
 int rebins[nPlots] = {1, 1, 1, 1,
 			1, 1, 1, 
			1, 1, 1,
			4, 4, 4,
			1, 1,  
			4, 4, 4}; 

  TH1D*  h0[2][nPlots];

  for (const int iPlot = 0; iPlot < nPlots; iPlot++){
    cout << cutLabel[iPlot] << endl;
    h0[0][iPlot] = (TH1D*) _file0->Get(cutLabel[iPlot]+"_"+sampleLabel[0]);
    h0[1][iPlot] = (TH1D*) _file0->Get(cutLabel[iPlot]+"_"+sampleLabel[1]);
        
    TCanvas *c1 = new TCanvas();
    h0[0][iPlot]->SetLineColor(kRed);
    h0[0][iPlot]->SetMarkerColor(kRed);
    h0[0][iPlot]->SetMarkerStyle(20);
    h0[0][iPlot]->SetMarkerSize(1);
    h0[1][iPlot]->SetMarkerStyle(20);
    h0[1][iPlot]->SetMarkerSize(1);
    h0[0][iPlot]->SetNormFactor(1);
    h0[1][iPlot]->SetNormFactor(1);
    h0[0][iPlot]->Rebin(rebins[iPlot]);
    h0[1][iPlot]->Rebin(rebins[iPlot]);
    double max = 1.3*(TMath::Max(h0[0][iPlot]->GetMaximum(), h0[1][iPlot]->GetMaximum()));
    h0[0][iPlot]->Draw("histo");
    h0[1][iPlot]->Draw("histo, sames");

    h0[0][iPlot]->GetYaxis()->SetRangeUser(0.01, max);
    h0[0][iPlot]->GetXaxis()->SetTitle(cutTitle[iPlot]);
    leg = new TLegend(0.60,0.80,0.99,0.99);
    leg->SetFillStyle(1001);
    leg->SetFillColor(kWhite);
    leg->SetBorderSize(1);
    leg->AddEntry(h0[0][iPlot],  properLabel[1], "l");
    leg->AddEntry(h0[1][iPlot],  properLabel[0], "l");
  
    leg->Draw();
    c1->SaveAs("plots/figure_" + finalState[mode] +"_"+ cutLabel[iPlot] +".png");
    c1->SaveAs("plots/figure_" + finalState[mode] +"_"+ cutLabel[iPlot] + ".pdf");
    gPad->SetLogy();
//    c1->SetLogy();
    c1->SaveAs("plots/figure_" + finalState[mode] +"_"+ cutLabel[iPlot]+ "_log.png");
    c1->SaveAs("plots/figure_" + finalState[mode] +"_"+ cutLabel[iPlot]+ "_log.pdf");
   
  }


  
}
