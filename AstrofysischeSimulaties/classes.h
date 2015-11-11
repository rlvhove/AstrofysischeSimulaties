//
//  classes.h
//  AstrofysischeSimulaties
//


#ifndef AstrofysischeSimulaties_classes_h
#define AstrofysischeSimulaties_classes_h

#include <cmath>
#include <vector>
#include <iostream>

using namespace std;

//De klasse Vec stelt een 2D vector voor die zal gebruikt worden voor de snelheden en posities.

class Vec {

public:
    Vec() {_x = 0.; _y = 0.;}
    Vec(double x, double y) { _x = x; _y = y;}
    
    double x() const {return _x;}
    double y() const {return _y;}
    double norm() const { return sqrt(_x*_x + _y*_y); }
    double norm2() const { return _x*_x + _y*_y; }
    double norm3() const { double r = sqrt(_x*_x + _y*_y); return r*r*r; }
    
    Vec& operator+=(Vec v);
    Vec& operator-=(Vec v);
    Vec& operator*=(double s);
    Vec& operator/=(double s);
    
//Ouput operator overloading, zodat een Vec in 1x kan geschreven worden naar een file.
    friend ostream& operator<<(ostream& os, const Vec& v);

private:
    double _x;
    double _y;
};

inline Vec operator+(Vec a, Vec b) { return a += b; }
inline Vec operator-(Vec a, Vec b) { return a -= b; }
inline Vec operator*(Vec b, double s) { return b *= s; }
inline Vec operator*(double s, Vec b) { return b *= s; }
inline Vec operator/(double s, Vec b) { return b /= s; }
inline Vec operator/(Vec b, double s) { return b /= s; }

//fromPolarToCart neemt als argument een positie Vec in poolcoö. en geeft een nieuwe Vec terug in Cartesische coö.
inline Vec fromPolarToCart(Vec polar) {
    return Vec(polar.x()*cos(polar.y()), polar.x()*sin(polar.y()));}



//De klasse Object stelt een massa voor en heeft 3 private velden.

class Object {

public:
    Object() {
        this->position = Vec(0,0);
        this->velocity = Vec(0,0);
        mass = 1.;}
    Object(Vec position, Vec velocity, double mass) {
        this->position = position;
        this->velocity = velocity;
        this->mass = mass;}
    Object(const Object& body) {
        this->position = body.position;
        this->velocity = body.velocity;
        this->mass = body.mass;}
    
    Vec getPosition() const {return position;}
    Vec getVelocity() const {return velocity;}
    double getMass() const {return mass;}
    double getKineticEnergy(){return 0.5*mass*velocity.norm2();}
    
    void setPosition(Vec position) {this->position = position;}
    void setVelocity(Vec velocity) {this->velocity = velocity;}
    void setMass(double mass) {this->mass = mass;}
    
//Twee operatoren die de uitbreiding zijn van dezelfde operatoren in Vec en zowel de positie als de snelheid opdaten. De massa blijft onveranderd.
    
    Object& operator+=(Object obj);
    Object& operator-=(Object obj);
    Object& operator*=(double s);
    
private:
    Vec position;
    Vec velocity;
    double mass;
};



//De klasse NBodySystem stelt het totale system voor met alle objecten. Er wordt een vector met objecten bijgehouden en er worden functies geïmplementeerd die handig zullen blijken voor de integratie en de hulpfuncties.

class NBodySystem {
    
public:
    NBodySystem() {bodies = vector<Object>(); numberOfBodies=0;}
    NBodySystem(vector<Object> bodies) {this->bodies = bodies; numberOfBodies = bodies.size();}
    NBodySystem(vector<Vec> positions, vector<Vec> velocities);
    
//getObjectAtI geeft een copy van het object op positie i in bodies terug.
    Object getObjectAtI(size_t i) const {return bodies[i];}
    size_t getNumberOfBodies() const {return numberOfBodies;}
    vector<Vec> getPositions() const;
    vector<Vec> getVelocities() const;
    
//Operatoren uitbreiding van deze in Object. Dit zal handig zijn bij het integreren.
    NBodySystem& operator+=(const NBodySystem& system);
    NBodySystem& operator-=(const NBodySystem& system);
    NBodySystem& operator*=(double s);
    
private:
    vector<Object> bodies;
    size_t numberOfBodies;
};

// +, * operatoren voor 2 NBodySystems.
inline NBodySystem operator+(NBodySystem a, NBodySystem b) { return a += b; }
inline NBodySystem operator-(NBodySystem a, NBodySystem b) { return a -= b; }
inline NBodySystem operator*(double s, NBodySystem b) { return b *= s; }
inline NBodySystem operator*(NBodySystem b, double s) { return b *= s; }

#endif















