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

std::string eventstring(int number = 6) {
    // Assuming the format is "newevents.dat/XXX.dat"
    std::ostringstream filename;
    filename << "newevents.dat/" << std::setw(3) << std::setfill('0') << number << ".dat";
    return filename.str();
}

std::vector<std::vector<double> > profile(std::string filename =  "newevents.dat/006.dat") {

    std::vector<std::vector<double> > result;

    // Open the file
    std::ifstream file(filename);

    // Read each line from the file
    std::string line;

    for (int i = 0; i < 8; ++i) {
        std::string dummyLine;
        std::getline(file, dummyLine);
    }

     // Skip the first 8 lines

    while (std::getline(file, line)) {
        std::vector<double> row;
        std::istringstream iss(line);
        double value;

        // Read each value from the line
        while (iss >> value) {
            row.push_back(value);
            //std::cout<< value << std::endl;
        }

        // Add the row to the result
        result.push_back(row);
    }

    // Close the file
    file.close();

    //std::cout << result[50][50];

    return result;
}

std::tuple<int,int> positionpick(std::vector<std::vector<double> > prof){

        srand((unsigned int)time(NULL));

        double x_prob = (double)rand() / RAND_MAX;
        double y_prob = (double)rand() / RAND_MAX;

        int xcount = 0;
        int ycount = 0;
 
        double cprobs1 = 0;
        double cprobs2 = 0;
        double cprobs3 = 0;

        for(int i = 0; i<100; i++){
            cprobs1 += std::accumulate(prof[i].begin(),prof[i].end(),0);
        }

        while(cprobs2 < x_prob){
            cprobs2+=  std::accumulate(prof[xcount].begin(),prof[xcount].end(),0)/cprobs1;
            xcount += 1;
        }
        xcount-=1;

        while(cprobs3 < y_prob){
            cprobs3+= prof[xcount][ycount]/std::accumulate(prof[xcount].begin(),prof[xcount].end(),0);
            ycount +=1;
        }
        ycount-=1;

        std::tuple<int, int> a = std::make_tuple(xcount, ycount);
        return a;
}

double weighfour(double x, double y, std::vector<std::vector<double> > profile){
   
    double w = 0;
    int x_f,x_c,y_f,y_c = 0;

    double ll,lh,hl,hh = 0;

    x_f = std::floor(x);
    x_c = x_f + 1;
    y_f = std::floor(y);
    y_c = y_f+1;

    ll = profile[x_f][y_f];
    lh = profile[x_f][y_c];
    hl = profile[x_c][y_f];
    hh = profile[x_c][y_c];

    return (ll+lh+hl+hh)/4;
}

double energyloss(std::vector<double> c, double Ei, double theta, std::vector<std::vector<double> > profile, double dt){

    std::tuple<int,int> pos = positionpick(profile);
    double x = 1.0*std::get<0>(pos);
    double y = 1.0*std::get<1>(pos);

    double E = Ei;

    double e_x = 0;
    double d = 0;

    int i = 0;
    while(i < 100/dt){
        x += dt*cos(theta);
        y += dt*sin(theta);

        if(x < 0||x>100||y<0||y>100){
            continue;
        }

        e_x = weighfour(x,y,profile);
        d = sqrt((x-1.0*std::get<0>(pos))*(x-1.0*std::get<0>(pos))+(y-1.0*std::get<1>(pos))*(y-1.0*std::get<1>(pos)));

        E = E - c[0];
        E = E - c[1]*d;
        E = E - c[2]*(d*d);
        E = E - c[3]*E;
        E = E - c[4]*E*d;
        E = E - c[5]*E*(d*d);
        E = E - c[6]*(E*E);
        E = E - c[7]*(E*E)*d;
        E = E - c[8]*(E*E)*(d*d);
        E = E - c[0]*e_x;
        E = E - c[10]*d*e_x;
        E = E - c[11]*(d*d)*e_x;
        E = E - c[12]*E*e_x;
        E = E - c[13]*E*d*e_x;
        E = E - c[14]*E*(d*d)*e_x;
        E = E - c[15]*(E*E)*e_x;
        E = E - c[16]*(E*E)*d*e_x;
        E = E - c[17]*(E*E)*(d*d)*e_x;
        E = E - c[18]*(e_x*e_x);
        E = E - c[19]*d*(e_x*e_x);
        E = E - c[20]*(d*d)*(e_x*e_x);
        E = E - c[21]*E*(e_x*e_x);
        E = E - c[22]*E*d*(e_x*e_x);
        E = E - c[23]*E*(d*d)*(e_x*e_x);
        E = E - c[24]*(E*E)*(e_x*e_x);
        E = E - c[25]*(E*E)*d*(e_x*e_x);
        E = E - c[26]*(E*E)*(d*d)*(e_x*e_x);

       i+=1; 

    }

    return E;
}

std::vector<double> xj_sample(std::vector<double> c, double dt, int iters){

    std::vector<double> xjs;
    srand((unsigned int)time(NULL));

    double E1,E2;
    double theta1,theta2;
    double pt1,pt2;
    double xj;
    
    for(int i = 0; i<iters; i++){
    
    E2 = 100;
    E1 = 200;

    std::string e = eventstring(i);
    std::vector<std::vector<double> >prof = profile(e);

    theta1 = ((double)rand() / RAND_MAX)*2*(3.1415926)-3.1415926;
    theta2 = theta1 + 3.1415926;

    pt1 = energyloss(c,E1,theta1,prof,dt);
    pt2 = energyloss(c,E2,theta2,prof,dt);

    if(pt1 < pt2){
        xj = pt1/pt2;
    }
    else{
        xj = pt2/pt1;
    }

    xjs.push_back(xj);

    }
    return xjs;
}

double xj_metric(std::vector<double> xj_sample){
    return 0;
}

double compute(std::vector<double> c,double dt, int iters){
    std::vector<double> xj = xj_sample(c,dt,iters);
    return xj_metric(xj);
}

std::vector<double> grad(double f,std::vector<double> c,double dc,double dt, int iters){

    //double f = compute(c,dt,iters);
    std::vector<double> d = c;
    std::vector<double> g;
    double q = 0;
    for(int i = 0; i<c.size(); i++){
        d[i] += dc;
        q = compute(d,dt,iters);
        g.push_back(q-f/(dc));
        d[i]-= dc;
    }
    return g;
}

std::vector<std::vector<double> > timesteps(std::vector<double> ci,int steps,double stepsize,double dc,double dt,int iters){

    std::vector<double>  c = ci;
    std::vector<double>  g;
    double f;
    std::vector<double> inf;
    std::vector<std::vector<double> > infs;

    for(int i = 0; i<steps;i++){
        f = compute(c,dt,iters);
        g = grad(f,c,dc,dt,iters);
        
        inf.push_back(i);
        for(int j = 0; j<c.size();j++){
            inf.push_back(c[j]);
        }
        inf.push_back(f);
        for(int l = 0; l<c.size();l++){
            inf.push_back(g[l]);
        }
        infs.push_back(inf);
        inf.clear();
        for(int k = 0; k<c.size();k++){
            ci[k] -= g[k]*stepsize;
        }

    }
    return infs;
    
}

void PSOrun(std::string outfilename,std::vector<std::vector<double> > positions, int steps, double stepsize, double dc, double dt, int iters){

     std::vector<std::vector<double> > tsteps;
     std::ofstream file(outfilename);


    for(int i = 0; i<positions.size(); i++){
        std::vector<std::vector<double> > tsteps = timesteps(positions[i],steps,stepsize,dc,dt,iters);

        for(int j = 0; j<tsteps.size(); j++){
            for(int k = 0; k<tsteps[0].size(); k++){
                file << tsteps[j][k] << " ";
            }
            file << std::endl;
        }

    }
    file.close();
}

int main(){
    return 0;

}
