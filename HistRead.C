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
    const int numRows = 120;
    const int numCols = 120;
    const double binWidth = 4.0;  // Assuming a fixed bin width of 4 GeV
    const double xOffset = 100.0;  // Starting point of x-axis
    const double yOffset = 50.0;   // Starting point of y-axis

    // Initialize a 2D vector with zeros
    std::vector<std::vector<double>> data(numRows, std::vector<double>(numCols, 0.0));

    // Open the text file
    std::ifstream outputFile1("output_file.txt");  // Replace with your actual file name

    if (!outputFile1.is_open()) {
        std::cerr << "Error: Unable to open input file." << std::endl;
    }

    // Read values from the text file into the 2D vector
    for (int i = 0; i < numRows; ++i) {
        for (int j = 0; j < numCols; ++j) {
            if (!(outputFile1 >> data[i][j])) {
                // Handle error if the file does not contain enough values
                std::cerr << "Error: Insufficient data in the file." << std::endl;  
            }
        }
    }

    // Close the file
    outputFile1.close();
    // Assuming data.size() represents the number of elements in the 2D array

    // Initialize the random seed
    srand(static_cast<unsigned int>(time(nullptr)));

   // Randomly pick an x index
    int xIndex = rand() % numRows;

    // Calculate the 1D CDF for the selected x index
    std::vector<double> cdf(numCols, 0.0);
    double sum = 0.0;

    for (int j = 0; j < numCols; ++j) {
        sum += data[xIndex][j];
        cdf[j] = sum;
    }

    // Normalize the CDF to make it a probability distribution
    for (int j = 0; j < numCols; ++j) {
        cdf[j] /= sum;
    }

    // Randomly pick a y index based on the calculated CDF
    double y_prob = static_cast<double>(rand()) / RAND_MAX;

    int yIndex = 0;
    while (y_prob > cdf[yIndex] && yIndex < numCols - 1) {
        ++yIndex;
    }

    // Display the selected indices
    std::cout << "Selected indices: (" << xIndex << ", " << yIndex << ")\n";
    
    int xVal = static_cast<int>(xIndex);
    int yVal = static_cast<int>(yIndex);

    // Display the value
    std::cout << data[xVal][yVal] << std::endl; 

     // Convert indices back to jet pT
    double leadingJetPt = xOffset + xIndex * binWidth;
    double subleadingJetPt = yOffset + yIndex * binWidth;

    // Display the selected indices and corresponding jet pT
    std::cout << "Selected indices: (" << xIndex << ", " << yIndex << ")\n";
    std::cout << "Leading Jet pT: " << leadingJetPt << " GeV\n";
    std::cout << "Subleading Jet pT: " << subleadingJetPt << " GeV\n";


}
