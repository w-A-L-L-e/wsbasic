#include "var.h"

#include <cmath>
#include <iostream>
#include <limits.h>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

// friend definitions
int Var::toInt() {
  if (bString) {
    cerr << "RUN ERROR: can't get int from string type" << endl;
    exit(1);
  }
  return (int) this->decVal;
}

Var operator+(const Var &left, const Var &right) {
  Var temp;
  if (!left.bString && !right.bString) {
    temp.bString = false;
    temp.decVal = left.decVal + right.decVal;
  } else if (left.bString && right.bString) {
    temp.bString = true;
    temp.strVal = left.strVal + right.strVal;
  } else if (left.bString && !right.bString) {
    double v = right.decVal;
    ostringstream os;
    os << v;
    temp.bString = true;
    temp.strVal = left.strVal + os.str();
  }
  else if (!left.bString && right.bString){
    double v = left.decVal;
    ostringstream os;
    os << v;
    temp.bString = true;
    temp.strVal = os.str() + right.strVal;
  }
  else {
    cerr << "RUN ERROR: can't add string + double" << endl;
    exit(1);
  }

  return temp;
}

Var operator-(const Var &left, const Var &right) {
  Var temp;
  if (!left.bString && !right.bString) {
    temp.bString = false;
    temp.decVal = left.decVal - right.decVal;
  } else {
    cerr << "RUN ERROR: can't subtract strings" << endl;
    exit(1);
  }

  return temp;
}

Var operator*(const Var &left, const Var &right) {
  Var temp;
  if (!left.bString && !right.bString) {
    temp.bString = false;
    temp.decVal = left.decVal * right.decVal;
  } else {
    cerr << "RUN ERROR: can't multiply strings" << endl;
    exit(1);
  }

  return temp;
}

Var operator/(const Var &left, const Var &right) {
  Var temp;
  if (!left.bString && !right.bString) {
    temp.bString = false;
    temp.decVal = left.decVal / right.decVal;
  } else {
    cerr << "RUN ERROR: can't divide strings" << endl;
    exit(1);
  }

  return temp;
}

Var operator%(const Var &left, const Var &right) {
  Var temp;
  if (!left.bString && !right.bString) {
    temp.bString = false;
    temp.decVal = (long)left.decVal % (long)right.decVal;
  } else {
    cerr << "RUN ERROR: can't do mod operator on strings" << endl;
    exit(1);
  }

  return temp;
}

bool operator==(const Var &left, const Var &right) {
  if (left.bString && right.bString)
    return left.strVal == right.strVal;
  if (!left.bString && !right.bString)
    return left.decVal == right.decVal;

  // different types always false
  return false;
}

bool operator!=(const Var &left, const Var &right) { return !(left == right); }

bool operator<(const Var &left, const Var &right) {
  if (left.bString && right.bString)
    return left.strVal < right.strVal;
  if (!left.bString && !right.bString)
    return left.decVal < right.decVal;

  // different types always false
  return false;
}

bool operator>(const Var &left, const Var &right) {
  if (left.bString && right.bString)
    return left.strVal > right.strVal;
  if (!left.bString && !right.bString)
    return left.decVal > right.decVal;

  // different types always false
  return false;
}

bool operator<=(const Var &left, const Var &right) {
  //  return ( (left < right) || (left == right) );
  if (left.bString && right.bString)
    return left.strVal <= right.strVal;
  if (!left.bString && !right.bString)
    return left.decVal <= right.decVal;

  // different types always false
  return false;
}

bool operator>=(const Var &left, const Var &right) {
  // return ( (left > right) || (left == right) );
  if (left.bString && right.bString)
    return left.strVal >= right.strVal;
  if (!left.bString && !right.bString)
    return left.decVal >= right.decVal;

  // different types always false
  return false;
}

ostream &operator<<(ostream &out, const Var &obj) {
  if (obj.bString) {
    out << obj.strVal;
  } else {
    out << obj.decVal;
  }
  return out;
}

// todo parse and make double if its double and string type for all else...
istream &operator>>(istream &in, Var &obj) {
  obj.bString = false;
  in >> obj.decVal;
  return in;
}



// public members
Var::Var() {
  bString = false;
  strVal = "";
  decVal = 0;
}

Var::Var(double x) {
  bString = false;
  strVal = "";
  decVal = x;
}

Var::Var(const char* str) {
  decVal = 0;
  bString = true;
  strVal = string(str);
}

Var::Var(const string& str) {
  decVal = 0;
  bString = true;
  strVal = str;
}

Var &Var::operator=(const Var &obj) {
  decVal = obj.decVal;
  strVal = obj.strVal;
  bString = obj.bString;
  return *this;
}

Var& Var::operator=( double n ){
  decVal=n;
  bString=false;
  return *this;
}

Var &Var::operator+=(const Var &obj) {
  *this = *this + obj;
  return *this;
}

Var &Var::operator-=(const Var &obj) {
  *this = *this - obj;
  return *this;
}

Var &Var::operator*=(const Var &obj) {
  *this = *this * obj;
  return *this;
}

Var &Var::operator/=(const Var &obj) {
  *this = *this / obj;
  return *this;
}
Var &Var::operator++() {
  *this = *this + 1;
  return *this;
}

Var Var::operator++(int) {
  Var before = *this;
  *this = *this + 1;
  return before;
}

Var &Var::operator--() {
  *this = *this - 1;
  return *this;
}

Var Var::operator--(int) {
  Var before = *this;
  *this = *this - 1;
  return before;
}

Var Var::operator+() const { return *this; }

Var Var::operator-() const {
  if (bString) {
    cerr << "Can't negate a string." << endl;
    exit(1);
  }
  Var temp;
  temp.decVal = -decVal;
  temp.bString = bString;
  return temp;
}
