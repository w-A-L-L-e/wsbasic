/*=============================================================================
author        : Walter Schreppers
filename      : treenode.h
description   : A node in an n-ary tree
bugreport(log): prevSibling and nextSibling sometimes segfault and are not optimal, in short, don't use em! :)
=============================================================================*/

#ifndef TREENODE_H
#define TREENODE_H

#include <string>
#include <list>
#include "var.h"

enum NodeType { 
                Unknown = -1,
                programNode,   // 0
                functionNode,
                functionCallNode,
                funcReturnNode,
                returnNode,
                exitNode,     // 5
                breakNode,      
                idListNode,
                blockNode,
                forNode,
                forEachNode,  // 10
                whileNode,    // 11
                ifNode,
                printNode,
                inputNode,
                assignNode,
                expressionNode,  // 16
                idNode,
                constantNode,
                stringConstantNode,  // 19
            
                addNode,
                mulNode,
                divNode,
                subNode,
                modNode,
                minusNode,

                nodeGE,
                nodeGT,
                nodeLE,
                nodeLT,
                nodeNE,
                nodeEQ,

                andNode,
                orNode,
                notNode,
                
                runNode,
                writeNode,
                substrNode
                
              };


class TreeNode:public list<TreeNode*> {

  public:
  
    //constructors/destructor
    //=======================
    TreeNode();
    TreeNode(TreeNode*); //give parent

    TreeNode(NodeType);
    TreeNode(NodeType,int,int); //row and col
    
    
    virtual ~TreeNode();
  
    //public members
    //==============
    void init();
    void setParent(TreeNode* p);
    TreeNode* getParent(){return parent;}
    
    virtual void show(int indent=0);
    void showTree(TreeNode* node, int indent=-1) const;
    
    void appendChild(TreeNode*);
    void appendSibling(TreeNode*); //works only if it has parent set!
    
    TreeNode* firstChild();
    TreeNode* secondChild();
    TreeNode* thirdChild();
    TreeNode* fourthChild();
    TreeNode* fifthChild();
    
    TreeNode* lastChild();    
    TreeNode* nextSibling();   
    TreeNode* prevSibling();
    
    NodeType getType();
    void setType(NodeType t);
    
    void setName(const string&);
    string getName() const;  
    
    void setValue(const Var&);
    void setValue(double d){ value = d; }
    void setValue(const string& s){ value = s; }
    
    Var getValue();
    int getRow(){ return fRow; }
    int getCol(){ return fCol; }

    void setStrValue(const string&);
    string getStrValue();
    
    TreeNode::iterator lookup(); //gives location in parent list as iterator (used by prevSibling and nextSibling)
    bool hasChildren(){ return size()!=0; }

    TreeNode& operator=(const TreeNode&);
  
  protected:
    TreeNode* parent;
    
        
  private:

    //private members
    //===============
    void destroy(TreeNode*);

    //private locals
    //==============
    string name;
    NodeType fType;
    Var value;
    string strValue;
    
    int fRow, fCol; //for runtime error messages.
    
};

#endif



