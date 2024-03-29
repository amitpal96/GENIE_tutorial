
#include <TTree.h>

using namespace genie;

void exercise1()
{
  TH1D *hTotal = new TH1D("hTotal", "", 100, 0, 10.0);

  TH1D *hQEL = new TH1D("hQEL", "", 100, 0, 10.0);
  TH1D *hDIS = new TH1D("hDIS", "", 100, 0, 10.0);
  TH1D *hRES = new TH1D("hRES", "", 100, 0, 10.0);
  TH1D *hMEC = new TH1D("hMEC", "", 100, 0, 10.0);
  TH1D *hCOH = new TH1D("hCOH", "", 100, 0, 10.0);

  TH1D *hTotal_xsec = new TH1D("hTotal_xsec", "", 100, 0, 10.0);
  TH1D *hQEL_xsec = new TH1D("hQEL_xsec", "", 100, 0, 10.0);
  TH1D *hDIS_xsec = new TH1D("hDIS_xsec", "", 100, 0, 10.0);
  TH1D *hRES_xsec = new TH1D("hRES_xsec", "", 100, 0, 10.0);
  TH1D *hMEC_xsec = new TH1D("hMEC_xsec", "", 100, 0, 10.0);
  TH1D *hCOH_xsec = new TH1D("hCOH_xsec", "", 100, 0, 10.0);
  //Open GHEP/ROOT file
  TFile *myFile = new TFile("gntp.0.ghep.numu1000060120.root");

  //Get the GENIE GHEP tree and set its branch address
  TTree *myTree = dynamic_cast<TTree*>(myFile->Get("gtree"));
  genie::NtpMCEventRecord* myEventRecord = new NtpMCEventRecord();
  myTree->SetBranchAddress("gmcrec", &myEventRecord);
  
  //Number of entries in the tree
  int nentries = myTree->GetEntries();

  //Loop over event
  for(int i=0; i<nentries; i++)
    {
      //Fetch i'th event
      myTree->GetEntry(i);

      genie::EventRecord *myEvent = myEventRecord->event;

      const ProcessInfo & proc = myEvent->Summary()->ProcInfo();
      const Kinematics & kine = myEvent->Summary()->Kine();

      //For interaction info look at Framework/Interaction/ProcessInfo.h
      bool is_dis = proc.IsDeepInelastic();
      bool is_res = proc.IsResonant();
      bool is_qel = proc.IsQuasiElastic();
      bool is_cohpr = proc.IsCoherentProduction();
      bool is_mec = proc.IsMEC();

      bool is_cc  = proc.IsWeakCC();
      bool is_nc = proc.IsWeakNC();

      
      
      //Cross-section of the process
      double xsec_event = myEvent->XSec() / (5.07*pow(10, 13) * 5.07*pow(10,13)); //unit in cm^2

      //Access initial and final state lepton vector
      GHepParticle * neu = myEvent->Probe();
      //GHepParticle * fsl = myEvent->FinalStatePrimaryLepton();
  
      //get their corresponding 4-momenta in the LAB frame
      const TLorentzVector & k1 = *(neu->P4());
      //const TLorentzVector & k2 = *(fsl->P4());

      //Initial lepton
      double leptonEnergy_i = k1.Energy();
      int leptonPdg_i = neu->Pdg();

      //Final lepton
      //double leptonEnergy_f = k2.Energy();
      //int leptonPdg_f = fsl->Pdg();


      hTotal->Fill(leptonEnergy_i);
      
      if(is_qel) {hQEL->Fill(leptonEnergy_i);}
      if(is_dis) {hDIS->Fill(leptonEnergy_i);}
      if(is_res) {hRES->Fill(leptonEnergy_i);}
      if(is_cohpr) {hCOH->Fill(leptonEnergy_i);}
      if(is_mec) {hMEC->Fill(leptonEnergy_i);}

      hTotal_xsec->Fill(leptonEnergy_i, xsec_event);
      if(is_qel) {hQEL_xsec->Fill(leptonEnergy_i, xsec_event);}
      if(is_dis) {hDIS_xsec->Fill(leptonEnergy_i, xsec_event);}
      if(is_res) {hRES_xsec->Fill(leptonEnergy_i, xsec_event);}
      if(is_cohpr) {hCOH_xsec->Fill(leptonEnergy_i, xsec_event);}
      if(is_mec) {hMEC_xsec->Fill(leptonEnergy_i, xsec_event);}
    }

  
  TCanvas *c1 = new TCanvas("c1", "", 700, 600);
  hTotal->SetStats(0);
  hTotal->GetXaxis()->SetTitle("Neutrino energy (GeV)");
  hTotal->GetXaxis()->CenterTitle();
  hTotal->GetXaxis()->SetTitleOffset(0.85);
  hTotal->GetXaxis()->SetTitleSize(0.054);
  hTotal->GetXaxis()->SetLabelSize(0.044);
  hTotal->GetYaxis()->SetLabelSize(0.044);
  double max1a = 1.2*std::max(hRES->GetMaximum(), hDIS->GetMaximum());
  hRES->SetMaximum(max1a);
  hRES->SetLineWidth(4);
  hRES->SetLineColor(kTeal+10);
  hQEL->SetLineWidth(4);
  hQEL->SetLineColor(kGreen-3);
  hDIS->SetLineWidth(4);
  hDIS->SetLineColor(kAzure+8);
  hCOH->SetLineWidth(4);
  hCOH->SetLineColor(kRed-9);
  hMEC->SetLineWidth(4);
  hMEC->SetLineColor(kMagenta-7);
  hTotal->SetLineWidth(4);
  hTotal->SetLineColor(kBlack);
  hTotal->Draw("hist");
  hRES->Draw("hist same");
  hQEL->Draw("hist same");
  hDIS->Draw("hist same");
  hCOH->Draw("hist same");
  hMEC->Draw("hist same");

  TLegend* l1a = new TLegend(0.7,0.55,0.85,0.85,"");
  l1a->AddEntry(hTotal, "Total", "L");
  l1a->AddEntry(hQEL, "QEL", "L");
  l1a->AddEntry(hRES, "RES","L");
  l1a->AddEntry(hDIS, "DIS","L");
  l1a->AddEntry(hCOH, "COH","L");
  l1a->AddEntry(hMEC, "MEC","L");
  l1a->SetTextSize(0.04);
  l1a->SetTextFont(42);
  l1a->Draw();

  c1->SaveAs("InteractionEnergy.png");
  c1->SaveAs("InteractionEnergy.pdf");
  

  TCanvas *c2 = new TCanvas("c2", "", 700, 600);
  hTotal_xsec->SetStats(0);
  hTotal_xsec->GetXaxis()->SetTitle("Neutrino energy (GeV)");
  hTotal_xsec->GetXaxis()->CenterTitle();
  hTotal_xsec->GetXaxis()->SetTitleOffset(0.85);
  hTotal_xsec->GetXaxis()->SetTitleSize(0.054);
  hTotal_xsec->GetXaxis()->SetLabelSize(0.044);
  hTotal_xsec->GetYaxis()->SetLabelSize(0.044);
  double max2a = 1.2*std::max(hRES_xsec->GetMaximum(), hDIS_xsec->GetMaximum());
  hRES_xsec->SetMaximum(max2a);
  hRES_xsec->SetLineWidth(4);
  hRES_xsec->SetLineColor(kTeal+10);
  hQEL_xsec->SetLineWidth(4);
  hQEL_xsec->SetLineColor(kGreen-3);
  hDIS_xsec->SetLineWidth(4);
  hDIS_xsec->SetLineColor(kAzure+8);
  hCOH_xsec->SetLineWidth(4);
  hCOH_xsec->SetLineColor(kRed-9);
  hMEC_xsec->SetLineWidth(4);
  hMEC_xsec->SetLineColor(kMagenta-7);
  hTotal_xsec->SetLineWidth(4);
  hTotal_xsec->SetLineColor(kBlack);
  hTotal_xsec->Draw("hist");
  hRES_xsec->Draw("hist same");
  hQEL_xsec->Draw("hist same");
  hDIS_xsec->Draw("hist same");
  hCOH_xsec->Draw("hist same");
  hMEC_xsec->Draw("hist same");

  TLegend* l2a = new TLegend(0.7,0.55,0.85,0.85,"");
  l2a->AddEntry(hTotal_xsec, "Total", "L");
  l2a->AddEntry(hQEL_xsec, "QEL", "L");
  l2a->AddEntry(hRES_xsec, "RES","L");
  l2a->AddEntry(hDIS_xsec, "DIS","L");
  l2a->AddEntry(hCOH_xsec, "COH","L");
  l2a->AddEntry(hMEC_xsec, "MEC","L");
  l2a->SetTextSize(0.04);
  l2a->SetTextFont(42);
  l2a->Draw();

  c2->SaveAs("InteractionXsec.png");
  c2->SaveAs("InteractionXsec.pdf");
}
