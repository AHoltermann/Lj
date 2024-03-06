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

using namespace std;

string eventstring(int number = 6) {
    // Assuming the format is "newevents.dat/XXX.dat"
    // returns string pointing to n-th file name 
    ostringstream filename;
    //filename << "newevents.dat/" << setw(3) << setfill('0') << number << ".dat";
    filename << "../pbpb_50k/" << setw(4) << setfill('0') << number << ".dat";
    return filename.str();
}

string filename(string tag, int number){

    ostringstream filename;
    filename << tag << number << ".dat";
    return filename.str();
}

vector<vector<double>> profile(string filename =  "../newevents.dat/006.dat") {
    //given a TRENTO File, outputs a 2d array containing the information here
    vector<vector<double> > result;
    result.reserve(100);

    // Open the file
    ifstream file(filename);
    // Read each line from the file - skipping the header (first 8 lines)
    string line;
    for (int i = 0; i < 8; ++i) {
        string dummyLine;
        getline(file, dummyLine);
    }
    
    while (getline(file, line)) {
        vector<double> row;
        row.reserve(100);
        istringstream iss(line);
        double value;

        // Read each value from the line
        while (iss >> value) {
            row.push_back(value);
            //cout<< value << endl;
        }

        // Add the row to the result
        result.push_back(row);
    }

    // Close the file
    file.close();

    return result;
}

vector<vector<double>> eprofile(string filename = "energies.dat"){
    int totjets = 6243475;
    ifstream outputFile1(filename); 

    int numRows = 100;
    int numCols = 120;
    
    // Initialize a 2D vector with zeros
    vector<vector<double>> data(numRows, vector<double>(numCols));


    if (!outputFile1.is_open()) {
        cerr << "Error: Unable to open input file." << endl;
    }

    // Read values from the text file into the 2D vector
    for (int i = 0; i < numRows; ++i) {
        for (int j = 0; j < numCols; ++j) {
            if (!(outputFile1 >> data[i][j])) {
                // Handle error if the file does not contain enough values
                cerr << "Error: Insufficient data in the file." << endl;  
            }
        }
    }

    // Close the file
    outputFile1.close();

    //cout << data.size() << " " << data[0].size() << endl;

    return data;
}

vector<double> randoms(int length = 2){

    random_device rd;
    mt19937 gen(rd());
    vector<double> m(length,0);
    double lower_bound = 0.0;
    double upper_bound = 1.0;
    uniform_real_distribution<double> distribution(lower_bound, upper_bound);
    // Generate two random double-precision floating-point numbers
    for(int i = 0; i< length; i++){
        double ran = distribution(gen);
        m[i] = ran;
    }

    return m;
}

double jet_susceptibility(double pT = 0){

    vector<double> sus3 = {0.153,0.304,0.373,0.320,0.170,0.078,0.031,0.017,0.015,0.015,0.010,0.006};
    vector<double> sus2 = {0.123,0.255,0.375,0.460,0.398,0.234,0.111,0.048,0.029,0.023,0.015,0.006};
    vector<double> sus1 = {0.063,0.096,0.144,0.205,0.232,0.178,0.094,0.044,0.023,0.015,0.010,0.006};
    //vector<double> x = {0.0035,0.0055,0.008,0.012,0.018,0.027,0.041,0.063,0.094,0.145,0.22,0.33};
    double intsus1 = 1.11;
    double intsus2 = 2.077;
    double intsus3 = 1.492;
    double int0 = 0;

    vector<double> sus(12);
    if(pT > 315){
        sus=sus3;
        int0 = intsus3;

    }
    else if(pT > 215){
        sus=sus2;
        int0 = intsus2;
    }
    else{
        sus=sus1;
        int0 = intsus1;
    }

    vector<double> a = randoms(1);
    double c = 0;
    double xv = 0;


    while(c < a[0]){
        c+= sus[xv]/int0;
        xv+=1;
    }
    xv-=1;


    double raa = 0.8 - (0.4/11)*xv;
    double sloss_v = 0.255-0.255*raa;
    

    return sloss_v/0.0765;
}

tuple<int,int> energypick(vector<vector<double>> data){
   
        //cout << "energypick" << endl;

        vector<double>q = randoms(2);
        double x_prob = q[0];
        double y_prob = q[1];

        //cout << x_prob << " " << y_prob << endl;
    
        int xcount = 0;
        int ycount = 0;
 
        double cprobs1 = 0;
        double cprobs2 = 0;
        double cprobs3 = 0;

        double p2idx =0;
        double p3idx = 0;

        for(int i = 0; i<100; i++){
            for(int j = 0; j<120; j++){
                cprobs1 += data[i][j];
            }
        }
        while((cprobs2 < x_prob) && (xcount < 99)){
            for(int k = 0; k<120; k++){
                p2idx += data[xcount][k];
            } 
            cprobs2 += p2idx/cprobs1;
            p2idx = 0;
            //cout<< "cprobs2: " << cprobs2 << " y bin: " << xcount << endl;
            xcount += 1;
        }
        xcount -=1;
        if(xcount < 0){
            xcount =0;
        }
        if(xcount > 99){
            xcount =99;
        }

        for(int l = 0; l<120; l++){
            p3idx += data[xcount][l];
        }
        while((cprobs3 < y_prob)&&(ycount < 119)){
            //cout<< "p3idx: " << p3idx << endl;
            cprobs3 += data[xcount][ycount]/p3idx;
            //cout<< "cprobs3: " << cprobs3 << " x bin: " << ycount << endl;
            ycount +=1;    
        }
        ycount-=1;
        if(ycount > 119){
            ycount =118;
        }
        if(ycount < 0){
            ycount =0;
        }
    //cout << xcount << " " << ycount << endl;
    //tuple<int,int> Es = make_tuple(static_cast<int>(subleadingJetPt),static_cast<int>(leadingJetPt));   
    tuple<int,int> Es = make_tuple(100+xcount*4,20+ycount*4); 
    return Es;
}

tuple<int,int> positionpick(vector<vector<double> > prof){
        //cout << "positionpick" << endl;

        vector<double>q = randoms(2);
        //double x_prob = q[0];
        double x_prob = q[0];
        double y_prob = q[1];

        //cout << "xprob: " << x_prob << " yprob: " << y_prob << endl;
    
        int xcount = 0;
        int ycount = 0;
 
        double cprobs1 = 0;
        double cprobs2 = 0;
        double cprobs3 = 0;

        double p2idx;
        double p3idx;

        for(int i = 0; i<100; i++){
            for(int j = 0; j<100; j++){
                cprobs1 += prof[i][j];
            }
        }

        //cout<< "cprobs1: " << cprobs1 << endl;

        while(cprobs2 < x_prob && (x_prob<99)){
            for(int k = 0; k<100; k++){
                p2idx += prof[xcount][k];
            } 
            cprobs2 += p2idx/cprobs1;
            p2idx = 0;
            //cout<< "cprobs2: " << cprobs2 << " y bin: " << xcount << endl;
            xcount += 1;
        }
        xcount -=1;
        if(xcount <=0){
            cout << " caught 0 index" << endl;
        }
        if(xcount >99){
            cout << " caught 100 index" << endl;
        }

        for(int l = 0; l<100; l++){
            p3idx += prof[xcount][l];
        }
        while((cprobs3 < y_prob)&&(ycount<99)){
            //cout<< "p3idx: " << p3idx << endl;
            cprobs3 += prof[xcount][ycount]/p3idx;
            //cout<< "cprobs3: " << cprobs3 << " x bin: " << ycount << endl;
            ycount +=1;    
        }
        ycount-=1;
        if(ycount <=0){
            cout << " caught 0 index" << endl;
        }
        if(ycount >99){
            cout << " caught 100 index" << endl;
        }
        //cout << xcount << " " << ycount << endl;
        tuple<int, int> a = make_tuple(xcount, ycount);
        //cout<< "finish with x: " << ycount << " y: " << xcount << endl;
        return a;
}

double weighfour(double x, double y, vector<vector<double> > profile){
   
    double w = 0;
    int x_f,x_c,y_f,y_c = 0;

    double ll,lh,hl,hh = 0;

    x_f = floor(x);
    x_c = x_f + 1;
    y_f = floor(y);
    y_c = y_f+1;

    ll = profile[x_f][y_f];
    lh = profile[x_f][y_c];
    hl = profile[x_c][y_f];
    hh = profile[x_c][y_c];

    return (ll+lh+hl+hh)/4;
}

vector<double> pathlength(double theta, vector<vector<double>> profile, double dt){

    tuple<int,int> pos = positionpick(profile);
    double x = 1.0*get<0>(pos);
    double y = 1.0*get<1>(pos);
    double e_x = 0;
    double d = 0;
    double path = 0;
    double e_path = 0;
    int i = 0;

     while(i < 100/dt){

        if(i > 1){
            if(e_x<=0.00015){
                break;
            }
        }
        if(x < 4||x> 96||y<4||y>96){
                break;
            }

        //cout << "x: " << x << " y: " << y << " d: " << d << " e_x: " << e_x << " E: " << E << endl;
        x += dt*cos(theta);
        y += dt*sin(theta);
        //d = sqrt((x-1.0*get<0>(pos))*(x-1.0*get<0>(pos))+(y-1.0*get<1>(pos))*(y-1.0*get<1>(pos)));
        e_x = weighfour(x,y,profile);
        e_path+=dt*e_x;
        path+=dt;
        

       i+=1; 
    }

    vector<double> paths(2,0);
    paths[0] = path;
    paths[1] = e_path; 

    return paths;
}

double energyloss(vector<double> c, vector<double> c_lim, double Ei, double theta, vector<vector<double> > profile, double dt){

    //cout << "eloss" << endl;

    tuple<int,int> pos = positionpick(profile);
    //pos = make_tuple(50,50);
    double x = 1.0*get<0>(pos);
    double y = 1.0*get<1>(pos);
    double E = Ei;
    //double js = jet_susceptibility(E);
    double js = 1;
    double e_x = 0;
    double d = 0;
    double d2 = 0;
    int i = 0;

    //cout << " eloss loop " << endl;

    while(i < 100/dt){

        //cout << "x: " << x << " y: " << y << " d: " << d << " e_x: " << e_x << " E: " << E << endl;

        if(i > 1){
            if((E < 28)||(e_x<=0.00015)){
                break;
            }
        }
        if(x < 4||x> 96||y<4||y>96){
                break;
            }

        x += dt*cos(theta);
        y += dt*sin(theta);

        //d = sqrt((x-1.0*get<0>(pos))*(x-1.0*get<0>(pos))+(y-1.0*get<1>(pos))*(y-1.0*get<1>(pos)));
        d += dt;
        e_x = weighfour(x,y,profile);

        
        E = E - c_lim[0]*c[0]*dt*js; //0.75
        E = E - c_lim[1]*c[1]*d*dt*js; //0.04
        E = E - c_lim[2]*c[2]*(d*d)*dt*js; //0.0012
        E = E - c_lim[3]*c[3]*E*dt*js; //0.005
        E = E - c_lim[4]*c[4]*E*d*dt*js; //0.0003
        E = E - c_lim[5]*c[5]*E*(d*d)*dt*js;//0.00001
        E = E - c_lim[6]*c[6]*(E*E)*dt*js; //0.00005
        E = E - c_lim[7]*c[7]*(E*E)*d*dt*js;//0.000003
        E = E - c_lim[8]*c[8]*(E*E)*(d*d)*dt*js; //0.0000001
        E = E - c_lim[9]*c[9]*e_x*dt*js; //0.8
        E = E - c_lim[10]*c[10]*d*e_x*dt*js; //0.05
        E = E - c_lim[11]*c[11]*(d*d)*e_x*dt*js; //0.002
        E = E - c_lim[12]*c[12]*E*e_x*dt*js; //0.0065
        E = E - c_lim[13]*c[13]*E*d*e_x*dt*js; //0.0005
        E = E - c_lim[14]*c[14]*E*(d*d)*e_x*dt*js; //0.00002
        E = E - c_lim[15]*c[15]*(E*E)*e_x*dt*js; //0.00006
        E = E - c_lim[16]*c[16]*(E*E)*d*e_x*dt*js; //0.000005
        E = E - c_lim[17]*c[17]*(E*E)*(d*d)*e_x*dt*js; //0.000004
        E = E - c_lim[18]*c[18]*(e_x*e_x)*dt*js; // 0.5
        E = E - c_lim[19]*c[19]*d*(e_x*e_x)*dt*js; //0.03
        E = E - c_lim[20]*c[20]*(d*d)*(e_x*e_x)*dt*js;  //0.001
        E = E - c_lim[21]*c[21]*E*(e_x*e_x)*dt*js; //0.0035
        E = E - c_lim[22]*c[22]*E*d*(e_x*e_x)*dt*js; //0.00025
        E = E - c_lim[23]*c[23]*E*(d*d)*(e_x*e_x)*dt*js; //0.000015
        E = E - c_lim[24]*c[24]*(E*E)*(e_x*e_x)*dt*js; //0.00004;
        E = E - c_lim[25]*c[25]*(E*E)*d*(e_x*e_x)*dt*js; //0.000004
        E = E - c_lim[26]*c[26]*(E*E)*(d*d)*(e_x*e_x)*dt*js; //0.0000002


        //cout << E << endl;
        
       i+=1; 
    } 
    return E;
}

// checked below 

vector<double> xj_sample(vector<double> c, vector<double> c_lim, double dt, int iters, int jetsamples, vector<vector<double>> data){

    cout << "xjsample" << endl;

    vector<double> xjs(2*iters*jetsamples,0);

    double E1,E2;
    double theta1,theta2;
    double pt1,pt2,ptL;
    double xj;
    int index = 0;
    for(int i = 0; i<iters; i++){

        vector<vector<double>>prof = profile(eventstring(i));

        for(int j = 0; j<jetsamples; j++){
            index = i*jetsamples+j;
            //cout << index << endl;
            //cout << "energypick " << endl;
            tuple<int,int> energypicks = energypick(data);
            E1 = 1.0*get<0>(energypicks);
            E2 = 1.0*get<1>(energypicks);
            //E1 = 150;
            //E2 = 100;

            //cout << E1 << " " << E2 << endl;

            vector<double>q = randoms(1);
            theta1 = q[0]*2*(3.1415926)-3.1415926;
            theta2 = theta1 + 3.1415926;

            //cout << theta1 << " " << theta2 << endl;
            pt1 = energyloss(c,c_lim,E1,theta1,prof,dt);
            pt2 = energyloss(c,c_lim,E2,theta2,prof,dt);

           
            //cout << pt1 << " " << pt2 << endl;

            if(pt1 < pt2){
                xj = pt1/pt2;
                ptL = pt2;
            }
            else{
                xj = pt2/pt1;
                ptL = pt1;
            }
            xjs[2*index] = ptL;
            xjs[2*index+1] =xj;

            if((i*jetsamples+j)%500 == 0){
                cout << setw(5) <<1.0*(i*jetsamples+j)/(iters*jetsamples) << " " << setw(5) << xjs[2*index] << " " << xjs[2*index+1] << endl;
            }
        }

    }

    return xjs;

}
    

//vector<vector<double>> xj_summary{std::vector<double>}
