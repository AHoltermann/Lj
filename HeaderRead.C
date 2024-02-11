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
    //TCanvas* canvas = new TCanvas("canvas", "Histogram", 800, 600);

    // Define the number of bins and range for the histogram
    int nBins =  400; // Adjust as needed 100
    int nBinsX = 100; // Adjust as needed
    int nBinsY = 100; // Adjust as needed
    double xMin = 0.0; // Adjust as needed
    double xMax = 1; // Adjust as needed 500
    double yMin = 0.0; // Adjust as needed
    double yMax = 140; // Adjust as needed

    // Create the histogram for the selected column
    TH1D* histogram = new TH1D("Energy weighted pathlength", "Energy weighted pathlength", nBins, xMin, xMax);
    TH1D* histogram2 = new TH1D("Pathlength", "Pathlength", nBins, xMin, xMax);
    TH1D* histogram3 = new TH1D("Eccentricity", "E_5 data", nBins, xMin, xMax);
    TH2D* histogram4 = new TH2D("Energy weighted pathlength vs npart", "npart", nBinsX, xMin, xMax, nBinsY, yMin, yMax);//
    TH2D* histogramPL = new TH2D("Pathlength vs npart", "npart", nBinsX, xMin, xMax, nBinsY, yMin, yMax);
    TH2D* histograme2 = new TH2D("Energy weighted pathlength vs e2", "e_2", nBinsX, xMin, xMax, nBinsY, yMin, yMax);
    TH2D* histogram5 = new TH2D("Energy weighted pathlength vs e3", "e_3", nBinsX, xMin, xMax, nBinsY, yMin, yMax);
    TH2D* histogram6 = new TH2D("Energy weighted pathlength vs e4", "e_4", nBinsX, xMin, xMax, nBinsY, yMin, yMax);
    TH2D* histogram7 = new TH2D("Energy weighted pathlength vs e5", "e_5", nBinsX, xMin, xMax, nBinsY, yMin, yMax);


    // Variables to store data from the file
    Double_t n, ew_path, path_l, npart, mult_c, e2, e3, e4, e5;

    // Read data from the file and fill the histogram for the selected column
    while (dataFile >> n >> ew_path >> path_l >> npart >> mult_c >> e2 >> e3 >> e4 >> e5) {
        //if(npart > 100) {
        histogram->Fill(ew_path);
        histogram2->Fill(path_l);
        histogram3->Fill(e5);
        histogram4->Fill(npart, ew_path);
        histogramPL->Fill(npart, path_l);
        histograme2->Fill(e2, ew_path);
        histogram5->Fill(e3, ew_path);
        histogram6->Fill(e4, ew_path);
        histogram7->Fill(e5, ew_path);
        //}
        
    }
    // Close the file
    dataFile.close();

//Calculate coefficient of variation for each bin
    std::vector<double> c_var;
    for (int binX = 1; binX <= histograme2->GetNbinsX(); ++binX) {
        double ratio = 0.0;
        double binContentSum = 0.0;
        double binErrorContentSquared = 0.0;
        int binEntries = 0;

        // Iterate over all bins in the y-direction (energy weighted pathlength) to calc the mean
        for (int binY = 1; binY <= histograme2->GetNbinsY(); ++binY) {
            double binContent = histograme2->GetBinContent(binX, binY);
            binContentSum += binContent;
            binEntries++;
        }

        // Compute mean for the current e2 bin
        double mean = binContentSum / binEntries;
        for (int binY = 1; binY <= histograme2->GetNbinsY(); ++binY) {
            double binContent = histograme2->GetBinContent(binX, binY);
            binErrorContentSquared += (binContent - mean)* (binContent - mean);
        }
        
        double variance = binErrorContentSquared / (binEntries - 1);
        double stdDeviation = std::sqrt(variance);

        // If mean is not zero, compute the ratio
        if (mean != 0) {
            ratio = stdDeviation / mean;
        } else {
            ratio = 0.0;
        }
        c_var.push_back(ratio);
    }

    //Graph
    TGraph *graph = new TGraph(c_var.size());

    // Fill the TGraph with e2 values and ratios
    for (int i = 0; i < c_var.size(); ++i) {
        double e2Value = histograme2->GetXaxis()->GetBinCenter(i + 1); // Get e2 value
        graph->SetPoint(i, e2Value, c_var[i]);
    }

    // Set axis titles
    graph->GetXaxis()->SetTitle("e2");
    graph->GetYaxis()->SetTitle("Coefficient of variation");

    // Create a canvas
    TCanvas* canvas = new TCanvas("canvas", "Graph", 800, 600);

    // Draw the graph
    graph->Draw("APL"); // APL = Axis, Points, Line

    // Display the canvas
    canvas->Draw();
    
    /*
    //histogram->SetLineColor(1);
    // Draw and display the histogram
    histograme2->Draw("colz");
    // Display the canvas
    canvas->Draw();
    */
}


void HeaderRead(const char* datFile = "/Users/marisolvelapatino/Desktop/rootworks/jet_dep/output10k.dat"){
    CreateHistogramForColumn(datFile, 3);
}