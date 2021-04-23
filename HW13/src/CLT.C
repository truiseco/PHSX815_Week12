/*************************************************************
* @author   Triston Ruiseco
* @file     CLT.C
* @date     04/23/2021
* @brief    Script for generating a visualization of the
            manifestation of the central limit theorem
            as the number of observations averaged per
            sample is increased.
*************************************************************/

// C++ Standard Library Includes
#include <iostream>
#include <sstream>

// ROOT Includes
#include "TH1D.h"
#include "TRandom3.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TColor.h"

// Begin primary program function
int main(int argc, char** argv){

  // Experiment simulation routine parameters
  int spe = 10000000;      // Measurements per experiment
  int iops = 1;           // Initial observations averaged per sample
  int mops = 256;         // Maximum observations averaged per sample

  // Distribution parameters and tracking variables
  double rate = 0.05131;
  double sample_sum = 0.;
  double sample_avg = 0.;

  // ROOT visualization objects and helper variables
  int cd = 1;
  int bins = 250;
  std::string namestr = "\0";
  TCanvas* canvas = new TCanvas("canvas1","canvas1", 1920, 1200);

  // ROOT visualization constant preferences
  canvas->Divide(3, 3, 0, 0);
  gStyle->SetOptStat(kFALSE);

  // Array of groovy colors for histogram fill
  Int_t colors[9] = {TColor::GetColor("#f58582"),
                       TColor::GetColor("#dd9a44"),
                       TColor::GetColor("#7abd42"),
                       TColor::GetColor("#51c788"),
                       TColor::GetColor("#50c6ba"),
                       TColor::GetColor("#4ebaef"),
                       TColor::GetColor("#9b9afe"),
                       TColor::GetColor("#e876f0"),
                       TColor::GetColor("#fb74b7")};

  for(int ops = iops; ops <= mops; ops *= 2){ // Loop exponentially across
                                              // observations averaged per
                                              // sample
    // Unique ROOT histogram name
    std::stringstream name;
    name << "N = " << ops;
    namestr = name.str();

    // ROOT hist for data storage and visualization
    TH1D* hist = new TH1D(namestr.c_str(),namestr.c_str(),bins,0.,0.1);

    // Run and record one experiemt (spe samples of ops averaged observations)
    for(int s = 0; s < spe; s++){ // Loop linearly across samples
      sample_sum = 0;
      for(int o = 0; o < ops; o++){ // Loop linearly across observations
        sample_sum += gRandom->Exp(rate);
      }
      sample_avg = sample_sum / double(ops);
      hist->Fill(sample_avg);
    }

    // Pad-dependent visualization adjustments
    canvas->cd(cd);
    if(cd < 7){
      hist->GetXaxis()->SetTickLength(0);
    }
    if(!(cd == 1 || cd == 4 || cd == 7)){
      hist->GetYaxis()->SetTickLength(0);
    }
    hist->SetLineColor(colors[cd-1]);
    hist->SetFillColor(colors[cd-1]);

    // Pad-independent visualization adjustments
    hist->GetXaxis()->SetLimits(0.,0.1);
    hist->Scale(1./hist->Integral());
    hist->SetMaximum(0.06);
    hist->SetMinimum(0.);
    hist->SetFillStyle(1001);

    // Cache finalized histogram
    hist->Draw("HISTC");
    canvas->Update();
    cd++;
  }

  // Export final graphic
  canvas->SaveAs("CLT.png");

  return 0;
}
