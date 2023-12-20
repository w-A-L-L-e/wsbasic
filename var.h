/*=============================================================================
author        : Walter Schreppers
filename      : var.h
description   : Represent variable values and operations on them
bugreport(log):/
=============================================================================*/

#ifndef NUMBER_H
#define NUMBER_H


#include <string>
#include <iostream>
#include <sstream>

using namespace std;

class Var{
  public:
    
    //constructor/destructor
    //======================
    Var();
    Var(const Var&);
    ~Var(){}
    
    
    //public members
    //==============
    void toString(); //explicit conversion to string strVal
    void toDouble(); //explicit conversion to double val
    
    //operators
    //=========
    Var& operator=(const Var&);
    Var& operator=(const string&);
    Var& operator=(double);

    Var& operator+(const Var&);
    Var& operator-(const Var&);
    Var& operator%(const Var&);
    Var& operator*(const Var&);
    Var& operator/(const Var&);

    bool operator==(const Var&) const;
    bool operator!=(const Var&) const;
    bool operator<(const Var&) const;
    bool operator<=(const Var&) const;
    bool operator>(const Var&) const;
    bool operator>=(const Var&) const;

    friend ostream& operator<<(ostream&, const Var&); //outputs double or string
    friend istream& operator>>(istream&, Var&); //reads double
  

    //public members  
    bool bString; //set to true if Var represents a string...
    string strVal;
    double val;

};

#endif

