/***************************************************************************
                       plotting.cpp  -  GDL routines for plotting
                             -------------------
    begin                : July 22 2002
    copyright            : (C) 2002-2011 by Marc Schellens et al.
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
#include "plotting.hpp"
#include "math_utl.hpp"

#define DPI (double)(4*atan(1.0))
#define DEGTORAD DPI/180.0

namespace lib
{

  using namespace std;

  static DDouble lastTextPosX=0.0;
  static DDouble lastTextPosY=0.0;

  class xyouts_call: public plotting_routine_call
  {
    PLFLT currentBoxXmin, currentBoxXmax, currentBoxYmin, currentBoxYmax, currentBoxZmin, currentBoxZmax;
    PLFLT vpXmin, vpXmax, vpYmin, vpYmax;
    DDoubleGDL* yVal, *xVal;
    Guard<DDoubleGDL> xval_guard, yval_guard;
    DStringGDL* strVal;
    SizeT xEl, yEl, strEl;
    bool xLog, yLog, zLog;
    bool doClip, restoreClipBox;
    PLFLT savebox[4];
    bool kwWidth;
    PLFLT width;
    DLong minEl;
    DLongGDL *color;
    DFloatGDL *spacing,*orientation,*charthick,*alignement,*size;
    Guard<BaseGDL> color_guard, chartick_guard, alignement_guard, orientation_guard,size_guard;

  private:

    bool handle_args(EnvT* e)
    {
      // KEYWORDS are: CLIP(YES), COLOR(NO!), DATA(YES) , DEVICE(YES) ,
      // NORMAL(YES) , FONT(NO), ORIENTATION(YES), /NOCLIP(YES), T3D(NO), Z(NO)
      if ( nParam()==1 )
      {
        //string only...
        xVal=new DDoubleGDL(1, BaseGDL::ZERO);
        yVal=new DDoubleGDL(1, BaseGDL::ZERO);
        xval_guard.Reset(xVal); // delete upon exit
        yval_guard.Reset(yVal); // delete upon exit
        xEl=yEl=xVal->N_Elements();
        strVal=e->GetParAs<DStringGDL>(0);
        strEl=strVal->N_Elements();
      }
      else if ( nParam()==3 )
      {
        xVal=e->GetParAs< DDoubleGDL>(0);
        xEl=xVal->N_Elements();
        yVal=e->GetParAs< DDoubleGDL>(1);
        yEl=yVal->N_Elements();
        strVal=e->GetParAs<DStringGDL>(2);
        strEl=strVal->N_Elements();
      }
      else
      {
        e->Throw("Not enough parameters. Either 1 parameter or 3 "
                 "parameters valid.");
      }
      //align x y text sizes...
      minEl=(xEl<yEl)?xEl:yEl;
      minEl=(minEl<strEl)?minEl:strEl;

      return true;
    }

  private:
    void saveTextPos(GDLGStream *a, DDouble wx, DDouble wy)
    {
      a->WorldToDevice(wx, wy, lastTextPosX, lastTextPosY);
      if (GDL_DEBUG_PLSTREAM) fprintf(stderr,"saveTextPos: Saved norm: %lf %lf\n",lastTextPosX,lastTextPosY);
    }

    void getTextPos(GDLGStream *a, DDouble &wx, DDouble &wy)
    {
      a->DeviceToWorld(lastTextPosX, lastTextPosY, wx, wy);
      if (GDL_DEBUG_PLSTREAM) fprintf(stderr,"getTextPos: Got norm: %lf %lf giving %lf %lf world\n", lastTextPosX, lastTextPosY, wx, wy);
    }

    void old_body(EnvT* e, GDLGStream* actStream)
    {
      int clippingix=e->KeywordIx("CLIP");
      DFloatGDL* clipBox=NULL;

      //if string only, fill empty Xval Yval with current value:
      if ( nParam()==1 )
      {
        DDouble s,t;
        getTextPos(actStream, s, t);
        (*xVal)[0]=s;
        (*yVal)[0]=t;
      }
      // WIDTH keyword (read, write)
      static int widthIx=e->KeywordIx("WIDTH");
      kwWidth=e->KeywordPresent(widthIx);
      width=0.;

      enum
      {
        DATA=0,
        NORMAL,
        DEVICE
      } coordinateSystem=DATA;
      //check presence of DATA,DEVICE and NORMAL options
      if ( e->KeywordSet("DATA") ) coordinateSystem=DATA;
      if ( e->KeywordSet("DEVICE") ) coordinateSystem=DEVICE;
      if ( e->KeywordSet("NORMAL") ) coordinateSystem=NORMAL;
      // get_axis_type
      gdlGetAxisType("X", xLog);
      gdlGetAxisType("Y", yLog);
      gdlGetAxisType("Z", zLog);

      bool mapSet=false;
#ifdef USE_LIBPROJ4
      get_mapset(mapSet);
      if ( mapSet )
      {
        ref=map_init();
        if ( ref==NULL )
        {
          e->Throw("Projection initialization failed.");
        }
      }
#endif
      restoreClipBox=false;
      int noclipvalue=1;
      e->AssureLongScalarKWIfPresent( "NOCLIP", noclipvalue);
      doClip=(noclipvalue==0); //XYOUTS by default does not clip, even if clip is defined by CLIP= or !P.CLIP.
      clipBox=e->IfDefGetKWAs<DFloatGDL>(clippingix);
      if(doClip && clipBox!=NULL && clipBox->N_Elements()>=4 ) //clipbox exist, will be used: convert to device coords
                                                               //and save in !P.CLIP...
      {
        restoreClipBox=true; //restore later
        // save current !P.CLIP box, replace by our current clipbox in whatever coordinates, will
        // give back the !P.CLIP box at end...
        static DStructGDL* pStruct=SysVar::P();
        static unsigned clipTag=pStruct->Desc()->TagIndex("CLIP"); //must be in device coordinates
        static PLFLT tempbox[4];
        for ( int i=0; i<4; ++i ) savebox[i]=(*static_cast<DLongGDL*>(pStruct->GetTag(clipTag, 0)))[i];
        if ( coordinateSystem==DEVICE )
        {
          for ( int i=0; i<4; ++i ) tempbox[i]=(*clipBox)[i];
        }
        else if ( coordinateSystem==DATA )
        {
          //handle log: if existing box is already in log, use log of clipbox values.
          PLFLT worldbox[4];
          for ( int i=0; i<4; ++i ) worldbox[i]=(*clipBox)[i];
          if (xLog) {worldbox[0]=log10(worldbox[0]); worldbox[2]=log10(worldbox[2]);}
          if (yLog) {worldbox[1]=log10(worldbox[1]); worldbox[3]=log10(worldbox[3]);}
          bool okClipBox=true;
          for ( int i=0; i<4; ++i )
          {
            if (!(worldbox[i]==worldbox[i])) //NaN
            {
              okClipBox=false;restoreClipBox=false;doClip=false;
            }
          }
          if (okClipBox)
          {
            actStream->WorldToDevice(worldbox[0], worldbox[1], tempbox[0], tempbox[1]);
            actStream->WorldToDevice(worldbox[2], worldbox[3], tempbox[2], tempbox[3]);
          }
        }
        else
        {
          actStream->NormedDeviceToDevice((*clipBox)[0],(*clipBox)[1], tempbox[0], tempbox[1]);
          actStream->NormedDeviceToDevice((*clipBox)[2],(*clipBox)[3], tempbox[2], tempbox[3]);
        }
        //place in !P.CLIP
        for ( int i=0; i<4; ++i ) (*static_cast<DLongGDL*>(pStruct->GetTag(clipTag, 0)))[i]=tempbox[i];
      }

      PLFLT wun, wdeux, wtrois, wquatre;
      if ( coordinateSystem==DATA) //with PLOTS, we can plot *outside* the box(e)s in DATA coordinates.
                                   // convert to device coords in this case
      {
        actStream->pageWorldCoordinates(wun, wdeux, wtrois, wquatre);
      }

      actStream->OnePageSaveLayout(); // one page
      actStream->vpor(0, 1, 0, 1); //set full viewport

      if ( coordinateSystem==DEVICE )
      {
        actStream->wind(0.0, actStream->xPageSize(), 0.0, actStream->yPageSize());
        xLog=false;
        yLog=false;
      }
      else if ( coordinateSystem==NORMAL )
      {
        actStream->wind(0.0, 1.0, 0.0, 1.0);
        xLog=false;
        yLog=false;
      }
      else //with XYOUTS, we can plot *outside* the box(e)s in DATA coordinates.
      {
        actStream->wind(wun, wdeux, wtrois, wquatre);
      }

      PLFLT x,y,aspectw,aspectd;
      aspectw=actStream->boxAspectWorld();
      aspectd=actStream->boxAspectDevice();

        int colorIx=e->KeywordIx ( "COLOR" ); bool docolor=false;
        int charthickIx=e->KeywordIx ( "CHARTHICK" ); bool docharthick=false;
        int charsizeIx=e->KeywordIx ( "CHARSIZE" ); bool docharsize=false;
        if ( e->GetKW ( colorIx )!=NULL )
        {
          color=e->GetKWAs<DLongGDL>( colorIx ); docolor=true;
        }
        if ( e->GetKW ( charthickIx )!=NULL )
        {
          charthick=e->GetKWAs<DFloatGDL>( charthickIx ); docharthick=true;
        }
        if ( e->GetKW ( charsizeIx )!=NULL )
        {
          size=e->GetKWAs<DFloatGDL>( charsizeIx ); docharsize=true;
        }
        else  //for security in future conditional evaluation...
        {
          size=new DFloatGDL  ( dimension (1), BaseGDL::ZERO );
          size_guard.Init ( size);
          (*size)[0]=1.0;
        }
        int orientationIx=e->KeywordIx ( "ORIENTATION" );
        if ( e->GetKW ( orientationIx )!=NULL )
        {
          orientation=e->GetKWAs<DFloatGDL>( orientationIx ); 
        }
        else
        {
          orientation=new DFloatGDL  ( dimension (1), BaseGDL::ZERO );
          orientation_guard.Init ( orientation);
          (*orientation)[0]=0;
        }
        int alignIx=e->KeywordIx ( "ALIGNMENT" );
        if ( e->GetKW ( alignIx )!=NULL )
        {
          alignement=e->GetKWAs<DFloatGDL>( alignIx );
        }
        else
        {
          alignement=new DFloatGDL  ( dimension (1), BaseGDL::ZERO );
          alignement_guard.Init (alignement);
          (*alignement)[0]=0;
        }


      // make all clipping computations BEFORE setting graphic properties (color, size)
      bool stopClip=false;
      if ( doClip )  if ( startClipping(e, actStream, true)==TRUE ) stopClip=true;

      // *** start drawing by defalut values
      gdlSetGraphicsForegroundColorFromKw(e, actStream);
      gdlSetPlotCharthick(e, actStream);
      gdlSetPlotCharsize(e, actStream, true); //accept SIZE kw!

      for ( int i=0; i<minEl; ++i )
      {
        x=static_cast<PLFLT>((*xVal)[i]);
        y=static_cast<PLFLT>((*yVal)[i]);

#ifdef USE_LIBPROJ4
        if ( mapSet&& coordinateSystem==DATA )
        {
          LPTYPE idata;
          XYTYPE odata;
          idata.lam=x * DEG_TO_RAD;
          idata.phi=y * DEG_TO_RAD;
          odata=PJ_FWD(idata, ref);
          x=odata.x;
          y=odata.y;
        }
#endif

        if( xLog ) x=log10(x);
        if( yLog ) y=log10(y);

        if ( !isfinite(x)|| !isfinite(y) ) continue; //no plot
        if ( docharsize && ( *size )[i%size->N_Elements ( )] < 0) continue; //no plot either

        //plot!
        if (docharsize) actStream->sizeChar(( *size )[i%size->N_Elements ( )]);
        if (docolor) actStream->Color ( ( *color )[i%color->N_Elements ( )], true, 2 );
        if (docharthick) actStream->wid ( ( *charthick )[i%charthick->N_Elements ( )]);
        PLFLT ori=(( *orientation )[i%orientation->N_Elements ( )]) * DEGTORAD;
        PLFLT cosOri=cos(ori);
        PLFLT sinOri=sin(ori);
        PLFLT align=( *alignement )[i%alignement->N_Elements ( )];
        PLFLT dispx,dispy, chsize, dx, dy;
        // displacement due to offset (reference in IDL is baseline,
        // in plplot it's the half-height) is best computed in device coords
        chsize=actStream->dCharHeight()*0.5;
        actStream->WorldToDevice(x, y, dx, dy); //nx ny in relative coords
        actStream->DeviceToWorld(dx-chsize*sinOri,dy+chsize*cosOri,dispx,dispy);
        string out=(*strVal)[i];
        actStream->ptex(dispx, dispy, cosOri, sinOri*aspectw/aspectd, align, out.c_str());
        if ( i==minEl-1 )
        {
          width=(out.length())*actStream->nCharLength(); //normalized for /WIDTH= option
          PLFLT dWidth=(out.length())*actStream->dCharLength(); //device for internal repositioning
          //save last position
          actStream->DeviceToWorld(dx+(1.0-align)*dWidth*cosOri,dy+(1.0-align)*dWidth*sinOri,dispx,dispy);
          actStream->WorldToDevice(dispx, dispy, lastTextPosX, lastTextPosY);
//          width/=actStream->xPageSize(); //normed value
//          lastTextPosX=nx+(1.0-alignment)*width*cos(orientation*DEGTORAD); //normalized
//          lastTextPosY=ny+(1.0-alignment)*width*sin(orientation*DEGTORAD);
        }
      }
      if (stopClip) stopClipping(actStream);

      if ( kwWidth )
      {
        // width is in "normalized coordinates"
        e->SetKW(widthIx, new DFloatGDL(width));
      }
    } 

  private:

    void call_plplot(EnvT* e, GDLGStream* actStream) // {{{
    {
    } 

  private:

    virtual void post_call(EnvT* e, GDLGStream* actStream) // {{{
    {
      actStream->RestoreLayout();
      if (restoreClipBox)
      {
        static DStructGDL* pStruct=SysVar::P();
        static unsigned clipTag=pStruct->Desc()->TagIndex("CLIP"); //must be in device coordinates
        for ( int i=0; i<4; ++i ) (*static_cast<DLongGDL*>(pStruct->GetTag(clipTag, 0)))[i]=savebox[i];
      }
      actStream->sizeChar(1.0);
    }

  };

  void xyouts(EnvT* e)
  {
    xyouts_call xyouts;
    xyouts.call(e, 1);
  }

} // namespace
