#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <iomanip>
#include <vector>
#include <tuple>
#include <numeric>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <random>
//#include "xjsample.cpp"
#include "xjmetric.cpp"

using namespace std;

string filename(string tag, int number){

    std::ostringstream filename;
    filename << tag << number << ".dat";
    return filename.str();
}

class datapoint{

    public:
        vector<double> c; // c vector
        double dt; // timestep
        int events; // should be up to +5000 trento events used
        int jetsamples; // jets/event 
        int subsamples; // subdivisions of the xj_sample vector to be made
        string name;

    datapoint(vector<double> cvec,string names){
        c = cvec;
        dt = 1;
        events = 5000;
        jetsamples = 20;
        subsamples = 10;
        name = names;
    }

    datapoint(vector<double> cvec,double timestep,int nevents,int jetsampless,int subsampless,string names){
        c = cvec;
        dt = timestep;
        events = nevents;
        jetsamples = jetsampless;
        subsamples = subsampless;
        name = names;
    }

    vector<double> xjsample(vector<double> c_lim,vector<vector<double>> data){
        vector<double> xjs = xj_sample(c,c_lim,dt,events,jetsamples,data);
        return xjs;
    }

    vector<vector<vector<double>>> coag(vector<double> xj_sample){
        return coag_data(xj_sample,subsamples);
    
    }

    vector<vector<double>> mean_data(vector<vector<vector<double>>> dists){
        return xj_mean(dists);
    }

    vector<vector<double>> std_data(vector<vector<vector<double>>> dists){
        return xj_stddev(dists);
    }

    double likelihood(vector<vector<vector<double>>> dists,vector<vector<vector<vector<double>>>> values){
        return loglikelihood(dists,values,1.0*events*jetsamples/subsamples);
    }
    
    void filewrite(vector<vector<vector<double>>> dists,vector<vector<vector<vector<double>>>> values){

        ofstream file(name);

        file << "C Vector: ";
        for(int m = 0; m<c.size(); m++){
        file << c[m] << " ";
        }
        file << endl;

        double a = likelihood(dists,values);

        file << "dt: " << dt << endl;
        file << "events: " << events << endl;
        file << "dijets: " << events*jetsamples << endl;
        file << "subsamples: " << subsamples << endl;
        file << "Likelihood: " << a << endl;
        file << "Data: " << endl;

        vector<vector<double>> A = mean_data(dists);
        for(int i = 0; i< A.size(); i++){
            for(int j = 0; j<A[0].size(); j++){
            file << setw(12) << A[i][j];
            }
            file << endl;
        }

        file << "Errors: " << endl;

        vector<vector<double>> B = std_data(dists);
        for(int i = 0; i< B.size(); i++){
            for(int j = 0; j<B[0].size(); j++){
            file << setw(12) << B[i][j];
            }
            file << endl;
        }

        file.close();
    }
    
    double execute(vector<double> c_lim, vector<vector<double>> data, vector<vector<vector<vector<double>>>> values){

        vector<double> xj = xjsample(c_lim,data);
        vector<vector<vector<double>>> dists = coag(xj);
        filewrite(dists,values);

        return likelihood(dists,values);
    }

};

void dimscan(int idx, string filetag, vector<double> c_lim, vector<vector<double>> data, vector<vector<vector<vector<double>>>> values){

    string name;
    vector<double> c = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
    for(int i = 0; i<60){
        c[idx] = 0.05*i;
        name = filename(filetag,i);
        datapoint d(c,name);
        d.execute(c_lim,data,values);
    }
}

int main(){

     // Default c coeff values
    double c0 = 0.75;
    double c1 = 0.04;
    double c2 = 0.0012;
    double c3 = 0.005;
    double c4 = 0.0003;
    double c5 = 0.00001;
    double c6 = 0.00005;
    double c7 = 0.000003;
    double c8 = 0.0000001;
    double c9 = 0.8;
    double c10 = 0.05;
    double c11 = 0.002;
    double c12 = 0.0065;
    double c13 = 0.0005;
    double c14 = 0.00002;
    double c15 = 0.00006;
    double c16 = 0.000005;
    double c17 = 0.000004;
    double c18 = 0.5;
    double c19 = 0.03;
    double c20 = 0.001;
    double c21 = 0.0035;
    double c22 = 0.00025;
    double c23 = 0.000015;
    double c24 = 0.00004;
    double c25 = 0.000004;
    double c26 = 0.0000002;

    vector<vector<vector<vector<double>>>> values = importdata();
    vector<double> c_lim = {c0,c1,c2,c3,c4,c5,c6,c7,c8,c9,c10,c11,c12,c13,c14,c15,c16,c17,c18,c19,c20,c21,c22,c23,c24,c25,c26};
    vector<vector<double>> eprof = eprofile("../energies.dat");    

    dimscan(0,"c000_",c_lim,eprof,values);
    

return 0;
}
