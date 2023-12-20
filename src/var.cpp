/*=============================================================================
author        : Walter Schreppers
filename      : var.cpp
description   : Var class to implement value objects in simplest way 
                that allows intuitive coercion
bugreport(log):/
=============================================================================*/
#include "var.h"

Var::Var(){
  strVal="";
  val=0;
  bString=false;
}

Var::Var( const Var& n ){
  *this=n;
}

void Var::toString(){
  ostringstream os;
  os<<val;
  strVal=os.str();
  bString=true;
}

void Var::toDouble(){
  istringstream is(strVal);
  is>>val;
  bString=false;
}
    

Var& Var::operator=( const Var& n ){
  if(this != &n){

    if( bString == n.bString ){
      strVal=n.strVal;
      val=n.val;
      bString=n.bString;
    }
    
    if(n.bString){
      bString=true;
      strVal=n.strVal;
      
      Var tmp;
      tmp.strVal=n.strVal;
      tmp.toDouble();
      val=tmp.val;
    }
    
    if(!n.bString){
      bString=false;
      val=n.val;
      
      Var tmp;
      tmp.val=n.val;
      tmp.toString();
      strVal=tmp.strVal;
    }

  }
  return *this;
}


Var& Var::operator=( const string& s ){
  strVal=s;
  bString=true;
  return *this;
}


Var& Var::operator=( double n ){
  val=n;
  bString=false;
  return *this;
}



Var& Var::operator+( const Var& n ){
  if(!bString && !n.bString){
    val+=n.val;
  }
  else if(bString && n.bString){
    strVal+=n.strVal;
  }
  else{
    if(!bString){
      bool b=bString; 
      toString();
      bString=b;
    }
    
    if(!n.bString){
      Var tmp=n;
      tmp.toString();
      strVal+=tmp.strVal;
    }
    else{
      strVal+=n.strVal;
    }
    
    val+=n.val;
  }
  return *this;
}


Var& Var::operator%( const Var& n ){
  if(!bString && !n.bString){
    val = (long)val % (long)n.val;
  }
  else if(bString && n.bString){
    //strVal+=n.strVal;
    std::cerr << "Error: modulo not allowd on string" << std::endl;
  }
  else{
    //val+=n.val;
    std::cerr << "Error: invalid type for modulo operator" << std::endl;
  }
  return *this;
}



Var& Var::operator-( const Var& n ){
  if(!bString && !n.bString){
    val-=n.val;
  }
  else if(bString && n.bString){
    //strVal-=n.strVal;
    cerr<<"cannot subtract strings"<<endl;
  }
  else {
    val-=n.val;
    toString();
    bString=false;
  }

  return *this;
}


Var& Var::operator*( const Var& n ){
  if(!bString && !n.bString){
    val*=n.val;
  }
  else if(bString && n.bString){
    //strVal*=n.strVal;
    cerr<<"cannot multiply strings"<<endl;
  }
  else{
    val*=n.val;
    toString();
    bString=false;
  }
  return *this;
}


Var& Var::operator/( const Var& n ){
  if(!bString && !n.bString){
    val/=n.val;
  }
  else if(bString && n.bString){
    //strVal/=n.strVal;
    cerr<<"cannot divide strings"<<endl;
  }
  else{
    val/=n.val;
    toString();
    bString=false;
  }
  return *this;
}



bool Var::operator==( const Var& n ) const {
  if( bString && n.bString ) return strVal==n.strVal; 
  if( !bString && !n.bString ) return val == n.val;
  return false;
}


bool Var::operator!=( const Var& n ) const {
  if( bString && n.bString ) return strVal!=n.strVal; 
  if( !bString && !n.bString ) return val != n.val;
  return false;
}


bool Var::operator<( const Var& n ) const {
  if( bString && n.bString ) return strVal<n.strVal; 
  if( !bString && !n.bString ) return val < n.val;
  return false;
}


bool Var::operator<=( const Var& n ) const {
  if( bString && n.bString ) return strVal<=n.strVal; 
  if( !bString && !n.bString ) return val <= n.val;
  return false;
}


bool Var::operator>( const Var& n ) const {
  if( bString && n.bString ) return strVal>n.strVal; 
  if( !bString && !n.bString ) return val > n.val;
  return false;
}


bool Var::operator>=( const Var& n ) const {
  if( bString && n.bString ) return strVal>=n.strVal; 
  if( !bString && !n.bString ) return val >= n.val;
  return false;
}



//output double or string
ostream& operator<<(ostream& os, const Var& n){
  if(n.bString){
    os<<n.strVal;
  }  
  else{
    os<<n.val;
  }
  return os;
}

//read double
istream& operator>>(istream& is, Var& n){
  n.bString=false;
  is>>n.val;
  return is;
}




