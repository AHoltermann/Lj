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
        events = 1;
        jetsamples = 100;
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

    vector<double> xjsample(vector<double> c_lim){
        cout << "xj_sample" << endl;
        vector<double> xjs = xj_sample(c,c_lim,dt,events,jetsamples);
        return xjs;
    }

    vector<vector<vector<double>>> coag(vector<double> xj_sample){
        cout << "coag" << endl;
        return coag_data(xj_sample,subsamples);
    
    }

    vector<vector<double>> mean_data(vector<vector<vector<double>>> dists){
        cout << "averaging" << endl;
        return xj_mean(dists);
    }

    vector<vector<double>> std_data(vector<vector<vector<double>>> dists){
        cout << "variances" << endl;
        return xj_stddev(dists);
    }

    double likelihood(vector<vector<vector<double>>> dists,vector<vector<vector<vector<double>>>> values){
        cout << "llh" << endl;
        //return loglikelihood(dists,values,1.0*events*jetsamples/subsamples);
        return residuals(dists,values,1.0*events*jetsamples/subsamples);
    }
    
    void filewrite(vector<vector<vector<double>>> dists,vector<vector<vector<vector<double>>>> values){
        cout << "filewrite" << endl;
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
        //file << "Residuals: " << residuals(dists,values,1.0*events*jetsamples/subsamples) << endl;
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
    
    double execute(vector<double> c_lim, vector<vector<vector<vector<double>>>> values){
        vector<double> xj = xjsample(c_lim);
        vector<vector<vector<double>>> dists = coag(xj);
        filewrite(dists,values);

        return likelihood(dists,values);
    }

};

void dimscan(int idx, string filetag, vector<double> c_lim, vector<vector<vector<vector<double>>>> values){

    string name;
    vector<double> c = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    for(int i = 0; i<60; i++){
        c[idx] = 0.05*i;
        name = filename(filetag,i);
        datapoint d(c,name);
        d.execute(c_lim,values);
    }
}

/*vector<double> ceiling(vector<double> c,vector<double> c_lim){

    for(int i = 0; i<c.size(); i++){
        if(c[i] < -5*c_lim[i]) {
            c[i] = -5*c_lim[i];
        }
        else if(c[i] > 10*c_lim[i]){
            c[i] = 10*c_lim[i];
        }
    }
    return c;
}*/

void gradient_descent(string filetag, vector<double> c_init, vector<double> c_lim,vector<vector<vector<vector<double>>>> values, int steps){

    string name;
    name = filename(filetag,1);
    vector<double> c_old = c_init;
    vector<double> c_new = c_init;
    datapoint d(c_new,name);

    double old_value = d.execute(c_lim,values);
    double value = 0;
    double totalgrad = 0;
    vector<double> gradient(c_lim.size(),0);

    ofstream file(filename(filetag,0));
    

    for(int i = 0; i<steps; i++){
        
        //write stuff to file
        cout << "c old timestep" << endl;
        file << setw(10) << old_value;
        for(int i = 0; i<c_lim.size(); i++){
            file << setw(10) << c_old[i];
            cout << c_old[i] << " ";
        }
        file << endl;
        cout << endl;
        
        cout << "c new timestep" << endl;
        //update position according to gaussian proposal
        vector<double> rands = gauss_randoms(c_lim.size());
        for(int i = 0; i<c_lim.size(); i++){
            c_old[i] += 0.01*rands[i];
            cout << c_old[i] << " ";
        }
        //c_old[i] = ceiling(c_old[i],c_lim);
        //evaluate function at this point
        name = filename(filetag,i+2);
        datapoint q(c_old,name);
        value = q.execute(c_lim,values);
        cout << "old value" << old_value << endl;
        cout << "new value" << value << endl;

        if(value > old_value){

            cout << " step not accepted, going back... " << endl;

            for(int i = 0; i<c_lim.size(); i++){
                c_old[i] -= 0.005*rands[i];
            }
        }
        else{
        //calculate gradient
            cout << " step accepted!" << endl;

            for(int i = 0; i<c_lim.size(); i++){
                gradient[i] = (value - old_value)/(0.01*rands[i]);
                if((gradient[i] >= 5000) || (gradient[i] <= -5000)){
                    gradient[i] = rands[i];   
                }
            }

            //move
            for(int i = 0; i<c_lim.size(); i++){
                c_old[i]-= 0.005*(gradient[i]);
                
            }
            datapoint m(c_old,name);
            old_value = m.execute(c_lim,values);  
            }

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
    //vector<vector<double>> eprof = eprofile("../energies.dat");    
    
    vector<double> c = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    //dimscan(0,"cjq_",c_lim,values);
    gradient_descent("tgd_",c,c_lim,values,100);
    //cout << xjs.size() << endl;
    //for(int i=0; i<xjs[0].size(); i++){
    //    cout << xjs[0][i] << endl;
    //}


    //printvec(data_dist(2000,values));
    //printvec(xjs);

    //vector<vector<double>> dists = xj_dist(qqbar);
    
    //datapoint cho(c,1,5000,10,5,"check.dat");
    //cho.execute(c_lim,values);



return 0;
}
