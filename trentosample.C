#include <complex>
#include <tuple>
#include <sstream>
#include <fstream>
#include <iostream>
#include <string>
#include <cmath>
#include <vector>

#include "trentolength.C"


void trentosample(const char* out = "small.root",int n_iters = 1000, int e = 50, double p = 0.0){

    //init histograms
    TH1D*dist = new TH1D("L_{j} Distribution","L_{j} Distribution",100,0,1);
    TH1D*dist2 = new TH1D("L_{jg} Distribution","L_{jg} Distribution",100,0,2);

    //init random numbers 
    double pi = 3.14159265358979;
    gRandom->SetSeed();
    TRandom *rand = new TRandom2();
    rand->SetSeed();

    // sample pp data 
    double x1s [n_iters];
    double x2s [n_iters];
    TFile*f = new TFile("proton_hist.root");
    TH2D* xj_proton = (TH2D*)f->Get("hpt1pt2");
    double x1,x2;
    for(int i = 0; i<n_iters; i++){
         xj_proton->GetRandom2(x1,x2);
         x1s[i] = x1;
         x2s[i] = x2;
    }
    
    //get initial state
    TFile*g = new TFile(out);
    std::string eventString = "event_" + std::to_string(e);
    const char* string = eventString.c_str();
    TH2D*h = (TH2D*)g->Get(string);
    double x,y;
    
    for(int eg = 0; e < 100; e++){

    for(int j = 0; j<1000; j++){
        if(j%100==0){cout << j << endl;}
        //get random x,y in initial state
        h->GetRandom2(x,y);
        if(h->GetBinContent(h->FindBin(x,y)) < 0.02){continue;}

        //pick random angle, angle + pi
        double a = rand->Rndm() * 2 * pi - pi;
        double a2 = a + pi;
        if(a2 > pi){
            a2 -= 2*pi;
        }

        //evaluate length and photon 
        double l1 = x1s[j]/(trentolength(out,e,x,y,a,p));
        double l2 = x2s[j]/(trentolength(out,e,x,y,a2,p));
        //double l1 = 1/(trentolength(out,eg,x,y,a,p));
        //double l2 = 1/(trentolength(out,eg,x,y,a2,p));

        double lg = x2s[j+1];

        //cout << x << ", " << y << ", " << a << ", " << a2 << ", " << endl;

        double lj = 0;
        double ljg = 0;

        //evaluate subleading and define lj
        if(l1 > l2){
            lj = l2/l1;
        }
        else{
            lj = l1/l2;
        }
        dist->Fill(lj);
        //evaluate subleading and define ljg
        if(lg > l1){
            ljg = l1/lg;
        }
        else{
            ljg = lg/l1;
        }
        dist2->Fill(ljg);
    }
    }


    TFile*out2 = new TFile("t2.root","UPDATE");
    out2->cd();
    //dist->Write();
    //dist2->Write();
    dist->Draw();
    //dist2->Draw();

    f->Close();
    g->Close();

    }



