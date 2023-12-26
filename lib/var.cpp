#include "var.h"

#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <limits.h>
using namespace std;


//friend functions definitions
Var operator+(const Var& left, const Var& right) {
  Var temp;
  if(!left.bString && !right.bString){
    temp.bString = false;
    temp.val = left.val + right.val;
  }
  else if(left.bString && right.bString) {
    temp.bString = true;
    temp.strVal = left.strVal + right.strVal;
  }
  else{
    cerr << "RUN ERROR: can't add string + double" << endl;
    exit(1);
  }

  return temp;
}

Var operator-(const Var& left, const Var& right) {
  Var temp;
  if(!left.bString && !right.bString) {
    temp.bString = false;
    temp.val = left.val - right.val;
  }
  else{
    cerr << "RUN ERROR: can't subtract strings" << endl;
    exit(1);
  }

  return temp;
}

Var operator*(const Var& left, const Var& right) {
  Var temp;
  if(!left.bString && !right.bString) {
    temp.bString = false;
    temp.val = left.val * right.val;
  }
  else{
    cerr << "RUN ERROR: can't multiply strings" << endl;
    exit(1);
  }

  return temp;

}

Var operator/(const Var& left, const Var& right) {
  Var temp;
  if(!left.bString && !right.bString) {
    temp.bString = false;
    temp.val = left.val / right.val;
  }
  else{
    cerr << "RUN ERROR: can't divide strings" << endl;
    exit(1);
  }

  return temp;

}

Var operator%(const Var& left, const Var& right) {
  Var temp;
  if(!left.bString && !right.bString) {
    temp.bString = false;
    temp.val = (long)left.val % (long)right.val;
  }
  else{
    cerr << "RUN ERROR: can't do mod operator on strings" << endl;
    exit(1);
  }

  return temp;

}



bool operator==(const Var& left, const Var& right) {
  if (left.bString && right.bString) return left.strVal == right.strVal;
  if (!left.bString && !right.bString) return left.val == right.val;

  // different types always false
  return false;
}

bool operator!=(const Var& left, const Var& right) {
  return !(left == right);
}

bool operator<(const Var& left, const Var& right) {
  if (left.bString && right.bString) return left.strVal < right.strVal;
  if (!left.bString && !right.bString) return left.val < right.val;

  // different types always false
  return false;
}

bool operator>(const Var& left, const Var& right) {
  if (left.bString && right.bString) return left.strVal > right.strVal;
  if (!left.bString && !right.bString) return left.val > right.val;

  // different types always false
  return false;
}

bool operator<=(const Var& left, const Var& right) {
  //  return ( (left < right) || (left == right) );
  if (left.bString && right.bString) return left.strVal <= right.strVal;
  if (!left.bString && !right.bString) return left.val <= right.val;

  // different types always false
  return false;

}

bool operator>=(const Var& left, const Var& right) {
  // return ( (left > right) || (left == right) );
  if (left.bString && right.bString) return left.strVal >= right.strVal;
  if (!left.bString && !right.bString) return left.val >= right.val;

  // different types always false
  return false;
}

ostream& operator<<(ostream& out, const Var& obj) {
  if(obj.bString){
    out << obj.strVal;
  }  
  else{
    out << obj.val;
  }
  return out;
}

// todo parse and make double if its double and string type for all else...
istream& operator>>(istream& in, Var& obj) {
  obj.bString=false;
  in >> obj.val;
  return in;
}



//member function definition
Var::Var() {
  bString = false;
  strVal = "";
  val = 0;
}

Var::Var(double x) {
  bString = false;
  strVal = "";
  val = x;
}


Var& Var::operator=(const Var& obj) {
  val = obj.val;
  strVal = obj.strVal;
  bString = obj.bString;
  return *this;
}

Var& Var::operator+=(const Var& obj) {
  *this = *this+obj;
  return *this;
}

Var& Var::operator-=(const Var& obj) {
  *this = *this-obj;
  return *this;
}

Var& Var::operator*=(const Var& obj) {
  *this = *this*obj;
  return *this;
}

Var& Var::operator/=(const Var& obj) {
  *this = *this/obj;
  return *this;
}
Var& Var::operator++() {
  *this = *this+1;
  return *this;
}

Var Var::operator++(int) {
  Var before = *this;
  *this = *this+1;
  return before;
}

Var& Var::operator--() {
  *this = *this-1;
  return *this;
}

Var Var::operator--(int) {
  Var before = *this;
  *this = *this-1;
  return before;
}

Var Var::operator+() const {
  return *this;
}

Var Var::operator-() const {
  if (bString) {
    cerr <<"Can't negate a string."<<endl;
    exit(1);
  }
  Var temp;
  temp.val = -val;
  temp.bString = bString;
  return temp;
}

