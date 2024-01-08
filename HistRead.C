#include <iostream>
#include <fstream>
#include <TFile.h>
#include <TH2D.h>
#include <vector>
#include <cstdlib>
#include <ctime>

void HistRead() {
    // Open the ROOT file containing the 2D histograms
    TFile* inputFile = TFile::Open("com_5020_minpt_50_merged.root", "READ");

    if (!inputFile) {
        std::cerr << "Error: Unable to open input ROOT file." << std::endl;
    }

    // Get the 2D histogram from the file (replace "your_histogram_name" with the actual name)
    TH2D* hist2D = dynamic_cast<TH2D*>(inputFile->Get("hpt1pt2"));

    if (!hist2D) {
        std::cerr << "Error: Unable to retrieve 2D histogram from the ROOT file." << std::endl;
        inputFile->Close();
    }

    // Open a text file for writing (replace "output_file.txt" with your desired output file name)
    std::ofstream outputFile("output_file.txt");

    if (!outputFile.is_open()) {
        std::cerr << "Error: Unable to open output text file for writing." << std::endl;
        inputFile->Close();     
    }
    
    int numBinsX = hist2D->GetNbinsX();
    int numBinsY = hist2D->GetNbinsY();

    // Loop through the bins of the 2D histogram and write the bin content to the text file
    for (int j = 1; j <= numBinsY; ++j) {
        for (int i = 1; i <= numBinsX; ++i) {
            double binContent = hist2D->GetBinContent(i, j);
            outputFile << binContent << "\t";

            // Start a new line after numBinsX columns
            if (i == numBinsX) {
                outputFile << "\n";
            }   
        }
    }

    // Close the files and clean up
    outputFile.close();
    inputFile->Close();

    std::cout << "Conversion successful." << std::endl;

    // Read data from the text file into a 2D vector
    const char* filename1 = "output_file.txt";  // Replace with your actual file name
    std::ifstream output(filename1);

    if (!output.is_open()) {
        std::cerr << "Error: Unable to open input file." << std::endl;
    }

    std::vector<std::vector<double>> data;
    double value;

    while (output >> value) {
        data.push_back({value});
    }

    output.close();

    // Assuming data.size() represents the number of elements in the 2D array

    // Initialize the random seed
    srand(static_cast<unsigned int>(time(nullptr)));

    // Generate random probabilities
    double x_prob = static_cast<double>(rand()) / RAND_MAX;
    double y_prob = static_cast<double>(rand()) / RAND_MAX;

    // Calculate the cumulative distribution function (CDF) for both dimensions
    double x_cdf = 0.0;
    double y_cdf = 0.0;

    // Calculate the CDF for the X dimension
    for (const auto& row : data) {
        for (const auto& val : row) {
            x_cdf += val;
        }
    }

    // Normalize the X CDF to make it a probability
    x_cdf /= data.size();

    // Calculate the CDF for the Y dimension
    for (size_t j = 0; j < data[0].size(); ++j) {
        for (size_t i = 0; i < data.size(); ++i) {
            y_cdf += data[i][j];
        }
    }

    // Normalize the Y CDF to make it a probability
    y_cdf /= data.size();

    // Initialize new indices
    int new_x_index = -1;
    int new_y_index = -1;

    // Sample from the 2D array based on probabilities
    for (size_t i = 0; i < data.size(); ++i) {
        for (size_t j = 0; j < data[0].size(); ++j) {
            x_prob -= data[i][j] / x_cdf;
            y_prob -= data[i][j] / y_cdf;

            if (x_prob <= 0.0 && new_x_index == -1) {
                new_x_index = i;
            }

            if (y_prob <= 0.0 && new_y_index == -1) {
                new_y_index = j;
            }
        }
    }
    
    int xVal = static_cast<int>(new_x_index);
    int yVal = static_cast<int>(new_y_index);

    // Display the selected indices
    std::cout << "Selected indices: (" << new_x_index << ", " << new_y_index << ")\n";
    std::cout << data[xVal][yVal] << std::endl; 

}
