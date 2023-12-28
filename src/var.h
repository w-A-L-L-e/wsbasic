/*=============================================================================
author        : Walter Schreppers
filename      : var.h
description   : Represent variable values and operations on them
                This is now also the main libwsbasic.a header used by compiled
                scripts.
                TODO: most likely make this a base class and then add
                string, double, int, array as seperate derived classes.
                Then we can use polymorphism (however this will imply some pointers etc).
bugreport(log):
=============================================================================*/

#ifndef WSBASIC_VAR_H 
#define WSBASIC_VAR_H 

#include <string>
#include <iostream>
#include <sstream>

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

  friend std::ostream& operator<<(std::ostream& out, const Var& obj);
  friend std::istream& operator>>(std::istream& in, Var& obj);
  
  public:
    // constructors
    Var();
    Var(double val);
    Var(const char* str);
    Var(const std::string& str);

    int toInt();
    double toDecimal();
    const char* toString();

    Var& operator=(const Var& obj);
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
   
    bool bString;
    double decVal;
    std::string strVal;

    // TODO: add integer also (but most likely switch to polymorphism first...)
    // bool    bDecimal;
    // int     intVal;
};
#endif
