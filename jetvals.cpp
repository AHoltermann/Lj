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
#include "methods1.cpp"
#include <cmath>


double getRandomValue() {
    std::random_device rd;
    std::mt19937 gen(rd());
    double lower_bound = 0.0;
    double upper_bound = 1.0;
    std::uniform_real_distribution<double> distribution(lower_bound, upper_bound);
    return distribution(gen);
}

std::vector<double> calc_pathlength(std::string filename) {
    //initialize profile
    std::vector<std::vector<double> > myprof = profile(filename);
    //generate random angle
    double rand_val = getRandomValue();
    double theta = rand_val * (M_PI / 180);

    //dt, and e_select
    double dt = 1.0;
    //create path and epath
    std::vector<double> path = pathlength(theta, myprof, dt);
    
    return path;
}

int jetdepmethod(const char* datFile) {
    std::ifstream inputFile(datFile);

    if (!inputFile) {
        std::cerr << "Error opening file." << std::endl;
        return 1;
    }

    std::vector<double> mult_c, e2_c, e3_c, e4_c, e5_c;
    double_t e_n, b, npart, mult, e2, e3, e4, e5;

    while (inputFile >> e_n >> b >> npart >> mult >> e2 >> e3 >> e4 >> e5) {
        
        mult_c.push_back(mult);
        e2_c.push_back(e2);
        e3_c.push_back(e3);
        e4_c.push_back(e4);
        e5_c.push_back(e5);
    }

    // Now we have 5 columns containing the values from the file
    // Open the output file
    std::ofstream outputFile("output.dat");

    if (!outputFile) {
        std::cerr << "Error opening file." << std::endl;
        return 1;
    }

    // Write the values of the vectors to the output file
    for (size_t i = 0; i < mult_c.size(); ++i) {
        outputFile << std::setw(10) << mult_c[i]
                   << std::setw(15) << e2_c[i]
                   << std::setw(15) << e3_c[i]
                   << std::setw(15) << e4_c[i]
                   << std::setw(15) << e5_c[i]
                   << "\n";
    }

    // Close the output file
    outputFile.close();

    std::cout << "Data has been written to output.dat" << std::endl;
    return 0;
}

int main(){
    jetdepmethod("pbpb_headers.dat");

    std::vector<double> path_l, ew_path, num_event;
    for (size_t i = 0; i < 1000; ++i) {
        std::vector<double> paths = calc_pathlength(eventstring(i));
        path_l.push_back(paths[2*i]);
        ew_path.push_back(paths[2*i + 1]);
        num_event.push_back(i);
    }


    //Write out second output file
    std::ofstream outputFile("output2.dat");

    if (!outputFile) {
        std::cerr << "Error opening file." << std::endl;
        return 1;
    }

    // Write the values of the vectors to the output file
    for (size_t i = 0; i < num_event.size(); ++i) {
        outputFile << std::setw(10) << num_event[i]
                   << std::setw(15) << ew_path[i]
                   << std::setw(15) << path_l[i]
                   << "\n";
    }

    // Close the output file
    outputFile.close();

    std::cout << "Data has been written to output2.dat" << std::endl;
}
