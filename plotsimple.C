 #include "TFile.h"
#include "TTree.h"
#include "TLorentzVector.h"

//#include "setTDRStyle.C"
 
void plotsimple(int mode = 1){
  
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  gStyle->SetErrorX(0);
  
  gStyle->SetOptStat(0);
  gStyle->SetPalette(1);
 // gROOT->SetBatch(1);
    
  gStyle->SetCanvasBorderMode(0);
  gStyle->SetCanvasColor(kWhite);
  gStyle->SetCanvasDefH(600);
  gStyle->SetCanvasDefW(600);
  gStyle->SetLabelFont(18,"");
  
  TString sampleLabel[2] = { "ttH125",	"TTJets"};
  TString properLabel[2] ={"ttH signal", "t#bar{t} background" };
 
  char newRootFile[300];
  sprintf(newRootFile,"results/output_gen_100k.root");
  
  TFile *_file0 = TFile::Open(newRootFile);
 

  const int nPlots = 7;
  TString cutLabel[nPlots] = { "deltaR_qq", "deltaR_qqs", "deltaR_lq",
  				"lepton_pt", "Higgs_pt", "deltaR_Hpt", "deltaR_lpt" };
  TString cutTitle[nPlots] = { "#DeltaR_{qq}", "#DeltaR_{l - qq system}", "#DeltaR_{lq}",
  			       "p_{T} of the lepton", "p_{T} of the Higgs boson", 
			       "Higgs boson", 
			       "Lepton "}; 
 int rebins[nPlots] = {2.5, 2.5, 2.5, 3, 5, 2.5, 2.5}; 

  TH1D*  h0[nPlots-2];
  TH2D*  h1[2];

  for (const int iPlot = 0; iPlot < nPlots-2; iPlot++){
    cout << cutLabel[iPlot] << endl;
    h0[iPlot] = (TH1D*) _file0->Get(cutLabel[iPlot]+"_"+sampleLabel[0]);
   
        
    TCanvas *c1 = new TCanvas();
    h0[iPlot]->SetLineColor(kRed);
    h0[iPlot]->SetMarkerColor(kRed);
    h0[iPlot]->SetMarkerStyle(20);
    h0[iPlot]->SetMarkerSize(1);
    h0[iPlot]->SetNormFactor(1);
    h0[iPlot]->SetNormFactor(1);
    h0[iPlot]->SetLineWidth(3);
    h0[iPlot]->Rebin(rebins[iPlot]);
    h0[iPlot]->Rebin(rebins[iPlot]);
    h0[iPlot]->Draw("histo");
    if (iPlot < 3) h0[iPlot]->GetXaxis()->SetRangeUser(0, 4);
    h0[iPlot]->GetXaxis()->SetTitle(cutTitle[iPlot]);
    h0[iPlot]->GetYaxis()->SetTitle("Normalized to 1");
    leg = new TLegend(0.70,0.80,0.90,0.90);
    leg->SetFillStyle(1001);
    leg->SetFillColor(kWhite);
    leg->SetBorderSize(1);
    leg->AddEntry(h0[iPlot],  properLabel[0], "l");
  
    leg->Draw();
    c1->SaveAs("plots/gen_100k_" + cutLabel[iPlot] +".png");
    c1->SaveAs("plots/gen_100k_" + cutLabel[iPlot] + ".pdf");
    gPad->SetLogy();
//    c1->SetLogy();
    c1->SaveAs("plots/gen_100k_" + cutLabel[iPlot]+ "_log.png");
    c1->SaveAs("plots/gen_100k_" + cutLabel[iPlot]+ "_log.pdf");
   
  }
  for (const int iPlot = 0; iPlot < 2; iPlot++){
    cout << cutLabel[iPlot+5] << endl;
    h1[iPlot] = (TH2D*) _file0->Get(cutLabel[iPlot+5]+"_"+sampleLabel[0]);
   
        
    TCanvas *c1 = new TCanvas();
    h1[iPlot]->SetLineColor(kRed);
    h1[iPlot]->SetMarkerColor(kRed);
    h1[iPlot]->SetMarkerStyle(20);
    h1[iPlot]->SetMarkerSize(1);
   
   
    h1[iPlot]->GetXaxis()->SetTitle("#DeltaR_{lq}");
    h1[iPlot]->GetYaxis()->SetTitle("p_{T}");
    h1[iPlot]->Draw();
    h1[iPlot]->SetTitle(cutTitle[iPlot+5]);
   
    leg = new TLegend(0.70,0.80,0.90,0.90);
    leg->SetFillStyle(1001);
    leg->SetFillColor(kWhite);
    leg->SetBorderSize(1);
    leg->AddEntry(h1[iPlot],  properLabel[0], "l");
  
    leg->Draw();
    c1->SaveAs("plots/gen_100k_" + cutLabel[iPlot+5] +".png");
    c1->SaveAs("plots/gen_100k_" + cutLabel[iPlot+5] + ".pdf");
    gPad->SetLogy();
//    c1->SetLogy();
    c1->SaveAs("plots/gen_100k_" + cutLabel[iPlot+5]+ "_log.png");
    c1->SaveAs("plots/gen_100k_" + cutLabel[iPlot+5]+ "_log.pdf");
   
  }

  
}
