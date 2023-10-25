#include <complex>
#include <tuple>
#include <sstream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>

#include <iostream>
#include <H5Cpp.h>

int trentoconvert(const char* outfile = "small.root", std::string filename = "sbatch_o24.hdf", std::string datasetName = "event_15") {
    H5::H5File file(filename, H5F_ACC_RDONLY);
    H5::DataSet dataset = file.openDataSet(datasetName);
    H5::DataSpace dataspace = dataset.getSpace();
    int rank = dataspace.getSimpleExtentNdims();
    hsize_t dims_out[2];
    dataspace.getSimpleExtentDims(dims_out, NULL);
    double* data = new double[dims_out[0] * dims_out[1]];
    dataset.read(data, H5::PredType::NATIVE_DOUBLE);

    const char* string = datasetName.c_str();
    TH2D* hist = new TH2D(string, "initial state;x;y", dims_out[0], 0, dims_out[0], dims_out[1], 0, dims_out[1]);
    for (hsize_t i = 0; i < dims_out[0]; i++) {
        for (hsize_t j = 0; j < dims_out[1]; j++) {
            double value = data[i * dims_out[1] + j];
            // Do something with 'value' (your dataset element)
            hist->SetBinContent(i + 1, j + 1, data[i * dims_out[1] + j]);
    
        }
    }
    file.close();

    TFile*n = new TFile(outfile,"UPDATE");
    n->cd();
    hist->Write();
    hist->Clear();
    n->Close();
    return 0;
}

int trentoread(const char* outfile = "small.root", std::string filename = "sbatch_o24.hdf", int nevents = 100){

        int j = 0;

        for (int i = 0; i < nevents; i++) {
        if(i%1000 ==0){
            cout << i << endl;
        }
        std::string eventString = "event_" + std::to_string(i);
        
        j = trentoconvert(outfile,filename,eventString);
        }
        return 0;
}