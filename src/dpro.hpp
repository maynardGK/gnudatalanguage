/***************************************************************************
                          dpro.hpp  -  GDL procedure/function
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

#ifndef DPRO_HPP_
#define DPRO_HPP_

#include <deque>
#include <string>
#include <algorithm>
#include <vector>
//#include <stack>

#include "basegdl.hpp"
#include "dcommon.hpp"
#include "dvar.hpp"
#include "dnode.hpp"
#include "str.hpp"

#include "antlr/Token.hpp"

template<typename T>  class Is_eq: public std::unary_function<T,bool>
{
  std::string name;
public:
  explicit Is_eq(const std::string& n): name(n) {}
  bool operator() (const T* p) const 
  { return (p->Name() == name);}
};

// for jumps (goto, on_ioerror)
struct LabelT
{
  std::string label;
  DNode* target;

  LabelT( const std::string& l, DNode* t): label( l), target( t) {}
};

class LabelListT
{
  std::vector<LabelT> list;

public:
  LabelListT(): list() {}
  ~LabelListT() {}

  void Add( const std::string& l, DNode* t)
  {
    list.push_back( LabelT( l, t));
  }
  SizeT Size() { return list.size();}
  int Find( const std::string& s)
  {
    SizeT nEl = list.size();
    for( SizeT i=0; i<nEl; i++)
      if( list[ i].label == s) return static_cast<int>(i);
    return -1;
  }
  DNode* Get( SizeT ix)
  {
    return list[ ix].target;
  }
  const std::string& GetLabel( SizeT ix)
  {
    return list[ ix].label;
  }
  void Clear()
  {
    list.clear();
  }
};

// Fun/Pro classes *****************************************************
// Base class **********************************************************
class DSub
{
public:
  enum ExtraType {
    NONE=0,
    EXTRA,
    REFEXTRA
  };

protected:
  std::string name;         // name (of procedure/function)
  std::string object;       // name of class for methods (might not be defined
                            // when method is compiled)
  
  // keywords are pushed in front so they are first
  // followed by the parameters 
  // (which are pushed back,
  // but due to the syntax are first,
  // so the layout is:
  // keyVar_N (ID in key[0]),..,keyVar_1 (key[n-1]),
  // par_1,..,par_nPar, var1,..,varK
  // N=size(key)
  // K=size(var)-nPar-N
  IDList              key;    // keyword names (IDList: typedefs.hpp)
                              // (KEYWORD_NAME=keyword_value)
  int                 nPar;   // number of parameters (-1 = infinite)

  ExtraType           extra;
  int                 extraIx; // index of extra keyword

  IDList  warnKey;    // keyword names to accept but warn
  // (IDList: typedefs.hpp)

public:
  DSub( const std::string& n, const std::string& o=""): 
    name(n), object(o), nPar(0), extra(NONE), extraIx(-1) 
  {}

  virtual ~DSub(); // polymorphism

  const std::string& Name()   const { return name;}
  const std::string& Object() const { return object;}
  std::string  ObjectName() const
  { 
    if( object == "") return name;
    return object+"::"+name;
  }
  std::string ObjectFileName() const
  { 
    if( object == "") return name;
    return object+"__"+name;
  }

  ExtraType Extra() 
  {
    return extra;
  }

  int ExtraIx() 
  {
    return extraIx;
  }

  // returns the keyword value index 
  int FindKey(const std::string& s)
  {
    String_abbref_eq searchKey(s);
    int ix=0;
    for(IDList::iterator i=key.begin();
	i != key.end(); i++, ix++) if( searchKey(*i)) {
	  return ix;
	}
    return -1;
  }

  //  bool AKey() { return aKey;} // additional keywords allowed

  friend class EnvT;
  friend class ExtraT;
};

// Lib pro/fun ********************************************************
class EnvT;

typedef void     (*LibPro)(EnvT*);
typedef BaseGDL* (*LibFun)(EnvT*);

// library procedure/function (in cases both are handled the same way)
class DLib: public DSub
{
public:
  DLib( const std::string& n, const std::string& o, const int nPar_,
	const std::string keyNames[],
	const std::string warnKeyNames[]);

  virtual const std::string ToString() = 0;
};

// library procedure
class DLibPro: public DLib
{
  LibPro pro;
public:
  // warnKeyNames are keywords wich are not supported, but which
  // should not make the compilation fail.
  // A warning will be issued.
  // use this for keywords which are truly optional and don't
  // change the results.
  // Note that due to their nature, there should never be keywords
  // on which a value is returned.
  DLibPro( LibPro p, const std::string& n, const int nPar_=0, 
	   const std::string keyNames[]=NULL,
	   const std::string warnKeyNames[]=NULL);

  DLibPro( LibPro p, const std::string& n, const std::string& o, const int nPar_=0, 
	   const std::string keyNames[]=NULL,
	   const std::string warnKeyNames[]=NULL);

  LibPro Pro() { return pro;}

  const std::string ToString();
};

// library function
class DLibFun: public DLib
{
  LibFun fun;
public:
  DLibFun( LibFun f, const std::string& n, const int nPar_=0, 
	   const std::string keyNames[]=NULL,
	   const std::string warnKeyNames[]=NULL);

  DLibFun( LibFun f, const std::string& n, const std::string& o, const int nPar_=0, 
	   const std::string keyNames[]=NULL,
	   const std::string warnKeyNames[]=NULL);

  LibFun Fun() { return fun;}

  const std::string ToString();
};

// UD pro/fun ********************************************************
// function/procedure (differ because they are in different lists)
// User Defined
class DSubUD: public DSub 
{
  std::string file;        // filename were procedure is defined in

  IDList              var;    // keyword values, parameters, local variables

  CommonBaseListT     common; // common blocks or references 
  RefDNode            tree;   // the 'code'

  LabelListT          labelList;

public:
  DSubUD(const std::string&,const std::string& o="",const std::string& f="");
  ~DSubUD(); 

  void AddCommon(DCommonBase* c) { common.push_back(c);}
  
  LabelListT& LabelList() { return labelList;}
  DNode* GotoTarget( int ix) 
  { 
    if( ix >= labelList.Size())
      throw GDLException( "Undefined label.");
    return labelList.Get( ix);
  } 
//   int LabelOrd( int ix) { return labelList.GetOrd( ix);} 
  int NDefLabel() { return labelList.Size();} 

  std::string GetFilename()
  {
    return file;
  }

  // add variables
  void     AddPar(const std::string&); // add paramter
  unsigned AddVar(const std::string&); // add local variable
  void     AddKey(const std::string&, const std::string&); // add keyword=value
  
  // search for variable returns true if its found in var or common blocks
  bool Find(const std::string& n)
  {
    IDList::iterator f=std::find(var.begin(),var.end(),n);
    if( f != var.end()) return true;

    CommonBaseListT::iterator c=
      std::find_if(common.begin(),common.end(),DCommon_contains_var(n));

    return (c != common.end());
  }

  // returns common block with name n
  DCommon* Common(const std::string& n)
  {
    CommonBaseListT::iterator c = common.begin();
    for(; c != common.end(); ++c)
      if( dynamic_cast< DCommon*>( *c) != NULL && (*c)->Name() == n)
	return static_cast< DCommon*>( *c);
    return NULL;
  }

  // returns common block which holds variable n
  DCommonBase* FindCommon(const std::string& n)
  {
    CommonBaseListT::iterator c=
      std::find_if(common.begin(),common.end(),DCommon_contains_var(n));

    return (c != common.end())? *c : NULL;
  }

  const std::string& GetVarName( SizeT ix)
  {
    return var[ix];
  }
  
  bool GetCommonVarName(const BaseGDL* p, std::string& varName)
  {
    for( CommonBaseListT::iterator c=common.begin();
	 c != common.end(); c++)
      {
	int vIx = (*c)->Find( p);
	if( vIx >= 0) 
	  {
	    varName=(*c)->VarName( vIx);
	    return true;
	  }
      }
    return false;
  }
  BaseGDL** GetCommonVarPtr(const BaseGDL* p)
  {
    for( CommonBaseListT::iterator c=common.begin();
	 c != common.end(); c++)
      {
	int vIx = (*c)->Find( p);
	if( vIx >= 0) 
	  {
	    DVar* var = (*c)->Var( vIx);
	    return &(var->Data());
	  }
      }
    return NULL;
  }

  // returns the variable index (-1 if not found)
  int FindVar(const std::string& s)
  {
    return FindInIDList(var,s);
  }

  // returns ptr to common variable (NULL if not found)
  DVar* FindCommonVar(const std::string& s) 
  { 
    for(CommonBaseListT::iterator c=common.begin();
   	c != common.end(); c++)
      {
       	DVar* v=(*c)->Find(s);
       	if( v) return v;
      }
    return NULL;
  }

  void SetTree(RefDNode& n)
  {
    tree=n; // copied here (operator=)
  }

  RefDNode& GetTree()
  {
    return tree;
  }

  friend class EnvT;
};

class DPro: public DSubUD
{
public:
  // for main function, not inserted into proList
  DPro(): DSubUD("$MAIN$","","") {}

  DPro(const std::string& n,const std::string& o="",const std::string& f=""): 
    DSubUD(n,o,f)
  {}
  
  ~DPro() {}; 
};

class DFun: public DSubUD
{
public:
  DFun(const std::string& n,const std::string& o="",const std::string& f=""): 
    DSubUD(n,o,f)
  {} 
  
  ~DFun() {};
};


typedef std::deque<DFun*> FunListT;
typedef std::deque<DPro*> ProListT;

typedef std::deque<DLibFun*> LibFunListT;
typedef std::deque<DLibPro*> LibProListT;

#endif
