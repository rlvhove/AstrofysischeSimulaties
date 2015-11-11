//
//  classes.cpp
//  AstrofysischeSimulaties


#include "classes.h"

Vec& Vec::operator+=(Vec v) {
    _x += v._x;
    _y += v._y;
    return *this;
}

Vec& Vec::operator-=(Vec v){
    _x -= v._x;
    _y -= v._y;
    return *this;
}

Vec& Vec::operator*=(double s) {
    _x *= s;
    _y *= s;
    return *this;
}

Vec& Vec::operator/=(double s) {
    _x /= s;
    _y /= s;
    return *this;
}

ostream& operator<<(ostream& os, const Vec& v){
    os << v._x << ' ' << v._y;
    return os;
}

Object& Object::operator+=(Object obj) {
    position += obj.position;
    velocity += obj.velocity;
    return *this;
}

Object& Object::operator-=(Object obj) {
    position -= obj.position;
    velocity -= obj.velocity;
    return *this;
}

Object& Object::operator*=(double s) {
    position *= s;
    velocity *= s;
    return *this;
}


NBodySystem::NBodySystem(vector<Vec> positions, vector<Vec> velocities) {
    unsigned int i;
    numberOfBodies = positions.size();
    for (i=0; i!=positions.size(); i++) {
        bodies.push_back(Object(positions[i], velocities[i], 1));
    }
}


vector<Vec> NBodySystem::getPositions() const{
    unsigned int i;
    vector<Vec> positions;
    for(i=0; i!= numberOfBodies; i++)
        positions.push_back((bodies[i]).getPosition());
    return positions;
}

vector<Vec> NBodySystem::getVelocities() const{
    unsigned int i;
    vector<Vec> velocities;
    for(i=0; i!= numberOfBodies; i++)
        velocities.push_back((bodies[i]).getVelocity());
    return velocities;
}

NBodySystem& NBodySystem::operator+=(const NBodySystem& system) {
    unsigned int i;
    for(i=0; i!=this->numberOfBodies; i++)
        bodies[i] += (system.bodies)[i];
    return *this;
}

NBodySystem& NBodySystem::operator-=(const NBodySystem& system) {
    unsigned int i;
    for(i=0; i!=this->numberOfBodies; i++)
        bodies[i] -= (system.bodies)[i];
    return *this;
}

NBodySystem& NBodySystem::operator*=(double s) {
    unsigned int i;
    for(i=0; i!=this->numberOfBodies; i++)
        bodies[i] *= s;
    return *this;
}







