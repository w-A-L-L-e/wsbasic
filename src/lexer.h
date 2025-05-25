/*=============================================================================
author        : Walter Schreppers
filename      : lexer.h
description   : Split an input stream up into tokens, eat up white space and
                comments and keep track of row and column
bugreport(log): column will not be incremented enough when vars are read
=============================================================================*/

#ifndef LEXER_H
#define LEXER_H

#include <stdlib.h>

#include "var.h"
#include <fstream>
#include <string>

using namespace std;

enum types {
  tokIf = -40,
  tokElse,
  tokElsif,
  tokWhile,
  tokFor,
  tokTo,
  tokStep,
  tokNumber,
  tokString,
  tokId,
  tokDef,
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
  tokExit,
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
  Var val;
  string str;
  int type;
};

class Lexer {

public:
  // constructor and destructor
  //==========================
  Lexer(ifstream &);
  ~Lexer();

  // public members
  //==============
  token lex(); // return token
  int getRow();
  int getCol();

private:
  // private members
  //===============
  int getChar();
  void ungetChar(int);
  void skipComment();
  void skipWhite();
  void checkKeywords(token &);
  int getNumber(Var &);
  int getName(string &);
  void getStringConstant(token &t);
  std::string unicodeConvert(char c1, char c2, char c3, char c4);

  // private locals
  //==============
  ifstream *in;
  int row, col, prevCol;
};

#endif
