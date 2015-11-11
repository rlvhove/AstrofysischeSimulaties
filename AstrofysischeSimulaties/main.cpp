//
//  main.cpp
//  AstrofysischeSimulaties

//
#include "classes.h"
#include "functions.h"
#include "integration.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <cmath>

using namespace std;

#define PI 3.14159265

//methode om vanuit txt bestand begincondities te kiezen
vector<Object> LaunchFromFile(string cosystem){
    vector<Object> bodies = vector<Object>();
    ifstream input ("solarconditions.txt", ifstream::in); //VOOR NIET LINUX GEBRUIKERS MOET HIER OOK HET PATH GESPECIIFIEERD WORDEN VERMOED IK
    //NAAM VERANDEREN DOOR CORRECTE BESTANDSNAAM
    
    if (cosystem == "Polar"){
        //Laat het programma lopen met begincondities gegeven in poolcoordinaten van een txt bestand  met indeling "r theta v mass(zonsmassas)"
        //eventueel kunnen we de methode herschrijven om automatisch,bijvoorbeeld aan het aantal kolommen te beslissen welk coystem ge gebruiken.
        double r, theta, v, mass;
        while(input >> r >> theta >> v >> mass){
            Vec position = fromPolarToCart(Vec(r,theta));
            Vec velocity = Vec(-1.0*v*sin(theta),v*cos(theta));
            Object object = Object(position,velocity,mass);
            bodies.push_back(object);
        }
    }
    
    if (cosystem == "Carth"){
        //Laat het programma lopen met begincondities gegeven in carthesische coordinaten van een txt bestand met indeling "x y vx vy mass(zonsmassas)"
        double x, y, vx, vy, mass;
        while(input >> x >> y >> vx >> vy >> mass){
            Object object = Object(Vec(x,y),Vec(vx,vy),mass);
            bodies.push_back(object);
        }
    }
    return bodies;
}



void NbodySystem(string cosystem){
    string filename = "Nbodies.txt"; //Verander bestandsnaam!
    double integrationTime = 13; //Integration time is in aard jaren.
    NBodySystem system = NBodySystem(LaunchFromFile(cosystem));
    
    
//    integration(system, filename, 0.001, integrationTime*2*PI, "RG"); // RG voor runge kutta integratie, LF voor leapfrog
    
    
}

void SolarSystem(ofstream& outfile) {
//    Als we G=1 nemen, en we kiezen de eenheid voor afstand 1AU en de eenheid voor massa 1 zonsmassa, dan is de corresponderende eenheid voor de tijd (1 aardjaar / 2pi).
//Deze functie schrijft een bestand weg met daarin op iedere lijn 2*aantal deeltjes kolommen met telkens de xpositie en ypositie voor elke deeltje.
    
    double integrationTime = 1.; //Integration time is in aard jaren.
    double earthmass = 3.003*pow(10,-6);
    
    
    Vec EarthPos = fromPolarToCart(Vec(0.994, 0.554));
    Vec MarsPos = fromPolarToCart(Vec(1.662595, 2.52));
    Vec VenusPos = fromPolarToCart(Vec(0.720, 1.317));
    Vec MercuryPos = fromPolarToCart(Vec(0.350, 2.643));
    Vec JupiterPos = fromPolarToCart(Vec(5.401945, 2.757));
    
    Vec EarthSpeed = Vec(-1*1.0042*sin(0.554), 1.0042*cos(0.554));
    Vec MarsSpeed = Vec(-1*0.73593*sin(2.52), 0.73593*cos(2.52));
    Vec VenusSpeed = Vec(-1*1.1882*sin(1.317), 1.1882*cos(1.317));
    Vec MercurySpeed = Vec(-1*1.7*sin(2.643), 1.7*cos(2.643));
    Vec JupiterSpeed = Vec(-1*0.42817*sin(2.643), 0.42817*cos(2.643));
    
    vector<Object> bodies = vector<Object>();
    Object Sun = Object(Vec(0,0), Vec(0,0), 1);
    Object Earth = Object(EarthPos, EarthSpeed, earthmass);
    Object Mars = Object(MarsPos, MarsSpeed, 0.107*earthmass);
    Object Venus = Object(VenusPos, VenusSpeed, 0.8*earthmass);
    Object Mercury = Object(MercuryPos, MercurySpeed, 0.055*earthmass);
    Object Jupiter = Object(JupiterPos, JupiterSpeed, 317.8*earthmass);
    
    bodies.push_back(Sun);
    bodies.push_back(Earth);
//    bodies.push_back(Mars);
//    bodies.push_back(Venus);
//    bodies.push_back(Mercury);
//    bodies.push_back(Jupiter);
    
    NBodySystem system = NBodySystem(bodies);
    
    integration(system, outfile, "RK", "energy", pow(10.,-4.), integrationTime*2*PI, 1, false);
}

void burrau(ofstream& outfile) {

    Object obj1 = Object(Vec(0,0), Vec(0,0), 5);
    Object obj2 = Object(Vec(3,0), Vec(0,0), 4);
    Object obj3 = Object(Vec(0,4), Vec(0,0), 3);
    vector<Object> bodies;
    bodies.push_back(obj1);
    bodies.push_back(obj2);
    bodies.push_back(obj3);
    
    NBodySystem system = NBodySystem(bodies);
    
    integration(system, outfile, "GBS", "positions", 0.8*pow(10.,-4.), 70, 1000, true);
}

int main(int argc, const char * argv[]) {
    clock_t tStart = clock();
    
    string filename = "/Users/Robijn/Desktop/textfiles/testastro.txt"; //Verander bestandsnaam!
    ofstream outfile(filename);
    outfile << setprecision(15);
    
    burrau(outfile);
//    SolarSystem(outfile);
    
    outfile.close();
    
    cout << (double)(clock() - tStart)/CLOCKS_PER_SEC << endl;
    return 1;
}









