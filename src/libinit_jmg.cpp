/***************************************************************************
                  libinit_jmg.cpp  -  initialization of GDL library routines
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#else
// default: assume we have HDF
#define USE_HDF 1
#endif

#include <string>
#include <fstream>

#include "envt.hpp"
#include "dpro.hpp"


#include "basic_fun_jmg.hpp"
#include "math_fun_jmg.hpp"
#include "basic_pro_jmg.hpp"

#include "gsl_fun.hpp"
#include "image.hpp"

#if defined(USE_HDF)
#include "hdf_fun.hpp"
#include "hdf_pro.hpp"
#endif

using namespace std;

void LibInit_jmg()
{
  const char KLISTEND[] = "";
  
  // the following by Joel Gales <jomoga@users.sourceforge.net>
  const string sizeKey[]={"L64","DIMENSIONS","FILE_LUN","N_DIMENSIONS",
			  "N_ELEMENTS","STRUCTURE","TNAME","TYPE",KLISTEND};
  new DLibFun(lib::size,string("SIZE"),1,sizeKey);
  new DLibFun(lib::fstat,string("FSTAT"),1);

#if defined(HAVE_LIBGSL) && defined(HAVE_LIBGSLCBLAS)
  const string invertKey[]={"DOUBLE",KLISTEND};
  new DLibFun(lib::invert_fun,string("INVERT"),2,invertKey);

  const string fftKey[]={"DOUBLE","INVERSE","OVERWRITE",KLISTEND};
  new DLibFun(lib::fft_fun,string("FFT"),2,fftKey);

  const string randomKey[]={"DOUBLE","GAMMA","LONG","NORMAL",
			    "BINOMIAL","POISSON",KLISTEND};
  new DLibFun(lib::random_fun,string("RANDOMU"),MAXRANK,randomKey);
  new DLibFun(lib::random_fun,string("RANDOMN"),MAXRANK,randomKey);

  const string checkmathKey[]={"MASK","NOCLEAR","PRINT",KLISTEND};
  new DLibFun(lib::check_math_fun,string("CHECK_MATH"),0,checkmathKey);

  const string histogramKey[]={"BINSIZE","INPUT","MAX","MIN","NBINS",
			       "OMAX","OMIN","REVERSE_INDICES",KLISTEND};
  new DLibFun(lib::histogram_fun,string("HISTOGRAM"),1,histogramKey);
#endif

  const string macharKey[]={"DOUBLE",KLISTEND};
  new DLibFun(lib::machar_fun,string("MACHAR"),0,macharKey);

  const string finiteKey[]={"INFINITY",KLISTEND};
  new DLibFun(lib::finite_fun,string("FINITE"),1,finiteKey);

  const string make_arrayKey[]={"NOZERO","DIMENSION","INDEX","SIZE",
				"TYPE","VALUE","BYTE","INT",
				"UINT","LONG","ULONG","L64","UL64",
				"FLOAT","DOUBLE","COMPLEX",
				"DCOMPLEX",KLISTEND};
  new DLibFun(lib::make_array,string("MAKE_ARRAY"),MAXRANK,make_arrayKey);

  const string reformKey[]={"OVERWRITE",KLISTEND};
  new DLibFun(lib::reform,string("REFORM"),MAXRANK,reformKey);

  new DLibPro(lib::point_lun,string("POINT_LUN"),2);

  new DLibPro(lib::linkimage,string("LINKIMAGE"),4);

#if defined(USE_HDF)
  const string hdf_openKey[]={"READ","RDWR","CREATE",KLISTEND};
  new DLibFun(lib::hdf_open_fun,string("HDF_OPEN"),2,hdf_openKey);

  new DLibFun(lib::hdf_vg_getid_fun,string("HDF_VG_GETID"),2);

  const string hdf_vg_attachKey[]={"READ","WRITE",KLISTEND};
  new DLibFun(lib::hdf_vg_attach_fun,string("HDF_VG_ATTACH"),2,hdf_vg_attachKey);

  const string hdf_vd_attachKey[]={"READ","WRITE",KLISTEND};
  new DLibFun(lib::hdf_vd_attach_fun,string("HDF_VD_ATTACH"),2,hdf_vd_attachKey);
  new DLibFun(lib::hdf_vd_find_fun,string("HDF_VD_FIND"),2);

  const string hdf_vd_readKey[]={"FIELDS","NRECORDS","FULL_INTERLACE","NO_INTERLACE",KLISTEND};
  new DLibFun(lib::hdf_vd_read_fun,string("HDF_VD_READ"),2,hdf_vd_readKey);

  const string hdf_vg_getinfoKey[]={"CLASS","NAME","NENTRIES","REF","TAG",KLISTEND};
  new DLibPro(lib::hdf_vg_getinfo_pro,string("HDF_VG_GETINFO"),1,
	      hdf_vg_getinfoKey);

  const string hdf_vd_getKey[]={"CLASS","NAME","COUNT","REF","TAG",KLISTEND};
  new DLibPro(lib::hdf_vd_get_pro,string("HDF_VD_GET"),1,
	      hdf_vd_getKey);


  new DLibPro(lib::hdf_vg_gettrs_pro,string("HDF_VG_GETTRS"),3);
  new DLibPro(lib::hdf_vg_detach_pro,string("HDF_VG_DETACH"),1);

  new DLibPro(lib::hdf_vd_detach_pro,string("HDF_VD_DETACH"),1);

  const string hdf_sd_createKey[]={"BYTE","DFNT_INT8","DFNT_UINT8",
				   "SHORT","INT","DFNT_INT16","DFNT_UINT16",
				   "LONG","DFNT_INT32","DFNT_UINT32",
				   "FLOAT","DFNT_FLOAT32",
				   "DOUBLE","DFNT_FLOAT64",
				   "STRING","DFNT_CHAR","HDF_TYPE",KLISTEND};
  new DLibFun(lib::hdf_sd_create_fun,string("HDF_SD_CREATE"),3,
	      hdf_sd_createKey);

  const string hdf_sd_startKey[]={"READ","RDWR","CREATE",KLISTEND};
  new DLibFun(lib::hdf_sd_start_fun,string("HDF_SD_START"),2,hdf_sd_startKey);
  new DLibFun(lib::hdf_sd_nametoindex_fun,string("HDF_SD_NAMETOINDEX"),2);
  new DLibFun(lib::hdf_sd_attrfind_fun,string("HDF_SD_ATTRFIND"),2);
  new DLibFun(lib::hdf_sd_select_fun,string("HDF_SD_SELECT"),2);

  const string hdf_sd_getdataKey[]={"START","STRIDE","COUNT",KLISTEND};
  new DLibPro(lib::hdf_sd_getdata_pro,string("HDF_SD_GETDATA"),2,
	      hdf_sd_getdataKey);
  const string hdf_sd_adddataKey[]={"START","STRIDE","COUNT",KLISTEND};
  new DLibPro(lib::hdf_sd_adddata_pro,string("HDF_SD_ADDDATA"),2,
	      hdf_sd_adddataKey);
  new DLibPro(lib::hdf_sd_fileinfo_pro,string("HDF_SD_FILEINFO"),3);

  const string hdf_sd_getinfoKey[]={"DIMS","HDF_TYPE","NAME",
				    "NATTS","NDIMS","TYPE",KLISTEND};
  new DLibPro(lib::hdf_sd_getinfo_pro,string("HDF_SD_GETINFO"),1,
	      hdf_sd_getinfoKey);

  const string hdf_sd_attrinfoKey[]={"COUNT","DATA","HDF_TYPE","NAME",
				     "TYPE",KLISTEND};
  new DLibPro(lib::hdf_sd_attrinfo_pro,string("HDF_SD_ATTRINFO"),2,
	      hdf_sd_attrinfoKey);

  new DLibPro(lib::hdf_sd_endaccess_pro,string("HDF_SD_ENDACCESS"),1);
  new DLibPro(lib::hdf_sd_end_pro,string("HDF_SD_END"),1);
  new DLibPro(lib::hdf_close_pro,string("HDF_CLOSE"),1);
#endif

  const string tvKey[]={"TRUE",KLISTEND};
  new DLibPro(lib::tv,string("TV"),4,tvKey);

  const string tvrdKey[]={"CHANNEL","ORDER","TRUE","WORDS",KLISTEND};
  new DLibFun(lib::tvrd,string("TVRD"),5,tvrdKey);

  new DLibPro(lib::loadct,string("LOADCT"),1);

}

