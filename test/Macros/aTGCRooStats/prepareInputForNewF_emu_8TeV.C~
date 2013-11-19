void prepareInputForNewF_4e_8TeV(){

  //  double bins[6]={100.,200.,300.,400.,600.,800.};
  double bins[11]={100.,200.,300.,400.,500.,600.,700.,800.,900.,1000.,1100.};

  ///////////////////////////////////////////////////////////////
  //
  //                      SM
  //


  TFile * file_signal=new TFile("/afs/cern.ch/work/s/senka/aTGC_ZZ_8TeV_corrected/f5_eeee_aTGC_f5Zf5G_take2.root","read");
  file_signal->cd();
  TH1F * histo_SM=new TH1F("histo_SM","histo_SM",10,bins);
  eeeeFinalhighMass->Draw("mass>>histo_SM","weight*(f5z_grid==0.&&f5g_grid==0.)");
  int N_bins=histo_SM->GetNbinsX();
  double yield=histo_SM->GetBinContent(N_bins)+histo_SM->GetBinContent(N_bins+1);
  histo_SM->SetBinContent(N_bins,yield);


  ///////////////////////////////////////////////////////////////
  //
  //                      data
  //


  TFile * file_data=new TFile("/afs/cern.ch/work/s/senka/aTGC_ZZ_8TeV_corrected/DATA_2012_final_selected.root","read");
  file_data->cd();
  TH1F * histo_data=new TH1F("histo_data","histo_data",10,bins);
  eeeeFinalhighMass->Draw("mass>>histo_data","");
  yield=histo_data->GetBinContent(N_bins)+histo_data->GetBinContent(N_bins+1);
  histo_data->SetBinContent(N_bins,yield);

  ///////////////////////////////////////////////////////////////
  //
  //                      bkg
  //

  TFile * file_bkg=new TFile("/afs/cern.ch/work/s/senka/aTGC_ZZ_8TeV_corrected/DATA_2012_final_selected.root","read");
  file_bkg->cd();
  TH1F * histo_bkg=new TH1F("histo_bkg","histo_bkg",5,bins);
  histo_bkg=eeeeBG;
  yield=histo_bkg->GetBinContent(N_bins)+histo_bkg->GetBinContent(N_bins+1);
  histo_bkg->SetBinContent(N_bins,yield);
  
  ///////////////////////////////////////////////////////////////
  //
  //                      output
  //

  TFile * file_out=new TFile("e4_boosted_8TeV.root","recreate");
  file_out->cd();
  histo_SM->Write("diboson");
  histo_data->Write("data_obs");
  histo_bkg->Write("background");
  histo_bkg->Scale(1.3);
  histo_bkg->Write("background_elboosted_backshapeUp");
  histo_bkg->Scale(0.7/1.3);
  histo_bkg->Write("background_elboosted_backshapeDown");

}
