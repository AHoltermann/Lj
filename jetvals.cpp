#define _USE_MATH_DEFINES

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
#include "methods2.cpp"
#include <cmath>

struct JetDepData {
    std::vector<double> npart_c;
    std::vector<double> mult_c;
    std::vector<double> e2_c;
    std::vector<double> e3_c;
    std::vector<double> e4_c;
    std::vector<double> e5_c;
};

double getRandomValue() {
    std::random_device rd;
    std::mt19937 gen(rd());
    double lower_bound = 0.0;
    double upper_bound = 1.0;
    std::uniform_real_distribution<double> distribution(lower_bound, upper_bound);
    return distribution(gen);
}

std::vector<double> calc_eloss(std::string filename, double theta1, double theta2, double dt, std::vector<double> c, std::vector<double> c_lim){
    
    //initialize profile
    std::vector<std::vector<double> > myprof = profile(filename);
    std::vector<double> earray;
    std::tuple<int,int> energypicks = energypick(myprof);
    double E1 = 1.0*std::get<0>(energypicks);
    double E2 = 1.0*std::get<1>(energypicks);
    double pt1 = energyloss(c,c_lim,E1,theta1,myprof, dt);
    double pt2 = energyloss(c,c_lim,E2,theta2,myprof, dt);
    double m = (E1 - pt1)/E1;
    double m1 = (E2 - pt2)/E2;
    earray.push_back(m);
    earray.push_back(m1);

    return earray;
}

std::vector<double> calc_pathlength(std::string filename, double theta1, double dt) {
    //initialize profile
    std::vector<std::vector<double> > myprof = profile(filename);
    
    //create path and energy profile
    //path[0], stores the path and path[1] stores the energy
    std::vector<double> path = pathlength(theta1, myprof, dt);
    
    return path;
}

JetDepData jetdepmethod(const char* datFile) {
    
    JetDepData data;
    std::ifstream inputFile(datFile);
    if (!inputFile) {
        std::cerr << "Error opening file." << std::endl;
        // Returning empty vectors
        return data;
    }

    std::vector<double> npart_c, mult_c, e2_c, e3_c, e4_c, e5_c;
    double_t e_n, b, npart, mult, e2, e3, e4, e5;

    while (inputFile >> e_n >> b >> npart >> mult >> e2 >> e3 >> e4 >> e5) {
        for (int j = 0; j < 10; ++j) {
            npart_c.push_back(npart);
            mult_c.push_back(mult);
            e2_c.push_back(e2);
            e3_c.push_back(e3);
            e4_c.push_back(e4);
            e5_c.push_back(e5);
            std::cout << npart << std::endl;
        }
    }
    
    inputFile.close();

    data.npart_c = npart_c;
    data.mult_c = mult_c;
    data.e2_c = e2_c;
    data.e3_c = e3_c;
    data.e4_c = e4_c;
    data.e5_c = e5_c;

    return data;
    
}

int main(){
    //cvals
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

    std::vector<double> c_lim = {c0,c1,c2,c3,c4,c5,c6,c7,c8,c9,c10,c11,c12,c13,c14,c15,c16,c17,c18,c19,c20,c21,c22,c23,c24,c25,c26};
    std::vector<double> dc = {c0*0.01,c1*0.01,c2*0.01,c3*0.01,c4*0.01,c5*0.01,c6*0.01,c7*0.01,c8*0.01,c9*0.01,c10*0.01,c11*0.01,c12*0.01,c13*0.01,c14*0.01,c15*0.01,c16*0.01,c17*0.01,c18*0.01,c19*0.01,c20*0.01,c21*0.01,c22*0.01,c23*0.01,c24*0.01,c25*0.01,c26*0.01};
    std::vector<double> c = {0.5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    
    
    //get reference values for each event
    JetDepData data = jetdepmethod("/Volumes/Memorex USB/pbpb_10k.dat/pbpb_10k_header.dat");
    
    // Now we have 5 columns containing the values from the file
    std::vector<double> npart_c = data.npart_c;
    std::vector<double> mult_c = data.mult_c;
    std::vector<double> e2_c = data.e2_c;
    std::vector<double> e3_c = data.e3_c;
    std::vector<double> e4_c = data.e4_c;
    std::vector<double> e5_c = data.e5_c;

    //Generate the jets
    //Generate random angles
    double rand_val = getRandomValue();
    double theta1 = rand_val * (M_PI / 180);
    double theta2 = theta1 + M_PI;
    //Define timestep
    double dt = 1.0;
    //creating path and eweighted path length vectors   
    std::vector<double> path_l, ew_path, releloss, num_event;
    for (size_t i = 0; i < 10000; ++i) {
        for (int j = 0; j < 10; ++j) {
            std::vector<double> paths = calc_pathlength(eventstring(i), theta1, dt);
            //create energy loss for each jet
            std::vector<double> earray = calc_eloss(eventstring(i), theta1, theta2, dt, c, c_lim);

            releloss.push_back(earray[0]);
            releloss.push_back(earray[1]);
            path_l.push_back(paths[0]);
            ew_path.push_back(paths[1]);
            num_event.push_back(i);
        }
    }

    //Write out the output file
    std::ofstream outputFile("output10kt2.dat");

    if (!outputFile) {
        std::cerr << "Error opening file." << std::endl;
        return 1;
    }

    // Write the values of the vectors to the output file
    for (size_t i = 0; i < mult_c.size(); ++i) {
        if (npart_c[i] > 78.626455621) {
            outputFile << std::setw(10) << num_event[i]
                    << std::setw(15) << ew_path[i]
                    << std::setw(15) << path_l[i]
                    << std::setw(15) << releloss[i]
                    << std::setw(15) << npart_c[i]
                    << std::setw(15) << mult_c[i]
                    << std::setw(15) << e2_c[i]
                    << std::setw(15) << e3_c[i]
                    << std::setw(15) << e4_c[i]
                    << std::setw(15) << e5_c[i]    
                    << "\n";
        }
    }

    // Close the output file
    outputFile.close();

    std::cout << "Data has been written to output10kt2.dat" << std::endl;
}   
