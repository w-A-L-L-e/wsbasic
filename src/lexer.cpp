/*=============================================================================
author        : Walter Schreppers
filename      : lexer.cpp
description   : Turns plaintext into tokens for use in the parser classes
bugreport(log):/
=============================================================================*/

#include "lexer.h"
#include <codecvt>

Lexer::~Lexer(){
}

Lexer::Lexer( ifstream& ifstr ){
  in = &ifstr;
  row=1;
  col=prevCol=1;
}


int Lexer::getRow(){
  return row;
}

int Lexer::getCol(){
  return col;
}

int Lexer::getChar(){
  int i=in->get();
  if( i == '\n' ){
    row++;
    prevCol=col;
    col=1;
  }
  else{
    col++;
  }
  return i;
}

void Lexer::ungetChar(int ch){
  if( ch == '\n' ){
    row--;
    col=prevCol;
  }
  else{
    col--;
  }
  
  in->unget();
}


int Lexer::getNumber(Var& n){
  (*in)>>n;
  if( in->good() ){
    return tokNumber;
  }
  else if( in->eof() ){
    return tokEof;
  }
  else{
    return tokError;
  }
}


int Lexer::getName(string& s){
  char look=getChar();
  if(isalpha(look)){
    while( ( isalpha(look) || isdigit(look) || look == '_') && !in->eof() ){
      s+=look;
      look=getChar();
    }
    ungetChar(look); //read one too much
    return tokId;
  }
  else{
    return tokError;  
  }
}


void Lexer::checkKeywords(token& t){
  if(t.type == tokId){
    if(      t.str == "begin"     ) t.type=tokBegin;
    else if( t.str == "def"       ) t.type=tokDef;
    else if( t.str == "end"       ) t.type=tokEnd;
    else if( t.str == "while"     ) t.type=tokWhile;
    else if( t.str == "if"        ) t.type=tokIf;
    else if( t.str == "else"      ) t.type=tokElse;
    else if( t.str == "print"     ) t.type=tokPrint;
    else if( t.str == "println"   ) t.type=tokPrintLn;
    else if( t.str == "input"     ) t.type=tokInput;
    else if( t.str == "for"       ) t.type=tokFor;
    else if( t.str == "to"        ) t.type=tokTo;
    else if( t.str == "step"      ) t.type=tokStep;
    else if( t.str == "and"       ) t.type=tokAnd;
    else if( t.str == "or"        ) t.type=tokOr;
    else if( t.str == "not"       ) t.type=tokNot;
    else if( t.str == "return"    ) t.type=tokReturn;
    else if( t.str == "exit"      ) t.type=tokExit;
    else if( t.str == "break"     ) t.type=tokBreak;
    else if( t.str == "run"       ) t.type=tokRun;
    else if( t.str == "write"     ) t.type=tokWrite;
    else if( t.str == "substr"    ) t.type=tokSubstr;
    else if( t.str == "foreach"   ) t.type=tokForEach;
    else if( t.str == "in"        ) t.type=tokIn;
    else if( t.str == "seperated" ) t.type=tokSeperated;
    else if( t.str == "by"        ) t.type=tokBy;
    //else if( t.str == "mod"       ) t.type=tokMod;
        
    else t.type=tokId;
  }
}

void Lexer::skipComment(){
  char look=getChar();
  while ( !in->eof() && look == '#' ){
    while( !in->eof() && look!='\n' ){
      look=getChar();
    }
    skipWhite();
    look=getChar();
  }
  ungetChar(look);
}

void Lexer::skipWhite(){
  char look=getChar();
  while( !in->eof() && isspace(look) ) look=getChar();
  ungetChar(look);
}

/* version that works up to c++17
string Lexer::unicodeConvert(char c1, char c2, char c3, char c4){
  std::string unicodePointStr = "";
  unicodePointStr += c1;
  unicodePointStr += c2;
  unicodePointStr += c3;
  unicodePointStr += c4;

  std::istringstream is(unicodePointStr);
  int unicodeCodePoint;
  is >> std::hex >> unicodeCodePoint;

  // Create a UTF-8 string with the Unicode character
  wchar_t unicodeChar = static_cast<wchar_t>(unicodeCodePoint);
  std::wstring_convert<std::codecvt_utf8<wchar_t> > converter;
  std::string utf8String = converter.to_bytes(unicodeChar);

  return utf8String;
}
*/

// version for newer gcc on my linux ubuntu 22.0.4 generated with llama3:
string Lexer::unicodeConvert(char c1, char c2, char c3, char c4) {
  std::string unicodePointStr = "";
  unicodePointStr += c1;
  unicodePointStr += c2;
  unicodePointStr += c3;
  unicodePointStr += c4;

  std::istringstream is(unicodePointStr);
  int unicodeCodePoint;
  is >> std::hex >> unicodeCodePoint;

  // Create a UTF-8 string with the Unicode character
  char utf8Bytes[4]; // max 4 bytes for a UTF-8 code point
  int bytesWritten = 0;
  if (unicodeCodePoint <= 0x7F) {
    utf8Bytes[bytesWritten++] = static_cast<char>(unicodeCodePoint);
  } else if (unicodeCodePoint <= 0x7FF) {
    utf8Bytes[bytesWritten++] = 0xC0 | (unicodeCodePoint >> 6);
    utf8Bytes[bytesWritten++] = 0x80 | (unicodeCodePoint & 0x3F);
  } else if (unicodeCodePoint <= 0xFFFF) {
    utf8Bytes[bytesWritten++] = 0xE0 | (unicodeCodePoint >> 12);
    utf8Bytes[bytesWritten++] = 0x80 | ((unicodeCodePoint >> 6) & 0x3F);
    utf8Bytes[bytesWritten++] = 0x80 | (unicodeCodePoint & 0x3F);
  } else {
    utf8Bytes[bytesWritten++] = 0xF0 | (unicodeCodePoint >> 18);
    utf8Bytes[bytesWritten++] = 0x80 | ((unicodeCodePoint >> 12) & 0x3F);
    utf8Bytes[bytesWritten++] = 0x80 | ((unicodeCodePoint >> 6) & 0x3F);
    utf8Bytes[bytesWritten++] = 0x80 | (unicodeCodePoint & 0x3F);
  }

  std::string utf8String(utf8Bytes, bytesWritten);
  return utf8String;
}

void Lexer::getStringConstant(token& t){
  string constStr=""; 
  int ch=getChar();
  int c1, c2, c3, c4; // for u unicode escape seq
                      //
  while( ch != '"' && !in->eof() ){
    
    if(ch == '\\'){ //escape sequence 
      ch=getChar();
      switch(ch){
        case 'n': constStr+='\n'; break;
        case 't': constStr+='\t'; break;
        case 'f': constStr+='\f'; break;
        case 'u':  // utf \u escape sequence
          c1=getChar(); c2=getChar(); c3=getChar(); c4=getChar();
          constStr += unicodeConvert(c1,c2,c3,c4);
          break;
        case '0':  // ansi escape char support
          c1=getChar(); c2=getChar(); 
          if ((c1=='3') && (c2=='3')) constStr.push_back('\033');
          else {
            cerr << "Unrecognized ansi escape \\0" << c1 << c2 << endl;
          }
          break;
        case '"': constStr+='"';  break;
        default : cerr << "Unrecognized escape char \\"
                       << (char)ch<<" in stringconstant, skipping!"
                       << endl; 
                  break;
      }
    } 
    else if(ch != '"'){ //anything but closing char
      constStr+=(char) ch;
    }
    
    ch=getChar();
  }
  
  t.str=constStr;
  t.type=tokString;
  t.val=0;
  
  if(in->eof()) t.type=tokEof;
}


token Lexer::lex(){
  token t;
  t.val=0;
  t.str="";
  t.type=tokEof;
  
  skipWhite();
  skipComment();
  skipWhite();
  
  char look=getChar();
  
  if(in->eof()){
    t.type=tokEof;
    return t;
  }
  
  if( isalpha(look) ){
    ungetChar(look);
    t.type=getName(t.str);
    checkKeywords(t);
  }
  else if( isdigit(look) ){
    ungetChar(look);
    t.type=getNumber(t.val);
  }
  else{
    switch(look){
      case '>': if( getChar() == '=' ){
                  t.type=tokGe;
                }
                else{
                  ungetChar(look);
                  t.type=tokGt;
                }
                break;
      
      case '<': if( getChar() == '=' ){
                  t.type=tokLe;
                }
                else{
                  ungetChar(look);
                  t.type=tokLt;
                }
                break;
      
      case '!': if( getChar() == '=' ){
                  t.type=tokNe;
                }
                else{
                  ungetChar(look);
                  t.type='!';
                }
                break;
      
      case '=': if( getChar() == '=' ){
                  t.type=tokEq;
                }
                else{
                  ungetChar(look);
                  t.type=tokAssign;
                }
                break;
      
      case '"': getStringConstant( t ); break;

      case '%': t.type=tokMod; break;
      
      default : t.type=look;   break;
    }
  }
  
  return t;
}


