//
//  functions.h
//  AstrofysischeSimulaties
//
//

#ifndef __AstrofysischeSimulaties__functions__
#define __AstrofysischeSimulaties__functions__

#include "classes.h"
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

Vec calcAccelByJ(unsigned int, unsigned int, const NBodySystem&);
Vec calcAccel(unsigned int, const NBodySystem&);
vector<Vec> calcTotalAccel(const NBodySystem&);
double calcEnergy(const NBodySystem&);
void writePositionsToFile(std::ofstream&, const NBodySystem&);
void writeEnergyDifferenceToFile(std::ofstream&, const NBodySystem&, double, double);
double VariableTimestep(double, const NBodySystem&);
bool checkMaxDistance(const NBodySystem&, const NBodySystem&, double);



#endif /* defined(__AstrofysischeSimulaties__functions__) */
