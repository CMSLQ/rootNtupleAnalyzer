#define analysisClass_cxx
#include "analysisClass.h"
#include <TH2.h>
#include <TH1F.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLorentzVector.h>
#include <TVector2.h>
#include <TVector3.h>
#include <vector>

analysisClass::analysisClass(string * inputList, string * treeName, TString * outputFileName)
  :baseClass(inputList, treeName, outputFileName)
{
  std::cout << "analysisClass::analysisClass(): begins " << std::endl;

  std::cout << "analysisClass::analysisClass(): ends " << std::endl;
}

analysisClass::~analysisClass()
{
  std::cout << "analysisClass::~analysisClass(): begins " << std::endl;

  std::cout << "analysisClass::~analysisClass(): ends " << std::endl;
}

void analysisClass::Loop()
{
   std::cout << "analysisClass::Loop() begins" <<std::endl;   
    
   if (fChain == 0) return;
   
   //////////book histos here

   TH1F *h_ele_E = new TH1F ("ele_E","ele_E",100,0,1000);
   TH1F *h_ele_Pt  = new TH1F ("ele_Pt","ele_Pt",100,0,1000);
   TH1F *h_ele_Phi  = new TH1F ("ele_Phi","ele_Phi",71,-3.55,3.55);
   TH1F *h_ele_Eta  = new TH1F ("ele_Eta","ele_Eta",121,-6.05,6.05);
   TH1F *h_ele_CaloEnergy  = new TH1F ("ele_CaloEnergy","ele_CaloEnergy",100,0,1000);

   TH1F *h_N_ele_matched = new TH1F ("N_ele_matched","N_ele_matched",5,-0.5,4.5);
   TH1F *h_ele_E_matched = new TH1F ("ele_E_matched","ele_E_matched",100,0,1000);
   TH1F *h_ele_Pt_matched  = new TH1F ("ele_Pt_matched","ele_Pt_matched",100,0,1000);
   TH1F *h_ele_Phi_matched  = new TH1F ("ele_Phi_matched","ele_Phi_matched",71,-3.55,3.55);
   TH1F *h_ele_Eta_matched  = new TH1F ("ele_Eta_matched","ele_Eta_matched",121,-6.05,6.05);
   TH1F *h_ele_CaloEnergy_matched  = new TH1F ("ele_CaloEnergy_matched","ele_CaloEnergy_matched",100,0,1000);

   TH1F *h_ele_E_ID_ISO = new TH1F ("ele_E_ID_ISO","ele_E_ID_ISO",100,0,1000);
   TH1F *h_ele_Pt_ID_ISO  = new TH1F ("ele_Pt_ID_ISO","ele_Pt_ID_ISO",100,0,1000);
   TH1F *h_ele_Phi_ID_ISO  = new TH1F ("ele_Phi_ID_ISO","ele_Phi_ID_ISO",71,-3.55,3.55);
   TH1F *h_ele_Eta_ID_ISO  = new TH1F ("ele_Eta_ID_ISO","ele_Eta_ID_ISO",121,-6.05,6.05);
   TH1F *h_ele_CaloEnergy_ID_ISO  = new TH1F ("ele_CaloEnergy_ID_ISO","ele_CaloEnergy_ID_ISO",100,0,1000);

   TH1F *h_N_ele_Gen = new TH1F ("N_ele_Gen","N_ele_Gen",5,-0.5,4.5);
   TH1F *h_ele_Pt_Gen  = new TH1F ("ele_Pt_Gen","ele_Pt_Gen",100,0,1000);
   TH1F *h_ele_Pt_Gen_etaCut  = new TH1F ("ele_Pt_Gen_etaCut","ele_Pt_Gen_etaCut",100,0,1000);
   TH1F *h_ele_Pt_Gen_matched  = new TH1F ("ele_Pt_Gen_matched","ele_Pt_Gen_matched",100,0,1000);
   TH1F *h_ele_Pt_Gen_matched_ID  = new TH1F ("ele_Pt_Gen_matched_ID","ele_Pt_Gen_matched_ID",100,0,1000);
   TH1F *h_ele_Pt_Gen_matched_ID_ISO = new TH1F ("ele_Pt_Gen_matched_ID_ISO","ele_Pt_Gen_matched_ID_ISO",100,0,1000);
   TH1F *h_ele_Eta_Gen  = new TH1F ("ele_Eta_Gen","ele_Eta_Gen",121,-6.05,6.05);
   TH1F *h_ele_Eta_Gen_lowPt  = new TH1F ("ele_Eta_Gen_lowPt","ele_Eta_Gen_lowPt",121,-6.05,6.05);
   TH1F *h_ele_Eta_Gen_etaCut  = new TH1F ("ele_Eta_Gen_etaCut","ele_Eta_Gen_etaCut",121,-6.05,6.05);
   TH1F *h_ele_Eta_Gen_matched  = new TH1F ("ele_Eta_Gen_matched","ele_Eta_Gen_matched",121,-6.05,6.05);
   TH1F *h_ele_Eta_Gen_matched_lowPt  = new TH1F ("ele_Eta_Gen_matched_lowPt","ele_Eta_Gen_matched_lowPt",121,-6.05,6.05);
   TH1F *h_ele_Eta_Gen_matched_ID  = new TH1F ("ele_Eta_Gen_matched_ID","ele_Eta_Gen_matched_ID",121,-6.05,6.05);
   TH1F *h_ele_Eta_Gen_matched_ID_ISO  = new TH1F ("ele_Eta_Gen_matched_ID_ISO","ele_Eta_Gen_matched_ID_ISO",121,-6.05,6.05);
   TH2F *h_ele_Eta_Pt_Gen = new TH2F ("ele_Eta_Pt_Gen","ele_Eta_Pt_Gen",100,0,1000,121,-6.05,6.05);

   TH1F *h_DeltaR_Gen_Reco = new TH1F("DeltaR_Gen_Reco","DeltaR_Gen_Reco",100,0,0.2);
   TH1F *h_DeltaR_Gen_2ndReco = new TH1F("DeltaR_Gen_2ndReco","DeltaR_Gen_2ndReco",200,0,1.0);

   TH1F *h_N_ele_Pre = new TH1F ("N_ele_Pre","N_ele_Pre",11,-0.5,10.5);
   TH1F *h_N_ele_Post_ID = new TH1F ("N_ele_Post_ID","N_ele_Post_ID",11,-0.5,10.5);
   TH1F *h_N_ele_Post_ID_ISO = new TH1F ("N_ele_Post_ID_ISO","N_ele_Post_ID_ISO",11,-0.5,10.5);

   TH1F *h_Energy_Res = new TH1F ("Energy_Res","Energy_Res",150,0,1.5);
   TH1F *h_Energy_Res_barrel = new TH1F ("Energy_Res_barrel","Energy_Res_barrel",150,0,1.5);
   TH1F *h_Energy_Res_endcap = new TH1F ("Energy_Res_endcap","Energy_Res_endcap",150,0,1.5);

   //////matched
   TH1F *h_eleHoE_barrel_matched = new TH1F("eleHoE_barrel_matched","eleHoE_barrel_matched",100,0,0.1);
   TH1F *h_eleSigmaEE_barrel_matched = new TH1F("eleSigmaEE_barrel_matched","eleSigmaEE_barrel_matched",100,0,0.05);
   TH1F *h_eleDeltaPhiTrkSC_barrel_matched = new TH1F("eleDeltaPhiTrkSC_barrel_matched","eleDeltaPhiTrkSC_barrel_matched",300,-0.15,0.15);
   TH1F *h_eleDeltaEtaTrkSC_barrel_matched = new TH1F("eleDeltaEtaTrkSC_barrel_matched","eleDeltaEtaTrkSC_barrel_matched",200,-0.02,0.02);
   TH1F *h_eleClassif_barrel_matched = new TH1F("eleClassif_barrel_matched","eleClassif_barrel_matched",200,0,200);

   TH1F *h_eleNumTrkIso_barrel_matched = new TH1F("eleNumTrkIso_barrel_matched","eleNumTrkIso_barrel_matched",11,-0.5,10.5);
   TH1F *h_eleTrkIso_barrel_matched = new TH1F("eleTrkIso_barrel_matched","eleTrkIso_barrel_matched",26,-0.5,26.5);
   TH1F *h_eleEcalRecHitIso_barrel_matched = new TH1F("eleEcalRecHitIso_barrel_matched","eleEcalRecHitIso_barrel_matched",21,-0.5,20.5);

   TH1F *h_eleHoE_endcap_matched = new TH1F("eleHoE_endcap_matched","eleHoE_endcap_matched",150,0,0.15);
   TH1F *h_eleSigmaEE_endcap_matched = new TH1F("eleSigmaEE_endcap_matched","eleSigmaEE_endcap_matched",100,0,0.05);
   TH1F *h_eleDeltaPhiTrkSC_endcap_matched = new TH1F("eleDeltaPhiTrkSC_endcap_matched","eleDeltaPhiTrkSC_endcap_matched",300,-0.15,0.15);
   TH1F *h_eleDeltaEtaTrkSC_endcap_matched = new TH1F("eleDeltaEtaTrkSC_endcap_matched","eleDeltaEtaTrkSC_endcap_matched",200,-0.02,0.02);
   TH1F *h_eleClassif_endcap_matched = new TH1F("eleClassif_endcap_matched","eleClassif_endcap_matched",200,0,200);

   TH1F *h_eleNumTrkIso_endcap_matched = new TH1F("eleNumTrkIso_endcap_matched","eleNumTrkIso_endcap_matched",11,-0.5,10.5);
   TH1F *h_eleTrkIso_endcap_matched = new TH1F("eleTrkIso_endcap_matched","eleTrkIso_endcap_matched",26,-0.5,25.5);
   TH1F *h_eleEcalRecHitIso_endcap_matched = new TH1F("eleEcalRecHitIso_endcap_matched","eleEcalRecHitIso_endcap_matched",21,-0.5,20.5);

   ////////unmatched
   TH1F *h_eleHoE_barrel_unmatched = new TH1F("eleHoE_barrel_unmatched","eleHoE_barrel_unmatched",100,0,0.1);
   TH1F *h_eleSigmaEE_barrel_unmatched = new TH1F("eleSigmaEE_barrel_unmatched","eleSigmaEE_barrel_unmatched",100,0,0.05);
   TH1F *h_eleDeltaPhiTrkSC_barrel_unmatched = new TH1F("eleDeltaPhiTrkSC_barrel_unmatched","eleDeltaPhiTrkSC_barrel_unmatched",300,-0.15,0.15);
   TH1F *h_eleDeltaEtaTrkSC_barrel_unmatched = new TH1F("eleDeltaEtaTrkSC_barrel_unmatched","eleDeltaEtaTrkSC_barrel_unmatched",200,-0.02,0.02);
   TH1F *h_eleClassif_barrel_unmatched = new TH1F("eleClassif_barrel_unmatched","eleClassif_barrel_unmatched",200,0,200);

   TH1F *h_eleNumTrkIso_barrel_unmatched = new TH1F("eleNumTrkIso_barrel_unmatched","eleNumTrkIso_barrel_unmatched",11,-0.5,10.5);
   TH1F *h_eleTrkIso_barrel_unmatched = new TH1F("eleTrkIso_barrel_unmatched","eleTrkIso_barrel_unmatched",26,-0.5,25.5);
   TH1F *h_eleEcalRecHitIso_barrel_unmatched = new TH1F("eleEcalRecHitIso_barrel_unmatched","eleEcalRecHitIso_barrel_unmatched",21,-0.5,20.5);

   TH1F *h_eleHoE_endcap_unmatched = new TH1F("eleHoE_endcap_unmatched","eleHoE_endcap_unmatched",150,0,0.15);
   TH1F *h_eleSigmaEE_endcap_unmatched = new TH1F("eleSigmaEE_endcap_unmatched","eleSigmaEE_endcap_unmatched",100,0,0.05);
   TH1F *h_eleDeltaPhiTrkSC_endcap_unmatched = new TH1F("eleDeltaPhiTrkSC_endcap_unmatched","eleDeltaPhiTrkSC_endcap_unmatched",300,-0.15,0.15);
   TH1F *h_eleDeltaEtaTrkSC_endcap_unmatched = new TH1F("eleDeltaEtaTrkSC_endcap_unmatched","eleDeltaEtaTrkSC_endcap_unmatched",200,-0.02,0.02);
   TH1F *h_eleClassif_endcap_unmatched = new TH1F("eleClassif_endcap_unmatched","eleClassif_endcap_unmatched",200,0,200);

   TH1F *h_eleNumTrkIso_endcap_unmatched = new TH1F("eleNumTrkIso_endcap_unmatched","eleNumTrkIso_endcap_unmatched",11,-0.5,10.5);
   TH1F *h_eleTrkIso_endcap_unmatched = new TH1F("eleTrkIso_endcap_unmatched","eleTrkIso_endcap_unmatched",26,-0.5,25.5);
   TH1F *h_eleEcalRecHitIso_endcap_unmatched = new TH1F("eleEcalRecHitIso_endcap_unmatched","eleEcalRecHitIso_endcap_unmatched",21,-0.5,20.5);

   TH1F *h_eleEff_Pt = new TH1F ("eleEff_Pt","eleEff_Pt",100,0,1000);
   TH1F *h_eleEff_Eta = new TH1F ("eleEff_Eta","eleEff_Eta",121,-6.05,6.05);
   TH1F *h_eleEff_Pt_ID = new TH1F ("eleEff_Pt_ID","eleEff_Pt_ID",100,0,1000);
   TH1F *h_eleEff_Eta_ID = new TH1F ("eleEff_Eta_ID","eleEff_Eta_ID",121,-6.05,6.05);
   TH1F *h_eleEff_Pt_ID_ISO = new TH1F ("eleEff_Pt_ID_ISO","eleEff_Pt_ID_ISO",100,0,1000);
   TH1F *h_eleEff_Eta_ID_ISO = new TH1F ("eleEff_Eta_ID_ISO","eleEff_Eta_ID_ISO",121,-6.05,6.05);


   /////////initialize variables
   int electron_PID=11;
   int LQ_PID=42;
   //int LQ_PID=23;
   float ConeSizeMCmatch_cut=0.07;

   /////////////Barrel Cuts ////////////////
   float eleHoE_barrel_cut=0.05; //0.05 = HEEP
   float eleSigmaEE_barrel_cut=0.011; //0.011 = HEEP
   float eleDeltaPhiTrkSC_barrel_cut=0.09; //0.09 = HEEP
   float eleDeltaEtaTrkSC_barrel_cut=0.0050; //0.005=HEEP

   int eleNumTrkIso_barrel_cut=5; //5=HEEP
   float eleTrkIso_barrel_cut=7.5; //7.5=HEEP
   float eleEcalRecHitIso_barrel_cut=6; // 6+0.1*Et = HEEP

   /////////////End Cap Cuts ////////////////
   float eleHoE_endcap_cut=0.1; //0.1 = HEEP
   float eleSigmaEE_endcap_cut=0.0275; //0.0275 = HEEP
   float eleDeltaPhiTrkSC_endcap_cut=0.09; //0.09 = HEEP
   float eleDeltaEtaTrkSC_endcap_cut=0.0070; //0.007=HEEP

   int eleNumTrkIso_endcap_cut=5;//5=HEEP
   float eleTrkIso_endcap_cut=15; //15=HEEP
   float eleEcalRecHitIso_endcap_cut=6; // 6+0.1*Et= HEEP 

   float elePt_cut=10.;
   float eleEta_cut=2.6;
   int n_ele_doubleMatched = 0;


   Long64_t nentries = fChain->GetEntriesFast();
   std::cout << "analysisClass::Loop(): nentries = " << nentries << std::endl;   

   ////// The following ~7 lines have been taken from rootNtupleClass->Loop() /////
   ////// If the root version is updated and rootNtupleClass regenerated,     /////
   ////// these lines may need to be updated.                                 /////    
   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
     Long64_t ientry = LoadTree(jentry);
     if (ientry < 0) break;
     nb = fChain->GetEntry(jentry);   nbytes += nb;
     if(jentry < 10 || jentry%100 == 0) std::cout << "analysisClass::Loop(): jentry = " << jentry << std::endl;   
     // if (Cut(ientry) < 0) continue;

     ////////////////////// User's code starts here ///////////////////////

     ///Stuff to be done every event

     int nEleMatched = 0;
     int nGenEle = 0;
     bool FirstLoop = true;
     int from_LQ_idx[2]={99,99};
     int matched_reco_idx[2]={99,99};
     int nEle_pre = 0;
     int nEle_post_ID = 0;
     int nEle_post_ID_ISO = 0;

     //////////Finding elec from LQ
     for(int igen=0;igen<GenParticleCount;igen++)
       {
 
	 //select electrons from LQ decay
	 if(abs(GenParticlePdgId[igen])==electron_PID
	    && abs(GenParticlePdgId[GenParticleMotherIndex[igen]])==LQ_PID)
	   {
	     nGenEle++;
	     h_ele_Pt_Gen->Fill(GenParticlePt[igen]);
	     h_ele_Eta_Gen->Fill(GenParticleEta[igen]);
	     h_ele_Eta_Pt_Gen->Fill(GenParticlePt[igen],GenParticleEta[igen]);
	     if (from_LQ_idx[0]==99) from_LQ_idx[0]=igen;
	     else from_LQ_idx[1]=igen;
	     if (fabs(GenParticleEta[igen])<eleEta_cut){
	       h_ele_Pt_Gen_etaCut->Fill(GenParticlePt[igen]);
	       h_ele_Eta_Gen_etaCut->Fill(GenParticleEta[igen]);
	     }
	     if (GenParticlePt[igen]<40)
	       h_ele_Eta_Gen_lowPt->Fill(GenParticleEta[igen]);
	   }
       }

     TVector3 ele1gen, ele2gen;
     ele1gen.SetPtEtaPhi(GenParticlePt[from_LQ_idx[0]],
 			   GenParticleEta[from_LQ_idx[0]],
 			   GenParticlePhi[from_LQ_idx[0]]);	
 	
     ele2gen.SetPtEtaPhi(GenParticlePt[from_LQ_idx[1]],
 			   GenParticleEta[from_LQ_idx[1]],
 			   GenParticlePhi[from_LQ_idx[1]]);	
 	
     ///////Calculating DeltaR and matching
     float minDeltaR1 = 99;
     float minDeltaR2 = 99;
     float min2ndDeltaR1 = 99;
     float min2ndDeltaR2 = 99;
     int RecoIndex1 = 99;
     int RecoIndex2 = 99;

     for (int iele=0; iele<eleCount; iele++) {
       if (elePt[iele]<elePt_cut) continue;
       TVector3 ele;
       ele.SetPtEtaPhi(elePt[iele],
 	       eleEta[iele],
 	       elePhi[iele]);	
 
       float DeltaR_ele_ele1gen = ele1gen.DeltaR(ele);
       float DeltaR_ele_ele2gen = ele2gen.DeltaR(ele);

       if (DeltaR_ele_ele1gen < minDeltaR1) {
	 min2ndDeltaR1=minDeltaR1;
	 minDeltaR1=DeltaR_ele_ele1gen;
	 RecoIndex1 = iele;
       }
       else if (DeltaR_ele_ele1gen < min2ndDeltaR1){
	 min2ndDeltaR1=DeltaR_ele_ele1gen;
       }
       if (DeltaR_ele_ele2gen < minDeltaR2) {
	 min2ndDeltaR2=minDeltaR2;
	 minDeltaR2=DeltaR_ele_ele2gen;
	 RecoIndex2 = iele;
       }
       else if (DeltaR_ele_ele2gen < min2ndDeltaR2){
	 min2ndDeltaR2=DeltaR_ele_ele2gen;
       }
     }
     h_DeltaR_Gen_Reco->Fill(minDeltaR1);
     h_DeltaR_Gen_Reco->Fill(minDeltaR2);
     h_DeltaR_Gen_2ndReco->Fill(min2ndDeltaR1);
     h_DeltaR_Gen_2ndReco->Fill(min2ndDeltaR2);
     if (minDeltaR1< ConeSizeMCmatch_cut) matched_reco_idx[0]=RecoIndex1;
     if (minDeltaR2< ConeSizeMCmatch_cut) matched_reco_idx[1]=RecoIndex2;
//      cout << "GenElec: " << from_LQ_idx[0] << " matched to: " << matched_reco_idx[0] << endl;
//      cout << "GenElec: " << from_LQ_idx[1] << " matched to: " << matched_reco_idx[1] << endl;
//      cout << "////////////////////" << endl;


     /////////Fill histograms appropriately
     for (int iele=0; iele<eleCount; iele++) {
       if (elePt[iele]<elePt_cut) continue;
       nEle_pre++;
       bool EleIsMatched = false;
       bool InBarrel = false;
       bool InEndCap = false;
       bool PassID = false;
       bool PassISO = false;
       int igen_matched = -1;

       h_ele_E->Fill(eleEnergy[iele]);
       h_ele_Pt->Fill(elePt[iele]);
       h_ele_Phi->Fill(elePhi[iele]);
       h_ele_Eta->Fill(eleEta[iele]);
       h_ele_CaloEnergy->Fill(eleCaloEnergy[iele]);

     ///////////do matching 
       TVector3 ele;
       ele.SetPtEtaPhi(elePt[iele],
 	       eleEta[iele],
 	       elePhi[iele]);	
 
       float DeltaR_ele_ele1gen = ele1gen.DeltaR(ele);
       float DeltaR_ele_ele2gen = ele2gen.DeltaR(ele);
 
       if((iele==matched_reco_idx[0])||(iele==matched_reco_idx[1])) EleIsMatched = true;
       if(iele==matched_reco_idx[0])igen_matched=from_LQ_idx[0] ;
       if(iele==matched_reco_idx[1])igen_matched=from_LQ_idx[1] ;
       if ((DeltaR_ele_ele1gen<ConeSizeMCmatch_cut)&&(DeltaR_ele_ele2gen<ConeSizeMCmatch_cut)) n_ele_doubleMatched++;
 
       if (EleIsMatched == true){
	 h_ele_Pt_Gen_matched->Fill(GenParticlePt[igen_matched]);
	 h_ele_Eta_Gen_matched->Fill(GenParticleEta[igen_matched]);
	 if (GenParticlePt[igen_matched]<40)
	   h_ele_Eta_Gen_matched_lowPt->Fill(GenParticleEta[igen_matched]);
	 h_ele_E_matched->Fill(eleEnergy[iele]);
	 h_ele_Pt_matched->Fill(elePt[iele]);
	 h_ele_Phi_matched->Fill(elePhi[iele]);
	 h_ele_Eta_matched->Fill(eleEta[iele]);
	 h_ele_CaloEnergy_matched->Fill(eleCaloEnergy[iele]);
	 nEleMatched++;
	 h_Energy_Res->Fill(eleEnergy[iele]/GenParticleE[igen_matched]);
 	 //cout << "GenEle " << igen_matched << " eta: " << GenParticleEta[igen_matched] << " is match to RecoEle " << iele << " eta: " << eleEta[iele] << endl;
       }

       //check for barrel or endcap
       float electronEta = sqrt(eleEta[iele]*eleEta[iele]);
       if (electronEta<1.422)InBarrel = true;
       if ((electronEta>1.560)&&(electronEta<2.5))InEndCap = true;
 
       ////matched
       if (EleIsMatched){
         if (InBarrel){
	   h_eleHoE_barrel_matched->Fill(eleHoE[iele]);
	   h_eleSigmaEE_barrel_matched->Fill(eleSigmaEE[iele]);
	   h_eleDeltaPhiTrkSC_barrel_matched->Fill(eleDeltaPhiTrkSC[iele]);
	   h_eleDeltaEtaTrkSC_barrel_matched->Fill(eleDeltaEtaTrkSC[iele]);
	   h_eleClassif_barrel_matched->Fill(eleClassif[iele]);
	   h_eleNumTrkIso_barrel_matched->Fill(eleNumTrkIso[iele]);
	   h_eleTrkIso_barrel_matched->Fill(eleTrkIso[iele]);
	   h_eleEcalRecHitIso_barrel_matched->Fill(eleEcalRecHitIso[iele]);
	   h_Energy_Res_barrel->Fill(eleEnergy[iele]/GenParticleE[igen_matched]);
         }
  
         if (InEndCap){
	   h_eleHoE_endcap_matched->Fill(eleHoE[iele]);
	   h_eleSigmaEE_endcap_matched->Fill(eleSigmaEE[iele]);
	   h_eleDeltaPhiTrkSC_endcap_matched->Fill(eleDeltaPhiTrkSC[iele]);
	   h_eleDeltaEtaTrkSC_endcap_matched->Fill(eleDeltaEtaTrkSC[iele]);
	   h_eleClassif_endcap_matched->Fill(eleClassif[iele]);
	   h_eleNumTrkIso_endcap_matched->Fill(eleNumTrkIso[iele]);
	   h_eleTrkIso_endcap_matched->Fill(eleTrkIso[iele]);
	   h_eleEcalRecHitIso_endcap_matched->Fill(eleEcalRecHitIso[iele]);
	   h_Energy_Res_endcap->Fill(eleEnergy[iele]/GenParticleE[igen_matched]);
         }
       }
       
       else { ////unmatched
	 //cout << "Found unmatched ele" << endl;
	 if (InBarrel){
	   h_eleHoE_barrel_unmatched->Fill(eleHoE[iele]);
	   h_eleSigmaEE_barrel_unmatched->Fill(eleSigmaEE[iele]);
	   h_eleDeltaPhiTrkSC_barrel_unmatched->Fill(eleDeltaPhiTrkSC[iele]);
	   h_eleDeltaEtaTrkSC_barrel_unmatched->Fill(eleDeltaEtaTrkSC[iele]);
	   h_eleClassif_barrel_unmatched->Fill(eleClassif[iele]);
	   h_eleNumTrkIso_barrel_unmatched->Fill(eleNumTrkIso[iele]);
	   h_eleTrkIso_barrel_unmatched->Fill(eleTrkIso[iele]);
	   h_eleEcalRecHitIso_barrel_unmatched->Fill(eleEcalRecHitIso[iele]);
	 }

	 if (InEndCap){
	   h_eleHoE_endcap_unmatched->Fill(eleHoE[iele]);
	   h_eleSigmaEE_endcap_unmatched->Fill(eleSigmaEE[iele]);
	   h_eleDeltaPhiTrkSC_endcap_unmatched->Fill(eleDeltaPhiTrkSC[iele]);
	   h_eleDeltaEtaTrkSC_endcap_unmatched->Fill(eleDeltaEtaTrkSC[iele]);
	   h_eleClassif_endcap_unmatched->Fill(eleClassif[iele]);
	   h_eleNumTrkIso_endcap_unmatched->Fill(eleNumTrkIso[iele]);
	   h_eleTrkIso_endcap_unmatched->Fill(eleTrkIso[iele]);
	   h_eleEcalRecHitIso_endcap_unmatched->Fill(eleEcalRecHitIso[iele]);
	 }
       } // end else unmatched

       ///check for ID 
       if (InBarrel)
 	 if ((eleHoE[iele]<eleHoE_barrel_cut)&&(eleSigmaEE[iele]<eleSigmaEE_barrel_cut)
 	     &&(eleDeltaPhiTrkSC[iele]*eleDeltaPhiTrkSC[iele])<(eleDeltaPhiTrkSC_barrel_cut*eleDeltaPhiTrkSC_barrel_cut)
 	     &&(eleDeltaEtaTrkSC[iele]*eleDeltaEtaTrkSC[iele])<(eleDeltaEtaTrkSC_barrel_cut*eleDeltaEtaTrkSC_barrel_cut)
	     //&&(eleClassif[iele])<40
	     )
 	   PassID=true;
 
       if (InEndCap)
 	 if ((eleHoE[iele]<eleHoE_endcap_cut)&&(eleSigmaEE[iele]<eleSigmaEE_endcap_cut)
 	     &&(eleDeltaPhiTrkSC[iele]*eleDeltaPhiTrkSC[iele])<(eleDeltaPhiTrkSC_endcap_cut*eleDeltaPhiTrkSC_endcap_cut)
 	     &&(eleDeltaEtaTrkSC[iele]*eleDeltaEtaTrkSC[iele])<(eleDeltaEtaTrkSC_endcap_cut*eleDeltaEtaTrkSC_endcap_cut)
	     //&&(eleClassif[iele])>100
	     )
 	   PassID=true;
 
       if (PassID) nEle_post_ID++;
 
       if ((EleIsMatched)&&(PassID)){
 	 h_ele_Pt_Gen_matched_ID->Fill(GenParticlePt[igen_matched]);
 	 h_ele_Eta_Gen_matched_ID->Fill(GenParticleEta[igen_matched]);
       }


       ///check for ISO
       if (InBarrel)
	 if ((eleNumTrkIso[iele]<eleNumTrkIso_barrel_cut)&&(eleTrkIso[iele]<eleTrkIso_barrel_cut)
 	   &&(eleEcalRecHitIso[iele]<(eleEcalRecHitIso_barrel_cut+(0.05*elePt[iele]))))
 	   PassISO=true;
 
       if (InEndCap)
	 if ((eleNumTrkIso[iele]<eleNumTrkIso_endcap_cut)&&(eleTrkIso[iele]<eleTrkIso_endcap_cut)
 	   &&(eleEcalRecHitIso[iele]<(eleEcalRecHitIso_endcap_cut+(0.10*elePt[iele]))))
 	   PassISO=true;
 
 
       if ((EleIsMatched)&&(PassID)&&(PassISO)){
 	 h_ele_Pt_Gen_matched_ID_ISO->Fill(GenParticlePt[igen_matched]);
 	 h_ele_Eta_Gen_matched_ID_ISO->Fill(GenParticleEta[igen_matched]);
         }
 
       if ((PassISO)&&(PassID)) {
 	 nEle_post_ID_ISO++;
 	 if (FirstLoop){
 	 h_ele_E_ID_ISO->Fill(eleEnergy[iele]);
 	 h_ele_Pt_ID_ISO->Fill(elePt[iele]);
 	 h_ele_Phi_ID_ISO->Fill(elePhi[iele]);
 	 h_ele_Eta_ID_ISO->Fill(eleEta[iele]);
 	 h_ele_CaloEnergy_ID_ISO->Fill(eleCaloEnergy[iele]);
 	 }
     }
  }// end ele loop
 
   h_N_ele_Pre->Fill(nEle_pre);
   h_N_ele_Post_ID->Fill(nEle_post_ID);
   h_N_ele_Post_ID_ISO->Fill(nEle_post_ID_ISO);
 
   h_N_ele_matched->Fill(nEleMatched);
   h_N_ele_Gen->Fill(nGenEle);
     //cout << "///////////////////////////////// Next Event /////////////////" << endl;
   }

     /////////////Loop to fill ID and ISO variable histos /////////////////
//      for (int iele=0; iele<eleCount; iele++) {
//        if (elePt[iele]<elePt_cut) continue;
//        bool EleIsMatched = false;

//        ///////////do matching 
//        TVector3 ele;
//        ele.SetPtEtaPhi(elePt[iele],
//  		       eleEta[iele],
//  		       elePhi[iele]);	
 
//        float DeltaR_ele_elegen = elegen.DeltaR(ele);
//        if(DeltaR_ele_elegen<ConeSizeMCmatch_cut) EleIsMatched = true;
 

//      } // end ele loop

     ////////////////////// User's code ends here ///////////////////////
   

   //////////write histos 
   h_ele_E->Write();
   h_ele_Pt->Write();
   h_ele_Phi->Write();
   h_ele_Eta->Write();
   h_ele_CaloEnergy->Write();

   h_N_ele_matched->Write();
   h_ele_E_matched->Write();
   h_ele_Pt_matched->Write();
   h_ele_Phi_matched->Write();
   h_ele_Eta_matched->Write();
   h_ele_CaloEnergy_matched->Write();

   h_ele_E_ID_ISO->Write();
   h_ele_Pt_ID_ISO->Write();
   h_ele_Phi_ID_ISO->Write();
   h_ele_Eta_ID_ISO->Write();
   h_ele_CaloEnergy_ID_ISO->Write();

   h_N_ele_Gen->Write();
   h_ele_Pt_Gen->Write();
   h_ele_Pt_Gen_etaCut->Write();
   h_ele_Pt_Gen_matched->Write();
   h_ele_Pt_Gen_matched_ID->Write();
   h_ele_Pt_Gen_matched_ID_ISO->Write();
   h_ele_Eta_Gen->Write();
   h_ele_Eta_Gen_lowPt->Write();
   h_ele_Eta_Gen_etaCut->Write();
   h_ele_Eta_Gen_matched->Write();
   h_ele_Eta_Gen_matched_lowPt->Write();
   h_ele_Eta_Gen_matched_ID->Write();
   h_ele_Eta_Gen_matched_ID_ISO->Write();
   h_ele_Eta_Pt_Gen->Write();

   h_DeltaR_Gen_Reco->Write();
   h_DeltaR_Gen_2ndReco->Write();

   h_N_ele_Pre->Write();
   h_N_ele_Post_ID->Write();
   h_N_ele_Post_ID_ISO->Write();

   h_Energy_Res->Write();
   h_Energy_Res_barrel->Write();
   h_Energy_Res_endcap->Write();

   ////matched
   h_eleHoE_barrel_matched->Write();
   h_eleSigmaEE_barrel_matched->Write();
   h_eleDeltaPhiTrkSC_barrel_matched->Write();
   h_eleDeltaEtaTrkSC_barrel_matched->Write();
   h_eleClassif_barrel_matched->Write();
   h_eleNumTrkIso_barrel_matched->Write();
   h_eleTrkIso_barrel_matched->Write();
   h_eleEcalRecHitIso_barrel_matched->Write();

   h_eleHoE_endcap_matched->Write();
   h_eleSigmaEE_endcap_matched->Write();
   h_eleDeltaPhiTrkSC_endcap_matched->Write();
   h_eleDeltaEtaTrkSC_endcap_matched->Write();
   h_eleClassif_endcap_matched->Write();
   h_eleNumTrkIso_endcap_matched->Write();
   h_eleTrkIso_endcap_matched->Write();
   h_eleEcalRecHitIso_endcap_matched->Write();

   ///unmathced
   h_eleHoE_barrel_unmatched->Write();
   h_eleSigmaEE_barrel_unmatched->Write();
   h_eleDeltaPhiTrkSC_barrel_unmatched->Write();
   h_eleDeltaEtaTrkSC_barrel_unmatched->Write();
   h_eleClassif_barrel_unmatched->Write();
   h_eleNumTrkIso_barrel_unmatched->Write();
   h_eleTrkIso_barrel_unmatched->Write();
   h_eleEcalRecHitIso_barrel_unmatched->Write();

   h_eleHoE_endcap_unmatched->Write();
   h_eleSigmaEE_endcap_unmatched->Write();
   h_eleDeltaPhiTrkSC_endcap_unmatched->Write();
   h_eleDeltaEtaTrkSC_endcap_unmatched->Write();
   h_eleClassif_endcap_unmatched->Write();
   h_eleNumTrkIso_endcap_unmatched->Write();
   h_eleTrkIso_endcap_unmatched->Write();
   h_eleEcalRecHitIso_endcap_unmatched->Write();

   h_eleEff_Pt->Divide(h_ele_Pt_Gen_matched,h_ele_Pt_Gen,1,1);
   h_eleEff_Pt->Write();
   h_eleEff_Eta->Divide(h_ele_Eta_Gen_matched,h_ele_Eta_Gen,1,1);
   h_eleEff_Eta->Write();

   h_eleEff_Pt_ID->Divide(h_ele_Pt_Gen_matched_ID,h_ele_Pt_Gen,1,1);
   h_eleEff_Pt_ID->Write();
   h_eleEff_Eta_ID->Divide(h_ele_Eta_Gen_matched_ID,h_ele_Eta_Gen,1,1);
   h_eleEff_Eta_ID->Write();

   h_eleEff_Pt_ID_ISO->Divide(h_ele_Pt_Gen_matched_ID_ISO,h_ele_Pt_Gen,1,1);
   h_eleEff_Pt_ID_ISO->Write();
   h_eleEff_Eta_ID_ISO->Divide(h_ele_Eta_Gen_matched_ID_ISO,h_ele_Eta_Gen,1,1);
   h_eleEff_Eta_ID_ISO->Write();

   cout << "Number of events where recoEle matches BOTH genEle: " << n_ele_doubleMatched << endl;
   std::cout << "analysisClass::Loop() ends" <<std::endl;   
}
