/***************************************************************************
                       overload.cpp  -  GDL operator overloading for objects
                             -------------------
    begin                : November 29 2012
    copyright            : (C) 2012 by Marc Schellens
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

#include "objects.hpp"
#include "overload.hpp"
#include "prognodeexpr.hpp"
#include "dinterpreter.hpp"
// #include "basic_pro.hpp"
#include "nullgdl.hpp"
#include "list.hpp"

using namespace std;

std::string overloadOperatorNames[] = 
{
  "_OVERLOADBRACKETSLEFTSIDE"
, "_OVERLOADBRACKETSRIGHTSIDE" 
, "_OVERLOADMINUSUNARY"
, "_OVERLOADNOT"
, "_OVERLOADTILDE" 
, "_OVERLOADPLUS"
, "_OVERLOADMINUS" 
, "_OVERLOADASTERISK" 
, "_OVERLOADSLASH" 
, "_OVERLOADCARET" 
, "_OVERLOADMOD" 
, "_OVERLOADLESSTHAN" 
, "_OVERLOADGREATERTHAN" 
, "_OVERLOADAND"
, "_OVERLOADOR"
, "_OVERLOADXOR" 
, "_OVERLOADEQ"
, "_OVERLOADNE"
, "_OVERLOADGE"
, "_OVERLOADGT"
, "_OVERLOADLE"
, "_OVERLOADLT"
, "_OVERLOADPOUND" 
, "_OVERLOADPOUNDPOUND" 
, "_OVERLOADISTRUE"
, "_OVERLOADFOREACH"
, "_OVERLOADHELP"
, "_OVERLOADPRINT"
, "_OVERLOADSIZE"
};

// except _OVERLOADBRACKETSLEFTSIDE all are functions
int OverloadOperatorIndexFun( std::string subName)
{
  assert( !subName.empty());
  if( subName[0] != '_') // optimization, true in most cases
    return -1;
  for( int i=1; i < NumberOfOverloadOperators; ++i)
    if( subName == overloadOperatorNames[ i])
      return i;
  return -1;
}
int OverloadOperatorIndexPro( std::string subName)
{
  if( subName == overloadOperatorNames[ 0])
    return 0;
  return -1;
}

// for proper error repoerting we need to unwind the stack when throwing from internal subroutines
void ThrowFromInternalUDSub( EnvUDT* e, const string& s)
  {
    ProgNodeP callingNode = e->CallingNode();
    string objectName =e->GetPro()->ObjectName();
    delete e;
    GDLException::Interpreter()->CallStack().pop_back();
    throw GDLException( callingNode, objectName+" (internal): " + s, false, false);
  }

// #include <iostream>
BaseGDL* _GDL_OBJECT_OverloadIsTrue( EnvUDT* e)
{
//   // debug/check
//   std::cout << "_GDL_OBJECT_OverloadIsTrue called" << std::endl;

// default behavior: Implict: Another object cannot be the null object
  return new DIntGDL(1); // if we reach here, defaul is to return 'TRUE'
}

void _GDL_OBJECT_OverloadBracketsLeftSide( EnvUDT* e)
{
  //   // debug/check
  //   std::cout << "_GDL_OBJECT_OverloadBracketsLeftSide called" << std::endl;

  // this is only called on scalar object references
  // IDL's default behavior is to just replace SELF (via OBJREF) by RVALUE
  // no index checking is done.
  SizeT nParam = e->NParam();
  if( nParam < 3) // consider implicit SELF
    return; // RVALUE not given -> ignore

  BaseGDL* objRef = e->GetKW(1);
  BaseGDL** objRefP = e->GetPtrTo( objRef);
  if( objRefP == NULL)
  {
    ThrowFromInternalUDSub( e, "Parameter 1 (OBJREF) must be a passed as reference in this context.");
  }
  BaseGDL* rValue = e->GetKW(2);
  if( rValue == NULL)
  {
    ThrowFromInternalUDSub( e, "Parameter 2 (RVALUE) is undefined.");
  }
  if( rValue->Type() != GDL_OBJ)
  {
    ThrowFromInternalUDSub( e, "Parameter 2 (RVALUE) must be an OBJECT in this context.");
  }
  
  GDLDelete( *objRefP);
  *objRefP = rValue->Dup();
}

BaseGDL* LIST_OverloadBracketsRightSide( EnvUDT* e)
{
  SizeT nParam = e->NParam(); // number of parameters actually given
//   int envSize = e->EnvSize(); // number of parameters + keywords 'e' (pro) has defined
  if( nParam < 3) // consider implicit SELF
    ThrowFromInternalUDSub( e, "Two parameters are needed: ISRANGE, SUB1 [, ...].");
  if( nParam > 3) // consider implicit SELF
    ThrowFromInternalUDSub( e, "Only one dimensional access allowed.");

  BaseGDL* selfP = e->GetKW( 0);
  if( selfP->Type() != GDL_OBJ)
    ThrowFromInternalUDSub( e, "SELF is not of type OBJECT.");
  if( !selfP->Scalar())
    ThrowFromInternalUDSub( e, "SELF must be a scalar OBJECT.");

  DObjGDL* selfObj = static_cast<DObjGDL*>( selfP);
  DObj selfID = (*selfObj)[0];
  DStructGDL* self = e->Interpreter()->GetObjHeap( selfID);
  

  static DString cNodeName("GDL_CONTAINER_NODE");
   
  DStructDesc* listDesc= self->Desc();

  // because of .RESET_SESSION, we cannot use static here
  DStructDesc* containerDesc=FindInStructList( structList, cNodeName);

  // here static is fine
  static unsigned pHeadTag = listDesc->TagIndex( "PHEAD");
  static unsigned pTailTag = listDesc->TagIndex( "PTAIL");
  static unsigned nListTag = listDesc->TagIndex( "NLIST");
  static unsigned pNextTag = containerDesc->TagIndex( "PNEXT");
  static unsigned pDataTag = containerDesc->TagIndex( "PDATA");

  // default behavior: Exact like scalar indexing
  BaseGDL* isRange = e->GetKW(1);
  if( isRange == NULL)
    ThrowFromInternalUDSub( e, "Parameter 1 (ISRANGE) is undefined.");
//   if( isRange->Rank() == 0)
//     ThrowFromInternalUDSub( e, "Parameter 1 (ISRANGE) must be an array in this context: " + e->Caller()->GetString(e->GetKW(1)));
  SizeT nIsRange = isRange->N_Elements();
  if( nIsRange > (nParam - 2)) //- SELF and ISRANGE
    ThrowFromInternalUDSub( e, "Parameter 1 (ISRANGE) must have "+i2s(nParam-2)+" elements.");
  Guard<DLongGDL> isRangeLongGuard;
  DLongGDL* isRangeLong;
  if( isRange->Type() == GDL_LONG)
    isRangeLong = static_cast<DLongGDL*>( isRange);
  else
  {
    try{
      isRangeLong = static_cast<DLongGDL*>( isRange->Convert2( GDL_LONG, BaseGDL::COPY));
    }
    catch( GDLException& ex)
    {
      ThrowFromInternalUDSub( e, ex.ANTLRException::getMessage());
    }
    isRangeLongGuard.Reset( isRangeLong);
  }
  
  ArrayIndexVectorT ixList;
//   IxExprListT exprList;
  try {
    for( int p=0; p<nIsRange; ++p)
    {
      BaseGDL* parX = e->GetKW( p + 2); // implicit SELF, ISRANGE, par1..par8
      if( parX == NULL)
	ThrowFromInternalUDSub( e, "Parameter is undefined: "  + e->Caller()->GetString(e->GetKW( p + 2)));

      DLong isRangeX = (*isRangeLong)[p];
      if( isRangeX != 0 && isRangeX != 1)
      {
	ThrowFromInternalUDSub( e, "Value of parameter 1 (ISRANGE["+i2s(p)+"]) is out of allowed range.");
      }
      if( isRangeX == 1)
      {
	if( parX->N_Elements() != 3)
	{
	  ThrowFromInternalUDSub( e, "Range vector must have 3 elements: " + e->Caller()->GetString(e->GetKW( p + 2)));
	}
	DLongGDL* parXLong;
	Guard<DLongGDL> parXLongGuard;
	if( parX->Type() != GDL_LONG)
	{
	  try{
	    parXLong = static_cast<DLongGDL*>( parX->Convert2( GDL_LONG, BaseGDL::COPY));
	    parXLongGuard.Reset( parXLong);
	  }
	  catch( GDLException& ex)
	  {
	    ThrowFromInternalUDSub( e, ex.ANTLRException::getMessage());
	  }
	}
	else
	{
	  parXLong = static_cast<DLongGDL*>( parX);
	}
	// negative end ix is fine -> CArrayIndexRangeS can handle [b:*:s] ([b,-1,s])
	ixList.push_back(new CArrayIndexRangeS( (*parXLong)[0], (*parXLong)[1], (*parXLong)[2]));
      }
      else // non-range
      {
	// ATTENTION: These two grab c1 (all others don't)
	// a bit unclean, but for maximum efficiency
	if( parX->Rank() == 0)
	  ixList.push_back( new CArrayIndexScalar( parX->Dup()));
	else
	  ixList.push_back( new CArrayIndexIndexed( parX->Dup()));
      }
    } // for
  }
  catch( GDLException& ex)
  {
    ixList.Destruct(); // ixList is not valid afterwards, but as we throw this is ok
    throw ex;
  }
  
  SizeT listSize = (*static_cast<DLongGDL*>(self->GetTag( nListTag, 0)))[0];
  
  ArrayIndexListT* aL;
  MakeArrayIndex( &ixList, &aL, NULL); // important to get the non-NoAssoc ArrayIndexListT
  // because only they clean up ixList on destruction
  Guard< ArrayIndexListT> aLGuard( aL);

  SpDLong t = SpDLong( dimension(listSize));
  aL->SetVariable( &t);
          
  AllIxBaseT* allIx = aL->BuildIx();
  
  if( allIx->size() == 1)
  {
    DPtr actP = (*static_cast<DPtrGDL*>(self->GetTag( pTailTag, 0)))[0];
    SizeT targetIx = allIx->operator[](0);
// already checked (IndexListT::SetVariable)
//     if( targetIx < 0)
//     {
//       targetIx += listSize;
//       if( targetIx < 0)
//       {
// 	ThrowFromInternalUDSub( e, "Position value too small ("+i2s(targetIx)+").");	
//       }
//     }
//     if( targetIx >= listSize)
//     {
//       ThrowFromInternalUDSub( e, "Position value too large ("+i2s(targetIx)+").");	
//     }
    for( SizeT elIx = 0; elIx < targetIx; ++elIx)
    {
      BaseGDL* actPHeap = e->Interpreter()->GetHeap( actP);
      if( actPHeap->Type() != GDL_STRUCT)
	ThrowFromInternalUDSub( e, "LIST node must be a STRUCT.");	
      DStructGDL* actPStruct = static_cast<DStructGDL*>( actPHeap);
      if( actPStruct->Desc() != containerDesc)
	ThrowFromInternalUDSub( e, "LIST node must be a GDL_CONTAINER_NODE STRUCT.");	

      actP = (*static_cast<DPtrGDL*>( actPStruct->GetTag( pNextTag, 0)))[0];
    }

    BaseGDL* actPHeap = e->Interpreter()->GetHeap( actP);
    if( actPHeap->Type() != GDL_STRUCT)
      ThrowFromInternalUDSub( e, "LIST node must be a STRUCT.");	
    DStructGDL* actPStruct = static_cast<DStructGDL*>( actPHeap);
    if( actPStruct->Desc() != containerDesc)
      ThrowFromInternalUDSub( e, "LIST node must be a GDL_CONTAINER_NODE STRUCT.");	

    actP = (*static_cast<DPtrGDL*>(actPStruct->GetTag( pDataTag, 0)))[0];
    
    BaseGDL* res = e->Interpreter()->GetHeap( actP);
    if( res == NULL)
      return NullGDL::GetSingleInstance();
    return res->Dup();
  }

  DStructGDL* listStruct= new DStructGDL( listDesc, dimension());
  DObj objID= e->NewObjHeap( 1, listStruct); // owns objStruct
  BaseGDL* newObj = new DObjGDL( objID); // the list object
  Guard<BaseGDL> newObjGuard( newObj);
  // we need ref counting here as the LIST (newObj) is a regular return value
//   e->Interpreter()->IncRefObj( objID);

  DStructGDL* cStructLast = NULL;
  DStructGDL* cStruct = NULL;
  DPtr cID = 0;
  for( SizeT i=0; i<allIx->size(); ++i)
  {
    SizeT actIx = allIx->operator[](i);
    DPtr pActNode = GetLISTNode( e, self, actIx);
    DStructGDL* actNode = GetLISTStruct( e, pActNode);   

    DPtr pData = (*static_cast<DPtrGDL*>(actNode->GetTag( pDataTag, 0)))[0];
    BaseGDL* data = BaseGDL::interpreter->GetHeap( pData);
    if( data != NULL) 
      data = data->Dup();
    DPtr dID = e->Interpreter()->NewHeap(1,data);
    
    cStruct = new DStructGDL( containerDesc, dimension());
    cID = e->Interpreter()->NewHeap(1,cStruct);
    (*static_cast<DPtrGDL*>( cStruct->GetTag( pDataTag, 0)))[0] = dID;
    
    if( cStructLast != NULL)
      (*static_cast<DPtrGDL*>( cStructLast->GetTag( pNextTag, 0)))[0] = cID;
    else
    { // 1st element
      (*static_cast<DPtrGDL*>( listStruct->GetTag( pTailTag, 0)))[0] = cID;	      
    }
          
    cStructLast = cStruct;
  }
  
  (*static_cast<DPtrGDL*>( listStruct->GetTag( pHeadTag, 0)))[0] = cID;	      
  (*static_cast<DLongGDL*>( listStruct->GetTag( nListTag, 0)))[0] = listSize + allIx->size();      

  newObjGuard.Release();
  return newObj;
}

BaseGDL* _GDL_OBJECT_OverloadBracketsRightSide( EnvUDT* e)
{
//   // debug/check
//   std::cout << "_GDL_OBJECT_OverloadBracketsRightSide called" << std::endl;

  SizeT nParam = e->NParam(); // number of parameters actually given
//   int envSize = e->EnvSize(); // number of parameters + keywords 'e' (pro) has defined
  if( nParam < 3) // consider implicit SELF
    ThrowFromInternalUDSub( e, "At least 2 parameters are needed: ISRANGE, SUB1 [, ...].");

  // default behavior: Exact like scalar indexing
  BaseGDL* isRange = e->GetKW(1);
  if( isRange == NULL)
    ThrowFromInternalUDSub( e, "Parameter 1 (ISRANGE) is undefined.");
  if( isRange->Rank() == 0)
    ThrowFromInternalUDSub( e, "Parameter 1 (ISRANGE) must be an array in this context: " + e->Caller()->GetString(e->GetKW(1)));
  SizeT nIsRange = isRange->N_Elements();
  if( nIsRange > (nParam - 2)) //- SELF and ISRANGE
    ThrowFromInternalUDSub( e, "Parameter 1 (ISRANGE) must have "+i2s(nParam-2)+" elements.");
  Guard<DLongGDL> isRangeLongGuard;
  DLongGDL* isRangeLong;
  if( isRange->Type() == GDL_LONG)
    isRangeLong = static_cast<DLongGDL*>( isRange);
  else
  {
    try{
      isRangeLong = static_cast<DLongGDL*>( isRange->Convert2( GDL_LONG, BaseGDL::COPY));
    }
    catch( GDLException& ex)
    {
      ThrowFromInternalUDSub( e, ex.ANTLRException::getMessage());
    }
    isRangeLongGuard.Reset( isRangeLong);
  }

  ArrayIndexVectorT ixList;
//   IxExprListT exprList;
  try {
    for( int p=0; p<nIsRange; ++p)
    {
      BaseGDL* parX = e->GetKW( p + 2); // implicit SELF, ISRANGE, par1..par8
      if( parX == NULL)
	ThrowFromInternalUDSub( e, "Parameter is undefined: "  + e->Caller()->GetString(e->GetKW( p + 2)));

      DLong isRangeX = (*isRangeLong)[p];
      if( isRangeX != 0 && isRangeX != 1)
      {
	ThrowFromInternalUDSub( e, "Value of parameter 1 (ISRANGE["+i2s(p)+"]) is out of allowed range.");
      }
      if( isRangeX == 1)
      {
	if( parX->N_Elements() != 3)
	{
	  ThrowFromInternalUDSub( e, "Range vector must have 3 elements: " + e->Caller()->GetString(e->GetKW( p + 2)));
	}
	DLongGDL* parXLong;
	Guard<DLongGDL> parXLongGuard;
	if( parX->Type() != GDL_LONG)
	{
	  try{
	    parXLong = static_cast<DLongGDL*>( parX->Convert2( GDL_LONG, BaseGDL::COPY));
	    parXLongGuard.Reset( parXLong);
	  }
	  catch( GDLException& ex)
	  {
	    ThrowFromInternalUDSub( e, ex.ANTLRException::getMessage());
	  }
	}
	else
	{
	  parXLong = static_cast<DLongGDL*>( parX);
	}
	// negative end ix is fine -> CArrayIndexRangeS can handle [b:*:s] ([b,-1,s])
	ixList.push_back(new CArrayIndexRangeS( (*parXLong)[0], (*parXLong)[1], (*parXLong)[2]));
      }
      else // non-range
      {
	// ATTENTION: These two grab c1 (all others don't)
	// a bit unclean, but for maximum efficiency
	if( parX->Rank() == 0)
	  ixList.push_back( new CArrayIndexScalar( parX->Dup()));
	else
	  ixList.push_back( new CArrayIndexIndexed( parX->Dup()));
      }
    } // for
  }
  catch( GDLException& ex)
  {
    ixList.Destruct(); // ixList is not valid afterwards, but as we throw this is ok
    throw ex;
  }
  
  ArrayIndexListT* aL;
  MakeArrayIndex( &ixList, &aL, NULL); // important to get the non-NoAssoc ArrayIndexListT
  // because only they clean up ixList on destruction
  Guard< ArrayIndexListT> aLGuard( aL);

  IxExprListT ixL;
  return aL->Index( e->GetKW( 0), ixL); // index SELF
}

BaseGDL* _GDL_OBJECT_OverloadEQOp( EnvUDT* e)
{
  SizeT nParam = e->NParam(); // number of parameters actually given
//   int envSize = e->EnvSize(); // number of parameters + keywords 'e' (pro) has defined
  if( nParam < 2) // consider implicit SELF
    ThrowFromInternalUDSub( e, "2 parameters are needed: LEFT, RIGHT.");

  // default behavior: Exact like scalar indexing
  BaseGDL* l = e->GetKW(1);
  if( l->Type() != GDL_OBJ)
    ThrowFromInternalUDSub( e, "Unable to convert parameter #1 to type object reference.");

  BaseGDL* r = e->GetKW(2);
  if( r->Type() != GDL_OBJ)
    ThrowFromInternalUDSub( e, "Unable to convert parameter #2 to type object reference.");
  
  DObjGDL* left = static_cast<DObjGDL*>(left);
  DObjGDL* right = static_cast<DObjGDL*>(right);
  
  ULong rEl=right->N_Elements();
  ULong nEl=left->N_Elements();
  //   if( nEl == 0)
  // 	 nEl=N_Elements();
  assert( rEl);
  assert( nEl);
  //  if( !rEl || !nEl) throw GDLException("Variable is undefined.");  

  Data_<SpDByte>* res;

  DObj s;
  if( right->StrictScalar(s)) 
    {
      res= new Data_<SpDByte>( left->Dim(), BaseGDL::NOZERO);
      if( nEl == 1)
	{
	  (*res)[0] = (s == (*left)[0]);
	  return res;
	}
      TRACEOMP( __FILE__, __LINE__)
#pragma omp parallel if (nEl >= CpuTPOOL_MIN_ELTS && (CpuTPOOL_MAX_ELTS == 0 || CpuTPOOL_MAX_ELTS <= nEl))
	{
#pragma omp for
	  for( OMPInt i=0; i < nEl; ++i)
	    (*res)[i] = ((*left)[i] == s);
	}    }
  else if( left->StrictScalar(s)) 
    {
      res= new Data_<SpDByte>( right->Dim(), BaseGDL::NOZERO);
      if( rEl == 1)
	{
	  (*res)[0] = ((*right)[0] == s);
	  return res;
	}
      TRACEOMP( __FILE__, __LINE__)
#pragma omp parallel if (rEl >= CpuTPOOL_MIN_ELTS && (CpuTPOOL_MAX_ELTS == 0 || CpuTPOOL_MAX_ELTS <= rEl))
	{
#pragma omp for
	  for( OMPInt i=0; i < rEl; ++i)
	    (*res)[i] = ((*right)[i] == s);
	}    }
  else if( rEl < nEl) 
    {
      res= new Data_<SpDByte>( right->Dim(), BaseGDL::NOZERO);
      TRACEOMP( __FILE__, __LINE__)
#pragma omp parallel if (rEl >= CpuTPOOL_MIN_ELTS && (CpuTPOOL_MAX_ELTS == 0 || CpuTPOOL_MAX_ELTS <= rEl))
	{
#pragma omp for
	  for( OMPInt i=0; i < rEl; ++i)
	    (*res)[i] = ((*right)[i] == (*left)[i]);
	}    }
  else // ( rEl >= nEl)
    {
      res= new Data_<SpDByte>( left->Dim(), BaseGDL::NOZERO);
      if( rEl == 1)
	{
	  (*res)[0] = ((*right)[0] == (*left)[0]);
	  return res;
	}
      TRACEOMP( __FILE__, __LINE__)
#pragma omp parallel if (nEl >= CpuTPOOL_MIN_ELTS && (CpuTPOOL_MAX_ELTS == 0 || CpuTPOOL_MAX_ELTS <= nEl))
	{
#pragma omp for
	  for( OMPInt i=0; i < nEl; ++i)
	    (*res)[i] = ((*right)[i] == (*left)[i]);
	}    }
  return res;
}

BaseGDL* _GDL_OBJECT_OverloadNEOp( EnvUDT* e)
{
  SizeT nParam = e->NParam(); // number of parameters actually given
//   int envSize = e->EnvSize(); // number of parameters + keywords 'e' (pro) has defined
  if( nParam < 2) // consider implicit SELF
    ThrowFromInternalUDSub( e, "Two parameters are needed: LEFT, RIGHT.");

  // default behavior: Exact like scalar indexing
  BaseGDL* l = e->GetKW(1);
  if( l->Type() != GDL_OBJ)
    ThrowFromInternalUDSub( e, "Unable to convert parameter #1 to type object reference.");

  BaseGDL* r = e->GetKW(2);
  if( r->Type() != GDL_OBJ)
    ThrowFromInternalUDSub( e, "Unable to convert parameter #2 to type object reference.");
  
  DObjGDL* left = static_cast<DObjGDL*>(left);
  DObjGDL* right = static_cast<DObjGDL*>(right);
  
  ULong rEl=right->N_Elements();
  ULong nEl=left->N_Elements();
  //   if( nEl == 0)
  // 	 nEl=N_Elements();
  assert( rEl);
  assert( nEl);
  //  if( !rEl || !nEl) throw GDLException("Variable is undefined.");  

  Data_<SpDByte>* res;

  DObj s;
  if( right->StrictScalar(s)) 
    {
      res= new Data_<SpDByte>( left->Dim(), BaseGDL::NOZERO);
      if( nEl == 1)
	{
	  (*res)[0] = (s != (*left)[0]);
	  return res;
	}
      TRACEOMP( __FILE__, __LINE__)
#pragma omp parallel if (nEl >= CpuTPOOL_MIN_ELTS && (CpuTPOOL_MAX_ELTS == 0 || CpuTPOOL_MAX_ELTS <= nEl))
	{
#pragma omp for
	  for( OMPInt i=0; i < nEl; ++i)
	    (*res)[i] = ((*left)[i] != s);
	}    }
  else if( left->StrictScalar(s)) 
    {
      res= new Data_<SpDByte>( right->Dim(), BaseGDL::NOZERO);
      if( rEl == 1)
	{
	  (*res)[0] = ((*right)[0] != s);
	  return res;
	}
      TRACEOMP( __FILE__, __LINE__)
#pragma omp parallel if (rEl >= CpuTPOOL_MIN_ELTS && (CpuTPOOL_MAX_ELTS == 0 || CpuTPOOL_MAX_ELTS <= rEl))
	{
#pragma omp for
	  for( OMPInt i=0; i < rEl; ++i)
	    (*res)[i] = ((*right)[i] != s);
	}    }
  else if( rEl < nEl) 
    {
      res= new Data_<SpDByte>( right->Dim(), BaseGDL::NOZERO);
      TRACEOMP( __FILE__, __LINE__)
#pragma omp parallel if (rEl >= CpuTPOOL_MIN_ELTS && (CpuTPOOL_MAX_ELTS == 0 || CpuTPOOL_MAX_ELTS <= rEl))
	{
#pragma omp for
	  for( OMPInt i=0; i < rEl; ++i)
	    (*res)[i] = ((*right)[i] != (*left)[i]);
	}    }
  else // ( rEl >= nEl)
    {
      res= new Data_<SpDByte>( left->Dim(), BaseGDL::NOZERO);
      if( rEl == 1)
	{
	  (*res)[0] = ((*right)[0] != (*left)[0]);
	  return res;
	}
      TRACEOMP( __FILE__, __LINE__)
#pragma omp parallel if (nEl >= CpuTPOOL_MIN_ELTS && (CpuTPOOL_MAX_ELTS == 0 || CpuTPOOL_MAX_ELTS <= nEl))
	{
#pragma omp for
	  for( OMPInt i=0; i < nEl; ++i)
	    (*res)[i] = ((*right)[i] != (*left)[i]);
	}    }
  return res;
}

// for GDL_OBJECT all other overloads are illegal operations as default
// however, they need to be accessible for (nonsense) parent method calls.
// But we can use just this one function for all of them
BaseGDL* _GDL_OBJECT_OverloadReportIllegalOperation( EnvUDT* e)
{
  ThrowFromInternalUDSub( e, "Operation illegal with object reference types.");
  return 0;
}

// set up the _overload... subroutines for GDL_OBJECT
void SetupOverloadSubroutines()
{
//   // The call
//   BaseGDL* res=interpreter->call_fun(static_cast<DSubUD*>(newEnv->GetPro())->GetTree());
//   in call_fun eventually (in GDLInterpreter::statement) tree->Run() is called
  DStructDesc* gdlObjectDesc = FindInStructList(structList, GDL_OBJECT_NAME);
  assert( gdlObjectDesc != NULL);
  DStructDesc* listDesc = FindInStructList(structList, "LIST");
  assert( listDesc != NULL);
  
  WRAPPED_FUNNode *tree;
  
  // automatically adds "SELF" parameter (object name is != "")
  DFun *_overloadIsTrue = new DFun("_OVERLOADISTRUE",GDL_OBJECT_NAME,"*INTERNAL*");
  WRAPPED_FUNNode *tree1 = new WRAPPED_FUNNode(_GDL_OBJECT_OverloadIsTrue);
  _overloadIsTrue->SetTree( tree1);
// we are NOT setting the operator to have (faster) default behavior
// the functions must be there nevertheless for expicit callingNode
// that's why we add them to the functions list  
  gdlObjectDesc->FunList().push_back(_overloadIsTrue);
//   gdlObjectDesc->SetOperator(OOIsTrue,_overloadIsTrue);

  DPro *_overloadBracketsLeftSide = new DPro("_OVERLOADBRACKETSLEFTSIDE",GDL_OBJECT_NAME,"*INTERNAL*");
  _overloadBracketsLeftSide->AddPar("OBJREF")->AddPar("RVALUE")->AddPar("ISRANGE");
  _overloadBracketsLeftSide->AddPar("SUB1")->AddPar("SUB2")->AddPar("SUB3")->AddPar("SUB4");
  _overloadBracketsLeftSide->AddPar("SUB5")->AddPar("SUB6")->AddPar("SUB7")->AddPar("SUB8");
  WRAPPED_PRONode *tree2 = new WRAPPED_PRONode(_GDL_OBJECT_OverloadBracketsLeftSide);
  _overloadBracketsLeftSide->SetTree( tree2); 
  gdlObjectDesc->ProList().push_back(_overloadBracketsLeftSide);
//   gdlObjectDesc->SetOperator(OOBracketsLeftSide,_overloadBracketsLeftSide);

  DFun *_overloadBracketsRightSide = new DFun("_OVERLOADBRACKETSRIGHTSIDE",GDL_OBJECT_NAME,"*INTERNAL*");
  _overloadBracketsRightSide->AddPar("ISRANGE");
  _overloadBracketsRightSide->AddPar("SUB1")->AddPar("SUB2")->AddPar("SUB3")->AddPar("SUB4");
  _overloadBracketsRightSide->AddPar("SUB5")->AddPar("SUB6")->AddPar("SUB7")->AddPar("SUB8");
  WRAPPED_FUNNode *tree3 = new WRAPPED_FUNNode(_GDL_OBJECT_OverloadBracketsRightSide);
  _overloadBracketsRightSide->SetTree( tree3);
  gdlObjectDesc->FunList().push_back(_overloadBracketsRightSide);
//   gdlObjectDesc->SetOperator(OOBracketsRightSide,_overloadBracketsRightSide);

   DFun *_overloadEQ = new DFun("_OVERLOADEQ",GDL_OBJECT_NAME,"*INTERNAL*");
  _overloadEQ->AddPar("LEFT")->AddPar("RIGHT");
  WRAPPED_FUNNode *tree4 = new WRAPPED_FUNNode(_GDL_OBJECT_OverloadEQOp);
  _overloadEQ->SetTree( tree4);
  gdlObjectDesc->FunList().push_back(_overloadEQ);
//   gdlObjectDesc->SetOperator(OOEQ,_overloadEQ);

  DFun *_overloadNE = new DFun("_OVERLOADNE",GDL_OBJECT_NAME,"*INTERNAL*");
  _overloadNE->AddPar("LEFT")->AddPar("RIGHT");
  WRAPPED_FUNNode *tree5 = new WRAPPED_FUNNode(_GDL_OBJECT_OverloadNEOp);
  _overloadNE->SetTree( tree5);
  gdlObjectDesc->FunList().push_back(_overloadNE);
//   gdlObjectDesc->SetOperator(OONE,_overloadNE);

  DFun *_overloadPlus = new DFun("_OVERLOADPLUS",GDL_OBJECT_NAME,"*INTERNAL*");
  _overloadPlus->AddPar("LEFT")->AddPar("RIGHT");
  WRAPPED_FUNNode *tree6 = new WRAPPED_FUNNode(_GDL_OBJECT_OverloadReportIllegalOperation);
  _overloadPlus->SetTree( tree6);
  gdlObjectDesc->FunList().push_back(_overloadPlus);
//   gdlObjectDesc->SetOperator(OOPLUS,_overloadPlus);

  DFun *_overloadMinus = new DFun("_OVERLOADMINUS",GDL_OBJECT_NAME,"*INTERNAL*");
  _overloadMinus->AddPar("LEFT")->AddPar("RIGHT");
  WRAPPED_FUNNode *tree7 = new WRAPPED_FUNNode(_GDL_OBJECT_OverloadReportIllegalOperation);
  _overloadMinus->SetTree( tree7);
  gdlObjectDesc->FunList().push_back(_overloadMinus);
//   gdlObjectDesc->SetOperator(OOMINUS,_overloadMinus);

// LIST  
  DFun *DFunLIST__overloadBracketsRightSide = new DFun("_OVERLOADBRACKETSRIGHTSIDE","LIST","*INTERNAL*");
  DFunLIST__overloadBracketsRightSide->AddPar("ISRANGE");
  DFunLIST__overloadBracketsRightSide->AddPar("SUB1")->AddPar("SUB2")->AddPar("SUB3")->AddPar("SUB4");
  DFunLIST__overloadBracketsRightSide->AddPar("SUB5")->AddPar("SUB6")->AddPar("SUB7")->AddPar("SUB8");
  tree = new WRAPPED_FUNNode( LIST_OverloadBracketsRightSide);
  DFunLIST__overloadBracketsRightSide->SetTree( tree);
  listDesc->FunList().push_back(DFunLIST__overloadBracketsRightSide);
  listDesc->SetOperator(OOBracketsRightSide,DFunLIST__overloadBracketsRightSide);
// LIST::ADD
  DPro *DProLIST__ADD = new DPro("ADD","LIST","*INTERNAL*");
  DProLIST__ADD->AddKey("EXTRACT","EXTRACT")->AddKey("NO_COPY","NO_COPY");
  DProLIST__ADD->AddPar("VALUE")->AddPar("INDEX");
  tree2 = new WRAPPED_PRONode( lib::list__add);
  DProLIST__ADD->SetTree( tree2);
  listDesc->ProList().push_back(DProLIST__ADD);
// LIST::REMOVE()
  DFun *DFunLIST__REMOVE = new DFun("REMOVE","LIST","*INTERNAL*");
  DFunLIST__REMOVE->AddKey("ALL","ALL");
  DFunLIST__REMOVE->AddPar("INDEX");
  tree = new WRAPPED_FUNNode( lib::list__remove_fun);
  DFunLIST__REMOVE->SetTree( tree);
  listDesc->FunList().push_back(DFunLIST__REMOVE);
// LIST::REMOVE PRO
  DPro *DProLIST__REMOVE = new DPro("REMOVE","LIST","*INTERNAL*");
  DProLIST__REMOVE->AddKey("ALL","ALL");
  DProLIST__REMOVE->AddPar("INDEX");
  tree2 = new WRAPPED_PRONode( lib::list__remove_pro);
  DProLIST__REMOVE->SetTree( tree2);
  listDesc->ProList().push_back(DProLIST__REMOVE);
// LIST::REVERSE PRO
  DPro *DProLIST__REVERSE = new DPro("REVERSE","LIST","*INTERNAL*");
  tree2 = new WRAPPED_PRONode( lib::list__reverse);
  DProLIST__REVERSE->SetTree( tree2);
  listDesc->ProList().push_back(DProLIST__REVERSE);
  
}