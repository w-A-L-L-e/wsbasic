/*=============================================================================
author        : Walter Schreppers
filename      : compiler.cpp
description   : Compiles a parsetree
bugreport(log): Work in progress. Alternate way of working. Generate c source
file from a .basic file and then compile that. This is easier to implement and
will be more performant however we do have a bigger depencency on gcc when used
=============================================================================*/

#include "c_compiler.h"

CCompiler::CCompiler(TreeNode* tree){
  this->tree = tree;
  functionTable.clear();
  bBreak=false;
  bReturn=false;
}

void CCompiler::run(){
  bBreak=false;
  bReturn=false;
  
  // compile program block
  TreeNode* node;
  TreeNode::const_iterator i;
  for(i = tree->begin() ; i != tree->end(); ++i ){

    node=*i;
    if( node->getType() == blockNode){
      symtable main;
      symbolTables.push( main ); //new symbol table for main block
      compile( node );  //compile into asm
      symbolTables.pop(); //free up stack
      
    }
    else if( node->getType() == functionNode ){
      string funcname=node->firstChild()->getName();
      functionTable[ funcname ] = node; //store for later use
    }

  }

}

void CCompiler::generate(const string& outfile){
  ofstream out(outfile);

  out << "#include <iostream>" << endl;
  out << endl;
  out << "int main(){" << endl;
  out << "  // todo generate code here by iterating our tree..." << endl;
  out << "  std::cout << \"Hello world!\" << std::endl;" << endl;
  out << "  return 0;" << endl;
  out << "}" << endl;

  out.close();
}

void CCompiler::link(const string& cppfile){
  // TODO: output file = cfile without .c extension
  string gcc_command = "g++ " + cppfile + " -o output";
  system(gcc_command.c_str());

  cout << "saved executable 'output'" << endl;

}

void CCompiler::compile(TreeNode* node){
  switch( node->getType() ){
    case blockNode          : compBlock( node );        break;
    case forNode            : compFor( node );          break;
    case forEachNode        : compForEach( node );      break;
    case whileNode          : compWhile( node );        break;
    case ifNode             : compIf( node );           break;
    case printNode          : compPrint( node );        break;
    case inputNode          : compInput( node );        break;
    case assignNode         : compAssign( node );       break;
    case expressionNode     : compExpression( node );   break;
    case idNode             : compId( node );           break;
    case constantNode       : compConstant( node );     break; 
    case stringConstantNode : compConstant( node );     break; //for now handle same as ordinary constant
    
    case addNode            : compAdd( node );          break;
    case mulNode            : compMul( node );          break;
    case divNode            : compDiv( node );          break;
    case subNode            : compSub( node );          break;
    case modNode            : compMod( node );          break;
    case minusNode          : compMinus( node );        break;
        
    case nodeGE             : compGE( node );           break; 
    case nodeGT             : compGT( node );           break;
    case nodeLE             : compLE( node );           break;
    case nodeLT             : compLT( node );           break;
    case nodeNE             : compNE( node );           break;
    case nodeEQ             : compEQ( node );           break;
    
    case andNode            : compAnd( node );          break;
    case orNode             : compOr( node );           break;
    case notNode            : compNot( node );          break;
    
    case functionCallNode   : compFunction( node );     break;
    case funcReturnNode     : compRetFunction( node );  break;
    case returnNode         : compReturn( node );       break;
    case breakNode          : compBreak( node );        break;
    
    case runNode            : compRun( node );          break;
    case writeNode          : compWrite( node );        break;
    case substrNode         : compSubstr( node );       break;
    
    default                 : cerr<<"Found unsupported node: name='"<<
                              node->getName()<<"', type="<<
                              node->getType()<<" in tree!"<<endl; 
                              break;
  }  
}

// dunno yet if I will persue this... maybe focus on a more general
// way to import a shared lib into the compiler instead ...
void CCompiler::compGraphics(TreeNode* node){
  //todo open gfx screen with node's params
}

void CCompiler::mayclearScr(TreeNode* node){
  //todo clear screen defined above
}

void CCompiler::drawPixel(TreeNode* node){
  //todo draw pixel directly to screen
}


void CCompiler::createScreenBuffer(TreeNode* node){
  //todo alocate double buffer 
}

void CCompiler::drawPixelBuffer(TreeNode* node){
  //todo draw pixel to double buffer provided in node param
}

void CCompiler::swapScreen(TreeNode* node){
  //fast copy buffer to screen node param 1 to node param 2
}

void CCompiler::drawRectangle(TreeNode* node){
  //ug etc etc unless... hmm well find a better way to write a shared lib that includes these drawing functions
  // into our language...
}


//compile a function
//first child   = function name
//second child  = parameters
void CCompiler::compFunction( TreeNode* node ){
  string funcname = node->firstChild()->getName();

  //locate function node  
  functable::iterator p=functionTable.find( funcname );
  if( p==functionTable.end() ){
    cerr<<"RUN ERROR: Call to undefined function : "<<funcname<<"."<<endl;
    return;
  }
  
  TreeNode* funcnode    = p->second;
  TreeNode* funcIds     = funcnode->secondChild();
  TreeNode* callparams  = node->secondChild();
    
  //check if number of parameters match
  if( callparams->size() != funcIds->size() ){
    cerr<<"RUN ERROR: Call to function "<<funcname<<" with wrong number of parameters."<<endl;
    return;
  }

  //pass parameters to function
  //by adding them to it's symboltable and setting the values
  TreeNode::iterator pfrom,pto=funcIds->begin();
  symtable funcSymTable;
  
  for(pfrom=callparams->begin(); pfrom!=callparams->end(); ++pfrom ){
    
    //compile the parameters which can be expressions
    compile( *pfrom ); 
    
    string idname=(*pto)->getName();
    funcSymTable[idname]= (*pfrom)->getValue();
    ++pto;
  
  }
  
  symbolTables.push(funcSymTable); //use new symboltable for current function
  
  //compile function statement block
  bReturn=false; //set to true when return is called
  compile( funcnode->thirdChild() );
  bReturn=false; //function compution done
  
  symbolTables.pop(); //release function symboltable    
}


//compile a function and expect and get return 
//value from stack
//first child   = function name
//second child  = parameters
void CCompiler::compRetFunction( TreeNode* node ){
  compFunction( node );
  if( runStack.size() == 0 ){
    cerr<<"RUN ERROR: function "<<node->firstChild()->getName()
        <<" did not return a value!"<<endl;
    return;
  }
  node->setValue( runStack.top() ); //set return val
  runStack.pop(); //remove from stack
}


void CCompiler::compReturn( TreeNode* node ){
  compile( node->firstChild() ); //compile return expression
  runStack.push( node->firstChild()->getValue() );
  bReturn=true; //notify blocks of return
}


void CCompiler::compBreak( TreeNode* node ){
  bBreak=true; //stops loop block compution
}


void CCompiler::compBlock( TreeNode* node ){
  //compile all statements in block
  TreeNode::iterator i;
  for( i=node->begin(); i!=node->end(); ++i ){
    compile( *i );

    if( bReturn || bBreak){
      break; //jump out of block
    }
    
  }
}




void CCompiler::compForEach( TreeNode* node ){
  //cout<<"sorry dude not implemented yet"<<endl;
  TreeNode* id         = node->firstChild();
  TreeNode* expr       = node->secondChild();
  TreeNode* seperator  = node->thirdChild();
  TreeNode* statements = node->fourthChild();
  
  compile( expr );
  compile( seperator );
  
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
    
    compile( statements );
    if( bBreak || bReturn ) break; //jump out loop;
  }
  bBreak=false;
  
}



void CCompiler::compFor( TreeNode* node ){
  TreeNode* id=node->firstChild();
  TreeNode* startNode=node->secondChild();
  TreeNode* stopNode=node->thirdChild();
  TreeNode* statements=node->fourthChild();
  
  string name=id->getName();

  compile(startNode);
  //assign startval to id
  Var startVal=startNode->getValue();
  ( symbolTables.top() )[ name ] = startVal;
  
  
  compile(stopNode);
  Var stopVal=stopNode->getValue();
  
  if(node->size() == 4 ){ //for loop without step part
    bBreak=false;
    for( double d=startVal.val; d<=stopVal.val; d=d+1 ){
      (symbolTables.top() )[name] = d;
      compile( statements );
      if( bBreak || bReturn ) break; //jump out loop
    }
    bBreak=false;
  }
  else{ //for loop with step part
    TreeNode* step  = node->fourthChild();
    statements      = node->fifthChild();
    
    compile(step);
    Var stepVal=step->getValue();
    bBreak=false;
    if( (stepVal.val >= 0.0) && (startVal.val <= stopVal.val) ){
      for( double d=startVal.val; d<=stopVal.val; d=d+stepVal.val ){
        (symbolTables.top() )[name] = d;
        compile( statements );
        if( bBreak || bReturn ) break; //jump out loop
      }
    }
    else if( (stepVal.val < 0.0) && (startVal.val >= stopVal.val) ){
      for( double d=startVal.val; d>=stopVal.val; d=d+stepVal.val ){
        (symbolTables.top() )[name] = d;
        compile( statements );
        if( bBreak || bReturn ) break; //jump out loop
      }    
    }
    bBreak=false;
  }

}



void CCompiler::compWhile( TreeNode* node ){

  TreeNode* condition = node->firstChild();
  TreeNode* statements = node->secondChild();

  bBreak=false;
  compile( condition );
  while( condition->getValue().val != 0 ){
    compile( statements );
    //if( bBreak || bReturn ) break; //jump out loop
    compile( condition );
  }
  bBreak=false;
}

     
void CCompiler::compIf( TreeNode* node ){

  TreeNode* condition = node->firstChild();
  TreeNode* ifblok = node->secondChild();

  //determine if there is an else part
  if( node->size() == 2 ){ //no else
    
    compile( condition );
    if( condition->getValue().val != 0 ){
      compile( ifblok );
    }   
 
  }
  else{ //else part given
    TreeNode* elseblok = node->thirdChild();
    compile( condition );
    if( condition->getValue().val != 0 ){
      compile( ifblok );
    }
    else{
      compile( elseblok );
    }

  }

}


void CCompiler::compPrint( TreeNode* node ){

  TreeNode::iterator i;
  for( i=node->begin(); i!=node->end(); ++i ){
    compile( *i ); //compile expression
    cout<< fixed<< (*i)->getValue();
  }
  cout<<flush;
}


/*     
void CCompiler::compPrintLn( TreeNode* node ){
  compPrint( node );
  cout<<endl;
}
*/


void CCompiler::compInput( TreeNode* node ){
  string varName = node->firstChild()->getName();
  Var val;
  
  //ask input from cin
  //cout<<"?"; // basic style , don't like it :)
  cin>>val;
  
  ( symbolTables.top() )[ varName ] = val;  
}

   
void CCompiler::compAssign( TreeNode* node ){
  TreeNode* var  = node->firstChild();
  TreeNode* expr = node->secondChild();

  compile( expr );
  ( symbolTables.top() )[ var->getName() ] = expr->getValue();
}

    
void CCompiler::compExpression( TreeNode* node ){
  cerr<<"compExpression is not implemented, because it should not be needed!"<<endl;
}


void CCompiler::compId( TreeNode* node ){
  node->setValue( ( symbolTables.top() )[ node->getName() ] );
}

void CCompiler::compConstant( TreeNode* node ){
  //do nothing, value is already set
}

Var CCompiler::getVal( TreeNode* node ){
  compile( node );
  return node->getValue();
}

        
void CCompiler::compAdd( TreeNode* node ){
  node->setValue( getVal( node->firstChild() )
                  + 
                  getVal( node->secondChild() ) );
}

       
void CCompiler::compMul( TreeNode* node ){
  node->setValue( getVal( node->firstChild() )
                  * 
                  getVal( node->secondChild() ) );
}

       
void CCompiler::compDiv( TreeNode* node ){
  node->setValue( getVal( node->firstChild() )
                  / 
                  getVal( node->secondChild() ) );
}

       
void CCompiler::compSub( TreeNode* node ){
  node->setValue( getVal( node->firstChild() )
                  - 
                  getVal( node->secondChild() ) );
}

void CCompiler::compMod( TreeNode* node ){
  node->setValue( getVal( node->firstChild() )
                  %
                  getVal( node->secondChild() ) );
}

       
void CCompiler::compLT( TreeNode* node ){
  node->setValue( (double) (
                  getVal( node->firstChild() )
                  < 
                  getVal( node->secondChild() ) 
                  ) 
                );
}

void CCompiler::compLE( TreeNode* node ){
  node->setValue( (double) (
                  getVal( node->firstChild() )
                  <= 
                  getVal( node->secondChild() ) 
                  ) 
                );
}

void CCompiler::compGT( TreeNode* node ){
  node->setValue( (double) (
                  getVal( node->firstChild() )
                  >
                  getVal( node->secondChild() ) 
                  ) 
                );
}

void CCompiler::compGE( TreeNode* node ){
  node->setValue( (double) (
                  getVal( node->firstChild() )
                  >=
                  getVal( node->secondChild() ) 
                  ) 
                );
}


void CCompiler::compEQ( TreeNode* node ){
  node->setValue( (double) (
                  getVal( node->firstChild() )
                  ==
                  getVal( node->secondChild() ) 
                  ) 
                );
}


void CCompiler::compNE( TreeNode* node ){
  node->setValue( (double) (
                  getVal( node->firstChild() )
                  !=
                  getVal( node->secondChild() ) 
                  ) 
                );
}


  
void CCompiler::compAnd( TreeNode* node ){
  bool nl = getVal( node->firstChild() ).val != 0;
  bool nr = getVal( node->secondChild() ).val != 0;
  node->setValue( (double) (nl && nr) );
}

       
void CCompiler::compOr( TreeNode* node ){
  bool nl = getVal( node->firstChild() ).val != 0;
  bool nr = getVal( node->secondChild() ).val != 0;
  node->setValue( (double) (nl || nr) );
}


void CCompiler::compNot( TreeNode* node ){
  node->setValue( 1 - getVal( node->firstChild() ).val ); 
}


void CCompiler::compMinus( TreeNode* node ){
  node->setValue( - getVal( node->firstChild() ).val ); 
}


string CCompiler::runCommand( const string& command ){
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


void CCompiler::compRun( TreeNode* node ){
  string cmd= getVal( node->firstChild() ).strVal;
  node->setValue( runCommand(cmd) );
}

void CCompiler::compWrite( TreeNode* node ){
  string fileName = getVal( node->firstChild() ).strVal;
  ofstream out(fileName.c_str());
  if(out.is_open()){
    out<<getVal( node->secondChild() );
  }
  else{
    cerr<<"could not open file :"<<fileName<<" for writing"<<endl;
  }
  out.close();
  
}

void CCompiler::compSubstr( TreeNode* node ){
  string id   = node->firstChild()->getName();
  int from    = (int) getVal( node->secondChild() ).val-1;
  int to      = (int) getVal( node->thirdChild() ).val;
  
  
  string val  = (symbolTables.top())[id].strVal;
  
  if( ( from < to ) && ( from >= 0 ) && ( to < (int)val.size() ) ){
    node->setValue( val.substr( from, to-from ) );
  }
  else{
    cerr<<"RUN ERROR: substring from, to arguments run out of string boundaries or from pos is not less than to."<<endl;
    node->setValue( "" );
  }
}

