#include <iostream>
#include <fstream>
#include <TFile.h>
#include <TH2D.h>

using namespace std;

int indexpick(std::string n){

    if(n == "c000"){
        return 0;
    }
    if(n == "c001"){
        return 1;
    }
    if(n == "c002"){
        return 2;
    }
    if(n == "c100"){
        return 3;
    }
    if(n == "c101"){
        return 4;
    }
    if(n == "c102"){
        return 5;
    }
    if(n == "c200"){
        return 6;
    }
    if(n == "c201"){
        return 7;
    }
    if(n == "c202"){
        return 8;
    }



    if(n == "c010"){
        return 9;
    }
    if(n == "c011"){
        return 10;
    }
    if(n == "c012"){
        return 11;
    }
    if(n == "c110"){
        return 12;
    }
    if(n == "c111"){
        return 13;
    }
    if(n == "c112"){
        return 14;
    }
    if(n == "c210"){
        return 15;
    }
    if(n == "c211"){
        return 16;
    }
    if(n == "c212"){
        return 17;
    }

    if(n == "c020"){
        return 18;
    }
    if(n == "c021"){
        return 19;
    }
    if(n == "c022"){
        return 20;
    }
    if(n == "c120"){
        return 21;
    }
    if(n == "c121"){
        return 22;
    }
    if(n == "c122"){
        return 23;
    }
    if(n == "c220"){
        return 24;
    }
    if(n == "c221"){
        return 25;
    }
    if(n == "c222"){
        return 26;
    }
    
    return -1;

}


double searchstring(string inname, string searchString){

    //ostringstream filename;
    //filename << inname << endl;
    //string files = filename.str();
    //cout << files << endl;
    //cout << files << endl;
    ifstream datFile(inname);

    //ifstream file(filename);
    if (!datFile.is_open()) {
        cerr << "Error opening file: " << endl;
        return -1; // Error code
    }

    string line;
    while (getline(datFile, line)) {
        size_t found = line.find(searchString);
        if (found != string::npos) {
            istringstream iss(line.substr(found + searchString.length()));
            double xjError;
            if (iss >> xjError) {
                datFile.close();
                return xjError;
            } else {
                cerr << "Error parsing xj error value." << endl;
                datFile.close();
                return -1; // Error code
            }
        }
    }

    datFile.close();
    return -1; // Line not found
}

vector<vector<double>> values(){

    vector<double> c_0 = {15.2062,     41.3234,     90.9704,     161.632,    232.724,     289.025,     312.841,      276.07,     252.169,    270.365};
    vector<double> c_1 = {9.20927,     22.8664,     47.8647,     77.6013,     102.391,     113.431,     106.923,     100.504,     111.725,     134.888};
    vector<double> c_2 = {9.63175,     24.0613,      42.097,     62.9545,     81.2144,     83.4514,     80.2273,     84.6649,     100.393,     117.915};
    vector<double> c_3 = {5.90014,     14.4146,     25.5641,     35.2113,     43.3903,     49.1762,     51.6226,     53.5876,     59.1087,      70.584};
    vector<double> c_4 = {6.96223,     12.0143,     18.5049,     25.3182,     31.0814,     34.4254,     34.8516,     36.7749,     43.8876,     50.6414};
    vector<double> c_5 = {3.90557,     6.30308,     9.05305,     12.1703,     15.3246,     18.0697,     19.4071,     19.9992,     23.4896,      28.664};
    vector<double> c_6 = {2.2303,     3.52381,     4.80708,     6.91878,     9.62079,     11.4875,     12.6656,     13.7317,     16.4264,     19.7538};
    vector<double> c_7 = {1.13148,     1.62244,     2.41742,     3.64951,     5.15719,     6.67655,     7.93728,     8.79549,     10.3739,     12.8324};
    vector<double> c_8 = {0.614162,    0.863227,     1.21074,     1.86202,     2.79509,     3.73385,     4.45709,     5.15835,     6.19558,     7.47922};
    vector<double> c_9 = {0.274148,     0.36088,    0.525717,     0.87591,     1.40775,     2.00956,     2.45845,     2.86961,     3.53143,     4.36919};
    vector<double> c_10 = {0.142055,    0.202745,    0.355734,    0.587768,    0.906859,     1.16796,     1.60543,     2.17398,     2.62761,     3.31809};
    vector<double> c_11 = {0.0426747,   0.0537139,   0.0665977,    0.115868,    0.188016,    0.282109,    0.403354,     0.55031,    0.753207,     1.01281};

    vector<vector<double>> v = {c_0,c_1,c_2,c_3,c_4,c_5,c_6,c_7,c_8,c_9,c_10,c_11};
    return v;
}
vector<vector<double>> uncerts(){

    
        vector<double> c01 = {       14.3398,     29.8179,     42.0894,     34.0363,     17.1532,     32.1391,     33.4303,      39.555,     42.0034,     37.7482}
        vector<double> c02= {       8.30776,     13.3152,     16.3867,     12.8933,     8.98717,     15.7486,     19.3466,     18.9861,     11.7387,     24.4541}
        vector<double> c03 = {      6.06046,     8.83962,     8.83241,     7.71378,     7.03837,     12.3936,      14.771,     9.80728,     8.47037,     18.9956}
        vector<double> c04 = {        3.3481,     4.98384,     4.76159,     4.55301,     3.68771,     3.07426,     3.62251,     5.60482,     5.77254,      7.5282}
        vector<double> c05 = {       1.97961,     2.07614,     2.33743,     2.84984,     2.93469,     2.09357,     2.53813,     4.52811,     4.69634,     4.83235}
        vector<double> c06 = {      0.715013,    0.783912,     1.01675,     1.23708,     1.12398,     1.23412,     1.08179,     2.01515,     2.29849,     2.49468}
        vector<double> c07= {      0.453185,    0.484085,    0.528191,    0.537244,    0.932148,    0.939615,    0.793815,    0.964256,     1.21934,     1.49459}
        vector<double> c08 = {      0.235928,    0.292724,    0.306588,    0.315111,    0.394754,     0.50879,    0.611129,    0.579404,    0.730388,    0.941974}
        vector<double> c09 = {      0.126543,    0.164637,    0.170723,    0.172826,    0.237782,    0.323928,    0.337737,    0.362223,    0.451087,     0.60644}
        vector<double> c010 = {     0.0474529,   0.0485092,   0.0635707,     0.10232,    0.129094,    0.185137,    0.230221,    0.228449,    0.272132,    0.358351}
        vector<double> c011 = {     0.0388096,   0.0462028,   0.0588944,   0.0759862,   0.0929076,    0.113642,    0.150615,    0.222336,    0.217872,    0.313144}
        vector<double> c012 = {     0.0120633,   0.0145654,   0.0189045,   0.0228928,    0.030877,   0.0431239,   0.0505243,   0.0564735,    0.066097,   0.0990286}

        vector<vector<double>> vv = {c01,c02,c03,c04,c05,c06,c07,c08,c09,c010,c011,c012};
        return vv;

}




class cmodel{

    public:
        std::string idx; //"c021"
        int idx2;

    cmodel(std::string a){
        idx = a;
        idx2 = indexpick(a);
    }

    string filename(int v){

        std::ostringstream a;
        a << "dimscan2/" << idx << "_" << v << ".dat";      
        std::string a1 = a.str();
        return a1;
    }


    double error(int v){
        string f = filename(v);
        double q;
        q = searchstring(f,"Likelihood:");
        return q;
    }

    vector<vector<double>> data(int v) {
        string f = filename(v);
        ifstream file(f);
        string line;

        vector<vector<double>> d(12,vector<double>(10,0));
        
        // Skip lines until "Data:" is found
        while (getline(file, line)) {
            if (line.find("Data:") != string::npos) {
                break;
            }
        }
        // Read and print the matrix values under "Data:"
        for (int i = 0; i < 12; ++i) {
            getline(file, line);
            stringstream ss(line);
            double value;
            int j = 0;
            while (ss >> value) {
                //cout << value << " ";
                d[i][j] = value;
                j++;
            }
            cout << endl;
        }

        file.close();
        return d;
    }

    vector<vector<double>> uncert(int v) {
        string f = filename(v);
        ifstream file(f);
        string line;

        vector<vector<double>> d(12,vector<double>(10,0));
        
        // Skip lines until "Data:" is found
        while (getline(file, line)) {
            if (line.find("Errors:") != string::npos) {
                break;
            }
        }
        // Read and print the matrix values under "Data:"
        for (int i = 0; i < 12; ++i) {
            getline(file, line);
            stringstream ss(line);
            double value;
            int j = 0;
            while (ss >> value) {
                //cout << value << " ";
                d[i][j] = value;
                j++;
            }
            cout << endl;
        }

        file.close();
        return d;
    }

    TGraph* errorgraph(int color = 2){

        TGraph*j0 = new TGraph(60);
        for(int i = 0; i<60; i++){
            j0->SetPoint(i,i*0.05,error(i));

        }
        j0->SetLineColor(color);
        j0->SetLineWidth(3);
        return j0;
    }

    TH2D* value(int v){

        vector<double> edges = {0.316228,0.354813,0.398107,0.446684,0.501187,0.562341,0.630957,0.707946,0.794328,0.891251,1.0};
    //std::vector<double> edges_2 = {0,1,2,3,4,5,6,7,8,9,10,11,12};
        vector<double> pT_edges = {100,112.202,125.893,141.254,158.489,177.828,199.526,223.189,251.189,281.838,316.118,398,500};

        TH2D*h0 = new TH2D("","",edges.size() - 1,edges.data(),pT_edges.size()-1,pT_edges.data());

        vector<vector<double>> a = data(v);
        for (int i = 0; i < a.size(); i++) {
            for (int j = 0; j < a[0].size(); j++) {
                cout << a[i][j] << " ";
                h0->Fill((edges[j] + edges[j + 1]) / 2.0, (pT_edges[i] + pT_edges[i + 1]) / 2.0, a[i][j]);
            }
        cout << endl;
        }
        return h0
    }

    TH1D*xjdist(int v,int pT){
        vector<double> edges = {0.316228,0.354813,0.398107,0.446684,0.501187,0.562341,0.630957,0.707946,0.794328,0.891251,1.0};
        vector<vector<double>> a = data(v);
        vector<double> b = a[pT];

        vector<vector<double>> c = uncert(v);
        vector<double> d = e[pT];

        TH1D*c = new TH1D("", "",edges.size() - 1,edges.data());
        for(int i = 0; i<data.size(); i++){
            c->SetBinContent(i+1,b[i]); 
            c->SetBinContent(i+1,e[i]); 
        }
    return c;

    }

};





int dimscan2(){

    cmodel c000("c000");
    cmodel c001("c001");
    cmodel c002("c002");
    cmodel c010("c010");
    cmodel c011("c011");
    cmodel c012("c012");
    cmodel c020("c020");
    cmodel c021("c021");
    cmodel c022("c022");

    cmodel c100("c100");
    cmodel c101("c101");
    cmodel c102("c102");
    cmodel c110("c110");
    cmodel c111("c111");
    cmodel c112("c112");
    cmodel c120("c120");
    cmodel c121("c121");
    cmodel c122("c122");

    cmodel c200("c200");
    cmodel c201("c201");
    cmodel c202("c202");
    cmodel c210("c210");
    cmodel c211("c211");
    cmodel c212("c212");
    cmodel c220("c220");
    cmodel c221("c221");
    cmodel c222("c222");
    /*

    TGraph*j000 = c000.errorgraph(2);
    TGraph*j001 = c001.errorgraph(3);
    TGraph*j002 = c002.errorgraph(4);

    TGraph*j010 = c010.errorgraph(2);
    TGraph*j011 = c011.errorgraph(3);
    TGraph*j012 = c012.errorgraph(4);

    TGraph*j020 = c020.errorgraph(2);
    TGraph*j021 = c021.errorgraph(3);
    TGraph*j022 = c022.errorgraph(4);


    TGraph*j100 = c100.errorgraph(2);
    TGraph*j101 = c101.errorgraph(3);
    TGraph*j102 = c102.errorgraph(4);

    TGraph*j110 = c110.errorgraph(2);
    TGraph*j111 = c111.errorgraph(3);
    TGraph*j112 = c112.errorgraph(4);

    TGraph*j120 = c120.errorgraph(2);
    TGraph*j121 = c121.errorgraph(3);
    TGraph*j122 = c122.errorgraph(4);


    TGraph*j200 = c200.errorgraph(2);
    TGraph*j201 = c201.errorgraph(3);
    TGraph*j202 = c202.errorgraph(4);

    TGraph*j210 = c210.errorgraph(2);
    TGraph*j211 = c211.errorgraph(3);
    TGraph*j212 = c212.errorgraph(4);

    TGraph*j220 = c220.errorgraph(2);
    TGraph*j221 = c221.errorgraph(3);
    TGraph*j222 = c222.errorgraph(4);


    TCanvas* c =new TCanvas("","",1000,1000);
    c->cd();

    j000->Draw("alp");
    j001->Draw("same");
    j002->Draw("same");

    j010->Draw("same");
    j011->Draw("same");
    j012->Draw("same");

    j020->Draw("same");
    j021->Draw("same");
    j022->Draw("same");

    j100->Draw("same");
    j101->Draw("same");
    j102->Draw("same");

    j110->Draw("same");
    j111->Draw("same");
    j112->Draw("same");

    j120->Draw("same");
    j121->Draw("same");
    j122->Draw("same");

    j200->Draw("same");
    j201->Draw("same");
    j202->Draw("same");

    j210->Draw("same");
    j211->Draw("same");
    j212->Draw("same");

    j220->Draw("same");
    j221->Draw("same");
    j222->Draw("same");
    */

    vector<double> edges = {0.316228,0.354813,0.398107,0.446684,0.501187,0.562341,0.630957,0.707946,0.794328,0.891251,1.0};
    vector<double> pT_edges = {100,112.202,125.893,141.254,158.489,177.828,199.526,223.189,251.189,281.838,316.118,398,500};

    TH2D*h0 = new TH2D("","",edges.size() - 1,edges.data(),pT_edges.size()-1,pT_edges.data());

    /*
    vector<vector<double>> a = c011.data(21);
    for(int i = 0; i<a.size(); i++){
        for(int j = 0; j<a[0].size(); j++){
            cout << a[i][j] <<" ";
            h0->Fill(edges[i]+0.001,pT_edges[j+1]+0.0001,a[i][j]);
        }
        cout << endl;
    }
    h0->Draw("colz");
    */
    
h0->Draw("lego");

        

    return 0;
}

