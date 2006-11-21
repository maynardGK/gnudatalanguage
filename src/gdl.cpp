/***************************************************************************
                          gdl.cpp  -  main program
                             -------------------
    begin                : Wed Apr 18 16:58:14 JST 2001
    copyright            : (C) 2002-2006 by Marc Schellens
    email                : m_schellens@users.sourceforge.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "includefirst.hpp"

// should (Makefile.am) not be used anyway for the python module
#ifndef PYTHON_MODULE

#ifndef VERSION
#define VERSION "0.9"
#endif

#include <string>
#include <csignal>
#include <cstdlib>
#include <unistd.h> // isatty

//#include <fenv.h>

#include "str.hpp"
#include "dinterpreter.hpp"
#include "terminfo.hpp"
#include "sigfpehandler.hpp"

#ifdef USE_MPI
#include "mpi.h"
#endif

using namespace std;

static void StartupMessage()
{
  cout << "GDL - GNU Data Language, Version " << VERSION << endl;
  //  cout << "For basic information type HELP,/INFO" << endl;
}

void LibInit(); // defined in libinit.cpp

void AtExit()
{
  cerr << flush; cout << flush; clog << flush;
  // clean up everything
  // (for debugging memory leaks)
  ResetObjects();
}

void InitGDL()
{
  // ncurses blurs the output, initialize TermWidth here
  TermWidth();

  // initializations
  InitObjects();

  // init library functions
  LibInit(); 
    
  // turn on all floating point exceptions
  //  feenableexcept(FE_DIVBYZERO | FE_INVALID | FE_OVERFLOW );

  signal(SIGINT,ControlCHandler);
  signal(SIGFPE,SigFPEHandler);
}

int main(int argc, char *argv[])
{
  if( atexit( AtExit) != 0) cerr << "atexit registration failed." << endl;

  for( SizeT a=1; a< argc; ++a)
    {
      if( string( argv[a]) == "--help")
	{
	  cout << "Usage: gdl [OPTION]" << endl;
	  cout << "Start the GDL interpreter (incremental compiler)" << endl;
	  cout << endl;
	  cout << "Options:" << endl;
	  cout << "  --help     display this message" << endl;
	  cout << "  --version  show version information" << endl;
	  cout << endl;
	  cout << "Homepage: http://gnudatalanguage.sf.net" << endl;
	  return 0;
	}
      if( string( argv[a]) == "--version")
	{
	  cout << "GDL - GNU Data Language, Version " << VERSION << endl;
	  return 0;
	}
    }

  InitGDL();

  if( isatty(0)) StartupMessage();

  // instantiate the interpreter
  DInterpreter interpreter;

  string gdlPath=GetEnvString("GDL_PATH");
  if( gdlPath == "") gdlPath=GetEnvString("IDL_PATH");
  if( gdlPath != "")
    {
      SysVar::SetGDLPath( gdlPath);
    }

  string startup=GetEnvString("GDL_STARTUP");
  if( startup == "") startup=GetEnvString("IDL_STARTUP");
  if( startup == "")
    {
      if (isatty(0)) cout << "'GDL_STARTUP'/'IDL_STARTUP' environment "
	"variables both not set.\n"
	"No startup file read." << endl;
    }
//   else
//     {
//       // if path not given, add users home
//       if( !PathGiven(startup))
// 	{
// 	  string home=GetEnvString("HOME");
// 	  if( home != "") 
// 	    {
// 	      AppendIfNeeded(home,"/");
// 	      startup=home+startup;
// 	    }
// 	}
//     }

#ifdef USE_MPI
  MPI_Init(&argc, &argv);
  int myrank = 0;
  MPI_Comm_rank( MPI_COMM_WORLD, &myrank);
  int size; 
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  int tag = 0;
  char* mpi_procedure = getenv("GDL_MPI");
  if (myrank == 0 && mpi_procedure != NULL){
    for( SizeT i = 0; i < size; i++)
      MPI_Send(mpi_procedure, strlen(mpi_procedure)+1, MPI_CHAR, i, 
	       tag, MPI_COMM_WORLD);
  }
#endif

  interpreter.InterpreterLoop( startup);

  return 0;
}

#endif
