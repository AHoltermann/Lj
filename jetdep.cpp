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
#include "methods.cpp"


int pathlength(const char* datFile){
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

    // Now col1, col2, col3, col4, col5 contain the values from the file
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

/*
void pathlength(int number){
    std::string stringevt = eventstring(number);
    double npart = 0.0;
    std::ifstream inputFile(stringevt); 
    if (!inputFile.is_open()) {
        std::cerr << "Error opening file." << std::endl;
    }

    std::string line;
    std::string key;

    while (std::getline(inputFile, line)) {
        std::istringstream iss(line);
        // Read the key and value from each line
        iss >> key;

        // Check if the key is "# mult" and if there is an equal sign
        if (key == "#") {
            // Check for "mult" and read the value
            iss >> key;
            if (key == "mult") {
                char equalSign;
                iss >> equalSign >> npart;
                break;  // Break the loop after finding the '# mult' value
            }
        }
    }
    std::cout << npart << std::endl;
    inputFile.close();
//    return npart;
}
*/
int main(){
    pathlength("pbpb_headers.dat");
}