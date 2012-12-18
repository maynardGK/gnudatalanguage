/***************************************************************************
			prognode_lexpr.cpp  -  LExpr functions
			-------------------
begin                : July 22 2002
copyright            : (C) 2011 by Marc Schellens
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

// from GDLInterpreter::l_expr
#include <cassert>
#include <string>

#include "dinterpreter.hpp"
#include "prognodeexpr.hpp"
#include "basegdl.hpp"
#include "arrayindexlistt.hpp"
//#include "envt.hpp"
#include "gdlexception.hpp"
#include "nullgdl.hpp"

// illegal
BaseGDL** ProgNode::LExpr( BaseGDL* right)
//	default:
{
	//   case ARRAYDEF:
	//   case EXPR:
	//   case NSTRUC:
	//   case NSTRUC_REF:
	//   case POSTDEC:
	//   case POSTINC:
	//   case STRUC:
	//   case DEC:
	//   case INC:
	//   case CONSTANT:
	throw GDLException( this, "Expression not allowed as l-value.",
				  true,false);
	return NULL; // avoid compiler warning
}


BaseGDL** QUESTIONNode::LExpr( BaseGDL* right)
		//case QUESTION:
	{
	ProgNodeP _t = this->getFirstChild();
	BaseGDL*       e1=interpreter->expr(_t);
	_t = interpreter->GetRetTree();
	std::auto_ptr<BaseGDL> e1_guard(e1);
	if( e1->True())
	{
	return _t->LExpr( right); //l_expr(_t, right);
	}
	else
	{
	_t=_t->GetNextSibling(); // jump over 1st expression
	return _t->LExpr( right); //l_expr(_t, right);
	}
	//SetRetTree( tIn->getNextSibling());
	//return res;
	}

BaseGDL** ARRAYEXPRNode::LExpr( BaseGDL* right) // 'right' is not owned
	//case ARRAYEXPR:
	{
	//res=l_array_expr(_t, right);
	if( right == NULL)
	throw GDLException( this, "Indexed expression not allowed in this context.",
		true,false);

	ArrayIndexListT* aL;
	ArrayIndexListGuard guard;

	BaseGDL** res=interpreter->l_indexable_expr( this->getFirstChild());
	if( (*res)->IsAssoc())
	  aL=interpreter->arrayindex_list( this->getFirstChild()->getNextSibling());
	else
	{
	  if( (*res)->Type() == GDL_OBJ && (*res)->StrictScalar())
	  {
	      // check for _overloadBracketsLeftSide
	      DObj s = (*static_cast<DObjGDL*>(*res))[0]; // is StrictScalar()
// 	      if( s != 0)  // no overloads for null object
// 	      {
// 		DStructGDL* oStructGDL= GDLInterpreter::GetObjHeapNoThrow( s);
// 		if( oStructGDL != NULL) // if object not valid -> default behaviour
// 		  {
// 		    DStructDesc* desc = oStructGDL->Desc();
// 		    DPro* bracketsLeftSideOverload = static_cast<DPro*>(desc->GetOperator( OOBracketsLeftSide));
		    DPro* bracketsLeftSideOverload = static_cast<DPro*>(GDLInterpreter::GetObjHeapOperator( s, OOBracketsLeftSide));
		    if( bracketsLeftSideOverload != NULL)
		    {
		      bool internalDSubUD = bracketsLeftSideOverload->GetTree()->IsWrappedNode();  

			// _overloadBracketsLeftSide
		      IxExprListT indexList;
		      interpreter->arrayindex_list_overload( this->getFirstChild()->getNextSibling(), indexList);
		      ArrayIndexListGuard guard(this->getFirstChild()->getNextSibling()->arrIxListNoAssoc);
		     
		      // hidden SELF is counted as well
		      int nParSub = bracketsLeftSideOverload->NPar();
		      assert( nParSub >= 1); // SELF
//  		      int indexListSizeDebug = indexList.size();
		      // indexList.size() + OBJREF + RVALUE > regular paramters w/o SELF
		      if( (indexList.size() + 2) > nParSub - 1)
		      {
			indexList.Cleanup();
			throw GDLException( this, bracketsLeftSideOverload->ObjectName() +
                                        ": Incorrect number of arguments.",
                                        false, false);
		      }

		      BaseGDL* self;
		      Guard<BaseGDL> selfGuard;
		      if( internalDSubUD)
		      {
			self = (*res); // internal subroutines behave well
		      }
		      else
		      {
			self = (*res)->Dup(); // res should be not changeable via SELF
			selfGuard.Reset( self);
		      }
		      
		      // adds already SELF parameter
		      EnvUDT* newEnv= new EnvUDT( this, bracketsLeftSideOverload, &self);
// 		      Guard<EnvUDT> newEnvGuard( newEnv);
		      
		      // parameters
		      newEnv->SetNextParUnchecked( res); // OBJREF  parameter
		      // Dup() here is not optimal
		      // avoid at least for internal overload routines (which do/must not change RVALUE)
		      if( internalDSubUD)  
			newEnv->SetNextParUnchecked( &right); // RVALUE  parameter, as reference to prevent cleanup in newEnv
		      else
			newEnv->SetNextParUnchecked( right->Dup()); // RVALUE parameter, as value
		      // pass as reference would be more efficient, but as the data might
		      // be deleted in bracketsLeftSideOverload it is not possible.
		      // BaseGDL* rightCopy = right;  
		      // newEnv->SetNextParUnchecked( &rightCopy); // RVALUE  parameter
		      for( SizeT p=0; p<indexList.size(); ++p)
			newEnv->SetNextParUnchecked( indexList[p]); // takes ownership
  
		      StackGuard<EnvStackT> stackGuard(interpreter->CallStack());
		      interpreter->CallStack().push_back( newEnv); 
		      
		      // make the call
		      interpreter->call_pro(static_cast<DSubUD*>(newEnv->GetPro())->GetTree());

		      if( !internalDSubUD && self != selfGuard.Get())
		      {
			// always put out warning first, in case of a later crash
			Warning( "WARNING: " + bracketsLeftSideOverload->ObjectName() + 
			      ": Assignment to SELF detected (GDL session still ok).");
			// assignment to SELF -> self was deleted and points to new variable
			// which it owns
			selfGuard.Release();
			if( self != NullGDL::GetSingleInstance())
			  selfGuard.Reset(self);
		      }
		      
		      return res;
		    }
// 		  }
// 	      }
	  }
	  aL=interpreter->arrayindex_list_noassoc( this->getFirstChild()->getNextSibling());  
	}
	guard.reset(aL);

	try {
		aL->AssignAt( *res, right);
	}
	catch( GDLException& ex)
	{
		ex.SetErrorNodeP( this);
		throw ex;
	}
	//_retTree = _t->getNextSibling();
	return res;
	}
// default ...Grab version
	
BaseGDL** SYSVARNode::LExpr( BaseGDL* right)
		//case SYSVAR:
	{
	if( right == NULL)
	throw GDLException( this, "System variable not allowed in this context.",
		true,false);

	BaseGDL** res=this->LEval(); //l_sys_var(this);
	std::auto_ptr<BaseGDL> conv_guard; //( rConv);
	BaseGDL* rConv = right;
	if( !(*res)->EqType( right))
	{
		rConv = right->Convert2( (*res)->Type(), BaseGDL::COPY);
		conv_guard.reset( rConv);
	}
	if( right->N_Elements() != 1 && ((*res)->N_Elements() != right->N_Elements()))
	{
		throw GDLException( this, "Conflicting data structures: <"+
			right->TypeStr()+" "+right->Dim().ToString()+">, !"+
			this->getText(),true,false);
	}
	(*res)->AssignAt( rConv); // linear copy
	return res;
	}
// default ...Grab version

// BaseGDL** XXXNode::LExpr( BaseGDL* right)
// // 	case FCALL:
// // 	case FCALL_LIB:
// // 	case MFCALL:
// // 	case MFCALL_PARENT:
// // 	case DEREF:
// // 	case VAR:
// // 	case VARPTR:
// 	{
// 	BaseGDL** res=this->LEval(); //l_simple_var(_t);
// 	//_retTree = _t->getNextSibling();
// 	if( right != NULL && right != (*res))
// 	{
// 		delete *res;
// 		*res = right->Dup();
// 	}
// 	return res;
// 	}

// #define LEXPRGRAB \
// 	{	BaseGDL** res=this->LEval(); \
// 	if( right != NULL && right != (*res)) \
// 	{	delete *res; *res = right;} \
// 	return res;}
// 
// BaseGDL** FCALLNode::LExprGrab( BaseGDL* right)
// LEXPRGRAB
// BaseGDL** MFCALLNode::LExprGrab( BaseGDL* right)
// LEXPRGRAB
// BaseGDL** MFCALL_PARENTNode::LExprGrab( BaseGDL* right)
// LEXPRGRAB
// BaseGDL** FCALL_LIBNode::LExprGrab( BaseGDL* right)
// LEXPRGRAB
// BaseGDL** DEREFNode::LExprGrab( BaseGDL* right)
// LEXPRGRAB
// BaseGDL** VARNode::LExprGrab( BaseGDL* right)
// LEXPRGRAB
// BaseGDL** VARPTRNode::LExprGrab( BaseGDL* right)
// LEXPRGRAB
// #undef LEXPRGRAB

#define LEXPR \
	{	BaseGDL** res=this->LEval(); \
	if( right != NULL && right != (*res)) \
	{	GDLDelete(*res); *res = right->Dup();} \
	return res;}
	
BaseGDL** FCALLNode::LExpr( BaseGDL* right)
LEXPR
BaseGDL** MFCALLNode::LExpr( BaseGDL* right)
LEXPR
BaseGDL** MFCALL_PARENTNode::LExpr( BaseGDL* right)
LEXPR
BaseGDL** FCALL_LIBNode::LExpr( BaseGDL* right)
LEXPR
BaseGDL** DEREFNode::LExpr( BaseGDL* right)
LEXPR
BaseGDL** VARNode::LExpr( BaseGDL* right)
LEXPR
BaseGDL** VARPTRNode::LExpr( BaseGDL* right)
LEXPR
#undef LEXPR

BaseGDL** ARRAYEXPR_FCALLNode::LExpr( BaseGDL* right)
{
  if( fcallNodeFunIx >= 0)
      return fcallNode->FCALLNode::LExpr( right);
  else if( fcallNodeFunIx == -2)
  {
    return arrayExprNode->ARRAYEXPRNode::LExpr( right);
  }
    
  assert( fcallNodeFunIx == -1);
  try{
      BaseGDL** res = fcallNode->FCALLNode::LExpr( right);
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
      BaseGDL** res = arrayExprNode->ARRAYEXPRNode::LExpr( right);
      fcallNodeFunIx = -2; // mark as ARRAYEXPR succeeded
      return res;
    }
    catch( GDLException& innerEx)
    {
	std::string msg = "Ambiguous: " + ex.toString() +
	"  or: " + innerEx.toString();
	throw GDLException(this,msg,true,false);
    }
  }
}
  

BaseGDL** ARRAYEXPR_MFCALLNode::LExpr( BaseGDL* right)
	//case ARRAYEXPR_MFCALL:
	{
	return interpreter->l_arrayexpr_mfcall(this, right);
	}
// default ...Grab version

BaseGDL** DOTNode::LExpr( BaseGDL* right)
	//case DOT:
	{
	if( right == NULL)
		throw GDLException( this, "Struct expression not allowed in this context.",
			true,false);

	ProgNodeP _t = this->getFirstChild();

	//SizeT nDot = tIn->nDot;
	std::auto_ptr<DotAccessDescT> aD( new DotAccessDescT(nDot+1));

	interpreter->l_dot_array_expr(_t, aD.get());
	_t = interpreter->GetRetTree();
	for( int d=0; d<nDot; ++d)
	{
	// if ((_t->getType() == ARRAYEXPR || _t->getType() == EXPR ||
	// _t->getType() == IDENTIFIER)) {
	interpreter->tag_array_expr(_t, aD.get());
	_t = interpreter->GetRetTree();
	//       }
	//       else {
	// 	break;
	//       }
	}
	aD->Assign( right);
	//res=NULL;
	//SetRetTree( tIn->getNextSibling());
	return NULL;
	}
// default ...Grab version

BaseGDL** ASSIGNNode::LExpr( BaseGDL* right)	
//case ASSIGN:
{
  ProgNodeP _t = _t->getFirstChild();
  if( NonCopyNode(_t->getType()))
  {
	  BaseGDL*       e1=interpreter->indexable_expr(_t);
	  _t = interpreter->GetRetTree();
  }
  else if( _t->getType() == GDLTokenTypes::FCALL_LIB)
  {
	  BaseGDL*       e1=interpreter->lib_function_call(_t);
	  _t = interpreter->GetRetTree();
	  if( !interpreter->CallStackBack()->Contains( e1))
		  GDLDelete(e1); // guard if no global data
  }
  else
  {
    //       case ASSIGN:
    //       case ASSIGN_REPLACE:
    //       case ASSIGN_ARRAYEXPR_MFCALL:
    //       case ARRAYDEF:
    //       case ARRAYEXPR:
    //       case ARRAYEXPR_MFCALL:
    //       case EXPR:
    //       case FCALL:
    //       case FCALL_LIB_RETNEW:
    //       case MFCALL:
    //       case MFCALL_PARENT:
    //       case NSTRUC:
    //       case NSTRUC_REF:
    //       case POSTDEC:
    //       case POSTINC:
    //       case STRUC:
    //       case DEC:
    //       case INC:
    //       case DOT:
    //       case QUESTION:
    BaseGDL*       e1=interpreter->indexable_tmp_expr(_t);
    _t = interpreter->GetRetTree();
    GDLDelete(e1);
  }
  //SetRetTree( tIn->getNextSibling());
  return _t->LExpr( right); //l_expr(_t, right);
}

BaseGDL** ASSIGN_ARRAYEXPR_MFCALLNode::LExpr( BaseGDL* right)
//case ASSIGN_ARRAYEXPR_MFCALL:
{
  ProgNodeP _t = _t->getFirstChild();

  if( NonCopyNode(_t->getType()))
  {
    BaseGDL*       e1=interpreter->indexable_expr(_t);
    _t = interpreter->GetRetTree();
  }
  else if( _t->getType() == GDLTokenTypes::FCALL_LIB)
  {
    BaseGDL*       e1=interpreter->lib_function_call(_t);
    _t = interpreter->GetRetTree();
    if( !interpreter->CallStackBack()->Contains( e1))
      GDLDelete(e1); // guard if no global data
  }
  else
  {
    //       case ASSIGN:
    //       case ASSIGN_REPLACE:
    //       case ASSIGN_ARRAYEXPR_MFCALL:
    //       case ARRAYDEF:
    //       case ARRAYEXPR:
    //       case ARRAYEXPR_MFCALL:
    //       case EXPR:
    //       case FCALL:
    //       case FCALL_LIB_RETNEW:
    //       case MFCALL:
    //       case MFCALL_PARENT:
    //       case NSTRUC:
    //       case NSTRUC_REF:
    //       case POSTDEC:
    //       case POSTINC:
    //       case STRUC:
    //       case DEC:
    //       case INC:
    //       case DOT:
    //       case QUESTION:
    BaseGDL*       e1=interpreter->indexable_tmp_expr(_t);
    _t = interpreter->GetRetTree();
    GDLDelete(e1);
  }
  ProgNodeP l = _t;
  BaseGDL** res;
  // try MFCALL
  try
  {
    res=interpreter->l_arrayexpr_mfcall_as_mfcall( l);
    if( right != (*res))
    {
      GDLDelete(*res);
      *res = right->Dup();
    }
  }
  catch( GDLException& ex)
  {
    // try ARRAYEXPR
    try
    {
      res=interpreter->l_arrayexpr_mfcall_as_arrayexpr(l, right);
    }
    catch( GDLException& ex2)
    {
      throw GDLException(ex.toString() + " or "+ex2.toString());
    }
  }
  //SetRetTree( tIn->getNextSibling());
  return res;
}


BaseGDL** ASSIGN_REPLACENode::LExpr( BaseGDL* right)
//	case ASSIGN_REPLACE:
{
  ProgNodeP _t = this->getFirstChild();

  if( _t->getType() == GDLTokenTypes::FCALL_LIB)
  {
	  BaseGDL* e1=interpreter->lib_function_call(_t);
	  _t = interpreter->GetRetTree();
	  if( !interpreter->CallStackBack()->Contains( e1))
	    GDLDelete(e1);
  }
  else
  {
  //     case ASSIGN:
  //     case ASSIGN_REPLACE:
  //     case ASSIGN_ARRAYEXPR_MFCALL:
  //     case ARRAYDEF:
  //     case ARRAYEXPR:
  //     case ARRAYEXPR_MFCALL:
  //     case CONSTANT:
  //     case DEREF:
  //     case EXPR:
  //     case FCALL:
  //     case FCALL_LIB_RETNEW:
  //     case MFCALL:
  //     case MFCALL_PARENT:
  //     case NSTRUC:
  //     case NSTRUC_REF:
  //     case POSTDEC:
  //     case POSTINC:
  //     case STRUC:
  //     case SYSVAR:
  //     case VAR:
  //     case VARPTR:
  //     case DEC:
  //     case INC:
  //     case DOT:
  //     case QUESTION:
  BaseGDL* e1=interpreter->tmp_expr(_t);
  _t = interpreter->GetRetTree();
  GDLDelete(e1);
  }

  // switch ( _t->getType()) {
  // case DEREF:
  //     // 	  {
  //     // 		  res=_t->LEval(); //l_deref(_t);
  //     // 		  _t = _retTree;
  //     // 		  break;
  //     // 	  }
  // case VAR:
  // case VARPTR:
  //     // {
  //     //     res=_t->LEval(); //l_simple_var(_t);
  //     //     _retTree = tIn->getNextSibling();
  //     //     //_t = _retTree;
  //     //     break;
  //     // }
  // default:
  //     // 	  case FCALL:
  //     // 	  case FCALL_LIB:
  //     // 	  case MFCALL:
  //     // 	  case MFCALL_PARENT:
  //     {
  BaseGDL** res=_t->LEval(); //l_function_call(_t);
  //_retTree = tIn->getNextSibling();
  //_t = _retTree;
  //         break;
  //     }
  // }
  if( right != (*res))
  {
    GDLDelete(*res);
    assert( right != NULL);
    *res = right->Dup();
  }
// 	SetRetTree( this->getNextSibling());
  return res;
}



// l_expr finish /////////////////////////////////////////////


