/*=============================================================================
author        : Walter Schreppers
filename      : executer.cpp
description   : Executes a parsetree
bugreport(log):/
=============================================================================*/

#include "executer.h"

using namespace std;

Executer::Executer(TreeNode* tree){
  this->tree = tree;
  functionTable.clear();
  bBreak=false;
  bReturn=false;
}


void Executer::run(){
  bBreak=false;
  bReturn=false;
  //find program block and execute it
  
  TreeNode* node;
  TreeNode::const_iterator i;
  for(i = tree->begin() ; i != tree->end(); ++i ){

    node=*i;
    if( node->getType() == blockNode){
      symtable main;
      symbolTables.push( main ); //new symbol table for main block
      execute( node );               //execute main block
      symbolTables.pop(); //free up stack
      
    }
    else if( node->getType() == functionNode ){
      string funcname=node->firstChild()->getName();
      functionTable[ funcname ] = node; //store for later use
    }

  }
}


void Executer::execute(TreeNode* node){
  switch( node->getType() ){
    case blockNode          : execBlock( node );        break;
    case forNode            : execFor( node );          break;
    case forEachNode        : execForEach( node );      break;
    case whileNode          : execWhile( node );        break;
    case ifNode             : execIf( node );           break;
    case printNode          : execPrint( node );        break;
    case inputNode          : execInput( node );        break;
    case assignNode         : execAssign( node );       break;
    case idNode             : execId( node );           break;
    case constantNode       : execConstant( node );     break; 
    case stringConstantNode : execConstant( node );     break; //for now handle same as ordinary constant
    
    case addNode            : execAdd( node );          break;
    case mulNode            : execMul( node );          break;
    case divNode            : execDiv( node );          break;
    case subNode            : execSub( node );          break;
    case modNode            : execMod( node );          break;
    case minusNode          : execMinus( node );        break;
        
    case nodeGE             : execGE( node );           break; 
    case nodeGT             : execGT( node );           break;
    case nodeLE             : execLE( node );           break;
    case nodeLT             : execLT( node );           break;
    case nodeNE             : execNE( node );           break;
    case nodeEQ             : execEQ( node );           break;
    
    case andNode            : execAnd( node );          break;
    case orNode             : execOr( node );           break;
    case notNode            : execNot( node );          break;
    
    case functionCallNode   : execFunction( node );     break;
    case funcReturnNode     : execRetFunction( node );  break;
    case returnNode         : execReturn( node );       break;
    case exitNode           : execExit( node );         break;
    case breakNode          : execBreak( node );        break;
    
    case runNode            : execRun( node );          break;
    case writeNode          : execWrite( node );        break;
    case substrNode         : execSubstr( node );       break;
    
    default                 : std::cerr<<"Found unsupported node: name='"<<
                              node->getName()<<"', type="<<
                              node->getType()<<" in tree!" << std::endl; 
                              break;
  }  
}


// dunno yet if I will persue this... maybe focus on a more general
// way to import a shared lib into the executer instead ...
void Executer::execGraphics(TreeNode* node){
  //todo open gfx screen with node's params
}

void Executer::mayclearScr(TreeNode* node){
  //todo clear screen defined above
}

void Executer::drawPixel(TreeNode* node){
  //todo draw pixel directly to screen
}


void Executer::createScreenBuffer(TreeNode* node){
  //todo alocate double buffer 
}


void Executer::drawPixelBuffer(TreeNode* node){
  //todo draw pixel to double buffer provided in node param
}


void Executer::swapScreen(TreeNode* node){
  //fast copy buffer to screen node param 1 to node param 2
}

void Executer::drawRectangle(TreeNode* node){
  //ug etc etc unless... hmm well find a better way to write a shared lib that includes these drawing functions
  // into our language...
}


//execute a function
//first child   = function name
//second child  = parameters
void Executer::execFunction( TreeNode* node ){
  string funcname = node->firstChild()->getName();

  //locate function node  
  functable::iterator p=functionTable.find( funcname );
  if( p==functionTable.end() ){
    std::cerr << "RUN ERROR: Call to undefined function : "<<funcname<<"."<<std::endl;
    return;
  }
  
  TreeNode* funcnode    = p->second;
  TreeNode* funcIds     = funcnode->secondChild();
  TreeNode* callparams  = node->secondChild();
    
  //check if number of parameters match
  if( callparams->size() != funcIds->size() ){
    std::cerr << "RUN ERROR: Call to function "<<funcname<<" with wrong number of parameters." << std::endl;
    return;
  }

  //pass parameters to function
  //by adding them to it's symboltable and setting the values
  TreeNode::iterator pfrom,pto=funcIds->begin();
  symtable funcSymTable;
  
  for(pfrom=callparams->begin(); pfrom!=callparams->end(); ++pfrom ){
    
    //execute the parameters which can be expressions
    execute( *pfrom ); 
    
    string idname=(*pto)->getName();
    funcSymTable[idname]= (*pfrom)->getValue();
    ++pto;
  
  }
  
  symbolTables.push(funcSymTable); // use new symboltable for current function
  
  // execute function statement block
  bReturn=false;  // set to true when return is called
  execute( funcnode->thirdChild() );
  bReturn=false;  // function execution done
  
  symbolTables.pop(); //release function symboltable    
}


// execute a function and expect and get return 
// value from stack
// first child   = function name
// second child  = parameters
void Executer::execRetFunction( TreeNode* node ){
  execFunction( node );
  if( runStack.size() == 0 ){
    std::cerr<<"RUN ERROR: function "<<node->firstChild()->getName()
        <<" did not return a value!"<< std::endl;
    return;
  }
  node->setValue( runStack.top() ); //set return val
  runStack.pop(); //remove from stack
}


void Executer::execReturn( TreeNode* node ){
  execute( node->firstChild() ); //execute return expression
  runStack.push( node->firstChild()->getValue() );
  bReturn=true; //notify blocks of return
}


void Executer::execExit( TreeNode* node ){
  execute( node->firstChild() );
  int exitcode = node->firstChild()->getValue().decVal;
  exit(exitcode);
}


void Executer::execBreak( TreeNode* node ){
  bBreak=true; //stops loop block execution
}


void Executer::execBlock( TreeNode* node ){
  //execute all statements in block
  TreeNode::iterator i;
  for( i=node->begin(); i!=node->end(); ++i ){
    execute( *i );

    if( bReturn || bBreak){
      break; //jump out of block
    }
    
  }
}


void Executer::execForEach( TreeNode* node ){
  //cout<<"sorry dude not implemented yet"<<endl;
  TreeNode* id         = node->firstChild();
  TreeNode* expr       = node->secondChild();
  TreeNode* seperator  = node->thirdChild();
  TreeNode* statements = node->fourthChild();
  
  execute( expr );
  execute( seperator );
  
  string idName = id->getName();
  string expStr = expr->getValue().strVal;
  string sepStr = seperator->getValue().strVal;
  
  bBreak=false;
  string::size_type pos;
  while( expStr.size() > 0 ){
    pos=expStr.find(sepStr);

    if( pos == string::npos ){ //no seperator found
      (symbolTables.top())[idName] = expStr; //entire string
      expStr="";
    }
    else{
      ( symbolTables.top() )[idName] = expStr.substr(0,pos);
      expStr.erase( 0, pos + sepStr.size() );
    }
    
    execute( statements );
    if( bBreak || bReturn ) break; //jump out loop;
  }
  bBreak=false;
  
}


void Executer::execFor( TreeNode* node ){
  TreeNode* id=node->firstChild();
  TreeNode* startNode=node->secondChild();
  TreeNode* stopNode=node->thirdChild();
  TreeNode* statements=node->fourthChild();
  
  string name=id->getName();

  execute(startNode);
  //assign startval to id
  Var startVal=startNode->getValue();
  ( symbolTables.top() )[ name ] = startVal;
  
  
  execute(stopNode);
  Var stopVal=stopNode->getValue();
  
  if(node->size() == 4 ){ //for loop without step part
    bBreak=false;
    for( double d=startVal.decVal; d<=stopVal.decVal; d=d+1 ){
      (symbolTables.top() )[name] = d;
      execute( statements );
      if( bBreak || bReturn ) break; //jump out loop
    }
    bBreak=false;
  }
  else{ //for loop with step part
    TreeNode* step  = node->fourthChild();
    statements      = node->fifthChild();
    
    execute(step);
    Var stepVal=step->getValue();
    bBreak=false;
    if( (stepVal.decVal >= 0.0) && (startVal.decVal <= stopVal.decVal) ){
      for( double d=startVal.decVal; d<=stopVal.decVal; d=d+stepVal.decVal ){
        (symbolTables.top() )[name] = d;
        execute( statements );
        if( bBreak || bReturn ) break; //jump out loop
      }
    }
    else if( (stepVal.decVal < 0.0) && (startVal.decVal >= stopVal.decVal) ){
      for( double d=startVal.decVal; d>=stopVal.decVal; d=d+stepVal.decVal ){
        (symbolTables.top() )[name] = d;
        execute( statements );
        if( bBreak || bReturn ) break; //jump out loop
      }    
    }
    bBreak=false;
  }
}


void Executer::execWhile( TreeNode* node ){
  TreeNode* condition = node->firstChild();
  TreeNode* statements = node->secondChild();

  bBreak=false;
  execute( condition );
  while( condition->getValue().decVal != 0 ){
    execute( statements );
    if( bBreak || bReturn ) break; //jump out loop
    execute( condition );
  }
  bBreak=false;
}

     
void Executer::execIf( TreeNode* node ){
  TreeNode* condition = node->firstChild();
  TreeNode* ifblok = node->secondChild();

  //determine if there is an else part
  if( node->size() == 2 ){ //no else
    
    execute( condition );
    if( condition->getValue().decVal != 0 ){
      execute( ifblok );
    }   
 
  }
  else{ //else part given
    TreeNode* elseblok = node->thirdChild();
    execute( condition );
    if( condition->getValue().decVal != 0 ){
      execute( ifblok );
    }
    else{
      execute( elseblok );
    }

  }

}


void Executer::execPrint( TreeNode* node ){
  TreeNode::iterator i;
  for( i=node->begin(); i!=node->end(); ++i ){
    execute( *i ); //execute expression
    cout << fixed << (*i)->getValue();
  }
  cout<<flush;
}

/*     
void Executer::execPrintLn( TreeNode* node ){
  execPrint( node );
  cout<<endl;
}
*/

void Executer::execInput( TreeNode* node ){
  string varName = node->firstChild()->getName();
  Var val;
  
  //ask input from cin
  //cout<<"?"; // basic style , don't like it :)
  cin>>val;
  
  ( symbolTables.top() )[ varName ] = val;  
}

   
void Executer::execAssign( TreeNode* node ){
  TreeNode* var  = node->firstChild();
  TreeNode* expr = node->secondChild();

  execute( expr );
  ( symbolTables.top() )[ var->getName() ] = expr->getValue();
}


void Executer::execId( TreeNode* node ){
  node->setValue( ( symbolTables.top() )[ node->getName() ] );
}


void Executer::execConstant( TreeNode* node ){
  //do nothing, value is already set
}


Var Executer::getVal( TreeNode* node ){
  execute( node );
  return node->getValue();
}

        
void Executer::execAdd( TreeNode* node ){
  node->setValue( getVal( node->firstChild() )
                  + 
                  getVal( node->secondChild() ) );
}

       
void Executer::execMul( TreeNode* node ){
  node->setValue( getVal( node->firstChild() )
                  * 
                  getVal( node->secondChild() ) );
}

       
void Executer::execDiv( TreeNode* node ){
  node->setValue( getVal( node->firstChild() )
                  / 
                  getVal( node->secondChild() ) );
}

       
void Executer::execSub( TreeNode* node ){
  node->setValue( getVal( node->firstChild() )
                  - 
                  getVal( node->secondChild() ) );
}


void Executer::execMod( TreeNode* node ){
  node->setValue( getVal( node->firstChild() )
                  %
                  getVal( node->secondChild() ) );
}

       
void Executer::execLT( TreeNode* node ){
  node->setValue( (double) (
                  getVal( node->firstChild() )
                  < 
                  getVal( node->secondChild() ) 
                  ) 
                );
}


void Executer::execLE( TreeNode* node ){
  node->setValue( (double) (
                  getVal( node->firstChild() )
                  <= 
                  getVal( node->secondChild() ) 
                  ) 
                );
}


void Executer::execGT( TreeNode* node ){
  node->setValue( (double) (
                  getVal( node->firstChild() )
                  >
                  getVal( node->secondChild() ) 
                  ) 
                );
}


void Executer::execGE( TreeNode* node ){
  node->setValue( (double) (
                  getVal( node->firstChild() )
                  >=
                  getVal( node->secondChild() ) 
                  ) 
                );
}


void Executer::execEQ( TreeNode* node ){
  node->setValue( (double) (
                  getVal( node->firstChild() )
                  ==
                  getVal( node->secondChild() ) 
                  ) 
                );
}


void Executer::execNE( TreeNode* node ){
  node->setValue( (double) (
                  getVal( node->firstChild() )
                  !=
                  getVal( node->secondChild() ) 
                  ) 
                );
}


void Executer::execAnd( TreeNode* node ){
  bool nl = getVal( node->firstChild() ).decVal != 0;
  bool nr = getVal( node->secondChild() ).decVal != 0;
  node->setValue( (double) (nl && nr) );
}

       
void Executer::execOr( TreeNode* node ){
  bool nl = getVal( node->firstChild() ).decVal != 0;
  bool nr = getVal( node->secondChild() ).decVal != 0;
  node->setValue( (double) (nl || nr) );
}


void Executer::execNot( TreeNode* node ){
  node->setValue( 1 - getVal( node->firstChild() ).decVal ); 
}


void Executer::execMinus( TreeNode* node ){
  node->setValue( - getVal( node->firstChild() ).decVal ); 
}


string Executer::runCommand( const string& command ){
  FILE *pstream;
  if(  ( pstream = popen( command.c_str(), "r" ) ) == NULL ) return "";
  
  string Line;
  char buf[100];
  
  while( fgets(buf, sizeof(buf), pstream) !=NULL){
    Line += buf;
  }
  pclose(pstream);

  return Line;
}


void Executer::execRun( TreeNode* node ){
  string cmd= getVal( node->firstChild() ).strVal;
  node->setValue( runCommand(cmd) );
}

void Executer::execWrite( TreeNode* node ){
  string fileName = getVal( node->firstChild() ).strVal;
  ofstream out(fileName.c_str());
  if(out.is_open()){
    out<<getVal( node->secondChild() );
  }
  else{
    std::cerr<<"could not open file :"<<fileName<<" for writing"<<std::endl;
  }
  out.close();
  
}

void Executer::execSubstr( TreeNode* node ){
  string id   = node->firstChild()->getName();
  int from    = (int) getVal( node->secondChild() ).decVal-1;
  int to      = (int) getVal( node->thirdChild() ).decVal;
  
  
  string val  = (symbolTables.top())[id].strVal;
  
  if( ( from < to ) && ( from >= 0 ) && ( to < (int)val.size() ) ){
    node->setValue( val.substr( from, to-from ) );
  }
  else{
    std::cerr << "RUN ERROR: substring from, to arguments run out of string boundaries or from pos is not less than to."<< std::endl;
    node->getValue().strVal = "";
  }
}

