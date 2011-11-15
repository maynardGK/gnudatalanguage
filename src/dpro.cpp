/***************************************************************************
                          dpro.cpp  -  GDL procedures/functions
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

// DPro/DFun classes

#include "includefirst.hpp"

#include "dpro.hpp"
#include "dvar.hpp"
#include "str.hpp"
#include "objects.hpp"

#include "GDLTreeParser.hpp"
#include "GDLParser.hpp" // SA: GDLParser::CompileOpt for isObsolete()/isHidden()

// print out AST tree
//#define GDL_DEBUG
//#undef GDL_DEBUG

#ifdef GDL_DEBUG
#include "print_tree.hpp"
#endif

using namespace std;

// vtable
DSub::~DSub() {}


// DLib ******************************************************
DLib::DLib( const string& n, const string& o, const int nPar_, 
	    const string keyNames[],
	    const string warnKeyNames[], const int nParMin_): 
  DSub(n,o)
{
  nPar=nPar_;
  nParMin = nParMin_;
  
  // find out number of keywords and set them
  SizeT nKey_=0;
  if( keyNames != NULL)
    {
    while( keyNames[nKey_] != "") ++nKey_;
    }

  key.resize(nKey_);
  for( SizeT k=0; k<nKey_; ++k) key[k]=keyNames[k];

  if( nKey_ >= 1)
    if( keyNames[0] == "_EXTRA")
      {
	extra = EXTRA;
	extraIx = 0;
      }
    else if( keyNames[0] == "_REF_EXTRA")
      {
	extra = REFEXTRA;
	extraIx = 0;
      }

  SizeT nWarnKey_=0;
  if( warnKeyNames != NULL)
    {
    while( warnKeyNames[nWarnKey_] != "") ++nWarnKey_;
    }

  warnKey.resize(nWarnKey_);
  for( SizeT wk=0; wk<nWarnKey_; ++wk) warnKey[wk]=warnKeyNames[wk];
}

const string DLibPro::ToString()
{
  string s;
  if( object != "") s += object+"::";
  s += name;
  if( nPar == -1)
    s += ",[inf. Args]";
  else if( nPar == 1)
    s += ",["+i2s( nPar)+" Arg]";
  else if( nPar > 1)
    s += ",["+i2s( nPar)+" Args]";
  for( SizeT i=0; i<key.size(); ++i)
    s += ","+key[ i];//+"=val";
  return s;
}

const string DLibFun::ToString()
{
  string s = "res=";
  if( object != "") s += object+"::";
  s += name+"(";
  if( nPar == -1)
    s += "[inf. Args]";
  else if( nPar == 1)
    s += "["+i2s( nPar)+" Arg]";
  else if( nPar > 1)
    s += "["+i2s( nPar)+" Args]";
  if( key.size() > 0)
    {
      if( nPar != 0) s += ",";
      for( SizeT i=0; i<key.size(); ++i)
	{
	  s += key[ i];//+"=val";
	    if( i+1 != key.size()) s += ",";
	}
    }
  s += ")";
  return s;
}

// const string DLibFunDirect::ToString()
// {
//   string s = "res=";
//   s += name+"(";
//   s += "["+i2s( nPar)+" Arg]";
//   s += ")";
//   return s;
// }

DLibPro::DLibPro( LibPro p, const string& n, const string& o, const int nPar_, 
	 const string keyNames[], const string warnKeyNames[], const int nParMin_)
  : DLib(n,o,nPar_,keyNames, warnKeyNames, nParMin_), pro(p)
{
  libProList.push_back(this);
}
DLibPro::DLibPro( LibPro p, const string& n, const int nPar_, 
	 const string keyNames[], const string warnKeyNames[], const int nParMin_)
  : DLib(n,"",nPar_,keyNames, warnKeyNames, nParMin_), pro(p)
{
  libProList.push_back(this);
}

DLibFun::DLibFun( LibFun f, const string& n, const string& o, const int nPar_, 
	 const string keyNames[], const string warnKeyNames[], const int nParMin_)
  : DLib(n,o,nPar_,keyNames, warnKeyNames, nParMin_), fun(f)
{
  libFunList.push_back(this);
}

DLibFun::DLibFun( LibFun f, const string& n, const int nPar_, 
	 const string keyNames[], const string warnKeyNames[], const int nParMin_)
  : DLib(n,"",nPar_,keyNames, warnKeyNames, nParMin_), fun(f)
{
  libFunList.push_back(this);
}
DLibFunRetNew::DLibFunRetNew( LibFun f, const string& n, 
			const string& o, const int nPar_, 
			const string keyNames[], const string warnKeyNames[], const int nParMin_)
  : DLibFun(f,n,o,nPar_,keyNames, warnKeyNames, nParMin_)
{}

DLibFunRetNew::DLibFunRetNew( LibFun f, const string& n, const int nPar_, 
			const string keyNames[], const string warnKeyNames[], bool rConstant,
			const int nParMin_)
  : DLibFun(f,n,nPar_,keyNames, warnKeyNames, nParMin_), retConstant( rConstant)
{}
// DLibFunRetNew::DLibFunRetNew( LibFun f, const string& n, const int nPar_, 
// 			bool rConstant)
//   : DLibFun(f,n,nPar_,NULL, NULL), retConstant( rConstant)
// {}
// DLibFunRetNew::DLibFunRetNew( LibFun f, const string& n, bool rConstant)
//   : DLibFun(f,n), retConstant( rConstant)
// {}
DLibFunDirect::DLibFunDirect( LibFunDirect f, const std::string& n, bool rConstant)
  : DLibFun(NULL,n,1,NULL,NULL,1), funDirect(f), retConstantDirect( rConstant)
{}


// DSubUD ****************************************************
DSubUD::~DSubUD()
{
  // delete only common references (common blocks only if owner)
  CommonBaseListT::iterator it;
  for( it=common.begin(); it !=common.end(); ++it)
    {
      DCommonRef* cRef=dynamic_cast<DCommonRef*>(*it);
      delete cRef; // also ok if cRef is NULL
    }

  labelList.Clear();
  delete tree;
}

DSubUD::DSubUD(const string& n,const string& o,const string& f) : 
  DSub(n,o), file(f),
  tree( NULL),
  labelList(),
  compileOpt(GDLParser::NONE),
  nForLoops( 0)
{
  if( o != "")
    AddPar( "SELF");
}

// for .RNEW and $MAIN$
// clears all COMMON blocks, VARIABLES, JUMPLABELS and the CODE
// name, keywords etc. are kept
void DSubUD::Reset()
{
  var.clear();

  // delete only common references (common blocks only if owner)
  CommonBaseListT::iterator it;
  for( it=common.begin(); it !=common.end(); ++it)
    {
      DCommonRef* cRef=dynamic_cast<DCommonRef*>(*it);
      delete cRef; // also ok if cRef is NULL
    }
  common.clear();

  DelTree();
}

void DSubUD::DelTree()
{
  labelList.Clear(); // labels are invalid after tree is deleted
  delete tree;
  tree = NULL; //static_cast<RefDNode>(antlr::nullAST);
  nForLoops = 0;
}

void DSubUD::AddPar(const string& p)
{
  var.push_back(p);
  nPar++;
}

unsigned DSubUD::AddVar(const string& v)
{
  var.push_back(v);
  return var.size()-1;
}

void DSubUD::AddKey(const string& k, const string& v)
{
  if( k == "_REF_EXTRA")
    {
      if( extra == EXTRA)
	throw GDLException("Routines cannot be declared with both"
			   " _EXTRA and _REF_EXTRA.");
      extra=REFEXTRA;
      extraIx=0;
    }
  else if( k == "_EXTRA")
    {
      if( extra == REFEXTRA)
	throw GDLException("Routines cannot be declared with both"
			   " _EXTRA and _REF_EXTRA.");
      extra=EXTRA;
      extraIx=0;
    }
  else
    {
      if(extraIx != -1) extraIx++; // update extra ix index
    }
  key.push_front(k);
  var.push_front(v);
}

// must be done after the tree is converted
void DSubUD::ResolveAllLabels()
{
  ResolveLabel( tree);
}

void DSubUD::ResolveLabel( ProgNodeP p)
{
  if( p == NULL) return;

// if( p->getNextSibling() != NULL)
// 	std::cout << "Resolve("<< p->getLine()<<"): " << p << " keepRight: " << p->KeepRight()<< ": "<< p->getText() <<"  r: "<< p->GetNextSibling()->getText() << std::endl;
// else
// 	std::cout << "Resolve("<< p->getLine()<<"): " << p << " keepRight: " << p->KeepRight()<< ": "<< p->getText() <<"  r: NULL"<< std::endl;
  
  if( p->getType() == GDLTreeParser::ON_IOERROR || 
      p->getType() == GDLTreeParser::GOTO)
    {
      int ix = labelList.Find( p->getText());
      if( ix == -1)
		throw GDLException( p, ObjectName()+": Undefined label "+p->getText()+
			    " referenced in GOTO statement.",false,false);
      
      p->SetGotoIx( ix);
    }
  else if( p->getType() == GDLTreeParser::LABEL)
    {
      labelList.SetLabelNode( p);
    }
  
  if( !p->KeepDown())
	ResolveLabel( p->getFirstChild());
  if( !p->KeepRight())
	ResolveLabel( p->getNextSibling());
//   else
// 	ResolveLabel( p->getNextSibling());
}

  // **************************************
  // the final "compilation" takes part here 
  // **************************************
void DSubUD::SetTree( RefDNode n)
{
  //
  // Converter Translation Transcription Rewrite RefDNode ProgNode ProgNodeP
  // ***************************************************
  // here the conversion RefDNode -> ProgNode is done
  // ***************************************************
  tree = ProgNode::NewProgNode( n);

//   if( tree != NULL)
    nForLoops = ProgNode::NumberForLoops( tree);
//   else
// 	nForLoops = 0;
  
#ifdef GDL_DEBUG
      cout << "DSubUD::SetTree:" << endl;
      antlr::print_tree pt;
      pt.pr_tree(tree);
      cout << "DSubUD::SetTree end." << endl;
#endif

}

bool DSubUD::isObsolete()
{
  return compileOpt & GDLParser::OBSOLETE;
}

bool DSubUD::isHidden()
{
  return compileOpt & GDLParser::HIDDEN;
}

