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
  sprintf(newRootFile,"results/output_gen_pt10.root");
  
  TFile *_file0 = TFile::Open(newRootFile);
 

  const int nPlots = 8;
  TString cutLabel[nPlots] = { "deltaR", "deltaR_qq", "deltaR_lq", "deltaR_l_q",
  				"deltaR_toplq", "deltaR_topl_q", "deltaR_hwwlb","deltaR_toplb"};
  TString cutTitle[nPlots] = { "#Delta R between SS leptons", "#Delta R between HWW quarks q and q'", "#Delta R between HWW lepton and closest HWW q",
  			       "#Delta R between HWW lepton and closest q", "#Delta R between top lepton and closest top light quark", 
			       "#Delta R between top lepton and closest light quark","#Delta R between HWW lepton and closest b quark", 
			       "#Delta R between top lepton and its b quark"}; 
 int rebins[nPlots] = {2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5}; 

  TH1D*  h0[nPlots];

  for (const int iPlot = 0; iPlot < nPlots; iPlot++){
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
    h0[iPlot]->GetXaxis()->SetRangeUser(0, 5);
    h0[iPlot]->Draw("histo");
    h0[iPlot]->GetXaxis()->SetTitle(cutTitle[iPlot]);
    h0[iPlot]->GetYaxis()->SetTitle("Normalized to 1");
    leg = new TLegend(0.70,0.80,0.90,0.90);
    leg->SetFillStyle(1001);
    leg->SetFillColor(kWhite);
    leg->SetBorderSize(1);
    leg->AddEntry(h0[iPlot],  properLabel[0], "l");
  
    leg->Draw();
    c1->SaveAs("plots/gen_pt10_" + cutLabel[iPlot] +".png");
    c1->SaveAs("plots/gen_pt10_" + cutLabel[iPlot] + ".pdf");
    gPad->SetLogy();
//    c1->SetLogy();
    c1->SaveAs("plots/gen_pt10_" + cutLabel[iPlot]+ "_log.png");
    c1->SaveAs("plots/gen_pt10_" + cutLabel[iPlot]+ "_log.pdf");
   
  }


  
}
