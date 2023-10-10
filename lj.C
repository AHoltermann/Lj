
#include <complex>
#include <tuple>
#include <sstream>
#include <fstream>
#include <iostream>
#include <string>
#include <cmath>
#include <vector>

static const double pi = 3.14159265358979;

double shape(double v2,
double v3, 
double psi2, 
double psi3,
double r,
double theta){

// Evaluate the shape border r(theta)
double sum = 1 + 2*v2*cos(2*(theta - psi2))+2*v3*cos(3*(theta-psi3));
return sum*r;
}

double lj(
double v2 = 0,
double v3 = 0, 
double psi2 = 0, 
double psi3 = 0, 
double r = 5,  
double x = 1,  
double y = 1,
double a = pi/4){

// a given in radians
double slope_x = cos(a);
double slope_y = sin(a);

double l = 0;
//cout << x << endl;
//cout << y << endl;
double x0 = x;
double y0 = y;
double radius = 0;
double theta = 0;

while (radius < shape(v2,v3,psi2,psi3,r,theta)){
    l += 0.00000001;
    x += slope_x*l;
    y += slope_y*l;
    //cout << l << endl;
    //cout << radius << endl;
    radius = sqrt(x*x+y*y);
    theta = atan2(y,x);
}
    //cout << radius << endl;
    //cout << "theta:" << theta << endl;
    //cout << shape(v2,v3,psi2,psi3,r,theta) << endl;
    //cout << l << endl;
    //cout << sqrt((x-x0)*(x-x0)+(y-y0)*(y-y0)) << endl;
return l;
}


