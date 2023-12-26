/*=============================================================================
author        : Walter Schreppers
filename      : var.h
description   : Represent variable values and operations on them
                This is now also the main libwsbasic.a header used by compiled
                scripts as they need the var class to operate correctly.
bugreport(log):
=============================================================================*/

#ifndef WSBASIC_VAR_H 
#define WSBASIC_VAR_H 

#include <string>
#include <iostream>
#include <sstream>

using namespace std;

class Var {
  // public friend operators
  friend Var operator+(const Var& left, const Var& right);
  friend Var operator-(const Var& left, const Var& right);
  friend Var operator*(const Var& left, const Var& right);
  friend Var operator/(const Var& left, const Var& right);
  friend Var operator%(const Var& left, const Var& right);

  friend bool operator==(const Var& left, const Var& right);
  friend bool operator!=(const Var& left, const Var& right);
  friend bool operator<(const Var& left, const Var& right);
  friend bool operator>(const Var& left, const Var& right);
  friend bool operator<=(const Var& left, const Var& right);
  friend bool operator>=(const Var& left, const Var& right);

  friend ostream& operator<<(ostream& out, const Var& obj);
  friend istream& operator>>(istream& in, Var& obj);
  
  public:
    // constructors
    Var();
    Var(double val);
    Var(const string& sval);

    int toInt();

    // Var(const char[]); // gives ambiguity errors
    // operator int(); // this gives loads of ambiguity errors
    // Var& operator=(const char[]);

    Var& operator=(const Var& obj);
    Var& operator=(const string&);
    Var& operator=(double);

    Var& operator+=(const Var& obj);
    Var& operator-=(const Var& obj);
    Var& operator*=(const Var& obj);
    Var& operator/=(const Var& obj);
    Var& operator++();
    Var operator++(int);
    Var& operator--();
    Var operator--(int);
    Var operator+() const;
    Var operator-() const;
   
  // TODO: make some getters/setters and return these to private
  // private:
    double val;
    string strVal;
    bool bString;
};
#endif
