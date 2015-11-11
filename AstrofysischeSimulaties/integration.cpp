//
//  integration.cpp
//  AstrofysischeSimulaties


#include "integration.h"
#include "classes.h"
#include "functions.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <vector>
#include <time.h>
#include <stdlib.h>

using namespace std;

#define CONVERGENCE_CRITERION pow(10., -10.)
#define GBS_ENDPOINT 7

//Update methode voor RG4 integratie.
void updateBodiesRG4(NBodySystem& system, double timestep){
    NBodySystem k1 = timestep*NBodySystem(system.getVelocities(), calcTotalAccel(system));
    
    NBodySystem k2 = timestep*NBodySystem((system + (0.5*k1)).getVelocities(), calcTotalAccel((system + (0.5*k1))));

    NBodySystem k3 = timestep*NBodySystem((system + (0.5*k2)).getVelocities(), calcTotalAccel((system + (0.5*k2))));
    
    NBodySystem k4 = timestep*NBodySystem((system + k3).getVelocities(), calcTotalAccel((system + k3)));
    
    system += ((1/6.)*k1 + (1/3.)*k2 + (1/3.)*k3 + (1/6.)*k4);
    
}

//Modified midpoint method gebruikt in GBS.
NBodySystem modifiedMidpoint(const NBodySystem& system, double macroT, int numberOfSteps) {
    size_t i;
    double timestep = macroT / numberOfSteps;
    NBodySystem z_prev = system;
    NBodySystem z_curr = (z_prev + timestep*NBodySystem(system.getVelocities(), calcTotalAccel(system)));
    NBodySystem z_next;
    
    for (i=2; i <= numberOfSteps; i++) {
        z_next = z_prev + 2*timestep*NBodySystem(z_curr.getVelocities(), calcTotalAccel(z_curr));
        z_prev = z_curr;
        z_curr = z_next;
    }
    return 0.5*(z_curr + z_prev + timestep*NBodySystem(z_curr.getVelocities(), calcTotalAccel(z_curr)));
}


//GBS integratie methode. Stopt het programma als er op een bepaald moment na 'endOfSequence' (standaard 7) modified midpoint methode's geen convergentei volgens breakConstant is.
void GBS(NBodySystem& system, double timestep) {
    double endOfSequence = GBS_ENDPOINT;
    double breakConstant = CONVERGENCE_CRITERION;
    int i, j;
    bool Stopped = false;
    NBodySystem T00 = modifiedMidpoint(system, timestep, 2);
    vector<NBodySystem> currRow;
    currRow.push_back(modifiedMidpoint(system, timestep, 4));
    currRow.push_back(currRow[0] + (1./3)*(currRow[0] - T00));
    if (checkMaxDistance(currRow[1], currRow[0], breakConstant) == true) {
        system = currRow.back(); return;}

    vector<NBodySystem> nextRow;
    
    for (i=2; i <= endOfSequence; i++) {
        nextRow.push_back(modifiedMidpoint(system, timestep, 2*(i+1)));
        for (j=0; j < i; j++) {
            nextRow.push_back(nextRow[j] + (1./(pow(double(i+1)/double(i-j),2) - 1)) * (nextRow[j] - currRow[j]));
        }
        currRow = nextRow;
        nextRow.clear();
        if (checkMaxDistance(currRow.at(i), currRow.at(i-1), breakConstant) == true) {
            Stopped = true; break;}
    }
    if (Stopped == true)
        system = currRow.back();
    else {
        cout << "GBS integration: Macro Timestep is too big." << endl;
        exit(EXIT_FAILURE);
    }
    
}

//Eerste stap in LeapFrog om alle posities een halve tijdstap te verplaatsen. Daarna kan updateLeapFrog gebruikt worden.
void firstLeapFrog(NBodySystem& system, double timestep) {
    system += ((timestep/2.)*NBodySystem(system.getVelocities(), vector<Vec>(system.getNumberOfBodies())) + (pow(timestep,2)/8.)*NBodySystem(calcTotalAccel(system), vector<Vec>(system.getNumberOfBodies())));
}

//Update methode voor LeapFrog.
void updateLeapFrog(NBodySystem& system, double timestep) {
    system += timestep*NBodySystem(vector<Vec>(system.getNumberOfBodies()), calcTotalAccel(system));
    system += timestep*NBodySystem(system.getVelocities(), vector<Vec>(system.getNumberOfBodies()));
}

//Integratie methode
void integration(NBodySystem& system, ofstream& outfile, string integrator, string output, double base, double endpoint, int plotFrequency, bool varTimestep) {
    int i=0;
    double time = 0;
    double timestep = base;
    double E0 = 0;
    updateBodies update = 0;
    
    if (output == "energy") {
        E0 = calcEnergy(system);
        writeEnergyDifferenceToFile(outfile, system, E0, time);
    }
    
    if (output == "positions")
        writePositionsToFile(outfile, system);
    
    if (integrator == "LF")
        firstLeapFrog(system, base);
    
    if (integrator == "LF")
        update = &updateLeapFrog;
    
    if (integrator == "GBS")
        update = &GBS;
    
    if (integrator == "RK")
        update = &updateBodiesRG4;
    
    while (time <= endpoint) {
        if (varTimestep == true)
            timestep = VariableTimestep(base, system);
        
        update(system, timestep);
        
        if (i%plotFrequency == 0 and output == "positions")
            writePositionsToFile(outfile, system);
        
        if (i%plotFrequency == 0 and output == "energy")
            writeEnergyDifferenceToFile(outfile, system, E0, time);
        
        time += timestep;
        i++;
    }
    
}


