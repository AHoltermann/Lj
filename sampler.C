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
double r = 5      
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

void sampler(
int n_iters = 100000,
double v2 = 0.1,
double v3 = 0.02, 
double psi2 = 0, 
double psi3 = 0, 
double r = 5)
{
    double lj;
    TH1D*dist = new TH1D("L_{j} Distribution","L_{j} Distribution",100,0,1);
    for(int i = 0; i<n_iters; i++){
    lj = sample(v2,v3,psi2,psi3,r); 
    //cout << lj << endl;
    dist->Fill(lj);
    if(i % 100 == 0){
        cout << (i*1.0)/n_iters << endl;
    }
    }

    dist->Draw("colz");

}
