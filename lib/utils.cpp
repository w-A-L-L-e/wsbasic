#include "wsbasic.h"

std::string runCommand( const std::string& command ){
  FILE *pstream;
  
  if(  ( pstream = popen( command.c_str(), "r" ) ) == NULL ) return "";
  
  std::string Line;
  char buf[100];
  
  while( fgets(buf, sizeof(buf), pstream) !=NULL){
    Line += buf;
  }
  pclose(pstream);

  return Line;
}

Var wsbasic_shell_run( const Var& command ){
  return runCommand(command.strVal);
}

