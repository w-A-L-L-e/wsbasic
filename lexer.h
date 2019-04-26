/*=============================================================================
author        : Walter Schreppers
filename      : lexer.h
description   : Split an input stream up into tokens, eat up white space and
                comments and keep track of row and column
bugreport(log): column will not be incremented enough when numbers are read
=============================================================================*/

#ifndef LEXER_H
#define LEXER_H

#include <stdlib.h>

#include <string>
#include <fstream>
#include "number.h"

using namespace std;



enum types {
  
  tokIf=-40,
  tokElse,
  tokWhile,
  tokFor,
  tokTo,
  tokStep,
  tokNumber,
  tokString,
  tokId,
  tokProcId,
  tokBegin,
  tokEnd,
  tokPrint,
  tokPrintLn,
  tokInput,
    
  tokOr,
  tokAnd,
  tokNot,
  
  tokGe,
  tokGt,
  tokLe,
  tokLt,
  tokNe,
  tokEq,
  tokAssign,

  tokReturn,
  tokBreak,

  tokForEach,
  tokIn,
  tokSeperated,
  tokBy,
    
  tokRun,
  tokMod,
  tokWrite,
  tokSubstr,
  tokEof,
  tokError

};


struct token {
  Number  val;
  string  str;
  int     type;
};


class Lexer {
  
  public:
    
    //constructor and destructor
    //==========================
    Lexer( ifstream& );
    ~Lexer();
  
    //public members
    //==============
    token lex(); //return token
    int getRow();
    int getCol();
    
    
  private:
    
    //private members
    //===============
    int getChar();
    void ungetChar(int);
    void skipComment();
    void skipWhite();
    void checkKeywords(token&);
    int getNumber(Number&);
    int getName(string&);
    void getStringConstant(token& t);
  
    //private locals
    //==============
    ifstream* in;
    int row,col,prevCol;
};


#endif


