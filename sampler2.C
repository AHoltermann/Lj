#include <complex>
#include <tuple>
#include <sstream>
#include <fstream>
#include <iostream>
#include <string>
#include <cmath>
#include <vector>

#include "lj.C"

double sample(
double v2 = 0,
double v3 = 0, 
double psi2 = 0, 
double psi3 = 0, 
double r = 5,
double x1 = 0,
double x2 = 0     
){
    double pi = 3.14159265358979;
    
    gRandom->SetSeed();
    TRandom *rand = new TRandom2();
    rand->SetSeed();

    double theta_p = rand->Rndm() * 2 * pi - pi;
    double r_p = rand->Rndm()*shape(v2,v3,psi2,psi3,r,theta_p);

    double x_p = r_p*cos(theta_p);
    double y_p = r_p*sin(theta_p);

    double a = rand->Rndm() * 2 * pi - pi;
    double a2 = a + pi;
    if(a2 > pi){
        a2 -= 2*pi;
    }

    double l1 = lj(v2,v3,psi2,psi3,r,x_p,y_p,a);
    double l2 = lj(v2,v3,psi2,psi3,r,x_p,y_p,a2);

    l1 *= x1;
    l2 *= x2;

    double ls = 0;
    double ll = 0;

    if(l1 > l2){
        ll = l1;
        ls = l2;
    }
    else{
        ls = l1;
        ll = l2;
    }
    return (ls/ll);
}

void sampler2(
int n_iters = 100000,
double v2 = 0.3,
double v3 = 0.02, 
double psi2 = 0, 
double psi3 = 0, 
double r = 5)
{
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
    
    f->Close();
   
    double lj;
    TH1D*dist = new TH1D("L_{j} Distribution","L_{j} Distribution",100,0,1);
    for(int i = 0; i<n_iters; i++){
    lj = sample(v2,v3,psi2,psi3,r,x1s[i],x2s[i]); 
    //cout << lj << endl;
    dist->Fill(lj);
    if(i % 100 == 0){
        cout << (i*1.0)/n_iters << endl;
    }
    }

    TFile*out = new TFile("out.root","RECREATE");
    out->cd();
    dist->Write();

}

