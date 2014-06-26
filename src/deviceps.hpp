/* *************************************************************************
                          deviceps.hpp  -  postscript device
                             -------------------
    begin                : July 22 2002
    copyright            : (C) 2002 by Marc Schellens
    email                : m_schellens@users.sf.net
 ***************************************************************************/

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef DEVICEPS_HPP_
#define DEVICEPS_HPP_

#include "gdlpsstream.hpp"
#include "plotting.hpp" // get_axis_crange for TV()
#include "initsysvar.hpp"
#include <gsl/gsl_const_mksa.h> // GSL_CONST_MKSA_INCH

#include "objects.hpp"


#  ifdef USE_PSLIB
#    include <stdio.h> // tmpnam
#    if !defined(_WIN32) || defined(__CYGWIN__)
#        include <sys/utsname.h> // uname
#    endif
#    include <libps/pslib.h>
#  endif

#  ifdef HAVE_OLDPLPLOT
#    define SETOPT SetOpt
#  else
#    define SETOPT setopt
#  endif

#ifdef _MSC_VER
#define cm2in (.01 / GSL_CONST_MKSA_INCH) // This is not good, but works
#define dpi 72.0 //in dpi;
#else
  static const float cm2in = .01 / GSL_CONST_MKSA_INCH;
  static const PLFLT dpi = 72.0 ; //in dpi;
#endif

class DevicePS: public GraphicsDevice
{
  std::string      fileName;
  GDLPSStream*     actStream;
  float            XPageSize;
  float            YPageSize;
  float            XOffset;
  float            YOffset;
  int              color;
  int              decomposed; // false -> use color table
  bool	           orient_portrait; 
  bool             encapsulated;
  float	           scale;

  GDLStream  *psUnit;

  void InitStream()
  {
    delete actStream;

    DLongGDL* pMulti = SysVar::GetPMulti();
    DLong nx = (*pMulti)[ 1];
    DLong ny = (*pMulti)[ 2];

    if( nx <= 0) nx = 1;
    if( ny <= 0) ny = 1;
    actStream = new GDLPSStream( nx, ny, (int)SysVar::GetPFont(), encapsulated, color);

    actStream->sfnam( fileName.c_str());

    // trying to solve bug report 3611898
    // AC 29-Avril-2013: the way I found to link GDLPSStream* and GDLStream*
    DLong lun=GetLUN();
    psUnit = &fileUnits[ lun-1];
    psUnit->Open(fileName,fstream::out,false,false,false,
		 defaultStreamWidth,false,false);
    (*static_cast<DLongGDL*>( dStruct->GetTag(dStruct->Desc()->TagIndex("UNIT"))))[0]=lun;

    // zeroing offsets (xleng and yleng are the default ones but they need to be specified 
    // for the offsets to be taken into account by spage(), works with plplot >= 5.9.9)
    actStream->spage(dpi, dpi, 540, 720, 0, 0); //plplot default: portrait!

    // as setting the offsets and sizes with plPlot is (extremely) tricky, and some of these setting
    // are hardcoded into plplot (like EPS header, and offsets in older versions of plplot)
    // here we only specify the aspect ratio - size an offset are handled by pslib when device,/close is called

    // patch 3611949 by Joanna, 29 Avril 2013
    PLFLT pageRatio=XPageSize/YPageSize;
    std::string as = i2s( pageRatio);
    actStream->SETOPT( "a", as.c_str());
    
    // plot orientation
    //std::cout  << "orientation : " << orient_portrait<< std::endl;
    
    actStream->sdiori(orient_portrait ? 1 : 2);
    
    // no pause on destruction
    actStream->spause( false);

    // extended fonts
    actStream->fontld( 1);
    
    // avoid to set color map 0 -- makes plplot very slow (?)
    PLINT r[ctSize], g[ctSize], b[ctSize];
    actCT.Get( r, g, b);
//    actStream->scmap0( r, g, b, ctSize);
    actStream->scmap1( r, g, b, ctSize);
    // default: black+white (IDL behaviour)

    if (color == 0)
    {
      actStream->SETOPT( "drvopt","text=0,color=0");
    }
    else
    {
      actStream->SETOPT( "drvopt","text=0,color=1"); //need to pass all options with the same 'setopt' command.
    }
    actStream->scolbg(255,255,255); // start with a white background

    actStream->Init();
    
    // need to be called initially. permit to fix things
    actStream->ssub(1,1);
    actStream->adv(0);
    // load font
    actStream->font( 1);
    actStream->vpor(0,1,0,1);
    actStream->wind(0,1,0,1);
    actStream->DefaultCharSize();
   //in case these are not initalized, here is a good place to do it.
    if (actStream->updatePageInfo()==true)
    {
        actStream->GetPlplotDefaultCharSize(); //initializes everything in fact..

    }
  }
    
private:
  void pslibHacks()
  {
#  ifndef USE_PSLIB
    Warning("Warning: pslib support is mandatory for the PostScript driver to handle the following");
    Warning("         keywords:  [X,Y]OFFSET, SCALE_FACTOR, ENCAPSULATED");
#  else
    PSDoc *ps = PS_new(); 
    GDLGuard<PSDoc> psGuard( ps, PS_delete);
    
    if (ps == NULL)
    {
      Warning("Warning: pslib failed to allocate memory.");
      return;
    }
    
    FILE *fp = tmpfile(); // this creates a file which should be deleted automaticaly when it is closed
    FILEGuard fpGuard( fp, fclose);
    
    if (fp == NULL) 
    {
      Warning("Warning: failed to create temporary PostScript file.");
//       PS_delete(ps);
      return;
    }
    if (PS_open_fp(ps, fp) == -1) 
    { 
      Warning("Warning: pslib failed to open a new PostScript file.");
      goto cleanup;
    }
    
    PS_set_parameter(ps, "imagereuse", "false");
    PS_set_info(ps, "Title", "Graphics produced by GDL"); 
    PS_set_info(ps, "Orientation", orient_portrait ? "Portrait" : "Landscape"); 
    {
      string tmp;
#if !defined(_WIN32) || defined(__CYGWIN__)
      struct utsname uts;
      uname(&uts);
      tmp = "GDL Version " + string(VERSION) + ", " + string(uts.sysname) + " " + string(uts.machine);
      PS_set_info(ps, "Creator", tmp.c_str()); 
      char* login = getlogin();
      if (login == NULL) Warning("Warning: getlogin() failed!");
      tmp = (login == NULL ? "?" : login) + string("@") + uts.nodename;
      PS_set_info(ps, "Author", tmp.c_str());
#else
      tmp = "GDL Version " + string(VERSION) + ", Microsoft Windows x32";
      PS_set_info(ps, "Creator", tmp.c_str());

      TCHAR username[257];
      char cusername[257];
      DWORD username_len = 257;
      GetUserName(username, &username_len);

      WideCharToMultiByte(CP_ACP, 0, username, username_len, cusername, username_len, NULL, NULL);

      PS_set_info(ps, "Author", cusername);
#endif
    }
    //bug: PSLIB does not return the correct boundingbox, it forgets offx and offy. Try to get it
    //back (using pslib own code!)!
        char *bb;
        FILE *feps;
        char buffer[1024]; //largely sufficient
        int nbytes;
        feps=fopen(fileName.c_str(), "r");
        nbytes=fread(buffer,sizeof(char),1023,feps);
        fclose(feps);
        buffer[1023]=0;
	bb = strstr(buffer, "%%BoundingBox:");
        float offx, offy, width, height;
	if(bb) {
            bb += 15;
            sscanf(bb, "%f %f %f %f", &offx, &offy, &width, &height);
	} else {
            offx=0;
            offy=0;
            width=500;
            height=500; //silly values, will be replaced afterwards hopefully.
        }

    // TODO
    //psfont = PS_findfont(ps, "Helvetica", "", 0); 
    //PS_setfont(ps, psfont, 8.0); 

      char bbstr [20], offstr [20];
      int bbXSize, bbYSize;
    {

      int bbXoff = XOffset*cm2in*dpi;
      int bbYoff = YOffset*cm2in*dpi;
      bbXSize = orient_portrait ? bbXoff + XPageSize*cm2in*dpi*scale : bbXoff + YPageSize*cm2in*dpi*scale;
      bbYSize = orient_portrait ? bbYoff + YPageSize*cm2in*dpi*scale : bbYoff + XPageSize*cm2in*dpi*scale;
      sprintf(bbstr,"%i %i %i %i",bbXoff,bbYoff,bbXSize,bbYSize);
      sprintf(offstr,"%i %i",bbXoff,bbYoff);
    
      PS_set_info(ps,"BoundingBox",bbstr);
      PS_begin_page(ps, bbXSize, bbYSize);
      {
        int psimage = PS_open_image_file(ps, "eps", fileName.c_str(), NULL, 0);
        if (psimage == 0)
        {
          Warning("Warning: pslib failed to load plPlot output file.");
          goto cleanup;
        }
	
        float scl = 0.98*min((bbXSize-bbXoff) / (width-offx), (bbYSize-bbYoff) / (height-offy) );
	int margx = ((bbXSize-bbXoff) - scl*(width-offx))/2;
	int margy = ((bbYSize-bbYoff) - scl*(height-offy))/2;
        PS_place_image(ps, psimage, 
		       bbXoff-offx*scl + margx,
		       bbYoff-offy*scl + margy,
		       scl
        );
        PS_close_image(ps, psimage); 
      }
      PS_end_page(ps);
      PS_close(ps);
    }
    
    // Replace PageBoundingBox and CropBox and write contents to fileName
    {
      rewind(fp);
      FILE *fp_plplot = fopen(fileName.c_str(), "w");
      FILEGuard fp_plplotGuard( fp_plplot, fclose);
      if (fp_plplot == NULL)
      {
        Warning("Warning: failed to open plPlot-generated file");
        goto cleanup;
      }

      // When multiple pages are supported, PageBoundingBox and the cropbox
      // will appear more than once. Then this section will need to be redone.

      // Edit: change the two 0's after the PageBoundingBox
      string pbstr=string("%%PageBoundingBox: ")+offstr;
      // edits will be in the first 12288 bytes; add the length of offstr-3
      const size_t buflen=12288 + pbstr.length()-22;
      //const size_t buflen=4096;
      char buff[buflen];

      //do the first read:
      size_t cnt = fread(&buff, 1, 12288, fp);
      string sbuff;
      sbuff = string(buff);

      // find the PageBoundingBox statement
      size_t pos = sbuff.find("%%PageBoundingBox: 0 0");
      if (pos != string::npos) {
	sbuff.replace(pos,22,pbstr); // will change the size of sbuff by offstr-3
	cnt = cnt + pbstr.length()-22;
      }

      // PSlib outputs pdfmarks which resize the PDF to the size of the boundingbox
      // this is nice, but not IDL behaviour (and anyway, the two 0's are wrong)
      char mychar[60];
      sprintf(mychar,"[ /CropBox [0 0 %i.00 %i.00] /PAGE pdfmark",bbXSize,bbYSize);
      string pdfstr=string(mychar); 
      string pdfrepl(pdfstr.length(),' ');
      pos = sbuff.find(pdfstr);
      if (pos != string::npos) {sbuff.replace(pos,pdfstr.length(),pdfrepl);} // will not change size of sbuff

      // write the first buflen to file
      strcpy(buff,sbuff.c_str());
      if (fwrite(&buff, 1, buflen, fp_plplot) < buflen)
        {
          Warning("Warning: failed to overwrite the plPlot-generated file with pslib output");
        }

      // read the rest of fp and write to file
      while (true)
      {
       cnt = fread(&buff, 1, buflen, fp);
         if (!cnt) break;
        if (fwrite(&buff, 1, cnt, fp_plplot) < cnt)
        {
          Warning("Warning: failed to overwrite the plPlot-generated file with pslib output");
        }
      }
//       fclose(fp_plplot);
    }

    cleanup:
//    PS_delete(ps);
//     fclose(fp); // this deletes the temporary file as well
    // PSlib changes locale - bug no. 3428043
#    ifdef HAVE_LOCALE_H
    setlocale(LC_ALL, "C");
#    endif
#  endif
  }

private:
  void epsHacks()
  {
    // using namespace std;
    //PLPLOT outputs a strange boundingbox; this hack directly edits the eps file.  
    //if the plplot bug ever gets fixed, this hack won't be needed.
    char *bb;
    FILE *feps;
    const size_t buflen=2048;//largely sufficient
    char buffer[buflen]; 
    int cnt;
    ifstream myfile (fileName.c_str());
    feps=fopen(fileName.c_str(), "r");
    cnt=fread(buffer,sizeof(char),buflen,feps);

    //read original boundingbox
    bb = strstr(buffer, "%%BoundingBox:");
    int offx, offy, width, height;
    bb += 15;
    sscanf(bb, "%i %i %i %i", &offx, &offy, &width, &height);
    float hsize = XPageSize*cm2in*dpi*scale;
	float vsize = YPageSize*cm2in*dpi*scale;
    float newwidth = (width - offx), newheight = (height - offy);
    float hscale = (orient_portrait ? hsize : vsize)/newwidth/5.0;
    float vscale = (orient_portrait ? vsize : hsize)/newheight/5.0;
    hscale = min(hscale,vscale)*0.98;
    vscale = hscale;
    float hoff = -5.*offx*hscale + ((orient_portrait ? hsize : vsize) - 5.0*hscale*newwidth)*0.5;
    float voff = -5.*offy*vscale + ((orient_portrait ? vsize : hsize) - 5.0*vscale*newheight)*0.5;

    //replace with a more sensible boundingbox
    string sbuff = string(buffer);
    stringstream searchstr,replstr;
    searchstr << "BoundingBox: " << offx << " " << offy << " " << width << " " << height;
    replstr << "BoundingBox: 0 0 " << floor((orient_portrait ? hsize : vsize)+0.5) << " " << floor((orient_portrait ? vsize : hsize)+0.5);
    size_t pos = sbuff.find(searchstr.str());
    int extralen;
    if (pos != string::npos) {
      sbuff.replace(pos,searchstr.str().length(),replstr.str()); 
      extralen = replstr.str().length()-searchstr.str().length();
    }

    //replace values of hscale, vscale
    searchstr.str("");
    searchstr << "{hs 3600 div} def" << endl << "/YScale" << endl << "   {vs 2700 div} def";
    replstr.str("");
    replstr << hscale << " def" << endl << "/YScale" << endl << "   " << vscale << " def";
    pos = sbuff.find(searchstr.str());
    if (pos != string::npos) {
      sbuff.replace(pos,searchstr.str().length(),replstr.str()); 
      extralen = extralen + replstr.str().length()-searchstr.str().length();
    }

    //replace the values of hoffset and voffset
    searchstr.str("");
    searchstr << "0 @hoffset" << endl << "0 @voffset";
    replstr.str("");
    replstr << floor(hoff+0.5) << " " << "@hoffset" << endl << floor(voff+0.5) << " " << "@voffset";
    pos = sbuff.find(searchstr.str());
    if (pos != string::npos) {
      sbuff.replace(pos,searchstr.str().length(),replstr.str()); 
      extralen = extralen + replstr.str().length()-searchstr.str().length();
    }

    //add landscape
    if (!orient_portrait) {
    searchstr.str("%%Page: 1 1");
    replstr.str("");
    replstr << "%%Page: 1 1" << endl << "%%PageOrientation: Landscape" << endl;
    pos = sbuff.find(searchstr.str());
    if (pos != string::npos) {
      sbuff.replace(pos,searchstr.str().length(),replstr.str()); 
      extralen = extralen + replstr.str().length()-searchstr.str().length();
    }
    }

    //open temp file
    FILE *fp = tmpfile(); // this creates a file which should be deleted automaticaly when it is closed
    FILEGuard fpGuard( fp, fclose);
    if (fp == NULL) { 
      Warning("Warning: failed to create temporary PostScript file."); 
      return;
    }

    // write the first buflen to temp file
    char *buffer2 = new char[buflen + extralen];
    strcpy(buffer2,sbuff.c_str());
    fwrite(&buffer2, 1, buflen+extralen, fp); 
	delete buffer2;

    // read the rest of feps and write to temp file
    while (true)
      {
    	cnt = fread(&buffer, 1, buflen, feps);
    	if (!cnt) break;
        if (fwrite(&buffer, 1, cnt, fp) < cnt)
    	  {
    	    Warning("Warning: failed to write to temporary file");
    	  }
      }
    fclose(feps);

    // copy temp file to fileName
    rewind(fp);
    FILE *fp_plplot = fopen(fileName.c_str(), "w");
    FILEGuard fp_plplotGuard( fp_plplot, fclose);
    if (fp_plplot == NULL) {
      Warning("Warning: failed to open plPlot-generated file");
      return;
    }
    while (true)
      {
    	cnt = fread(&buffer, 1, buflen, fp);
    	if (!cnt) break;
        if (fwrite(&buffer, 1, cnt, fp_plplot) < cnt)
    	  {
    	    Warning("Warning: failed to overwrite the plPlot-generated file with pslib output");
    	  }
      }

  }

public:
  DevicePS(): GraphicsDevice(), fileName( "gdl.ps"), actStream( NULL),
    XPageSize(17.78), YPageSize(12.7), XOffset(0.0),YOffset(0.0),
    color(0), decomposed( 0), encapsulated(false), scale(1.)
  {
    name = "PS";

    DLongGDL origin( dimension( 2));
    DLongGDL zoom( dimension( 2));
    zoom[0] = 1;
    zoom[1] = 1;

    dStruct = new DStructGDL( "!DEVICE");
    dStruct->InitTag("NAME",       DStringGDL( name)); 
    dStruct->InitTag("X_SIZE",     DLongGDL( XPageSize*scale*1000)); //29700/1000=29.7 cm
    dStruct->InitTag("Y_SIZE",     DLongGDL( YPageSize*scale*1000));
    dStruct->InitTag("X_VSIZE",    DLongGDL( XPageSize*scale*1000));
    dStruct->InitTag("Y_VSIZE",    DLongGDL( YPageSize*scale*1000));
    dStruct->InitTag("X_CH_SIZE",  DLongGDL( 360));
    dStruct->InitTag("Y_CH_SIZE",  DLongGDL( 360));
    dStruct->InitTag("X_PX_CM",    DFloatGDL( 1000.0)); //1000 pix/cm
    dStruct->InitTag("Y_PX_CM",    DFloatGDL( 1000.0)); 
    dStruct->InitTag("N_COLORS",   DLongGDL( 256)); 
    dStruct->InitTag("TABLE_SIZE", DLongGDL( 256)); 
    dStruct->InitTag("FILL_DIST",  DLongGDL( 1));
    dStruct->InitTag("WINDOW",     DLongGDL( -1)); 
    dStruct->InitTag("UNIT",       DLongGDL( 0)); 
    dStruct->InitTag("FLAGS",      DLongGDL( 266807)); 
    dStruct->InitTag("ORIGIN",     origin); 
    dStruct->InitTag("ZOOM",       zoom);

    SetPortrait();

#  ifdef USE_PSLIB
    PS_boot();
    // PSlib changes locale - bug no. 3428043
#    ifdef HAVE_LOCALE_H
    setlocale(LC_ALL, "C");
#    endif
#  endif
  }
  
  ~DevicePS()
  {
    delete actStream;
#  ifdef USE_PSLIB
    PS_shutdown();
    // PSlib changes locale - bug no. 3428043
#    ifdef HAVE_LOCALE_H
    setlocale(LC_ALL, "C");
#    endif
#  endif
  }

  GDLGStream* GetStream( bool open=true)
  {
    if( actStream == NULL) 
      {
	if( !open) return NULL;
	InitStream();
      }
    return actStream;
  }

  bool SetFileName( const std::string& f)
  {
    fileName = f;
    return true;
  }

  bool CloseFile()
  {
    // trying to solve bug report 3611898
    // this is needed to decrement Lun number ...
    (*static_cast<DLongGDL*>( dStruct->GetTag(dStruct->Desc()->TagIndex("UNIT"))))[0]=0;
    if (actStream != NULL)
    {
      psUnit->Close();
      psUnit->Free();
      psUnit=NULL;

      delete actStream;
      actStream = NULL;
      if (!encapsulated) pslibHacks(); else epsHacks(); // needs to be called after the plPlot-generated file is closed
    }
    return true;
  }

  bool SetXOffset( const float xo) // xo [cm]
  {
    XOffset=xo;
    return true;
  }

  bool SetYOffset( const float yo) // yo [cm]
  {
    YOffset=yo;
    return true;
  }

  bool SetXPageSize( const float xs) // xs [cm]
  {
    XPageSize=xs;
    (*static_cast<DLongGDL*>(dStruct->GetTag(dStruct->Desc()->TagIndex("X_SIZE"))))[0] 
      = DLong(floor(0.5+
        xs * (*static_cast<DFloatGDL*>(dStruct->GetTag(dStruct->Desc()->TagIndex("X_PX_CM"))))[0]
      ));
   (*static_cast<DLongGDL*>(dStruct->GetTag(dStruct->Desc()->TagIndex("X_VSIZE"))))[0]
      = DLong(floor(0.5+
        xs * (*static_cast<DFloatGDL*>(dStruct->GetTag(dStruct->Desc()->TagIndex("X_PX_CM"))))[0]
      ));
    return true;
  }

  bool SetYPageSize( const float ys) // ys [cm]
  {
    YPageSize=ys;
    (*static_cast<DLongGDL*>(dStruct->GetTag(dStruct->Desc()->TagIndex("Y_SIZE"))))[0] 
      = DLong(floor(0.5+
        ys * (*static_cast<DFloatGDL*>(dStruct->GetTag(dStruct->Desc()->TagIndex("Y_PX_CM"))))[0]
      ));
    (*static_cast<DLongGDL*>(dStruct->GetTag(dStruct->Desc()->TagIndex("Y_VSIZE"))))[0]
      = DLong(floor(0.5+
        ys * (*static_cast<DFloatGDL*>(dStruct->GetTag(dStruct->Desc()->TagIndex("Y_PX_CM"))))[0]
      ));
    return true;
  }

  bool SetColor(const long hascolor)
  {
    if (hascolor==1) color=1; else color=0;
    return true;
  }

  bool SetPortrait()
  {
    orient_portrait = true;
//    XPageSize = 7 * 100. * GSL_CONST_MKSA_INCH;
//    YPageSize = 5 * 100. * GSL_CONST_MKSA_INCH;
//    XOffset = .75 * 100. * GSL_CONST_MKSA_INCH;
//    YOffset = 3 * 100. * GSL_CONST_MKSA_INCH; // TODO: this is different from IDL docs
    return true;
  }

  bool SetLandscape()
  {
    orient_portrait = false;
//    XPageSize = 10 * 100. * GSL_CONST_MKSA_INCH;
//    YPageSize = 7 * 100. * GSL_CONST_MKSA_INCH;
//    XOffset = .5 * 100. * GSL_CONST_MKSA_INCH;
//    YOffset = .75 * 100. * GSL_CONST_MKSA_INCH;
    return true;
  }

  bool SetScale(float value)
  {
    scale = value;
    return true;
  }

  bool SetEncapsulated(bool val)
  {
    encapsulated = val;
    return true;
  }

  bool Decomposed( bool value)           
  {   
    decomposed = value;
    return true;
  }

  DLong GetDecomposed()        
  {
    return decomposed;  
  }

  // TODO: SA: this TV() should be merged with TV() in DeviceX and DeviceZ!
  // TODO: SA: just a draft - a lot more needs to be done...
  void TV( EnvT* e)
  {
    SizeT nParam=e->NParam( 1); 

    GDLGStream* actStream = GetStream();

    // TODO: use it if XSIZE and YSIZE is not specified!
    //DLong xsize = (*static_cast<DLongGDL*>( dStruct->GetTag( xSTag, 0)))[0];
    //DLong ysize = (*static_cast<DLongGDL*>( dStruct->GetTag( ySTag, 0)))[0];

    DLong pos=0; // TODO: handle it!
    DDouble xmin, ymin;
    {
      DDouble null;
      lib::gdlGetCurrentAxisRange("X", xmin, null);
      lib::gdlGetCurrentAxisRange("Y", ymin, null);
    }
    if (nParam == 2) {
      e->AssureLongScalarPar( 1, pos);
    } else if (nParam >= 3) {
      if (e->KeywordSet("NORMAL")) 
      {
        e->Throw("NORMAL keyword not supported yet");
	//e->AssureDoubleScalarPar( 1, xmin);
	//e->AssureDoubleScalarPar( 2, ymin);
	//xLL = (DLong) rint(xLLf * xsize);
	//yLL = (DLong) rint(yLLf * ysize);
      } 
      else if (e->KeywordSet("DEVICE")) 
      {
        e->Throw("DEVICE keyword not supported yet");
      }
      else // aka DATA
      {
	e->AssureDoubleScalarPar( 1, xmin);
	e->AssureDoubleScalarPar( 2, ymin);
      }
    }

    DByteGDL* p0B = e->GetParAs<DByteGDL>( 0);
    SizeT rank = p0B->Rank();

    int width, height;
    DLong tru=0;
    e->AssureLongScalarKWIfPresent( "TRUE", tru);
    if (rank == 2) 
      {
	if (tru != 0)
	  e->Throw( "Array must have 3 dimensions: "+
		    e->GetParString(0));
	width  = p0B->Dim(0);
	height = p0B->Dim(1);
      } 
    else if( rank == 3) 
      {
	if (tru == 1) {
	  width = p0B->Dim(1);
	  height = p0B->Dim(2);
	} else if (tru == 2) {
	  width = p0B->Dim(0);
	  height = p0B->Dim(2);
	} else if (tru == 3) {
	  width = p0B->Dim(0);
	  height = p0B->Dim(1);
	} else {
	  e->Throw( "TRUE must be between 1 and 3");
	}
      } else {
	e->Throw( "Image array must have rank 2 or 3");
      }
    if (tru != 0) e->Throw("Decomposed images not supported yet with PostScript + TV() (FIXME)"); // TODO!

    /* TODO...
    if( width + xLL > xsize || height + yLL > ysize)
      e->Throw( "Value of image coordinates is out of allowed range.");
    */

    class grid2d {
      public: PLFLT** data;
      private: GDLGStream *pls;
      private: int w, h;
      public: grid2d(GDLGStream *actStream, int w, int h) 
        : pls(actStream), w(w), h(h) { pls->Alloc2dGrid(&data, w, h); }
      public: ~grid2d() { pls->Free2dGrid(data, w, h); }
    } idata(actStream, width, height);
    for (int x=0; x < width; ++x)
      for (int y=0; y < height; ++y)
        idata.data[x][y] = (*p0B)[x + y * width]; 

    PLFLT xmax, ymax;
    DDouble xsize=-1.0, ysize=-1.0;
    bool sizeXNotSet=true;
    bool sizeYNotSet=true;

    if (e->KeywordSet("XSIZE")) 
    {
      e->AssureDoubleScalarKW("XSIZE", xsize);
      sizeXNotSet=false;
    }
    else 
    {
      xsize = (*static_cast<DLongGDL*>( dStruct->GetTag( xSTag, 0)))[0] ; // wrong for PS, see GDLGStream::GetGeometry;
      xsize /= 1000;
    }//e->Throw("Specification of XSIZE is mandatory for PostScript/TV() (FIXME!)"); // TODO!
    if (e->KeywordSet("YSIZE")) 
    {
      e->AssureDoubleScalarKW("YSIZE", ysize);
      sizeYNotSet=false;
    }
    else
    {
      ysize = (*static_cast<DLongGDL*>( dStruct->GetTag( ySTag, 0)))[0]; // wrong for PS, see GDLGStream::GetGeometry;
      ysize /= 1000;
    } //e->Throw("Specification of YSIZE is mandatory for PostScript/TV() (FIXME!)"); // TODO!
    if (sizeXNotSet && sizeYNotSet ) { //scale while preserving ratio
        DDouble ratiox=xsize/width; 
        DDouble ratioy=ysize/height;
        if (ratiox < ratioy ) ysize=height*ratiox; else xsize=width*ratioy;
        cout<<xsize<<" "<<ysize<<endl;
    } else if (sizeXNotSet) {
        xsize=(double)width/(double)height*ysize;
    } else  if (sizeYNotSet ) { //scale while preserving ratio
        ysize=(double)height/(double)width*xsize;
    } 
    xmax = xmin + xsize;
    ymax = ymin + ysize;

    // TODO: map projection (via the last two arguments - same as was done in CONTOUR e.g.)
    bool mapSet = false;
#ifdef USE_LIBPROJ4
    //get_mapset(mapSet);
#endif
    if (mapSet) e->Throw("PostScript + TV() + mapping combination not available yet (FIXME!)");
    //fprintf(stderr,"%d %d %f %f %f %f (%f %f)\n",width, height, xmin, xmax, ymin, ymax,actStream->xPageSize(),actStream->yPageSize());
    actStream->imagefr(idata.data, width, height, xmin, xmax, ymin, ymax, 0., 255., 0., 255., NULL, NULL); 
  }

};

#endif
