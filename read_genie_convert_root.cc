#include <TTree.h>

using namespace genie;

void read_genie_convert_root()
{
  
  //Open GHEP/ROOT file
  TFile *myFile = new TFile("gntp.0.ghep.numu1000060120.root");

  //Get the GENIE GHEP tree and set its branch address
  TTree *myTree = dynamic_cast<TTree*>(myFile->Get("gtree"));
  genie::NtpMCEventRecord* myEventRecord = new NtpMCEventRecord();
  myTree->SetBranchAddress("gmcrec", &myEventRecord);

  //Number of entries in the tree
  int nentries = myTree->GetEntries();

  TFile *outputFile = new TFile("genie_.numu1000060120.root", "RECREATE");
  TTree *Event = new TTree("Event", "Event info");

  int nupdg;
  double nuE;
  double nuPx, nuPy, nuPz;
  double xsection;
  bool IsQE, IsRES, IsDIS, IsCoh, IsMEC, IsCC, IsNC;
  double Q2, W;

  Event->Branch("nupdg", &nupdg, "nupdg/I");
  Event->Branch("nuE", &nuE, "nuE/D");
  Event->Branch("nuPx", &nuPx, "nuPx/D");
  Event->Branch("nuPy", &nuPy, "nuPy/D");
  Event->Branch("nuPz", &nuPz, "nuPz/D");
  Event->Branch("xsection", &xsection, "xsection/D");
  Event->Branch("IsQE", &IsQE, "IsQE/O");
  Event->Branch("IsRES", &IsRES, "IsRES/O");
  Event->Branch("IsDIS", &IsDIS, "IsDIS/O");
  Event->Branch("IsCoh", &IsCoh, "IsCoh/O");
  Event->Branch("IsMEC", &IsMEC, "IsMEC/O");
  Event->Branch("IsCC", &IsCC, "IsCC/O");
  Event->Branch("IsNC", &IsNC, "IsNC/O");
  //Event->Branch("Q2", &Q2, "Q2/D");
  //Event->Branch("W", &W, "W/D");

  TTree *Particles = new TTree("Particles", "Particles info");

  std::vector<int> status, pdg;
  std::vector<double> energy, px, py, pz;
  Particles->Branch("status", &status);
  Particles->Branch("pdg", &pdg);
  Particles->Branch("energy", &energy);
  Particles->Branch("px", &px);
  Particles->Branch("py", &py);
  Particles->Branch("pz", &pz);

  //Loop over event
  for(int i=0; i<nentries; i++)
    {
      myTree->GetEntry(i);

      genie::EventRecord *myEvent = myEventRecord->event;

      const ProcessInfo & proc = myEvent->Summary()->ProcInfo();
      const Kinematics & kine = myEvent->Summary()->Kine();

      GHepParticle * neu = myEvent->Probe();
      const TLorentzVector & k1 = *(neu->P4());

      nupdg = neu->Pdg();
      nuE = k1.Energy();
      nuPx = k1.Px();
      nuPy = k1.Py();
      nuPz = k1.Pz();
      
      xsection = myEvent->XSec()/(5.07*pow(10, 13) * 5.07*pow(10,13));
      
      IsQE = proc.IsQuasiElastic();
      IsRES = proc.IsResonant();
      IsDIS = proc.IsDeepInelastic();
      IsCoh = proc.IsCoherentProduction();
      IsMEC = proc.IsMEC();
      IsCC = proc.IsWeakCC();
      IsNC = proc.IsWeakNC();
      
      Event->Fill();
      
      TObjArrayIter iter(myEvent);
      GHepParticle * p = 0;
      status.clear();
      pdg.clear();
      energy.clear();
      px.clear();
      py.clear();
      pz.clear();
      
       //loop over event particles
       while ((p = dynamic_cast<GHepParticle *>(iter.Next())) != nullptr) {

	 status.push_back(p->Status()); // 0=initial particles, 1=final particles
	 pdg.push_back(p->Pdg());
	 energy.push_back(p->P4()->Energy());
	 px.push_back(p->P4()->Px());
	 py.push_back(p->P4()->Py());
	 pz.push_back(p->P4()->Pz());

	 // Particles->Fill();
       }
       Particles->Fill();
    }

  outputFile->Write();
  outputFile->Close();
}
