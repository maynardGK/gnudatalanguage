/***************************************************************************
                     libinit.cpp  -  initialization of GDL library routines
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

#include <string>
#include <fstream>

#include "envt.hpp"
#include "dpro.hpp"

#include "basic_fun.hpp"
#include "basic_pro.hpp"

#include "math_fun.hpp"

#include "plotting.hpp"

#include "file.hpp"

#include "gsl_fun.hpp"

#ifdef USE_PYTHON
#include "gdlpython.hpp"
#endif

// for extensions
#include "new.hpp"

void LibInit_jmg(); // libinit_jmg.cpp
void LibInit_cl(); // libinit_cl.cpp
void LibInit_mes(); // libinit_mes.cpp

using namespace std;

void LibInit()
{
  const char KLISTEND[] = "";

  const string bytsclKey[]={"MIN","MAX","TOP",KLISTEND};
  new DLibFun(lib::bytscl,string("BYTSCL"),1,bytsclKey);

  const string n_tagsKey[]={"LENGTH",KLISTEND};
  new DLibFun(lib::n_tags,string("N_TAGS"),1,n_tagsKey);
  
  const string byteorderKey[]={"SSWAP","LSWAP","L64SWAP",
			       "SWAP_IF_BIG_ENDIAN",
			       "SWAP_IF_LITTLE_ENDIAN",KLISTEND};
  new DLibPro(lib::byteorder,string("BYTEORDER"),-1,byteorderKey);

  const string obj_classKey[]={"COUNT","SUPERCLASS",KLISTEND};
  new DLibFun(lib::obj_class,string("OBJ_CLASS"),1,obj_classKey);

  new DLibFun(lib::obj_isa,string("OBJ_ISA"),2);

  const string rebinKey[]={"SAMPLE",KLISTEND};
  new DLibFun(lib::rebin_fun,string("REBIN"),9,rebinKey);

  const string convolKey[]={"CENTER","EDGE_TRUNCATE","EDGE_WRAP",KLISTEND};
  new DLibFun(lib::convol,string("CONVOL"),3,convolKey);

  const string file_searchKey[]={"COUNT","EXPAND_ENVIRONMENT","EXPAND_TILDE",
				 "FOLD_CASE","ISSUE_ACCESS_ERROR",
				 "MARK_DIRECTORY","NOSORT","QUOTE",
				 "MATCH_INITIAL_DOT",
				 "MATCH_ALL_INITIAL_DOT",KLISTEND};
  new DLibFun(lib::file_search,string("FILE_SEARCH"),2,file_searchKey);

  const string expand_pathKey[]={"ARRAY","ALL_DIRS","COUNT",KLISTEND};
  new DLibFun(lib::expand_path,string("EXPAND_PATH"),1,expand_pathKey);
  
  const string strjoinKey[]={"SINGLE",KLISTEND};
  new DLibFun(lib::strjoin,string("STRJOIN"),2,strjoinKey);

  new DLibFun(lib::eof_fun,string("EOF"),1);

  new DLibFun(lib::arg_present,string("ARG_PRESENT"),1);

  const string messageKey[]={"CONTINUE","INFORMATIONAL","IOERROR",
			     "NONAME","NOPREFIX","NOPRINT",
			     "RESET","TRACEBACK",KLISTEND};
  new DLibPro(lib::message,string("MESSAGE"),1,messageKey);
  
  const string cdKey[]={"CURRENT",KLISTEND};
  new DLibPro(lib::cd_pro,string("CD"),1,cdKey);

  const string file_testKey[]={"DIRECTORY","EXECUTABLE","READ",
			       "REGULAR","WRITE","ZERO_LENGTH",
			       "GET_MODE",
			       "BLOCK_SPECIAL","CHARACTER_SPECIAL",
			       "NAMED_PIPE","SOCKET","SYMLINK",KLISTEND};
  new DLibFun(lib::file_test,string("FILE_TEST"),1,file_testKey);

  new DLibFun(lib::shift_fun,string("SHIFT"),9);

  const string sortKey[]={"L64",KLISTEND};
  new DLibFun(lib::sort_fun,string("SORT"),1,sortKey);

  new DLibFun(lib::transpose,string("TRANSPOSE"),2);

  new DLibPro(lib::retall,string("RETALL"));

  const string catchKey[]={"CANCEL",KLISTEND};
  new DLibPro(lib::catch_pro,string("CATCH"),1,catchKey);
  new DLibPro(lib::on_error,string("ON_ERROR"),1);

  const string exitKey[]={"NO_CONFIRM","STATUS",KLISTEND};
  new DLibPro(lib::exitgdl,string("EXIT"),0,exitKey);
  
  const string helpKey[]={"STRUCTURES","LIB",KLISTEND};
  new DLibPro(lib::help,string("HELP"),-1,helpKey);

  // printKey, readKey and stringKey are closely associated
  // as the same functions are called "FORMAT" till "MONTH"
  // must be the first four keywords
  const string printKey[]={"FORMAT","AM_PM","DAYS_OF_WEEK","MONTH",
			   "STDIO_NON_FINITE",KLISTEND};
  new DLibPro(lib::print,string("PRINT"),-1,printKey);
  new DLibPro(lib::printf,string("PRINTF"),-1,printKey);

  const string readKey[]={"FORMAT","AM_PM","DAYS_OF_WEEK","MONTH",
			  "PROMPT",KLISTEND};
  new DLibPro(lib::read,string("READ"),-1,readKey);
  new DLibPro(lib::readf,string("READF"),-1,readKey);

  const string readsKey[]={"FORMAT","AM_PM","DAYS_OF_WEEK","MONTH",
			   KLISTEND}; // no PROMPT
  new DLibPro(lib::reads,string("READS"),-1,readsKey);

  // allow printing (of expressions) with all keywords 
  // (easier to implement this way)
  new DLibPro(lib::stop,string("STOP"),-1,printKey); 

  const string defsysvKey[]={"EXISTS",KLISTEND};
  new DLibPro(lib::defsysv,string("DEFSYSV"),3,defsysvKey); 

  new DLibPro(lib::ptr_free,string("PTR_FREE"),-1);

  const string arrKey[]={"NOZERO",KLISTEND};
  new DLibFun(lib::bytarr,string("BYTARR"),MAXRANK,arrKey);
  new DLibFun(lib::intarr,string("INTARR"),MAXRANK,arrKey);
  new DLibFun(lib::uintarr,string("UINTARR"),MAXRANK,arrKey);
  new DLibFun(lib::lonarr,string("LONARR"),MAXRANK,arrKey);
  new DLibFun(lib::ulonarr,string("ULONARR"),MAXRANK,arrKey);
  new DLibFun(lib::lon64arr,string("LON64ARR"),MAXRANK,arrKey);
  new DLibFun(lib::ulon64arr,string("ULON64ARR"),MAXRANK,arrKey);
  new DLibFun(lib::fltarr,string("FLTARR"),MAXRANK,arrKey);
  new DLibFun(lib::dblarr,string("DBLARR"),MAXRANK,arrKey);
  new DLibFun(lib::strarr,string("STRARR"),MAXRANK,arrKey);
  new DLibFun(lib::complexarr,string("COMPLEXARR"),MAXRANK,arrKey);
  new DLibFun(lib::dcomplexarr,string("DCOMPLEXARR"),MAXRANK,arrKey);

  const string ptrArrKey[]={"NOZERO","ALLOCATE_HEAP",KLISTEND};
  new DLibFun(lib::ptrarr,string("PTRARR"),MAXRANK,ptrArrKey);
  new DLibFun(lib::objarr,string("OBJARR"),MAXRANK,arrKey);

  const string ptr_newKey[]={"NO_COPY","ALLOCATE_HEAP",KLISTEND};
  new DLibFun(lib::ptr_new,string("PTR_NEW"),1,ptr_newKey);

  const string obj_validKey[]={"CAST","COUNT",KLISTEND};
  new DLibFun(lib::ptr_valid,string("PTR_VALID"),1,obj_validKey);
  new DLibFun(lib::obj_valid,string("OBJ_VALID"),1,obj_validKey);
  
  const string obj_newKey[]={"_REF_EXTRA",KLISTEND};
  new DLibFun(lib::obj_new,string("OBJ_NEW"),-1,obj_newKey);
  new DLibPro(lib::obj_destroy,string("OBJ_DESTROY"),-1,obj_newKey);

  // call... fun/pro
  new DLibFun(lib::call_function,string("CALL_FUNCTION"),-1,obj_newKey);
  new DLibPro(lib::call_procedure,string("CALL_PROCEDURE"),-1,obj_newKey);
  new DLibFun(lib::call_method_function, string("CALL_METHOD"),-1,obj_newKey);
  new DLibPro(lib::call_method_procedure,string("CALL_METHOD"),-1,obj_newKey);
  
  const string indKey[]={"TYPE","BYTE","COMPLEX","DCOMPLEX",
			 "DOUBLE","FLOAT","L64","LONG",
			 "STRING","UINT","UL64","ULONG",KLISTEND};
  new DLibFun(lib::bindgen,string("BINDGEN"),MAXRANK);
  new DLibFun(lib::indgen,string("INDGEN"),MAXRANK,indKey);
  new DLibFun(lib::uindgen,string("UINDGEN"),MAXRANK,indKey);
  new DLibFun(lib::sindgen,string("SINDGEN"),MAXRANK);
  new DLibFun(lib::lindgen,string("LINDGEN"),MAXRANK);
  new DLibFun(lib::ulindgen,string("ULINDGEN"),MAXRANK);
  new DLibFun(lib::l64indgen,string("L64INDGEN"),MAXRANK);
  new DLibFun(lib::ul64indgen,string("UL64INDGEN"),MAXRANK);
  new DLibFun(lib::findgen,string("FINDGEN"),MAXRANK);
  new DLibFun(lib::dindgen,string("DINDGEN"),MAXRANK);
  new DLibFun(lib::cindgen,string("CINDGEN"),MAXRANK);
  new DLibFun(lib::dcindgen,string("DCINDGEN"),MAXRANK);

  new DLibFun(lib::n_elements,string("N_ELEMENTS"),1);

  new DLibFun(lib::execute,string("EXECUTE"),2);

  const string openKey[]={"APPEND","COMPRESS","BUFSIZE",
			  "DELETE","ERROR","F77_UNFORMATTED",
			  "GET_LUN" /*7*/,"MORE","STDIO",
			  "SWAP_ENDIAN","SWAP_IF_BIG_ENDIAN",
			  "SWAP_IF_LITTLE_ENDIAN" /*12*/,
			  "VAX_FLOAT","WIDTH","XDR", "BLOCK",
			  "NOAUTOMODE","BINARY",
			  KLISTEND};
  new DLibPro(lib::openr,string("OPENR"),3,openKey);
  new DLibPro(lib::openw,string("OPENW"),3,openKey);
  new DLibPro(lib::openu,string("OPENU"),3,openKey);
  new DLibPro(lib::get_lun,string("GET_LUN"),1);

  const string closeKey[]={"EXIT_STATUS","FORCE","FILE","ALL",KLISTEND};
  new DLibPro(lib::close_lun,string("CLOSE"),-1,closeKey);
  
  const string free_lunKey[]={"EXIT_STATUS","FORCE",KLISTEND};
  new DLibPro(lib::free_lun,string("FREE_LUN"),-1,free_lunKey);

  const string writeuKey[]={"TRANSFER_COUNT",KLISTEND};
  new DLibPro(lib::writeu,string("WRITEU"),-1,writeuKey);
  new DLibPro(lib::readu,string("READU"),-1,writeuKey);

  const string assocKey[]={"PACKED",KLISTEND};
  new DLibFun(lib::assoc,string("ASSOC"),3,assocKey);
  
  const string stringKey[]={"FORMAT","AM_PM","DAYS_OF_WEEK","MONTH",
			    "PRINT",KLISTEND};
  new DLibFun(lib::string_fun,string("STRING"),-1,stringKey);
  new DLibFun(lib::byte_fun,string("BYTE"),10);
  new DLibFun(lib::fix_fun,string("FIX"),10);
  new DLibFun(lib::uint_fun,string("UINT"),10);
  new DLibFun(lib::long_fun,string("LONG"),10);
  new DLibFun(lib::ulong_fun,string("ULONG"),10);
  new DLibFun(lib::long64_fun,string("LONG64"),10);
  new DLibFun(lib::ulong64_fun,string("ULONG64"),10);
  new DLibFun(lib::float_fun,string("FLOAT"),10);
  new DLibFun(lib::double_fun,string("DOUBLE"),10);
  new DLibFun(lib::complex_fun,string("COMPLEX"),MAXRANK+2);
  new DLibFun(lib::dcomplex_fun,string("DCOMPLEX"),MAXRANK+2);

  new DLibFun(lib::gdl_logical_and,string("LOGICAL_AND"),2);
  new DLibFun(lib::gdl_logical_or,string("LOGICAL_OR"),2);
  new DLibFun(lib::logical_true,string("LOGICAL_TRUE"),1);

  new DLibFun(lib::replicate,string("REPLICATE"),9);

  new DLibFun(lib::sin_fun,string("SIN"),1);
  new DLibFun(lib::cos_fun,string("COS"),1);
  new DLibFun(lib::tan_fun,string("TAN"),1);

  new DLibFun(lib::sinh_fun,string("SINH"),1);
  new DLibFun(lib::cosh_fun,string("COSH"),1);
  new DLibFun(lib::tanh_fun,string("TANH"),1);

  new DLibFun(lib::asin_fun,string("ASIN"),1);
  new DLibFun(lib::acos_fun,string("ACOS"),1);
  new DLibFun(lib::atan_fun,string("ATAN"),2);

  new DLibFun(lib::alog_fun,string("ALOG"),1);
  new DLibFun(lib::alog10_fun,string("ALOG10"),1);

  new DLibFun(lib::sqrt_fun,string("SQRT"),1);
  new DLibFun(lib::abs_fun,string("ABS"),1);

  new DLibFun(lib::exp_fun,string("EXP"),1);

  new DLibFun(lib::round_fun,string("ROUND"),1);
  new DLibFun(lib::ceil_fun,string("CEIL"),1);
  new DLibFun(lib::floor_fun,string("FLOOR"),1);

  new DLibFun(lib::conj_fun,string("CONJ"),1);
  new DLibFun(lib::imaginary_fun,string("IMAGINARY"),1);
  
  const string strcompressKey[]={"REMOVE_ALL",KLISTEND};
  new DLibFun(lib::strcompress,string("STRCOMPRESS"),1,strcompressKey);
  new DLibFun(lib::strlowcase,string("STRLOWCASE"),1);
  new DLibFun(lib::strupcase,string("STRUPCASE"),1);
  new DLibFun(lib::strlen,string("STRLEN"),1);
  const string strmidKey[]={"REVERSE_OFFSET",KLISTEND};
  new DLibFun(lib::strmid,string("STRMID"),3,strmidKey);
  new DLibFun(lib::strtrim,string("STRTRIM"),2);
  const string strposKey[]={"REVERSE_OFFSET","REVERSE_SEARCH",KLISTEND};
  new DLibFun(lib::strpos,string("STRPOS"),3,strposKey);
  new DLibPro(lib::strput,string("STRPUT"),3);
  
  const string whereKey[]={"COMPLEMENT","NCOMPLEMENT",KLISTEND};
  new DLibFun(lib::where,string("WHERE"),2,whereKey);

  const string totalKey[]={"CUMULATIVE","DOUBLE","NAN",KLISTEND};
  new DLibFun(lib::total,string("TOTAL"),2,totalKey);

  new DLibFun(lib::n_params,string("N_PARAMS"),1); // IDL allows one paramter
  new DLibFun(lib::keyword_set,string("KEYWORD_SET"),1);

  const string array_equalKey[]={"NO_TYPECONV",KLISTEND};
  new DLibFun(lib::array_equal,string("ARRAY_EQUAL"),2,array_equalKey);
  
  const string minKey[]={"MAX",KLISTEND};
  new DLibFun(lib::min_fun,string("MIN"),2,minKey);
  const string maxKey[]={"MIN",KLISTEND};
  new DLibFun(lib::max_fun,string("MAX"),2,maxKey);

#ifdef USE_PYTHON

  const string python_funKey[]={"ARGV","DEFAULTRETURNVALUE",KLISTEND};
  new DLibFun(lib::gdlpython_fun,string("PYTHON"),-1,python_funKey);

  const string python_proKey[]={"ARGV",KLISTEND};
  new DLibPro(lib::gdlpython_pro,string("PYTHON"),-1,python_proKey);
#endif  

  // graphics *******************************************************
  const string windowKey[]={"COLORS","FREE","PIXMAP","RETAIN","TITLE",
			    "XPOS","YPOS","XSIZE","YSIZE",KLISTEND};
  new DLibPro(lib::window,string("WINDOW"),1,windowKey);
  new DLibPro(lib::wdelete,string("WDELETE"),-1);
  new DLibPro(lib::wset,string("WSET"),1);
  const string set_plotKey[]={"COPY","INTERPOLATE",KLISTEND};
  new DLibPro(lib::set_plot,string("SET_PLOT"),1,set_plotKey);

  const string tvlctKey[]={"GET","HLS","HSV",KLISTEND};
  new DLibPro(lib::tvlct,string("TVLCT"),4,tvlctKey);

  const string deviceKey[]=
    {
      "CLOSE_FILE","FILENAME","LANDSCAPE","PORTRAIT",
      KLISTEND
    };
  new DLibPro(lib::device,string("DEVICE"),0,deviceKey);

  const string plotKey[]=
    {
     // GRAPHIC KEYWORDS
     // 0
     "BACKGROUND","CHARSIZE","CHARTHICK","CLIP",
     // 4
     "COLOR",     "DATA",    "DEVICE",   "FONT",
     // 8
     "LINESTYLE", "NOCLIP",  "NODATA",   "NOERASE", 
     // 12
     "NORMAL",    "POSITION","PSYM",     "SUBTITLE",
     // 16
     "SYMSIZE",   "T3D",     "THICK",    "TICKLEN", 
     // 20
     "TITLE",
     // 21
     "XCHARSIZE",    "XGRIDSTYLE", "XMARGIN", "XMINOR",
     // 25
     "XRANGE",       "XSTYLE",     "XTHICK",  "XTICKFORMAT",
     // 29
     "XTICKINTERVAL","XTICKLAYOUT","XTICKLEN","XTICKNAME",
     // 33
     "XTICKS",       "XTICKUNITS", "XTICKV",  "XTICK_GET",
     // 37
     "XTITLE",
     // 38
     "YCHARSIZE",    "YGRIDSTYLE", "YMARGIN", "YMINOR",
     // 42
     "YRANGE",       "YSTYLE",     "YTHICK",  "YTICKFORMAT",
     // 46
     "YTICKINTERVAL","YTICKLAYOUT","YTICKLEN","YTICKNAME",
     // 50
     "YTICKS",       "YTICKUNITS", "YTICKV",  "YTICK_GET",
     // 51
     "YTITLE",
     // 55
     "ZCHARSIZE",    "ZGRIDSTYLE", "ZMARGIN", "ZMINOR",
     // 59
     "ZRANGE",       "ZSTYLE",     "ZTHICK",  "ZTICKFORMAT",
     // 63
     "ZTICKINTERVAL","ZTICKLAYOUT","ZTICKLEN","ZTICKNAME",
     // 67
     "ZTICKS",       "ZTICKUNITS", "ZTICKV",  "ZTICK_GET",
     // 71
     "ZTITLE",
     // 72
     "ZVALUE", 

     // PLOT keywords
     // 73
     "ISOTROPIC",
     "MAX_VALUE", "MIN_VALUE","NSUM",     "POLAR",
     "XLOG",     "YLOG",     "YNOZERO", 
     KLISTEND
    };
  new DLibPro(lib::plot,string("PLOT"),2,plotKey);

  const string oplotKey[]=
    {
      // GRAPHIC KEYWORDS
      // 0
      "CLIP","COLOR", "LINESTYLE", "NOCLIP",
      // 4
      "PSYM", "SYMSIZE",  "T3D",  "THICK",
      // 8
      "MAX_VALUE", "MIN_VALUE","NSUM",  "POLAR",
      KLISTEND
    };
  new DLibPro(lib::oplot, string("OPLOT"),2,oplotKey);

  const string plotsKey[]=
    {
      // GRAPHIC KEYWORDS
      // 0
      "CLIP","COLOR", "LINESTYLE", "NOCLIP",
      // 4
      "PSYM", "SYMSIZE",  "T3D",  "THICK",
      // 8
      "DATA","DEVICE","NORMAL",
      KLISTEND
    };

  new DLibPro(lib::plots, string("PLOTS"), 3, plotsKey);
  LibInit_jmg();
  LibInit_cl();
  LibInit_mes();
}
