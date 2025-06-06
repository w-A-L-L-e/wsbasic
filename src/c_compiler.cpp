/*=============================================================================
author        : Walter Schreppers
filename      : c_compiler.cpp
description   : Compiles a parsetree
bugreport(log): Work in progress. Alternate way of working. Generate c source
file from a .basic file and then compile that. This is easier to implement and
will be more performant however we do have a bigger depencency on gcc when used
=============================================================================*/

#include "c_compiler.h"

using namespace std;

CCompiler::CCompiler(TreeNode* tree){
  this->tree = tree;
  functionTable.clear();
  bBreak=false;
  bReturn=false;
}

void CCompiler::generate(const string& outfile){
  out.open(outfile);
  hdr.open("output.h");

  out << "#include <stdio.h>" << std::endl;
  out << "#include <stdlib.h>" << std::endl;
  out << "#include <strings.h>" << std::endl;

  // we will store function implementations in seperate .h file
  out << "#include \"output.h\"" << std::endl; 
  out << std::endl;
  out << "int main(){" << std::endl;
  
  // we need this for variable type detection and coercion only
  symtable main_symtable; 
  symbolTables.push(main_symtable);

  // intial pass, just translate all top level blocks into main() 
  // keep track of functions in a functionTable for later use
  TreeNode* node;
  TreeNode::const_iterator i;
  for(i = tree->begin() ; i != tree->end(); ++i ){
    node=*i;
    if (node->getType() == functionNode ) {
      string funcname=node->firstChild()->getName();
      functionTable[ funcname ] = node; // store in map for later use
    } 
    else if (node->getType() == blockNode) {
      compile(node, out);
    }
  }

  // if user uses his own return this will be called first.
  // by default always return 0
  out << "  return 0;" << std::endl;
  out << "}" << std::endl;

  symbolTables.pop(); //release main symtable

  out.close();
  hdr.close(); // close output.h
}

void CCompiler::link(const string& cppfile){
  // TODO: output file = cfile without .c extension
  string gcc_command = "gcc " + cppfile + " -o output";
  int res = system(gcc_command.c_str());

  if (res != 0) cerr << "gcc compile failed" << endl;

  cout << "saved executable 'output'" << std::endl;

}

void CCompiler::compile(TreeNode* node, ofstream& out){
  switch( node->getType() ){
    case blockNode          : compBlock( node, out );           break;
    case printNode          : compPrint( node, out );           break;
    case stringConstantNode : compConstantString( node, out);   break;
    case assignNode         : compAssign( node, out );          break;
    case idNode             : compId( node, out );              break;
    case exitNode           : compExit( node, out );            break;
    case constantNode       : compConstant( node, out );        break; 

    case functionCallNode   : compFunction( node, out );        break;
    case addNode            : compAdd( node, out );             break;
    case mulNode            : compMul( node, out );             break;
    case divNode            : compDiv( node, out );             break;
    case subNode            : compSub( node, out );             break;
    case modNode            : compMod( node, out );             break;
    case minusNode          : compMinus( node, out );           break;

    case nodeGE             : compGE( node, out );              break; 
    case nodeGT             : compGT( node, out );              break;
    case nodeLE             : compLE( node, out );              break;
    case nodeLT             : compLT( node, out );              break;
    case nodeNE             : compNE( node, out );              break;
    case nodeEQ             : compEQ( node, out );              break;
    case andNode            : compAnd( node, out );             break;
    case orNode             : compOr( node, out );              break;
    case notNode            : compNot( node, out );             break;

    case whileNode          : compWhile( node, out );           break;
    case breakNode          : compBreak( node, out );           break;
    case ifNode             : compIf( node, out );              break;
    // case inputNode          : compInput( node, out );           break;

    // case funcReturnNode     : compRetFunction( node );  break;
    // case returnNode         : compReturn( node );       break;
    // case forNode            : compFor( node );          break;
    // case forEachNode        : compForEach( node );      break;
  
    // case runNode            : compRun( node );          break;
    // case writeNode          : compWrite( node );        break;
    // case substrNode         : compSubstr( node );       break;
     
    default                 : cerr<<"Found unsupported node: name='"<<
                              node->getName()<<"', type="<<
                              node->getType()<<" in tree!"<<std::endl; 
                              break;
  }  
}

 
void CCompiler::compBlock( TreeNode* node, ofstream& out ){
  //compile all statements in block
  TreeNode::iterator i;
  for( i=node->begin(); i!=node->end(); ++i ){
    out << "  ";
    compile( *i, out );
    out << std::endl;
  }
}

void CCompiler::compPrint( TreeNode* node, ofstream& out ){
  TreeNode::iterator i;
  out << "printf(\"%f %s\", ";
  for( i=node->begin(); i!=node->end(); ++i ){
    // again here we need the type. to keep it simple we assume number/double here always
    if (i!=node->begin()) out << ",";
    compile( *i, out ); //compile expression
  }
  out << ");";
}

void CCompiler::compConstantString( TreeNode* node, ofstream& out ){
  if (node->getName() == "newline") {
    out << "\"\\n\"";
    return;
  }

  out << "\"" << node->getValue().strVal << "\"";
}

void CCompiler::compConstant( TreeNode* node, ofstream& out ){
  Var v = node->getValue();
  if (v.bString) {
    out << "\"" << v.strVal << "\"";
  }
  else {
    out << v.decVal;
  }
}

void CCompiler::compAssign( TreeNode* node, ofstream& out ){
  TreeNode* expr = node->secondChild();
  
  // assume double but ideally we should compile first to get type and then output it!
  // problem we would need to compile the RHS first for this without outputing to stream
  // then determine type and output double or not and then output the RHS code...
  string vartype = "double";
  TreeNode* var = node->firstChild();
  string varname = var->getName();

   // also searching in top might not be enough here. ideally we bubble up...
   if ( (symbolTables.top()).find(varname) == (symbolTables.top()).end()) {
     // new veriable insert it into symtable
     // assume it will be a double by setting bstring == false
     Var var_val;
     var_val.bString = false;
 
     ( symbolTables.top() )[ varname ] = var_val;
     out << vartype << " " << varname << " = ";
   }
   else {
     // variable already existed, regular assign.
     // again we should typecheck this here as x = "hello" then later x = x / 2 -> should give error
     // as we cant device strings by 2...
     out << varname << " = ";
   }

  // again also first assign we should determine the type here and set it for subsecquent assigns
  compile(expr, out);

  // out << expr->getValue().val; // this is for a constant
  out << ";";
}


// maybe we can get away with doing a compile type thing without writing to output
// void CCompiler::compExpressionType( TreeNode* node ){
// this should just iterate our ast and determine expression type that would be returned when executed
// }


 
void CCompiler::compId( TreeNode* node, ofstream& out ){
  // TODO: store type here on symtable instead of val!!!
  // node->setValue( ( symbolTables.top() )[ node->getName() ] );
  out << node->getName();
}

void CCompiler::compExit( TreeNode* node, ofstream& out ){
  out << "exit(";
  // todo: verify expression is an int or double here
  compile( node->firstChild(), out );
  out << ");";
}


//compile a function call will also compile the function implementation
// this way we only compile what is actually used and it allows for setting param types based on how
// it is called.
// first child   = function name
// second child  = parameters
// this is the variant wihtout a return and should gen a void function implementation
void CCompiler::compFunction( TreeNode* node, ofstream& out ){
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
    
  // check if number of parameters between call and definition match
  if( callparams->size() != funcIds->size() ){
    // in a more strongly typed language you'll also check the params match in type
    cerr<<"RUN ERROR: Call to function "<<funcname<<" with wrong number of parameters."<<endl;
    return;
  }

  //pass parameters to function
  //by adding them to it's symboltable and setting the values
  TreeNode::iterator pfrom,pto=funcIds->begin();
  symtable funcSymTable;
  
  out << funcname << "(";
  for(pfrom=callparams->begin(); pfrom!=callparams->end(); ++pfrom ){
    //compile the parameters which can be expressions
    if (pfrom != callparams->begin()) out<< ", ";
    compile( *pfrom, out ); 
    
    // string idname=(*pto)->getName();
    // funcSymTable[idname]= (*pfrom)->getValue();
    // ++pto;
  }
  out << ");" << std::endl;
  
  symbolTables.push(funcSymTable); //use new symboltable for current function
  bReturn=false; //set to true when return is called

  // compile function definition into header, we also now know the call param types.
  // for now to get poc working we just use double always

  // append function body in hdr
  hdr << "void " << funcname << "(";
  for(pto = funcIds->begin(); pto != funcIds->end(); ++pto){
    if (pto != funcIds->begin()) hdr << ", ";
    hdr << "double " << (*pto)->getName();
  }
  hdr << ") {" << std::endl;
  compile( funcnode->thirdChild(), hdr );
  hdr << "}" << std::endl << std::endl;

  bReturn=false; //function compution done
  symbolTables.pop(); //release function symboltable    
}


void CCompiler::compBinaryOperator( TreeNode* node, const string& opp, ofstream& out) {
  compile( node->firstChild(), out );
  out << opp;
  compile( node-> secondChild(), out );
}
        
void CCompiler::compAdd( TreeNode* node, ofstream& out ){
  compBinaryOperator(node, " + ", out);
}

void CCompiler::compMul( TreeNode* node, ofstream& out ){
  compBinaryOperator(node, " * ", out);
}

void CCompiler::compDiv( TreeNode* node, ofstream& out ){
  compBinaryOperator(node, " / ", out);
}

void CCompiler::compSub( TreeNode* node, ofstream& out ){
  compBinaryOperator(node, " - ", out);
}

void CCompiler::compMod( TreeNode* node, ofstream& out ){
  compBinaryOperator(node, " % ", out);
}

void CCompiler::compMinus( TreeNode* node, ofstream& out ){
  out << "-";
  compile( node->firstChild(), out);
}

void CCompiler::compGE( TreeNode* node, ofstream& out ){
  compBinaryOperator(node, " >= ", out);
}

void CCompiler::compGT( TreeNode* node, ofstream& out ){
  compBinaryOperator(node, " > ", out);
}
void CCompiler::compLE( TreeNode* node, ofstream& out ){
  compBinaryOperator(node, " <= ", out);
}

void CCompiler::compLT( TreeNode* node, ofstream& out ){
  compBinaryOperator(node, " < ", out);
}

void CCompiler::compNE( TreeNode* node, ofstream& out ){
  compBinaryOperator(node, " != ", out);
}

void CCompiler::compEQ( TreeNode* node, ofstream& out ){
  compBinaryOperator(node, " == ", out);
}

void CCompiler::compAnd( TreeNode* node, ofstream& out ){
  compBinaryOperator(node, " && ", out);
}

void CCompiler::compOr( TreeNode* node, ofstream& out ){
  compBinaryOperator(node, " || ", out);
}

void CCompiler::compNot( TreeNode* node, ofstream& out ){
  out << "!";
  compile( node->firstChild(), out);
}


void CCompiler::compWhile( TreeNode* node, ofstream& out ){
  TreeNode* condition = node->firstChild();
  TreeNode* statements = node->secondChild();

  out << "while (";
  // bBreak=false;
  compile( condition, out );
  out << ") {" << std::endl;

  // TODO: increment compile indent here
  compile( statements, out );

  out << "  }" << std::endl;

 // bBreak=false;
}

void CCompiler::compBreak( TreeNode* node, ofstream& out ){
  // bBreak=true; //stops loop block compution
  out << "break";
}

void CCompiler::compIf( TreeNode* node, ofstream& out ){
  TreeNode* condition = node->firstChild();
  TreeNode* ifblok = node->secondChild();

  //determine if there is an else part
  if( node->size() == 2 ){ //no else
    out << "if (";
    compile( condition, out );
    out << ") {" << endl;
    compile( ifblok, out );
    out << "}" << endl;
  }
  else{ //else part given
    TreeNode* elseblok = node->thirdChild();
    out << "if (";
    compile( condition, out );
    out << ") {" << endl;

    compile( ifblok, out );
    out << endl << "  }" << endl;

    out << "  else {" << endl;
    compile( elseblok, out );
    out << endl << "  }" << endl;
  }
}

 
// void CCompiler::compInput( TreeNode* node ){
//   string varName = node->firstChild()->getName();
//   Var val;
//   
//   //ask input from cin
//   //cout<<"?"; // basic style , don't like it :)
//   cin>>val;
//   
//   ( symbolTables.top() )[ varName ] = val;  
// }



// GETVAL AND SETVAL IS NOT NEEDED HERE!
// Var CCompiler::getVal( TreeNode* node, ofstream& out ){
//   compile( node, out );
//   return node->getValue();
// }

// //compile a function and expect and get return 
// //value from stack
// //first child   = function name
// //second child  = parameters
// void CCompiler::compRetFunction( TreeNode* node ){
//   compFunction( node );
//   if( runStack.size() == 0 ){
//     cerr<<"RUN ERROR: function "<<node->firstChild()->getName()
//         <<" did not return a value!"<<endl;
//     return;
//   }
//   node->setValue( runStack.top() ); //set return val
//   runStack.pop(); //remove from stack
// }
// 
// 
// void CCompiler::compReturn( TreeNode* node ){
//   compile( node->firstChild() ); //compile return expression
//   runStack.push( node->firstChild()->getValue() );
//   bReturn=true; //notify blocks of return
// }
// 
// 
// 
// void CCompiler::compForEach( TreeNode* node ){
//   //cout<<"sorry dude not implemented yet"<<endl;
//   TreeNode* id         = node->firstChild();
//   TreeNode* expr       = node->secondChild();
//   TreeNode* seperator  = node->thirdChild();
//   TreeNode* statements = node->fourthChild();
//   
//   compile( expr );
//   compile( seperator );
//   
//   string idName = id->getName();
//   string expStr = expr->getValue().strVal;
//   string sepStr = seperator->getValue().strVal;
//   
//   bBreak=false;
//   string::size_type pos;
//   while( expStr.size() > 0 ){
//     pos=expStr.find(sepStr);
// 
//     if( pos == string::npos ){ //no seperator found
//       (symbolTables.top())[idName] = expStr; //entire string
//       expStr="";
//     }
//     else{
//       ( symbolTables.top() )[idName] = expStr.substr(0,pos);
//       expStr.erase( 0, pos + sepStr.size() );
//     }
//     
//     compile( statements );
//     if( bBreak || bReturn ) break; //jump out loop;
//   }
//   bBreak=false;
//   
// }
// 
// void CCompiler::compFor( TreeNode* node ){
//   TreeNode* id=node->firstChild();
//   TreeNode* startNode=node->secondChild();
//   TreeNode* stopNode=node->thirdChild();
//   TreeNode* statements=node->fourthChild();
//   
//   string name=id->getName();
// 
//   compile(startNode);
//   //assign startval to id
//   Var startVal=startNode->getValue();
//   ( symbolTables.top() )[ name ] = startVal;
//   
//   
//   compile(stopNode);
//   Var stopVal=stopNode->getValue();
//   
//   if(node->size() == 4 ){ //for loop without step part
//     bBreak=false;
//     for( double d=startVal.val; d<=stopVal.val; d=d+1 ){
//       (symbolTables.top() )[name] = d;
//       compile( statements );
//       if( bBreak || bReturn ) break; //jump out loop
//     }
//     bBreak=false;
//   }
//   else{ //for loop with step part
//     TreeNode* step  = node->fourthChild();
//     statements      = node->fifthChild();
//     
//     compile(step);
//     Var stepVal=step->getValue();
//     bBreak=false;
//     if( (stepVal.val >= 0.0) && (startVal.val <= stopVal.val) ){
//       for( double d=startVal.val; d<=stopVal.val; d=d+stepVal.val ){
//         (symbolTables.top() )[name] = d;
//         compile( statements );
//         if( bBreak || bReturn ) break; //jump out loop
//       }
//     }
//     else if( (stepVal.val < 0.0) && (startVal.val >= stopVal.val) ){
//       for( double d=startVal.val; d>=stopVal.val; d=d+stepVal.val ){
//         (symbolTables.top() )[name] = d;
//         compile( statements );
//         if( bBreak || bReturn ) break; //jump out loop
//       }    
//     }
//     bBreak=false;
//   }
// 
// }
// 
// /*     
// void CCompiler::compPrintLn( TreeNode* node ){
//   compPrint( node );
//   cout<<endl;
// }
// */
// 
////     
// 
// void CCompiler::compNot( TreeNode* node ){
//   node->setValue( 1 - getVal( node->firstChild() ).val ); 
// }
// 
// 
// string CCompiler::runCommand( const string& command ){
//   FILE *pstream;
//   
//   if(  ( pstream = popen( command.c_str(), "r" ) ) == NULL ) return "";
//   
//   string Line;
//   char buf[100];
//   
//   while( fgets(buf, sizeof(buf), pstream) !=NULL){
//     Line += buf;
//   }
//   pclose(pstream);
// 
//   return Line;
// }
// 
// 
// void CCompiler::compRun( TreeNode* node ){
//   string cmd= getVal( node->firstChild() ).strVal;
//   node->setValue( runCommand(cmd) );
// }
// 
// void CCompiler::compWrite( TreeNode* node ){
//   string fileName = getVal( node->firstChild() ).strVal;
//   ofstream out(fileName.c_str());
//   if(out.is_open()){
//     out<<getVal( node->secondChild() );
//   }
//   else{
//     cerr<<"could not open file :"<<fileName<<" for writing"<<endl;
//   }
//   out.close();
//   
// }
// 
// void CCompiler::compSubstr( TreeNode* node ){
//   string id   = node->firstChild()->getName();
//   int from    = (int) getVal( node->secondChild() ).val-1;
//   int to      = (int) getVal( node->thirdChild() ).val;
//   
//   
//   string val  = (symbolTables.top())[id].strVal;
//   
//   if( ( from < to ) && ( from >= 0 ) && ( to < (int)val.size() ) ){
//     node->setValue( val.substr( from, to-from ) );
//   }
//   else{
//     cerr<<"RUN ERROR: substring from, to arguments run out of string boundaries or from pos is not less than to."<<endl;
//     node->setValue( "" );
//   }
// }
