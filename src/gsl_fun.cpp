/***************************************************************************
                          gsl_fun.cpp  -  GDL GSL library function
                             -------------------
    begin                : Jan 20 2004
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

#include <map>
#include <cmath>

#include "datatypes.hpp"
#include "envt.hpp"
#include "basic_fun.hpp"
#include "gsl_fun.hpp"
#include "dinterpreter.hpp"

#include <gsl/gsl_sys.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_fft_real.h>
#include <gsl/gsl_fft_real_float.h>
#include <gsl/gsl_fft_complex.h>
#include <gsl/gsl_fft_halfcomplex.h>
#include <gsl/gsl_fft_halfcomplex_float.h>
#include <gsl/gsl_fft_complex_float.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_histogram.h>
#include <gsl/gsl_interp.h>
#include <gsl/gsl_spline.h>

// newton/broyden
#include <gsl/gsl_multiroots.h>
#include <gsl/gsl_vector.h>

// constant
#include <gsl/gsl_const_mksa.h>
#include <gsl/gsl_const_num.h>
#include <gsl/gsl_math.h>
#ifdef USE_UDUNITS
#  include <udunits2.h>
#endif

// binomialcoef
#include <gsl/gsl_sf_gamma.h>

#define LOG10E 0.434294

namespace lib {

  using namespace std;

  const int szdbl=sizeof(double);
  const int szflt=sizeof(float);

  BaseGDL* invert_fun( EnvT* e)
  {
    SizeT nParam=e->NParam();
    int s;
    float f32;
    double f64;
    double det;
    long singular=0;

    if( nParam == 0)
      e->Throw( "Incorrect number of arguments.");

    BaseGDL* p0 = e->GetParDefined( 0);

    SizeT nEl = p0->N_Elements();
    if( nEl == 0)
      e->Throw( "Variable is undefined: " + e->GetParString(0));
  
    if (p0->Rank() > 2)
      e->Throw( "Input must be a square matrix:" + e->GetParString(0));
    
    if (p0->Rank() > 1) {
      if (p0->Dim(0) != p0->Dim(1))
        e->Throw( "Input must be a square matrix:" + e->GetParString(0));
    }
    
    // status 
    BaseGDL** p1L;
    if (nParam == 2) {
      e->AssureGlobalPar( 1);
      p1L = &e->GetPar( 1);
      delete (*p1L); 
      //*p1L = new DLongGDL( singular); 
    }
    
    // only one element matrix
    
    if( nEl == 1) {
      if( p0->Type() == COMPLEXDBL) {
	DComplexDblGDL* res = static_cast<DComplexDblGDL*>
	  (p0->Convert2(COMPLEXDBL, BaseGDL::COPY));
	double a, b, deno;
	a=real((*res)[0]);
	b=imag((*res)[0]);
	deno=a*a+b*b;
	if (deno == 0.0) {
	  singular=1;
	  (*res)[0]= DComplexDbl(0., 0.);
	} else {
	(*res)[0]= DComplexDbl(a/deno, -b/deno);
	}
	if (nParam == 2) *p1L = new DLongGDL( singular); 
	return res;
      }
      if( p0->Type() == COMPLEX) {
	DComplexGDL* res = static_cast<DComplexGDL*>
	  (p0->Convert2(COMPLEX, BaseGDL::COPY));
	float a, b, deno;
	a=real((*res)[0]);
	b=imag((*res)[0]);
	deno=a*a+b*b;
	if (deno == 0.0) {
	  singular=1;
	  (*res)[0]= DComplex(0., 0.);
	} else {
	  (*res)[0]= DComplex(a/deno, -b/deno);
	}
	 if (nParam == 2) *p1L = new DLongGDL( singular); 
	return res;
      }
      if( p0->Type() == DOUBLE) {
	DDoubleGDL* res = static_cast<DDoubleGDL*>
	  (p0->Convert2(DOUBLE, BaseGDL::COPY));
	if ((*res)[0] == 0.0) {
	  singular=1;
	} else {
	  double unity=1.0 ;
	  (*res)[0]= unity / ((*res)[0]);
	}
	 if (nParam == 2) *p1L = new DLongGDL( singular); 
	return res;
      }

      // all other cases (including STRING, Float, Int, ... )
      //      if( p0->Type() == STRING) {
      DFloatGDL* res = static_cast<DFloatGDL*>
	(p0->Convert2( FLOAT, BaseGDL::COPY));
      if ((*res)[0] == 0.0) {
	singular=1;
      } else {
	(*res)[0]= 1.0 / ((*res)[0]);
      }
       if (nParam == 2) *p1L = new DLongGDL( singular);
      return res;
    }
    
    // more than one element matrix

    if( p0->Type() == COMPLEX)
      {
	DComplexGDL* p0C = static_cast<DComplexGDL*>( p0);
	DComplexGDL* res = new DComplexGDL( p0C->Dim(), BaseGDL::NOZERO);

	float f32_2[2];
	double f64_2[2];

	gsl_matrix_complex *mat = 
	  gsl_matrix_complex_alloc(p0->Dim(0), p0->Dim(1));
	gsl_matrix_complex *inverse = 
	  gsl_matrix_complex_calloc(p0->Dim(0), p0->Dim(1));
	gsl_permutation *perm = gsl_permutation_alloc(p0->Dim(0));

	for( SizeT i=0; i<nEl; ++i) {
	  memcpy(f32_2, &(*p0C)[i], szdbl);
	  f64 = (double) f32_2[0];
	  memcpy(&mat->data[2*i], &f64, szdbl);

	  f64 = (double) f32_2[1];
	  memcpy(&mat->data[2*i+1], &f64, szdbl);
	}
 
	gsl_linalg_complex_LU_decomp (mat, perm, &s);
       	det = gsl_linalg_complex_LU_lndet(mat);
	if (gsl_isinf(det) == 0) {
	  gsl_linalg_complex_LU_invert (mat, perm, inverse);
	  if (det * LOG10E < 1e-5) singular = 2;
	}
	else singular = 1;

	for( SizeT i=0; i<nEl; ++i) {
	  memcpy(&f64_2[0], &inverse->data[2*i], szdbl*2);
	  f32_2[0] = (float) f64_2[0];
	  f32_2[1] = (float) f64_2[1];

	  memcpy(&(*res)[i], &f32_2[0], szflt*2);
	}

	gsl_permutation_free(perm);
	gsl_matrix_complex_free(mat);
	gsl_matrix_complex_free(inverse);

	 if (nParam == 2) *p1L = new DLongGDL( singular);
	return res;
      }
    else if( p0->Type() == COMPLEXDBL)
      {
	DComplexDblGDL* p0C = static_cast<DComplexDblGDL*>( p0);
	DComplexDblGDL* res = new DComplexDblGDL( p0C->Dim(), BaseGDL::NOZERO);

	gsl_matrix_complex *mat = 
	  gsl_matrix_complex_alloc(p0->Dim(0), p0->Dim(1));
	gsl_matrix_complex *inverse = 
	  gsl_matrix_complex_calloc(p0->Dim(0), p0->Dim(1));
	gsl_permutation *perm = gsl_permutation_alloc(p0->Dim(0));

	memcpy(mat->data, &(*p0C)[0], nEl*szdbl*2);

	gsl_linalg_complex_LU_decomp (mat, perm, &s);
	det = gsl_linalg_complex_LU_lndet(mat);
	if (gsl_isinf(det) == 0) {
	  gsl_linalg_complex_LU_invert (mat, perm, inverse);
	  if (det * LOG10E < 1e-5) singular = 2;
	}
	else singular = 1;

	memcpy(&(*res)[0], inverse->data, nEl*szdbl*2);

	gsl_permutation_free(perm);
	gsl_matrix_complex_free(mat);
	gsl_matrix_complex_free(inverse);

	 if (nParam == 2) *p1L = new DLongGDL( singular);
	return res;
      }
    else if( p0->Type() == DOUBLE)
      {
	DDoubleGDL* p0D = static_cast<DDoubleGDL*>( p0);
	DDoubleGDL* res = new DDoubleGDL( p0->Dim(), BaseGDL::NOZERO);

	gsl_matrix *mat = gsl_matrix_alloc(p0->Dim(0), p0->Dim(1));
	gsl_matrix *inverse = gsl_matrix_calloc(p0->Dim(0), p0->Dim(1));
	gsl_permutation *perm = gsl_permutation_alloc(p0->Dim(0));

	memcpy(mat->data, &(*p0D)[0], nEl*szdbl);

	gsl_linalg_LU_decomp (mat, perm, &s);
	det = gsl_linalg_LU_lndet(mat);
	if (gsl_isinf(det) == 0) {
	  gsl_linalg_LU_invert (mat, perm, inverse);
	  if (det * LOG10E < 1e-5) singular = 2;
	}
	else singular = 1;

	memcpy(&(*res)[0], inverse->data, nEl*szdbl);

	gsl_permutation_free(perm);
	gsl_matrix_free(mat);
	gsl_matrix_free(inverse);

	 if (nParam == 2) *p1L = new DLongGDL( singular);
	return res;
      }
    else if( p0->Type() == FLOAT ||
	     p0->Type() == LONG ||
	     p0->Type() == ULONG ||
	     p0->Type() == INT ||
	     p0->Type() == STRING ||
	     p0->Type() == UINT ||
	     p0->Type() == BYTE)
      {
	DFloatGDL* p0F = static_cast<DFloatGDL*>( p0);
	DLongGDL* p0L = static_cast<DLongGDL*>( p0);
	DULongGDL* p0UL = static_cast<DULongGDL*>( p0);
	DIntGDL* p0I = static_cast<DIntGDL*>( p0);
	//	DStringGDL* p0S = static_cast<DStringGDL*>( p0);
	DUIntGDL* p0UI = static_cast<DUIntGDL*>( p0);
	DByteGDL* p0B = static_cast<DByteGDL*>( p0);

	//	if (p0->Type() == STRING) {
	  DFloatGDL* p0SS = static_cast<DFloatGDL*>
	    (p0->Convert2( FLOAT, BaseGDL::COPY));
	  //}

	DFloatGDL* res = new DFloatGDL( p0->Dim(), BaseGDL::NOZERO);

	gsl_matrix *mat = gsl_matrix_alloc(p0->Dim(0), p0->Dim(1));
	gsl_matrix *inverse = gsl_matrix_calloc(p0->Dim(0), p0->Dim(1));
	gsl_permutation *perm = gsl_permutation_alloc(p0->Dim(0));

	for( SizeT i=0; i<nEl; ++i) {
	  switch ( p0->Type()) {
	  case FLOAT: f64 = (double) (*p0F)[i]; break;
	  case LONG:  f64 = (double) (*p0L)[i]; break;
	  case ULONG: f64 = (double) (*p0UL)[i]; break;
	  case INT:   f64 = (double) (*p0I)[i]; break;
	  case STRING:f64 = (double) (*p0SS)[i]; break;
	  case UINT:  f64 = (double) (*p0UI)[i]; break;
	  case BYTE:  f64 = (double) (*p0B)[i]; break;
	  }
	  memcpy(&mat->data[i], &f64, szdbl);
	}

	gsl_linalg_LU_decomp (mat, perm, &s);
	det = gsl_linalg_LU_lndet(mat);
	if (gsl_isinf(det) == 0) {
	  gsl_linalg_LU_invert (mat, perm, inverse);
	  if (det * LOG10E < 1e-5) singular = 2;
	}
	else singular = 1;

	for( SizeT i=0; i<nEl; ++i) {
	  f32 = (float) inverse->data[i];
	  memcpy(&(*res)[i], &f32, 4);
	}

	gsl_permutation_free(perm);
	gsl_matrix_free(mat);
	gsl_matrix_free(inverse);

	 if (nParam == 2) *p1L = new DLongGDL( singular);
	return res;
      }
    else 
      {
	cout << "Should never reach this point ! Please report it !" << endl; 
	DFloatGDL* res = static_cast<DFloatGDL*>
	  (p0->Convert2( FLOAT, BaseGDL::COPY));

	if (nParam == 2) *p1L = new DLongGDL( singular);
	return res;
      }
  }


  template< typename T1, typename T2>
  int cp2data2_template( BaseGDL* p0, T2* data, SizeT nEl, 
			 SizeT offset, SizeT stride_in, SizeT stride_out)
  {
    T1* p0c = static_cast<T1*>( p0);

    for( SizeT i=0; i<nEl; ++i) 
      data[2*(i*stride_out+offset)] = (T2) (*p0c)[i*stride_in+offset]; 

    return 0;
  }


  template< typename T>
  int cp2data_template( BaseGDL* p0, T* data, SizeT nEl, 
			 SizeT offset, SizeT stride_in, SizeT stride_out)
  {
    switch ( p0->Type()) {
    case DOUBLE: 
      cp2data2_template< DDoubleGDL, T>( p0, data, nEl, offset, 
					 stride_in, stride_out);
      break;
    case FLOAT: 
      cp2data2_template< DFloatGDL, T>( p0, data, nEl, offset, 
					stride_in, stride_out);
      break;
    case LONG:
      cp2data2_template< DLongGDL, T>( p0, data, nEl, offset, 
				       stride_in, stride_out);
      break;
    case ULONG: 
      cp2data2_template< DULongGDL, T>( p0, data, nEl, offset, 
					stride_in, stride_out);
      break;
    case INT: 
      cp2data2_template< DIntGDL, T>( p0, data, nEl, offset, 
				      stride_in, stride_out);
      break;
    case UINT: 
      cp2data2_template< DUIntGDL, T>( p0, data, nEl, offset, 
				       stride_in, stride_out);
      break;
    case BYTE: 
      cp2data2_template< DByteGDL, T>( p0, data, nEl, offset, 
				       stride_in, stride_out);
      break;
    }
  }


  template< typename T>
  int unpack_real_mxradix_template(T *dptr, SizeT nEl, double direct, 
				   SizeT offset, SizeT stride) {
  
    if (direct == -1) {
      for( SizeT i=0; i<nEl; ++i) dptr[2*(i*stride+offset)] /= nEl;
    }

    for( SizeT i=1; i<nEl/2+(nEl%2); ++i) {
      dptr[2*stride*i+1+2*offset]       = +dptr[2*stride*2*i+2*offset];
      dptr[2*stride*(nEl-i)+1+2*offset] = -dptr[2*stride*2*i+2*offset];
    }

    for( SizeT i=2; i<nEl/2+(nEl%2); ++i) {
      dptr[2*stride*i+2*offset]       = +dptr[2*stride*(2*i-1)+2*offset];
    }

    for( SizeT i=2; i<nEl/2+(nEl%2); ++i) {
      dptr[2*stride*(nEl-i)+2*offset] = +dptr[2*stride*i+2*offset];
    }

    dptr[1] = 0;
    if ((nEl%2) == 0) 
      dptr[stride*nEl+2*offset] = dptr[2*stride*(nEl-1)+2*offset];
    dptr[2*stride*(nEl-1)+2*offset] = dptr[2*stride+2*offset];


    if (direct == + 1) {
      for( SizeT i=1; i<nEl; ++i) 
	dptr[2*(i*stride+offset)+1] = -dptr[2*(i*stride+offset)+1];
    }
  }

  template< typename T, typename T1, typename T2>
  int real_fft_transform_template(BaseGDL* p0, T *dptr, SizeT nEl, 
				  double direct, 
				  SizeT offset, SizeT stride_in, SizeT stride_out,
				  SizeT radix2,
				  int (*complex_radix2_forward) 
				  (T[], const size_t, size_t),
				  int (*complex_radix2_backward) 
				  (T[], const size_t, size_t),
				  int (*real_transform)
				  (T[], const size_t, size_t,
				   const T1*, T2*),
				  T1 *(*wavetable_alloc)(size_t),
				  T2 *(*workspace_alloc)(size_t),
				  void (*wavetable_free)(T1*),
				  void (*workspace_free)(T2*))
  {
    cp2data_template< T>( p0, dptr, nEl, offset, stride_in, stride_out);

    if (radix2) {

      if (direct == -1) {
	(*complex_radix2_forward) (&dptr[2*offset], stride_out, nEl);
	for( SizeT i=0; i<nEl; ++i) 
	  ((std::complex<T> &) dptr[2*(i*stride_out+offset)]) /= nEl;
      } else if (direct == +1) {
	(*complex_radix2_backward) (&dptr[2*offset], stride_out, nEl);
      }
    }
    else if (!radix2) {

      T1 *wave;
      T2 *work;
      
      work = (*workspace_alloc) (nEl);
      wave = (*wavetable_alloc) (nEl);

      (*real_transform) (&dptr[2*offset], 2*stride_out, nEl, wave, work);

      unpack_real_mxradix_template< T>( dptr, nEl, direct, offset, stride_out);

      (*workspace_free) (work);
      (*wavetable_free) (wave);
    }
  }


  template< typename T, typename T1, typename T2>
  int complex_fft_transform_template(BaseGDL* p0, T *dptr, SizeT nEl, 
				     double direct, 
				     SizeT offset, SizeT stride, SizeT radix2,
				     int (*complex_radix2_forward) 
				     (T[], const size_t, size_t),
				     int (*complex_radix2_backward) 
				     (T[], const size_t, size_t),
				     int (*complex_forward_transform)
				     (T[], const size_t, size_t,
				      const T1*, T2*),
				     int (*complex_backward_transform)
				     (T[], const size_t, size_t,
				      const T1*, T2*),
				     T1 *(*wavetable_alloc)(size_t),
				     T2 *(*workspace_alloc)(size_t),
				     void (*wavetable_free)(T1*),
				     void (*workspace_free)(T2*))
  {
    if (radix2) {

      if (direct == -1) {
	(*complex_radix2_forward) (&dptr[2*offset], stride, nEl);
	for( SizeT i=0; i<nEl; ++i) 
	  ((std::complex<T> &) dptr[2*(i*stride+offset)]) /= nEl;
      } else if (direct == +1) {
	(*complex_radix2_backward) (&dptr[2*offset], stride, nEl);
      }
    }
    else if (!radix2) {

      T1 *wave;
      T2 *work;
      
      work = (*workspace_alloc) (nEl);
      wave = (*wavetable_alloc) (nEl);

      if (direct == -1) {
	(*complex_forward_transform) (&dptr[2*offset], stride, nEl, 
				      wave, work);
	for( SizeT i=0; i<nEl; ++i) 
	  ((std::complex<T> &) dptr[2*stride*i+2*offset]) /= nEl;

      } else if (direct == +1) {
	(*complex_backward_transform) (&dptr[2*offset], stride, nEl, 
				       wave, work);
      }

      (*workspace_free) (work);
      (*wavetable_free) (wave);
    }
  }


  template < typename T>
  T* fft_template(BaseGDL* p0,
		  SizeT nEl, SizeT dbl, SizeT overwrite, 
		  double direct, DLong dimension)
  {
    SizeT offset;
    SizeT stride;

    T* res;

    if (overwrite == 0)
      if (dimension == 0)
	res = new T( p0->Dim(), BaseGDL::ZERO);
      else
	res = new T( p0->Dim(dimension-1), BaseGDL::ZERO);
    else
      res = (T*) p0;


    if( p0->Rank() == 1 || dimension > 0) {
	offset=0;
	stride=1;

	fft_1d( p0, &(*res)[0], nEl, offset, stride, 
		direct, dbl, dimension);
    } else if ( p0->Rank() == 2) {
      stride=p0->Dim(0);
      for( SizeT i=0; i<p0->Dim(0); ++i) {
	fft_1d( p0, &(*res)[0], p0->Dim(1), i, stride, 
		direct, dbl, dimension);
      }
      for( SizeT i=0; i<p0->Dim(1); ++i) {
	fft_1d( res, &(*res)[0], 
		p0->Dim(0), i*p0->Dim(0), 1, 
		direct, dbl, dimension);
      }
    } else if( p0->Rank() >= 3) {
      unsigned char *used = new unsigned char [nEl];

      stride = nEl;
      for( SizeT i=p0->Rank(); i<nEl; ++i) used[i] = 0;

      for (SizeT k=p0->Rank(); k>0; --k) {
	for( SizeT i=0; i<nEl; ++i) used[i] = 0;
	stride /= p0->Dim(k-1);

	SizeT cnt=1;
	offset = 0;
	while(cnt <= nEl/p0->Dim(k-1)) {
	  if (used[offset] != 1) {
	    cnt++;
	    for( SizeT i=0; i<p0->Dim(k-1); ++i) 
	      used[offset+i*stride] = 1;
	    if (k == p0->Rank())
	      fft_1d( p0, &(*res)[0], p0->Dim(k-1), offset, stride, 
		      direct, dbl, dimension);
	    else
	      fft_1d( res, &(*res)[0], p0->Dim(k-1), offset, stride, 
		      direct, dbl, dimension);
	  }
	  offset++;
	}
      }
      delete used;
    }

    return res;
  }


  BaseGDL* fft_fun( EnvT* e)
  {
    /*
      Program Flow
      ------------
      fft_fun
         fft_template
	    fft_1d
	       (real/complex)_fft_transform_template
	          cp2data_template (real only)
		     cp2data_2_template (real only)
    */

    SizeT nParam=e->NParam();
    SizeT overwrite=0, dbl=0;
    SizeT stride;
    SizeT offset;

    double direct=-1.0;

    if( nParam == 0)
      throw GDLException( e->CallingNode(), 
			  "FFT: Incorrect number of arguments.");

    BaseGDL* p0 = e->GetNumericArrayParDefined( 0);

    SizeT nEl = p0->N_Elements();
    if( nEl == 0)
      throw GDLException( e->CallingNode(), 
			  "FFT: Variable is undefined: "+e->GetParString(0));
  
    if( nParam == 2) {
      BaseGDL* p1 = e->GetPar( 1);
      if (p1->N_Elements() > 1)
	throw GDLException( e->CallingNode(), 
			    "FFT: Expression must be a scalar or 1 element array: "
			    +e->GetParString(1));

      DDoubleGDL* direction = 
	static_cast<DDoubleGDL*>(p1->Convert2( DOUBLE, BaseGDL::COPY));
      direct = GSL_SIGN((*direction)[0]);
    }

    if( e->KeywordSet(0)) dbl = 1;
    if( e->KeywordSet(1)) direct = +1.0;
    if( e->KeywordSet(2)) overwrite = 1;

    // Check for dimension keyword
    DLong dimension=0;
    if(e->KeywordSet(3)) {
      e->AssureLongScalarKW( 3, dimension);
      for( SizeT i=0; i<p0->Rank(); ++i)
	if (i != (dimension-1)) nEl /= p0->Dim(i);
      overwrite = 0;  // Disable overwrite
    }

    if( p0->Type() == COMPLEXDBL || p0->Type() == DOUBLE || dbl) { 

      return fft_template< DComplexDblGDL> (p0, nEl, dbl, overwrite, 
					    direct, dimension);

    }
    else if( p0->Type() == COMPLEX) {

      DComplexGDL* res;

      return fft_template< DComplexGDL> (p0, nEl, dbl, overwrite, 
					 direct, dimension);

    }
    else if (p0->Type() == FLOAT ||
	     p0->Type() == LONG ||
	     p0->Type() == ULONG ||
	     p0->Type() == INT ||
	     p0->Type() == UINT ||
	     p0->Type() == BYTE) {

      overwrite = 0;
      return fft_template< DComplexGDL> (p0, nEl, dbl, overwrite, 
					 direct, dimension);

    } else {
      DFloatGDL* res = static_cast<DFloatGDL*>
	(p0->Convert2( FLOAT, BaseGDL::COPY));

      return res;

    }
  }


  int fft_1d( BaseGDL* p0, void* data, SizeT nEl, SizeT offset, SizeT stride, 
	      double direct, SizeT dbl, DLong dimension)
  {
    float f32[2];
    double f64[2];

    // Determine if radix2
    SizeT radix2 = 0;
    for( SizeT i=0; i<32; ++i) {
      if (nEl == (2 << i)) {
	radix2 = 1;
	break;
      }
    }

    // Determine input stride
    SizeT stride_in=1;
    if (dimension > 0)
      for( SizeT i=0; i<dimension-1; ++i) stride_in *= p0->Dim(i);
    else
      stride_in = stride;

    if( p0->Type() == COMPLEX && dbl == 0)
      {
	DComplexGDL* p0C = static_cast<DComplexGDL*>( p0);
	float *dptr;
	dptr = (float*) data;

	if (dimension > 0) {
	  for( SizeT i=0; i<nEl; ++i) {
	    memcpy(&dptr[2*(i*stride+offset)], 
		   &(*p0C)[i*stride_in+offset], szflt*2);
	  }
	} else {
	  // NO dimension Keyword
	  if (stride == 1 && offset == 0) {
	    if ((void*) dptr != (void*) &(*p0C)[0]) 
	      memcpy(dptr, &(*p0C)[0], szflt*2*nEl);
	  } else {
	    for( SizeT i=0; i<nEl; ++i) {
	      memcpy(&dptr[2*(i*stride+offset)], 
		     &(*p0C)[i*stride+offset], szflt*2);
	    }
	  }
	}

	complex_fft_transform_template<float, 
	  gsl_fft_complex_wavetable_float,
	  gsl_fft_complex_workspace_float> 
	  (p0, dptr, nEl, direct, offset, stride, radix2,
	   gsl_fft_complex_float_radix2_forward,
	   gsl_fft_complex_float_radix2_backward,
	   gsl_fft_complex_float_forward,
	   gsl_fft_complex_float_backward,
	   gsl_fft_complex_wavetable_float_alloc,
	   gsl_fft_complex_workspace_float_alloc,
	   gsl_fft_complex_wavetable_float_free,
	   gsl_fft_complex_workspace_float_free);

	return 0;
      }
    else if( p0->Type() == COMPLEXDBL || 
	     (p0->Type() == COMPLEX && dbl))
      {
	DComplexDblGDL* p0C = static_cast<DComplexDblGDL*>( p0);
	DComplexGDL* p0CF = static_cast<DComplexGDL*>( p0);

	double *dptr;
	dptr = (double*) data;

	if( p0->Type() == COMPLEXDBL) {
	  for( SizeT i=0; i<nEl; ++i) {
	    memcpy(&dptr[2*(i*stride+offset)], 
		   &(*p0C)[i*stride_in+offset], szdbl*2);
	  }
	}
	else if( p0->Type() == COMPLEX) {
	  DComplexDbl c128;

	  for( SizeT i=0; i<nEl; ++i) {
	    c128 = (*p0CF)[i*stride_in+offset];
	      memcpy(&dptr[2*(i*stride+offset)], &c128, 2*szdbl);
	  }
	}
	
	complex_fft_transform_template<double, 
	  gsl_fft_complex_wavetable,
	  gsl_fft_complex_workspace> 
	  (p0, dptr, nEl, direct, offset, stride, radix2,
	   gsl_fft_complex_radix2_forward,
	   gsl_fft_complex_radix2_backward,
	   gsl_fft_complex_forward,
	   gsl_fft_complex_backward,
	   gsl_fft_complex_wavetable_alloc,
	   gsl_fft_complex_workspace_alloc,
	   gsl_fft_complex_wavetable_free,
	   gsl_fft_complex_workspace_free);
	
	return 0;
      }
    else if( p0->Type() == DOUBLE || dbl) 
      {
	double *dptr;
	dptr = (double*) data;

	real_fft_transform_template<double, 
	  gsl_fft_real_wavetable,
	  gsl_fft_real_workspace> 
	  (p0, dptr, nEl, direct, offset, stride_in, stride, radix2,
	   gsl_fft_complex_radix2_forward,
	   gsl_fft_complex_radix2_backward,
	   gsl_fft_real_transform,
	   gsl_fft_real_wavetable_alloc,
	   gsl_fft_real_workspace_alloc,
	   gsl_fft_real_wavetable_free,
	   gsl_fft_real_workspace_free);

	//	printf("real_fft_transform_template\n");

	return 0;
      }
    else if( p0->Type() == FLOAT ||
	     p0->Type() == LONG ||
	     p0->Type() == ULONG ||
	     p0->Type() == INT ||
	     p0->Type() == UINT ||
	     p0->Type() == BYTE)
      {
	float *dptr;
	dptr   = (float*) data;

	real_fft_transform_template<float, 
	  gsl_fft_real_wavetable_float,
	  gsl_fft_real_workspace_float> 
	  (p0, dptr, nEl, direct, offset, stride_in, stride, radix2,
	   gsl_fft_complex_float_radix2_forward,
	   gsl_fft_complex_float_radix2_backward,
	   gsl_fft_real_float_transform,
	   gsl_fft_real_wavetable_float_alloc,
	   gsl_fft_real_workspace_float_alloc,
	   gsl_fft_real_wavetable_float_free,
	   gsl_fft_real_workspace_float_free);

	//	printf("real_fft_transform_template (float)\n");

	return 0;
      }
  }


  template< typename T1, typename T2>
  int random_template( EnvT* e, T1* res, gsl_rng *r, 
		       dimension dim, 
		       DDoubleGDL* binomialKey, DDoubleGDL* poissonKey)
  {
    SizeT nEl = res->N_Elements();

    if( e->KeywordSet(1)) {// GAMMA
      DLong n;
      e->AssureLongScalarKWIfPresent( "GAMMA", n);
      for( SizeT i=0; i<nEl; ++i) (*res)[ i] = 
				    (T2) gsl_ran_gamma_int (r,n);
    } else if( e->KeywordSet(3)) { // NORMAL
      SizeT nEl = res->N_Elements();
      for( SizeT i=0; i<nEl; ++i) (*res)[ i] =
				    (T2) gsl_ran_ugaussian (r);
    } else if( e->KeywordSet(4)) { // BINOMIAL
      if (binomialKey != NULL) {
	DULong  n = (DULong)  (*binomialKey)[0];
	DDouble p = (DDouble) (*binomialKey)[1];
	SizeT nEl = res->N_Elements();
	for( SizeT i=0; i<nEl; ++i) (*res)[ i] =
				      (T2) gsl_ran_binomial (r, p, n);
      }
    } else if( e->KeywordSet(5)) { // POISSON
      if (poissonKey != NULL) {
	DDouble mu = (DDouble) (*poissonKey)[0];
	SizeT nEl = res->N_Elements();
	if (mu < 100000) {
	  for( SizeT i=0; i<nEl; ++i) (*res)[ i] =
					(T2) gsl_ran_poisson (r, mu);
	} else {
	  for( SizeT i=0; i<nEl; ++i) (*res)[ i] =
					(T2) gsl_ran_ugaussian (r);
	  for( SizeT i=0; i<nEl; ++i) (*res)[ i] *= sqrt(mu);
	  for( SizeT i=0; i<nEl; ++i) (*res)[ i] += mu;
	}
      }
    } else if( e->KeywordSet(6)) { // UNIFORM
      SizeT nEl = res->N_Elements();
      for( SizeT i=0; i<nEl; ++i) (*res)[ i] =
				    (T2) gsl_rng_uniform (r);
    } else if ( e->GetProName() == "RANDOMU") {
      for( SizeT i=0; i<nEl; ++i) (*res)[ i] = 
				    (T2) gsl_rng_uniform (r);
    } else if ( e->GetProName() == "RANDOMN") {
      for( SizeT i=0; i<nEl; ++i) (*res)[ i] = 
				    (T2) gsl_ran_ugaussian (r);
    }

    return 0;
  }


  BaseGDL* random_fun( EnvT* e)
  {
    const unsigned long seedMul = 65535;

    SizeT nParam = e->NParam( 1);

    dimension dim;
    if( nParam > 1)
      arr( e, dim, 1);

    DLongGDL* seed;
    static DLong seed0 = 0;

    gsl_rng *r;

    if( e->GlobalPar( 0))
      {
	DLongGDL* p0L = e->IfDefGetParAs< DLongGDL>( 0);
	if( p0L != NULL) // defined global -> use and update
	  {
	    seed0 = (*p0L)[ 0];	    

	    r = gsl_rng_alloc (gsl_rng_mt19937);
	    gsl_rng_set (r, seed0);

	    seed0 += dim.N_Elements() * seedMul; // avoid repetition in next call
	    // if called with undefined global

	    seed = new DLongGDL( seed0);
	    e->SetPar( 0, seed);
	  }
	else // undefined global -> init
	  {
	    if( seed0 == 0) // first time
	      {
		time_t t1;
		time(&t1);
		seed0 = static_cast<DLong>( t1);
	      }

	    r = gsl_rng_alloc (gsl_rng_mt19937);
	    gsl_rng_set (r, seed0);

	    seed0 += dim.N_Elements() * seedMul; // avoid repetition in next call
	    // which would be defined global if used in a loop
	    
	    seed = new DLongGDL( seed0);
	    e->SetPar( 0, seed);
	  }
      } 
    else // local (always defined) -> just use it
      {
	seed = e->GetParAs< DLongGDL>( 0);
	seed0 = (*seed)[0];

	r = gsl_rng_alloc (gsl_rng_mt19937);
	gsl_rng_set (r, seed0);

	seed0 += dim.N_Elements() * seedMul; // avoid repetition in next call
	// if called with undefined global
      }
    
    if( e->KeywordSet(2)) { // LONG

      DLongGDL* res = new DLongGDL(dim, BaseGDL::NOZERO);
      SizeT nEl = res->N_Elements();
      for( SizeT i=0; i<nEl; ++i) (*res)[ i] =
				    (DLong) (gsl_rng_uniform (r) * 2147483646);
      gsl_rng_free (r);
      //      *p0L = new DULongGDL( (DULong) (4294967296.0 * (*res)[0]) );
      return res;
    }

    DSub* pro=dynamic_cast<DSub*>(e->GetPro());
    DDoubleGDL* binomialKey = e->IfDefGetKWAs<DDoubleGDL>( 4);
    DDoubleGDL* poissonKey = e->IfDefGetKWAs<DDoubleGDL>( 5);

    if( e->KeywordSet(0)) { // DOUBLE
      DDoubleGDL* res = new DDoubleGDL(dim, BaseGDL::NOZERO);

      random_template< DDoubleGDL, double>( e, res, r, dim, 
					    binomialKey, poissonKey);

      gsl_rng_free (r);
      //      *p0L = new DULongGDL( (DULong) (4294967296.0 * (*res)[0]) );
      return res;
    } else {
      DFloatGDL* res = new DFloatGDL(dim, BaseGDL::NOZERO);

      random_template< DFloatGDL, float>( e, res, r, dim, 
					  binomialKey, poissonKey);

      gsl_rng_free (r);
      //      *p0L = new DULongGDL( (DULong) (4294967296.0 * (*res)[0]) );
      return res;
    }
  }

#ifdef HAVE_NEXTTOWARD
  // SA: in C99 / C++TR1 / Boost there is the nextafter() function
  //     the code below provides an alternative if needed
  //     based on the nexttoward.c from mingw (mingw-runtime-3.8/mingwex/math) 
  //     by Danny Smith <dannysmith@users.sourceforge.net> 
  /*
     nexttoward.c
     Contributed by Danny Smith <dannysmith@users.sourceforge.net>
     No copyright claimed, absolutely no warranties.

     2005-05-10
  */
  double nexttoward(double x, long double y)
  {
    union
    {
      double d;
      unsigned long long ll;
    } u;

    long double xx = x;

    if (isnan (y) || isnan (x)) return x + y;

     /* nextafter (0.0, -O.0) should return -0.0.  */
    if (xx == y) return y;

    u.d = x;
    if (x == 0.0)
    {
      u.ll = 1;
      return y > 0.0L ? u.d : -u.d;
    }

    /* Non-extended encodings are lexicographically ordered,
       with implicit "normal" bit.  */
    if (((x > 0.0) ^ (y > xx)) == 0) u.ll++;
    else u.ll--;
    return u.d;
  }
#endif

  BaseGDL* histogram_fun( EnvT* e)
  {
    double a;
    double b;
    DULong nri;

    SizeT nParam=e->NParam();

    BaseGDL* p0 = e->GetNumericParDefined( 0);
    SizeT nEl = p0->N_Elements();

    if( p0->Type() == COMPLEX || p0->Type() == COMPLEXDBL)
      e->Throw( "Complex expression not allowed in this context: "
		+e->GetParString(0));
    
    BaseGDL* binsizeKW = e->GetKW( 0);
    DDouble bsize = 1.0;
    if( binsizeKW != NULL)
      {
	e->AssureDoubleScalarKW( 0, bsize);
	if( bsize <= 0.0)
	  e->Throw( "Illegal BINSIZE.");
      }

    BaseGDL* maxKW = e->GetKW( 2);
    BaseGDL* minKW = e->GetKW( 3);

    BaseGDL* nbinsKW = e->GetKW( 4);
    DLong nbins;
    if( nbinsKW != NULL)
      {
	e->AssureLongScalarKW( 4, nbins);
	if( nbins < 0)
	  e->Throw( "Illegal NBINS (<0).");
	if( nbins == 0) // NBINS=0 is ignored
	  nbinsKW = NULL;
      }

    if( binsizeKW != NULL && nbinsKW != NULL && maxKW != NULL)
      e->Throw( "Conflicting keywords.");

    DDoubleGDL *p0D = dynamic_cast<DDoubleGDL*>(p0);
    auto_ptr<BaseGDL> guard;
    if( p0D == NULL)
      {
	p0D = static_cast<DDoubleGDL*>(p0->Convert2( DOUBLE, BaseGDL::COPY));
	guard.reset( p0D);
      }

    // get min max
    // use MinMax here when NAN will be supported
    DDouble minVal = (*p0D)[0];
    DDouble maxVal = minVal;
    for( SizeT i=1; i<nEl; ++i)
      if ((*p0D)[i] < minVal) 
	minVal = (*p0D)[i];
      else if ((*p0D)[i] > maxVal) 
	maxVal = (*p0D)[i];

    // min
    if (minKW == NULL) 
      {
	if( p0->Type() == BYTE)
	  a = 0.0;
	else 
	  a = minVal;
      } 
    else 
      e->AssureDoubleScalarKW( 3, a);
    // max
    if (maxKW == NULL) 
      {
	// !MAX && BINSIZE && NBINS -> determine MAX
	if (binsizeKW != NULL && nbinsKW != NULL) 
	  b = a + bsize * nbins;
        // SA: !MAX && !BINSIZE && NBINS -> binsize = (max - min) / (nbins - 1)
        else if (binsizeKW == NULL && nbinsKW != NULL) 
        {
          bsize = (maxVal - minVal) / (nbins - 1);
          b = a + nbins * bsize;
        }
	else if( p0->Type() == BYTE)
	  b = 255.0;
	else 
	  b = maxVal;
      } 
    else
      {
	e->AssureDoubleScalarKW( 2, b);

	// MAX && !BINSIZE && NBINS -> determine BINSIZE
	if( binsizeKW == NULL && nbinsKW != NULL)
	  bsize = (b - a) / (nbins - 1);
      }

    if( bsize < 0 || a > b || !isfinite(a) || !isfinite(b))
      e->Throw( "Illegal binsize or max/min.");

    // gsl histogram needs this adjustment
    double aOri = a, bOri = b;
    a = nexttoward(a, -DBL_MAX);
    b = nexttoward(b, DBL_MAX);

    // -> NBINS
    if( nbinsKW == NULL)
      nbins = static_cast< DLong>( floor( (b - a) / bsize) + 1);

    // INPUT keyword
    static int inputIx = e->KeywordIx( "INPUT"); 
    DLongGDL* input = e->IfDefGetKWAs<DLongGDL>( inputIx);
    if (input != NULL)
      if (input->N_Elements() < nbins)
	throw GDLException( e->CallingNode(), 
			    "HISTOGRAM: Expression " +e->GetString(inputIx) + 
			    " does not have enough elements.");
      else if (input->N_Elements() > nbins)
	nbins = input->N_Elements();
 
    // Adjust "b" if binsize specified otherwise gsl_histogram_set_ranges_uniform
    // will change bsize to (b-a)/nbins
    // SA: another case when it's needed: !MAX && !BINSIZE && NBINS
    if ( 
      binsizeKW != NULL 
      || (binsizeKW == NULL && maxKW == NULL && nbinsKW != NULL)
    ) b = a + nbins * bsize;
 
    gsl_histogram* hh = gsl_histogram_alloc( nbins);
    gsl_histogram_set_ranges_uniform( hh, a, b);

    // Set maxVal from keyword if present
    if (maxKW != NULL) e->AssureDoubleScalarKW( 2, maxVal);

    // Generate histogram
    for( SizeT i=0; i<nEl; ++i) {
      if ((*p0D)[i] <= maxVal)
	gsl_histogram_increment(hh, (*p0D)[i]);
    }

    dimension dim( nbins);
    DLongGDL* res = new DLongGDL(dim, BaseGDL::NOZERO);

    for( SizeT i=0; i<nbins; ++i) {
      (*res)[i] = static_cast<DLong>( gsl_histogram_get(hh, i));
    }

    // Add input to output if present
    if (input != NULL)
      for( SizeT i=0; i<nbins; ++i) (*res)[i] += (*input)[i];

    // SA: using aOri/bOri instead of gsl_histogram_min(hh) (as in calculation of LOCATIONS) 
    //     otherwise, when converting e.g. to INT the conversion might give bad results
    // OMAX
    if( e->KeywordPresent( 5)) {
      // e->SetKW( 5, (new DDoubleGDL( gsl_histogram_max(hh)))->Convert2(p0->Type(), BaseGDL::CONVERT));
      e->SetKW( 5, (new DDoubleGDL( bOri))->Convert2(p0->Type(), BaseGDL::CONVERT));
    }
    // OMIN
    if( e->KeywordPresent( 6)) {
      // e->SetKW( 6, (new DDoubleGDL( gsl_histogram_min(hh)))->Convert2(p0->Type(), BaseGDL::CONVERT));
      e->SetKW( 6, (new DDoubleGDL( aOri))->Convert2(p0->Type(), BaseGDL::CONVERT));
    }

    // REVERSE_INDICES
    if( e->KeywordPresent( 7)) {

      if (input != NULL)
	throw GDLException( e->CallingNode(), 
			    "HISTOGRAM: Conflicting keywords.");

      DULong k = 0;
      multimap< size_t, SizeT> bin_j;
      for( SizeT j=0; j<nEl; ++j) {
	if( (*p0D)[j] >= a && (*p0D)[j] <= b) 
	  {
	    ++k;

	    size_t bin;
	    gsl_histogram_find (hh, (*p0D)[j], &bin);

	    bin_j.insert( make_pair( bin, j));
	  }
      }

      nri = nbins + k + 1;
      DLongGDL* revindKW = new DLongGDL( dimension( nri), BaseGDL::NOZERO);

      k = 0;
      for( SizeT i=0; i<nbins; ++i) 
	{
	  typedef multimap< size_t, SizeT>::const_iterator mmI;
	  
	  pair< mmI, mmI> b = bin_j.equal_range( i);
	  
	  for( mmI j = b.first; j != b.second; ++j)
	    {
	      (*revindKW)[nbins+1+k] = j->second;
	      k++;
	    }	    
	}

//       for( SizeT i=nbins+1; i<nri; ++i)
// 	cout << (*revindKW)[i] << " ";
//       cout << endl;


//       k = 0;
//       for( SizeT i=0; i<nbins; ++i) {
// 	for( SizeT j=0; j<nEl; ++j) {

// 	  if( (*p0D)[j] >= a && (*p0D)[j] <= b) 
// 	    {
// 	      size_t bin;
// 	      gsl_histogram_find (hh, (*p0D)[j], &bin);
	      
// 	      if( bin == i) 
// 		{
// 		  (*revindKW)[nbins+1+k] = j;
// 		  k++;
// 		}
// 	    }
// 	}
//       }

//       for( SizeT i=nbins+1; i<nri; ++i)
// 	cout << (*revindKW)[i] << " ";
//       cout << endl;

      (*revindKW)[0] = nbins + 1;
      k = 0;
      for( SizeT i=1; i<=nbins; ++i) {
	k += (*res)[i-1];
	(*revindKW)[i] = k + nbins + 1;
      }

      e->SetKW( 7, revindKW);
    }
    
    // LOCATIONS
    if( e->KeywordPresent( 8)) {
      BaseGDL** locationsKW = &e->GetKW( 8);
      delete (*locationsKW);

      dimension dim( nbins);
      if( p0->Type() == DOUBLE) {

	*locationsKW = new DDoubleGDL( dim, BaseGDL::NOZERO);
	for( SizeT i=0; i<nbins; ++i)
	  (*static_cast<DDoubleGDL*>( *locationsKW))[i] = 
	    static_cast<DDouble>(aOri + bsize * i);

      } else if (p0->Type() == FLOAT) {

	*locationsKW = new DFloatGDL( dim, BaseGDL::NOZERO);
	for( SizeT i=0; i<nbins; ++i)
	  (*static_cast<DFloatGDL*>( *locationsKW))[i] = 
	    static_cast<DFloat>(aOri + bsize * i);

      } else if (p0->Type() == LONG) {

	*locationsKW = new DLongGDL( dim, BaseGDL::NOZERO);
	for( SizeT i=0; i<nbins; ++i)
	  (*static_cast<DLongGDL*>( *locationsKW))[i] = 
	    static_cast<DLong>(aOri + bsize * i);

      } else if (p0->Type() == ULONG) {

	*locationsKW = new DULongGDL( dim, BaseGDL::NOZERO);
	for( SizeT i=0; i<nbins; ++i)
	  (*static_cast<DULongGDL*>( *locationsKW))[i] = 
	    static_cast<DULong>(aOri + bsize * i);

      } else if (p0->Type() == LONG64) {

	*locationsKW = new DLong64GDL( dim, BaseGDL::NOZERO);
	for( SizeT i=0; i<nbins; ++i)
	  (*static_cast<DLong64GDL*>( *locationsKW))[i] = 
	    static_cast<DLong64>(aOri + bsize * i);

      } else if (p0->Type() == ULONG64) {

	*locationsKW = new DULong64GDL( dim, BaseGDL::NOZERO);
	for( SizeT i=0; i<nbins; ++i)
	  (*static_cast<DULong64GDL*>( *locationsKW))[i] = 
	    static_cast<DULong64>(aOri + bsize * i);

      } else if (p0->Type() == INT) {

	*locationsKW = new DIntGDL( dim, BaseGDL::NOZERO);
	for( SizeT i=0; i<nbins; ++i)
	  (*static_cast<DIntGDL*>( *locationsKW))[i] = 
	    static_cast<DInt>(aOri + bsize * i);

      } else if (p0->Type() == UINT) {

	*locationsKW = new DUIntGDL( dim, BaseGDL::NOZERO);
	for( SizeT i=0; i<nbins; ++i)
	  (*static_cast<DUIntGDL*>( *locationsKW))[i] = 
	    static_cast<DUInt>(aOri + bsize * i);

      } else if (p0->Type() == BYTE) {

	*locationsKW = new DByteGDL( dim, BaseGDL::NOZERO);
	for( SizeT i=0; i<nbins; ++i)
	  (*static_cast<DByteGDL*>( *locationsKW))[i] = 
	    static_cast<DByte>(aOri + bsize * i);
      }

    }
    gsl_histogram_free (hh);

    return(res);
  }


  void interpolate_linear(gsl_interp_accel *acc, gsl_interp *interp, 
			  double *xa, SizeT nx, const double ya[], 
			  double x[], double y[])
  {
    for( SizeT i=0; i<nx; ++i)
      y[i] = gsl_interp_eval (interp, xa, ya, x[i], acc);
  }


  void interpolate_cubic(gsl_interp_accel *acc, gsl_spline *spline, 
			 double *xa, SizeT nx, const double ya[], 
			 double x[], double y[])
  {
    for( SizeT i=0; i<nx; ++i)
      y[i] = gsl_spline_eval (spline, x[i], acc);
  }


  void twoD_lin_interpolate(SizeT ninterp,
			    double *xa, bool grid, 
			    SizeT nx, SizeT ny, SizeT nxa, SizeT nya,
			    double *p0, double *p1, double *p2, double *res)  
  {
    gsl_interp_accel *acc = gsl_interp_accel_alloc ();
    gsl_interp *interp;

    // Allocate and initialize interpolation arrays
    interp = gsl_interp_alloc (gsl_interp_linear, nxa);
    //    gsl_interp_init (interp, xa, &(*p0D)[0], nxa);

    // Allocate "work" arrays
    SizeT ny2 = (ny == 1) * 2 + (ny > 1) * ny;
    double **work = new double*[ny2];
    for( SizeT k=0; k<ny2; ++k) work[k] = new double[nx];

    double **ya = new double*[nya];
    for( SizeT k=0; k<nya; ++k) ya[k] = new double[nxa];

    for( SizeT i=0; i<ninterp; ++i) {

      // Get input y-axis values
      for( SizeT k=0; k<nya; ++k) {
	for( SizeT j=0; j<nxa; ++j) {
	  ya[k][j] = p0[i+(ninterp*j)+(ninterp*nxa)*k];
	  //  cout << k << "  " << j << "  " << ya[k][j] << endl;
	}
      }

      bool first = true;
      DLong lastrow;
      double *dptr;
      for( SizeT k=0; k<ny; ++k) {
	//	    printf("k: %d\n", k);
	// Interpolate along rows
	DLong row = (DLong) floor(p2[k]);
	if (grid) dptr = &p1[0]; else dptr = &p1[k]; 

	if (first || (row != lastrow) || !grid) {

	  // Correct if row out of bounds
	  if (row < 0) row = 0;
	  if (row >= nya) row = nya - 1;

	  interpolate_linear(acc, interp, xa, nx, 
			     ya[row], dptr, &work[0][0]);

	  // Correct if row out of bounds
	  if (row < -1) row = -1;
	  if (row >= nya-1) row = nya - 2;

	  interpolate_linear(acc, interp, xa, nx, 
			     ya[(row+1)], dptr, &work[1][0]);
	}
	first = false;
	lastrow = row;

	if (grid) {
	  // Interpolate between rows
	  for( SizeT j=0; j<nx; ++j) {
	    res[i+ninterp*j+(ninterp*nx)*k] = 
	      work[0][j] + (work[1][j]-work[0][j]) * (p2[k] - row); 
	  } // column (j) loop
	} else {
	  res[i+ninterp*k] = 
	    work[0][0] + (work[1][0]-work[0][0]) * (p2[k] - row); 
	}

      } // row (k) loop
    } // interp loop 

    // Free dynamic arrays
    for( SizeT k=0; k<ny2; ++k) 
      delete [] work[k];
    delete [] work;

    for( SizeT k=0; k<nya; ++k) 
      delete [] ya[k];
    delete [] ya;

    gsl_interp_accel_free (acc);
    gsl_interp_free (interp);
  }


  BaseGDL* interpolate_fun( EnvT* e)
  {
    SizeT nParam=e->NParam();

    if( nParam < 2)
      throw GDLException( e->CallingNode(), 
			  "INTERPOLATE: Incorrect number of arguments.");

    BaseGDL* p0 = e->GetParDefined( 0);
    BaseGDL* p1 = e->GetParDefined( 1);
    BaseGDL* p2=NULL;
    BaseGDL* p3=NULL;
    if ( nParam >= 3) p2 = e->GetParDefined( 2);
    if ( nParam >= 4) p3 = e->GetParDefined( 3);

    DDoubleGDL* p0D;
    DDoubleGDL* p1D;
    DDoubleGDL* p2D;
    DDoubleGDL* p3D;
    auto_ptr<BaseGDL> guard0;
    auto_ptr<BaseGDL> guard1;
    auto_ptr<BaseGDL> guard2;
    auto_ptr<BaseGDL> guard3;

    if( p0->Rank() < nParam-1)
      throw GDLException( e->CallingNode(), 
	    "INTERPOLATE: Number of parameters must agree with dimensions of argument.");

    bool cubic = false;
    if ( e->KeywordSet(0)) cubic = true;

    bool grid = false;
    if ( e->KeywordSet(1)) grid = true;

    if ( nParam == 3)
      if (p1->N_Elements() == 1 && p2->N_Elements() == 1)
	grid = false;

    // If not GRID then check that rank and dims match
    if ( nParam == 3 && !grid) {
      if (p1->Rank() != p2->Rank())
	throw GDLException( e->CallingNode(), 
	      "INTERPOLATE: Coordinate arrays must have same length if Grid not set.");
      else {
	for( SizeT i=0; i<p1->Rank(); ++i) {
	  if (p1->Dim(i) != p2->Dim(i))
	    throw GDLException( e->CallingNode(), 
		  "INTERPOLATE: Coordinate arrays must have same length if Grid not set.");
	}
      }
    }

    if (p0->Type() == DOUBLE)
	p0D = static_cast<DDoubleGDL*> ( p0);
    else
	{
	p0D = static_cast<DDoubleGDL*>
	  (p0->Convert2( DOUBLE, BaseGDL::COPY));
	guard0.reset( p0D);
	}

    // Determine dimensions of output
    DDoubleGDL* res;
    DLong dims[8]={0,0,0,0,0,0,0,0};
    SizeT resRank;
    // Linear Interpolation or No GRID
    if ( nParam == 2 || !grid) {
      for( SizeT i=0; i<p0->Rank()-(nParam-1); ++i) 
	dims[i] = p0->Dim(i);
      for( SizeT i=0; i<p1->Rank(); ++i) 
	dims[i+p0->Rank()-(nParam-1)] = p1->Dim(i);
      resRank = p0->Rank()-(nParam-1)+p1->Rank();
      if (p0->N_Elements() == 1 && 
	  p1->N_Elements() == 1 && 
	  !e->KeywordSet(1) && 
	  nParam < 3) {
	dims[0] = 0;
	resRank = 0;
      }
    } else {
      // GRID
      for( SizeT i=0; i<p0->Rank()-(nParam-1); ++i) 
	dims[i] = p0->Dim(i);

      dims[p0->Rank()-(nParam-1)] = p1->Dim(0);
      for( SizeT i=1; i<p1->Rank(); ++i) 
	dims[p0->Rank()-(nParam-1)] *= p1->Dim(i);

      dims[p0->Rank()-(nParam-1)+1] = p2->Dim(0);
      for( SizeT i=1; i<p2->Rank(); ++i) 
	dims[p0->Rank()-(nParam-1)+1] *= p2->Dim(i);

      if (nParam == 4) {
	dims[p0->Rank()-(nParam-1)+2] = p3->Dim(0);
	for( SizeT i=1; i<p3->Rank(); ++i) 
	  dims[p0->Rank()-(nParam-1)+2] *= p3->Dim(i);
      }

      resRank = 0;
      for( SizeT i=0; i<8; ++i) { 
	if (dims[i] != 0) resRank++;
      }
    }
    dimension dim((DLong *) dims, resRank);
    res = new DDoubleGDL(dim, BaseGDL::NOZERO);

    // Determine number of interpolations
    SizeT ninterp = 1;
    for( SizeT i=0; i<p0->Rank()-(nParam-1); ++i) ninterp *= p0->Dim(i);

    // 1D Interpolation
    if( nParam == 2) {

      // Setup interpolation arrays
      gsl_interp_accel *acc = gsl_interp_accel_alloc ();
      gsl_interp *interp;
      gsl_spline *spline;

      // Determine number and value of input points along x-axis
      SizeT nxa = p0->Dim(p0->Rank()-nParam+1);
      double *xa = new double[nxa];
      for( SizeT i=0; i<nxa; ++i) xa[i] = (double) i;
      
      // Allocate and initialize interpolation arrays
      if( cubic) {
	spline = gsl_spline_alloc (gsl_interp_cspline, nxa);
	gsl_spline_init (spline, xa, &(*p0D)[0], nxa);
      } else {
	interp = gsl_interp_alloc (gsl_interp_linear, nxa);
	gsl_interp_init (interp, xa, &(*p0D)[0], nxa);
      }

      if ( p1->Type() == DOUBLE) 
	p1D = static_cast<DDoubleGDL*> ( p1);
      else
	{
	p1D = static_cast<DDoubleGDL*>
	  (p1->Convert2( DOUBLE, BaseGDL::COPY));
	guard1.reset( p1D);
	}	

      // Single Interpolation
      if (ninterp == 1) {
	if( cubic)
	  // cubic interpolation
	  interpolate_cubic(acc, spline, xa, p1D->N_Elements(), 
			    &(*p0D)[0], &(*p1D)[0], &(*res)[0]);
	else
	  // linear interpolation
	  interpolate_linear(acc, interp, xa, p1D->N_Elements(), 
			     &(*p0D)[0], &(*p1D)[0], &(*res)[0]);
      } else {
	// Multiple Interpolation
	for( SizeT i=0; i<ninterp; ++i) {

	  double *ya = new double[nxa];
	  for( SizeT j=0; j<nxa; ++j) ya[j] = (*p0D)[j*ninterp+i];

	  double *y = new double[p1D->N_Elements()];

	  if( cubic)
	    // cubic interpolation
	    interpolate_cubic(acc, spline, xa, p1D->N_Elements(), 
			      ya, &(*p1D)[0], &(*res)[0]);
	  else
	    // linear interpolation
	    interpolate_linear(acc, interp, xa, p1D->N_Elements(), 
			       ya, &(*p1D)[0], y);

	  // Write to output array
	  for( SizeT j=0; j<p1D->N_Elements(); ++j) (*res)[j*ninterp+i] = y[j];

	  delete [] ya;
	  delete [] y;
	}
      }

      gsl_interp_accel_free (acc);

      if (cubic)
	gsl_spline_free (spline);
      else
	gsl_interp_free (interp);
    }


    // 2D Interpolation
    if( nParam == 3) {

      if( cubic)
	throw GDLException( e->CallingNode(), 
	      "INTERPOLATE: Bicubic interpolation not yet supported.");

      if ( p1->Type() == DOUBLE) 
	p1D = static_cast<DDoubleGDL*> ( p1);
      else
	{
	p1D = static_cast<DDoubleGDL*>
	  (p1->Convert2( DOUBLE, BaseGDL::COPY));
	guard1.reset( p1D);
	}	
      if ( p2->Type() == DOUBLE) 
	p2D = static_cast<DDoubleGDL*> ( p2);
      else
	{
	p2D = static_cast<DDoubleGDL*>
	  (p2->Convert2( DOUBLE, BaseGDL::COPY));
	guard2.reset( p2D);
	}	

      // Compute nx,ny (number of interpolated points along each dimension
      SizeT nx = 1;
      if (grid) nx = res->Dim(resRank-2);
      
      SizeT ny = 1;
      if (resRank != 0 && res->Dim(resRank-1) != 0) ny = res->Dim(resRank-1);
      if (resRank == 1) ny = dims[0];
      if (!grid) ny = p1->N_Elements();

      // Determine number and value of input points along x-axis
      SizeT nxa = p0->Dim(p0->Rank()-nParam+1);
      double *xa = new double[nxa];
      for( SizeT i=0; i<nxa; ++i) xa[i] = (double) i;

      // Determine number and value of input points along y-axis
      SizeT nya = p0->Dim(p0->Rank()-1);
      
      twoD_lin_interpolate(ninterp,
			   xa, grid, 
			   nx, ny, nxa, nya,
			   &(*p0D)[0], &(*p1D)[0], &(*p2D)[0], &(*res)[0]);
      
      delete [] xa;

    } // if( nParam == 3)


    // 3D Interpolation
    if( nParam == 4) {

      if( cubic)
	throw GDLException( e->CallingNode(), 
	      "INTERPOLATE: Bicubic interpolation not supported.");

      if ( p1->Type() == DOUBLE) 
	p1D = static_cast<DDoubleGDL*> ( p1);
      else
	{
	p1D = static_cast<DDoubleGDL*>
	  (p1->Convert2( DOUBLE, BaseGDL::COPY));
	guard1.reset( p1D);
	}	

      if ( p2->Type() == DOUBLE) 
	p2D = static_cast<DDoubleGDL*> ( p2);
      else
	{
	p2D = static_cast<DDoubleGDL*>
	  (p2->Convert2( DOUBLE, BaseGDL::COPY));
	guard2.reset( p2D);
	}	

      if ( p3->Type() == DOUBLE) 
	p3D = static_cast<DDoubleGDL*> ( p3);
      else
	{
	p3D = static_cast<DDoubleGDL*>
	  (p3->Convert2( DOUBLE, BaseGDL::COPY));
	guard3.reset( p3D);
	}	


      // Compute nx,ny (number of interpolated points along each dimension
      SizeT nx = 1;
      if (grid) nx = res->Dim(resRank-3);
      
      SizeT ny = 1;
      if (resRank != 0 && res->Dim(resRank-2) != 0) ny = res->Dim(resRank-2);
      if (resRank == 1) ny = dims[0];
      if (!grid) ny = p1->N_Elements();

      // Determine number and value of input points along x-axis
      SizeT nxa = p0->Dim(p0->Rank()-nParam+1);
      double *xa = new double[nxa];
      for( SizeT i=0; i<nxa; ++i) xa[i] = (double) i;

      // Determine number and value of input points along y-axis
      SizeT nya = p0->Dim(p0->Rank()-2);

      // Determine number and value of input points along z-axis
      SizeT nza = p0->Dim(p0->Rank()-1);

      double *work_xy = new double [nza*nx*ny];
      for( SizeT i=0; i<nza; ++i) {
	twoD_lin_interpolate(ninterp,
			     xa, grid, 
			     nx, ny, nxa, nya,
			     &(*p0D)[nxa*nya*i], &(*p1D)[0], &(*p2D)[0], 
			     &work_xy[i*nx*ny]);
      }

      double *za = new double[nza];
      for( SizeT i=0; i<nza; ++i) za[i] = (double) i;
      double *work_za  = new double [nza];

      SizeT nz = p3D->N_Elements();
      double *work_z   = new double [nz];

      gsl_interp *interp_z;
      gsl_interp_accel *acc = gsl_interp_accel_alloc ();
      interp_z = gsl_interp_alloc (gsl_interp_linear, nza);
      //      gsl_interp_init (interp_z, za, &(*p0D)[0], nza);

      if (grid ) {
	for( SizeT i=0; i<nx; ++i) {
	  for( SizeT j=0; j<ny; ++j) {
	    for( SizeT k=0; k<nza; ++k) 
	      work_za[k] = work_xy[k*nx*ny + j*nx + i];
	    interpolate_linear(acc, interp_z, za, nz, work_za, 
			       &(*p3D)[0], work_z);

	    for( SizeT k=0; k<nz; ++k) (*res)[k*nx*ny + j*nx + i] = work_z[k];
	  }
	}
      } else {
	for( SizeT i=0; i<nz; ++i) {
	  for( SizeT k=0; k<nza; ++k) work_za[k] = work_xy[k*ny + i];
	  interpolate_linear(acc, interp_z, za, 1, work_za, 
			     &(*p3D)[i], &(*res)[i]);
	}
      }

      gsl_interp_accel_free (acc);
      gsl_interp_free (interp_z);

      delete [] xa;
      delete [] za;
      delete [] work_xy;
      delete [] work_z;
      delete [] work_za;

    } // if( nParam == 4)


//    if (p0->Type() != DOUBLE) delete p0D;
//    if (p1->Type() != DOUBLE) delete p1D;
//    if (p2 != NULL)
//      if (p2->Type() != DOUBLE) delete p2D;
//    if (p3 != NULL)
//      if (p3->Type() != DOUBLE) delete p3D;

    if (p0->Type() == DOUBLE) {
      return res;	
    } else if (p0->Type() == FLOAT) {
      DFloatGDL* res1 = static_cast<DFloatGDL*>
	(res->Convert2( FLOAT, BaseGDL::COPY));
      delete res;
      return res1;
    } else if (p0->Type() == INT) {
      DIntGDL* res1 = static_cast<DIntGDL*>
	(res->Convert2( INT, BaseGDL::COPY));
      delete res;
      return res1;
    } else if (p0->Type() == UINT) {
      DUIntGDL* res1 = static_cast<DUIntGDL*>
	(res->Convert2( UINT, BaseGDL::COPY));
      delete res;
      return res1;
    } else if (p0->Type() == LONG) {
      DLongGDL* res1 = static_cast<DLongGDL*>
	(res->Convert2( LONG, BaseGDL::COPY));
      delete res;
      return res1;
    } else if (p0->Type() == ULONG) {
      DULongGDL* res1 = static_cast<DULongGDL*>
	(res->Convert2( ULONG, BaseGDL::COPY));
      delete res;
      return res1;
    } else if (p0->Type() == LONG64) {
      DLong64GDL* res1 = static_cast<DLong64GDL*>
	(res->Convert2( LONG64, BaseGDL::COPY));
      delete res;
      return res1;
    } else if (p0->Type() == ULONG64) {
      DULong64GDL* res1 = static_cast<DULong64GDL*>
	(res->Convert2( ULONG64, BaseGDL::COPY));
      delete res;
      return res1;
    }  else if (p0->Type() == BYTE) {
      DByteGDL* res1 = static_cast<DByteGDL*>
	(res->Convert2( BYTE, BaseGDL::COPY));
      delete res;
      return res1;
    } else {
      return res;
    }
    
  }


  void la_trired_pro( EnvT* e)
  {
    SizeT nParam=e->NParam();
    float f32;
    double f64;

    if( nParam != 3)
      throw GDLException( e->CallingNode(), 
			  "LA_TRIRED: Incorrect number of arguments.");

    BaseGDL* p0 = e->GetNumericArrayParDefined( 0);

    SizeT nEl = p0->N_Elements();
    if( nEl == 0)
      throw GDLException( e->CallingNode(), 
			  "LA_TRIRED: Variable is undefined: "+ 
			  e->GetParString(0));
    
    if (p0->Dim(0) != p0->Dim(1))
      throw GDLException( e->CallingNode(), 
			  "LA_TRIRED: Input must be a square matrix: "+ 
			  e->GetParString(0));

    if( p0->Type() == COMPLEX)
      {
	DComplexGDL* p0C = static_cast<DComplexGDL*>( p0);

	float f32_2[2];
	double f64_2[2];

	gsl_matrix_complex *mat = 
	  gsl_matrix_complex_alloc(p0->Dim(0), p0->Dim(0));
	gsl_matrix_complex *Q = 
	  gsl_matrix_complex_alloc(p0->Dim(0), p0->Dim(0));
	gsl_vector_complex *tau = gsl_vector_complex_alloc(p0->Dim(0)-1);
	gsl_vector *diag = gsl_vector_alloc(p0->Dim(0));
	gsl_vector *subdiag = gsl_vector_alloc(p0->Dim(0)-1);

	for( SizeT i=0; i<nEl; ++i) {
	  memcpy(f32_2, &(*p0C)[i], szdbl);
	  f64 = (double) f32_2[0];
	  memcpy(&mat->data[2*i], &f64, szdbl);

	  f64 = (double) f32_2[1];
	  memcpy(&mat->data[2*i+1], &f64, szdbl);
	}

	gsl_linalg_hermtd_decomp (mat, tau);
	gsl_linalg_hermtd_unpack (mat, tau, Q, diag, subdiag);

	DLong dims[2] = {p0->Dim(0), p0->Dim(0)};
	dimension dim0(dims, (SizeT) 2);
	BaseGDL** p0Co = &e->GetPar( 0);
	delete (*p0Co);
	*p0Co = new DComplexGDL(dim0, BaseGDL::NOZERO);

	DLong n = p0->Dim(0);
	dimension dim1(&n, (SizeT) 1);
	BaseGDL** p1F = &e->GetPar( 1);
	delete (*p1F);
	*p1F = new DFloatGDL(dim1, BaseGDL::NOZERO);

	n--;
	dimension dim2(&n, (SizeT) 1);
	BaseGDL** p2F = &e->GetPar( 2);
 	delete (*p2F);
 	*p2F = new DFloatGDL(dim2, BaseGDL::NOZERO);

 	for( SizeT i=0; i<p0->Dim(0)*p0->Dim(0); i++) {
 	  memcpy(&f64_2[0], &Q->data[2*i], 2*szdbl);
 	  f32_2[0] = (float) f64_2[0];
 	  f32_2[1] = (float) f64_2[1];
 	  memcpy(&(*(DComplexGDL*) *p0Co)[i], &f32_2, 2*szflt);
 	}

 	for( SizeT i=0; i<p0->Dim(0); i++) {
 	  memcpy(&f64, &diag->data[i], szdbl);
 	  f32 = (float) f64;
 	  memcpy(&(*(DFloatGDL*) *p1F)[i], &f32, szflt);
 	}

	for( SizeT i=0; i<p0->Dim(0)-1; i++) {
	  memcpy(&f64, &subdiag->data[i], szdbl);
	  f32 = (float) f64;
 	  memcpy(&(*(DFloatGDL*) *p2F)[i], &f32, szflt);
	}

	gsl_matrix_complex_free(mat);
	gsl_matrix_complex_free(Q);
	gsl_vector_complex_free(tau);
	gsl_vector_free(diag);
	gsl_vector_free(subdiag);
      }
    else if( p0->Type() == COMPLEXDBL)
      {
	DComplexDblGDL* p0C = static_cast<DComplexDblGDL*>( p0);

	gsl_matrix_complex *mat = 
	  gsl_matrix_complex_alloc(p0->Dim(0), p0->Dim(0));
	gsl_matrix_complex *Q = 
	  gsl_matrix_complex_alloc(p0->Dim(0), p0->Dim(0));
	gsl_vector_complex *tau = gsl_vector_complex_alloc(p0->Dim(0)-1);
	gsl_vector *diag = gsl_vector_alloc(p0->Dim(0));
	gsl_vector *subdiag = gsl_vector_alloc(p0->Dim(0)-1);

	memcpy(mat->data, &(*p0C)[0], nEl*szdbl*2);

	gsl_linalg_hermtd_decomp (mat, tau);
	gsl_linalg_hermtd_unpack (mat, tau, Q, diag, subdiag);

	DLong dims[2] = {p0->Dim(0), p0->Dim(0)};
	dimension dim0(dims, (SizeT) 2);
	BaseGDL** p0Co = &e->GetPar( 0);
	delete (*p0Co);
	*p0Co = new DComplexDblGDL(dim0, BaseGDL::NOZERO);

	DLong n = p0->Dim(0);
	dimension dim1(&n, (SizeT) 1);
	BaseGDL** p1D = &e->GetPar( 1);
	delete (*p1D);
	*p1D = new DDoubleGDL(dim1, BaseGDL::NOZERO);

	n--;
	dimension dim2(&n, (SizeT) 1);
	BaseGDL** p2D = &e->GetPar( 2);
 	delete (*p2D);
 	*p2D = new DDoubleGDL(dim2, BaseGDL::NOZERO);

	memcpy(&(*(DComplexDblGDL*) *p0Co)[0], Q->data, 
	       p0->Dim(0)*p0->Dim(0)*szdbl*2);

	memcpy(&(*(DDoubleGDL*) *p1D)[0], diag->data, p0->Dim(0)*szdbl);
	memcpy(&(*(DDoubleGDL*) *p2D)[0], subdiag->data, 
	       (p0->Dim(0)-1)*szdbl);

	gsl_matrix_complex_free(mat);
	gsl_matrix_complex_free(Q);
	gsl_vector_complex_free(tau);
	gsl_vector_free(diag);
	gsl_vector_free(subdiag);
      }
    else if( p0->Type() == DOUBLE)
      {
	DDoubleGDL* p0D = static_cast<DDoubleGDL*>( p0);

	gsl_matrix *mat = gsl_matrix_alloc(p0->Dim(0), p0->Dim(0));
	gsl_matrix *Q = gsl_matrix_alloc(p0->Dim(0), p0->Dim(0));
	gsl_vector *tau = gsl_vector_alloc(p0->Dim(0)-1);
	gsl_vector *diag = gsl_vector_alloc(p0->Dim(0));
	gsl_vector *subdiag = gsl_vector_alloc(p0->Dim(0)-1);

	memcpy(mat->data, &(*p0D)[0], nEl*szdbl);

	gsl_linalg_symmtd_decomp (mat, tau);
	gsl_linalg_symmtd_unpack (mat, tau, Q, diag, subdiag);

	DLong dims[2] = {p0->Dim(0), p0->Dim(0)};
	dimension dim0(dims, (SizeT) 2);
	BaseGDL** p0Do = &e->GetPar( 0);
	delete (*p0Do);
	*p0Do = new DDoubleGDL(dim0, BaseGDL::NOZERO);

	DLong n = p0->Dim(0);
	dimension dim1(&n, (SizeT) 1);
	BaseGDL** p1D = &e->GetPar( 1);
	delete (*p1D);
	*p1D = new DDoubleGDL(dim1, BaseGDL::NOZERO);

	n--;
	dimension dim2(&n, (SizeT) 1);
	BaseGDL** p2D = &e->GetPar( 2);
 	delete (*p2D);
 	*p2D = new DDoubleGDL(dim2, BaseGDL::NOZERO);

	memcpy(&(*(DDoubleGDL*) *p0Do)[0], Q->data, 
	       p0->Dim(0)*p0->Dim(0)*szdbl);

	memcpy(&(*(DDoubleGDL*) *p1D)[0], diag->data, p0->Dim(0)*szdbl);
	memcpy(&(*(DDoubleGDL*) *p2D)[0], subdiag->data, 
	       (p0->Dim(0)-1)*szdbl);

	gsl_matrix_free(mat);
	gsl_matrix_free(Q);
	gsl_vector_free(tau);
	gsl_vector_free(diag);
	gsl_vector_free(subdiag);
      }
    else if( p0->Type() == FLOAT ||
	     p0->Type() == LONG ||
	     p0->Type() == ULONG ||
	     p0->Type() == INT ||
	     p0->Type() == UINT ||
	     p0->Type() == BYTE)
      {
	DFloatGDL* p0F = static_cast<DFloatGDL*>( p0);
	DLongGDL* p0L = static_cast<DLongGDL*>( p0);
	DULongGDL* p0UL = static_cast<DULongGDL*>( p0);
	DIntGDL* p0I = static_cast<DIntGDL*>( p0);
	DUIntGDL* p0UI = static_cast<DUIntGDL*>( p0);
	DByteGDL* p0B = static_cast<DByteGDL*>( p0);

	gsl_matrix *mat = gsl_matrix_alloc(p0->Dim(0), p0->Dim(0));
	gsl_matrix *Q = gsl_matrix_alloc(p0->Dim(0), p0->Dim(0));
	gsl_vector *tau = gsl_vector_alloc(p0->Dim(0)-1);
	gsl_vector *diag = gsl_vector_alloc(p0->Dim(0));
	gsl_vector *subdiag = gsl_vector_alloc(p0->Dim(0)-1);

	for( SizeT i=0; i<nEl; ++i) {
	  switch ( p0->Type()) {
	  case FLOAT: f64 = (double) (*p0F)[i]; break;
	  case LONG:  f64 = (double) (*p0L)[i]; break;
	  case ULONG: f64 = (double) (*p0UL)[i]; break;
	  case INT:   f64 = (double) (*p0I)[i]; break;
	  case UINT:  f64 = (double) (*p0UI)[i]; break;
	  case BYTE:  f64 = (double) (*p0B)[i]; break;
	  }
	  memcpy(&mat->data[i], &f64, szdbl);
	}

	gsl_linalg_symmtd_decomp (mat, tau);
	gsl_linalg_symmtd_unpack (mat, tau, Q, diag, subdiag);

	DLong dims[2] = {p0->Dim(0), p0->Dim(0)};
	dimension dim0(dims, (SizeT) 2);
	BaseGDL** p0Fo = &e->GetPar( 0);
	delete (*p0Fo);
	*p0Fo = new DFloatGDL(dim0, BaseGDL::NOZERO);

	DLong n = p0->Dim(0);
	dimension dim1(&n, (SizeT) 1);
	BaseGDL** p1F = &e->GetPar( 1);
	delete (*p1F);
	*p1F = new DFloatGDL(dim1, BaseGDL::NOZERO);

	n--;
	dimension dim2(&n, (SizeT) 1);
	BaseGDL** p2F = &e->GetPar( 2);
 	delete (*p2F);
 	*p2F = new DFloatGDL(dim2, BaseGDL::NOZERO);

 	for( SizeT i=0; i<p0->Dim(0)*p0->Dim(0); i++) {
 	  memcpy(&f64, &Q->data[i], szdbl);
 	  f32 = (float) f64;
 	  memcpy(&(*(DFloatGDL*) *p0Fo)[i], &f32, szflt);
 	}

 	for( SizeT i=0; i<p0->Dim(0); i++) {
 	  memcpy(&f64, &diag->data[i], szdbl);
 	  f32 = (float) f64;
 	  memcpy(&(*(DFloatGDL*) *p1F)[i], &f32, szflt);
 	}

	for( SizeT i=0; i<p0->Dim(0)-1; i++) {
	  memcpy(&f64, &subdiag->data[i], szdbl);
	  f32 = (float) f64;
 	  memcpy(&(*(DFloatGDL*) *p2F)[i], &f32, szflt);
	}

	gsl_matrix_free(mat);
	gsl_matrix_free(Q);
	gsl_vector_free(tau);
	gsl_vector_free(diag);
	gsl_vector_free(subdiag);
      }
    else 
      {
	DFloatGDL* res = static_cast<DFloatGDL*>
	  (p0->Convert2( FLOAT, BaseGDL::COPY));
      }
  }

  // gsl_multiroot_function-compatible function serving as a wrapper to the 
  // user-defined function passed (by name) as the second arg. to NEWTON or BROYDEN
  class n_b_param 
  { 
    public: 
    EnvT* envt; 
    EnvUDT* nenvt; 
    DDoubleGDL* arg; 
    string errmsg; 
  };
  int n_b_function(const gsl_vector* x, void* params, gsl_vector* f)
  {
    n_b_param *p = static_cast<n_b_param*>(params);
    // copying from GSL to GDL
    for (size_t i = 0; i < x->size; i++) (*(p->arg))[i] = gsl_vector_get(x, i);
    // executing GDL code
    BaseGDL* res;
    res = p->envt->Interpreter()->call_fun(
      static_cast<DSubUD*>(p->nenvt->GetPro())->GetTree()
    );
    // TODO: no guarding if res is an optimized constant
    // NO!!! the return value of call_fun() is always owned by the caller (constants are Dup()ed)
   auto_ptr<BaseGDL> res_guard(res);
    // sanity checks
    if (res->Rank() != 1 || res->N_Elements() != x->size) 
    {
      p->errmsg = "user-defined function must evaluate to a vector of the size of its argument";
      return GSL_EBADFUNC;
    }
    DDoubleGDL* dres;
    try
    {
      // BUT: Convert2(...) with CONVERT already deletes 'res' here if the type is changed
      dres = static_cast<DDoubleGDL*>(
        res->Convert2(DOUBLE, BaseGDL::CONVERT_THROWIOERROR)
      );
    }
    catch (GDLIOException& ex) 
    {
      p->errmsg = "failed to convert the result of the user-defined function to double";
      return GSL_EBADFUNC;
    }
    if (res != dres)
{
// prevent 'res' from being deleted again
res_guard.release();
res_guard.reset (dres);
}
    // copying from GDL to GSL
    for (size_t i = 0; i < x->size; i++) gsl_vector_set(f, i, (*dres)[i]);
    return GSL_SUCCESS;
  }

  // a simple error handler for GSL issuing GDL warning messages
  // an initial call (with file=NULL, line=-1 and gsl_errno=-1) sets a prefix to "reason: "
  void n_b_gslerrhandler(const char* reason, const char* file, int line, int gsl_errno)
  {
    static string prefix;
    if (line == -1 && gsl_errno == -1 && file == NULL) prefix = string(reason) + ": ";
    else Warning(prefix + "GSL: " + reason);
  }

  // a guard object ensuring freeing of GSL-allocated memory
  class n_b_gslguard {
    private:
    gsl_vector* x;
    gsl_multiroot_fsolver* solver;
    gsl_error_handler_t* old_handler;
    public:
    n_b_gslguard(gsl_vector* x_, gsl_multiroot_fsolver* solver_, gsl_error_handler_t* old_handler_)
    {
      x = x_;
      solver = solver_;
      old_handler = old_handler_;
    }
    ~n_b_gslguard() 
    {
      gsl_multiroot_fsolver_free(solver);
      gsl_vector_free(x);
      gsl_set_error_handler(old_handler);
    }
  };

  // SA: the library routine registered in libinit.cpp both for newton(),
  //     broyden() and used in imsl_zerosys.pro (/HYBRID keyword)
  BaseGDL* newton_broyden(EnvT* e)
  {
    // sanity check (for number of parameters)
    SizeT nParam = e->NParam();

    // 1-st argument : initial guess vector
    BaseGDL* p0 = e->GetParDefined(0);   
    if (p0->Rank() != 1) e->Throw("the first argument is expected to be a vector");
    BaseGDL* par = p0->Convert2(DOUBLE, BaseGDL::COPY);
    auto_ptr<BaseGDL> par_guard(par);

    // 2-nd argument : name of user function defining the system
    DString fun;
    e->AssureScalarPar<DStringGDL>(1, fun);    
    fun = StrUpCase(fun);
    if (LibFunIx(fun) != -1) 
      e->Throw("only user-defined functions allowed (library-routine name given)");

    // GDL magick
    StackGuard<EnvStackT> guard(e->Interpreter()->CallStack());
    EnvUDT* newEnv = new EnvUDT(e, funList[GDLInterpreter::GetFunIx(fun)], NULL);
    newEnv->SetNextPar(&par);
    e->Interpreter()->CallStack().push_back(newEnv);

    // GSL function parameter initialization
    n_b_param param;
    param.envt = e;
    param.nenvt = newEnv;
    param.arg = static_cast<DDoubleGDL*>(par); 

    // GSL function initialization
    gsl_multiroot_function F; 
    F.f = &n_b_function;
    F.n = p0->N_Elements();
    F.params = &param;

    // GSL error handling
    gsl_error_handler_t* old_handler = gsl_set_error_handler(&n_b_gslerrhandler);
    n_b_gslerrhandler(e->GetProName().c_str(), NULL, -1, -1);

    // GSL vector initialization
    gsl_vector *x = gsl_vector_alloc(F.n);
    for (size_t i = 0; i < F.n; i++) gsl_vector_set(x, i, (*(DDoubleGDL*) par)[i]);

    // GSL solver initialization
    gsl_multiroot_fsolver* solver;
    {
      const gsl_multiroot_fsolver_type* T; 
      if (e->KeywordSet("HYBRID"))           T = gsl_multiroot_fsolver_hybrid;     
      else if (e->GetProName() == "NEWTON")  T = gsl_multiroot_fsolver_dnewton;
      else if (e->GetProName() == "BROYDEN") T = gsl_multiroot_fsolver_broyden;
      else assert(false);
      solver = gsl_multiroot_fsolver_alloc(T, F.n);
    }
    gsl_multiroot_fsolver_set(solver, &F, x);

    // GSL ensuring memory de-allocation
    n_b_gslguard gslguard = n_b_gslguard(x, solver, old_handler);

    // GDL handling fine-tuning keywords
    // (intentionally not making keyword indices static here (NEWTON vs. BROYDEN))
    DLong iter_max = 200;
    e->AssureLongScalarKWIfPresent(e->KeywordIx("ITMAX"), iter_max);
    DDouble tolx = 1e-7, tolf = 1e-4;
    e->AssureDoubleScalarKWIfPresent(e->KeywordIx("TOLX"), tolx);
    e->AssureDoubleScalarKWIfPresent(e->KeywordIx("TOLF"), tolf);

    // GSL root-finding loop
    size_t iter = 0;
    int status;
    do
    {
      iter++;
      status = gsl_multiroot_fsolver_iterate(solver);
      if (status) break;
      { // TOLF check
        double test_tolf = 0;
        for (size_t i = 0; i < F.n; i++) test_tolf = max(test_tolf, abs(gsl_vector_get(solver->f, i)));
          if (test_tolf < tolf) break;
      }
      { // TOLX check
        double test_tolx = 0;
        for (size_t i = 0; i < F.n; i++) test_tolx = max(test_tolx, 
          abs(gsl_vector_get(solver->dx, i))/max(abs(gsl_vector_get(solver->x, i)), 1.)
        );
        if (test_tolx < tolx) break; 
      }
      // a check from GSL doc
      // if (gsl_multiroot_test_residual(solver->f, 1e-7) != GSL_CONTINUE) break;
    }
    while (iter <= iter_max);

    // remembering the result
    for (size_t i = 0; i < F.n; i++) (*(DDoubleGDL*) par)[i] = gsl_vector_get(solver->x, i);

    // handling errors from GDL via GSL
    if (status == GSL_EBADFUNC) e->Throw(static_cast<n_b_param*>(F.params)->errmsg);

    // showing an error message if ITMAX reached
    if (iter > iter_max) e->Throw("maximum number of iterations reached");

    // returning the result 
    par_guard.release();    // reusing par for the return value
    return par->Convert2(   // converting to float if neccesarry
      e->KeywordSet("DOUBLE") || p0->Type() == DOUBLE ? DOUBLE : FLOAT, 
      BaseGDL::CONVERT
    );
  }

  /*
   * SA: TODO:
   * constants: Catalan
   * units: ounce, oz, AU, mill, Fahrenheit, stoke, Abcoulomb, ATM (atm works!)
   * prefixes: u (micro)
   */
  BaseGDL* constant(EnvT* e)
  {
    string name;
    bool twoparams;
    static DStructGDL *Values = SysVar::Values();
    static double nan = (*static_cast<DDoubleGDL*>(Values->GetTag(Values->Desc()->TagIndex("D_NAN"), 0)))[0];
#ifdef USE_UDUNITS
    string unit;
#endif
    {
#ifdef USE_UDUNITS
      DString tmpunit;
#endif
      if (twoparams = (e->NParam(1) == 2)) 
      {
#ifdef USE_UDUNITS
        e->AssureScalarPar<DStringGDL>(1, tmpunit);    
        unit.reserve(tmpunit.length());
        for (string::iterator it = tmpunit.begin(); it < tmpunit.end(); it++) 
          if (*it != ' ') unit.append(1, *it);
#else
        e->Throw("GDL was compiled without support for UDUNITS");
#endif
      } 
      DString tmpname;
      e->AssureScalarPar<DStringGDL>(0, tmpname);    
      name.reserve(tmpname.length());
      for (string::iterator it = tmpname.begin(); it < tmpname.end(); it++) 
        if (*it != ' ' && *it != '_') name.append(1, (char)tolower(*it));
    }

#ifdef USE_UDUNITS
    ut_set_error_message_handler(ut_ignore); 
    // making the unit catalogue static to gain performace in the next call
    static ut_system* unitsys = ut_read_xml(NULL);
    if (unitsys == NULL) e->Throw("UDUNITS: failed to load the default unit database");
    ut_unit* unit_from;
#endif
    DDoubleGDL *res;

    if (name.compare("amu") == 0) 
    {
      res = new DDoubleGDL(GSL_CONST_MKSA_UNIFIED_ATOMIC_MASS);
#ifdef USE_UDUNITS
      if (twoparams) unit_from = ut_parse(unitsys, "kg", UT_ASCII);
#endif
    }
    else if (name.compare("atm") == 0 || name.compare("standardpressure") == 0) 
    {
      res = new DDoubleGDL(GSL_CONST_MKSA_STD_ATMOSPHERE);
#ifdef USE_UDUNITS
      if (twoparams) unit_from = ut_parse(unitsys, "N/m2", UT_ASCII);
#endif
    }
    else if (name.compare("au") == 0) 
    {
      res = new DDoubleGDL(GSL_CONST_MKSA_ASTRONOMICAL_UNIT);
#ifdef USE_UDUNITS
      if (twoparams) unit_from = ut_parse(unitsys, "m", UT_ASCII);
#endif
    }
    else if (name.compare("avogadro") == 0) 
    {
      res = new DDoubleGDL(GSL_CONST_NUM_AVOGADRO);
#ifdef USE_UDUNITS
      if (twoparams) unit_from = ut_parse(unitsys, "1/mole", UT_ASCII);
#endif
    }
    else if (name.compare("boltzman") == 0) 
    {
      res = new DDoubleGDL(GSL_CONST_MKSA_BOLTZMANN);
#ifdef USE_UDUNITS
      if (twoparams) unit_from = ut_parse(unitsys, "J/K", UT_ASCII);
#endif
    }
    else if (name.compare("c") == 0 || name.compare("speedlight") == 0) 
    {
      res = new DDoubleGDL(GSL_CONST_MKSA_SPEED_OF_LIGHT);
#ifdef USE_UDUNITS
      if (twoparams) unit_from = ut_parse(unitsys, "m/s", UT_ASCII);
#endif
    }
//    else if (name.compare("catalan") == 0) 
//    {
//      res = new DDoubleGDL(); // TODO: Dirichlet Beta function! 
//#ifdef USE_UDUNITS
//      if (twoparams) unit_from = ut_parse(unitsys, "", UT_ASCII);
//#endif
//    }
    else if (name.compare("e") == 0) 
    {
      res = new DDoubleGDL(M_E);
#ifdef USE_UDUNITS
      if (twoparams) unit_from = ut_get_dimensionless_unit_one(unitsys); 
#endif
    }
    else if (name.compare("electroncharge") == 0) 
    {
      res = new DDoubleGDL(GSL_CONST_MKSA_ELECTRON_CHARGE);
#ifdef USE_UDUNITS
      if (twoparams) unit_from = ut_parse(unitsys, "C", UT_ASCII);
#endif
    }
    else if (name.compare("electronmass") == 0) 
    {
      res = new DDoubleGDL(GSL_CONST_MKSA_MASS_ELECTRON);
#ifdef USE_UDUNITS
      if (twoparams) unit_from = ut_parse(unitsys, "kg", UT_ASCII);
#endif
    }
    else if (name.compare("electronvolt") == 0) 
    {
      res = new DDoubleGDL(GSL_CONST_MKSA_ELECTRON_VOLT);
#ifdef USE_UDUNITS
      if (twoparams) unit_from = ut_parse(unitsys, "J", UT_ASCII);
#endif
    }
    else if (name.compare("euler") == 0 || name.compare("gamma") == 0) 
    {
      res = new DDoubleGDL(M_EULER);
#ifdef USE_UDUNITS
      if (twoparams) unit_from = ut_get_dimensionless_unit_one(unitsys);
#endif
    }
    else if (name.compare("faraday") == 0) 
    {
      res = new DDoubleGDL(GSL_CONST_MKSA_FARADAY);
#ifdef USE_UDUNITS
      if (twoparams) unit_from = ut_parse(unitsys, "C/mole", UT_ASCII);
#endif
    }
    else if (name.compare("finestructure") == 0) 
    {
      res = new DDoubleGDL(GSL_CONST_NUM_FINE_STRUCTURE);
#ifdef USE_UDUNITS
      if (twoparams) unit_from = ut_get_dimensionless_unit_one(unitsys);
#endif
    }
    else if (name.compare("gas") == 0) 
    {
      res = new DDoubleGDL(GSL_CONST_MKSA_MOLAR_GAS);
#ifdef USE_UDUNITS
      if (twoparams) unit_from = ut_parse(unitsys, "J/mole/K", UT_ASCII);
#endif
    }
    else if (name.compare("gravity") == 0) 
    {
      res = new DDoubleGDL(GSL_CONST_MKSA_GRAVITATIONAL_CONSTANT);
#ifdef USE_UDUNITS
      if (twoparams) unit_from = ut_parse(unitsys, "N m2 kg2", UT_ASCII);
#endif
    }
    else if (name.compare("hbar") == 0) 
    {
      res = new DDoubleGDL(GSL_CONST_MKSA_PLANCKS_CONSTANT_HBAR);
#ifdef USE_UDUNITS
      if (twoparams) unit_from = ut_parse(unitsys, "J s", UT_ASCII);
#endif
    }
    else if (name.compare("perfectgasvolume") == 0) 
    {
      res = new DDoubleGDL(GSL_CONST_MKSA_STANDARD_GAS_VOLUME);
#ifdef USE_UDUNITS
      if (twoparams) unit_from = ut_parse(unitsys, "m3/mole", UT_ASCII);
#endif
    }
    else if (name.compare("pi") == 0) 
    {
      res = new DDoubleGDL(M_PI);
#ifdef USE_UDUNITS
      if (twoparams) unit_from = ut_get_dimensionless_unit_one(unitsys);
#endif
    }
    else if (name.compare("planck") == 0) 
    {
      res = new DDoubleGDL(GSL_CONST_MKSA_PLANCKS_CONSTANT_H);
#ifdef USE_UDUNITS
      if (twoparams) unit_from = ut_parse(unitsys, "J s", UT_ASCII);
#endif
    }
    else if (name.compare("protonmass") == 0) 
    {
      res = new DDoubleGDL(GSL_CONST_MKSA_MASS_PROTON);
#ifdef USE_UDUNITS
      if (twoparams) unit_from = ut_parse(unitsys, "kg", UT_ASCII);
#endif
    }
    else if (name.compare("rydberg") == 0) 
    {
      res = new DDoubleGDL(
        GSL_CONST_MKSA_MASS_ELECTRON * pow(GSL_CONST_MKSA_ELECTRON_CHARGE, 4) / (
          8. * pow(GSL_CONST_MKSA_VACUUM_PERMITTIVITY, 2) * 
          pow(GSL_CONST_MKSA_PLANCKS_CONSTANT_H, 3) * GSL_CONST_MKSA_SPEED_OF_LIGHT
        )
      );
#ifdef USE_UDUNITS
      if (twoparams) unit_from = ut_parse(unitsys, "m-1", UT_ASCII);
#endif
    }
    else if (name.compare("standardgravity") == 0) 
    {
      res = new DDoubleGDL(GSL_CONST_MKSA_GRAV_ACCEL);
#ifdef USE_UDUNITS
      if (twoparams) unit_from = ut_parse(unitsys, "m/s2", UT_ASCII);
#endif
    }
    else if (name.compare("stefanboltzman") == 0) 
    {
      res = new DDoubleGDL(GSL_CONST_MKSA_STEFAN_BOLTZMANN_CONSTANT);
#ifdef USE_UDUNITS
      if (twoparams) unit_from = ut_parse(unitsys, "W/K4/m2", UT_ASCII);
#endif
    }
    else if (name.compare("watertriple") == 0) 
    {
      res = new DDoubleGDL(273.16); // e.g. http://wtt-lite.nist.gov/cgi-bin/openindex.cgi?cid=7732185
#ifdef USE_UDUNITS
      if (twoparams) unit_from = ut_parse(unitsys, "K", UT_ASCII);
#endif
    }
    else 
    { 
      Warning("IMSL_CONSTANT: unknown constant");
      res = new DDoubleGDL(nan);
      twoparams = false;
    }

    // units
#ifdef USE_UDUNITS
    if (twoparams)
    {
      assert(unit_from != NULL);
      ut_unit* unit_to = ut_parse(unitsys, unit.c_str(), UT_ASCII);
      if (unit_to == NULL) 
      {
        Warning("IMSL_CONSTANT: UDUNITS: failed to parse unit");
        (*res)[0] = nan;
      }
      else
      {
        cv_converter* converter = ut_get_converter(unit_from, unit_to);
        if (converter == NULL) 
        {
          Warning("IMSL_CONSTANT: UDUNITS: units not convertible");
          (*res)[0] = nan;
        }
        else
        {
          (*res)[0] = cv_convert_double(converter, (*res)[0]);
          cv_free(converter);
        }
      }
      ut_free(unit_from); // leaks?
      ut_free(unit_to);   // leaks?
      //ut_free_system(unitsys); // (made static above)
    }
#endif    

    static int doubleIx = e->KeywordIx("DOUBLE");
    return res->Convert2(e->KeywordSet(doubleIx) ? DOUBLE : FLOAT, BaseGDL::CONVERT);
  }

  BaseGDL* binomialcoef(EnvT* e)
  {
    SizeT nParam=e->NParam(2);
    if (!IntType(e->GetParDefined(0)->Type()) || !IntType(e->GetParDefined(1)->Type()))
      e->Throw("Arguments must not be floating point numbers");
    DLong n, m;
    e->AssureLongScalarPar(0, n);
    e->AssureLongScalarPar(1, m);
    if (n < 0 || m < 0 || n < m) e->Throw("Arguments must fulfil n >= m >= 0");
    BaseGDL* res = new DDoubleGDL(gsl_sf_choose(n, m));
    static int doubleIx = e->KeywordIx("DOUBLE");
    return res->Convert2(e->KeywordSet(doubleIx) ? DOUBLE : FLOAT, BaseGDL::CONVERT);
  }

} // namespace


