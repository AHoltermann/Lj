#include <complex>
#include <tuple>
#include <sstream>
#include <fstream>
#include <iostream>
#include <string>
#include <cmath>
#include <vector>

double trentolength(const char* outfile = "small.root",int nevent = 48, double x = 66.1462, double y = 62.4062, double a =  -1.89224, double p = 0.5){

double pi = 3.14159265358979;

// a given in radians
TFile*f = new TFile(outfile);
std::string eventString = "event_" + std::to_string(nevent);
const char* string = eventString.c_str();
//cout << string << endl;
TH2D*h = (TH2D*)f->Get(string);

double l = 0;

double dx = 0;
double dy = 0;
double ang = 0;

for(int i = 0; i < h->GetNbinsX(); i++){
    for(int j = 0; j<h->GetNbinsY(); j++){
        
        dx = h->GetXaxis()->GetBinCenter(i) - x;
        dy = h->GetYaxis()->GetBinCenter(j) - y;
       
        if(dx > 0 && dy > 0){
            ang = atan2(abs(dy),abs(dx));
        } 
        else if(dx < 0 && dy > 0){
            ang = pi - atan2(abs(dy),abs(dx));
        } 
        else if(dx < 0 && dy < 0){
            ang = -1*pi + atan2(abs(dy),abs(dx));
        }
        else if(dx > 0 && dy < 0){
            ang =  -1*atan2(abs(dy),abs(dx));
        }   
        if((abs(fmod((ang-a),(2*pi))) < 0.2)||(2*pi - abs(fmod((ang-a),(2*pi))) < 0.2)){
        l+= pow(h->GetBinContent(i,j),p);
        //h->SetBinContent(h->GetBin(i,j),0);
     
        }   
    }
}


//h->Draw("colz");
//cout << l << endl;

f->Close();

return l;
}

