/***************************************************************************
                          gdlexception.cpp  -  exception handling
                             -------------------
    begin                : July 22 2002
    copyright            : (C) 2002 by Marc Schellens
    email                : m_schellens@users.sf.net
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

#include "gdlexception.hpp"
#include "dnode.hpp"
#include "initsysvar.hpp"
#include "gdljournal.hpp"

//#define GDLDEBUG
#undef GDLDEBUG

using namespace std;

GDLException::GDLException(const string& s, bool pre): 
  ANTLRException(s),
  errorNode(static_cast<RefDNode>(antlr::nullAST)),
  errorNodeP( NULL),
  line( 0), col( 0), prefix( pre),
		  targetEnv( NULL)
{
  // note: This is for cases, when form a destructor is thrown
  // in these cases, program aborts
#ifdef GDLDEBUG
   cerr << s << endl;
#endif
}

GDLException::GDLException(const RefDNode eN, const string& s): 
  ANTLRException(s), 
  errorNode(eN),
  errorNodeP( NULL),
  line( 0), col( 0), prefix( true),
		  targetEnv( NULL)
{
#ifdef GDLDEBUG
   cerr << s << endl;
#endif
}

GDLException::GDLException(const ProgNodeP eN, const string& s): 
  ANTLRException(s), 
  errorNode(static_cast<RefDNode>(antlr::nullAST)),
  errorNodeP( eN),
  line( 0), col( 0), prefix( true),
		  targetEnv( NULL)
{
#ifdef GDLDEBUG
   cerr << s << endl;
#endif
}

GDLException::GDLException(SizeT l, SizeT c, const string& s): 
  ANTLRException(s),
  errorNode(static_cast<RefDNode>(antlr::nullAST)),
  errorNodeP( NULL),
  line( l), col( c), prefix( true),
		  targetEnv( NULL)
{
#ifdef GDLDEBUG
   cerr << s << endl;
#endif
}

void Message(const string& s) 
{
  if( SysVar::Quiet() == 0)
    {
      cout << SysVar::MsgPrefix() << s << endl; 
      lib::write_journal_comment( SysVar::MsgPrefix() + s);
    }
} 

void Warning(const std::string& s) 
{
  cout << SysVar::MsgPrefix() << s << endl; 
  lib::write_journal_comment( SysVar::MsgPrefix() + s);
} 

void ThrowGDLException( const std::string& str)
{
throw GDLException( str);
}

