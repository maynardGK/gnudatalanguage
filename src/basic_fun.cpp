/***************************************************************************
                          basic_fun.cpp  -  basic GDL library function
                             -------------------
    begin                : July 22 2002
    copyright            : (C) 2002 by Marc Schellens (exceptions see below)
    email                : m_schellens@hotmail.com

 strtok_fun, getenv_fun, findfile_fun, tag_names_fun, stregex_fun:
 (C) 2004 by Peter Messmer    
 
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <string>
#include <fstream>
//#include <memory>
#include <regex.h> // stregex

#ifdef __APPLE__
# include <crt_externs.h>
# define environ (*_NSGetEnviron())
#else
#include <unistd.h>
#endif

#include "datatypes.hpp"
#include "envt.hpp"
#include "dpro.hpp"
#include "dinterpreter.hpp"
#include "basic_pro.hpp"
#include "terminfo.hpp"
#include "typedefs.hpp"


#define GDL_DEBUG
//#undef GDL_DEBUG

namespace lib {

  using namespace std;
  using namespace antlr;

  // assumes all paramters from pOffs till end are dim
  void arr( EnvT* e, dimension& dim, SizeT pOffs=0)
  {
    int nParam=e->NParam()-pOffs;

    if( nParam <= 0)
      throw GDLException( e->CallingNode(), "Incorrect number of arguments.");

    // max number checked in interpreter
    SizeT endIx=nParam+pOffs;
    for( SizeT i=pOffs; i<endIx; i++)
      {
	BaseGDL* par=e->GetParDefined( i);

	SizeT newDim;
	int ret=par->Scalar2index( newDim);
	if( ret < 1 || newDim == 0)
	  throw GDLException( e->CallingNode(), "Arguments must be all scalar > 0.");
	
	dim << newDim;
      }
  }

  BaseGDL* bytarr( EnvT* e)
  {
    dimension dim;
    try{
      arr( e, dim); 
      if( e->KeywordSet(0)) return new DByteGDL(dim, BaseGDL::NOZERO);
      return new DByteGDL(dim);
    }
    catch( GDLException ex)
      {
	throw GDLException( e->CallingNode(), "BYTARR: "+ex.getMessage());
      }
  }
  BaseGDL* intarr( EnvT* e)
  {
    dimension dim;
    try{
      arr( e, dim); 
      if( e->KeywordSet(0)) return new DIntGDL(dim, BaseGDL::NOZERO);
      return new DIntGDL(dim);
    }
    catch( GDLException ex)
      {
	throw GDLException( e->CallingNode(), "INTARR: "+ex.getMessage());
      }
  }
  BaseGDL* uintarr( EnvT* e)
  {
    dimension dim;
    try{
      arr( e, dim); 
      if( e->KeywordSet(0)) return new DUIntGDL(dim, BaseGDL::NOZERO);
      return new DUIntGDL(dim);
    }
    catch( GDLException ex)
      {
	throw GDLException( e->CallingNode(), "UINTARR: "+ex.getMessage());
      }
  }
  BaseGDL* lonarr( EnvT* e)
  {
    dimension dim;
    try{
      arr( e, dim); 
      if( e->KeywordSet(0)) return new DLongGDL(dim, BaseGDL::NOZERO);
      return new DLongGDL(dim);
    }
    catch( GDLException ex)
      {
	throw GDLException( e->CallingNode(), "LONARR: "+ex.getMessage());
      }
  }
  BaseGDL* ulonarr( EnvT* e)
  {
    dimension dim;
    try{
      arr( e, dim); 
      if( e->KeywordSet(0)) return new DULongGDL(dim, BaseGDL::NOZERO);
      return new DLongGDL(dim);
    }
    catch( GDLException ex)
      {
	throw GDLException( e->CallingNode(), "ULONARR: "+ex.getMessage());
      }
  }
  BaseGDL* lon64arr( EnvT* e)
  {
    dimension dim;
    try{
      arr( e, dim); 
      if( e->KeywordSet(0)) return new DLong64GDL(dim, BaseGDL::NOZERO);
      return new DLongGDL(dim);
    }
    catch( GDLException ex)
      {
	throw GDLException( e->CallingNode(), "LON64ARR: "+ex.getMessage());
      }
  }
  BaseGDL* ulon64arr( EnvT* e)
  {
    dimension dim;
    try{
      arr( e, dim); 
      if( e->KeywordSet(0)) return new DULong64GDL(dim, BaseGDL::NOZERO);
      return new DLongGDL(dim);
    }
    catch( GDLException ex)
      {
	throw GDLException( e->CallingNode(), "ULON64ARR: "+ex.getMessage());
      }
  }
  BaseGDL* fltarr( EnvT* e)
  {
    dimension dim;
    try{
      arr( e, dim); 
      if( e->KeywordSet(0)) return new DFloatGDL(dim, BaseGDL::NOZERO);
      return new DFloatGDL(dim);
    }
    catch( GDLException ex)
      {
	throw GDLException( e->CallingNode(), "FLTARR: "+ex.getMessage());
      }
  }
  BaseGDL* dblarr( EnvT* e)
  {
    dimension dim;
    try{
      arr( e, dim); 
      if( e->KeywordSet(0)) return new DDoubleGDL(dim, BaseGDL::NOZERO);
      return new DDoubleGDL(dim);
    }
    catch( GDLException ex)
      {
	throw GDLException( e->CallingNode(), "DBLARR: "+ex.getMessage());
      }
  }
  BaseGDL* strarr( EnvT* e)
  {
    dimension dim;
    try{
      arr( e, dim); 
      if( e->KeywordSet(0)) 
	throw GDLException( e->CallingNode(), "Keyword parameters not allowed in call.");
      return new DStringGDL(dim);
    }
    catch( GDLException ex)
      {
	throw GDLException( e->CallingNode(), "STRARR: "+ex.getMessage());
      }
  }
  BaseGDL* complexarr( EnvT* e)
  {
    dimension dim;
    try{
      arr( e, dim); 
      if( e->KeywordSet(0)) return new DComplexGDL(dim, BaseGDL::NOZERO);
      return new DComplexGDL(dim);
    }
    catch( GDLException ex)
      {
	throw GDLException( e->CallingNode(), "COMPLEXARR: "+ex.getMessage());
      }
  }
  BaseGDL* dcomplexarr( EnvT* e)
  {
    dimension dim;
    try{
      arr( e, dim); 
      if( e->KeywordSet(0)) return new DComplexDblGDL(dim, BaseGDL::NOZERO);
      return new DComplexDblGDL(dim);
    }
    catch( GDLException ex)
      {
	throw GDLException( e->CallingNode(), "DCOMPLEXARR: "+ex.getMessage());
      }
  }
  BaseGDL* ptrarr( EnvT* e)
  {
    dimension dim;
    try{
      arr( e, dim); 

      DPtrGDL* ret;

      if( e->KeywordSet(0) || e->KeywordSet(1)) 
	ret= new DPtrGDL(dim, BaseGDL::NOZERO);
      else
	return new DPtrGDL(dim);

      if( e->KeywordSet(1))
	{
	  SizeT nEl=ret->N_Elements();
	  SizeT sIx=e->NewHeap(nEl);
	  for( SizeT i=0; i<nEl; i++) (*ret)[i]=sIx++;
	}
      return ret;
    }
    catch( GDLException ex)
      {
	throw GDLException( e->CallingNode(), "PTRARR: "+ex.getMessage());
      }
  }
  BaseGDL* objarr( EnvT* e)
  {
    dimension dim;
    try{
      arr( e, dim); 
      if( e->KeywordSet(0)) return new DObjGDL(dim, BaseGDL::NOZERO);
      return new DObjGDL(dim);
    }
    catch( GDLException ex)
      {
	throw GDLException( e->CallingNode(), "OBJARR: "+ex.getMessage());
      }
  }

  BaseGDL* ptr_new( EnvT* e)
  {
    int nParam=e->NParam();
    
    if( nParam > 0)
      {
	if( e->KeywordSet(0)) // NO_COPY
	  {
	    BaseGDL** p= &e->GetPar( 0);
	    if( *p == NULL)
	      throw GDLException( e->CallingNode(), "Parameter undefined: "+e->GetParString(0));

	    DPtr heapID= e->NewHeap( 1, *p);
	    *p=NULL;
	    return new DPtrGDL( heapID);
	  }
	else
	  {
	    BaseGDL* p= e->GetParDefined( 0);

	    DPtr heapID= e->NewHeap( 1, p->Dup());
	    return new DPtrGDL( heapID);
	  }
      }
    else
      {
	if( e->KeywordSet(1)) // ALLOCATE_HEAP
	  {
	    DPtr heapID= e->NewHeap();
	    return new DPtrGDL( heapID);
	  }
	else
	  {
	    return new DPtrGDL( 0); // null ptr
	  }
      }
  }

  BaseGDL* ptr_valid( EnvT* e)
  {
    int nParam=e->NParam();
    
    if( e->KeywordPresent( 1)) // COUNT
      {
	e->SetKW( 1, new DLongGDL( e->Interpreter()->HeapSize()));
      }

    if( nParam == 0)
      {
	return e->Interpreter()->GetAllHeap();
      } 

    BaseGDL* p = e->GetPar( 0);
    if( p == NULL)
      {
	return new DByteGDL( 0);
      } 

    if( e->KeywordSet( 0)) // CAST
      {
	DLongGDL* pL = dynamic_cast<DLongGDL*>( p);
	if( pL == NULL)
	  {
	    pL = static_cast<DLongGDL*>(p->Convert2(LONG,BaseGDL::COPY)); 
	    e->Guard( pL);
	  }
	
	SizeT nEl = pL->N_Elements();
	DPtrGDL* ret = new DPtrGDL( pL->Dim()); // zero
	GDLInterpreter* interpreter = e->Interpreter();
	for( SizeT i=0; i<nEl; ++i)
	  {
	    if( interpreter->PtrValid( (*pL)[ i])) 
	      (*ret)[ i] = (*pL)[ i];
	  }
	return ret;
      }

    DPtrGDL* pPtr = dynamic_cast<DPtrGDL*>( p);
    if( pPtr == NULL)
      {
	return new DByteGDL( p->Dim()); // zero
      }

    SizeT nEl = pPtr->N_Elements();
    DByteGDL* ret = new DByteGDL( pPtr->Dim()); // zero
    GDLInterpreter* interpreter = e->Interpreter();
    for( SizeT i=0; i<nEl; ++i)
      {
	if( interpreter->PtrValid( (*pPtr)[ i])) 
	  (*ret)[ i] = 1;
      }
    return ret;
  }

  BaseGDL* obj_valid( EnvT* e)
  {
    int nParam=e->NParam();
    
    if( e->KeywordPresent( 1)) // COUNT
      {
	e->SetKW( 1, new DLongGDL( e->Interpreter()->ObjHeapSize()));
      }

    if( nParam == 0)
      {
	return e->Interpreter()->GetAllObjHeap();
      } 

    BaseGDL* p = e->GetPar( 0);
    if( p == NULL)
      {
	return new DByteGDL( 0);
      } 

    if( e->KeywordSet( 0)) // CAST
      {
	DLongGDL* pL = dynamic_cast<DLongGDL*>( p);
	if( pL == NULL)
	  {
	    pL = static_cast<DLongGDL*>(p->Convert2(LONG,BaseGDL::COPY)); 
	    e->Guard( pL);
	  }
	
	SizeT nEl = pL->N_Elements();
	DObjGDL* ret = new DObjGDL( pL->Dim()); // zero
	GDLInterpreter* interpreter = e->Interpreter();
	for( SizeT i=0; i<nEl; ++i)
	  {
	    if( interpreter->ObjValid( (*pL)[ i])) 
	      (*ret)[ i] = (*pL)[ i];
	  }
	return ret;
      }

    DObjGDL* pObj = dynamic_cast<DObjGDL*>( p);
    if( pObj == NULL)
      {
	return new DByteGDL( p->Dim()); // zero
      }

    SizeT nEl = pObj->N_Elements();
    DByteGDL* ret = new DByteGDL( pObj->Dim()); // zero
    GDLInterpreter* interpreter = e->Interpreter();
    for( SizeT i=0; i<nEl; ++i)
      {
	if( interpreter->ObjValid( (*pObj)[ i])) 
	  (*ret)[ i] = 1;
      }
    return ret;
  }

  BaseGDL* obj_new( EnvT* e)
  {
    StackGuard<EnvStackT> guard( e->Interpreter()->CallStack());
    
    int nParam=e->NParam();
    
    if( nParam == 0)
      {
	return new DObjGDL( 0);
      }
    
    DString objName;
    e->AssureScalarPar<DStringGDL>( 0, objName);

    // this is a struct name -> convert to UPPERCASE
    objName=StrUpCase(objName);

    DStructDesc* objDesc=e->Interpreter()->GetStruct( objName, e->CallingNode());

    DStructGDL* objStruct= new DStructGDL( objDesc);

    DObj objID= e->NewObjHeap( 1, objStruct); // owns objStruct

    BaseGDL* newObj = new DObjGDL( objID); // the object

    try {
      // call INIT function
      DFun* objINIT= objDesc->GetFun( "INIT");
      if( objINIT != NULL)
	{
	  // morph to obj environment and push it onto the stack again
	  e->PushNewEnv( objINIT, 1, &newObj);
	
	  BaseGDL* res=e->Interpreter()->call_fun( objINIT->GetTree());
	
	  if( res == NULL || (!res->Scalar()) || res->False())
	    {
	      delete res;
	      return new DObjGDL( 0);
	    }
	  delete res;
	}
    } catch(...) {
      e->FreeObjHeap( objID); // newObj might be changed
      delete newObj;
      throw;
    }

    return newObj;
  }

  BaseGDL* bindgen( EnvT* e)
  {
    dimension dim;
    try{
      arr( e, dim); 
      return new DByteGDL(dim, BaseGDL::INDGEN);
    }
    catch( GDLException ex)
      {
	throw GDLException( e->CallingNode(), "BINDGEN: "+ex.getMessage());
      }
  }
  // keywords not supported yet
  BaseGDL* indgen( EnvT* e)
  {
    dimension dim;
    try{
      arr( e, dim); 
      return new DIntGDL(dim, BaseGDL::INDGEN);
    }
    catch( GDLException ex)
      {
	throw GDLException( e->CallingNode(), "INDGEN: "+ex.getMessage());
      }
  }
  BaseGDL* uindgen( EnvT* e)
  {
    dimension dim;
    try{
      arr( e, dim); 
      return new DUIntGDL(dim, BaseGDL::INDGEN);
    }
    catch( GDLException ex)
      {
	throw GDLException( e->CallingNode(), "UINDGEN: "+ex.getMessage());
      }
  }
  BaseGDL* sindgen( EnvT* e)
  {
    dimension dim;
    try{
      arr( e, dim); 
      DULongGDL* iGen = new DULongGDL(dim, BaseGDL::INDGEN);
      return iGen->Convert2( STRING);
    }
    catch( GDLException ex)
      {
	throw GDLException( e->CallingNode(), "SINDGEN: "+ex.getMessage());
      }
  }
  BaseGDL* lindgen( EnvT* e)
  {
    dimension dim;
    try{
      arr( e, dim); 
      return new DLongGDL(dim, BaseGDL::INDGEN);
    }
    catch( GDLException ex)
      {
	throw GDLException( e->CallingNode(), "LINDGEN: "+ex.getMessage());
      }
  }
  BaseGDL* ulindgen( EnvT* e)
  {
    dimension dim;
    try{
      arr( e, dim); 
      return new DULongGDL(dim, BaseGDL::INDGEN);
    }
    catch( GDLException ex)
      {
	throw GDLException( e->CallingNode(), "ULINDGEN: "+ex.getMessage());
      }
  }
  BaseGDL* l64indgen( EnvT* e)
  {
    dimension dim;
    try{
      arr( e, dim); 
      return new DLong64GDL(dim, BaseGDL::INDGEN);
    }
    catch( GDLException ex)
      {
	throw GDLException( e->CallingNode(), "L64INDGEN: "+ex.getMessage());
      }
  }
  BaseGDL* ul64indgen( EnvT* e)
  {
    dimension dim;
    try{
      arr( e, dim); 
      return new DULong64GDL(dim, BaseGDL::INDGEN);
    }
    catch( GDLException ex)
      {
	throw GDLException( e->CallingNode(), "UL64INDGEN: "+ex.getMessage());
      }
  }
  BaseGDL* findgen( EnvT* e)
  {
    dimension dim;
    try{
      arr( e, dim); 
      return new DFloatGDL(dim, BaseGDL::INDGEN);
    }
    catch( GDLException ex)
      {
	throw GDLException( e->CallingNode(), "FINDGEN: "+ex.getMessage());
      }
  }
  BaseGDL* dindgen( EnvT* e)
  {
    dimension dim;
    try{
      arr( e, dim); 
      return new DDoubleGDL(dim, BaseGDL::INDGEN);
    }
    catch( GDLException ex)
      {
	throw GDLException( e->CallingNode(), "DINDGEN: "+ex.getMessage());
      }
  }
  BaseGDL* cindgen( EnvT* e)
  {
    dimension dim;
    try{
      arr( e, dim); 
      return new DComplexGDL(dim, BaseGDL::INDGEN);
    }
    catch( GDLException ex)
      {
	throw GDLException( e->CallingNode(), "CINDGEN: "+ex.getMessage());
      }
  }
  BaseGDL* dcindgen( EnvT* e)
  {
    dimension dim;
    try{
      arr( e, dim); 
      return new DComplexDblGDL(dim, BaseGDL::INDGEN);
    }
    catch( GDLException ex)
      {
	throw GDLException( e->CallingNode(), "DCINDGEN: "+ex.getMessage());
      }
  }

  BaseGDL* n_elements( EnvT* e)
  {
    SizeT nParam=e->NParam();

    if( nParam != 1)
      throw GDLException( e->CallingNode(), 
			  "N_ELEMENTS: Incorrect number of arguments.");

    BaseGDL* p0=e->GetPar( 0);

    if( p0 == NULL) return new DLongGDL( 0);
    
    return new DLongGDL( p0->N_Elements());
  }

  template< typename ComplexGDL, typename Complex, typename Float>
  BaseGDL* complex_fun_template( EnvT* e)
  {
    SizeT nParam=e->NParam( 1);
  
    if( nParam <= 2)
      {
	if( nParam == 2)
	  {
	    BaseGDL* p0=e->GetParDefined( 0);
	    BaseGDL* p1=e->GetParDefined( 1);
	    auto_ptr<Float> p0Float( static_cast<Float*>
				     (p0->Convert2( Float::t,BaseGDL::COPY)));
	    auto_ptr<Float> p1Float( static_cast<Float*>
				     (p1->Convert2( Float::t,BaseGDL::COPY)));

	    if( p0Float->N_Elements() >= p1Float->N_Elements())
	      {
		ComplexGDL* res = new ComplexGDL( p1Float->Dim(), 
						  BaseGDL::NOZERO);

		SizeT nE=p1Float->N_Elements();
		for( SizeT i=0; i<nE; i++)
		  {
		    (*res)[i]=Complex( (*p0Float)[i], (*p1Float)[i]);
		  }
		return res;
	      }
	    else
	      {
		ComplexGDL* res = new ComplexGDL( p0Float->Dim(), 
						  BaseGDL::NOZERO);
		
		SizeT nE=p0Float->N_Elements();
		for( SizeT i=0; i<nE; i++)
		  {
		    (*res)[i]=Complex( (*p0Float)[i], (*p1Float)[i]);
		  }
		return res;
	      }
	  }
	else
	  {
	    BaseGDL* p0 = e->GetParDefined( 0);
	    return p0->Convert2( ComplexGDL::t, BaseGDL::COPY);
	  }
      }
    else // COMPLEX( expr, offs, dim1,..,dim8)
      {
	BaseGDL* p0 = e->GetParDefined( 0);
	// *** WRONG: with offs data is converted bytewise
	auto_ptr<Float> p0Float(static_cast<Float*>
				(p0->Convert2( Float::t,
					       BaseGDL::COPY)));
	DLong offs;
	e->AssureLongScalarPar( 1, offs);
      
	dimension dim;
	arr( e, dim, 2);

	SizeT nElCreate=dim.N_Elements();
	
	SizeT nElSource=p0->N_Elements();
      
	if( (offs+2*nElCreate) > nElSource)
	  e->Throw( "Specified offset to"
		    " array is out of range: "+e->GetParString(0));
	
	ComplexGDL* res=new ComplexGDL( dim, BaseGDL::NOZERO);
	
	for( SizeT i=0; i<nElCreate; i++)
	  {
	    SizeT srcIx=2*i+offs;
	    (*res)[i]=Complex( (*p0Float)[srcIx], (*p0Float)[srcIx+1]);
	  }
	
	return res;
      }
  }

  BaseGDL* complex_fun( EnvT* e)
  {
    return complex_fun_template< DComplexGDL, DComplex, DFloatGDL>( e);
  }
  BaseGDL* dcomplex_fun( EnvT* e)
  {
    return complex_fun_template< DComplexDblGDL, DComplexDbl, DDoubleGDL>( e);
  }

  template< class TargetClass>
  BaseGDL* type_fun( EnvT* e)
  {
    SizeT nParam=e->NParam(1);

    if( nParam == 1)
      {
	BaseGDL* p0=e->GetParDefined( 0);
	// type_fun( expr) just convert
	return p0->Convert2( TargetClass::t, BaseGDL::COPY);
      }
    
    BaseGDL* p0=e->GetNumericParDefined( 0);

    // BYTE( expr, offs, dim1,..,dim8)
    DLong offs;
    e->AssureLongScalarPar( 1, offs);

    dimension dim;
    arr( e, dim, 2);
    
    TargetClass* res=new TargetClass( dim, BaseGDL::NOZERO);

    SizeT nByteCreate=res->NBytes(); // net size of new data
      
    SizeT nByteSource=p0->NBytes(); // net size of src
      
    if( (offs+nByteCreate) > nByteSource)
      {
	delete res;
	e->Throw( "Specified offset to"
		  " expression is out of range: "+e->GetParString(0));
      }

    //*** POSSIBLE ERROR because of alignment here
    void* srcAddr = static_cast<void*>( static_cast<char*>(p0->DataAddr()) + 
					offs);
    void* dstAddr = static_cast<void*>(&(*res)[0]);
    memcpy( dstAddr, srcAddr, nByteCreate);

//     char* srcAddr = reinterpret_cast<char*>(p0->DataAddr());
//     char* dstAddr = reinterpret_cast<char*>(&(*res)[0]);
//     copy( srcAddr, srcAddr+nByteCreate, dstAddr);

    return res;
  }

  BaseGDL* byte_fun( EnvT* e)
  {
    return type_fun<DByteGDL>( e);
  }
  BaseGDL* fix_fun( EnvT* e)
  {
    DIntGDL* type = e->IfDefGetKWAs<DIntGDL>( 0);
    if (type != NULL) {
      int typ = (*type)[0];
      if (typ == BYTE) return type_fun<DByteGDL>( e);
      if (typ == INT) return type_fun<DIntGDL>( e);
      if (typ == UINT) return type_fun<DUIntGDL>( e);
      if (typ == LONG) return type_fun<DLongGDL>( e);
      if (typ == ULONG) return type_fun<DULongGDL>( e);
      if (typ == LONG64) return type_fun<DLong64GDL>( e);
      if (typ == ULONG64) return type_fun<DULong64GDL>( e);
      if (typ == FLOAT) return type_fun<DFloatGDL>( e);
      if (typ == DOUBLE) return type_fun<DDoubleGDL>( e);
      if (typ == COMPLEX) return type_fun<DComplexGDL>( e);
      if (typ == COMPLEXDBL) return type_fun<DComplexDblGDL>( e);
      throw GDLException( e->CallingNode(), 
			  "FIX: Improper TYPE value.");
    }
    return type_fun<DIntGDL>( e);
  }
  BaseGDL* uint_fun( EnvT* e)
  {
    return type_fun<DUIntGDL>( e);
  }
  BaseGDL* long_fun( EnvT* e)
  {
    return type_fun<DLongGDL>( e);
  }
  BaseGDL* ulong_fun( EnvT* e)
  {
    return type_fun<DULongGDL>( e);
  }
  BaseGDL* long64_fun( EnvT* e)
  {
    return type_fun<DLong64GDL>( e);
  }
  BaseGDL* ulong64_fun( EnvT* e)
  {
    return type_fun<DULong64GDL>( e);
  }
  BaseGDL* float_fun( EnvT* e)
  {
    return type_fun<DFloatGDL>( e);
  }
  BaseGDL* double_fun( EnvT* e)
  {
    return type_fun<DDoubleGDL>( e);
  }
  // STRING function behaves different
  BaseGDL* string_fun( EnvT* e)
  {
    SizeT nParam=e->NParam();

    if( nParam == 0)
      throw GDLException( e->CallingNode(), 
			  "STRING: Incorrect number of arguments.");

    bool printKey =  e->KeywordSet( 4);
    if( printKey || (e->GetKW( 0) != NULL)) // PRINT or FORMAT
      {
	stringstream os;

	SizeT width = 0;
	if( printKey) // otherwise: FORMAT -> width is ignored
	  {
	    width = TermWidth();
	  }
	
	print_os( &os, e, 0, width);
	
	deque<DString> buf;
	while( os.good())
	  {
	    string line;
	    getline( os, line);
	    if( os.good()) buf.push_back( line);
	  }

	SizeT bufSize = buf.size();
	if( bufSize == 0)
	  throw GDLException( "Internal error: STRING: print buffer empty.");

	if( bufSize > 1) 
	  {
	    DStringGDL* retVal = 
	      new DStringGDL( dimension( bufSize), BaseGDL::NOZERO);

	    for( SizeT i=0; i<bufSize; ++i)
	      (*retVal)[ i] = buf[ i];

	    return retVal;
	  }
	else
	  return new DStringGDL( buf[0]);
      }
    else
      {
	if( nParam == 1) // nParam == 1 -> conversion
	  {
	    BaseGDL* p0 = e->GetParDefined( 0);
	    return p0->Convert2( STRING, BaseGDL::COPY);
	  }
	else // concatenation
	  {
	    DString s;
	    for( SizeT i=0; i<nParam; ++i)
	      {
		BaseGDL* p = e->GetParDefined( i);
		DStringGDL* sP = static_cast<DStringGDL*>
		  ( p->Convert2(STRING,
				BaseGDL::COPY_BYTE_AS_INT));

		SizeT nEl = sP->N_Elements();
		for( SizeT e=0; e<nEl; ++e)
		    s += (*sP)[ e];
		delete sP;
	      }
	    // IDL here breaks the string into tty-width substrings
	    return new DStringGDL( s);
	  }
      }
  }

  BaseGDL* call_function( EnvT* e)
  {
    StackGuard<EnvStackT> guard( e->Interpreter()->CallStack());

    int nParam=e->NParam();
    if( nParam == 0)
      throw GDLException( e->CallingNode(),
			  "CALL_FUNCTION: No function specified.");
    
    DString callF;
    e->AssureScalarPar<DStringGDL>( 0, callF);

    // this is a function name -> convert to UPPERCASE
    callF = StrUpCase( callF);

    // first search library funcedures
    int funIx=LibFunIx( callF);
    if( funIx != -1)
      {
	e->PushNewEnv( libFunList[ funIx], 1);
	
	// make the call
	EnvT* newEnv = e->Interpreter()->CallStack().back();
	return static_cast<DLibFun*>(newEnv->GetPro())->Fun()(newEnv);
      }
    else
      {
	funIx = GDLInterpreter::GetFunIx( callF);
	
	e->PushNewEnv( funList[ funIx], 1);
	
	// make the call
	EnvT* newEnv = e->Interpreter()->CallStack().back();
	return e->Interpreter()->
	  call_fun(static_cast<DSubUD*>(newEnv->GetPro())->GetTree());
      }
  }

  BaseGDL* call_method_function( EnvT* e)
  {
    StackGuard<EnvStackT> guard( e->Interpreter()->CallStack());

    int nParam=e->NParam();
    if( nParam < 2)
      throw GDLException( e->CallingNode(),
			  "CALL_METHOD: Name and object reference"
			  " must be specified.");
    
    DString callP;
    e->AssureScalarPar<DStringGDL>( 0, callP);

    // this is a procedure name -> convert to UPPERCASE
    callP = StrUpCase( callP);
    
    DStructGDL* oStruct = e->GetObjectPar( 1);

    DFun* method= oStruct->Desc()->GetFun( callP);

    if( method == NULL)
      throw GDLException( e->CallingNode(), "Method not found: "+callP);

    e->PushNewEnv( method, 2, &e->GetPar( 1));
    
    // make the call
    return e->Interpreter()->call_fun( method->GetTree());
  }

  BaseGDL* execute( EnvT* e)
  {
    int nParam=e->NParam( 1);

    bool quietCompile = false;
    if( nParam == 2)
      {
	BaseGDL* p1 = e->GetParDefined( 1);
	
	if( !p1->Scalar())
	  throw GDLException( e->CallingNode(), "Expression"
			      " must be scalar in this context: "+
			      e->GetParString(1));
	
	quietCompile = p1->True();
      }
    
    DString line;
    e->AssureScalarPar<DStringGDL>( 0, line);

    // remove current environment (own one)
    EnvT* caller = e->Caller();
    e->Interpreter()->CallStack().pop_back();
    delete e;

    istringstream istr(line+"\n");

    RefDNode theAST;
    try {  
      GDLLexer   lexer(istr, "");
      GDLParser& parser=lexer.Parser();
    
      parser.interactive();
    
      theAST=parser.getAST();
    }
    catch( GDLException ex)
      {
	if( !quietCompile) GDLInterpreter::ReportCompileError( ex);
	return new DIntGDL( 0);
      }
    catch( ANTLRException ex)
      {
	if( !quietCompile) cerr << "EXECUTE: Lexer/Parser exception: " <<  
			     ex.getMessage() << endl;
	return new DIntGDL( 0);
      }
    
    if( theAST == NULL) return new DIntGDL( 1);

    RefDNode trAST;
    try
      {
	GDLTreeParser treeParser( caller);
	  
	treeParser.interactive(theAST);

	trAST=treeParser.getAST();
      }
    catch( GDLException ex)
      {
	if( !quietCompile) GDLInterpreter::ReportCompileError( ex);
	return new DIntGDL( 0);
      }

    catch( ANTLRException ex)
      {
	if( !quietCompile) cerr << "EXECUTE: Compiler exception: " <<  
			     ex.getMessage() << endl;
	return new DIntGDL( 0);
      }
      
    if( trAST == NULL) return new DIntGDL( 1);

    try
      {
	caller->Interpreter()->execute( trAST);

	return new DIntGDL( 1);
      }
    catch( GDLException ex)
      {
	if( !quietCompile) cerr << "EXECUTE: Unhandled GDL exception: " <<  
			     ex.getMessage() << endl;
	return new DIntGDL( 0);
      }
    catch( ANTLRException ex)
      {
	if( !quietCompile) cerr << "EXECUTE: Interpreter exception: " <<  
			     ex.getMessage() << endl;
	return new DIntGDL( 0);
      }

    return new DIntGDL( 0); // control flow cannot reach here - compiler shut up
  }

  BaseGDL* assoc( EnvT* e)
  {
    SizeT nParam=e->NParam();
    if( nParam < 2)
      throw GDLException( e->CallingNode(),
			  "ASSOC: Incorrect number of arguments.");
    DLong lun;
    e->AssureLongScalarPar( 0, lun);

    bool stdLun = check_lun( e, lun);
    if( stdLun)
      throw GDLException( e->CallingNode(),
			  "ASSOC: File unit does not allow"
			  " this operation. Unit: "+i2s( lun));

    SizeT offset = 0;
    if( nParam >= 3) e->AssureLongScalarPar( 2, lun);
    
    BaseGDL* arr = e->GetParDefined( 1);
    
    if( arr->Scalar())
      throw GDLException( e->CallingNode(),
			  "ASSOC: Scalar variable not allowed in this"
			  " context: "+e->GetParString(1));
    
    return arr->AssocVar( lun, offset);
  }

  // gdl_ naming because of weired namespace problem in MSVC
  BaseGDL* gdl_logical_and( EnvT* e)
  {
    SizeT nParam=e->NParam();
    if( nParam != 2)
      throw GDLException( e->CallingNode(),
			  "LOGICAL_AND: Incorrect number of arguments.");

    BaseGDL* e1=e->GetParDefined( 0);//, "LOGICAL_AND");
    BaseGDL* e2=e->GetParDefined( 1);//, "LOGICAL_AND");

    ULong nEl1 = e1->N_Elements();
    ULong nEl2 = e2->N_Elements();

    Data_<SpDByte>* res;

    if( e1->Scalar()) 
      {
	if( e1->LogTrue(0)) 
	  {
	    res= new Data_<SpDByte>( e2->Dim(), BaseGDL::NOZERO);
	    for( SizeT i=0; i < nEl2; i++)
	      (*res)[i] = e2->LogTrue( i) ? 1 : 0;
	  }
	else
	  {
	    return new Data_<SpDByte>( e2->Dim());
	  }
      }
    else if( e2->Scalar()) 
      {
	if( e2->LogTrue(0)) 
	  {
	    res= new Data_<SpDByte>( e1->Dim(), BaseGDL::NOZERO);
	    for( SizeT i=0; i < nEl1; i++)
	      (*res)[i] = e1->LogTrue( i) ? 1 : 0;
	  }
	else
	  {
	    return new Data_<SpDByte>( e1->Dim());
	  }
      }
    else if( nEl2 < nEl1) 
      {
	res= new Data_<SpDByte>( e2->Dim(), BaseGDL::NOZERO);
	for( SizeT i=0; i < nEl2; i++)
	  (*res)[i] = (e1->LogTrue( i) && e2->LogTrue( i)) ? 1 : 0;
      }
    else // ( nEl2 >= nEl1)
      {
	res= new Data_<SpDByte>( e1->Dim(), BaseGDL::NOZERO);
	for( SizeT i=0; i < nEl1; i++)
	  (*res)[i] = (e1->LogTrue( i) && e2->LogTrue( i)) ? 1 : 0;
      }
  return res;
  }

  // gdl_ naming because of weired namespace problem in MSVC
  BaseGDL* gdl_logical_or( EnvT* e)
  {
    SizeT nParam=e->NParam();
    if( nParam != 2)
      throw GDLException( e->CallingNode(),
			  "LOGICAL_OR: Incorrect number of arguments.");

    BaseGDL* e1=e->GetParDefined( 0);//, "LOGICAL_OR");
    BaseGDL* e2=e->GetParDefined( 1);//, "LOGICAL_OR");

    ULong nEl1 = e1->N_Elements();
    ULong nEl2 = e2->N_Elements();

    Data_<SpDByte>* res;

    if( e1->Scalar()) 
      {
	if( e1->LogTrue(0)) 
	  {
	    res= new Data_<SpDByte>( e2->Dim(), BaseGDL::NOZERO);
	    for( SizeT i=0; i < nEl2; i++)
	      (*res)[i] = 1;
	  }
	else
	  {
	    res= new Data_<SpDByte>( e2->Dim(), BaseGDL::NOZERO);
	    for( SizeT i=0; i < nEl2; i++)
	      (*res)[i] = e2->LogTrue( i) ? 1 : 0;
	  }
      }
    else if( e2->Scalar()) 
      {
	if( e2->LogTrue(0)) 
	  {
	    res= new Data_<SpDByte>( e1->Dim(), BaseGDL::NOZERO);
	    for( SizeT i=0; i < nEl1; i++)
	      (*res)[i] = 1;
	  }
	else
	  {
	    res= new Data_<SpDByte>( e1->Dim(), BaseGDL::NOZERO);
	    for( SizeT i=0; i < nEl1; i++)
	      (*res)[i] = e1->LogTrue( i) ? 1 : 0;
	  }
      }
    else if( nEl2 < nEl1) 
      {
	res= new Data_<SpDByte>( e2->Dim(), BaseGDL::NOZERO);
	for( SizeT i=0; i < nEl2; i++)
	  (*res)[i] = (e1->LogTrue( i) || e2->LogTrue( i)) ? 1 : 0;
      }
    else // ( nEl2 >= nEl1)
      {
	res= new Data_<SpDByte>( e1->Dim(), BaseGDL::NOZERO);
	for( SizeT i=0; i < nEl1; i++)
	  (*res)[i] = (e1->LogTrue( i) || e2->LogTrue( i)) ? 1 : 0;
      }
    return res;
  }

  BaseGDL* logical_true( EnvT* e)
  {
    SizeT nParam=e->NParam();
    if( nParam != 1)
      throw GDLException( e->CallingNode(),
			  "LOGICAL_TRUE: Incorrect number of arguments.");

    BaseGDL* e1=e->GetParDefined( 0);//, "LOGICAL_TRUE");
    
    ULong nEl1 = e1->N_Elements();

    Data_<SpDByte>* res = new Data_<SpDByte>( e1->Dim(), BaseGDL::NOZERO);
    for( SizeT i=0; i < nEl1; i++)
      (*res)[i] = e1->LogTrue( i) ? 1 : 0;
    
    return res;
  }

  BaseGDL* replicate( EnvT* e)
  {
    SizeT nParam=e->NParam();
    if( nParam < 2)
      throw GDLException( e->CallingNode(),
			  "REPLICATE: Incorrect number of arguments.");
    dimension dim;
    arr( e, dim, 1);

    BaseGDL* p0=e->GetParDefined( 0);//, "REPLICATE");
    if( !p0->Scalar())
      throw GDLException( e->CallingNode(),
			  "REPLICATE: Expression must be a "
			  "scalar in this context: "+
			  e->GetParString(0));

    return p0->New( dim, BaseGDL::INIT);
  }

  BaseGDL* strtrim( EnvT* e)
  {
    SizeT nParam = e->NParam( 1);//, "STRTRIM");

    BaseGDL* p0 = e->GetPar( 0);
    if( p0 == NULL)
      throw GDLException( e->CallingNode(),
			  "STRTRIM: Variable is undefined: "+
			  e->GetParString(0));
    DStringGDL* p0S = static_cast<DStringGDL*>
      (p0->Convert2(STRING,BaseGDL::COPY));
    
    DLong mode = 0;
    if( nParam == 2)
      {
	BaseGDL* p1 = e->GetPar( 1);
	if( p1 == NULL)
	  throw GDLException( e->CallingNode(),
			      "Variable is undefined: "+e->GetParString(1));
	if( !p1->Scalar())
	  throw GDLException( e->CallingNode(),
			      "STRTRIM: Expression must be a "
			      "scalar in this context: "+
			      e->GetParString(1));
	DLongGDL* p1L = static_cast<DLongGDL*>
	  (p1->Convert2(LONG,BaseGDL::COPY));

	mode = (*p1L)[ 0];

	delete p1L;

	if( mode < 0 || mode > 2)
	  {
	    ostringstream os;
	    p1->ToStream( os);
	    throw GDLException( e->CallingNode(),
				"STRTRIM: Value of <"+ p1->TypeStr() +
				"  ("+os.str()+
				")> is out of allowed range.");
	  }
      }
    
    SizeT nEl = p0S->N_Elements();

    if( mode == 2) // both
      for( SizeT i=0; i<nEl; ++i)
	{
	  unsigned long first= (*p0S)[ i].find_first_not_of(" \t");
	  if( first == (*p0S)[ i].npos)
	    {
	      (*p0S)[ i] = "";
	    }
	  else
	    {
	      unsigned long last = (*p0S)[ i].find_last_not_of(" \t");
	      (*p0S)[ i] = (*p0S)[ i].substr(first,last-first+1);
	    }
	}
    else if( mode == 1) // leading
      for( SizeT i=0; i<nEl; ++i)
	{
	  unsigned long first= (*p0S)[ i].find_first_not_of(" \t");
	  if( first == (*p0S)[ i].npos)
	    {
	      (*p0S)[ i] = "";
	    }
	  else
	    {
	      (*p0S)[ i] = (*p0S)[ i].substr(first);
	    }
	}
    else // trailing
      for( SizeT i=0; i<nEl; ++i)
	{
	  unsigned long last = (*p0S)[ i].find_last_not_of(" \t");
	  if( last == (*p0S)[ i].npos)
	    {
	      (*p0S)[ i] = "";
	    }
	  else
	    {
	      (*p0S)[ i] = (*p0S)[ i].substr(0,last+1);
	    }
	}

    return p0S;
  }

  BaseGDL* strcompress( EnvT* e)
  {
    e->NParam( 1);

    DStringGDL* p0S = e->GetParAs<DStringGDL>( 0);

    bool removeAll =  e->KeywordSet(0);

    DStringGDL* res = new DStringGDL( p0S->Dim(), BaseGDL::NOZERO);

    SizeT nEl = p0S->N_Elements();
    for( SizeT i=0; i<nEl; ++i)
      {
	(*res)[ i] = StrCompress((*p0S)[ i], removeAll);
      }
    
    return res;
  }

  BaseGDL* strpos( EnvT* e)
  {
    e->NParam( 2);//, "STRPOS");

    bool reverseOffset =  e->KeywordSet(0); // REVERSE_OFFSET
    bool reverseSearch =  e->KeywordSet(1); // REVERSE_SEARCH

    DStringGDL* p0S = e->GetParAs<DStringGDL>( 0);

    DString searchString;
    e->AssureScalarPar<DStringGDL>( 1, searchString);

    DLong pos = string::npos;
    BaseGDL* p2 = e->GetPar( 2);
    if( p2 != NULL) e->AssureLongScalarPar( 2, pos);

    DLongGDL* res = new DLongGDL( p0S->Dim(), BaseGDL::NOZERO);

    SizeT nSrcStr = p0S->N_Elements();
    for( long i=0; i<nSrcStr; ++i)
      {
	(*res)[ i] = StrPos((*p0S)[ i], searchString, pos, 
			    reverseOffset, reverseSearch);
      }
    
    return res;
  }

  BaseGDL* strmid( EnvT* e)
  {
    e->NParam( 2);//, "STRMID");

    bool reverse =  e->KeywordSet(0);

    DStringGDL* p0S = e->GetParAs<DStringGDL>( 0);
    DLongGDL*   p1L = e->GetParAs<DLongGDL>( 1);

    BaseGDL*  p2  = e->GetPar( 2);
    DLongGDL* p2L = NULL;
    if( p2 != NULL) p2L = e->GetParAs<DLongGDL>( 2);

    DLong scVal1;
    bool sc1 = p1L->Scalar( scVal1);

    DLong scVal2 = string::npos;
    bool sc2 = true;
    if( p2L != NULL) sc2 = p2L->Scalar( scVal2);

    DLong stride;
    if( !sc1 && !sc2)
      {
	stride = p1L->Dim( 0);
	if( stride != p2L->Dim( 0))
	  throw GDLException( e->CallingNode(),
			      "STRMID: Starting offset and Length arguments "
			      "have incompatible first dimension.");	  
      }
    else
      {
	// at least one scalar, p2L possibly NULL
	if( p2L == NULL)
	  stride = p1L->Dim( 0);
	else
	  stride = max( p1L->Dim( 0), p2L->Dim( 0));
	
	stride = (stride > 0)? stride : 1;
      }

    dimension resDim( p0S->Dim());
    if( stride > 1)
      resDim >> stride;

    DStringGDL* res = new DStringGDL( resDim, BaseGDL::NOZERO);

    SizeT nEl1 = p1L->N_Elements();
    SizeT nEl2 = (sc2)? 1 : p2L->N_Elements();

    SizeT nSrcStr = p0S->N_Elements();
    for( long i=0; i<nSrcStr; ++i)
      {
	for( long ii=0; ii<stride; ++ii)
	  {
	    SizeT destIx = i * stride + ii;
	    DLong actFirst = (sc1)? scVal1 : (*p1L)[ destIx % nEl1];
	    DLong actLen   = (sc2)? scVal2 : (*p2L)[ destIx % nEl2];
	    (*res)[ destIx] = StrMid((*p0S)[ i], actFirst, actLen, reverse);
	  }
      }
    
    return res;
  }

  BaseGDL* strlowcase( EnvT* e)
  {
    e->NParam( 1);//, "STRLOWCASE");

    DStringGDL* p0S = e->GetParAs<DStringGDL>( 0);

    DStringGDL* res = new DStringGDL( p0S->Dim(), BaseGDL::NOZERO);
    
    SizeT nEl = p0S->N_Elements();
    for( SizeT i=0; i<nEl; ++i)
      {
	(*res)[ i] = StrLowCase((*p0S)[ i]);
      }
    
    return res;
  }

  BaseGDL* strupcase( EnvT* e)
  {
    e->NParam( 1);//, "STRUPCASE");

    DStringGDL* p0S = e->GetParAs<DStringGDL>( 0);

    DStringGDL* res = new DStringGDL( p0S->Dim(), BaseGDL::NOZERO);
    
    SizeT nEl = p0S->N_Elements();
    for( SizeT i=0; i<nEl; ++i)
      {
	(*res)[ i] = StrUpCase((*p0S)[ i]);
      }
    
    return res;
  }

  BaseGDL* strlen( EnvT* e)
  {
    e->NParam( 1);//, "STRLEN");

    DStringGDL* p0S = e->GetParAs<DStringGDL>( 0);

    DLongGDL* res = new DLongGDL( p0S->Dim(), BaseGDL::NOZERO);

    SizeT nEl = p0S->N_Elements();
    for( SizeT i=0; i<nEl; ++i)
      {
	(*res)[ i] = (*p0S)[ i].length();
      }
    
    return res;
  }

  BaseGDL* strjoin( EnvT* e)
  {
    SizeT nParam = e->NParam( 1);

    DStringGDL* p0S = e->GetParAs<DStringGDL>( 0);
    SizeT nEl = p0S->N_Elements();

    DString delim = "";
    if( nParam > 1)
      e->AssureStringScalarPar( 1, delim);
    
    bool single = e->KeywordSet( 0); // SINGLE

    if( single)
      {
	DStringGDL* res = new DStringGDL( (*p0S)[0]);
	DString&    scl = (*res)[0];

	for( SizeT i=1; i<nEl; ++i)
	  scl += delim + (*p0S)[i];

	return res;
      }

    dimension resDim( p0S->Dim());
    resDim.Purge();
    
    SizeT stride = resDim.Stride( 1);

    resDim.Remove( 0);

    DStringGDL* res = new DStringGDL( resDim, BaseGDL::NOZERO);
    for( SizeT src=0, dst=0; src<nEl; ++dst)
      {
	(*res)[ dst] = (*p0S)[ src++];
	for(SizeT l=1; l<stride; ++l)
	  (*res)[ dst] += delim + (*p0S)[ src++];
      }
    
    return res;
  }

  BaseGDL* where( EnvT* e)
  {
    SizeT nParam = e->NParam( 1);//, "WHERE");

    BaseGDL* p0 = e->GetParDefined( 0);//, "WHERE");

    SizeT nEl = p0->N_Elements();

    SizeT count = 0;
    SizeT nIx = nEl-1;

    DLong* ixList = new DLong[ nEl];
    ArrayGuard<DLong> guard( ixList);
    
    for( SizeT i=0; i<nEl; ++i)
      {
	if( p0->LogTrue( i))
	  {
	    ixList[ count++] = i;
	  }
	else
	  {
	    ixList[ nIx--] = i;
	  }
      }
    
    SizeT nCount = nEl - count;

    if( e->KeywordPresent( 0)) // COMPLEMENT
      {
	if( nCount == 0)
	  {
	    e->SetKW( 0, new DLongGDL( -1));
	  }
	else
	  {
	    DLongGDL* cIxList = new DLongGDL( dimension( &nCount, 1), 
					      BaseGDL::NOZERO);
	    
	    SizeT cIx = nEl - 1;
	    for( SizeT i=0; i<nCount; ++i)
	      (*cIxList)[ i] = ixList[ cIx--];
	    
	    e->SetKW( 0, cIxList);
	  }
      }

    if( e->KeywordPresent( 1)) // NCOMPLEMENT
      {
	e->SetKW( 1, new DLongGDL( nCount));
      }

    if( nParam == 2)
      {
	e->SetPar( 1, new DLongGDL( count));
      }

    if( count == 0) return new DLongGDL( -1);

    DLongGDL* res = new DLongGDL( dimension( &count, 1), 
				  BaseGDL::NOZERO);
    for( SizeT i=0; i<count; ++i)
      (*res)[ i] = ixList[ i];

    return res;
  }

  BaseGDL* n_params( EnvT* e) 
  {
    EnvT* caller = e->Caller();
    if( caller == NULL) return new DLongGDL( 0);
    DLong nP = caller->NParam();
    return new DLongGDL( nP);
  }

  BaseGDL* keyword_set( EnvT* e)
  {
    e->NParam( 1);//, "KEYWORD_SET");

    BaseGDL* p0 = e->GetPar( 0);
    if( p0 == NULL) return new DIntGDL( 0);
    if( !p0->Scalar()) return new DIntGDL( 1);
    if( p0->LogTrue()) return new DIntGDL( 1);
    return new DIntGDL( 0);
  }

  // total over one dim
  template< typename T>
  BaseGDL* total_over_dim_template( T* src, 
				    const dimension& srcDim, 
				    SizeT sumDimIx)
  {
    SizeT nEl = src->N_Elements();
    
    // get dest dim and number of summations
    dimension destDim = srcDim;
    SizeT nSum = destDim.Remove( sumDimIx);

    T* res = new T( destDim); // zero fields

    // sumStride is also the number of linear src indexing
    SizeT sumStride = srcDim.Stride( sumDimIx); 
    SizeT outerStride = srcDim.Stride( sumDimIx + 1);
    SizeT sumLimit = nSum * sumStride;
    SizeT rIx=0;
    for( SizeT o=0; o < nEl; o += outerStride)
      for( SizeT i=0; i < sumStride; ++i)
	{
	  SizeT oi = o+i;
	  SizeT oiLimit = sumLimit + oi;
	  for( SizeT s=oi; s<oiLimit; s += sumStride)
	    (*res)[ rIx] += (*src)[ s];
	  ++rIx;
	}
    return res;
  }

  // cumulative over one dim
  template< typename T>
  BaseGDL* total_over_dim_cu_template( T* res, 
				       SizeT sumDimIx)
  {
    SizeT nEl = res->N_Elements();
    const dimension& resDim = res->Dim();
    SizeT cumStride = resDim.Stride( sumDimIx); 
    SizeT outerStride = resDim.Stride( sumDimIx + 1);
    for( SizeT o=0; o < nEl; o += outerStride)
      {
	SizeT cumLimit = o+outerStride;
	for( SizeT i=o+cumStride, ii=o; i<cumLimit; ++i, ++ii)
	  (*res)[ i] += (*res)[ ii];
      }
    return res;
  }

  BaseGDL* total( EnvT* e)
  {
    SizeT nParam = e->NParam( 1);//, "TOTAL");

    BaseGDL* p0 = e->GetParDefined( 0);//, "TOTAL");

    SizeT nEl = p0->N_Elements();
    if( nEl == 0)
      throw GDLException( e->CallingNode(), 
			  "TOTAL: Variable is undefined: "+e->GetParString(0));

    if( p0->Type() == STRING)
      throw GDLException( e->CallingNode(), 
			  "TOTAL: String expression not allowed "
			  "in this context: "+e->GetParString(0));
    
    bool cumulative = e->KeywordSet( 0);
    bool doubleRes  = e->KeywordSet( 1);
    bool nan        = e->KeywordSet( 2);

    DLong sumDim = 0;
    if( nParam == 2)
      e->AssureLongScalarPar( 1, sumDim);

    if( sumDim == 0)
      {
	if( !cumulative)
	  {
	    if( p0->Type() == DOUBLE)
	      {
		return new DDoubleGDL( static_cast<DDoubleGDL*>( p0)->Sum()); 
	      }
	    if( p0->Type() == COMPLEXDBL)
	      {
		return 
		  new DComplexDblGDL( static_cast<DComplexDblGDL*>( p0)->Sum()); 
	      }
	    if( !doubleRes)
	      {
		if( p0->Type() == FLOAT)
		  {
		    return new DFloatGDL( static_cast<DFloatGDL*>( p0)->Sum()); 
		  }
		if( p0->Type() == COMPLEX)
		  {
		    return 
		      new DComplexGDL( static_cast<DComplexGDL*>( p0)->Sum()); 
		  }
		DFloatGDL* p0F = static_cast<DFloatGDL*>
		  (p0->Convert2( FLOAT,BaseGDL::COPY));
		e->Guard( p0F);
		return new DFloatGDL( p0F->Sum()); 
	      }
	    if( p0->Type() == COMPLEX)
	      {
		DComplexDblGDL* p0D = static_cast<DComplexDblGDL*>
		  (p0->Convert2( COMPLEXDBL,BaseGDL::COPY));
		e->Guard( p0D);
		return new DComplexDblGDL( p0D->Sum()); 
	      }
	    
	    DDoubleGDL* p0D = static_cast<DDoubleGDL*>
	      (p0->Convert2( DOUBLE,BaseGDL::COPY));
	    e->Guard( p0D);
	    return new DDoubleGDL( p0D->Sum()); 
	  }
	else // cumulative
	  {
	    // special case as DOUBLE type overrides /DOUBLE
	    if( p0->Type() == DOUBLE)
	      {
		DDoubleGDL* res = static_cast<DDoubleGDL*>( p0)->Dup();
		for( SizeT i=1,ii=0; i<nEl; ++i,++ii)
		  (*res)[i] += (*res)[ii];
		return res;
	      }
	    if( p0->Type() == COMPLEXDBL)
	      {
		DComplexDblGDL* res = static_cast<DComplexDblGDL*>( p0)->Dup();
		for( SizeT i=1,ii=0; i<nEl; ++i,++ii)
		  (*res)[i] += (*res)[ii];
		return res;
	      }
	    if( !doubleRes)
	      {
		// special case for FLOAT has no advatage here
		if( p0->Type() == COMPLEX)
		  {
		    DComplexGDL* res = static_cast<DComplexGDL*>( p0)->Dup();
		    for( SizeT i=1,ii=0; i<nEl; ++i,++ii)
		      (*res)[i] += (*res)[ii];
		    return res;
		  }
		DFloatGDL* res = static_cast<DFloatGDL*>
		  (p0->Convert2( FLOAT,BaseGDL::COPY));
		for( SizeT i=1,ii=0; i<nEl; ++i,++ii)
		  (*res)[i] += (*res)[ii];
		return res;
	      }
	    if( p0->Type() == COMPLEX)
	      {
		DComplexDblGDL* res = static_cast<DComplexDblGDL*>
		  (p0->Convert2( COMPLEXDBL,BaseGDL::COPY));
		for( SizeT i=1,ii=0; i<nEl; ++i,++ii)
		  (*res)[i] += (*res)[ii];
		return res;
	      }
	    DDoubleGDL* res = static_cast<DDoubleGDL*>
	      (p0->Convert2( DOUBLE,BaseGDL::COPY));
	    for( SizeT i=1,ii=0; i<nEl; ++i,++ii)
	      (*res)[i] += (*res)[ii];
	    return res;
	  }
      }

    // total over sumDim
    dimension srcDim = p0->Dim();
    SizeT srcRank = srcDim.Rank();

    if( sumDim < 1 || sumDim > srcRank)
      throw GDLException( e->CallingNode(), 
			  "TOTAL: Array must have "+i2s(sumDim)+
			  " dimensions: "+e->GetParString(0));

    if( !cumulative)
      {
	if( p0->Type() == DOUBLE)
	  {
	    return total_over_dim_template< DDoubleGDL>
	      ( static_cast<DDoubleGDL*>(p0), srcDim, sumDim-1);
	  }
	if( p0->Type() == COMPLEXDBL)
	  {
	    return total_over_dim_template< DComplexDblGDL>
	      ( static_cast<DComplexDblGDL*>(p0), srcDim, sumDim-1);
	  }
	if( !doubleRes)
	  {
	    if( p0->Type() == FLOAT)
	      {
		return total_over_dim_template< DFloatGDL>
		  ( static_cast<DFloatGDL*>(p0), srcDim, sumDim-1);
	      }
	    if( p0->Type() == COMPLEX)
	      {
		return total_over_dim_template< DComplexGDL>
		  ( static_cast<DComplexGDL*>(p0), srcDim, sumDim-1);
	      }
	    // default for NOT /DOUBLE
	    DFloatGDL* p0F = static_cast<DFloatGDL*>
	      (p0->Convert2( FLOAT,BaseGDL::COPY));
	    e->Guard( p0F);
	    return total_over_dim_template< DFloatGDL>
	      ( p0F, srcDim, sumDim-1);
	  }
	if( p0->Type() == COMPLEX)
	  {
	    DComplexDblGDL* p0D = static_cast<DComplexDblGDL*>
	      (p0->Convert2( COMPLEXDBL,BaseGDL::COPY));
	    e->Guard( p0D);
	    return total_over_dim_template< DComplexDblGDL>
	      ( p0D, srcDim, sumDim-1);
	  }
	// default for /DOUBLE
	DDoubleGDL* p0D = static_cast<DDoubleGDL*>
	  (p0->Convert2( DOUBLE,BaseGDL::COPY));
	e->Guard( p0D);
	return total_over_dim_template< DDoubleGDL>( p0D, srcDim, sumDim-1);
      }
    else // cumulative
      {
	if( p0->Type() == DOUBLE)
	  {
	    return total_over_dim_cu_template< DDoubleGDL>
	      ( static_cast<DDoubleGDL*>(p0)->Dup(), sumDim-1);
	  }
	if( p0->Type() == COMPLEXDBL)
	  {
	    return total_over_dim_cu_template< DComplexDblGDL>
	      ( static_cast<DComplexDblGDL*>(p0)->Dup(), sumDim-1);
	  }
	if( !doubleRes)
	  {
	    // special case for FLOAT has no advatage here
	    if( p0->Type() == COMPLEX)
	      {
		return total_over_dim_cu_template< DComplexGDL>
		  ( static_cast<DComplexGDL*>(p0)->Dup(), sumDim-1);
	      }
	    // default for NOT /DOUBLE
	    DFloatGDL* p0F = static_cast<DFloatGDL*>
	      (p0->Convert2( FLOAT,BaseGDL::COPY));
	    return total_over_dim_cu_template< DFloatGDL>
	      ( p0F, sumDim-1);
	  }
	if( p0->Type() == COMPLEX)
	  {
	    DComplexDblGDL* p0D = static_cast<DComplexDblGDL*>
	      (p0->Convert2( COMPLEXDBL,BaseGDL::COPY));
	    return total_over_dim_cu_template< DComplexDblGDL>
	      ( p0D, sumDim-1);
	  }
	// default for /DOUBLE
	DDoubleGDL* p0D = static_cast<DDoubleGDL*>
	  (p0->Convert2( DOUBLE,BaseGDL::COPY));
	return total_over_dim_cu_template< DDoubleGDL>( p0D, sumDim-1);
      }
  }

  BaseGDL* array_equal( EnvT* e)
  {
    e->NParam( 2);//, "ARRAY_EQUAL");

    BaseGDL* p0 = e->GetParDefined( 0);//, "ARRAY_EQUAL");
    BaseGDL* p1 = e->GetParDefined( 1);//, "ARRAY_EQUAL");

    if( p0 == p1) return new DByteGDL( 1);

    SizeT nEl0 = p0->N_Elements();
    SizeT nEl1 = p1->N_Elements();
    if( nEl0 != nEl1 && nEl0 != 1 && nEl1 != 1)
      return new DByteGDL( 0);
    
    if( p0->Type() != p1->Type())
      {
	if( e->KeywordSet( 0)) // NO_TYPECONV
	  return new DByteGDL( 0);
	else
	  {
	    DType aTy=p0->Type();
	    DType bTy=p1->Type();
	    if( DTypeOrder[aTy] >= DTypeOrder[bTy])
	      {
		p1 = p1->Convert2( aTy, BaseGDL::COPY);
		e->Guard( p1);
	      }
	    else
	      {
		p0 = p0->Convert2( bTy, BaseGDL::COPY);
		e->Guard( p0);
	      }
	  }
      }
    
    if( p0->ArrayEqual( p1)) return new DByteGDL( 1);

    return new DByteGDL( 0);
  }

  BaseGDL* min_fun( EnvT* e)
  {
    SizeT nParam = e->NParam( 1);//, "MIN");
    
    BaseGDL* searchArr = e->GetParDefined( 0);//, "MIN");

    DLong    minEl;
    BaseGDL* res;

    if( nParam == 2)
	e->AssureGlobalPar( 1);

    if( e->KeywordPresent( 0)) // MAX keyword given
      {
	e->AssureGlobalKW( 0);
	delete e->GetKW( 0);
 
	DLong maxEl;

	searchArr->MinMax( &minEl, &maxEl, &res, &e->GetKW( 0));
      }
    else // no MAX keyword
      {
	searchArr->MinMax( &minEl, NULL, &res, NULL);
      }

    // handle index
    if( nParam == 2)
      {
	delete e->GetPar( 1);
	e->GetPar( 1) = new DLongGDL( minEl);
      }
    else
      {
	SysVar::SetC( minEl);
      }

    return res;
  }

  BaseGDL* max_fun( EnvT* e)
  {
    SizeT nParam = e->NParam( 1);//, "MAX");
    
    BaseGDL* searchArr = e->GetParDefined( 0);//, "MAX");

    DLong    maxEl;
    BaseGDL* res;

    if( nParam == 2)
	e->AssureGlobalPar( 1);

    if( e->KeywordPresent( 0)) // MIN keyword given
      {
	e->AssureGlobalKW( 0);
	delete e->GetKW( 0);
 
	DLong minEl;

	searchArr->MinMax( &minEl, &maxEl, &e->GetKW( 0), &res);
      }
    else // no MAX keyword
      {
	searchArr->MinMax( NULL, &maxEl, NULL, &res);
      }

    // handle index
    if( nParam == 2)
      {
	delete e->GetPar( 1);
	e->GetPar( 1) = new DLongGDL( maxEl);
      }
    else
      {
	SysVar::SetC( maxEl);
      }

    return res;
  }
 
  BaseGDL* transpose( EnvT* e)
  {
    SizeT nParam=e->NParam( 1); 

    BaseGDL* p0 = e->GetParDefined( 0);
    if( p0->Type() == STRUCT)
      e->Throw("Struct expression not allowed in this context: "+
	       e->GetParString(0));
    
    SizeT rank = p0->Rank();
    if( rank == 0)
      e->Throw( "Expression must be an array "
		"in this context: "+ e->GetParString(0));
    
    DUInt* perm = NULL;
    auto_ptr<DUInt> perm_guard;
    if( nParam == 2) 
      {
	BaseGDL* p1 = e->GetParDefined( 1);
	if( p1->N_Elements() != rank)
	  e->Throw("Incorrect number of elements in permutation.");

	perm = new DUInt[rank];
	perm_guard.reset( perm);

	DUIntGDL* p1L = static_cast<DUIntGDL*>
	  (p1->Convert2( UINT, BaseGDL::COPY));
	for( SizeT i=0; i<rank; ++i) perm[i] = (*p1L)[ i];

	// check permutaion vector
	for( SizeT i=0; i<rank; ++i) 
	  {
	    DUInt j;
	    for( j=0; j<rank; ++j) if( perm[j] == i) break;
	    if (j == rank)
	      e->Throw( "Incorrect permutation vector.");
	  }
      }

    return p0->Transpose( perm);
  }

 

  // helper function for sort_fun, recursive
  // optimized version
  template< typename IndexT>
  void MergeSortOpt( BaseGDL* p0, IndexT* hhS, IndexT* h1, IndexT* h2,
		     SizeT len) 
  {
    if( len <= 1) return;       

    SizeT h1N = len / 2;
    SizeT h2N = len - h1N;

    // 1st half
    MergeSortOpt(p0, hhS, h1, h2, h1N);

    // 2nd half
    IndexT* hhM = &hhS[h1N]; 
    MergeSortOpt(p0, hhM, h1, h2, h2N);

    SizeT i;
    for(i=0; i<h1N; ++i) h1[i] = hhS[ i];
    for(i=0; i<h2N; ++i) h2[i] = hhM[ i];

    SizeT  h1Ix = 0;
    SizeT  h2Ix = 0;
    for( i=0; (h1Ix < h1N) && (h2Ix < h2N); ++i) 
      {
	// the actual comparisson
	if( p0->Greater( h1[h1Ix], h2[h2Ix])) 
	  hhS[ i] = h2[ h2Ix++];
	else
	  hhS[ i] = h1[ h1Ix++];
      }
    for(; h1Ix < h1N; ++i) hhS[ i] = h1[ h1Ix++];
    for(; h2Ix < h2N; ++i) hhS[ i] = h2[ h2Ix++];
  }

  // helper function for sort_fun, recursive
  void MergeSort( BaseGDL* p0, SizeT* hh, SizeT* h1, SizeT* h2,
		  SizeT start, SizeT end) 
  {
    if( start+1 >= end) return;       

    SizeT middle = (start+end) / 2;

    MergeSort(p0, hh, h1, h2, start, middle);
    MergeSort(p0, hh, h1, h2, middle, end);

    SizeT h1N = middle - start;
    SizeT h2N = end - middle;

    SizeT* hhS = &hh[start];

    SizeT i;
    for(i=0; i<h1N; ++i) h1[i] = hhS[ i];
    for(i=0; i<h2N; ++i) h2[i] = hh[middle + i];

    SizeT  h1Ix = 0;
    SizeT  h2Ix = 0;
    for( i=0; (h1Ix < h1N) && (h2Ix < h2N); ++i) 
      {
	// the actual comparisson
	if( p0->Greater( h1[h1Ix], h2[h2Ix])) 
	  hhS[ i] = h2[ h2Ix++];
	else
	  hhS[ i] = h1[ h1Ix++];
      }
    for(; h1Ix < h1N; ++i) hhS[ i] = h1[ h1Ix++];
    for(; h2Ix < h2N; ++i) hhS[ i] = h2[ h2Ix++];
  }

  // sort function uses MergeSort
  BaseGDL* sort_fun( EnvT* e)
  {
    e->NParam( 1);
    
    BaseGDL* p0 = e->GetParDefined( 0);

    if( p0->Type() == STRUCT)
      e->Throw( "Struct expression not allowed in this context: "+
		e->GetParString(0));
    
    static int l64Ix = e->KeywordIx( "L64");
    bool l64 = e->KeywordSet( l64Ix);
    
    SizeT nEl = p0->N_Elements();
    
    // helper arrays
    DLongGDL* res = new DLongGDL( dimension( nEl), BaseGDL::INDGEN);

    DLong *hh = static_cast<DLong*>(res->DataAddr());

    DLong* h1 = new DLong[ nEl/2];
    DLong* h2 = new DLong[ (nEl+1)/2];
    // call the sort routine
    MergeSortOpt<DLong>( p0, hh, h1, h2, nEl);
    delete[] h1;
    delete[] h2;

    if( l64) 
      {
	// leave it this way, as sorting of more than 2^31
	// items seems not feasible in the future we might 
	// use MergeSortOpt<DLong64>(...) for this 
	return res->Convert2( LONG64);
      }

    return res;
  }

  BaseGDL* shift_fun( EnvT* e)
  {
    SizeT nParam = e->NParam( 2);

    BaseGDL* p0 = e->GetParDefined( 0);

    SizeT nShift = nParam - 1;
    if( nShift == 1)
      {
	DLong s1;
	e->AssureLongScalarPar( 1, s1);

	return p0->CShift( s1);
      }
    
    if( p0->Rank() != nShift)
      e->Throw( "Incorrect number of arguments.");

    DLong sIx[ MAXRANK];
    for( SizeT i=0; i< nShift; i++)
	e->AssureLongScalarPar( i+1, sIx[ i]);

    return p0->CShift( sIx);
  }

  BaseGDL* arg_present( EnvT* e)
  {
    e->NParam( 1);
    
    if( !e->GlobalPar( 0))
      return new DIntGDL( 0);

    EnvT* caller = e->Caller();
    if( caller == NULL)
      return new DIntGDL( 0);

    BaseGDL** pp0 = &e->GetPar( 0);
    
    int ix = caller->FindGlobalKW( pp0);
    if( ix == -1)
      return new DIntGDL( 0);

    return new DIntGDL( 1);
  }

  BaseGDL* eof_fun( EnvT* e)
  {
    e->NParam( 1);

    DLong lun;
    e->AssureLongScalarPar( 0, lun);

    bool stdLun = check_lun( e, lun);
    if( stdLun)
      return new DIntGDL( 0);

    // nicer error message
    if( !fileUnits[ lun-1].IsOpen())
      e->Throw( "File unit is not open: "+i2s( lun)+".");

    if( fileUnits[ lun-1].Eof())
      return new DIntGDL( 1);

    return new DIntGDL( 0);
  }

  BaseGDL* convol( EnvT* e)
  {
    SizeT nParam=e->NParam( 2); 

    BaseGDL* p0 = e->GetNumericParDefined( 0);
    if( p0->Rank() == 0) 
      e->Throw( "Expression must be an array in this context: "+
		e->GetParString(0));
    
    BaseGDL* p1 = e->GetNumericParDefined( 1);
    if( p1->Rank() == 0) 
      e->Throw( "Expression must be an array in this context: "+
		e->GetParString(1));
    
    if( p0->N_Elements() <= p1->N_Elements())
      e->Throw( "Incompatible dimensions for Array and Kernel.");

    // rank 1 for kernel works always
    if( p1->Rank() != 1)
      {
	SizeT rank = p0->Rank();
	if( rank != p1->Rank())
	  e->Throw( "Incompatible dimensions for Array and Kernel.");

	for( SizeT r=0; r<rank; ++r)
	  if( p0->Dim( r) <= p1->Dim( r))
	    e->Throw( "Incompatible dimensions for Array and Kernel.");
      }

    // convert kernel to array type
    auto_ptr<BaseGDL> p1Guard;
    if( p0->Type() == BYTE)
      {
	if( p1->Type() != INT)
	  {
	    p1 = p1->Convert2( INT, BaseGDL::COPY); 
	    p1Guard.reset( p1);
	  }
      }
    else if( p0->Type() != p1->Type())
      {
	p1 = p1->Convert2( p0->Type(), BaseGDL::COPY); 
	p1Guard.reset( p1);
      }

    BaseGDL* scale;
    auto_ptr<BaseGDL> scaleGuard;
    if( nParam > 2)
      {
	scale = e->GetParDefined( 2);
	if( scale->Rank() > 0)
	  e->Throw( "Expression must be a scalar in this context: "+
		    e->GetParString(2));

	// p1 here handles BYTE case also
	if( p1->Type() != scale->Type())
	  {
	    scale = scale->Convert2( p1->Type(),BaseGDL::COPY); 
	    scaleGuard.reset( scale);
	  }
      }
    else
      {
	scale = p1->New( dimension(), BaseGDL::ZERO);
      }

    bool center = true;
    static int centerIx = e->KeywordIx( "CENTER");
    if( e->KeywordPresent( centerIx))
      {
	DLong c;
	e->AssureLongScalarKW( centerIx, c);
	center = (c != 0);
      }

    // overrides EDGE_TRUNCATE
    static int edge_wrapIx = e->KeywordIx( "EDGE_WRAP");
    bool edge_wrap = e->KeywordSet( edge_wrapIx);
    static int edge_truncateIx = e->KeywordIx( "EDGE_TRUNCATE");
    bool edge_truncate = e->KeywordSet( edge_truncateIx);

    int edgeMode = 0; 
    if( edge_wrap)
      edgeMode = 1;
    else if( edge_truncate)
      edgeMode = 2;

    // p0, p1 and scale have same type
    // p1 has rank of 1 or same rank as p0 with each dimension smaller than p0
    // scale is a scalar
    return p0->Convol( p1, scale, center, edgeMode);
  }

  BaseGDL* rebin_fun( EnvT* e)
  {
    SizeT nParam = e->NParam( 2);

    BaseGDL* p0 = e->GetNumericParDefined( 0);

    SizeT rank = p0->Rank();

    if( rank == 0) 
      e->Throw( "Expression must be an array in this context: "+
		e->GetParString(0));
    
    SizeT resDimInit[ MAXRANK];
    for( SizeT p=1; p<nParam; ++p)
      {
	DLong newDim;
	e->AssureLongScalarPar( p, newDim);

	if( newDim <= 0)
	  e->Throw( "Array dimensions must be greater than 0.");
	
	if( rank >= p)
	  {
	    SizeT oldDim = p0->Dim( p-1);

	    if( newDim > oldDim)
	      {
		if( (newDim % oldDim) != 0)
		  e->Throw( "Result dimensions must be integer factor "
			    "of original dimensions.");
	      }
	    else
	      {
		if( (oldDim % newDim) != 0)
		  e->Throw( "Result dimensions must be integer factor "
			    "of original dimensions.");
	      }
	  }
	
	resDimInit[ p-1] = newDim; 
      }

    dimension resDim( resDimInit, nParam-1);

    static int sampleIx = e->KeywordIx( "SAMPLE");
    bool sample = e->KeywordSet( sampleIx);
    
    return p0->Rebin( resDim, sample);
  }

  BaseGDL* obj_class( EnvT* e)
  {
    SizeT nParam = e->NParam();

    static int countIx = e->KeywordIx( "COUNT");
    static int superIx = e->KeywordIx( "SUPERCLASS");

    bool super = e->KeywordSet( superIx);

    bool count = e->KeywordPresent( countIx);
    if( count)
      e->AssureGlobalKW( countIx);

    if( nParam > 0)
      {
	BaseGDL* p0 = e->GetParDefined( 0);

	if( p0->Type() != STRING && p0->Type() != OBJECT)
	  e->Throw( "Argument must be a scalar object reference or string: "+
		    e->GetParString(0));

	if( !p0->Scalar())
	  e->Throw( "Expression must be a scalar or 1 element "
		    "array in this context: "+e->GetParString(0));

	DStructDesc* objDesc;

	if( p0->Type() == STRING)
	  {
	    DString objName;
	    e->AssureScalarPar<DStringGDL>( 0, objName);
	    objName = StrUpCase( objName);

	    objDesc = FindInStructList( structList, objName);
	    if( objDesc == NULL)
	      {
		if( count)
		  e->SetKW( countIx, new DLongGDL( 0));
		return new DStringGDL( "");
	      }
	  }
	else // OBJECT
	  {
	    DObj objRef;
	    e->AssureScalarPar<DObjGDL>( 0, objRef);

	    if( objRef == 0)
	      {
		if( count)
		  e->SetKW( countIx, new DLongGDL( 0));
		return new DStringGDL( "");
	      }

	    DStructGDL* oStruct;
	    try {
	      oStruct = e->GetObjHeap( objRef);
	    }
	    catch ( GDLInterpreter::HeapException)
	      { // non valid object
		if( count)
		  e->SetKW( countIx, new DLongGDL( 0));
		return new DStringGDL( "");
	      }

	    objDesc = oStruct->Desc(); // cannot be NULL
	  }

	if( !super)
	  {
	    if( count)
	      e->SetKW( countIx, new DLongGDL( 1));
	    return new DStringGDL( objDesc->Name());
	  }
	
	deque< string> pNames;
	objDesc->GetParentNames( pNames);

	SizeT nNames = pNames.size();
	    
	if( count)
	  e->SetKW( countIx, new DLongGDL( nNames));

	if( nNames == 0)
	  {
	    return new DStringGDL( "");
	  }

	DStringGDL* res = new DStringGDL( dimension( nNames), 
					  BaseGDL::NOZERO);

	for( SizeT i=0; i<nNames; ++i)
	  {
	    (*res)[i] = pNames[i];
	  }
	
	return res;
      }

    if( super)
      e->Throw( "Conflicting keywords.");

    SizeT nObj = structList.size();

    DStringGDL* res = new DStringGDL( dimension( nObj), 
				      BaseGDL::NOZERO);

    for( SizeT i=0; i<nObj; ++i)
      {
	(*res)[i] = structList[i]->Name();
      }
	
    return res;
  }

  BaseGDL* obj_isa( EnvT* e)
  {
    SizeT nParam = e->NParam( 2);

    BaseGDL* p0 = e->GetPar( 0);
    if( p0 == NULL || p0->Type() != OBJECT)
      e->Throw( "Object reference type required in this context: "+
		e->GetParString(0));

    DString className;
    e->AssureScalarPar<DStringGDL>( 1, className);
    className = StrUpCase( className);

    DObjGDL* pObj = static_cast<DObjGDL*>( p0);

    DByteGDL* res = new DByteGDL( pObj->Dim()); // zero 

    GDLInterpreter* interpreter = e->Interpreter();

    SizeT nElem = pObj->N_Elements();
    for( SizeT i=0; i<nElem; ++i)
      {
	if( interpreter->ObjValid( (*pObj)[ i])) 
	  {
	    DStructGDL* oStruct = e->GetObjHeap( (*pObj)[i]);
	    if( oStruct->Desc()->IsParent( className))
	      (*res)[i] = 1;
	  }
      }
    
    return res;
  }

  BaseGDL* n_tags( EnvT* e)
  {
    e->NParam( 1);

    BaseGDL* p0 = e->GetPar( 0);
    if( p0 == NULL)
      return new DLongGDL( 0);
    
    if( p0->Type() != STRUCT)
      return new DLongGDL( 0);
    
    DStructGDL* s = static_cast<DStructGDL*>( p0);

    static int lengthIx = e->KeywordIx( "LENGTH");
    bool length = e->KeywordSet( lengthIx);
    
    if( length)
      return new DLongGDL( s->NBytes() / s->N_Elements());

    return new DLongGDL( s->Desc()->NTags());
  }

  BaseGDL* bytscl( EnvT* e)
  {
    e->NParam( 1);

    BaseGDL* p0=e->GetNumericParDefined( 0);

    static int minIx = e->KeywordIx( "MIN");
    static int maxIx = e->KeywordIx( "MAX");
    static int topIx = e->KeywordIx( "TOP");

    DLong topL=255;
    if( e->GetKW( topIx) != NULL)
      e->AssureLongScalarKW( topIx, topL);
    DByte top = static_cast<DByte>(topL);
    DDouble dTop = static_cast<DDouble>(top);

    DDouble min;
    bool minSet = false;
    if( e->GetKW( minIx) != NULL)
      {
      e->AssureDoubleScalarKW( minIx, min);
      minSet = true;
      }

    DDouble max;
    bool maxSet = false;
    if( e->GetKW( maxIx) != NULL)
      {
      e->AssureDoubleScalarKW( maxIx, max);
      maxSet = true;
      }

    DDoubleGDL* dRes = 
      static_cast<DDoubleGDL*>(p0->Convert2( DOUBLE, BaseGDL::COPY));

    DLong maxEl, minEl;
    if( !maxSet || !minSet)
      dRes->MinMax( &minEl, &maxEl, NULL, NULL);
    if( !minSet)
      min = (*dRes)[ minEl];
    if( !maxSet)
      max = (*dRes)[ maxEl];

    SizeT nEl = dRes->N_Elements();
    for( SizeT i=0; i<nEl; ++i)
      {
	DDouble& d = (*dRes)[ i];
	if( d <= min)
	  (*dRes)[ i] = 0;
	else if( d >= max)
	  (*dRes)[ i] = dTop;
	else
	  (*dRes)[ i] =  round((d - min) / (max-min) * dTop);
      }

    return dRes->Convert2( BYTE);
  } 

  BaseGDL* strtok_fun( EnvT* e)
  {
    SizeT nParam=e->NParam( 1);
    
    DString stringIn;
    e->AssureStringScalarPar( 0, stringIn);

    DString pattern = " \t";
    if(nParam > 1) {
      e->AssureStringScalarPar( 1, pattern);
    }
    
    static int extractIx = e->KeywordIx( "EXTRACT");
    bool extract = e->KeywordSet( extractIx);

    static int lengthIx = e->KeywordIx( "LENGTH");
    bool lengthPresent = e->KeywordPresent( lengthIx);

    if( extract && lengthPresent)
      e->Throw( "Conflicting keywords.");
    
    static int pre0Ix = e->KeywordIx( "PRESERVE_NULL");
    bool pre0 = e->KeywordSet( pre0Ix);

    deque<long> tokenStart;
    deque<long> tokenLen;
 
    int strLen = stringIn.length();

    DString escape = "";
    e->AssureStringScalarKWIfPresent( "ESCAPE", escape);
    deque<long> escList;
    long pos = 0;
    while(pos != string::npos)
      {
	pos = stringIn.find_first_of( escape, pos);
	if( pos != string::npos)
	  {
	    escList.push_back( pos+1); // remember escaped char
	    pos += 2; // skip escaped char
	  }
      }
    deque<long>::iterator escBeg = escList.begin();
    deque<long>::iterator escEnd = escList.end();

    long tokB = 0;
    long nextE = 0;
    for(;;)
      {
	long tokE = stringIn.find_first_of( pattern, nextE);
	if( tokE == string::npos)
	  {
	    long actLen = strLen - tokB;
	    if( actLen > 0 || pre0)
	      {
		tokenStart.push_back( tokB);
		tokenLen.push_back( actLen);
	      }
	    break;
	  }
	
	if( find( escBeg, escEnd, tokE) == escEnd) 
	  { 
	    long actLen = tokE - tokB;
	    if( actLen > 0 || pre0)
	      {
		tokenStart.push_back( tokB);
		tokenLen.push_back( actLen);
	      }
	    tokB = tokE+1;
	  }

	nextE = tokE+1;
      } // for(;;)

    SizeT nTok = tokenStart.size();

    if( !extract)
      {    
	dimension dim(nTok);
	if( lengthPresent) 
	  {
	    e->AssureGlobalKW( lengthIx);
	    
	    if( nTok > 0)
	      {
		DLongGDL* len = new DLongGDL(dim);
		for(int i=0; i < nTok; i++)
		  (*len)[i] = tokenLen[i];

		e->SetKW( lengthIx, len);
	      }
	    else
	      {
		e->SetKW( lengthIx, new DLongGDL( 0));
	      }
	  }
	
	if( nTok == 0) return new DLongGDL( 0);
    
	DLongGDL* d = new DLongGDL(dim);
	for(int i=0; i < nTok; i++)
	  (*d)[i] = tokenStart[i];
	return d; 
      } 

  // EXTRACT
  if( nTok == 0) return new DStringGDL( "");

  dimension dim(nTok);
  DStringGDL *d = new DStringGDL(dim);
  for(int i=0; i < nTok; i++) 
  {
    (*d)[i] = stringIn.substr(tokenStart[i], tokenLen[i]);	

    // remove escape
    DString& act = (*d)[i];
    long escPos = act.find_first_of( escape, 0);
    while( escPos != string::npos)
	  {
	    act = act.substr( 0, escPos)+act.substr( escPos+1);
	    escPos = act.find_first_of( escape, escPos+1);
	  }
      }
    return d;
  }

  BaseGDL* getenv_fun( EnvT* e)
  {
    SizeT nParam=e->NParam();

    static int environmentIx = e->KeywordIx( "ENVIRONMENT" );
    bool environment = e->KeywordSet( environmentIx );
  
    SizeT nEnv; 
    DStringGDL* env;

    if( environment) {
      // determine number of environment entries
      for(nEnv = 0; environ[nEnv] != NULL  ; ++nEnv);

      dimension dim( nEnv );
      env = new DStringGDL(dim);

      // copy stuff into local string array
      for(SizeT i=0; i < nEnv ; ++i)
        (*env)[i] = environ[i];

    } else {

      if(nParam != 1) 
        e->Throw( "Incorrect number of arguments.");

      DStringGDL* name = e->GetParAs<DStringGDL>(0);
      nEnv = name->N_Elements();

      dimension dim( nEnv );
      env = new DStringGDL(dim);
 
      // copy the stuff into local string only if param found
      char *resPtr;
      for(SizeT i=0; i < nEnv ; ++i){
	if( (resPtr = getenv((*name)[i].c_str())) ) 
	  (*env)[i] = resPtr;
      }

    }
    
    return env;
  }

  BaseGDL* tag_names_fun( EnvT* e)
  {
    SizeT nParam=e->NParam();
    DStructGDL* struc= e->GetParAs<DStructGDL>(0);

    static int structureNameIx = e->KeywordIx( "STRUCTURE_NAME" );
    bool structureName = e->KeywordSet( structureNameIx );
    
    DStringGDL* tagNames;

    if(structureName){
        
	tagNames =  new DStringGDL((*struc).Desc()->Name());
    } else {
      SizeT nTags = (*struc).Desc()->NTags();
    
      tagNames = new DStringGDL(dimension(nTags));
      for(int i=0; i < nTags; ++i)
        (*tagNames)[i] = (*struc).Desc()->TagName(i);
    }

    return tagNames;
  }

  BaseGDL* stregex_fun( EnvT* e)
  {
    SizeT nParam=e->NParam( 2);
    
    DStringGDL* stringExpr= e->GetParAs<DStringGDL>(0);
    DString pattern;
    e->AssureStringScalarPar(1, pattern);

    static int booleanIx = e->KeywordIx( "BOOLEAN" );
    bool booleanKW = e->KeywordSet( booleanIx );

    static int extractIx = e->KeywordIx( "EXTRACT" );
    bool extractKW = e->KeywordSet( extractIx );

    static int foldCaseIx = e->KeywordIx( "FOLD_CASE" );
    bool foldCaseKW = e->KeywordSet( foldCaseIx );

    static int lengthIx = e->KeywordIx( "LENGTH" );
    bool lengthKW = e->KeywordSet( lengthIx );
   
    static int subexprIx = e->KeywordIx( "SUBEXPR" );
    bool subexprKW = e->KeywordSet( subexprIx );
 
    if( (lengthKW &&  (booleanKW || extractKW))  ||
        (subexprKW && booleanKW) ) 
        e->Throw( "Conflicting keywords.");
    if( subexprKW) 
        e->Throw( "Subexpression not yet implemented.");

  
    int nStr = stringExpr->N_Elements();
    dimension dim( nStr);

    DLongGDL* len;
    if( lengthKW) {
      e->AssureGlobalKW( lengthIx);
      len = new DLongGDL(dim);
    } 
    
    BaseGDL* result;

    if( booleanKW) 
      result = new DByteGDL(dim);
    else if( extractKW)
      result = new DStringGDL(dim); 
    else 
      result = new DLongGDL(dim); 
 
    // compile the regular expression
    regex_t regexp;
    int cflags = 0;
    int compRes = regcomp( &regexp, pattern.c_str(), cflags);

    regmatch_t pmatch[1];
    int nmatch = 1;
    int eflags = 0; 
 
    // now match towards the string
    for(SizeT i=0; i<nStr; ++i){ 

       int matchres = regexec( &regexp, (*stringExpr)[i].c_str(), 
         nmatch, pmatch, eflags);

       if( booleanKW) 
	   (* static_cast<DByteGDL*>(result))[i] = (matchres == 0);
       else if ( extractKW)
           (* static_cast<DStringGDL*>(result))[i] = (*stringExpr)[i].substr(
               pmatch[0].rm_so, pmatch[0].rm_eo - pmatch[0].rm_so);
       else
           (* static_cast<DLongGDL*>(result))[i] = (matchres == 0) ? 1 : -1;
       

       if( lengthKW)
           (*len)[i] = pmatch[0].rm_eo - pmatch[0].rm_so;
    }

   regfree( &regexp);

   if( lengthKW)
     e->SetKW( lengthIx, len);    

   return result;
  }



} // namespace


