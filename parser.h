/*=============================================================================
author        :Walter Schreppers
filename      :parser.h
description   :Parse source code by calling lexer repeadetly and build a tree
               so that it can be executed by Executer
bugreport(log):/
=============================================================================*/


/*======================= THE GRAMMAR =============================

  BNF for arithmetic expressions (improved unary minus)

    <expression>    ::= <term> [<addop> <term>]*
    <term>          ::= <signed factor> [<mulop> <signed factor>]*
    <signed factor> ::= [<addop>] <factor>
    <factor>        ::= <integer> | <variable> | (<expression>)



  DONE DIFFERENTLY!: 
  BNF for boolean algebra

    <b-expression>::= <b-term> [<orop> <b-term>]*
    <b-term>      ::= <not-factor> [AND <not-factor>]*
    <not-factor>  ::= [NOT] <b-factor>
    <b-factor>    ::= <b-literal> | <b-variable> | (<b-expression>)


  ... if, while, write, writeln have also been done...

=================================================================*/

#ifndef PARSER_H
#define PARSER_H


#include "lexer.h"
#include "treenode.h"


class Parser{
  public:
    //constructor and destructor
    //==========================
    Parser(ifstream& in);
    ~Parser();

    //public members
    //==============        
    bool parse();
    TreeNode* getTree();
        
  
  private:
  
    //private members
    //===============
    bool isAddOp(token);
    bool isMulOp(token);
    
    void getToken();
    void Match(int);
    void Error(const string&);
    void Expected(const string&);

    TreeNode* Program();
    TreeNode* Function();
    TreeNode* IdList();
    TreeNode* ParamList();
    TreeNode* Block();
    TreeNode* Statement();
    
    TreeNode* runFunction();
    TreeNode* writeFunction();
    TreeNode* substrFunction();

    TreeNode* getId();
    TreeNode* signedFactor();
    TreeNode* Factor();
    TreeNode* Term();
    TreeNode* Expression();
    
    TreeNode* Assignment  ( const string& );
    TreeNode* FunctionCall( const string& );
    TreeNode* Other();
    
    TreeNode* While();
    TreeNode* For();
    TreeNode* ForEach();
    TreeNode* If();
    TreeNode* getString();
    TreeNode* Print();
    TreeNode* NewLineNode();
    TreeNode* PrintLn();
    TreeNode* Input();
    TreeNode* Return();
    TreeNode* Break();

    //string toString(number);

    //private locals
    //==============    
    Lexer* lexer;
    token look;
    
    TreeNode* tree; 
    bool bNoErrors;
    int row,col;
};

#endif


