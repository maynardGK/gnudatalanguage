/***************************************************************************
                          basic_fun_jmg.cpp  -  basic GDL library function
                             -------------------
    begin                : 2004
    copyright            : (C) 2004 by Joel Gales
    email                : jomoga@users.sourceforge.net
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

#include <string>
#include <fstream>
#include <memory>

#include "datatypes.hpp"
#include "envt.hpp"
#include "basic_fun.hpp"
#include "io.hpp"
#include "objects.hpp"
#include "basic_fun_jmg.hpp"

//#define GDL_DEBUG
#undef GDL_DEBUG

namespace lib {

  using namespace std;

  BaseGDL* size( EnvT* e) 
  {
    e->NParam( 1); // might be UNDEF, but must be given

    // BaseGDL* p0 = e->GetParDefined( 0); //, "SIZE");
    BaseGDL* p0 = e->GetPar( 0); //, "SIZE");

    SizeT nEl = 0;
    SizeT Rank = 0;
    SizeT vType = UNDEF;

    if (p0 != NULL) {
      nEl = p0->N_Elements();
      Rank = p0->Rank();
      vType = p0->Type();
    }

    // DIMENSIONS
    static int dimIx = e->KeywordIx( "DIMENSIONS");

    if( e->KeywordSet( dimIx)) { 
      if( Rank == 0) 
	if( e->KeywordSet(0))
	  return new DLong64GDL( 0);
	else
	  return new DLongGDL( 0);

      dimension dim( Rank);

      if( e->KeywordSet(0)) { // L64
	DLong64GDL* res = new DLong64GDL( dim, BaseGDL::NOZERO);
	(*res)[0] = 0;
	for( SizeT i=0; i<Rank; ++i) (*res)[ i] = p0->Dim(i);
	return res;
      } else {
	DLongGDL* res = new DLongGDL( dim, BaseGDL::NOZERO);
	(*res)[0] = 0;
	for( SizeT i=0; i<Rank; ++i) (*res)[ i] = p0->Dim(i);
	return res;
      }

    // FILE_LUN
    } else if( e->KeywordSet(2)) { 

      e->Throw( "FILE_LUN not supported yet.");

    // N_DIMENSIONS
    } else if( e->KeywordSet(3)) { 

      return new DLongGDL( Rank);

    //N_ELEMENTS
    } else if( e->KeywordSet(4)) { 

      if( e->KeywordSet(0))
	return new DULongGDL( nEl);
      else
	return new DLongGDL( nEl);

    // STRUCTURE
    } else if( e->KeywordSet(5)) { 

      e->Throw( "STRUCTURE not supported yet.");

    // TNAME
    } else if( e->KeywordSet(6)) { 

      if( p0 == NULL)
	return new DStringGDL( "UNDEFINED");

      return new DStringGDL( p0->TypeStr());

    // TYPE
    } else if( e->KeywordSet(7)) { 

      return new DLongGDL( vType );

    } else {

      dimension dim( 3 + Rank);

      if( e->KeywordSet(0)) {
	DLong64GDL* res = new DLong64GDL( dim, BaseGDL::NOZERO);
	(*res)[ 0] = Rank;
	for( SizeT i=0; i<Rank; ++i) (*res)[ i+1] = p0->Dim(i);
	(*res) [ Rank+1] = vType;
	(*res) [ Rank+2] = nEl;

	return res;
      } else {
	DLongGDL* res = new DLongGDL( dim, BaseGDL::NOZERO);
	(*res)[ 0] = Rank;
	for( SizeT i=0; i<Rank; ++i) (*res)[ i+1] = p0->Dim(i);
	(*res) [ Rank+1] = vType;
	(*res) [ Rank+2] = nEl;

	return res;
      }
    }
    return new DIntGDL( 0); // default for not supported
  }

  BaseGDL* fstat( EnvT* e) 
  { 
    e->NParam( 1);//, "FSTAT");

    DLong lun;
    e->AssureLongScalarPar( 0, lun);

    if( lun < -2 || lun > maxLun)
      throw GDLException( e->CallingNode(), 
			  "FSTAT:  File unit is not within allowed range.");

    DStructGDL*  fstat = new DStructGDL( "FSTAT");
   
    fstat->InitTag("UNIT", DLongGDL( lun));

    if( lun == -2)
      {
	fstat->InitTag("NAME", DStringGDL( "<stderr>"));
	// fstat->InitTag("SIZE", DLongGDL( 0)); 
	fstat->InitTag("OPEN", DByteGDL( 1)); 
	fstat->InitTag("ISATTY", DByteGDL( 1)); 
	// fstat->InitTag("ISAGUI", DByteGDL( 0)); 
	fstat->InitTag("INTERACTIVE", DByteGDL( 1)); 
	// fstat->InitTag("XDR", DByteGDL( 0)); 
	// fstat->InitTag("COMPRESS", DByteGDL( 0)); 
	// fstat->InitTag("READ", DByteGDL( 0)); 
	fstat->InitTag("WRITE", DByteGDL( 1)); 
      }
    else if( lun == -1)
      {
	fstat->InitTag("NAME", DStringGDL( "<stdout>"));
	// fstat->InitTag("SIZE", DLongGDL( 0)); 
	fstat->InitTag("OPEN", DByteGDL( 1)); 
	fstat->InitTag("ISATTY", DByteGDL( 1)); 
	// fstat->InitTag("ISAGUI", DByteGDL( 0)); 
	fstat->InitTag("INTERACTIVE", DByteGDL( 1)); 
	// fstat->InitTag("XDR", DByteGDL( 0)); 
	// fstat->InitTag("COMPRESS", DByteGDL( 0)); 
	// fstat->InitTag("READ", DByteGDL( 0)); 
	fstat->InitTag("WRITE", DByteGDL( 1)); 
      }
    else if( lun == 0)
      {
	fstat->InitTag("NAME", DStringGDL( "<stdin>"));
	// fstat->InitTag("SIZE", DLongGDL( 0)); 
	fstat->InitTag("OPEN", DByteGDL( 1)); 
	fstat->InitTag("ISATTY", DByteGDL( 1)); 
	// fstat->InitTag("ISAGUI", DByteGDL( 0)); 
	fstat->InitTag("INTERACTIVE", DByteGDL( 1)); 
	// fstat->InitTag("XDR", DByteGDL( 0)); 
	// fstat->InitTag("COMPRESS", DByteGDL( 0)); 
	fstat->InitTag("READ", DByteGDL( 1)); 
	// fstat->InitTag("WRITE", DByteGDL( 0)); 
      }
    else
      { // normal file
	GDLStream& actUnit = fileUnits[ lun-1];

	if( !actUnit.IsOpen()) 
	  return fstat; // OPEN tag is init to zero (SpDByte::GetInstance())

	fstat->InitTag("NAME", DStringGDL( actUnit.Name()));
	fstat->InitTag("SIZE", DLongGDL( actUnit.Size())); 
	fstat->InitTag("OPEN", DByteGDL( 1)); 
	// fstat->InitTag("ISATTY", DByteGDL( 0)); 
	// fstat->InitTag("ISAGUI", DByteGDL( 0)); 
	// fstat->InitTag("INTERACTIVE", DByteGDL( 0)); 
	// fstat->InitTag("COMPRESS", DByteGDL( 0)); 
	fstat->InitTag("READ", DByteGDL( actUnit.IsReadable()?1:0)); 
	fstat->InitTag("WRITE", DByteGDL( actUnit.IsWriteable()?1:0)); 
        fstat->InitTag("CUR_PTR", DLongGDL( actUnit.Tell()));
      }

    return fstat;
  }
  
  template< typename T>
  BaseGDL* make_array_template( EnvT* e, DLongGDL* dimKey, BaseGDL* value)
  {
    try{
      if (dimKey != NULL) {
	SizeT ndim = dimKey->N_Elements();
	dimension dim((SizeT *) &(*dimKey)[0], (SizeT) ndim);
	if( value != NULL)
	  {
	    T* v = static_cast<T*>( value);
	    T* res = new T(dim, BaseGDL::NOZERO);
	    SizeT nEl = dim.N_Elements();
	    for( SizeT i=0; i<nEl; ++i)
	      (*res)[i] = (*v)[0];
	    return res;
	  }
	if( e->KeywordSet(0)) return new T(dim, BaseGDL::NOZERO);
	if( e->KeywordSet(2)) return new T(dim, BaseGDL::INDGEN);
	return new T(dim);
      } else {
	dimension dim;
	arr( e, dim);
	if( value != NULL)
	  {
	    T* v = static_cast<T*>( value);
	    T* res = new T(dim, BaseGDL::NOZERO);
	    SizeT nEl = dim.N_Elements();
	    for( SizeT i=0; i<nEl; ++i)
	      (*res)[i] = (*v)[0];
	    return res;
	  }
	if( e->KeywordSet(0)) return new T(dim, BaseGDL::NOZERO);
	if( e->KeywordSet(2)) return new T(dim, BaseGDL::INDGEN);
	return new T(dim);
      } 
    }
    catch( GDLException ex)
      {
	e->Throw( ex.getMessage());
      }
  }

  BaseGDL* make_array( EnvT* e)
  {
    DLong type=0;
    e->AssureLongScalarKWIfPresent( "TYPE", type);

    DLongGDL* dimKey=NULL;

    static int sizeix = e->KeywordIx( "SIZE"); 
    static int dimensionix = e->KeywordIx( "DIMENSION"); 

    BaseGDL* size=e->GetKW(sizeix);
    BaseGDL* b_dimension=e->GetKW(dimensionix);
    if(b_dimension != NULL)
      {
	DLongGDL* l_dimension = e->GetKWAs<DLongGDL>( dimensionix);
	if(e->NParam() == 0 && size == NULL) 
	  {
	    dimension dim(l_dimension->N_Elements(),1);
	    dimKey=new DLongGDL(dim, BaseGDL::NOZERO);
	    e->Guard( dimKey);
	    for (int i=0;i<l_dimension->N_Elements();++i)
	      (*dimKey)[i]=(*l_dimension)[i];
	  }
	  
      } 
    else if(size != NULL)
      {
	DLongGDL* l_size = e->GetKWAs<DLongGDL>( dimensionix);
	if( l_size->N_Elements() < 4 || l_size->N_Elements() > 11)
	  e->Throw("Keyword array parameter SIZE must have from "
		   "4 to 11 elements.");
	type=(*l_size)[(*l_size)[0]+1];
	if(e->NParam() == 0) 
	  {
	    dimension dim((*l_size)[0],1);
	    dimKey=new DLongGDL(dim, BaseGDL::NOZERO);
	    e->Guard( dimKey);
	    for (int i=1;i<=(*l_size)[0];++i)
	      (*dimKey)[i-1]=(*l_size)[i];
	  }
	    
      }

    if( type < 0 || type > ULONG64 || type == STRUCT)
      {
	e->Throw("Invalid type specified for result.");
      }

    static int valueix = e->KeywordIx( "VALUE"); 
    BaseGDL* value = e->GetKW( valueix);
    if( value != NULL)
      {
	if( !value->Scalar())
	  e->Throw("Keyword must be a scalar in this context: "+
		   e->GetString(valueix));
	if( type == 0)
	  type = value->Type();
	else
	  {
	    value = value->Convert2( static_cast<DType>(type), BaseGDL::COPY);
	    e->Guard( value);
	  }
      }

    // BYTE
    if (e->KeywordSet(6) || type == BYTE) {

      return make_array_template< DByteGDL>( e, dimKey, value);

      //INT
    } else if (e->KeywordSet(7) || type == INT) {

      return make_array_template< DIntGDL>( e, dimKey, value);

      // UINT
    } else if (e->KeywordSet(8) || type == UINT) {

      return make_array_template< DUIntGDL>( e, dimKey, value);

      // LONG
    } else if (e->KeywordSet(9) || type == LONG) {

      return make_array_template< DLongGDL>( e, dimKey, value);

      // ULONG
    } else if (e->KeywordSet(10) || type == ULONG) {

      return make_array_template< DULongGDL>( e, dimKey, value);

      // LONG64
    } else if (e->KeywordSet(11) || type == LONG64) {

      return make_array_template< DLong64GDL>( e, dimKey, value);

      // ULONG64
    } else if (e->KeywordSet(12) || type == ULONG64) {

      return make_array_template< DULong64GDL>( e, dimKey, value);
      
      // DOUBLE
    } else if (e->KeywordSet(14) || type == DOUBLE) {

      return make_array_template< DDoubleGDL>( e, dimKey, value);

      // COMPLEX
    } else if (e->KeywordSet(15) || type == COMPLEX) {

      return make_array_template< DComplexGDL>( e, dimKey, value);

      // DCOMPLEX
    } else if (e->KeywordSet(16) || type == COMPLEXDBL) {

      return make_array_template< DComplexDblGDL>( e, dimKey, value);

      // STRUCT
    } else if ( type == STRUCT) {
      
      assert( value != NULL);

      DStructGDL* v = static_cast<DStructGDL*>( value);
      if (dimKey != NULL) {
	SizeT ndim = dimKey->N_Elements();
	dimension dim((SizeT *) &(*dimKey)[0], (SizeT) ndim);
	return v->New( dim, BaseGDL::INIT);
      } else {
	dimension dim;
	arr( e, dim);
	return v->New( dim, BaseGDL::INIT);
      } 
    } else {
      return make_array_template< DFloatGDL>( e, dimKey, value);
    }
    return 0;
  }

//   template< typename T>
//   BaseGDL* reform_template( EnvT* e, dimension dim)
//   {
//     T* res = static_cast<T*>( e->GetParDefined( 0)->Dup());
//     res->SetDim(dim);
//     return res;
//   }

  BaseGDL* reform( EnvT* e)
  {
    SizeT nParam=e->NParam(1);

    BaseGDL** p0P = &e->GetParDefined( 0);
    BaseGDL* p0 = *p0P;

    SizeT nEl = p0->N_Elements();
    SizeT Rank = p0->Rank();
    SizeT Type = p0->Type();

    dimension dim;

    if (nParam == 1) {
      SizeT j=0;
      for( SizeT i=0; i<MAXRANK; ++i) {
	if (p0->Dim(i) == 0) break;
	if (p0->Dim(i) != 1) {
	  dim.Set(j,p0->Dim(i));
	  j++;
	}
      }
    } else arr( e, dim, 1);


    if (dim.N_Elements() != nEl) 
      e->Throw( "New subscripts must not change the number elements in " 
		+ e->GetParString( 0));


    if (e->KeywordSet( "OVERWRITE")) {

      // make a copy if p0 is not global
      //      if( !e->GlobalPar( 0))
      //	p0 = p0->Dup();
      // better: steal p0
      if( !e->GlobalPar( 0))
	*p0P = NULL; // prevent local parameter form deletion

      p0->SetDim(dim);
      return p0;
    }

    // steal local parmeter
    if( !e->GlobalPar( 0))
      {
	*p0P = NULL;
	p0->SetDim( dim);
	return p0;
      }

    // global paramter - make a copy
    BaseGDL* res = p0->Dup();
    res->SetDim(dim);
    return res;

//     // BYTE
//     if (Type == BYTE) {
    
//       return reform_template< DByteGDL>( e, dim);
    
//       //INT
//     } else if (Type || Type == INT) {
    
//       return reform_template< DIntGDL>( e, dim);
    
//       // UINT
//     } else if (Type == UINT) {
    
//       return reform_template< DUIntGDL>( e, dim);
    
//       // LONG
//     } else if (Type == LONG) {
    
//       return reform_template< DLongGDL>( e, dim);
    
//       // ULONG
//     } else if (Type == ULONG) {
    
//       return reform_template< DULongGDL>( e, dim);
    
//       // LONG64
//     } else if (Type == LONG64) {
    
//       return reform_template< DLong64GDL>( e, dim);
    
//       // ULONG64
//     } else if (Type == ULONG64) {
    
//       return reform_template< DULong64GDL>( e, dim);
      
//       // FLOAT
//     } else if (Type == FLOAT) {
    
//       return reform_template< DFloatGDL>( e, dim);
    
//       // DOUBLE
//     } else if (Type == DOUBLE) {
    
//       return reform_template< DDoubleGDL>( e, dim);
    
//     }
    
  }
    
    
} // namespace


