/***************************************************************************
                          math_fun_ac.cpp  -  math GDL library function (AC)
                             -------------------
    begin                : 20 April 2007
    copyright            : (C) 2007 by Alain Coulais
    email                : alaingdl@users.sourceforge.net

****************************************************************************/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/*

  using the Besel functions provided by GSL
 
  http://www.physics.ohio-state.edu/~ntg/780/gsl_examples/J0_test.cpp
  http://www.gnu.org/software/gsl/manual/html_node/Bessel-Functions.html

  ----------------------- Warning -------------

  (obsolete) As is on 20/April/2007: Warning : GSL allows only INTEGER type for order 
  Thibaut contributes to extend to REAL in July 2009

  Important information: since bad formating issue in IDL
  (only first field in BESEL*([1], [1,2,3.]) is OK, doc said IDL must return a [1elem] array
  but return a [3elem] (see converse case BESELI( [1,2,3.], [1])))
  WE decided to follow IDL but to compute all the cases ...

  Note on 28/July/2009: when called with only X elements in IDL, Besel*
  functions are computed with implicit N==0, we don't follow this undocumented feature ...

  ----------------------- Warning -------------

  Since functions are clones derivated from the first one,
  please propagates bugs' correction and improvments.
  Don't know how to symplified :-(

  ----------------------- Warning -------------

  some codes here (macro) are common with "math_fun_gm.cpp" and "math_fun_ng.cpp"

*/

#define GM_EPS   1.0e-6
#define GM_ITER  50
#define GM_TINY  1.0e-18

#define GM_MIN(a, b) ((a) < (b) ? (a) : (b))

#define GM_5P0(a)							\
  e->NParam(a);								\
  									\
  DDoubleGDL* p0 = e->GetParAs<DDoubleGDL>(0);				\
  SizeT nElp0 = p0->N_Elements();					\
  if (nElp0 == 0)							\
    throw GDLException(e->CallingNode(), "Variable is undefined: "+e->GetParString(0));	\
  									\
  DType t0 = e->GetParDefined(0)->Type();				\
  //if (t0 == GDL_COMPLEX || t0 == GDL_COMPLEXDBL)				\
  //  e->Throw("Complex not implemented (GSL limitation). ");

#define AC_2P1()							\
  SizeT nElp1;								\
  DIntGDL* p1;								\
  DFloatGDL* p1_float;							\
  DType t1;								\
									\
  if  (e->NParam() == 1)						\
    {									\
      p1 = new DIntGDL(1, BaseGDL::NOZERO);				\
      (*p1)[0]=0;							\
      nElp1=1;								\
      t1 = GDL_INT;								\
      p1_float = new DFloatGDL(1, BaseGDL::NOZERO);			\
      (*p1_float)[0]=0.000;						\
    }									\
  else									\
    {									\
      p1 = e->GetParAs<DIntGDL>(1);					\
      nElp1 = p1->N_Elements();						\
      t1 = e->GetParDefined(1)->Type();					\
      p1_float = e->GetParAs<DFloatGDL>(1);				\
    }									\
									\
  const double dzero = 0.0000000000000000000 ;				\
									\
  //    throw GDLException(e->CallingNode(), "Variable is undefined: "+e->GetParString(1)); \
  									\
//  DType t1 = e->GetParDefined(1)->Type();				\
  //  if (t1 == GDL_COMPLEX || t1 == GDL_COMPLEXDBL)				\
  // e->Throw("Complex not implemented (GSL limitation). ");

#define GM_DF2()							\
  									\
  DDoubleGDL* res;							\
  if (nElp0 == 1 && nElp1 == 1)						\
    res = new DDoubleGDL(1, BaseGDL::NOZERO);				\
  else if (nElp0 > 1 && nElp1 == 1)					\
    res = new DDoubleGDL(p0->Dim(), BaseGDL::NOZERO);			\
  else if (nElp0 == 1 && nElp1 > 1)					\
    res = new DDoubleGDL(p1->Dim(), BaseGDL::NOZERO);			\
  else if (nElp0 <= nElp1)						\
    res = new DDoubleGDL(p0->Dim(), BaseGDL::NOZERO);			\
  else									\
    res = new DDoubleGDL(p1->Dim(), BaseGDL::NOZERO);			\
  									\
  SizeT nElp = res->N_Elements();					\

#define GM_CV0()					\
  static DInt doubleKWIx = e->KeywordIx("DOUBLE");	\
							\
  if (e->KeywordSet(doubleKWIx))			\
    return res;						\
  else							\
    return res->Convert2(GDL_FLOAT, BaseGDL::CONVERT);

#define GM_CV1()					\
  static DInt doubleKWIx = e->KeywordIx("DOUBLE");	\
							\
  if (t0 != GDL_DOUBLE && !e->KeywordSet(doubleKWIx))	\
    return res->Convert2(GDL_FLOAT, BaseGDL::CONVERT);	\
  else							\
    return res;

#define GM_CC1()						\
  static DInt coefKWIx = e->KeywordIx("ITER");			\
  if(e->KeywordPresent(coefKWIx))				\
    {								\
      cout << "ITER keyword not used, always return -1)" << endl;	\
      e->SetKW( coefKWIx, new DLongGDL( -1));			\
    }

#define AC_HELP()				\
  if (e->KeywordSet("HELP")) {			\
    string inline_help[]={						\
      "Usage: res="+e->GetProName()+"(x, [n,] double=double)",		\
      " -- x is a number or an array",					\
      " -- n is a number or an array (if missing, set to 0)",					\
      " If x and n dimensions differ, reasonnable rules applied"};	\
    int size_of_s = sizeof(inline_help) / sizeof(inline_help[0]);	\
    e->Help(inline_help, size_of_s);					\
  }

#include "includefirst.hpp"
#include "initsysvar.hpp"  // Used to define Double Infinity and Double NaN
#include "math_fun_ac.hpp"
#include <gsl/gsl_sf_bessel.h>

#ifdef _MSC_VER
#define isfinite _finite
#define isinf !_finite
#endif

using namespace std;

namespace lib {

  BaseGDL* beseli_fun(EnvT* e)
  {
    AC_HELP();
    GM_5P0(1);
    AC_2P1();
    GM_DF2();

    SizeT count;

    // GSL Limitation for X : must be lower than ~708
    for (count = 0;count<nElp0;++count)
	  if ((*p0)[count] > 708.)
	    e->Throw("Value of X is out of allowed range.");

    // we need to check if N values (array) are Integer or not
    int test=0;
    
    for (count = 0;count<nElp1;++count)
      if (abs((*p1_float)[count]-(float)(*p1)[count]) > 0.000001) // don't know if a "machar" value exists
	test=1;
	
    if (test==0)
      {
	if(nElp0==1)
	  {
	    for (count = 0;count<nElp;++count)
	      (*res)[count] = gsl_sf_bessel_In((*p1)[count],(*p0)[0]);
	  }
	else if(nElp1==1)
	  {
	    for (count = 0;count<nElp;++count)
	      (*res)[count] = gsl_sf_bessel_In((*p1)[0],(*p0)[count]);
	  }
	else
	  {
	    for (count = 0;count<nElp;++count)
	      (*res)[count] = gsl_sf_bessel_In((*p1)[count],(*p0)[count]);
	  }
      }
    else
      {
	// we need to check if X values (array) are positives
	for (count = 0;count<nElp0;++count)
	  if ((*p0)[count] < dzero) // don't know if a "machar" value exists
	    e->Throw("Value of X is out of allowed range (Only positive values when N is non integer).");

	// we need to check if N values (array) are positives
	for (count = 0;count<nElp1;++count)
	  if ((*p1)[count] < dzero) // don't know if a "machar" value exists
	    e->Throw("Value of N is out of allowed range (Only positive values when N is non integer).");

	if(nElp0==1)
	  {
	    for (count = 0;count<nElp;++count)
	      (*res)[count] = gsl_sf_bessel_Inu((*p1_float)[count],(*p0)[0]);
	  }
	else if(nElp1==1)
	  {
	    for (count = 0;count<nElp;++count)
	      (*res)[count] = gsl_sf_bessel_Inu((*p1_float)[0],(*p0)[count]);
	  }
	else
	  {
	    for (count = 0;count<nElp;++count)
	      (*res)[count] = gsl_sf_bessel_Inu((*p1_float)[count],(*p0)[count]);
	  }
      }
    GM_CC1();
    GM_CV1();
  }


  BaseGDL* beselj_fun(EnvT* e)
  {
    AC_HELP();
    GM_5P0(1);
    AC_2P1();
    GM_DF2();

    SizeT count;

    // we need to check if N values (array) are Integer or not
    int test=0;
    
    for (count = 0;count<nElp1;++count)
      if (abs((*p1_float)[count]-(float)(*p1)[count]) > 0.000001) // don't know if a "machar" value exists
	test=1;

    if (test==0)
      {
	if(nElp0==1)
	  {
	    for (count = 0;count<nElp;++count)
	      (*res)[count] = gsl_sf_bessel_Jn((*p1)[count],(*p0)[0]);
	  }
	else if(nElp1==1)
	  {
	    for (count = 0;count<nElp;++count)
	      (*res)[count] = gsl_sf_bessel_Jn((*p1)[0],(*p0)[count]);
	  }
	else
	  {
	    for (count = 0;count<nElp;++count)
	      (*res)[count] = gsl_sf_bessel_Jn((*p1)[count],(*p0)[count]);
	  }
      }
    else
      {
	// we need to check if X values (array) are positives
	for (count = 0;count<nElp0;++count)
	  if ((*p0)[count] < dzero) // don't know if a "machar" value exists
	    e->Throw("Value of X is out of allowed range (Only positive values when N is non integer).");

	// we need to check if N values (array) are positives
	for (count = 0;count<nElp1;++count)
	  if ((*p1)[count] < dzero) // don't know if a "machar" value exists
	    e->Throw("Value of N is out of allowed range (Only positive values when N is non integer).");

	if(nElp0==1)
	  {
	    for (count = 0;count<nElp;++count)
	      (*res)[count] = gsl_sf_bessel_Jnu((*p1_float)[count],(*p0)[0]);
	  }

	else if(nElp1==1)
	  {
	    for (count = 0;count<nElp;++count)
	      (*res)[count] = gsl_sf_bessel_Jnu((*p1_float)[0],(*p0)[count]);
	  }

	else
	  {
	    for (count = 0;count<nElp;++count)
	      (*res)[count] = gsl_sf_bessel_Jnu((*p1_float)[count],(*p0)[count]);
	  }
      }
    GM_CC1();
    GM_CV1();
  }

  // very preliminary version:
  // should not work when N LT 0
  // should return Inf at x==0
  // should not work for x LT 0

  BaseGDL* beselk_fun(EnvT* e)
  {
    AC_HELP();
    GM_5P0(1);
    AC_2P1();
    GM_DF2();

    // we need to check if N values (array) are Integer or not
    SizeT count;

    // do we have negative numbers ?
    for (count = 0;count<nElp0;++count)
      if ((*p0)[count] < dzero) // don't know if a "machar" value exists
        e->Throw("Value of X is out of allowed range (Only positive values).");
    
    int test=0;
    
    for (count = 0;count<nElp1;++count)
      if (abs((*p1_float)[count]-(float)(*p1)[count]) > 0.000001) // don't know if a "machar" value exists
	test=1;

    if (test==0)
      {
	// when X value is below ~1e-20 --> return -Inf (we use log(0.) which gives -Inf)
	const double smallVal = 1e-38 ;

	if(nElp0==1)
	  {
	    for (count = 0;count<nElp;++count)
	      if (abs((*p0)[0]) < smallVal)
		(*res)[count] =log(dzero) ;
	      else
		(*res)[count] = gsl_sf_bessel_Kn((*p1)[count],(*p0)[0]);
	  }
	else if(nElp1==1)
	  {
	    for (count = 0;count<nElp;++count)
	      if (abs((*p0)[count]) < smallVal)
		(*res)[count] =log(dzero) ;
	      else
		(*res)[count] = gsl_sf_bessel_Kn((*p1)[0],(*p0)[count]);
	  }
	else
	  {
	    for (count = 0;count<nElp;++count)
	      if (abs((*p0)[count]) < smallVal)
		(*res)[count] =log(dzero) ;
	      else
		(*res)[count] = gsl_sf_bessel_Kn((*p1)[count],(*p0)[count]);
	  }
      }
    else
      {
	// we need to check if N values (array) are positives
	for (count = 0;count<nElp1;++count)
	  if ((*p1)[count] < dzero) // don't know if a "machar" value exists
	    e->Throw("Value of N is out of allowed range (Only positive values when N is non integer).");

	// when X value is below ~1e-20 --> return -Inf (we use log(0.) which gives -Inf)
	const double smallVal = 1e-38 ;

	if(nElp0==1)
	  {
	    for (count = 0;count<nElp;++count)
	      if (abs((*p0)[0]) < smallVal)
		(*res)[count] =log(dzero) ;
	      else
		(*res)[count] = gsl_sf_bessel_Knu((*p1_float)[count],(*p0)[0]);
	  }
	else if(nElp1==1)
	  {
	    for (count = 0;count<nElp;++count)
	      if (abs((*p0)[count]) < smallVal)
		(*res)[count] =log(dzero) ;
	      else
		(*res)[count] = gsl_sf_bessel_Knu((*p1_float)[0],(*p0)[count]);
	  }
	else
	  {
	    for (count = 0;count<nElp;++count)
	      if (abs((*p0)[count]) < smallVal)
		(*res)[count] =log(dzero) ;
	      else
		(*res)[count] = gsl_sf_bessel_Knu((*p1_float)[count],(*p0)[count]);
	  }
      }
    GM_CC1();
    GM_CV1();
  }
    
  // very preliminary version:
  // should not work when N LT 0
  // should return Inf at x==0
  // should not work for x LT 0
    
  BaseGDL* besely_fun(EnvT* e)
  { 
    AC_HELP();
    GM_5P0(1);
    AC_2P1();
    GM_DF2();

    // we need to check if N values (array) are Integer or not
    SizeT count;
    // do we have negative numbers ?
    for (count = 0;count<nElp0;++count)
      if ((*p0)[count] < dzero) // don't know if a "machar" value exists
        e->Throw("Value of X is out of allowed range (Only positive values).");

    int test=0;
    
    for (count = 0;count<nElp1;++count)
      if (abs((*p1_float)[count]-(float)(*p1)[count]) > 0.000001) // don't know if a "machar" value exists
	test=1;

    if (test==0)
      {
	// when X value is below ~1e-20 --> return -Inf (we use log(0.) which gives -Inf)
	const double smallVal = 1e-38 ;

	if(nElp0==1)
	  {
	    for (count = 0;count<nElp;++count)
	      if (abs((*p0)[0]) < smallVal)
		(*res)[count] =log(dzero) ;
	      else
		(*res)[count] = gsl_sf_bessel_Yn((*p1)[count],(*p0)[0]);
	  }
	else if(nElp1==1)
	  {
	    for (count = 0;count<nElp;++count)
	      if (abs((*p0)[count]) < smallVal)
		(*res)[count] =log(dzero) ;
	      else
		(*res)[count] = gsl_sf_bessel_Yn((*p1)[0],(*p0)[count]);
	  }
	else
	  {
	    for (count = 0;count<nElp;++count)
	      if (abs((*p0)[count]) < smallVal)
		(*res)[count] =log(dzero) ;
	      else
		(*res)[count] = gsl_sf_bessel_Yn((*p1)[count],(*p0)[count]);
	  }
      }
    else
      {
	// we need to check if N values (array) are positives
	for (count = 0;count<nElp1;++count)
	  if ((*p1)[count] < dzero) // don't know if a "machar" value exists
	    e->Throw("Value of N is out of allowed range (Only positive values when N is non integer).");

	// when X value is below ~1e-20 --> return -Inf (we use log(0.) which gives -Inf)
	const double smallVal = 1e-38 ;

	if(nElp0==1)
	  {
	    for (count = 0;count<nElp;++count)
	      if (abs((*p0)[0]) < smallVal)
		(*res)[count] =log(dzero) ;
	      else
		(*res)[count] = gsl_sf_bessel_Ynu((*p1_float)[count],(*p0)[0]);
	  }
	else if(nElp1==1)
	  {
	    for (count = 0;count<nElp;++count)
	      if (abs((*p0)[count]) < smallVal)
		(*res)[count] =log(dzero) ;
	      else
		(*res)[count] = gsl_sf_bessel_Ynu((*p1_float)[0],(*p0)[count]);
	  }
	else
	  {
	    for (count = 0;count<nElp;++count)
	      if (abs((*p0)[count]) < smallVal)
		(*res)[count] =log(dzero) ;
	      else
		(*res)[count] = gsl_sf_bessel_Ynu((*p1_float)[count],(*p0)[count]);
	  }
      }
    GM_CC1();
    GM_CV1();
  }

  // SPLINE
  // what does not work like IDL : warning messages when Inf/Nan or Zero/Negative X steps, X and Y not same size

  BaseGDL* spl_init_fun( EnvT* e)
  {
    if (e->KeywordSet("HELP")) {
      string inline_help[]={
	"Usage: y2a=SPL_INIT(xa, ya, yp0=yp0, ypn_1= ypn_1, double=double)",
	" -- xa is a N elements *ordered* array",
	" -- ya is a N elements array containing values of the function",
	" -- yp0 is the value of derivate of YA function at first point",
	" -- ypN_1 is the value of derivate of YA function at last point",
	"If X or Y contain NaN or Inf, output is NaN"};
      int size_of_s = sizeof(inline_help) / sizeof(inline_help[0]);
      e->Help(inline_help, size_of_s);
    }
    
    DDoubleGDL* Xpos = e->GetParAs<DDoubleGDL>(0);
    SizeT nElpXpos = Xpos->N_Elements();
 
    DDoubleGDL* Ypos = e->GetParAs<DDoubleGDL>(1);
    SizeT nElpYpos = Ypos->N_Elements();

    // we only issue a message
    if (nElpXpos != nElpYpos) {
	cout << "SPL_INIT (warning): X and Y arrays do not have same lengths !" << endl;
	// all next computations to be done on MIN(nElpXpos,nElpYpos) (except NaN/Inf checks)
    	if (nElpXpos > nElpYpos)
	  nElpXpos=nElpYpos;
    }

    // creating result array
    DDoubleGDL* res;  // the "res" array;
    res = new DDoubleGDL(nElpXpos, BaseGDL::NOZERO);

    SizeT count, count1;

    // before all, we check wether inputs arrays does contains NaN or Inf
    static DStructGDL *Values =  SysVar::Values();
    DDouble d_nan=(*static_cast<DDoubleGDL*>(Values->GetTag(Values->Desc()->TagIndex("D_NAN"), 0)))[0];

    for (count = 0; count < nElpXpos; ++count) {
      if (!isfinite((*Xpos)[count])) {
	cout << "SPL_INIT (fatal): at least one value in X input array is NaN or Inf ..." << endl;
	for (count1 = 0; count1 < nElpXpos; ++count1) (*res)[count1] =d_nan;
	return res;
      }
    }
    for (count = 0; count < nElpYpos; ++count) {
      if (!isfinite((*Ypos)[count])) {
	cout << "SPL_INIT (fatal): at least one value in Y input array is NaN or Inf ..." << endl;
	for (count1 = 0; count1 < nElpXpos; ++count1) (*res)[count1] =d_nan;
	return res;
      }
    }

    // we also check wether X input array is well ordered ...
    double step;
    int flag_skip=0;
    for (count = 1; count < nElpXpos; ++count) {
      step=(*Xpos)[count]-(*Xpos)[count-1];
      if (step < 0.0) {
	if (flag_skip == 0) {
	  cout << "SPL_INIT (warning): at least one x[n+1]-x[n] step is negative: X is assumed to be ordered" << endl;
	  flag_skip = 1; 
	}
      }
      if (abs(step) == 0.0) {
	cout << "SPL_INIT (fatal): at least two consecutive X values are identical" << endl;
 	for (count1 = 0; count1 < nElpXpos; ++count1) (*res)[count1] =d_nan;
	return res;
      }
    }

    auto_ptr<BaseGDL> U_guard;
    DDoubleGDL* U;
    U = new DDoubleGDL(nElpXpos, BaseGDL::NOZERO);
    U_guard.reset(U); // delete upon exit
    
    // may be we will have to check the size of these arrays ?

    BaseGDL* Yderiv0=e->GetKW(e->KeywordIx("YP0"));
    DDoubleGDL* YP0;

    if(Yderiv0 !=NULL && !isinf((*(YP0=e->GetKWAs<DDoubleGDL>(e->KeywordIx("YP0"))))[0] )){ 
    // first derivative at the point X0 is defined and different to Inf
      (*res)[0]=-0.5;
      (*U)[0] = ( 3. / ((*Xpos)[1]-(*Xpos)[0])) * (((*Ypos)[1]-(*Ypos)[0]) / 
                ((*Xpos)[1]-(*Xpos)[0]) - (*YP0)[0] );

    }else{ 
      // YP0 is omitted or equal to Inf
      (*res)[0]=0.;
      (*U)[0]=0.;
    }
  
    double psig, pu, x, xm, xp, y, ym, yp, p, dx, qn;

    for (count = 1; count < nElpXpos-1; ++count) {
      x=(*Xpos)[count];
      xm=(*Xpos)[count-1];
      xp=(*Xpos)[count+1];
      psig=(x-xm)/(xp-xm);
      
      y=(*Ypos)[count];
      ym=(*Ypos)[count-1];
      yp=(*Ypos)[count+1];
      pu=((ym-y)/(xm-x)-(y-yp)/(x-xp))/(xm-xp);
      
      p=psig*(*res)[count-1]+2.;
      (*res)[count]=(psig-1.)/p;
      (*U)[count]=(6.00*pu-psig*(*U)[count-1])/p;
    }

    BaseGDL* YderivN=e->GetKW(e->KeywordIx("YPN_1"));
    DDoubleGDL* YPN;

    if(YderivN !=NULL && !isinf((*(YPN=e->GetKWAs<DDoubleGDL>(e->KeywordIx("YPN_1"))))[0] )){ 
    // first derivative at the point XN-1 is defined and different to Inf 
      (*res)[nElpXpos-1] =0.;
      qn=0.5;

      dx=((*Xpos)[nElpXpos-1]-(*Xpos)[nElpXpos-2]);
      (*U)[nElpXpos-1]= (3./dx)*((*YPN)[0]-((*Ypos)[nElpXpos-1]-(*Ypos)[nElpXpos-2])/dx);

    }else{
    // YPN_1 is omitted or equal to Inf
      qn=0.;
      (*U)[nElpXpos-1]=0.;
    } 

    (*res)[nElpXpos-1] =((*U)[nElpXpos-1]-qn*(*U)[nElpXpos-2])/(qn*(*res)[nElpXpos-2]+ 1.);

    for (count = nElpXpos-2; count != -1; --count){
       (*res)[count] =(*res)[count]*(*res)[count+1]+(*U)[count];
    }
      
    GM_CV0();
    
  }
  
  BaseGDL* spl_interp_fun( EnvT* e)
  {
    if (e->KeywordSet("HELP")) {
      //  string inline_help[]={};
      // e->Help(inline_help, 0);
      string inline_help[]={
	"Usage: res=SPL_INTERP(xa, ya, y2a, new_x, double=double)",
	" -- xa is a N elements *ordered* array",
	" -- ya is a N elements array containing values of the function",
	" -- y2a is the value of derivate of YA function at first point",
	" -- new_x is an array for new X positions where we want to compute SPLINE",
	"This function should be called only after use of SPL_INIT() !"};
      int size_of_s = sizeof(inline_help) / sizeof(inline_help[0]);
      e->Help(inline_help, size_of_s);
    }   

    DDoubleGDL* Xpos = e->GetParAs<DDoubleGDL>(0);
    SizeT nElpXpos = Xpos->N_Elements();
    DType t0 = e->GetParDefined(0)->Type(); 

    DDoubleGDL* Ypos = e->GetParAs<DDoubleGDL>(1);
    SizeT nElpYpos = Ypos->N_Elements();

    DDoubleGDL* Yderiv2 = e->GetParAs<DDoubleGDL>(2);
    SizeT nElpYderiv2 = Yderiv2->N_Elements();

    // Do the 3 arrays have same lengths ?
    if ((nElpXpos != nElpYpos) || (nElpXpos != nElpYderiv2))
      e->Throw("Arguments XA, YA, and Y2A must have the same number of elements.");
    
    DDoubleGDL* Xnew = e->GetParAs<DDoubleGDL>(3);
    SizeT nElpXnew = Xnew->N_Elements();
    
    DDoubleGDL* res;
    res = new DDoubleGDL(nElpXnew, BaseGDL::NOZERO);

    int debug =0;
    SizeT ilo, ihi, imiddle;
    double xcur, xposcur, h, aa, bb;

    for (SizeT count = 0; count < nElpXnew; ++count) {
      xcur=(*Xnew)[count];
      ilo=0;
      ihi=nElpXpos-1;
      while ((ihi-ilo) > 1){
	imiddle=(ilo+ihi)/2;
	xposcur=(*Xpos)[imiddle];
	if (xposcur > xcur) ihi=imiddle;
	else ilo=imiddle;
      }
      h=(*Xpos)[ihi]-(*Xpos)[ilo];
      if (abs(h) == 0.0)  e->Throw("SPL_INTERP: Bad XA input (XA not ordered or zero step in XA).");
      
      if (debug == 1) cout << "h " << h << " lo/hi" << ilo << " " <<ihi<< endl;
      aa=((*Xpos)[ihi]-xcur)/h;
      bb=(xcur-(*Xpos)[ilo])/h;
      (*res)[count]=aa*(*Ypos)[ilo]+bb*(*Ypos)[ihi];
      (*res)[count]=(*res)[count]+((aa*aa*aa-aa)*(*Yderiv2)[ilo]+(bb*bb*bb-bb)*(*Yderiv2)[ihi])*(h*h)/6.;
    }

    GM_CV0();
  }

  BaseGDL* sobel_fun( EnvT* e){
    e->Throw( "sorry, SOBEL not ready.");
    return NULL;
  }

  BaseGDL* roberts_fun( EnvT* e){
  
    DDoubleGDL* p0 = e->GetParAs<DDoubleGDL>(0);
    if( p0->Rank() != 2)
      e->Throw( "Array must have 2 dimensions: "+ e->GetParString(0));

    DDoubleGDL* res = new DDoubleGDL(p0->Dim(), BaseGDL::NOZERO);
    SizeT nbX = p0->Dim(0);
    SizeT nbY = p0->Dim(1);
    
    int debug=0;
    if (debug ==1) {
      cout << "nbX : " << nbX << endl; 
      cout << "nbY : " << nbY << endl; 
    }

    //DDoubleGDL z = new DDoubleGDL[nbX,nbY];
    for( SizeT k=0; k<nbY-1; k++) {
      for( SizeT j=0; j< nbX-1; j++) {
	(*res)[j+nbX*k] = abs((*p0)[j+nbX*k]-(*p0)[j+1+nbX*(k+1)])+
	  abs((*p0)[j+nbX*(k+1)]-(*p0)[j+1+nbX*k]);
      }
    }
    return res;
  }
  
  BaseGDL* prewitt_fun( EnvT* e){
    e->Throw( "sorry, PREWITT not ready.");
    return NULL;
  }

  // AC 04-Feb-2013 : an experimental Fast Matrix Multiplication code
  // require to be compiled with the Eigen Lib 
  // (succesfully tested with Eigen 3.2 and gcc 4.1, 
  // as fast as IDL 8 on M#transpose(M), with M=[400,60000] matrix multiply.)
#if defined(USE_EIGEN)
  
  BaseGDL* matmul_fun( EnvT* e){

    if (e->KeywordSet("AVAILABLE")) return new DLongGDL(1);
    
    if (e->GetParDefined(0)->Type() == GDL_STRING)
      e->Throw( "Array Type cannot be STRING here: "+ e->GetParString(0));
    if (e->GetParDefined(1)->Type() == GDL_STRING)
      e->Throw( "Array Type cannot be STRING here: "+ e->GetParString(1));
    if (e->GetParDefined(0)->Type() == GDL_STRUCT)
      e->Throw( "Array Type cannot be a STRUCTURE here: "+ e->GetParString(0));
    if (e->GetParDefined(1)->Type() == GDL_STRUCT)
      e->Throw( "Array Type cannot be STRUCTURE here: "+ e->GetParString(1));
    
    int debug=0;
    if (e->KeywordSet("DEBUG") || (debug == 1)) {
      cout << "Rank Matrix A : "<< e->GetParDefined(0)->Rank() << endl;
      cout << "Dim Matrix A : " << e->GetParDefined(0)->Dim() << endl;
      cout << "Rank Matrix B : "<< e->GetParDefined(1)->Rank() << endl;
      cout << "Dim Matrix B : " << e->GetParDefined(1)->Dim() << endl;
    }

    if (e->GetParDefined(0)->Rank() > 2)
      e->Throw( "Array must have 1 or 2 dimensions: "+ e->GetParString(0));
    if (e->GetParDefined(1)->Rank() > 2)
      e->Throw( "Array must have 1 or 2 dimensions: "+ e->GetParString(1));
    
    if ((e->GetParDefined(0)->Type() == GDL_COMPLEX) || (e->GetParDefined(0)->Type() == GDL_COMPLEXDBL))
      e->Throw( "We are not ready here for COMPLEX type, plese use # operator: "+ e->GetParString(0));
    if ((e->GetParDefined(1)->Type() == GDL_COMPLEX) || (e->GetParDefined(1)->Type() == GDL_COMPLEXDBL))
      e->Throw( "We are not ready here for COMPLEX type, plese use # operator: "+ e->GetParString(1));

    DDoubleGDL* p0 = e->GetParAs<DDoubleGDL>(0);
    DDoubleGDL* p1 = e->GetParAs<DDoubleGDL>(1);
    
    long NbCol0, NbRow0, NbCol1, NbRow1, tmp_permut;

    if (e->GetParDefined(0)->Rank() == 2) {
      NbCol0=p0->Dim(0);
      NbRow0=p0->Dim(1);
    } else {
      NbCol0=p0->Dim(0);
      NbRow0=1;
    }
    if (e->KeywordSet("ATRANSPOSE")) {
      tmp_permut=NbCol0;
      NbCol0=NbRow0;
      NbRow0=tmp_permut;
    } 
    //    cout << "NbCol0, NbRow0 : "<< NbCol0 << " " << NbRow0 << endl;
    MatrixXd m0 (NbCol0,NbRow0);

    if (e->KeywordSet("ATRANSPOSE")) {
      for (SizeT j=0; j<NbRow0; j++) 
	for (SizeT i=0; i<NbCol0; i++)
	  m0(i,j)=(*p0)[i*NbRow0+j];
    }else {
      for (SizeT i=0; i<NbCol0; i++) 
	for (SizeT j=0; j<NbRow0; j++)
	  m0(i,j)=(*p0)[j*NbCol0+i];
    }

    //    cout << m0 << endl;

    if (e->GetParDefined(1)->Rank() == 2) {
      NbCol1=p1->Dim(0);
      NbRow1=p1->Dim(1);
    } else {
      NbCol1=p1->Dim(0);
      NbRow1=1;
    }
    if (e->KeywordSet("BTRANSPOSE")) {
      tmp_permut=NbCol1;
      NbCol1=NbRow1;
      NbRow1=tmp_permut;
    } 

    MatrixXd m1 (NbCol1,NbRow1);

    if (e->KeywordSet("BTRANSPOSE")) {
      for (SizeT j=0; j<NbRow1; j++) 
	for (SizeT i=0; i<NbCol1; i++)
	  m1(i,j)=(*p1)[i*NbRow1+j];
    } else {
      for (SizeT i=0; i<NbCol1; i++) 
	for (SizeT j=0; j<NbRow1; j++)
	  m1(i,j)=(*p1)[j*NbCol1+i];
    }
    if (NbRow0 != NbCol1)
      e->Throw( "Incompatible dimensions [m,n]#[n,o] expected ");

    if (e->KeywordSet("DEBUG") || (debug == 1)) {
      cout << "NbCol0, NbRow0 : "<< NbCol0 << " " << NbRow0 << endl;
      cout << "NbCol1, NbRow1 : "<< NbCol1 << " " << NbRow1 << endl;
      //cout << m0 << endl;
    }

    MatrixXd tmp_res (NbCol0,NbRow1);
    tmp_res=m0*m1;

    dimension dim(NbCol0,NbRow1);

    if ((e->GetParDefined(0)->Type() == GDL_DOUBLE) || (e->GetParDefined(1)->Type() == GDL_DOUBLE)) {
      DDoubleGDL* res = new DDoubleGDL(dim, BaseGDL::NOZERO);
      for (SizeT i=0; i<NbCol0; i++) 
	for (SizeT j=0; j<NbRow1; j++)
	  (*res)[j*NbCol0+i] = tmp_res(i,j);
      return res;
    } else {
      DFloatGDL* res = new DFloatGDL(dim, BaseGDL::NOZERO);
      for (SizeT i=0; i<NbCol0; i++) 
	for (SizeT j=0; j<NbRow1; j++)
	  (*res)[j*NbCol0+i] = tmp_res(i,j);      
      return res;
    }
  }
  
#else
  BaseGDL* matmul_fun( EnvT* e){
    
    if (e->KeywordSet("AVAILABLE")) {
      if (!e->KeywordSet("QUIET")) {
	Message(e->GetProName()+": GDL was compiled without Eigen Lib. support.");
	Message(e->GetProName()+": This Lib. provides fast algo. for MATRIX_MULTIPLY() function");
      }
      return new DLongGDL(0);
    }
    
    e->Throw( "sorry, MATMUL not ready. GDL must be compiled with Eigen lib.");
    return NULL;
  }
  
#endif

} // namespace

