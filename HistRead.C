#include <iostream>
#include <fstream>
#include <TFile.h>
#include <TH2D.h>
#include <vector>
#include <cstdlib>
#include <ctime>

void HistRead() {
    //... READING ROOT FILE ...
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
    for (int j = numBinsY; j >= 0; --j) {
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

    //...CONVERSION FINISHED...

    // **Read data from the text file into a 2D vector**
    const int numRows = 120;
    const int numCols = 100;
    const double binWidth = 4.0;  // Assuming a fixed bin width of 4 GeV!!
    const double xOffset = 100.0;  // Starting point of x-axis
    const double yOffset = 20.0;   // Starting point of y-axis
    const int numPoints = 5000; //number of points to generate to check the method

    
    // Initialize a 2D vector with zeros
    std::vector<std::vector<double>> data(numRows, std::vector<double>(numCols));


    // Open the text file
    std::ifstream outputFile1("output_file.txt"); 

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
    
    //CHECK METHOD ...
    // Initialize the random seed
    srand(static_cast<unsigned int>(time(nullptr)));
    //checking for the correctness of the code
    // Create a TH2D histogram
    TH2D* hist = new TH2D("hist", "Randomly Generated Data;Leading Jet pT (GeV);Subleading Jet pT (GeV)",
                          100, xOffset, 500,
                          120, yOffset, 500);


    for (int k = 0; k < numPoints; ++k) {         
        // Randomly pick an x index
        /*
        0.) Make an x_cdf array or vector, and initialize some counters (Z)
        1.) Sum up all values of the array (I believe it’s like 6243475)
        2.) For i in range x indices:
		    for j in range y indices:	
				Z+= data[i][j]
		    x_cdf[i] = Z/6243475
            You will have a CDF vector, so x[100] should be 1.*/
        
        double randA = static_cast<double>(std::rand()) / RAND_MAX;
        std::vector<double> x_cdf(numRows, 0.0);
        double x_sum = 0.0;
        for (int i = 0; i < numRows; ++i) {
            for (int j = 0; j < numCols; ++j) {
                x_sum += data[i][j];
            }
            x_cdf[i] = x_sum/6243475;
        }
        //3.) With a random number randA between 0 and 1, find the greatest index such that x_cdf[i] < randA. 
        //That’s your x index, which you then plug into the next part of your calculation
        int xIndex = -1;
        for (int i = 0; i < x_cdf.size(); ++i) {
            if (x_cdf[i] < randA) {
                xIndex = i;
            } else {
            // Since x_cdf is assumed to be sorted, we can break the loop
            // as soon as x_cdf[i] is not less than randA
                break;
            }
        }

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
        // Convert indices back to jet pT
        double leadingJetPt = xOffset + xIndex * binWidth;
        double subleadingJetPt = yOffset + yIndex * binWidth;
        int xVal = static_cast<int>(xIndex);
        int yVal = static_cast<int>(yIndex);

        
        // Fill histogram
        hist->Fill(leadingJetPt, subleadingJetPt, data[xVal][yVal]);
        
    }
    /*
    // Display the selected indices
    std::cout << "Selected indices: (" << xIndex << ", " << yIndex << ")\n";
    
    int xVal = static_cast<int>(xIndex);
    int yVal = static_cast<int>(yIndex);

    // Display the value
    std::cout << data[xVal][yVal] << std::endl; 

    

    // Display the selected indices and corresponding jet pT
    std::cout << "Selected indices: (" << xIndex << ", " << yIndex << ")\n";
    std::cout << "Leading Jet pT: " << leadingJetPt << " GeV\n";
    std::cout << "Subleading Jet pT: " << subleadingJetPt << " GeV\n";
    */
    //Draw 
    // Create a canvas and draw the histogram
    TCanvas* canvas = new TCanvas("canvas", "Canvas", 800, 600);
    hist->Draw("colz");

    // Run the ROOT event loop
    canvas->Update();
    canvas->Draw();
    gSystem->ProcessEvents();
    
    



}
