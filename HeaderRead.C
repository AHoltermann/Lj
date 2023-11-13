#include <iostream>
#include <fstream>
#include <TCanvas.h>
#include <TH1D.h>

void CreateHistogramForColumn(const char* datFile, int columnIndex) {
    // Open the .dat file for reading
    std::ifstream dataFile(datFile);
    if (!dataFile.is_open()) {
        std::cerr << "Error: Unable to open the .dat file." << std::endl;
        return;
    }

    // Create a canvas to display the histogram
    TCanvas* canvas = new TCanvas("canvas", "Histogram", 800, 600);

    // Define the number of bins and range for the histogram
    int nBins = 100; // Adjust as needed 100
    int nBinsX = 100; // Adjust as needed
    int nBinsY = 40; // Adjust as needed
    double xMin = 0.0; // Adjust as needed
    double xMax = 425; // Adjust as needed 500
    double yMin = 0.0; // Adjust as needed
    double yMax = 20; // Adjust as needed

    // Create the histogram for the selected column
    TH1D* histogram = new TH1D("Nparts", "N parts data", nBins, xMin, xMax);
    TH1D* histogram2 = new TH1D("B HISTOGRAM", "B data", nBins, xMin, xMax);
    TH1D* histogram3 = new TH1D("Eccentricity", "E_5 data", nBins, xMin, xMax);
    TH2D* histogram4 = new TH2D("NPART VS B", "NPART VS B", nBinsX, xMin, xMax, nBinsY, yMin, yMax);


    // Variables to store data from the file
    Double_t e_n, b, npart, mult, e2, e3, e4, e5;

    // Read data from the file and fill the histogram for the selected column
    while (dataFile >> e_n >> b >> npart >> mult >> e2 >> e3 >> e4 >> e5) {
        //if(npart > 100) {
        histogram->Fill(npart);
        histogram2->Fill(b);
        histogram3->Fill(e5);
        histogram4->Fill(npart, b);
        //}
            //std::cout<<value<<std::endl;     
    }

    //TH1* cumulativehist = histogram->GetCumulative();
    // Close the file
    dataFile.close();
    histogram->SetLineColor(1);

    // Draw and display the histogram
    //histogram->Draw("colz");
    //cumulativehist->Scale(1/1000);
    
    //cumulativehist->Draw();
    histogram4->Draw("colz");
    
    // Display the canvas
    canvas->Draw();
}


void HeaderRead(const char* datFile = "headers.dat"){
    CreateHistogramForColumn(datFile, 3);
}