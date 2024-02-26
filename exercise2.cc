
#include <TTree.h>

using namespace genie;

void  exercise2()
{
  
  TH1D *hQ2 = new TH1D("hQ2", "", 100, 0, 5.0);
  TH1D *hq = new TH1D("hq", "", 100, 0, 5.0);
  TH1D *hMomTrans = new TH1D("hMomTrans", "", 100, 0, 5.0);
  TH1D *hBjx = new TH1D("hBjx", "", 100, 0, 2);
  TH1D *hBjy = new TH1D("hBjy", "", 100, 0, 2);

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

      //Access initial and final state lepton vector
      GHepParticle * neu = myEvent->Probe();
      GHepParticle * fsl = myEvent->FinalStatePrimaryLepton();
      //GHepParticle * nuc = myEvent->HitNucleon();

      //get their corresponding 4-momenta in the LAB frame
      const TLorentzVector & k1 = *(neu->P4());
      const TLorentzVector & k2 = *(fsl->P4());

      //int hitnuc_pdg = nuc->Pdg();
      //double mass_hitn = myEvent->HitNucleon()->Mass();
      double mass_hitn = 0.93827208;
      TLorentzVector q = k1 - k2;

      double Q2 = -1*q.M2(); //four momentum transfer
      double v = q.Energy(); //energy transfer
      double momTrans = sqrt((q.Z()*q.Z()) + (q.X()*q.X()) + (q.Y()*q.Y()));
      double x = Q2/(2*mass_hitn*v);
      double y = v/k1.Energy();

      hQ2->Fill(Q2);
      hq->Fill(v);
      hMomTrans->Fill(momTrans);
      hBjx->Fill(x);
      hBjy->Fill(y);
      
      }

  
  TCanvas *c1a = new TCanvas("c1a", "", 700, 600);
  hQ2->SetStats(0);
  hQ2->SetLineWidth(3);
  hQ2->GetXaxis()->SetTitle("Four momentum transfer (GeV)");
  hQ2->GetXaxis()->CenterTitle();
  hQ2->GetXaxis()->SetTitleOffset(0.85);
  hQ2->GetXaxis()->SetTitleSize(0.054);
  hQ2->GetXaxis()->SetLabelSize(0.044);
  hQ2->GetYaxis()->SetLabelSize(0.044);
  hQ2->Draw("hist");

  c1a->SaveAs("Q2.png");
  c1a->SaveAs("Q2.pdf");
  
  TCanvas *c1b = new TCanvas("c1b", "", 700, 600);
  hq->SetStats(0);
  hq->SetLineWidth(3);
  hq->GetXaxis()->SetTitle("Energy transfer (GeV)");
  hq->GetXaxis()->CenterTitle();
  hq->GetXaxis()->SetTitleOffset(0.85);
  hq->GetXaxis()->SetTitleSize(0.054);
  hq->GetXaxis()->SetLabelSize(0.044);
  hq->GetYaxis()->SetLabelSize(0.044);
  hq->Draw("hist");

  c1b->SaveAs("EnergyTransfer.png");
  c1b->SaveAs("EnergyTransfer.pdf");

  TCanvas *c1c = new TCanvas("c1c", "", 700, 600);
  hMomTrans->SetStats(0);
  hMomTrans->SetLineWidth(3);
  hMomTrans->GetXaxis()->SetTitle("Momentum transfer (GeV)");
  hMomTrans->GetXaxis()->CenterTitle();
  hMomTrans->GetXaxis()->SetTitleOffset(0.85);
  hMomTrans->GetXaxis()->SetTitleSize(0.054);
  hMomTrans->GetXaxis()->SetLabelSize(0.044);
  hMomTrans->GetYaxis()->SetLabelSize(0.044);
  hMomTrans->Draw("hist");

  c1c->SaveAs("MomentumTransfer.png");
  c1c->SaveAs("MomentumTransfer.pdf");
  
  TCanvas *c1d = new TCanvas("c1d", "", 700, 600);
  hBjx->SetStats(0);
  hBjx->SetLineWidth(3);
  hBjx->GetXaxis()->SetTitle("Bjorken x");
  hBjx->GetXaxis()->CenterTitle();
  hBjx->GetXaxis()->SetTitleOffset(0.85);
  hBjx->GetXaxis()->SetTitleSize(0.054);
  hBjx->GetXaxis()->SetLabelSize(0.044);
  hBjx->GetYaxis()->SetLabelSize(0.044);
  hBjx->Draw("hist");

  c1d->SaveAs("Bjorkenx.png");
  c1d->SaveAs("Bjorkenx.pdf");
  
  TCanvas *c1e = new TCanvas("c1e", "", 700, 600);
  hBjy->SetStats(0);
  hBjy->SetLineWidth(3);
  hBjy->GetXaxis()->SetTitle("Bjorken y");
  hBjy->GetXaxis()->CenterTitle();
  hBjy->GetXaxis()->SetTitleOffset(0.85);
  hBjy->GetXaxis()->SetTitleSize(0.054);
  hBjy->GetXaxis()->SetLabelSize(0.044);
  hBjy->GetYaxis()->SetLabelSize(0.044);
  hBjy->Draw("hist");

  c1e->SaveAs("Bjorkeny.png");
  c1e->SaveAs("Bjorkeny.pdf");
  
  
 }
