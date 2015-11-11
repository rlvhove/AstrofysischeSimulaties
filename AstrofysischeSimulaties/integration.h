//
//  integration.h
//  AstrofysischeSimulaties


#ifndef __AstrofysischeSimulaties__eulerIntegration__
#define __AstrofysischeSimulaties__eulerIntegration__

#include <stdio.h>
#include <iostream>
#include <fstream>
#include "classes.h"
#include "functions.h"

using namespace std;


void updateBodiesRG4(NBodySystem&, double);
void integration(NBodySystem&, ofstream&, string, string, double, double, int, bool);
void firstLeapFrog(NBodySystem&, double);
void updateLeapFrog(NBodySystem&, double);
NBodySystem modifiedMidpoint(const NBodySystem&, double, int);
void GBS(NBodySystem&, double);
typedef void (*updateBodies)(NBodySystem&, double);



#endif /* defined(__AstrofysischeSimulaties__eulerIntegration__) */
