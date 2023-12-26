#include "wsbasic.h"

string runCommand( const string& command ){
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

Var wsbasic_shell_run( const Var& command ){
  string res = runCommand(command.strVal);
  return Var(res);
}

