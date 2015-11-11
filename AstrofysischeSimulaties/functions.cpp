//
//  functions.cpp
//  AstrofysischeSimulaties



#include "functions.h"
#include "classes.h"
#include <vector>
#include <fstream>
#include <iostream>

using namespace std;

//calcAccel berekent de versnelling op object i door object j in het NBodySystem.
Vec calcAccelByJ(unsigned int i, unsigned int j, const NBodySystem& system){
    Object object_i = system.getObjectAtI(i);
    Object object_j = system.getObjectAtI(j);
    Vec positionDifference = (object_i.getPosition() - object_j.getPosition());
    return -1*object_j.getMass()*positionDifference / positionDifference.norm3();
}


//calcTotalAccel geeft een vector terug met daarin op positie de versnelling van deeltje i door alle andere deeltjes in het NBodySystem.
vector<Vec> calcTotalAccel(const NBodySystem& system) {
    size_t N = system.getNumberOfBodies();
    vector<Vec> totalAccel(N, Vec());
    unsigned int i, j;
    
    for (i=0; i!=N; i++){
        for (j=i+1; j != N; j++) {
            if (i != j) {
                totalAccel[i] += calcAccelByJ(i, j, system);
                totalAccel[j] += calcAccelByJ(j, i, system);
            }
        }
    }
    return totalAccel;
}


//calcEnergy calculates the total energy of the NBodySystem.
double calcEnergy(const NBodySystem& system){
    size_t N = system.getNumberOfBodies();
    unsigned int i, j;
    double Energy = 0;
    
    for(i=0; i!=N; i++){
        Energy += (system.getObjectAtI(i)).getKineticEnergy();
        for(j=i+1; j!=N; j++) {
            double mass_i = (system.getObjectAtI(i)).getMass();
            double mass_j = (system.getObjectAtI(j)).getMass();
            Vec positionDifference = (system.getObjectAtI(i)).getPosition() - (system.getObjectAtI(j).getPosition());
            Energy += -1*mass_i*mass_j / positionDifference.norm();
        }
    }
    return Energy;
}

//writePositionsToFile writes the current positions of the N bodies in system to the file 'file', and uses the output operator overloading of Vec and NBodySystem.

void writePositionsToFile(std::ofstream& file, const NBodySystem& system) {
    size_t N = system.getNumberOfBodies();
    unsigned int i;
    for(i=0; i!=N; i++) {
        file << (system.getObjectAtI(i)).getPosition() << ' ';
    }
    file << endl;
}


//writeEnergyDifferenceToFile schrijft het relatieve energieverschil naar een file gegeven en NBodySystem, een beginenergie en een een bepaalde tijd.
void writeEnergyDifferenceToFile(std::ofstream& file, const NBodySystem& system, double E0, double time) {
    
    file <<  time << ' ' << abs((calcEnergy(system) - E0) / E0) << endl;
    
}


double VariableTimestep(double base, const NBodySystem& system) {
    size_t N = system.getNumberOfBodies();
    size_t i, j;
    double distance = 1.;
    double compare;
    for (i=0; i!=N; i++) {
        for (j=i+1; j!=N; j++) {
            compare = ((system.getObjectAtI(i)).getPosition() - (system.getObjectAtI(j)).getPosition()).norm();
            if (compare < distance) distance = compare;
        }
    }
    return base * distance;
}

bool checkMaxDistance(const NBodySystem& system1, const NBodySystem& system2, double breakConstant) {
    size_t i;
    Vec pos1, pos2, vel1, vel2;
    for (i = 0; i!= system1.getNumberOfBodies(); i++) {
        pos1 = system1.getObjectAtI(i).getPosition();
        pos2 = system2.getObjectAtI(i).getPosition();
        vel1 = system1.getObjectAtI(i).getVelocity();
        vel2 = system2.getObjectAtI(i).getVelocity();
        if ((pos1 - pos2).norm() > breakConstant or (vel1 - vel2).norm() > breakConstant) {
            return false;
        }
    }
    return true;
}













