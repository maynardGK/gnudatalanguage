/***************************************************************************
                          prognodeexpr.cpp  -  GDL's AST is made of DNodes
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

#include <memory>
#include <cassert>

#include <antlr/ASTFactory.hpp>

#include "dinterpreter.hpp"
#include "prognodeexpr.hpp"
#include "basegdl.hpp"
#include "arrayindexlistt.hpp"
#include "envt.hpp"
#include "gdlexception.hpp"
#include "nullgdl.hpp"
#include "basic_fun.hpp"
#include "basic_fun_jmg.hpp"

using namespace std;


BinaryExpr::BinaryExpr( const RefDNode& refNode): DefaultNode( refNode)
{
  op1 = GetFirstChild();
  op2 = GetFirstChild()->GetNextSibling();
  setType( GDLTokenTypes::EXPR);
}

BinaryExprNC::BinaryExprNC( const RefDNode& refNode): BinaryExpr( refNode)
{
  op1NC = NonCopyNode( op1->getType());
  op2NC = NonCopyNode( op2->getType());
}

ProgNode::ProgNode(): // for NULLProgNode
	keepRight( false),
	keepDown( false),
	breakTarget( NULL),
  ttype( antlr::Token::NULL_TREE_LOOKAHEAD),
  text( "NULLProgNode"),
  down( NULL), 
  right( NULL),
  lineNumber( 0),
  cData( NULL),
  libPro( NULL),
  libFun( NULL),
  var( NULL),
  labelStart( 0),
  labelEnd( 0)
{}

BaseGDL* ProgNode::EvalNC()
{
  throw GDLException( this,
		      "Internal error. "
		      "ProgNode::EvalNC() called.",true,false);
}
BaseGDL* ProgNode::EvalNCNull()
{
  return this->EvalNC();
}

BaseGDL** ProgNode::LEval()
{
  throw GDLException( this,
		      "Internal error. "
		      "ProgNode::LEval() called.",true,false);
}
BaseGDL** ProgNode::EvalRefCheck( BaseGDL*& rEval) // default like Eval()
{
  rEval = this->Eval();
  return NULL;
}

RetCode   ProgNode::Run()
{ 
  throw GDLException( this,
		      "Internal error. "
		      "ProgNode::Run() called.",true,false);
  return RC_OK; // silence compiler
}

void ProgNode::SetRightDown( const ProgNodeP r, const ProgNodeP d)
{
  right = r;
  down  = d;
}

BaseGDL* ProgNode::Eval()
{ 
  throw GDLException( this,
		      "Internal error. "
		      "ProgNode::Eval() called.",true,false);
//   return ProgNode::interpreter->expr( this);
}



// converts inferior type to superior type
// for not (yet) overloaded operators
void ProgNode::AdjustTypes(Guard<BaseGDL>& a, Guard<BaseGDL>& b)
{
  DType aTy=a->Type();
  DType bTy=b->Type();
  if( aTy == bTy) return;

  // Will be checked by Convert2() function
//   if( DTypeOrder[aTy] > 100 || DTypeOrder[bTy] > 100) // GDL_STRUCT, GDL_PTR, OBJ
//     {
//       //exception
//       throw GDLException( "Expressions of this type cannot be converted.");
//     }
  
  // GDL_COMPLEX op GDL_DOUBLE = GDL_COMPLEXDBL
  if( (aTy == GDL_COMPLEX && bTy == GDL_DOUBLE) ||
      (bTy == GDL_COMPLEX && aTy == GDL_DOUBLE))
    {
      a.reset( a.release()->Convert2( GDL_COMPLEXDBL));
      b.reset( b.release()->Convert2( GDL_COMPLEXDBL));
      return;
    }

  // Change > to >= JMG
  if( DTypeOrder[aTy] >= DTypeOrder[bTy])
    {
      // convert b to a
      b.reset( b.release()->Convert2( aTy));
    }
  else
    {
      // convert a to b
      a.reset( a.release()->Convert2( bTy));
    }
}
// converts inferior type to superior type
// handles overloaded operators
void ProgNode::AdjustTypesObj(Guard<BaseGDL>& a, Guard<BaseGDL>& b)
{
  DType aTy=a->Type();
  DType bTy=b->Type();
  if( aTy == bTy) return;

  // Will be checked by Convert2() function
//   if( DTypeOrder[aTy] > 100 || DTypeOrder[bTy] > 100) // GDL_STRUCT, GDL_PTR, OBJ
//     {
//       //exception
//       throw GDLException( "Expressions of this type cannot be converted.");
//     }
  
  // GDL_COMPLEX op GDL_DOUBLE = GDL_COMPLEXDBL
  if( (aTy == GDL_COMPLEX && bTy == GDL_DOUBLE) ||
      (bTy == GDL_COMPLEX && aTy == GDL_DOUBLE))
    {
      a.reset( a.release()->Convert2( GDL_COMPLEXDBL));
      b.reset( b.release()->Convert2( GDL_COMPLEXDBL));
      return;
    }

  // Change > to >= JMG
  if( DTypeOrder[aTy] >= DTypeOrder[bTy])
    {
      // convert b to a
      if( aTy == GDL_OBJ) // only check for aTy is ok because GDL_OBJ has highest order
	return; // for operator overloading, do not convert other type then
      b.reset( b.release()->Convert2( aTy));
    }
  else
    {
      // convert a to b
      if( bTy == GDL_OBJ) // only check for bTy is ok because GDL_OBJ has highest order
	return; // for operator overloading, do not convert other type then
      a.reset( a.release()->Convert2( bTy));
    }
}

// for not (yet) overloaded operators
void BinaryExprNC::AdjustTypesNC(Guard<BaseGDL>& g1, BaseGDL*& e1,
				 Guard<BaseGDL>& g2, BaseGDL*& e2)
{
  if( op1NC)
    {
      e1 = op1->EvalNC();
    }
  else
    {
      e1 = op1->Eval();
      g1.reset( e1);
    }
  if( op2NC)
    {
      e2 = op2->EvalNC();
    }
  else
    {
      e2 = op2->Eval();
      g2.reset( e2);
    }

  DType aTy=e1->Type();
  DType bTy=e2->Type();
  if( aTy == bTy) return;

  // Will be checked by Convert2() function
//   if( DTypeOrder[aTy] > 100 || DTypeOrder[bTy] > 100) // GDL_STRUCT, GDL_PTR, OBJ
//     {
//       throw GDLException( "Expressions of this type cannot be converted.");
//     }

  // Change > to >= JMG
  if( DTypeOrder[aTy] >= DTypeOrder[bTy])
    {
      // GDL_COMPLEX op GDL_DOUBLE = GDL_COMPLEXDBL
      if( (aTy == GDL_COMPLEX && bTy == GDL_DOUBLE))
	{
	  e2 = e2->Convert2( GDL_COMPLEXDBL, BaseGDL::COPY);
	  g2.reset( e2); // delete former e2
	  e1 = e1->Convert2( GDL_COMPLEXDBL, BaseGDL::COPY);
	  g1.reset( e1); // delete former e1
	  return;
	}

      // convert e2 to e1
      e2 = e2->Convert2( aTy, BaseGDL::COPY);
      g2.reset( e2); // delete former e2
    }
  else
    {
      // GDL_COMPLEX op GDL_DOUBLE = GDL_COMPLEXDBL
      if( (bTy == GDL_COMPLEX && aTy == GDL_DOUBLE))
	{
	  e2 = e2->Convert2( GDL_COMPLEXDBL, BaseGDL::COPY);
	  g2.reset( e2); // delete former e2
	  e1 = e1->Convert2( GDL_COMPLEXDBL, BaseGDL::COPY);
	  g1.reset( e1); // delete former e1
	  return;
	}

      // convert e1 to e2
      e1 = e1->Convert2( bTy, BaseGDL::COPY);
      g1.reset( e1); // delete former e1
    }
}

// handles overloaded operators
void BinaryExprNC::SetupGuards(Guard<BaseGDL>& g1, BaseGDL*& e1,
				 Guard<BaseGDL>& g2, BaseGDL*& e2)
{
  if( op1NC)
    {
      e1 = op1->EvalNC();
    }
  else
    {
      e1 = op1->Eval();
      g1.Init( e1);
    }
  if( op2NC)
    {
      e2 = op2->EvalNC();
    }
  else
    {
      e2 = op2->Eval();
      g2.Init( e2);
    }
}


// only called from EqOp and NeOp
// also handles overloaded operators
void BinaryExprNC::AdjustTypesNCNull(Guard<BaseGDL>& g1, BaseGDL*& e1,
				 Guard<BaseGDL>& g2, BaseGDL*& e2)
{
  if( op1NC)
    {
      e1 = op1->EvalNCNull();
    }
  else
    {
      e1 = op1->Eval();
      g1.Init( e1);
    }
  if( op2NC)
    {
      e2 = op2->EvalNCNull();
    }
  else
    {
      e2 = op2->Eval();
      g2.Init( e2);
    }
      
  // if at least one is !NULL make sure this is e1 
  if( e1 == NullGDL::GetSingleInstance())
    return;
  if( e2 == NullGDL::GetSingleInstance())
  {
    // e1 is not !NULL (but might be NULL)
    BaseGDL* tmp = e1;
    e1 = e2;
    e2 = tmp;
    return;
  }

  if( e1 == NULL)
  {
    // provoke error
    e1 = op1->EvalNC();
    assert( false); // code should never reach here
  }
  else if(  e2 == NULL)
  {
    // provoke error
    e2 = op2->EvalNC();
    assert( false); // code should never reach here
  }
  
  DType aTy=e1->Type();
  DType bTy=e2->Type();
  if( aTy == bTy) return;

  // Will be checked by Convert2() function
//   if( DTypeOrder[aTy] > 100 || DTypeOrder[bTy] > 100) // GDL_STRUCT, GDL_PTR, OBJ
//     {
//       throw GDLException( "Expressions of this type cannot be converted.");
//     }

  // Change > to >= JMG
  if( DTypeOrder[aTy] >= DTypeOrder[bTy])
    {
      // GDL_COMPLEX op GDL_DOUBLE = GDL_COMPLEXDBL
      if( (aTy == GDL_COMPLEX && bTy == GDL_DOUBLE))
	{
	  e2 = e2->Convert2( GDL_COMPLEXDBL, BaseGDL::COPY);
	  g2.Reset( e2); // delete former e2
	  e1 = e1->Convert2( GDL_COMPLEXDBL, BaseGDL::COPY);
	  g1.Reset( e1); // delete former e1
	  return;
	}

      // no conversion because of operator overloads
      if( aTy == GDL_OBJ) // only check for aTy is ok because GDL_OBJ has highest order
	return;

      // convert e2 to e1
      e2 = e2->Convert2( aTy, BaseGDL::COPY);
      g2.Reset( e2); // delete former e2
    }
  else
    {
      // GDL_COMPLEX op GDL_DOUBLE = GDL_COMPLEXDBL
      if( (bTy == GDL_COMPLEX && aTy == GDL_DOUBLE))
	{
	  e2 = e2->Convert2( GDL_COMPLEXDBL, BaseGDL::COPY);
	  g2.Reset( e2); // delete former e2
	  e1 = e1->Convert2( GDL_COMPLEXDBL, BaseGDL::COPY);
	  g1.Reset( e1); // delete former e1
	  return;
	}

      // no conversion because of operator overloads
      if( bTy == GDL_OBJ) // only check for bTy is ok because GDL_OBJ has highest order
	return;

      // convert e1 to e2
      e1 = e1->Convert2( bTy, BaseGDL::COPY);
      g1.Reset( e1); // delete former e1
    }
}

BaseGDL* VARNode::Eval()
{
    BaseGDL* vData = this->EvalNC();
	if( vData == NULL)
        {
            throw GDLException( this, "Variable is undefined: "+this->getText(),true,false);
        }
	return vData->Dup();
}
BaseGDL* VARPTRNode::Eval()
{
    BaseGDL* vData = this->EvalNC();
	if( vData == NULL)
        {
            throw GDLException( this, "Common block variable is undefined.",true,false);
        }
	return vData->Dup();
}
BaseGDL* SYSVARNode::Eval()
{
	return this->EvalNC()->Dup();
}

BaseGDL* VARNode::EvalNC()
{
	EnvStackT& callStack=interpreter->CallStack();
	BaseGDL* res=static_cast<EnvUDT*> ( callStack.back() )->GetKW ( this->varIx );
	if ( res == NULL )
		throw GDLException ( this, "Variable is undefined: "+
		                     callStack.back()->GetString ( this->varIx ),true,false );
	return res;
}
BaseGDL* VARNode::EvalNCNull()
{
	EnvStackT& callStack=interpreter->CallStack();
	BaseGDL* res=static_cast<EnvUDT*> ( callStack.back() )->GetKW ( this->varIx );
// 	if ( res == NULL )
// 	  res = NullGDL::GetSingleInstance();
	return res;
}

BaseGDL* VARPTRNode::EvalNC()
{
      BaseGDL* res=this->var->Data();
      if( res == NULL)
	{
	  EnvStackT& callStack=interpreter->CallStack();
	  throw GDLException( this, "Variable is undefined: "+
			      callStack.back()->GetString( res),true,false);
	}
      return res;
}
BaseGDL* VARPTRNode::EvalNCNull()
{
      BaseGDL* res=this->var->Data();
//       if( res == NULL)
// 	  res = NullGDL::GetSingleInstance();
      return res;
}

BaseGDL* CONSTANTNode::EvalNC()
{
  return this->cData;
}
BaseGDL* CONSTANTNode::Eval()
{
  return this->cData->Dup();
}

BaseGDL* SYSVARNode::EvalNC()
{
  if( this->var == NULL) 
    {
      this->var=FindInVarList(sysVarList,this->getText());
      if( this->var == NULL)		    
		throw GDLException( this, "Not a legal system variable: !"+
			    this->getText(),true,false);
    }
  // system variables are always defined
  return this->var->Data(); 
}

BaseGDL** SYSVARNode::EvalRefCheck( BaseGDL*& rEval)
{
  return this->LEval();
}

BaseGDL** SYSVARNode::LEval()
{
  const ProgNodeP& sysVar = this;
  //match(antlr::RefAST(_t),SYSVAR);
  if( sysVar->var == NULL) 
  {
    sysVar->var=FindInVarList(sysVarList,sysVar->getText());
    if( sysVar->var == NULL)		    
      throw GDLException( sysVar, "Not a legal system variable: !"+
	  sysVar->getText(),true,false);
    
    // note: this works, because system variables are never 
    //       passed by reference
    SizeT rdOnlySize = sysVarRdOnlyList.size();
    for( SizeT i=0; i<rdOnlySize; ++i)
      if( sysVarRdOnlyList[ i] == sysVar->var)
	throw GDLException( this, 
	"Attempt to write to a readonly variable: !"+
	sysVar->getText(),true,false);
  }  
//   interpreter->SetRetTree( sysVar->getNextSibling());
  // system variables are always defined
  return &sysVar->var->Data();
}


BaseGDL* DEREFNode::Eval()
{
	BaseGDL** e2=this->LEval();
	if( *e2 == NULL)
		throw GDLException( this, "Variable is undefined: "+
			interpreter->Name(e2),true,false);
	return (*e2)->Dup();
}
	
BaseGDL* DEREFNode::EvalNC()
{
  Guard<BaseGDL> e1_guard;
  BaseGDL* e1;
  ProgNodeP evalExpr = this->getFirstChild();
  if( NonCopyNode( evalExpr->getType()))
  {
    e1 = evalExpr->EvalNC();
  }
  else
  {
    e1 = evalExpr->Eval();
    e1_guard.Reset(e1);
  }

  if( e1 == NULL || e1->Type() != GDL_PTR)
    throw GDLException( this, "Pointer type required"
			" in this context: "+interpreter->Name(e1),true,false);
  DPtrGDL* ptr=static_cast<DPtrGDL*>(e1);
  DPtr sc;
  if( !ptr->Scalar(sc))
    throw GDLException( this, "Expression must be a "
			"scalar in this context: "+interpreter->Name(e1),true,false);
  if( sc == 0)
    throw GDLException( this, "Unable to dereference"
			" NULL pointer: "+interpreter->Name(e1),true,false);
  
  try{
    return interpreter->GetHeap(sc);
  }
  catch( GDLInterpreter::HeapException)
    {
      throw GDLException( this, "Invalid pointer: "+interpreter->Name(e1),true,false);
    }
}

BaseGDL** DEREFNode::EvalRefCheck( BaseGDL*& rEval)
{
  return this->LEval();
}

BaseGDL** DEREFNode::LEval()
{
//   ProgNodeP retTree = this->getNextSibling();

  // use new env if set (during parameter parsing)
  EnvBaseT* actEnv = DInterpreter::CallStackBack()->GetNewEnv();
  if( actEnv == NULL) actEnv = DInterpreter::CallStackBack();

  assert( actEnv != NULL);

  Guard<BaseGDL> e1_guard;
  BaseGDL* e1;
  ProgNodeP evalExpr = this->getFirstChild();
  if( NonCopyNode( evalExpr->getType()))
    {
      e1 = evalExpr->EvalNC();
    }
  else if( evalExpr->getType() ==  GDLTokenTypes::FCALL_LIB)
    {
      e1=interpreter->lib_function_call(evalExpr);

      if( e1 == NULL) // ROUTINE_NAMES
	      throw GDLException( evalExpr, "Undefined return value", true, false);
      
      if( !DInterpreter::CallStackBack()->Contains( e1)) 
	{
  	  actEnv->DeleteAtExit( e1); 
	}
    }
  else
    {
      e1 = evalExpr->Eval();
      actEnv->DeleteAtExit( e1); 
    }

  if( e1 == NULL || e1->Type() != GDL_PTR)
  throw GDLException( evalExpr, "Pointer type required"
		      " in this context: "+interpreter->Name(e1),true,false);

  DPtrGDL* ptr=static_cast<DPtrGDL*>(e1);

  DPtr sc; 
  if( !ptr->StrictScalar(sc))
  throw GDLException( this, "Expression must be a "
  "scalar in this context: "+interpreter->Name(e1),true,false);
  if( sc == 0)
  throw GDLException( this, "Unable to dereference"
  " NULL pointer: "+interpreter->Name(e1),true,false);
  
  BaseGDL** res;
  try{
    res = &interpreter->GetHeap(sc);
  }
  catch( DInterpreter::HeapException)
  {
    throw GDLException( this, "Invalid pointer: "+interpreter->Name(e1),true,false);
  }
  
//   interpreter->SetRetTree( retTree);
  return res;
}






// trinary operator
BaseGDL* QUESTIONNode::Eval()
{
  Guard<BaseGDL> e1_guard;
  BaseGDL* e1;
  if( NonCopyNode( op1->getType()))
  {
	e1 = op1->EvalNC();
  }
  else
  {
	e1 = op1->Eval();
    e1_guard.Reset(e1);
  }
//  Guard<BaseGDL> e1( op1->Eval());
  if( e1->True())
    {
      return op2->Eval(); // right->down
    }
  return op3->Eval(); // right->right
}

ProgNodeP QUESTIONNode::AsParameter()
{
  Guard<BaseGDL> e1_guard;
  BaseGDL* e1;
  if( NonCopyNode( op1->getType()))
  {
	e1 = op1->EvalNC();
  }
  else
  {
	e1 = op1->Eval();
    e1_guard.Reset(e1);
  }
//  Guard<BaseGDL> e1( op1->Eval());
  if( e1->True())
    {
      return op2;
    }
  return op3;
}

// unary operators
BaseGDL* UMINUSNode::Eval()
{
  BaseGDL* e1 = down->Eval();
  return e1->UMinus(); // might delete e1 (GDL_STRING)
}
BaseGDL* NOT_OPNode::Eval()
{
  BaseGDL* e1 = down->Eval();
  return e1->NotOp();
}
BaseGDL* LOG_NEGNode::Eval()
{
  Guard<BaseGDL> e1( down->Eval());
  return e1->LogNeg();
}

// binary operators
BaseGDL* AND_OPNode::Eval()
{ BaseGDL* res;
  Guard<BaseGDL> e1( op1->Eval());
  Guard<BaseGDL> e2( op2->Eval());
  AdjustTypes(e1,e2);
  if( e1->StrictScalar()) 
    {
    res= e2->AndOpS(e1.get()); // scalar+scalar or array+scalar
    e2.release();
    }
  else
    if( e2->StrictScalar())
      {
      res= e1->AndOpInvS(e2.get()); // array+scalar
      e1.release();
      }
    else
      if( e1->N_Elements() <= e2->N_Elements())
	{
	res = e1->AndOpInv(e2.get()); // smaller_array + larger_array or same size
	e1.release();
	}
      else
	{
	res = e2->AndOp(e1.get()); // smaller + larger
	e2.release();
	}
  return res;
}
BaseGDL* OR_OPNode::Eval()
{ BaseGDL* res;
 Guard<BaseGDL> e1( op1->Eval());
 Guard<BaseGDL> e2( op2->Eval());
 AdjustTypes(e1,e2);
 if( e1->StrictScalar())
   {
     res= e2->OrOpS(e1.get()); // scalar+scalar or array+scalar
     e2.release();
   }
 else
   if( e2->StrictScalar())
     {
       res= e1->OrOpInvS(e2.get()); // array+scalar
       e1.release();
     }
   else
     if( e1->N_Elements() <= e2->N_Elements())
       {
	 res= e1->OrOpInv(e2.get()); // smaller_array + larger_array or same size
	 e1.release();
       }
     else
       {
	 res= e2->OrOp(e1.get()); // smaller + larger
	 e2.release();
       }
 return res;
}
BaseGDL* XOR_OPNode::Eval()
{ BaseGDL* res;
  Guard<BaseGDL> e1( op1->Eval());
  Guard<BaseGDL> e2( op2->Eval());
  AdjustTypes(e1,e2);
  if( e1->N_Elements() <= e2->N_Elements())
    {
    res= e1->XorOp(e2.get()); // smaller_array + larger_array or same size
	 e1.release();
    }
  else
    {
    res= e2->XorOp(e1.get()); // smaller + larger
	 e2.release();
    }
  return res;
}
BaseGDL* LOG_ANDNode::Eval()
{ BaseGDL* res;
  Guard<BaseGDL> e1( op1->Eval());
  if( !e1->LogTrue()) return new DByteGDL( 0);
  Guard<BaseGDL> e2( op2->Eval());
  if( !e2->LogTrue()) return new DByteGDL( 0);
  return new DByteGDL( 1);
}
BaseGDL* LOG_ORNode::Eval()
{ BaseGDL* res;
  Guard<BaseGDL> e1( op1->Eval());
  if( e1->LogTrue()) return new DByteGDL( 1); 
  Guard<BaseGDL> e2( op2->Eval());
  if( e2->LogTrue()) return new DByteGDL( 1);
  return new DByteGDL( 0);
}

BaseGDL* EQ_OPNode::Eval()
{ 
  Guard<BaseGDL> e1( op1->Eval());
  Guard<BaseGDL> e2( op2->Eval());
  AdjustTypesObj(e1,e2);
  if( e2->Type() == GDL_OBJ) 
  {
    if( e1->Type() != GDL_OBJ) 
    {
      // order is critical: overload might just be defined for one of the object types
      // use e2 only if e1 is no object
      BaseGDL* res=e2->EqOp(e1.get());
      return res;
    }
  }
  BaseGDL* res=e1->EqOp(e2.get());
  return res;
}
BaseGDL* NE_OPNode::Eval()
{ 
  Guard<BaseGDL> e1( op1->Eval());
  Guard<BaseGDL> e2( op2->Eval());
  AdjustTypesObj(e1,e2);
  if( e2->Type() == GDL_OBJ) 
  {
    if( e1->Type() != GDL_OBJ) 
    {
      // order is critical: overload might just be defined for one of the object types
      // use e2 only if e1 is no object
      BaseGDL* res=e2->NeOp(e1.get());
      return res;
    }
  }
  BaseGDL* res=e1->NeOp(e2.get());
  return res;
}
BaseGDL* LE_OPNode::Eval()
{ BaseGDL* res;
  Guard<BaseGDL> e1( op1->Eval());
  Guard<BaseGDL> e2( op2->Eval());
  AdjustTypes(e1,e2);
  res=e1->LeOp(e2.get());
  return res;
}
BaseGDL* LT_OPNode::Eval()
{ BaseGDL* res;
  Guard<BaseGDL> e1( op1->Eval());
  Guard<BaseGDL> e2( op2->Eval());
  AdjustTypes(e1,e2);
  res=e1->LtOp(e2.get());
  return res;
}
BaseGDL* GE_OPNode::Eval()
{ BaseGDL* res;
  Guard<BaseGDL> e1( op1->Eval());
  Guard<BaseGDL> e2( op2->Eval());
  AdjustTypes(e1,e2);
  res=e1->GeOp(e2.get());
  return res;
}
BaseGDL* GT_OPNode::Eval()
{ BaseGDL* res;
  Guard<BaseGDL> e1( op1->Eval());
  Guard<BaseGDL> e2( op2->Eval());
  AdjustTypes(e1,e2);
  res=e1->GtOp(e2.get());
  return res;
}
BaseGDL* PLUSNode::Eval()
{
  BaseGDL* res;
  Guard<BaseGDL> e1 ( op1->Eval() );
  Guard<BaseGDL> e2 ( op2->Eval() );
	
  DType aTy=e1->Type();
  DType bTy=e2->Type();
  if( aTy == bTy) 
  {
    if( aTy == GDL_OBJ) // we MUST do this here (correct guard handling)
      return e1->Add( e2.get());; // operator overloading
    
  }
      // GDL_COMPLEX op GDL_DOUBLE = GDL_COMPLEXDBL
  else if( (aTy == GDL_COMPLEX && bTy == GDL_DOUBLE) ||
      (bTy == GDL_COMPLEX && aTy == GDL_DOUBLE))
    {
      e1.reset( e1.release()->Convert2( GDL_COMPLEXDBL));
      e2.reset( e2.release()->Convert2( GDL_COMPLEXDBL));
    }
  // Change > to >= JMG
  else if( DTypeOrder[aTy] >= DTypeOrder[bTy])
    {
      // convert b to a
      if( aTy == GDL_OBJ) // only check for aTy is ok because GDL_OBJ has highest order
	return e1->Add( e2.get());; // for operator overloading, do not convert other type then
      e2.reset( e2.release()->Convert2( aTy));
    }
  else
    {
      // convert a to b
      if( bTy == GDL_OBJ) // only check for bTy is ok because GDL_OBJ has highest order
	return e2->AddInv( e1.get());; // for operator overloading, do not convert other type then
      e1.reset( e1.release()->Convert2( bTy));
    }
    
  if ( e1->StrictScalar() )
  {
    res= e2->AddInvS ( e1.get() ); // scalar+scalar or array+scalar
    e2.release();
  }
  else
  if ( e2->StrictScalar() )
  {
	  res= e1->AddS ( e2.get() ); // array+scalar
	  e1.release();
  }
  else
  if ( e1->N_Elements() <= e2->N_Elements() )
  {
	  res= e1->Add ( e2.get() ); // smaller_array + larger_array or same size
	  e1.release();
  }
  else
  {
	  res= e2->AddInv ( e1.get() ); // smaller + larger
	  e2.release();
  }
  return res;
}

BaseGDL* MINUSNode::Eval()
{ 
  BaseGDL* res;
  Guard<BaseGDL> e1( op1->Eval());
  Guard<BaseGDL> e2( op2->Eval());
//  AdjustTypes(e1,e2);
 
  DType aTy=e1->Type();
  DType bTy=e2->Type();
  if( aTy == bTy) 
  {
    if( aTy == GDL_OBJ) // we MUST do this here (correct guard handling)
      return e1->Sub( e2.get());; // operator overloading
    
  }
      // GDL_COMPLEX op GDL_DOUBLE = GDL_COMPLEXDBL
  else if( (aTy == GDL_COMPLEX && bTy == GDL_DOUBLE) ||
      (bTy == GDL_COMPLEX && aTy == GDL_DOUBLE))
    {
      e1.reset( e1.release()->Convert2( GDL_COMPLEXDBL));
      e2.reset( e2.release()->Convert2( GDL_COMPLEXDBL));
    }
  // Change > to >= JMG
  else if( DTypeOrder[aTy] >= DTypeOrder[bTy])
    {
      // convert b to a
      if( aTy == GDL_OBJ) // only check for aTy is ok because GDL_OBJ has highest order
	return e1->Sub( e2.get());; // for operator overloading, do not convert other type then
      e2.reset( e2.release()->Convert2( aTy));
    }
  else
    {
      // convert a to b
      if( bTy == GDL_OBJ) // only check for bTy is ok because GDL_OBJ has highest order
	return e2->SubInv( e1.get());; // for operator overloading, do not convert other type then
      e1.reset( e1.release()->Convert2( bTy));
    }
 
 if( e1->StrictScalar())
   {
     res= e2->SubInvS(e1.get()); // scalar+scalar or array+scalar
     e2.release();
   }
 else
   if( e2->StrictScalar())
     {
       res= e1->SubS(e2.get()); // array+scalar
       e1.release();
     }
   else
     if( e1->N_Elements() <= e2->N_Elements())
       {
	 res= e1->Sub(e2.get()); // smaller_array + larger_array or same size
	 e1.release();
       }
     else
       {
	 res= e2->SubInv(e1.get()); // smaller + larger
	 e2.release();
       }

 return res;
}
BaseGDL* LTMARKNode::Eval()
{ BaseGDL* res;
 Guard<BaseGDL> e1( op1->Eval());
 Guard<BaseGDL> e2( op2->Eval());
 AdjustTypes(e1,e2);
 if( e1->StrictScalar())
   {
     res= e2->LtMarkS(e1.get()); // scalar+scalar or array+scalar
     e2.release();
   }
 else
   if( e2->StrictScalar())
     {
       res= e1->LtMarkS(e2.get()); // array+scalar
       e1.release();
     }
   else
     if( e1->N_Elements() <= e2->N_Elements())
       {
	 res= e1->LtMark(e2.get()); // smaller_array + larger_array or same size
	 e1.release();
       }
     else
       {
	 res= e2->LtMark(e1.get()); // smaller + larger
	 e2.release();
       }
 return res;
}
BaseGDL* GTMARKNode::Eval()
{ BaseGDL* res;
 Guard<BaseGDL> e1( op1->Eval());
 Guard<BaseGDL> e2( op2->Eval());
 AdjustTypes(e1,e2);
 if( e1->StrictScalar())
   {
     res= e2->GtMarkS(e1.get()); // scalar+scalar or array+scalar
     e2.release();
   }
 else
   if( e2->StrictScalar())
     {
       res= e1->GtMarkS(e2.get()); // array+scalar
       e1.release();
     }
   else
     if( e1->N_Elements() <= e2->N_Elements())
       {
	 res= e1->GtMark(e2.get()); // smaller_array + larger_array or same size
	 e1.release();
       }
     else
       {
	 res= e2->GtMark(e1.get()); // smaller + larger
	 e2.release();
       }
 return res;
}
BaseGDL* ASTERIXNode::Eval()
{
	BaseGDL* res;
	Guard<BaseGDL> e1 ( op1->Eval() );
	Guard<BaseGDL> e2 ( op2->Eval() );
	AdjustTypes ( e1,e2 );
	if ( e1->StrictScalar() )
	{
		res= e2->MultS ( e1.get() ); // scalar+scalar or array+scalar
		e2.release();
	}
	else if ( e2->StrictScalar() )
	{
		res= e1->MultS ( e2.get() ); // array+scalar
		e1.release();
	}
	else
		if ( e1->N_Elements() <= e2->N_Elements() )
		{
			res= e1->Mult ( e2.get() ); // smaller_array + larger_array or same size
			e1.release();
		}
		else
		{
			res= e2->Mult ( e1.get() ); // smaller + larger
			e2.release();
		}
	return res;
}

BaseGDL* MATRIX_OP1Node::Eval()
{ BaseGDL* res;
  Guard<BaseGDL> e1( op1->Eval());
  Guard<BaseGDL> e2( op2->Eval());
  DType aTy=e1->Type();
  DType bTy=e2->Type();
  DType maxTy=(DTypeOrder[aTy] >= DTypeOrder[bTy])? aTy: bTy;

  DType cTy=maxTy;
  if( maxTy == GDL_BYTE || maxTy == GDL_INT)
    cTy=GDL_LONG;
  else if( maxTy == GDL_UINT)
    cTy=GDL_ULONG;

  if( aTy != cTy)
    e1.reset( e1.release()->Convert2( cTy));

  AdjustTypes(e1,e2);
  res=e1->MatrixOp(e2.get());
  return res;
}
BaseGDL* MATRIX_OP2Node::Eval()
{ BaseGDL* res;
  Guard<BaseGDL> e1( op1->Eval());
  Guard<BaseGDL> e2( op2->Eval());
  DType aTy=e1->Type();
  DType bTy=e2->Type();
  DType maxTy=(DTypeOrder[aTy] >= DTypeOrder[bTy])? aTy: bTy;

  DType cTy=maxTy;
  if( maxTy == GDL_BYTE || maxTy == GDL_INT)
    cTy=GDL_LONG;
  else if( maxTy == GDL_UINT)
    cTy=GDL_ULONG;

  if( aTy != cTy) 
    e1.reset( e1.release()->Convert2( cTy));

  AdjustTypes(e1,e2);
  res=e2->MatrixOp(e1.get());
  return res;
}
BaseGDL* SLASHNode::Eval()
{ BaseGDL* res;
 Guard<BaseGDL> e1( op1->Eval());
 Guard<BaseGDL> e2( op2->Eval());
 AdjustTypes(e1,e2);
 if( e1->StrictScalar())
   {
     res= e2->DivInvS(e1.get()); // scalar+scalar or array+scalar
     e2.release();
   }
 else
   if( e2->StrictScalar())
     {
       res= e1->DivS(e2.get()); // array+scalar
       e1.release();
     }
   else
     if( e1->N_Elements() <= e2->N_Elements())
       {
	 res= e1->Div(e2.get()); // smaller_array + larger_array or same size
	 e1.release();
       }
     else
       {
	 res= e2->DivInv(e1.get()); // smaller + larger
	 e2.release();
       }

 return res;
}
BaseGDL* MOD_OPNode::Eval()
{ BaseGDL* res;
 Guard<BaseGDL> e1( op1->Eval());
 Guard<BaseGDL> e2( op2->Eval());
 AdjustTypes(e1,e2);
 if( e1->StrictScalar())
   {
     res= e2->ModInvS(e1.get()); // scalar+scalar or array+scalar
     e2.release();
   }
 else
   if( e2->StrictScalar())
     {
       res= e1->ModS(e2.get()); // array+scalar
       e1.release();
     }
   else
     if( e1->N_Elements() <= e2->N_Elements())
       {
	 res= e1->Mod(e2.get()); // smaller_array + larger_array or same size
	 e1.release();
       }
     else
       {
	 res= e2->ModInv(e1.get()); // smaller + larger
	 e2.release();
       }
 return res;
}


BaseGDL* POWNode::Eval()
{ BaseGDL* res;
  Guard<BaseGDL> e1( op1->Eval());
  Guard<BaseGDL> e2( op2->Eval());
  // special handling for aTy == complex && bTy != complex
  DType aTy=e1->Type();
  DType bTy=e2->Type();
 if( aTy == GDL_STRING)
   {
     e1.reset( e1->Convert2( GDL_FLOAT, BaseGDL::COPY));
     aTy = GDL_FLOAT;
   }
 if( bTy == GDL_STRING)
   {
     e2.reset( e2->Convert2( GDL_FLOAT, BaseGDL::COPY));
     bTy = GDL_FLOAT;
   }
  if( ComplexType( aTy))
    {
      if( IntType( bTy))
	{
	  e2.reset( e2.release()->Convert2( GDL_LONG));
	  res = e1->Pow( e2.get());
	  if( res == e1.get())
	    e1.release();
	  return res;
	}
      if( aTy == GDL_COMPLEX)
	{
	  if( bTy == GDL_DOUBLE)
	    {
	      e1.reset( e1.release()->Convert2( GDL_COMPLEXDBL));
	      aTy = GDL_COMPLEXDBL;
	    }
	  else if( bTy == GDL_FLOAT)
	    {
	      res = e1->Pow( e2.get());
	      if( res == e1.get())
		e1.release();
	      return res;
	    }
	}
      if( aTy == GDL_COMPLEXDBL)
	{
	  if( bTy == GDL_FLOAT)
	    {
	      e2.reset( e2.release()->Convert2( GDL_DOUBLE));
	      bTy = GDL_DOUBLE;
	    }
	  if( bTy == GDL_DOUBLE)
	    {
	      res = e1->Pow( e2.get());
	      if( res == e1.get())
		e1.release();
	      return res;
	    }
	}
    }

  if( IntType( bTy) && FloatType( aTy))
    {
      e2.reset( e2.release()->Convert2( GDL_LONG));

      res = e1->PowInt( e2.get());
      if( res == e1.get())
	e1.release();

      return res;
    }
  
  DType convertBackT; 

  // convert back
  if( IntType( bTy) && (DTypeOrder[ bTy] > DTypeOrder[ aTy]))
//   if( IntType( bTy) && (DTypeOrder[ bTy] > DTypeOrder[ aTy]))
    convertBackT = aTy;
  else
    convertBackT = GDL_UNDEF;
  
  //  first operand determines type JMG
  //  AdjustTypes(e2,e1); // order crucial here (for converting back)
  AdjustTypes(e1,e2); // order crucial here (for converting back)

  if( e1->StrictScalar())
    {
      res= e2->PowInvS(e1.get()); // scalar+scalar or array+scalar
      e2.release();
    }
  else
    if( e2->StrictScalar())
      {
      res= e1->PowS(e2.get()); // array+scalar
      e1.release();
      }
    else
      if( e1->N_Elements() <= e2->N_Elements())
	{
	res= e1->Pow(e2.get()); // smaller_array + larger_array or same size
	e1.release();
	}
      else
	{
	  res= e2->PowInv(e1.get()); // smaller + larger
	  e2.release();
	}
  if( convertBackT != GDL_UNDEF)
    {
      res = res->Convert2( convertBackT, BaseGDL::CONVERT);
    }
 endPOW:
  return res;
}
// BaseGDL* DECNode::Eval()
// { BaseGDL* res;
//   return new DECNode( refNode);
// }
// BaseGDL* INCNode::Eval()
// { BaseGDL* res;
//   return new INCNode( refNode);
// }
// BaseGDL* POSTDECNode::Eval()
// { BaseGDL* res;
//   return new POSTDECNode( refNode);
// }
// BaseGDL* POSTINCNode::Eval()
// { BaseGDL* res;
//   return new POSTINCNode( refNode);
// }

// ***********************
// **** nonCopy nodes ****
// ***********************
BaseGDL* AND_OPNCNode::Eval()
{ BaseGDL* res;
 Guard<BaseGDL> g1;
 Guard<BaseGDL> g2;
 BaseGDL *e1, *e2; AdjustTypesNC( g1, e1, g2, e2); 

 if( e1->StrictScalar())
   {
     if( g2.get() == NULL) return e2->AndOpSNew( e1);  else  g2.release();
     res= e2->AndOpS(e1); // scalar+scalar or array+scalar
     
   }
 else
   if( e2->StrictScalar())
     {
       if( g1.get() == NULL) return e1->AndOpInvSNew( e2); else g1.release();
       res= e1->AndOpInvS(e2); // array+scalar
       
     }
   else
     if( e1->N_Elements() == e2->N_Elements())
       {
	 if( g1.get() != NULL)
	   {
	     g1.release();
	     return e1->AndOpInv(e2);
	   }
	 if( g2.get() != NULL) 
	   {
	     g2.release();
	     res = e2->AndOp(e1);
	     res->SetDim( e1->Dim());
	     return res;
	   }
	 else
	   {
	     return e1->AndOpInvNew(e2);
	   }
       }
     else if( e1->N_Elements() < e2->N_Elements())
       {
	 if( g1.get() == NULL) return e1->AndOpInvNew( e2); else g1.release();
	 res = e1->AndOpInv(e2); // smaller_array + larger_array or same size
	 
       }
     else
       {
	 if( g2.get() == NULL) return e2->AndOpNew( e1); else g2.release();
	 res = e2->AndOp(e1); // smaller + larger
	 
       }

 return res;
}
BaseGDL* OR_OPNCNode::Eval()
{ BaseGDL* res;
 Guard<BaseGDL> g1;
 Guard<BaseGDL> g2;
 BaseGDL *e1, *e2; AdjustTypesNC( g1, e1, g2, e2); 

 if( e1->StrictScalar())
   {
     if( g2.get() == NULL) return e2->OrOpSNew( e1); else g2.release();
     res= e2->OrOpS(e1); // scalar+scalar or array+scalar
     
   }
 else
   if( e2->StrictScalar())
     {
       if( g1.get() == NULL) return e1->OrOpInvSNew( e2); else g1.release();
       res= e1->OrOpInvS(e2); // array+scalar
       
     }
   else
 if( e1->N_Elements() == e2->N_Elements())
   {
     if( g1.get() != NULL)
       {
	 g1.release();
	 return e1->OrOpInv(e2);
       }
     if( g2.get() != NULL) 
       {
	 g2.release();
	 res = e2->OrOp(e1);
	 res->SetDim( e1->Dim());
	 return res;
       }
     else
       {
	 return e1->OrOpInvNew(e2);
       }
   }
else if( e1->N_Elements() < e2->N_Elements())
       {
	 if( g1.get() == NULL) return e1->OrOpInvNew( e2); else g1.release();
	 res= e1->OrOpInv(e2); // smaller_array + larger_array or same size
	 
       }
     else
       {
	 if( g2.get() == NULL) return e2->OrOpNew( e1); else g2.release();
	 res= e2->OrOp(e1); // smaller + larger
	 
       }
 return res;
}
BaseGDL* XOR_OPNCNode::Eval()
{ BaseGDL* res;
  Guard<BaseGDL> g1;
  Guard<BaseGDL> g2;
  BaseGDL *e1, *e2; AdjustTypesNC( g1, e1, g2, e2); 

  if( e1->StrictScalar())
    {
      if( g2.get() == NULL) return e2->XorOpSNew( e1); else g2.release();
      res= e2->XorOpS(e1); // scalar+scalar or array+scalar
     
    }
  else if( e2->StrictScalar())
      {
	if( g1.get() == NULL) return e1->XorOpSNew( e2); else g1.release();
	res= e1->XorOpS(e2); // array+scalar
       
      }
    else if( e1->N_Elements() == e2->N_Elements())
      {
	if( g1.get() != NULL)
	  {
	    g1.release();
	    return e1->XorOp(e2);
	  }
	if( g2.get() != NULL) 
	  {
	    g2.release();
	    res = e2->XorOp(e1);
	    res->SetDim( e1->Dim());
	    return res;
	  }
	else
	  {
	    return e1->XorOpNew(e2);
	  }
      }
    else if( e1->N_Elements() < e2->N_Elements())
      {
	if( g1.get() == NULL) return e1->XorOpNew( e2); else g1.release();
	res= e1->XorOp(e2); // smaller_array + larger_array or same size
     
      }
    else
      {
	if( g2.get() == NULL) return e2->XorOpNew( e1); else g2.release();
	res= e2->XorOp(e1); // smaller + larger
     
      }
  return res;
}
BaseGDL* LOG_ANDNCNode::Eval()
{ BaseGDL* res;
 Guard<BaseGDL> g1;
 Guard<BaseGDL> g2;
 BaseGDL *e1, *e2;
 if( op1NC)
   {
     e1 = op1->EvalNC();
   }
 else
   {
     e1 = op1->Eval();
     g1.reset( e1);
   }
 if( !e1->LogTrue()) return new DByteGDL( 0);

 if( op2NC)
   {
     e2 = op2->EvalNC();
   }
 else
   {
     e2 = op2->Eval();
     g2.reset( e2);
   }
 if( !e2->LogTrue()) return new DByteGDL( 0);
 return new DByteGDL( 1);
}
BaseGDL* LOG_ORNCNode::Eval()
{ BaseGDL* res;
 Guard<BaseGDL> g1;
 Guard<BaseGDL> g2;
 BaseGDL *e1, *e2;
 if( op1NC)
   {
     e1 = op1->EvalNC();
   }
 else
   {
     e1 = op1->Eval();
     g1.reset( e1);
   }
 if( e1->LogTrue()) return new DByteGDL( 1); 
 if( op2NC)
   {
     e2 = op2->EvalNC();
   }
 else
   {
     e2 = op2->Eval();
     g2.reset( e2);
   }
 if( e2->LogTrue()) return new DByteGDL( 1);
 return new DByteGDL( 0);
}


BaseGDL* EQ_OPNCNode::Eval()
{ BaseGDL* res;
  Guard<BaseGDL> g1;
  Guard<BaseGDL> g2;
  BaseGDL *e1, *e2;
  AdjustTypesNCNull( g1, e1, g2, e2);
  if( e2->Type() == GDL_OBJ) 
  {
    if( e1->Type() != GDL_OBJ) 
    {
      // order is critical: overload might just be defined for one of the object types
      // use e2 only if e1 is no object
      BaseGDL* res=e2->EqOp(e1);
      return res;
    }
  }     
  res=e1->EqOp(e2);
  return res;
}
BaseGDL* NE_OPNCNode::Eval()
{ BaseGDL* res;
  Guard<BaseGDL> g1;
  Guard<BaseGDL> g2;
  BaseGDL *e1, *e2;
  AdjustTypesNCNull( g1, e1, g2, e2);
  if( e2->Type() == GDL_OBJ) 
  {
    if( e1->Type() != GDL_OBJ) 
    {
      // order is critical: overload might just be defined for one of the object types
      // use e2 only if e1 is no object
      BaseGDL* res=e2->NeOp(e1);
      return res;
    }
  }
  res=e1->NeOp(e2);
  return res;
}
BaseGDL* LE_OPNCNode::Eval()
{ BaseGDL* res;
  Guard<BaseGDL> g1;
  Guard<BaseGDL> g2;
  BaseGDL *e1, *e2;
  AdjustTypesNC( g1, e1, g2, e2);
  res=e1->LeOp(e2);
  return res;
}
BaseGDL* LT_OPNCNode::Eval()
{ BaseGDL* res;
  Guard<BaseGDL> g1;
  Guard<BaseGDL> g2;
  BaseGDL *e1, *e2;
  AdjustTypesNC( g1, e1, g2, e2);
  res=e1->LtOp(e2);
  return res;
}
BaseGDL* GE_OPNCNode::Eval()
{ BaseGDL* res;
  Guard<BaseGDL> g1;
  Guard<BaseGDL> g2;
  BaseGDL *e1, *e2;
  AdjustTypesNC( g1, e1, g2, e2);
  res=e1->GeOp(e2);
  return res;
}
BaseGDL* GT_OPNCNode::Eval()
{ BaseGDL* res;
  Guard<BaseGDL> g1;
  Guard<BaseGDL> g2;
  BaseGDL *e1, *e2;
  AdjustTypesNC( g1, e1, g2, e2);
  res=e1->GtOp(e2);
  return res;
}
BaseGDL* PLUSNC12Node::Eval()
{
  BaseGDL *e1 = op1->EvalNC();
  BaseGDL *e2 = op2->EvalNC();
  DType aTy=e1->Type();
  DType bTy=e2->Type();
  if( aTy == bTy)
  {
    // as all Add...New() functions are implemented for GDL_OBJ, we can save the if(...) here
    // (as there are no guards up to here)
    //     if( aTy == GDL_OBJ) // this saves us to implement all Add...New() functions
    //       return e1->Add( e2);
    
    if ( e1->StrictScalar() )
    {
      return e2->AddInvSNew( e1 ); // scalar+scalar or array+scalar
    }
    else if ( e2->StrictScalar() )
    {
      return e1->AddSNew( e2); // array+scalar
    }
    else if ( e1->N_Elements() <= e2->N_Elements() )
    {
      return e1->AddNew ( e2 );
    }
    else // e1->N_Elements() > e2->N_Elements() )
    {
      return e2->AddInvNew( e1); // smaller + larger
    }
  }
  Guard<BaseGDL> g1;
  Guard<BaseGDL> g2;
  if( DTypeOrder[aTy] >= DTypeOrder[bTy])
    {
      if( aTy == GDL_OBJ)
	return e1->Add( e2);

      // GDL_COMPLEX op GDL_DOUBLE = GDL_COMPLEXDBL
      if(aTy == GDL_COMPLEX && bTy == GDL_DOUBLE)
      {
	  e2 = e2->Convert2( GDL_COMPLEXDBL, BaseGDL::COPY);
	  g2.reset( e2);
	  e1 = e1->Convert2( GDL_COMPLEXDBL, BaseGDL::COPY);
	  g1.reset( e1);
      }
      else
      {
	// convert e2 to e1
	e2 = e2->Convert2( aTy, BaseGDL::COPY);
	g2.reset( e2);
      }
    }
  else
    {
      if( bTy == GDL_OBJ)
	return e2->AddInv( e1);

      // GDL_COMPLEX op GDL_DOUBLE = GDL_COMPLEXDBL
      if( (bTy == GDL_COMPLEX && aTy == GDL_DOUBLE))
	{
	  e2 = e2->Convert2( GDL_COMPLEXDBL, BaseGDL::COPY);
	  g2.reset( e2);
	  e1 = e1->Convert2( GDL_COMPLEXDBL, BaseGDL::COPY);
	  g1.reset( e1);
	}
      else
	{// convert e1 to e2
	    e1 = e1->Convert2( bTy, BaseGDL::COPY);
	    g1.reset( e1);
	}
    }
    
  // 'classic handling from here
  BaseGDL* res;
  if ( e1->StrictScalar() )
  {
	  if ( g2.get() == NULL )
	  {
		  res= e2->AddInvSNew( e1 ); // scalar+scalar or array+scalar
	  }
	  else
	  {
		  g2.release();
		  res= e2->AddInvS( e1 ); // scalar+scalar or array+scalar
	  }
  }
  else if ( e2->StrictScalar() )
  {
	  if ( g1.get() == NULL )
		  res= e1->AddSNew( e2); // array+scalar
	  else
	  {
		  g1.release();
		  res= e1->AddS( e2); // array+scalar
	  }
  }
  else if ( e1->N_Elements() == e2->N_Elements() )
  {
	  if ( g1.get() != NULL )
	  {
		  g1.release();
		  return e1->Add ( e2 );
	  }
	  if ( g2.get() != NULL )
	  {
		  g2.release();
		  res = e2->AddInv ( e1 );
		  res->SetDim ( e1->Dim() );
		  return res;
	  }
	  else
	  {
		  return e1->AddNew ( e2 );
	  }
  }
  else if ( e1->N_Elements() < e2->N_Elements() )
  {
	  if ( g1.get() == NULL )
		  res= e1->AddNew ( e2 ); // smaller_array + larger_array or same size
	    else
	    {
		  g1.release();
		  res= e1->Add ( e2 ); // smaller_array + larger_array or same size
	  }
  }
  else // e1->N_Elements() > e2->N_Elements() )
  {
	  if ( g2.get() == NULL )
		  res= e2->AddInvNew( e1); // smaller + larger
	  else
	  {
		  g2.release();
		  res= e2->AddInv( e1); // smaller + larger
	  }
  }
  return res;
}
BaseGDL* PLUSNCNode::Eval()
{
  BaseGDL* res;
  Guard<BaseGDL> g1;
  Guard<BaseGDL> g2;
  BaseGDL *e1, *e2; 
  SetupGuards( g1, e1, g2, e2 );
  
  DType aTy=e1->Type();
  DType bTy=e2->Type();
  if( aTy == bTy) 
  {
    if( aTy == GDL_OBJ) // we MUST do this here (correct guard handling)
      return e1->Add( e2);

    // otherwise continue below
  }
  else // aTy != bTy
  {
    // Change > to >= JMG
    if( DTypeOrder[aTy] >= DTypeOrder[bTy])
    {
      // GDL_COMPLEX op GDL_DOUBLE = GDL_COMPLEXDBL
      if( (aTy == GDL_COMPLEX && bTy == GDL_DOUBLE))
	{
	  e2 = e2->Convert2( GDL_COMPLEXDBL, BaseGDL::COPY);
	  g2.Reset( e2); // delete former e2
	  e1 = e1->Convert2( GDL_COMPLEXDBL, BaseGDL::COPY);
	  g1.Reset( e1); // delete former e1
	}
      else if( aTy == GDL_OBJ) // only check for aTy is ok because GDL_OBJ has highest order
	return e1->Add(e2); // for operator overloading, do not convert other type then
      else
      {
	// convert e2 to e1
	e2 = e2->Convert2( aTy, BaseGDL::COPY);
	g2.Reset( e2); // delete former e2
      }
    }
    else
    {
      // GDL_COMPLEX op GDL_DOUBLE = GDL_COMPLEXDBL
      if( (bTy == GDL_COMPLEX && aTy == GDL_DOUBLE))
	{
	  e2 = e2->Convert2( GDL_COMPLEXDBL, BaseGDL::COPY);
	  g2.Reset( e2); // delete former e2
	  e1 = e1->Convert2( GDL_COMPLEXDBL, BaseGDL::COPY);
	  g1.Reset( e1); // delete former e1
	}
      else if( bTy == GDL_OBJ) // only check for bTy is ok because GDL_OBJ has highest order
	return e2->AddInv( e1); // for operator overloading, do not convert other type then
      else
      {
	// convert e1 to e2
	e1 = e1->Convert2( bTy, BaseGDL::COPY);
	g1.Reset( e1); // delete former e1
      }
    }
  } // aTy != bTy
  
  if ( e1->StrictScalar() )
  {
    if ( g2.IsNull() )
    {
	    res= e2->AddInvSNew( e1 ); // scalar+scalar or array+scalar
    }
    else
    {
	    g2.Release();
	    res= e2->AddInvS( e1 ); // scalar+scalar or array+scalar
    }
  }
  else if ( e2->StrictScalar() )
  {
    if ( g1.IsNull() )
	    res= e1->AddSNew( e2); // array+scalar
    else
    {
	    g1.Release();
	    res= e1->AddS( e2); // array+scalar
    }
  }
  else if ( e1->N_Elements() == e2->N_Elements() )
  {
    if ( !g1.IsNull() )
    {
	    g1.Release();
	    return e1->Add ( e2 );
    }
    if ( !g2.IsNull() )
    {
	    g2.Release();
	    res = e2->AddInv ( e1 );
	    res->SetDim ( e1->Dim() );
	    return res;
    }
    else
    {
	    return e1->AddNew ( e2 );
    }
  }
  else if ( e1->N_Elements() < e2->N_Elements() )
  {
    if ( g1.IsNull() )
	    res= e1->AddNew ( e2 ); // smaller_array + larger_array or same size
      else
      {
	    g1.Release();
	    res= e1->Add ( e2 ); // smaller_array + larger_array or same size
    }
  }
  else // e1->N_Elements() > e2->N_Elements() )
  {
    if ( g2.IsNull() )
	    res= e2->AddInvNew( e1); // smaller + larger
    else
    {
	    g2.Release();
	    res= e2->AddInv( e1); // smaller + larger
    }
  }
  return res;
}


BaseGDL* MINUSNC12Node::Eval()
{ 
  BaseGDL *e1 = op1->EvalNC();
  BaseGDL *e2 = op2->EvalNC();
  DType aTy=e1->Type();
  DType bTy=e2->Type();
  if( aTy == bTy)
  {
    // as all Sub...New() functions are implemented for GDL_OBJ, we can save the if(...) here
    // (as there are no guards up to here)
    //     if( aTy == GDL_OBJ) // this saves us to implement all Add...New() functions
    //       return e1->Add( e2);
    
    if ( e1->StrictScalar() )
    {
      return e2->SubInvSNew( e1 ); // scalar+scalar or array+scalar
    }
    else if ( e2->StrictScalar() )
    {
      return e1->SubSNew( e2); // array+scalar
    }
    else if ( e1->N_Elements() <= e2->N_Elements() )
    {
      return e1->SubNew ( e2 );
    }
    else // e1->N_Elements() > e2->N_Elements() )
    {
      return e2->SubInvNew( e1); // smaller + larger
    }
  }

  Guard<BaseGDL> g1;
  Guard<BaseGDL> g2;
  if( DTypeOrder[aTy] >= DTypeOrder[bTy])
    {
      if( aTy == GDL_OBJ)
	return e1->Sub( e2);

      // GDL_COMPLEX op GDL_DOUBLE = GDL_COMPLEXDBL
      if( aTy == GDL_COMPLEX && bTy == GDL_DOUBLE)
      {
	  e2 = e2->Convert2( GDL_COMPLEXDBL, BaseGDL::COPY);
	  g2.reset( e2);
	  e1 = e1->Convert2( GDL_COMPLEXDBL, BaseGDL::COPY);
	  g1.reset( e1);
      }
      else
      {
	// convert e2 to e1
	e2 = e2->Convert2( aTy, BaseGDL::COPY);
	g2.reset( e2);
      }
    }
  else
    {
      if( bTy == GDL_OBJ)
	return e2->SubInv( e1);
      
      // GDL_COMPLEX op GDL_DOUBLE = GDL_COMPLEXDBL
      if( (bTy == GDL_COMPLEX && aTy == GDL_DOUBLE))
	{
	  e2 = e2->Convert2( GDL_COMPLEXDBL, BaseGDL::COPY);
	  g2.reset( e2);
	  e1 = e1->Convert2( GDL_COMPLEXDBL, BaseGDL::COPY);
	  g1.reset( e1);
	}
      else
	{// convert e1 to e2
	    e1 = e1->Convert2( bTy, BaseGDL::COPY);
	    g1.reset( e1);
	}
    }
    
  // 'classic handling from here
  BaseGDL* res;
  if( e1->StrictScalar())
    {
      if( g2.get() == NULL) return e2->SubInvSNew( e1); else g2.release();
      res= e2->SubInvS(e1); // scalar+scalar or array+scalar
      
    }
  else
    if( e2->StrictScalar())
      {
	if( g1.get() == NULL) return e1->SubSNew( e2); else g1.release();
	res= e1->SubS(e2); // array+scalar
	
      }
    else if( e1->N_Elements() == e2->N_Elements())
    {
      if( g1.get() != NULL)
	{
	  g1.release();
	  return e1->Sub(e2);
	}
      if( g2.get() != NULL) 
	{
	  g2.release();
	  res = e2->SubInv(e1);
	  res->SetDim( e1->Dim());
	  return res;
	}
      else
	{
	return e1->SubNew(e2); 
	}
    }
  else if( e1->N_Elements() < e2->N_Elements())
	{
	  if( g1.get() == NULL) return e1->SubNew( e2); else g1.release();
	  res= e1->Sub(e2); // smaller_array + larger_array or same size
	  
	}
      else
	{
	  if( g2.get() == NULL) return e2->SubInvNew( e1); else g2.release();
	  res= e2->SubInv(e1); // smaller + larger
	  
	}
  return res;
}
BaseGDL* MINUSNCNode::Eval()
{ 
 BaseGDL* res;
 Guard<BaseGDL> g1;
 Guard<BaseGDL> g2;
 BaseGDL *e1, *e2; 
//  AdjustTypesNC( g1, e1, g2, e2); 

  SetupGuards( g1, e1, g2, e2 );
  
  DType aTy=e1->Type();
  DType bTy=e2->Type();
  if( aTy == bTy) 
  {
    if( aTy == GDL_OBJ) // we MUST do this here (correct guard handling)
      return e1->Sub( e2);

    // otherwise continue below
  }
  else // aTy != bTy
  {
    // Change > to >= JMG
    if( DTypeOrder[aTy] >= DTypeOrder[bTy])
    {
      // GDL_COMPLEX op GDL_DOUBLE = GDL_COMPLEXDBL
      if( (aTy == GDL_COMPLEX && bTy == GDL_DOUBLE))
	{
	  e2 = e2->Convert2( GDL_COMPLEXDBL, BaseGDL::COPY);
	  g2.Reset( e2); // delete former e2
	  e1 = e1->Convert2( GDL_COMPLEXDBL, BaseGDL::COPY);
	  g1.Reset( e1); // delete former e1
	}
      else if( aTy == GDL_OBJ) // only check for aTy is ok because GDL_OBJ has highest order
	return e1->Sub(e2); // for operator overloading, do not convert other type then
      else
      {
	// convert e2 to e1
	e2 = e2->Convert2( aTy, BaseGDL::COPY);
	g2.Reset( e2); // delete former e2
      }
    }
    else
    {
      // GDL_COMPLEX op GDL_DOUBLE = GDL_COMPLEXDBL
      if( (bTy == GDL_COMPLEX && aTy == GDL_DOUBLE))
	{
	  e2 = e2->Convert2( GDL_COMPLEXDBL, BaseGDL::COPY);
	  g2.Reset( e2); // delete former e2
	  e1 = e1->Convert2( GDL_COMPLEXDBL, BaseGDL::COPY);
	  g1.Reset( e1); // delete former e1
	}
      else if( bTy == GDL_OBJ) // only check for bTy is ok because GDL_OBJ has highest order
	return e2->SubInv( e1); // for operator overloading, do not convert other type then
      else
      {
	// convert e1 to e2
	e1 = e1->Convert2( bTy, BaseGDL::COPY);
	g1.Reset( e1); // delete former e1
      }
    }
  } // aTy != bTy
 
 if( e1->StrictScalar())
   {
     if( g2.Get() == NULL) return e2->SubInvSNew( e1); else g2.Release();
     res= e2->SubInvS(e1); // scalar+scalar or array+scalar
     
   }
 else
   if( e2->StrictScalar())
     {
       if( g1.Get() == NULL) return e1->SubSNew( e2); else g1.Release();
       res= e1->SubS(e2); // array+scalar
       
     }
   else if( e1->N_Elements() == e2->N_Elements())
   {
     if( g1.Get() != NULL)
       {
	 g1.Release();
	 return e1->Sub(e2);
       }
     if( g2.Get() != NULL) 
       {
	 g2.Release();
	 res = e2->SubInv(e1);
	 res->SetDim( e1->Dim());
	 return res;
       }
     else
       {
       return e1->SubNew(e2); 
       }
   }
else if( e1->N_Elements() < e2->N_Elements())
       {
	 if( g1.Get() == NULL) return e1->SubNew( e2); else g1.Release();
	 res= e1->Sub(e2); // smaller_array + larger_array or same size
	 
       }
     else
       {
	 if( g2.Get() == NULL) return e2->SubInvNew( e1); else g2.Release();
	 res= e2->SubInv(e1); // smaller + larger
	 
       }
 return res;
}
BaseGDL* LTMARKNCNode::Eval()
{ BaseGDL* res;
 Guard<BaseGDL> g1;
 Guard<BaseGDL> g2;
 BaseGDL *e1, *e2; AdjustTypesNC( g1, e1, g2, e2); 

 if( e1->StrictScalar())
   {
     if( g2.get() == NULL) return e2->LtMarkSNew( e1); else g2.release();
     res= e2->LtMarkS(e1); // scalar+scalar or array+scalar
     
   }
 else
   if( e2->StrictScalar())
     {
       if( g1.get() == NULL) return e1->LtMarkSNew( e2); else g1.release();
       res= e1->LtMarkS(e2); // array+scalar
       
     }
   else if( e1->N_Elements() == e2->N_Elements())
   {
     if( g1.get() != NULL)
       {
	 g1.release();
	 return e1->LtMark(e2);
       }
     if( g2.get() != NULL) 
       {
	 g2.release();
	 res = e2->LtMark(e1);
	 res->SetDim( e1->Dim());
	 return res;
       }
     else
       {
       return e1->LtMarkNew(e2);
       }
   }
   else if( e1->N_Elements() < e2->N_Elements())
       {
	 if( g1.get() == NULL) return e1->LtMarkNew( e2); else g1.release();
	 res= e1->LtMark(e2); // smaller_array + larger_array or same size
	 
       }
     else
       {
	 if( g2.get() == NULL) return e2->LtMarkNew( e1); else g2.release();
	 res= e2->LtMark(e1); // smaller + larger
	 
       }
 return res;
}
BaseGDL* GTMARKNCNode::Eval()
{ BaseGDL* res;
 Guard<BaseGDL> g1;
 Guard<BaseGDL> g2;
 BaseGDL *e1, *e2; AdjustTypesNC( g1, e1, g2, e2); 

 if( e1->StrictScalar())
   {
     if( g2.get() == NULL) return e2->GtMarkSNew( e1); else g2.release();
     res= e2->GtMarkS(e1); // scalar+scalar or array+scalar
     
   }
 else
   if( e2->StrictScalar())
     {
       if( g1.get() == NULL) return e1->GtMarkSNew( e2); else g1.release();
       res= e1->GtMarkS(e2); // array+scalar
       
     }
   else if( e1->N_Elements() == e2->N_Elements())
   {
     if( g1.get() != NULL)
       {
	 g1.release();
	 return e1->GtMark(e2);
       }
     if( g2.get() != NULL) 
       {
	 g2.release();
	 res = e2->GtMark(e1);
	 res->SetDim( e1->Dim());
	 return res;
       }
     else
       {
       return e1->GtMarkNew(e2);
       }
   }
else if( e1->N_Elements() < e2->N_Elements())
       {
	 if( g1.get() == NULL) return e1->GtMarkNew( e2); else g1.release();
	 res= e1->GtMark(e2); // smaller_array + larger_array or same size
	 
       }
     else
       {
	 if( g2.get() == NULL) return e2->GtMarkNew( e1); else g2.release();
	 res= e2->GtMark(e1); // smaller + larger
	 
       }
 return res;
}

BaseGDL* ASTERIXNC12Node::Eval()
{
  BaseGDL *e1 = op1->EvalNC();
  BaseGDL *e2 = op2->EvalNC();
  DType aTy=e1->Type();
  DType bTy=e2->Type();
  if( aTy == bTy)
  {
    if ( e1->StrictScalar() )
    {
      return e2->MultSNew( e1 ); // scalar+scalar or array+scalar
    }
    else if ( e2->StrictScalar() )
    {
      return e1->MultSNew( e2); // array+scalar
    }
    else if ( e1->N_Elements() <= e2->N_Elements() )
    {
      return e1->MultNew ( e2 );
    }
    else // e1->N_Elements() > e2->N_Elements() )
    {
      return e2->MultNew( e1); // smaller + larger
    }
  }
  
  Guard<BaseGDL> g1;
  Guard<BaseGDL> g2;
  if( DTypeOrder[aTy] >= DTypeOrder[bTy])
    {
      // GDL_COMPLEX op GDL_DOUBLE = GDL_COMPLEXDBL
      if( aTy == GDL_COMPLEX && bTy == GDL_DOUBLE)
      {
	  e2 = e2->Convert2( GDL_COMPLEXDBL, BaseGDL::COPY);
	  g2.reset( e2);
	  e1 = e1->Convert2( GDL_COMPLEXDBL, BaseGDL::COPY);
	  g1.reset( e1);
      }
      else
      {
	// convert e2 to e1
	e2 = e2->Convert2( aTy, BaseGDL::COPY);
	g2.reset( e2);
      }
    }
  else
    {
      // GDL_COMPLEX op GDL_DOUBLE = GDL_COMPLEXDBL
      if( (bTy == GDL_COMPLEX && aTy == GDL_DOUBLE))
	{
	  e2 = e2->Convert2( GDL_COMPLEXDBL, BaseGDL::COPY);
	  g2.reset( e2);
	  e1 = e1->Convert2( GDL_COMPLEXDBL, BaseGDL::COPY);
	  g1.reset( e1);
	}
      else
	{// convert e1 to e2
	    e1 = e1->Convert2( bTy, BaseGDL::COPY);
	    g1.reset( e1);
	}
    }
    
  // 'classic handling from here
  BaseGDL* res;
  if ( e1->StrictScalar() )
  {
	  if ( g2.get() == NULL )
	  {
		  res= e2->MultSNew( e1); // scalar+scalar or array+scalar
	  }
	  else
	  {
		  g2.release();
		  res= e2->MultS( e1); // scalar+scalar or array+scalar
	  }
  }
  else	if ( e2->StrictScalar() )
  {
	  if ( g1.get() == NULL )
	  {
// 			return e1->New( e2);
		  res= e1->MultSNew( e2); // array+scalar
	  }
	  else
	  {
		  g1.release();
		  res= e1->MultS( e2); // array+scalar
	  }
  }
  else if ( e1->N_Elements() == e2->N_Elements() )
  {
	  if ( g1.get() != NULL )
	  {
		  g1.release();
		  return e1->Mult( e2);
	  }
	  else if ( g2.get() != NULL )
	  {
		  g2.release();
		  res = e2->Mult( e1);
		  res->SetDim( e1->Dim());
		  return res;
	  }
	  else
	  {
		  return e1->MultNew( e2);
// 			return e1->Dup()->Mult ( e2 );
	  }
  }
  else if ( e1->N_Elements() < e2->N_Elements() )
  {
	  if ( g1.get() == NULL )
	  {
		  // 		 return e1->New( e2);
		  res= e1->MultNew( e2); // smaller_array + larger_array or same size
	    }
	  else
	  {
		  g1.release();
		  res= e1->Mult( e2); // smaller_array + larger_array or same size
	  }
  }
  else
  {
	  if ( g2.get() == NULL )
	  {
		  res = e2->MultNew( e1); // smaller + larger
	  }
	    else
	  {
		  g2.release();
		  res= e2->Mult( e1); // smaller + larger
	  }
  }
  return res;
}
BaseGDL* ASTERIXNCNode::Eval()
{
	BaseGDL* res;
	Guard<BaseGDL> g1;
	Guard<BaseGDL> g2;
	BaseGDL *e1, *e2; AdjustTypesNC ( g1, e1, g2, e2 );

	if ( e1->StrictScalar() )
	{
		if ( g2.get() == NULL )
		{
			res= e2->MultSNew( e1); // scalar+scalar or array+scalar
		}
		else
		{
			g2.release();
			res= e2->MultS( e1); // scalar+scalar or array+scalar
		}
	}
	else	if ( e2->StrictScalar() )
	{
		if ( g1.get() == NULL )
		{
// 			return e1->New( e2);
			res= e1->MultSNew( e2); // array+scalar
		}
		else
		{
			g1.release();
			res= e1->MultS( e2); // array+scalar
		}
	}
	else if ( e1->N_Elements() == e2->N_Elements() )
	{
		if ( g1.get() != NULL )
		{
			g1.release();
			return e1->Mult( e2);
		}
		else if ( g2.get() != NULL )
		{
			g2.release();
			res = e2->Mult( e1);
			res->SetDim( e1->Dim());
			return res;
		}
		else
		{
			return e1->MultNew( e2);
// 			return e1->Dup()->Mult ( e2 );
		}
	}
	else if ( e1->N_Elements() < e2->N_Elements() )
	{
		if ( g1.get() == NULL )
		{
			// 		 return e1->New( e2);
			res= e1->MultNew( e2); // smaller_array + larger_array or same size
		 }
		else
		{
			g1.release();
			res= e1->Mult( e2); // smaller_array + larger_array or same size
		}
	}
	else
	{
		if ( g2.get() == NULL )
		{
			res = e2->MultNew( e1); // smaller + larger
		}
		 else
		{
			g2.release();
			res= e2->Mult( e1); // smaller + larger
		}
	}
	return res;
}

BaseGDL* MATRIX_OP1NCNode::Eval()
{ BaseGDL* res;
 Guard<BaseGDL> g1;
 Guard<BaseGDL> g2;
 BaseGDL *e1, *e2;
 if( op1NC)
   {
     e1 = op1->EvalNC();
   }
 else
   {
     e1 = op1->Eval();
     g1.reset( e1);
   }
 if( op2NC)
   {
     e2 = op2->EvalNC();
   }
 else
   {
     e2 = op2->Eval();
     g2.reset( e2);
   }
 DType aTy=e1->Type();
 DType bTy=e2->Type();
 DType maxTy=(DTypeOrder[aTy] >= DTypeOrder[bTy])? aTy: bTy;
 if( maxTy > 100)
   {
     throw GDLException( "Expressions of this type cannot be converted.");
   }

 DType cTy=maxTy;
 if( maxTy == GDL_BYTE || maxTy == GDL_INT)
   cTy=GDL_LONG;
 else if( maxTy == GDL_UINT)
   cTy=GDL_ULONG;

 if( aTy != cTy)
   {
     e1 = e1->Convert2( cTy, BaseGDL::COPY);
     g1.reset( e1);
   }
 if( bTy != cTy)
   {
     e2 = e2->Convert2( cTy, BaseGDL::COPY);
     g2.reset( e2);
   }
 
 res=e1->MatrixOp(e2);
 return res;
}
BaseGDL* MATRIX_OP2NCNode::Eval()
{
 BaseGDL* res;
 Guard<BaseGDL> g1;
 Guard<BaseGDL> g2;
 BaseGDL *e1, *e2;
 if( op1NC)
   {
     e1 = op1->EvalNC();
   }
 else
   {
     e1 = op1->Eval();
     g1.reset( e1);
   }
 if( op2NC)
   {
     e2 = op2->EvalNC();
   }
 else
   {
     e2 = op2->Eval();
     g2.reset( e2);
   }
 DType aTy=e1->Type();
 DType bTy=e2->Type();
 DType maxTy=(DTypeOrder[aTy] >= DTypeOrder[bTy])? aTy: bTy;
 if( maxTy > 100)
   {
     throw GDLException( "Expressions of this type cannot be converted.");
   }

 DType cTy=maxTy;
 if( maxTy == GDL_BYTE || maxTy == GDL_INT)
   cTy=GDL_LONG;
 else if( maxTy == GDL_UINT)
   cTy=GDL_ULONG;

 if( aTy != cTy)
   {
     e1 = e1->Convert2( cTy, BaseGDL::COPY);
     g1.reset( e1);
   }
 if( bTy != cTy)
   {
     e2 = e2->Convert2( cTy, BaseGDL::COPY);
     g2.reset( e2);
   }

 res=e2->MatrixOp(e1);
 return res;
}

BaseGDL* SLASHNC12Node::Eval()
{
  BaseGDL *e1 = op1->EvalNC();
  BaseGDL *e2 = op2->EvalNC();
  DType aTy=e1->Type();
  DType bTy=e2->Type();
  if( aTy == bTy)
  {
    if ( e1->StrictScalar() )
    {
      return e2->DivInvSNew( e1 ); // scalar+scalar or array+scalar
    }
    else if ( e2->StrictScalar() )
    {
      return e1->DivSNew( e2); // array+scalar
    }
    else if ( e1->N_Elements() <= e2->N_Elements() )
    {
      return e1->DivNew ( e2 );
    }
    else // e1->N_Elements() > e2->N_Elements() )
    {
      return e2->DivInvNew( e1); // smaller + larger
    }
  }

  Guard<BaseGDL> g1;
  Guard<BaseGDL> g2;
  if( DTypeOrder[aTy] >= DTypeOrder[bTy])
    {
      // GDL_COMPLEX op GDL_DOUBLE = GDL_COMPLEXDBL
      if( aTy == GDL_COMPLEX && bTy == GDL_DOUBLE)
      {
	  e2 = e2->Convert2( GDL_COMPLEXDBL, BaseGDL::COPY);
	  g2.reset( e2);
	  e1 = e1->Convert2( GDL_COMPLEXDBL, BaseGDL::COPY);
	  g1.reset( e1);
      }
      else
      {
	// convert e2 to e1
	e2 = e2->Convert2( aTy, BaseGDL::COPY);
	g2.reset( e2);
      }
    }
  else
    {
      // GDL_COMPLEX op GDL_DOUBLE = GDL_COMPLEXDBL
      if( (bTy == GDL_COMPLEX && aTy == GDL_DOUBLE))
	{
	  e2 = e2->Convert2( GDL_COMPLEXDBL, BaseGDL::COPY);
	  g2.reset( e2);
	  e1 = e1->Convert2( GDL_COMPLEXDBL, BaseGDL::COPY);
	  g1.reset( e1);
	}
      else
	{// convert e1 to e2
	    e1 = e1->Convert2( bTy, BaseGDL::COPY);
	    g1.reset( e1);
	}
    }
    
 // 'classic handling from here
 BaseGDL* res;
 if( e1->StrictScalar())
   {
     if( g2.get() == NULL) return e2->DivInvSNew( e1); else g2.release();
     res= e2->DivInvS(e1); // scalar+scalar or array+scalar
     
   }
 else
   if( e2->StrictScalar())
     {
       if( g1.get() == NULL) return e1->DivSNew( e2); else g1.release();
       res= e1->DivS(e2); // array+scalar
       
     }
   else if( e1->N_Elements() == e2->N_Elements())
   {
     if( g1.get() != NULL)
       {
	 g1.release();
	 return e1->Div(e2);
       }
     if( g2.get() != NULL) 
       {
	 g2.release();
	 res = e2->DivInv(e1);
	 res->SetDim( e1->Dim());
	 return res;
       }
     else
       {
       return e1->DivNew(e2); 
       }
   }
else if( e1->N_Elements() < e2->N_Elements())
       {
	 if( g1.get() == NULL) return e1->DivNew( e2); else g1.release();
	 res= e1->Div(e2); // smaller_array + larger_array or same size
	 
       }
     else
       {
	 if( g2.get() == NULL) return e2->DivInvNew( e1); else g2.release();
	 res= e2->DivInv(e1); // smaller + larger
	 
       }

 return res;
}
BaseGDL* SLASHNCNode::Eval()
{ BaseGDL* res;
 Guard<BaseGDL> g1;
 Guard<BaseGDL> g2;
 BaseGDL *e1, *e2; AdjustTypesNC( g1, e1, g2, e2); 

 if( e1->StrictScalar())
   {
     if( g2.get() == NULL) return e2->DivInvSNew( e1); else g2.release();
     res= e2->DivInvS(e1); // scalar+scalar or array+scalar
     
   }
 else
   if( e2->StrictScalar())
     {
       if( g1.get() == NULL) return e1->DivSNew( e2); else g1.release();
       res= e1->DivS(e2); // array+scalar
       
     }
   else if( e1->N_Elements() == e2->N_Elements())
   {
     if( g1.get() != NULL)
       {
	 g1.release();
	 return e1->Div(e2);
       }
     if( g2.get() != NULL) 
       {
	 g2.release();
	 res = e2->DivInv(e1);
	 res->SetDim( e1->Dim());
	 return res;
       }
     else
       {
       return e1->DivNew(e2); 
       }
   }
else if( e1->N_Elements() < e2->N_Elements())
       {
	 if( g1.get() == NULL) return e1->DivNew( e2); else g1.release();
	 res= e1->Div(e2); // smaller_array + larger_array or same size
	 
       }
     else
       {
	 if( g2.get() == NULL) return e2->DivInvNew( e1); else g2.release();
	 res= e2->DivInv(e1); // smaller + larger
	 
       }

 return res;
}
BaseGDL* MOD_OPNCNode::Eval()
{ BaseGDL* res;
 Guard<BaseGDL> g1;
 Guard<BaseGDL> g2;
 BaseGDL *e1, *e2; AdjustTypesNC( g1, e1, g2, e2); 

 if( e1->StrictScalar())
   {
     if( g2.get() == NULL) return e2->ModInvSNew( e1); else g2.release();
     res= e2->ModInvS(e1); // scalar+scalar or array+scalar
     
   }
 else if( e2->StrictScalar())
     {
       if( g1.get() == NULL) return e1->ModSNew( e2); else g1.release();
       res= e1->ModS(e2); // array+scalar
       
     }
   else if( e1->N_Elements() == e2->N_Elements())
   {
     if( g1.get() != NULL)
       {
	 g1.release();
	 return e1->Mod(e2);
       }
     if( g2.get() != NULL) 
       {
	 g2.release();
	 res = e2->ModInv(e1);
	 res->SetDim( e1->Dim());
	 return res;
       }
     else
       {
       return e1->ModNew(e2); 
       }
   }
else if( e1->N_Elements() < e2->N_Elements())
       {
	 if( g1.get() == NULL) return e1->ModNew( e2); else g1.release();
	 res= e1->Mod(e2); // smaller_array + larger_array or same size
	 
       }
     else
       {
	 if( g2.get() == NULL) return e2->ModInvNew( e1); else g2.release();
	 res= e2->ModInv(e1); // smaller + larger
	 
       }
 return res;
}


BaseGDL* POWNCNode::Eval()
{
  BaseGDL* res;
  Guard<BaseGDL> g1;
  Guard<BaseGDL> g2;
  BaseGDL *e1, *e2;
  if( op1NC)
    {
      e1 = op1->EvalNC();
    }
  else
    {
      e1 = op1->Eval();
      g1.reset( e1);
    }
  if( op2NC)
    {
      e2 = op2->EvalNC();
    }
  else
    {
      e2 = op2->Eval();
      g2.reset( e2);
    }

  DType aTy=e1->Type();
  DType bTy=e2->Type();

  if( aTy == GDL_STRING)
    {
      e1 = e1->Convert2( GDL_FLOAT, BaseGDL::COPY);
      g1.reset( e1); // deletes old e1
      aTy = GDL_FLOAT;
    }
  if( bTy == GDL_STRING)
    {
      e2 = e2->Convert2( GDL_FLOAT, BaseGDL::COPY);
      g2.reset( e2); // deletes old e2
      bTy = GDL_FLOAT;
    }

  if( ComplexType(aTy))
    {
      if( IntType( bTy))
	{
	  if( bTy != GDL_LONG)
	    {
	      e2 = e2->Convert2( GDL_LONG, BaseGDL::COPY);
	      g2.reset( e2);
	    }
	  if( g1.get() == NULL) 
	    {
	      return e1->PowNew( e2);
	    }
	  else 
	    {
	      res = g1->Pow( e2);
	      if( res == g1.get())
		g1.release();
	      return res;
	    }
	}
      if( aTy == GDL_COMPLEX)
	{
	  if( bTy == GDL_DOUBLE)
	    {
	      e1 = e1->Convert2( GDL_COMPLEXDBL, BaseGDL::COPY);
	      g1.reset( e1); 
	      aTy = GDL_COMPLEXDBL;
	    }
	  else if( bTy == GDL_FLOAT)
	    {
	      if( g1.get() == NULL) 
		{
		  return e1->PowNew( e2);
		}
	      else 
		{
		  res = g1->Pow( e2);
		  if( res == g1.get())
		    g1.release();
		  return res;
		}
	    }
	}
      if( aTy == GDL_COMPLEXDBL)
	{
	  if( bTy == GDL_FLOAT)
	    {
	      e2 = e2->Convert2( GDL_DOUBLE, BaseGDL::COPY);
	      g2.reset( e2);
	      bTy = GDL_DOUBLE;
	    }
	  if( bTy == GDL_DOUBLE)
	    {
	      if( g1.get() == NULL) 
		{
		  return e1->PowNew( e2);
		}
	      else
		{
		  res = g1->Pow( e2);
		  if( res == g1.get())
		    g1.release();
		  return res;
		}
	    }
	}
    }

  if( IntType( bTy) && FloatType( aTy))
    {
      if( bTy != GDL_LONG)
	{
	  e2 = e2->Convert2( GDL_LONG, BaseGDL::COPY);
	  g2.reset( e2);
	}
      if( g1.get() == NULL)
	res = e1->PowIntNew( e2);
      else 
	{
	  res = g1->PowInt( e2);
	  if( res == g1.get())
		g1.release();
	}
      return res;
    }

  DType convertBackT; 
 
  // convert back
  if( IntType( bTy) && (DTypeOrder[ bTy] > DTypeOrder[ aTy]))
    convertBackT = aTy;
  else
    convertBackT = GDL_UNDEF;

  if( aTy != bTy) 
    {
      if( aTy > 100 || bTy > 100)
	{
	  throw GDLException( "Expressions of this type cannot be converted.");
	}

      if( DTypeOrder[aTy] >= DTypeOrder[bTy]) // crucial: '>' -> '>='
	{
	  // convert e2 to e1
	  e2 = e2->Convert2( aTy, BaseGDL::COPY);
	  g2.reset( e2); // delete former e2
	}
      else
	{
	  // convert e1 to e2
	  e1 = e1->Convert2( bTy, BaseGDL::COPY);
	  g1.reset( e1); // delete former e1
	}
    }

  // AdjustTypes(e2,e1); // order crucial here (for converting back)
if( e1->StrictScalar())
    {
	if( g2.get() == NULL)
		res = e2->PowInvSNew( e1);
	else
	{
		g2.release();
		res = e2->PowInvS(e1); // scalar+scalar or array+scalar
	}
    }
  else if( e2->StrictScalar())
    {
      if( g1.get() == NULL) 
	{
	  res = e1->PowSNew( e2);
// 	  res = e1->PowS(e2); // array+scalar
	}
      else 
	{
	  g1.release();
	  res= e1->PowS(e2); // array+scalar
	}
    }
  else if( e1->N_Elements() == e2->N_Elements())
    {
      if( g1.get() != NULL)
	{
	  g1.release();
	  res = e1->Pow(e2);
	}
      else if( g2.get() != NULL) 
	{
	  g2.release();
	  res = e2->PowInv(e1);
	  res->SetDim( e1->Dim());
	}
      else
	{
	res = e1->PowNew( e2);
// 	  res = e1->Pow(e2); 
	}
    }
  else if( e1->N_Elements() < e2->N_Elements())
      {
	if( g1.get() == NULL) 
	  {
	    res = e1->PowNew( e2);
// 	    res= e1->Pow(e2); // smaller_array + larger_array or same size
	  }
	else 
	  {
	    g1.release();
	    res= e1->Pow(e2); // smaller_array + larger_array or same size
	  }
      }
    else
      {
	if( g2.get() == NULL)
		res = e2->PowInvNew( e1);
	else
	{
		g2.release();
		res= e2->PowInv(e1); // smaller + larger
	}	 
      }
  if( convertBackT != GDL_UNDEF)
    {
      res = res->Convert2( convertBackT, BaseGDL::CONVERT);
    }
  return res;
}
// BaseGDL* DECNCNode::Eval()
// { BaseGDL* res;
//   return new DECNode( refNode);
// }
// BaseGDL* INCNCNode::Eval()
// { BaseGDL* res;
//   return new INCNode( refNode);
// }
// BaseGDL* POSTDECNCNode::Eval()
// { BaseGDL* res;
//   return new POSTDECNode( refNode);
// }
// BaseGDL* POSTINCNCNode::Eval()
// { BaseGDL* res;
//   return new POSTINCNode( refNode);
// }

  BaseGDL** FCALL_LIB_N_ELEMENTSNode::LEval()
  {
      // better than auto_ptr: auto_ptr wouldn't remove newEnv from the stack
    StackGuard<EnvStackT> guard(ProgNode::interpreter->CallStack());
    throw GDLException(this,"Internal error: N_ELEMENTS called as left expr.");
  }

  BaseGDL* FCALL_LIB_N_ELEMENTSNode::Eval()
  {
    try
      {
	BaseGDL* param;
	bool isReference = 
	  static_cast<ParameterNode*>(this->getFirstChild())->ParameterDirect( param);
	Guard<BaseGDL> guard;
	if( !isReference)
	  guard.Reset( param);

	if( param == NULL)
	  return new DLongGDL( 0);
	
	return new DLongGDL( param->N_Elements());   
      } 
    catch( GDLException& e)
      {
	// an error occured -> parameter is undefined 
	return new DLongGDL( 0);
      }
  }

  BaseGDL** FCALL_LIB_RETNEWNode::LEval()
  {
    throw GDLException(this,"Internal error: FCALL_LIB_RETNEW as left expr.");
  }

  BaseGDL* FCALL_LIB_RETNEWNode::Eval()
  {
// 	match(antlr::RefAST(_t),FCALL_LIB_RETNEW);
//	_t = _t->getFirstChild();
// 	match(antlr::RefAST(_t),IDENTIFIER);
    EnvT* newEnv=new EnvT( this, this->libFun);//libFunList[fl->funIx]);
//     Guard< EnvT> guardEnv( newEnv);
// 	_t =_t->getFirstChild();
// 	EnvT* newEnv=new EnvT( fl, fl->libFun);//libFunList[fl->funIx]);
	// special handling for N_ELEMENTS()
//     static int n_elementsIx = LibFunIx("N_ELEMENTS");
//     static DLibFun* n_elementsFun = libFunList[n_elementsIx];
// 
//     if( this->libFun == n_elementsFun)
//         {
//             ProgNode::interpreter->parameter_def_n_elements(this->getFirstChild(), newEnv);
//         }
//     else
//         {
            ProgNode::interpreter->parameter_def_nocheck(this->getFirstChild(), newEnv);
//         }
    // push id.pro onto call stack
// 	guardEnv.release();
	Guard<EnvT> guardEnv( newEnv);
//     // better than auto_ptr: auto_ptr wouldn't remove newEnv from the stack
//     StackGuard<EnvStackT> guard(ProgNode::interpreter->CallStack());
//     ProgNode::interpreter->CallStack().push_back(newEnv);
    // make the call
    //*** MUST always return a defined expression
    BaseGDL* res = static_cast<DLibFun*>(newEnv->GetPro())->Fun()(newEnv);
    assert( res != NULL);
    return res;
  }

  BaseGDL** FCALL_LIB_DIRECTNode::LEval()
  {
    throw GDLException(this,"Internal error: FCALL_LIB_DIRECTNode as left expr.");
  }

  BaseGDL* FCALL_LIB_DIRECTNode::Eval()
  {
    BaseGDL* param;
    bool isReference = 
      static_cast<ParameterNode*>(this->getFirstChild())->ParameterDirect( param);
    Guard<BaseGDL> guard;
    if( !isReference)
      guard.Reset( param);
    // check already here to keep functions leaner
    if( param == NULL)
    {
      assert( isReference);
      // unfortunately we cannot retrieve the variable's name here without some effort
      throw GDLException(this, 
			 this->libFun->ObjectName()+": Variable is undefined.",
			 false,false);
    }
    try {
      BaseGDL* res = 
	static_cast<DLibFunDirect*>(this->libFun)->FunDirect()(param, isReference);
      assert( res != NULL); //*** MUST always return a defined expression
      if( res == param)
	guard.release();
      return res;
    }
    catch( GDLException& ex)
    {
      // annotate exception
      throw GDLException(this, this->libFun->ObjectName()+": "+ ex.getMessage(),false,false); 
    }
  }

  BaseGDL** FCALL_LIBNode::EvalRefCheck( BaseGDL*& rEval)
  {
    EnvT* newEnv=new EnvT( this, this->libFun);//libFunList[fl->funIx]);

    ProgNode::interpreter->parameter_def_nocheck(this->getFirstChild(), newEnv);
	Guard<EnvT> guardEnv( newEnv);

    // make the call
    rEval = static_cast<DLibFun*>(newEnv->GetPro())->Fun()(newEnv);
    BaseGDL** res = ProgNode::interpreter->CallStackBack()->GetPtrTo( rEval);
    return res; // NULL ok, rEval set properly

  }
  
  BaseGDL** FCALL_LIBNode::LEval()
  {
//     // better than auto_ptr: auto_ptr wouldn't remove newEnv from the stack
//     StackGuard<EnvStackT> guard(ProgNode::interpreter->CallStack());
    // 	match(antlr::RefAST(_t),FCALL_LIB);
    EnvT* newEnv=new EnvT( this, this->libFun);//libFunList[fl->funIx]);

//     EnvUDT* callerEnv = ProgNode::interpreter->CallStackBack();

    ProgNode::interpreter->parameter_def_nocheck(this->getFirstChild(), newEnv);
    Guard<EnvT> guardEnv( newEnv);

//     // push id.pro onto call stack
//     ProgNode::interpreter->CallStack().push_back(newEnv);
    // make the call
    static DSub* scopeVarfetchPro = libFunList[ LibFunIx("SCOPE_VARFETCH")];
    if( scopeVarfetchPro == newEnv->GetPro())
    {
      BaseGDL**  sV = lib::scope_varfetch_reference( newEnv);
      if( sV != NULL)
	return sV;
      // should never happen
      throw GDLException( this, "SCOPE_VARFETCH returned no l-value: "+this->getText());
    }
    static DSub* routine_namesPro = libFunList[ LibFunIx("ROUTINE_NAMES")];
    if( routine_namesPro == newEnv->GetPro())
    {
      BaseGDL**  sV = lib::routine_names_reference( newEnv);
      if( sV != NULL)
	return sV;
      // should never happen
      throw GDLException( this, "ROUTINE_NAMES returned no l-value: "+this->getText());
    }
    BaseGDL* libRes = static_cast<DLibFun*>(newEnv->GetPro())->Fun()(newEnv);
    BaseGDL** res = ProgNode::interpreter->CallStackBack()->GetPtrTo( libRes);
    if( res == NULL)
    {
      GDLDelete( libRes);
      
      throw GDLException( this, "Library function must return a "
	    "l-value in this context: "+this->getText());
    }
    return res;
  }

  // returns new or existing variable
  BaseGDL* FCALL_LIBNode::EvalFCALL_LIB()
  {
//     // better than auto_ptr: auto_ptr wouldn't remove newEnv from the stack
//     StackGuard<EnvStackT> guard(ProgNode::interpreter->CallStack());
    EnvT* newEnv=new EnvT( this, this->libFun);//libFunList[fl->funIx]);
	
    ProgNode::interpreter->parameter_def_nocheck(this->getFirstChild(), newEnv);
	Guard<EnvT> guardEnv( newEnv);

    assert( dynamic_cast<EnvUDT*>(ProgNode::interpreter->CallStackBack()) != NULL);
    EnvUDT* callStackBack = static_cast<EnvUDT*>(ProgNode::interpreter->CallStackBack());
		
//     // push id.pro onto call stack
//     ProgNode::interpreter->CallStack().push_back(newEnv);
    // make the call
    BaseGDL* res=static_cast<DLibFun*>(newEnv->GetPro())->Fun()(newEnv);
    // *** MUST always return a defined expression
    assert( res != NULL);
    return res;
  }

  // returns always a new variable - see EvalFCALL_LIB
  BaseGDL* FCALL_LIBNode::Eval() 
  {
//     // better than auto_ptr: auto_ptr wouldn't remove newEnv from the stack
//     StackGuard<EnvStackT> guard(ProgNode::interpreter->CallStack());
    // 	match(antlr::RefAST(_t),FCALL_LIB);
    EnvT* newEnv=new EnvT( this, this->libFun);//libFunList[fl->funIx]);

    ProgNode::interpreter->parameter_def_nocheck(this->getFirstChild(), newEnv);

	Guard<EnvT> guardEnv( newEnv);

    assert( dynamic_cast<EnvUDT*>(ProgNode::interpreter->CallStackBack()) != NULL);
    EnvUDT* callStackBack = static_cast<EnvUDT*>(ProgNode::interpreter->CallStackBack());

//     // push id.pro onto call stack
//     ProgNode::interpreter->CallStack().push_back(newEnv);
    // make the call
    BaseGDL* res=static_cast<DLibFun*>(newEnv->GetPro())->Fun()(newEnv);
    // *** MUST always return a defined expression
    assert( res != NULL);
    //       throw GDLException( _t, "");

    if( callStackBack->Contains( res))
	return res = res->Dup();

//     static DSub* scopeVarfetchPro = libFunList[ LibFunIx("SCOPE_VARFETCH")];
//     if( scopeVarfetchPro == newEnv->GetPro())
//       return res->Dup();

    //ProgNode::interpreter->SetRetTree( this->getNextSibling());
    return res;
  }

  BaseGDL** MFCALLNode::EvalRefCheck( BaseGDL*& rEval)
  {
    StackGuard<EnvStackT> guard(ProgNode::interpreter->CallStack());
		
    ProgNodeP _t = this->getFirstChild();

    BaseGDL* self=_t->Eval(); //ProgNode::interpreter->expr(_t);
    Guard<BaseGDL> self_guard(self);
    
    _t = _t->getNextSibling();
    //match(antlr::RefAST(_t),IDENTIFIER);

    EnvUDT* newEnv=new EnvUDT( self, _t);
    
    self_guard.release();
    
    _t = _t->getNextSibling();
    
    ProgNode::interpreter->parameter_def(_t, newEnv); // temporary owns newEnv (-> no guard necessary)
  	
    // push environment onto call stack
    ProgNode::interpreter->CallStack().push_back(newEnv); // now guarded by "guard"
    
    // make the call
    rEval=ProgNode::interpreter->call_fun(static_cast<DSubUD*>(newEnv->GetPro())->GetTree());
    
    BaseGDL** res = ProgNode::interpreter->CallStackBack()->GetPtrTo( rEval);
    return res; // NULL ok, rEval set properly
    
  }  
  
  BaseGDL** MFCALLNode::LEval()
  {
        // better than auto_ptr: auto_ptr wouldn't remove newEnv from the stack
    StackGuard<EnvStackT> guard(ProgNode::interpreter->CallStack());

//		match(antlr::RefAST(_t),MFCALL);
    ProgNodeP _t = this->getFirstChild();
    BaseGDL* self=_t->Eval(); //ProgNode::interpreter->expr(_t);
    Guard<BaseGDL> self_guard(self);

    ProgNodeP mp = _t->getNextSibling();
//		match(antlr::RefAST(_t),IDENTIFIER);
    _t = mp->getNextSibling();

    EnvUDT* newEnv=new EnvUDT( self, mp, "", true);

    self_guard.release();

    ProgNode::interpreter->parameter_def(_t, newEnv);
    
    // push environment onto call stack
    ProgNode::interpreter->CallStack().push_back(newEnv);
	    
	    // make the call
    BaseGDL**	res=ProgNode::interpreter->
		    call_lfun(static_cast<DSubUD*>(newEnv->GetPro())->GetTree());

 	//ProgNode::interpreter->SetRetTree( this->getNextSibling());
    return res;
  }

  BaseGDL* MFCALLNode::Eval()
  {
      // better than auto_ptr: auto_ptr wouldn't remove newEnv from the stack
    StackGuard<EnvStackT> guard(ProgNode::interpreter->CallStack());
		
    ProgNodeP _t = this->getFirstChild();

    BaseGDL* self=_t->Eval(); //ProgNode::interpreter->expr(_t);
    Guard<BaseGDL> self_guard(self);
    
    _t = _t->getNextSibling();
    //match(antlr::RefAST(_t),IDENTIFIER);

    EnvUDT* newEnv=new EnvUDT( self, _t);
    
    self_guard.release();
    
    _t = _t->getNextSibling();
    
    ProgNode::interpreter->parameter_def(_t, newEnv);
  	
    // push environment onto call stack
    ProgNode::interpreter->CallStack().push_back(newEnv);
    
    // make the call
    BaseGDL* res=ProgNode::interpreter->call_fun(static_cast<DSubUD*>(newEnv->GetPro())->GetTree());
    
    //ProgNode::interpreter->SetRetTree( this->getNextSibling());
    return res;
  }

  BaseGDL** MFCALL_PARENTNode::EvalRefCheck( BaseGDL*& rEval)
  {
    StackGuard<EnvStackT> guard(ProgNode::interpreter->CallStack());
//  match(antlr::RefAST(_t),MFCALL_PARENT);
    ProgNodeP _t = this->getFirstChild();
    BaseGDL* self=_t->Eval(); //ProgNode::interpreter->expr(_t);
    Guard<BaseGDL> self_guard(self);
    
    _t = _t->getNextSibling();
    ProgNodeP parent = _t;
// 		match(antlr::RefAST(_t),IDENTIFIER);
    _t = _t->getNextSibling();
    ProgNodeP p = _t;
// 		match(antlr::RefAST(_t),IDENTIFIER);
    _t = _t->getNextSibling();

    EnvUDT* newEnv=new EnvUDT( self, p,	parent->getText());
    
    self_guard.release();
    
    ProgNode::interpreter->parameter_def(_t, newEnv);
  	
    // push environment onto call stack
    ProgNode::interpreter->CallStack().push_back(newEnv);
    
    // make the call
    rEval=ProgNode::interpreter->call_fun(static_cast<DSubUD*>(newEnv->GetPro())->GetTree());
    BaseGDL** res = ProgNode::interpreter->CallStackBack()->GetPtrTo( rEval);
    return res; // NULL ok, rEval set properly    
  }
  
  BaseGDL** MFCALL_PARENTNode::LEval()
  {
      // better than auto_ptr: auto_ptr wouldn't remove newEnv from the stack
    StackGuard<EnvStackT> guard(ProgNode::interpreter->CallStack());
//			match(antlr::RefAST(_t),MFCALL_PARENT);
	ProgNodeP _t = this->getFirstChild();
	BaseGDL* self=_t->Eval(); //ProgNode::interpreter->expr(_t);
	Guard<BaseGDL> self_guard(self);

	_t = _t->getNextSibling();
	ProgNodeP parent = _t;
// 		match(antlr::RefAST(_t),IDENTIFIER);
	_t = _t->getNextSibling();
	ProgNodeP p = _t;
// 		match(antlr::RefAST(_t),IDENTIFIER);
	_t = _t->getNextSibling();

	EnvUDT* newEnv=new EnvUDT( self, p,	parent->getText(), true);

	self_guard.release();

	ProgNode::interpreter->parameter_def(_t, newEnv);

		// push environment onto call stack
	ProgNode::interpreter->CallStack().push_back(newEnv);
		
		// make the call
	BaseGDL**	res=ProgNode::interpreter->
			call_lfun(static_cast<DSubUD*>(newEnv->GetPro())->GetTree());

 	//ProgNode::interpreter->SetRetTree( this->getNextSibling());
	return res;
  }

  BaseGDL* MFCALL_PARENTNode::Eval()
  {
    // better than auto_ptr: auto_ptr wouldn't remove newEnv from the stack
    StackGuard<EnvStackT> guard(ProgNode::interpreter->CallStack());
//  match(antlr::RefAST(_t),MFCALL_PARENT);
    ProgNodeP _t = this->getFirstChild();
    BaseGDL* self=_t->Eval(); //ProgNode::interpreter->expr(_t);
    Guard<BaseGDL> self_guard(self);
    
    _t = _t->getNextSibling();
    ProgNodeP parent = _t;
// 		match(antlr::RefAST(_t),IDENTIFIER);
    _t = _t->getNextSibling();
    ProgNodeP p = _t;
// 		match(antlr::RefAST(_t),IDENTIFIER);
    _t = _t->getNextSibling();

    EnvUDT* newEnv=new EnvUDT( self, p,	parent->getText());
    
    self_guard.release();
    
    ProgNode::interpreter->parameter_def(_t, newEnv);
  	
    // push environment onto call stack
    ProgNode::interpreter->CallStack().push_back(newEnv);
    
    // make the call
    BaseGDL* res=ProgNode::interpreter->call_fun(static_cast<DSubUD*>(newEnv->GetPro())->GetTree());
    
    //ProgNode::interpreter->SetRetTree( this->getNextSibling());
    return res;
  }

  BaseGDL** FCALLNode::EvalRefCheck( BaseGDL*& rEval)
  {
    StackGuard<EnvStackT> guard(ProgNode::interpreter->CallStack());
    ProgNode::interpreter->SetFunIx( this);
    
    EnvUDT* newEnv=new EnvUDT( this, funList[this->funIx]);
    
    ProgNode::interpreter->parameter_def(this->getFirstChild(), newEnv);

    // push environment onto call stack
    ProgNode::interpreter->CallStack().push_back(newEnv);
    
    // make the call
    rEval=ProgNode::interpreter->call_fun(static_cast<DSubUD*>(newEnv->GetPro())->GetTree());
    BaseGDL** res = ProgNode::interpreter->CallStackBack()->GetPtrTo( rEval);
    return res; // NULL ok, rEval set properly    
  }
  
  BaseGDL** ARRAYEXPR_FCALLNode::EvalRefCheck( BaseGDL*& rEval)
  {
    if( fcallNodeFunIx >= 0)
      return fcallNode->FCALLNode::EvalRefCheck( rEval);
    else if( fcallNodeFunIx == -2)
    {
      rEval = arrayExprNode->ARRAYEXPRNode::Eval();
      return NULL;
    }
    
    assert( fcallNodeFunIx == -1);
    try{
      BaseGDL** res = fcallNode->FCALLNode::EvalRefCheck( rEval);
      fcallNodeFunIx = fcallNode->funIx;
    } catch( GDLException& ex)
    {
      // keep FCALL if already compiled (but runtime error)
      if(fcallNode->funIx >= 0)
      {
	fcallNodeFunIx = fcallNode->funIx;
	throw ex;
      }
      try{
	rEval = arrayExprNode->ARRAYEXPRNode::Eval();
	assert( rEval != NULL);
	fcallNodeFunIx = -2; // mark as ARRAYEXPR succeeded
	return NULL;
      }
      catch( GDLException& innerEx)
      {
	string msg = "Ambiguous: " + ex.toString() +
	"  or: " + innerEx.toString();
	throw GDLException(this,msg,true,false);
      }
    }
  }
  
  BaseGDL** FCALLNode::LEval()
  {
    // better than auto_ptr: auto_ptr wouldn't remove newEnv from the stack
    StackGuard<EnvStackT> guard(ProgNode::interpreter->CallStack());
    //	match(antlr::RefAST(_t),FCALL);
    ProgNodeP	_t = this->getFirstChild();

    ProgNode::interpreter->SetFunIx( this);

    EnvUDT* newEnv=new EnvUDT( this, funList[this->funIx], true);

    ProgNode::interpreter->parameter_def(_t, newEnv);

    // push environment onto call stack
    ProgNode::interpreter->CallStack().push_back(newEnv);

    // make the call
    BaseGDL** res=
    ProgNode::interpreter->call_lfun(static_cast<DSubUD*>(newEnv->GetPro())->GetTree());

    //ProgNode::interpreter->SetRetTree( this->getNextSibling());
    return res;
  }

  BaseGDL** ARRAYEXPR_FCALLNode::LEval()
  {
    if( fcallNodeFunIx >= 0)
      return fcallNode->FCALLNode::LEval();
    else if( fcallNodeFunIx == -2)
    {
      return arrayExprNode->ARRAYEXPRNode::LEval();
    }

    assert( fcallNodeFunIx == -1);
    try{
      BaseGDL** res = fcallNode->FCALLNode::LEval();
      fcallNodeFunIx = fcallNode->funIx;
      return res;
    } catch( GDLException& ex)
    {
      // keep FCALL if already compiled (but runtime error)
      if(fcallNode->funIx >= 0)
      {
	fcallNodeFunIx = fcallNode->funIx;
	throw ex;
      }
      try{
	BaseGDL** res = arrayExprNode->ARRAYEXPRNode::LEval();
	fcallNodeFunIx = -2; // mark as ARRAYEXPR succeeded
	return res;
      }
      catch( GDLException& innerEx)
      {
	string msg = "Ambiguous: " + ex.toString() +
	"  or: " + innerEx.toString();
	throw GDLException(this,msg,true,false);
      }
    }
  }

  BaseGDL* FCALLNode::Eval()
  {
      // better than auto_ptr: auto_ptr wouldn't remove newEnv from the stack
    StackGuard<EnvStackT> guard(ProgNode::interpreter->CallStack());
    ProgNode::interpreter->SetFunIx( this);
    
    EnvUDT* newEnv=new EnvUDT( this, funList[this->funIx]);
    
    ProgNode::interpreter->parameter_def(this->getFirstChild(), newEnv);

    // push environment onto call stack
    ProgNode::interpreter->CallStack().push_back(newEnv);
    
    // make the call
    BaseGDL*
    res=ProgNode::interpreter->call_fun(static_cast<DSubUD*>(newEnv->GetPro())->GetTree());
    
    //ProgNode::interpreter->SetRetTree( this->getNextSibling());
    return res;
  }

  BaseGDL* ARRAYEXPR_FCALLNode::Eval()
  {
    if( fcallNodeFunIx >= 0)
      return fcallNode->FCALLNode::Eval();
    else if( fcallNodeFunIx == -2)
    {
	return arrayExprNode->ARRAYEXPRNode::Eval();
    }
    
    assert( fcallNodeFunIx == -1);
    try{
      BaseGDL* res = fcallNode->FCALLNode::Eval();
      fcallNodeFunIx = fcallNode->funIx;
      return res;
    } catch( GDLException& ex)
    {
      // keep FCALL if already compiled (but runtime error)
      if(fcallNode->funIx >= 0)
      {
	fcallNodeFunIx = fcallNode->funIx;
	throw ex;
      }
      try{
	BaseGDL* res = arrayExprNode->ARRAYEXPRNode::Eval();
	fcallNodeFunIx = -2; // mark as ARRAYEXPR succeeded
	return res;
      }
      catch( GDLException& innerEx)
      {
	string msg = "Ambiguous: " + ex.toString() +
	"  or: " + innerEx.toString();
	throw GDLException(this,msg,true,false);
      }
    }
  }
  
  
BaseGDL** ARRAYEXPR_MFCALLNode::EvalRefCheck( BaseGDL*& rEval)
  {
    StackGuard<EnvStackT> guard(ProgNode::interpreter->CallStack());
//		match(antlr::RefAST(_t),ARRAYEXPR_MFCALL);
    ProgNodeP mark = this->getFirstChild();

    ProgNodeP _t = mark->getNextSibling(); // skip DOT

    BaseGDL* self=_t->Eval(); //ProgNode::interpreter->expr(_t);
    Guard<BaseGDL> self_guard(self);

    ProgNodeP mp2 = _t->getNextSibling();
    //match(antlr::RefAST(_t),IDENTIFIER);

    _t = mp2->getNextSibling();

    BaseGDL** res;

    EnvUDT* newEnv;
    try {
	    newEnv=new EnvUDT( self, mp2);
	    self_guard.release();
    }
    catch( GDLException& ex)
    {
	    goto tryARRAYEXPR;
    }

    ProgNode::interpreter->parameter_def(_t, newEnv);

    // push environment onto call stack
    ProgNode::interpreter->CallStack().push_back(newEnv);
    
    // make the call
    rEval= ProgNode::interpreter->
		    call_fun(static_cast<DSubUD*>(newEnv->GetPro())->GetTree());
    res = ProgNode::interpreter->CallStackBack()->GetPtrTo( rEval);
    return res; // NULL ok, rEval set properly    
    
    tryARRAYEXPR:;
    //_t = mark;
    
    ProgNodeP dot = mark;
    // 	match(antlr::RefAST(_t),DOT);
    _t = mark->getFirstChild();
	    
    SizeT nDot=dot->nDot;
    Guard<DotAccessDescT> aD( new DotAccessDescT(nDot+1));
	    
    ProgNode::interpreter->r_dot_array_expr(_t, aD.get());
    _t = _t->getNextSibling();
    for (; _t != NULL;) {
    ProgNode::interpreter->tag_array_expr(_t, aD.get());
    _t = _t->getNextSibling();
    }
    rEval= aD->ADResolve();
    return NULL;  // always r-value
  }
  
// from  l_arrayexpr_mfcall_as_mfcall
BaseGDL** ARRAYEXPR_MFCALLNode::LEval()
  {
    // better than auto_ptr: auto_ptr wouldn't remove newEnv from the stack
    StackGuard<EnvStackT> guard(ProgNode::interpreter->CallStack());
    BaseGDL *self;
    EnvUDT*   newEnv;
    
    ProgNodeP _t = this->getFirstChild();    
    _t = _t->getNextSibling(); // skip DOT
    
    self= _t->Eval(); //interpreter->expr(_t);
    ProgNodeP mp2 = _t->getNextSibling(); // interpreter->GetRetTree();
    //match(antlr::RefAST(_t),IDENTIFIER);
    
    Guard<BaseGDL> self_guard(self);
    
    newEnv=new EnvUDT( self, mp2, "", true);
    
    self_guard.release();
    
    ProgNode::interpreter->parameter_def( mp2->getNextSibling(), newEnv);
    
    // push environment onto call stack
    ProgNode::interpreter->CallStack().push_back(newEnv);
    
    // make the call
    return ProgNode::interpreter->call_lfun(static_cast<DSubUD*>(
    newEnv->GetPro())->GetTree());
  }
    
  

  BaseGDL* ARRAYEXPR_MFCALLNode::Eval()
  {
      // better than auto_ptr: auto_ptr wouldn't remove newEnv from the stack
    StackGuard<EnvStackT> guard(ProgNode::interpreter->CallStack());
//		match(antlr::RefAST(_t),ARRAYEXPR_MFCALL);
    ProgNodeP mark = this->getFirstChild();

    ProgNodeP _t = mark->getNextSibling(); // skip DOT

    BaseGDL* self=_t->Eval(); //ProgNode::interpreter->expr(_t);
    Guard<BaseGDL> self_guard(self);

    ProgNodeP mp2 = _t->getNextSibling();
    //match(antlr::RefAST(_t),IDENTIFIER);

    _t = mp2->getNextSibling();

    BaseGDL* res;

    EnvUDT* newEnv;
    try {
	    newEnv=new EnvUDT( self, mp2);
	    self_guard.release();
    }
    catch( GDLException& ex)
    {
	    goto tryARRAYEXPR;
    }

    ProgNode::interpreter->parameter_def(_t, newEnv);

    // push environment onto call stack
    ProgNode::interpreter->CallStack().push_back(newEnv);
    
    // make the call
    res=
	    ProgNode::interpreter->
		    call_fun(static_cast<DSubUD*>(newEnv->GetPro())->GetTree());
    
    //ProgNode::interpreter->SetRetTree( this->getNextSibling());
    return res;
    
    tryARRAYEXPR:;
    //_t = mark;
    
    ProgNodeP dot = mark;
    // 	match(antlr::RefAST(_t),DOT);
    _t = mark->getFirstChild();
	    
    SizeT nDot=dot->nDot;
    Guard<DotAccessDescT> aD( new DotAccessDescT(nDot+1));
	    
    ProgNode::interpreter->r_dot_array_expr(_t, aD.get());
    _t = _t->getNextSibling();
    for (; _t != NULL;) {
    ProgNode::interpreter->tag_array_expr(_t, aD.get());
    _t = _t->getNextSibling();
    }
    res= aD->ADResolve();
    
    //ProgNode::interpreter->SetRetTree( this->getNextSibling());
    return res;
  }

//   BaseGDL** ARRAYEXPR_MFCALLNode::LEval()
//   {
//       // better than auto_ptr: auto_ptr wouldn't remove newEnv from the stack
//     StackGuard<EnvStackT> guard(ProgNode::interpreter->CallStack());
//   }

BaseGDL** VARNode::EvalRefCheck( BaseGDL*& rEval)
  {
    return this->LEval();
  }
  BaseGDL** VARNode::LEval()
	{
//   	ProgNode::interpreter->SetRetTree( this->getNextSibling());
	return &ProgNode::interpreter->CallStackBack()->GetKW(this->varIx);
	}
BaseGDL** VARPTRNode::EvalRefCheck( BaseGDL*& rEval)
  {
    return this->LEval();
  }
  BaseGDL** VARPTRNode::LEval()
	{
//   	ProgNode::interpreter->SetRetTree( this->getNextSibling());
	return &this->var->Data();
	}


BaseGDL** EXPRNode::EvalRefCheck( BaseGDL*& rEval)
  {
    return this->LEval();
  }
BaseGDL** EXPRNode::LEval()
{
  return this->getFirstChild()->LExpr( NULL); //interpreter->l_expr( this->getFirstChild(), NULL);
}

BaseGDL* DOTNode::Eval()
{
  BaseGDL* res;

  ProgNodeP _t = this->getFirstChild();

  // SizeT nDot=this->nDot;

  DotAccessDescT aD( nDot+1);

  //interpreter->r_dot_array_expr(_t, &aD);
  // r_dot_array_expr /////////////////////
  BaseGDL*         r;

  // clears aL when destroyed
  ArrayIndexListGuard guard; 
  
  if( _t->getType() == GDLTokenTypes::ARRAYEXPR)
  {
    ProgNodeP tIn = _t;

    _t = _t->getFirstChild();

    r = interpreter->r_dot_indexable_expr(_t, &aD);

    _t = interpreter->GetRetTree();

    ArrayIndexListT* aL=interpreter->arrayindex_list(_t);

    guard.reset(aL);

    _t = tIn->getNextSibling();

    // check here for object and get struct
    //structR=dynamic_cast<DStructGDL*>(r);
    // this is much faster than a dynamic_cast
    interpreter->SetRootR( tIn, &aD, r, aL);
  }
  else
// 	case EXPR:
// 	case SYSVAR:
// 	case VAR:
// 	case VARPTR:
  {
    r=interpreter->r_dot_indexable_expr(_t, &aD);
    _t = interpreter->GetRetTree();

    interpreter->SetRootR( _t, &aD, r, NULL);
  }
/////////

  for (; _t != NULL;) {
	  interpreter->tag_array_expr(_t, &aD); // nDot times
	  _t = interpreter->GetRetTree();
  }
  return aD.ADResolve();
} // DOTNode::Eval
  
BaseGDL* ARRAYEXPRNode::Eval()
{
  BaseGDL* res;

  ExprListT        exprList; // for cleanup
  IxExprListT      ixExprList;
  SizeT nExpr;
  BaseGDL* s;

  //	match(antlr::RefAST(_t),ARRAYEXPR);
  ProgNodeP	_t = this->getFirstChild();

  BaseGDL* r;
  Guard<BaseGDL> rGuard;
  if( NonCopyNode(_t->getType()))
  {
      r=_t->EvalNC();
      //r=indexable_expr(_t);
  }
  else if( _t->getType() == GDLTokenTypes::FCALL_LIB)
  {
      // better than Eval(): no copying here if not necessary
      r=ProgNode::interpreter->lib_function_call(_t);

      if( !ProgNode::interpreter->CallStack().back()->Contains( r))
	rGuard.Reset( r); // guard if no global data	  
  }
  else
  {
      r=ProgNode::interpreter->indexable_tmp_expr(_t);
      rGuard.Reset( r);  
  }

  ProgNodeP ixListNode = _t->getNextSibling();

  if( r->Type() == GDL_OBJ && r->StrictScalar())
  {
      // check for _overloadBracketsRightSide
      
      DObj s = (*static_cast<DObjGDL*>(r))[0]; // is StrictScalar()
//       if( s != 0)  // no overloads for null object
//       {
// 	DStructGDL* oStructGDL= GDLInterpreter::GetObjHeapNoThrow( s);
// 	if( oStructGDL != NULL) // if object not valid -> default behaviour
// 	  {
// 	    DStructDesc* desc = oStructGDL->Desc();
// 
// 	    DFun* bracketsRightSideOverload = static_cast<DFun*>(desc->GetOperator( OOBracketsRightSide));
	    DFun* bracketsRightSideOverload = static_cast<DFun*>(GDLInterpreter::GetObjHeapOperator( s, OOBracketsRightSide));
	    if( bracketsRightSideOverload != NULL)
	      {
		// _overloadBracketsRightSide
		BaseGDL* self = rGuard.Get();
		if( self == NULL)
		{
		  self = r->Dup(); // not set -> not owner
		  rGuard.Reset( self);
		}
      // we are now the proud owner of 'self'

		IxExprListT indexList;
		// uses arrIxListNoAssoc
		interpreter->arrayindex_list_overload( ixListNode, indexList);
		ArrayIndexListGuard guard(ixListNode->arrIxListNoAssoc);

		// hidden SELF is counted as well
		int nParSub = bracketsRightSideOverload->NPar();
		assert( nParSub >= 1); // SELF
		// indexList.size() > regular paramters w/o SELF
		if( indexList.size() > nParSub - 1)
		{
		  indexList.Cleanup();
		  throw GDLException( this, bracketsRightSideOverload->ObjectName() +
				      ": Incorrect number of arguments.",
				      false, false);
		}

		// adds already SELF parameter
		EnvUDT* newEnv= new EnvUDT( this, bracketsRightSideOverload, &self);
		// no guarding of newEnv here (no exceptions until push_back())
		
		// parameters
		for( SizeT p=0; p<indexList.size(); ++p)
		  newEnv->SetNextParUnchecked( indexList[p]); // takes ownership

		StackGuard<EnvStackT> stackGuard(interpreter->CallStack());
		interpreter->CallStack().push_back( newEnv); 
		
		// make the call, return the result
		BaseGDL* res = interpreter->call_fun(static_cast<DSubUD*>(newEnv->GetPro())->GetTree());

		if( self != rGuard.Get())
		{
		  // always put out warning first, in case of a later crash
		  Warning( "WARNING: " + bracketsRightSideOverload->ObjectName() + 
			   ": Assignment to SELF detected (GDL session still ok).");
		  // assignment to SELF -> self was deleted and points to new variable
		  // which it owns
		  rGuard.Release();
		  if( self != NullGDL::GetSingleInstance())
		    rGuard.Reset(self);
		}

		return res;
	      }
// 	  }
//       }
  }
  
  // first use NoAssoc case
  ArrayIndexListT* aL = ixListNode->arrIxListNoAssoc;
  assert( aL != NULL);
  nExpr = aL->NParam();
	  
  _t = ixListNode->getFirstChild();
	  
  if( nExpr == 0)
  {
    goto empty;
  }
	  
  while( true)
  {
    if( NonCopyNode(_t->getType()))
    {
	s=_t->EvalNC();//indexable_expr(_t);
	assert(s != NULL);
    }
    else if( _t->getType() == GDLTokenTypes::FCALL_LIB)
    {
	s=ProgNode::interpreter->lib_function_call(_t);
	if( !ProgNode::interpreter->CallStack().back()->Contains( s))
		  exprList.push_back( s);
	assert(s != NULL);
    }
    else
    {
	s=_t->Eval(); //ProgNode::interpreter->indexable_tmp_expr(_t);
	exprList.push_back( s);
	assert(s != NULL);
    }

    ixExprList.push_back( s);
    if( ixExprList.size() == nExpr)
      break; // for -> finish

    _t = _t->getNextSibling(); // set to next index
  } // for

  empty:

  if( r->IsAssoc())
  {
    ArrayIndexListT* aLAssoc = ixListNode->arrIxList;
    assert( aLAssoc != NULL);
    ArrayIndexListGuard guardAssoc(aLAssoc);
    return aLAssoc->Index( r, ixExprList);	  
  }
  else
  {
    ArrayIndexListGuard guard(aL);
    return aL->Index( r, ixExprList);
  }
  assert( false);
  return NULL;
}
