/***************************************************************************
                          dstructdesc.hpp  -  GDL struct descriptor
                             -------------------
    begin                : July 22 2002
    copyright            : (C) 2002 by Marc Schellens
    email                : m_schellens@hotmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef DSTRUCTDESC_HPP_
#define DSTRUCTDESC_HPP_

#include <deque>
#include <string>
#include <functional>

#include "basegdl.hpp"
#include "dpro.hpp"
#include "typedefs.hpp"

class DStructBase
{
protected:
  std::deque<BaseGDL*> tags; // Data_<Sp> for data, 'Sp' for structList elements

public:
  DStructBase()
  {}
  
  virtual ~DStructBase();

  BaseGDL*& operator[] (const SizeT d1)
  { return tags[d1];}

  void Add( BaseGDL* t)   { tags.push_back(t);} // grabs
  void Remove( SizeT ix)  { tags.erase( tags.begin() + ix);}
  SizeT NTags() const     { return tags.size();}
};


class DStructDesc;
typedef std::deque<DStructDesc*> StructListT;

// descriptor of structs layout ************************************************
// unnamed struct
class DUStructDesc: public DStructBase
{
private:
  std::deque<std::string>  tNames;  // tag names
  
public:
  DUStructDesc(): DStructBase()
  {}

  //  ~DUStructDesc();

  void AddTag( const std::string& tagName, const BaseGDL* data);
  
  int TagIndex( const std::string& tN) const
  {
    for( SizeT i=0; i< tNames.size(); i++)
      if( tNames[i] == tN) return static_cast<int>(i);
    return -1;
  }
  const std::string& TagName( const SizeT ix) const
  {
    return tNames[ ix];
  }
};

// named struct
class DStructDesc: public DUStructDesc
{
private:
  std::string              name;
  StructListT              parent;  // parent classes 
  IDList                   noDirectMembers;
  
  FunListT                 fun; // member functions
  ProListT                 pro; // member procedures

public:
  DStructDesc( const std::string n): DUStructDesc()
  {
    name=n;
  }

  ~DStructDesc();

  friend bool operator==(const DStructDesc& left, const DStructDesc& right);
  friend bool operator!=(const DStructDesc& left, const DStructDesc& right);

  const std::string& Name() const { return name;}

  FunListT& FunList()
  {
    return fun;
  }

  DFun* FindInFunList( const std::string& n)
  {
  FunListT::iterator p=std::find_if(fun.begin(),fun.end(),Is_eq<DFun>(n));
  if( p != fun.end()) return *p;
  return NULL;
  }
  
  ProListT& ProList()
  {
    return pro;
  }

  DPro* FindInProList( const std::string& n)
  {
    ProListT::iterator p=std::find_if(pro.begin(),pro.end(),Is_eq<DPro>(n));
    if( p != pro.end()) return *p;
    return NULL;
  }

  DPro* GetPro( const std::string& pName);
  DPro* GetPro( const std::string& pName, const std::string& parentName);
  DFun* GetFun( const std::string& pName);
  DFun* GetFun( const std::string& pName, const std::string& parentName);

  bool IsParent( const std::string& p)
  {
    if( p == name) return true;

    SizeT nParents=parent.size();
    for( SizeT i=0; i<nParents; i++)
      {
	if( parent[i]->IsParent( p)) return true;
      }
    return false;
  }

  void AddParent( DStructDesc*);

  void GetParentNames( std::deque< std::string>& pNames) const
  {
    SizeT nParents=parent.size();
    for( SizeT i=0; i<nParents; ++i)
      {
	pNames.push_back( parent[i]->Name());
      }
  }

  void AssureEqual( DStructDesc* d);
  DStructDesc* FindEqual( const StructListT& sL);
};


class DStruct_eq: public std::unary_function<DStructDesc,bool>
{
  std::string name;

public:
  explicit DStruct_eq(const std::string& s): name(s) {}
  bool operator() (const DStructDesc* v) const { return v->Name() == name;}
};

DStructDesc* FindInStructList(StructListT, const std::string& name);

// actually holding the data **********************************************
// class DStructGDL see dstructgdl.hpp

#endif
