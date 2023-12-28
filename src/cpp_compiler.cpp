/*=============================================================================
author        : Walter Schreppers
filename      : cpp_compiler.cpp
description   : Compiles a parsetree
bugreport(log): Work in progress. Alternate way of working. Generate c++ source
file from a .basic file and then compile that. This is easier to implement and
will be more performant however we do have a bigger depencency on gcc when used
=============================================================================*/

#include "cpp_compiler.h"
#include <iomanip>

using namespace std;

CppCompiler::CppCompiler(TreeNode *tree) {
  this->tree = tree;
  functionTable.clear();
  bBreak = false;
  bReturn = false;
}

void CppCompiler::generate(const string &outfile) {
  string header_name = outfile;
  replace_all(header_name, ".cpp", ".h");

  out.open(outfile);
  hdr.open(header_name);

  // in code generation also make doubles always have decimal
  out << std::fixed;
  hdr << std::fixed;

  out << "#include <iostream>" << std::endl;
  out << "#include <string>" << std::endl;

  // we will store function implementations in seperate .h file
  out << "#include \"" << header_name << "\"" << std::endl;
  hdr << "#include \"lib/wsbasic.h\"" << std::endl;

  out << std::endl;
  out << "int main(){" << std::endl;

  // set fixed, matches executer way of displaying (always a decimal in double)
  out << "  std::cout << std::fixed;" << std::endl;

  // we need this for variable type detection and coercion only
  symtable main_symtable;
  symbolTables.push(main_symtable);

  // intial pass, just translate all top level blocks into main()
  // keep track of functions in a functionTable for later use
  TreeNode *node;
  TreeNode::const_iterator i;
  for (i = tree->begin(); i != tree->end(); ++i) {
    node = *i;
    if (node->getType() == functionNode) {
      string funcname = node->firstChild()->getName();
      functionTable[funcname] = node; // store in map for later use
    } else if (node->getType() == blockNode) {
      compile(node, out);
    }
  }

  // if user uses his own return this will be called first.
  // by default always return 0
  out << "  return 0;" << std::endl;
  out << "}" << std::endl;

  symbolTables.pop(); // release main symtable

  out.close();
  hdr.close(); // close output.h
}

void CppCompiler::link(const string &cppfile) {
  string exe_name = cppfile;
  replace_all(exe_name, ".cpp", "");
  string header_name = exe_name + ".h";

  // TODO: have a wsbasic.h and libwsbasic.a here and link with those
  // instead of var.o
  string gcc_command = "g++ -O3 -Wall " + cppfile + " lib/libwsbasic.a -o " + exe_name;
  system(gcc_command.c_str());

  cout << "saved executable '" << exe_name << "'" << std::endl;
}

void CppCompiler::compile(TreeNode *node, ofstream &out) {
  switch (node->getType()) {
    case blockNode:  compBlock(node, out);  break;
    case printNode:  compPrint(node, out);  break;
    case stringConstantNode:  compConstantString(node, out);  break;
    case constantNode:        compConstant(node, out);        break;
    case functionCallNode:    compFunction(node, out);        break;
    case assignNode:          compAssign(node, out);          break;
    case idNode:    compId(node, out);    break;
    case exitNode:  compExit(node, out);  break;
    case addNode:   compAdd(node, out);   break;
    case mulNode:   compMul(node, out);   break;
    case divNode:   compDiv(node, out);   break;
    case subNode:   compSub(node, out);   break;
    case modNode:   compMod(node, out);   break;
    case minusNode: compMinus(node, out); break;
    case nodeGE:    compGE(node, out);    break;
    case nodeGT:    compGT(node, out);    break;
    case nodeLE:    compLE(node, out);    break;
    case nodeLT:    compLT(node, out);    break;
    case nodeNE:    compNE(node, out);    break;
    case nodeEQ:    compEQ(node, out);    break;
    case andNode:   compAnd(node, out);   break;
    case orNode:    compOr(node, out);    break;
    case notNode:   compNot(node, out);   break;
    case whileNode: compWhile(node, out); break;
    case breakNode: compBreak(node, out); break;
    case ifNode:    compIf(node, out);    break;
    case inputNode: compInput(node, out); break;
    case runNode:   compRun(node, out);   break;

    // case funcReturnNode     : compRetFunction( node );  break;
    // case returnNode         : compReturn( node );       break;
    // case forNode            : compFor( node );          break;
    // case forEachNode        : compForEach( node );      break;

    // case writeNode          : compWrite( node );        break;
    // case substrNode         : compSubstr( node );       break;

    default:
      cerr << "Found unsupported node: name='" << node->getName()
           << "', type=" << node->getType() << " in tree!" << std::endl;
      break;
  }
}

void CppCompiler::compBlock(TreeNode *node, ofstream &out) {
  // compile all statements in block
  TreeNode::iterator i;
  for (i = node->begin(); i != node->end(); ++i) {
    out << "  ";
    compile(*i, out);
    out << std::endl;
  }
}

void CppCompiler::compPrint(TreeNode *node, ofstream &out) {
  out << "std::cout";

  TreeNode::iterator i;
  for (i = node->begin(); i != node->end(); ++i) {
    out << " << ";
    compile(*i, out); // compile expression
  }

  out << ";";
}

// we re-use compPrint and just added a newline node to params above
// void CppCompiler::compPrintLn( TreeNode* node ){
//   compPrint( node );
//   cout<<endl;
// }

void CppCompiler::compConstantString(TreeNode *node, ofstream &out) {
  if (node->getName() == "newline") {
    out << "std::endl";
    return;
  }

  // make escaped newline work (todo apply this to all excaped chars)
  string cstr = node->getValue().strVal;
  replace_all(cstr, "\n", "\\n");
  replace_all(cstr, "\r", "\\r");
  // TODO: use generic backslash escape as we need \033 and \u also to work here
  // replace_all(cstr, "\u", "\\u");

  out << "Var(\"" << cstr << "\")";
}

void CppCompiler::compConstant(TreeNode *node, ofstream &out) {
  Var v = node->getValue();
  out << "Var(";
  if (v.bString) {
    out << "\"" << v.strVal << "\"";
  } else {
    out << v.decVal;
  }
  out << ")";
}

void CppCompiler::compAssign(TreeNode *node, ofstream &out) {
  TreeNode *expr = node->secondChild();
  // for our asm and c versions of compiler this wont work anymore
  // however if we want truly dynamic object oriented scripts everything is a
  // Var that way you can go from x = 1.0 to x = "some str" later in your
  // scripts
  string vartype = "Var";
  TreeNode *var = node->firstChild();
  string varname = var->getName();

  // also searching in top might not be enough here. ideally we bubble up...
  if ((symbolTables.top()).find(varname) == (symbolTables.top()).end()) {
    // new veriable insert it into symtable
    // assume it will be a double by setting bstring == false
    Var var_val;
    var_val.bString = false;

    (symbolTables.top())[varname] = var_val;
    out << vartype << " " << varname << " = ";
  } else {
    // variable already existed, regular assign.
    // by using Var type everywhere this just works and the Var class handles
    // conversion/coercion
    out << varname << " = ";
  }

  // again also first assign we should determine the type here and set it for
  // subsecquent assigns
  compile(expr, out);
  out << ";";
}

void CppCompiler::compId(TreeNode *node, ofstream &out) {
  // node->setValue( ( symbolTables.top() )[ node->getName() ] );
  out << node->getName();
}

// compile a function call will also compile the function implementation
//  this way we only compile what is actually used and it allows for setting
//  param types based on how it is called. first child   = function name second
//  child  = parameters this is the variant wihtout a return and should gen a
//  void function implementation
void CppCompiler::compFunction(TreeNode *node, ofstream &out) {
  string funcname = node->firstChild()->getName();

  // locate function node
  functable::iterator p = functionTable.find(funcname);
  if (p == functionTable.end()) {
    cerr << "COMPILE ERROR: Call to undefined function : " << funcname << "."
         << endl;
    exit(1);
  }

  TreeNode *funcnode = p->second;
  TreeNode *funcIds = funcnode->secondChild();
  TreeNode *callparams = node->secondChild();

  // check if number of parameters between call and definition match
  if (callparams->size() != funcIds->size()) {
    // in a more strongly typed language you'll also check the params match in
    // type
    cerr << "COMPILE ERROR: Call to function " << funcname
         << " with wrong number of parameters." << endl;
    exit(1);
  }

  // pass parameters to function
  // by adding them to it's symboltable and setting the values
  TreeNode::iterator pfrom, pto = funcIds->begin();
  symtable funcSymTable;

  out << funcname << "(";
  for (pfrom = callparams->begin(); pfrom != callparams->end(); ++pfrom) {
    // compile the parameters which can be expressions
    if (pfrom != callparams->begin())
      out << ", ";
    compile(*pfrom, out);

    // string idname=(*pto)->getName();
    // funcSymTable[idname]= (*pfrom)->getValue();
    // ++pto;
  }
  out << ");" << std::endl;

  symbolTables.push(funcSymTable); // use new symboltable for current function
  bReturn = false;                 // set to true when return is called

  // compile function definition into header, we also now know the call param
  // types. for now to get poc working we just use double always

  // append function body in hdr
  hdr << "void " << funcname << "(";
  for (pto = funcIds->begin(); pto != funcIds->end(); ++pto) {
    if (pto != funcIds->begin())
      hdr << ", ";
    hdr << "const Var& " << (*pto)->getName();
  }
  hdr << ") {" << std::endl;
  compile(funcnode->thirdChild(), hdr);
  hdr << "}" << std::endl << std::endl;

  bReturn = false;    // function compution done
  symbolTables.pop(); // release function symboltable
}

void CppCompiler::compBinaryOperator(TreeNode *node, const string &opp,
                                     ofstream &out) {
  compile(node->firstChild(), out);
  out << opp;
  compile(node->secondChild(), out);
}

void CppCompiler::compAdd(TreeNode *node, ofstream &out) {
  compBinaryOperator(node, " + ", out);
}

void CppCompiler::compMul(TreeNode *node, ofstream &out) {
  compBinaryOperator(node, " * ", out);
}

void CppCompiler::compDiv(TreeNode *node, ofstream &out) {
  compBinaryOperator(node, " / ", out);
}

void CppCompiler::compSub(TreeNode *node, ofstream &out) {
  compBinaryOperator(node, " - ", out);
}

void CppCompiler::compMod(TreeNode *node, ofstream &out) {
  compBinaryOperator(node, " % ", out);
}

void CppCompiler::compMinus(TreeNode *node, ofstream &out) {
  out << "-";
  compile(node->firstChild(), out);
}

void CppCompiler::compGE(TreeNode *node, ofstream &out) {
  compBinaryOperator(node, " >= ", out);
}

void CppCompiler::compGT(TreeNode *node, ofstream &out) {
  compBinaryOperator(node, " > ", out);
}
void CppCompiler::compLE(TreeNode *node, ofstream &out) {
  compBinaryOperator(node, " <= ", out);
}

void CppCompiler::compLT(TreeNode *node, ofstream &out) {
  compBinaryOperator(node, " < ", out);
}

void CppCompiler::compNE(TreeNode *node, ofstream &out) {
  compBinaryOperator(node, " != ", out);
}

void CppCompiler::compEQ(TreeNode *node, ofstream &out) {
  compBinaryOperator(node, " == ", out);
}

void CppCompiler::compAnd(TreeNode *node, ofstream &out) {
  compBinaryOperator(node, " && ", out);
}

void CppCompiler::compOr(TreeNode *node, ofstream &out) {
  compBinaryOperator(node, " || ", out);
}

void CppCompiler::compNot(TreeNode *node, ofstream &out) {
  out << "!";
  compile(node->firstChild(), out);
}

void CppCompiler::compWhile(TreeNode *node, ofstream &out) {
  TreeNode *condition = node->firstChild();
  TreeNode *statements = node->secondChild();

  out << "while (";
  // bBreak=false;
  compile(condition, out);
  out << ") {" << std::endl;

  compile(statements, out);

  out << "}" << std::endl;

  // bBreak=false;
}

void CppCompiler::compBreak(TreeNode *node, ofstream &out) {
  // bBreak=true; //stops loop block compution
  out << "break";
}

void CppCompiler::compIf(TreeNode *node, ofstream &out) {
  TreeNode *condition = node->firstChild();
  TreeNode *ifblok = node->secondChild();

  // determine if there is an else part
  if (node->size() == 2) { // no else
    out << "if (";
    compile(condition, out);
    out << ") {" << endl;
    compile(ifblok, out);
    out << "}" << endl;
  } else { // else part given
    TreeNode *elseblok = node->thirdChild();
    out << "if (";
    compile(condition, out);
    out << ") {" << endl;

    compile(ifblok, out);
    out << endl << "  }" << endl;

    out << "  else {" << endl;
    compile(elseblok, out);
    out << endl << "  }" << endl;
  }
}

void CppCompiler::compInput(TreeNode *node, ofstream &out) {
  out << "std::cin >> ";
  out << node->firstChild()->getName(); // var name
  out << ";";
  // ( symbolTables.top() )[ varName ] = val;
}

void CppCompiler::compExit(TreeNode *node, ofstream &out) {
  out << "exit( Var(";
  compile(node->firstChild(), out);
  out << ").toInt() );";
}

void CppCompiler::compRun(TreeNode *node, ofstream &out) {
  out << "wsbasic_shell_run(";
  compile(node->firstChild(), out);
  out << ")";
}

// //compile a function and expect and get return
// //value from stack
// //first child   = function name
// //second child  = parameters
// void CppCompiler::compRetFunction( TreeNode* node ){
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
// void CppCompiler::compReturn( TreeNode* node ){
//   compile( node->firstChild() ); //compile return expression
//   runStack.push( node->firstChild()->getValue() );
//   bReturn=true; //notify blocks of return
// }
//
//
//
// void CppCompiler::compForEach( TreeNode* node ){
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
// void CppCompiler::compFor( TreeNode* node ){
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
// void CppCompiler::compWrite( TreeNode* node ){
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
// void CppCompiler::compSubstr( TreeNode* node ){
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
//     cerr<<"RUN ERROR: substring from, to arguments run out of string
//     boundaries or from pos is not less than to."<<endl; node->setValue( "" );
//   }
// }
