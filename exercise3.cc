
#include <TTree.h>

using namespace genie;

void exercise3()
{
  TH1D *hNuE = new TH1D("hNuE", "", 100, 0, 10.0);
  TH1D *hNuE_reco = new TH1D("hNuE_reco", "", 100, 0, 10.0);
  TH2D *hNuE_RecoTrue = new TH2D("hNuE_RecoTrue", "", 100, 0, 10.0, 100, 0, 10.0);

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

      GHepParticle * neu = myEvent->Probe();
      GHepParticle * fsl = myEvent->FinalStatePrimaryLepton();
      
      const TLorentzVector & k1 = *(neu->P4());
      const TLorentzVector & k2 = *(fsl->P4());
      
      double leptonEnergy_i = k1.Energy();
      double leptonEnergy_f = k2.Energy();
      
      TObjArrayIter iter(myEvent);
      GHepParticle * p = 0;

      double Energy_nucleon = 0; //energy of nucleon
      double E_m = 0;  //energy of meson

      //loop over event particles
      while ((p = dynamic_cast<GHepParticle *>(iter.Next())) != nullptr) {

	int pdgc = p->Pdg();
	int status = p->Status(); // 0=initial particles, 1=final particles
	double en = p->P4()->Energy();

	bool isNucleon = (pdgc==2212 || pdgc==2112);
        bool isLepton = (( abs(pdgc) >= 11) && ( abs(pdgc) <= 16)) ;

	if(status==1)
          {
	    if(isNucleon){
	      Energy_nucleon = Energy_nucleon + (p->P4()->Energy()) - (p->Mass());
	    }
	    
            if(!isNucleon && !isLepton)
              {
                E_m = E_m + (p->P4()->Energy());
              }
          }
	

      }

      double E_lep = leptonEnergy_f;
      double epsilon_n = 0.025; //25 MeV for Carbon target (arXiv:2208.03681)
      double E_lep_initial_reco = E_lep + Energy_nucleon + epsilon_n + E_m;

      hNuE->Fill(leptonEnergy_i);
      hNuE_reco->Fill(E_lep_initial_reco);

      hNuE_RecoTrue->Fill(E_lep_initial_reco, leptonEnergy_i);
      

    }

   
  TCanvas *c5 = new TCanvas("c5", "", 700, 600);
  hNuE->SetStats(0);
  hNuE->GetXaxis()->SetTitle("Neutrino energy (GeV)");
  hNuE->GetXaxis()->CenterTitle();
  hNuE->GetXaxis()->SetTitleOffset(0.85);
  hNuE->GetXaxis()->SetTitleSize(0.054);
  hNuE->GetXaxis()->SetLabelSize(0.044);
  hNuE->GetYaxis()->SetLabelSize(0.044);
  hNuE->SetLineWidth(3);
  hNuE->SetLineColor(kBlack);
  hNuE_reco->SetLineWidth(3);
  hNuE_reco->SetLineColor(kBlue);
  hNuE->Draw("hist");
  hNuE_reco->Draw("hist same");

  TLegend* l5 = new TLegend(0.45,0.75,0.85,0.85,"");
  l5->AddEntry(hNuE, "True", "L");
  l5->AddEntry(hNuE_reco, "Reco (calorimetric)", "L");
  l5->SetTextSize(0.04);
  l5->SetTextFont(42);
  l5->Draw();

  c5->SaveAs("RecoVsTrueNeutrinoEnergy.png");
  c5->SaveAs("RecoVsTrueNeutrinoEnergy.pdf");
  
  TCanvas *c6 = new TCanvas("c6", "", 700, 600);
  c6->SetRightMargin(0.15);
  hNuE_RecoTrue->SetStats(0);
  hNuE_RecoTrue->GetXaxis()->SetTitle("Reconstructed energy (GeV)");
  hNuE_RecoTrue->GetXaxis()->CenterTitle();
  hNuE_RecoTrue->GetXaxis()->SetTitleOffset(0.85);
  hNuE_RecoTrue->GetXaxis()->SetTitleSize(0.054);
  hNuE_RecoTrue->GetYaxis()->SetTitle("True energy (GeV)");
  hNuE_RecoTrue->GetYaxis()->CenterTitle();
  hNuE_RecoTrue->GetYaxis()->SetTitleOffset(0.85);
  hNuE_RecoTrue->GetYaxis()->SetTitleSize(0.054);
  hNuE_RecoTrue->GetXaxis()->SetLabelSize(0.044);
  hNuE_RecoTrue->GetYaxis()->SetLabelSize(0.044);
  hNuE_RecoTrue->SetLineWidth(4);
  hNuE_RecoTrue->Draw("colz");

  c6->SaveAs("RecoVsTrueNeutrinoEnergy_2d.png");
  c6->SaveAs("RecoVsTrueNeutrinoEnergy_2d.pdf");

}
