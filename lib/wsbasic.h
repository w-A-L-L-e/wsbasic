/*=============================================================================
author        : Walter Schreppers
filename      : var.h
description   : Represent variable values and operations on them
                This is now also the main libwsbasic.a header used by compiled
                scripts as they need the var class to operate correctly.
bugreport(log):
=============================================================================*/

#ifndef WSBASIC_H 
#define WSBASIC_H 

#include "var.h"
#include <string>
#include <iostream>

Var wsbasic_shell_run( const Var& command );

#endif
