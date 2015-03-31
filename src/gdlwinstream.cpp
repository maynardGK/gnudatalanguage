/* *************************************************************************
						  gdlwinstream.cpp  -  graphic stream M$ windows
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

#include "includefirst.hpp"

#include <iostream>

#include "graphicsdevice.hpp"
using namespace std;
#include "gdlwinstream.hpp"
#include "devicewin.hpp"
#ifndef PLESC_TELLME
#define PLESC_TELLME  41
#endif
void GDLWINStream::Init()
{
	this->plstream::init();
	plgpls(&pls);
	wingcc_Dev* dev = (wingcc_Dev *)pls->dev;
	dev->waiting = 1;

	UnsetFocus();
}

void GDLWINStream::EventHandler()
{
	MSG Message;
	if (PeekMessage(&Message, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
}
bool GDLWINStream::GetGin(PLGraphicsIn *gin, int mode) {
    LPPOINT lpt;

  enum CursorOpt {
    NOWAIT = 0,
    WAIT, //1
    CHANGE, //2
    DOWN, //3
    UP //4
    };
 
	plgpls(&pls);
 // plstream::cmd( PLESC_GETC, gin );
   wingcc_Dev *dev = (wingcc_Dev *) pls->dev;

    HCURSOR    cursor;
    HCURSOR    crosshair;
    HCURSOR    previous;
	RECT rcClient;
    //RECT rcOldClip;
	POINT Point;
    UINT SWP = (SWP_NOMOVE | SWP_NOSIZE |SWP_SHOWWINDOW);
	HWND resetFG;
	resetFG = GetForegroundWindow();
	
	HINSTANCE hInstance = (HINSTANCE)GetModuleHandle(NULL);
	// Create a custom cursor at run time. 
	cursor = CreateCursor(hInstance,   // app. instance 
		15,                // horizontal position of hot spot 
		14,                // vertical position of hot spot 
		32,                // cursor width 
		32,                // cursor height
		ANDmaskCursor,     // AND mask 
		XORmaskCursor);   // XOR mask 
#ifdef _WIN64
    SetClassLongPtr( dev->hwnd, GCLP_HCURSOR, (LONG_PTR) cursor );
#else
    SetClassLong( dev->hwnd, GCLP_HCURSOR, (LONG) cursor );
#endif
//   SetClassLongPtr( dev->hwnd, GCLP_HCURSOR, (LONG_PTR) dev->cursor );
    previous = SetCursor( cursor );

    SWP = (SWP_NOMOVE | SWP_NOSIZE |SWP_SHOWWINDOW);

    SetWindowPos(dev->hwnd, HWND_TOP, 0,0,0,0, SWP);
    GetClientRect(dev->hwnd, &rcClient); // https://msdn.microsoft.com/library/windows/desktop/ms633503%28v=vs.85%29.aspx

 	Point.x=-1;   // negative Xs are unlikely.
//   NOWAIT = 0,    WAIT, //1    CHANGE, //2    DOWN, //3    UP //4
// http://msdn.microsoft.com/en-us/library/windows/desktop/ms645602(v=vs.85).aspx

	bool rbutton, xbutton, mbutton;
	gin->button=0;
    while ( Point.x < 0 )
    {
		rbutton=false; xbutton=false; mbutton=false;
        GetMessage( &dev->msg, NULL, 0, 0 );
        TranslateMessage( &dev->msg );
        switch ( (int) dev->msg.message )
        {
        case WM_XBUTTONDOWN:  xbutton=true;
        case WM_RBUTTONDOWN:  rbutton=true;
        case WM_MBUTTONDOWN:  mbutton=true;

        case WM_LBUTTONDOWN:
			if( mode == 4) {   // Looking for button up
				DispatchMessage(&dev->msg);
				break;
				}
			gin->button=1;
			GetCursorPos(&Point);
            break;
        case WM_XBUTTONUP:		xbutton=true;
        case WM_RBUTTONUP:		rbutton=true;
        case WM_MBUTTONUP:  mbutton=true;
		case WM_LBUTTONUP:
			if( mode == 3) {  // Looking for button down
				DispatchMessage(&dev->msg);
				break;
				}
			gin->button=1;
			GetCursorPos(&Point);
            break;
        case WM_CHAR:
			GetCursorPos(&Point);
            gin->keysym = dev->msg.wParam;
            break;
		default:
			if(mode == 0) {
        		GetCursorPos(&Point);
	 		} else	DispatchMessage(&dev->msg);
        }
    }
	ScreenToClient(dev->hwnd, &Point); // https://msdn.microsoft.com/library/windows/desktop/dd162952%28v=vs.85%29.aspx
	gin->pX = Point.x;
	gin->pY = (rcClient.bottom - rcClient.top) - Point.y;
	if (xbutton) gin->button = 4; else
		if(rbutton) gin->button=3; else
			if(mbutton) gin->button=2;
	gin->dX = ((PLFLT)gin->pX) / (rcClient.right - rcClient.left - 1);
	gin->dY = ((PLFLT)gin->pY) / (rcClient.bottom - rcClient.top - 1);

 	SetForegroundWindow(resetFG);
    SetCursor( previous );

    Sleep(2);
	BringWindowToTop(dev->hwnd);
    Sleep(2);
    SetFocus(resetFG);

  return true;  
}

bool GDLWINStream::PaintImage( unsigned char *idata, PLINT nx, PLINT ny, 
				DLong *pos, DLong tru, DLong chan ) 
  {
  plstream::cmd( PLESC_FLUSH, NULL );
  
  wingcc_Dev *dev = (wingcc_Dev *) pls->dev;
  HDC hdc = dev->hdc;

  PLINT xoff = (PLINT) pos[0];
  PLINT yoff = (PLINT) pos[2];
  PLINT kx, ky;

  PLINT xsize = pls->phyxma;
  PLINT ysize = pls->phyyma;

  PLINT kxLimit = xsize - xoff;
  PLINT kyLimit = ysize - yoff;

  HBITMAP hbitmap;
  BITMAPINFO bi = { 0 };

  if ( nx < kxLimit ) kxLimit = nx;
  if ( ny < kyLimit ) kyLimit = ny;

  hbitmap = CreateCompatibleBitmap(hdc, kxLimit, kyLimit);

  if ( nx > 0 && ny > 0 ) {
    char iclr1, ired, igrn, iblu;
    long curcolor;

	bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bi.bmiHeader.biWidth = kxLimit;
	bi.bmiHeader.biHeight = kyLimit;
	bi.bmiHeader.biPlanes = 1;
	bi.bmiHeader.biBitCount = 24;
	bi.bmiHeader.biCompression = BI_RGB;
	unsigned long ulBmpSize = kxLimit * kyLimit;
	RGBTRIPLE *lpbitmap = new RGBTRIPLE[ulBmpSize];
	GetDIBits(hdc, hbitmap, 0, kyLimit, lpbitmap, &bi, DIB_RGB_COLORS);
	for ( SizeT ix = 0; ix < kxLimit; ++ix ) {
      for ( SizeT iy = 0; iy < kyLimit; ++iy ) {
        kx = xoff + ix;
        ky = yoff + iy;

        if ( tru == 0 && chan == 0 ) {
          iclr1 = idata[iy * nx + ix];

          //curcolor = RGB( pls->cmap1[iclr1].r, pls->cmap1[iclr1].g, pls->cmap1[iclr1].b );
		  lpbitmap[ky*kxLimit + ix].rgbtBlue = pls->cmap1[iclr1].b;
		  lpbitmap[ky*kxLimit + ix].rgbtGreen = pls->cmap1[iclr1].g;
		  lpbitmap[ky*kxLimit + ix].rgbtRed = pls->cmap1[iclr1].r;
          //	 			 printf("ix: %d  iy: %d  pixel: %d\n", ix,iy,curcolor.pixel);

        } else {
          if ( chan == 0 ) {
            if ( tru == 1 ) {
              ired = idata[3 * (iy * nx + ix) + 0];
              igrn = idata[3 * (iy * nx + ix) + 1];
              iblu = idata[3 * (iy * nx + ix) + 2];
            } else if ( tru == 2 ) {
              ired = idata[nx * (iy * 3 + 0) + ix];
              igrn = idata[nx * (iy * 3 + 1) + ix];
              iblu = idata[nx * (iy * 3 + 2) + ix];
            } else if ( tru == 3 ) {
              ired = idata[nx * (0 * ny + iy) + ix];
              igrn = idata[nx * (1 * ny + iy) + ix];
              iblu = idata[nx * (2 * ny + iy) + ix];
            }
			lpbitmap[ky*kxLimit + ix].rgbtBlue = iblu;
			lpbitmap[ky*kxLimit + ix].rgbtGreen = igrn;
			lpbitmap[ky*kxLimit + ix].rgbtRed = ired;
          } else if ( chan == 1 ) {
            ired = idata[1 * (iy * nx + ix) + 0];
			lpbitmap[ky*kxLimit + ix].rgbtRed = ired;
		  }
		  else if (chan == 2) {
            igrn = idata[1 * (iy * nx + ix) + 1];
			lpbitmap[ky*kxLimit + ix].rgbtGreen = igrn;
		  }
		  else if (chan == 3) {
            iblu = idata[1 * (iy * nx + ix) + 2];
			lpbitmap[ky*kxLimit + ix].rgbtBlue = iblu;
		  } // if (chan == 0) else
        } // if (tru == 0  && chan == 0) else
      } // for() inner (indent error)
    } // for() outer
	SetDIBitsToDevice(hdc, 0, 0, kxLimit, kyLimit, 0, 0, 0, kyLimit, lpbitmap, &bi, DIB_RGB_COLORS);
	delete [] lpbitmap;
  }
  DeleteObject(hbitmap);
  RECT rt = { 0, 0, kxLimit, kyLimit};
  RECT rt2;
  AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, FALSE);
  SetWindowPos(dev->hwnd, 0, rt.right, rt.left, rt.right - rt.left, rt.bottom - rt.top, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);

  // Reposition window if it is out of work area
  SystemParametersInfo(SPI_GETWORKAREA, 0, &rt, 0);
  GetWindowRect(dev->hwnd, &rt2);
  LONG wdiff = 0, hdiff = 0;
  if (rt.right < rt2.right) wdiff = rt2.right - rt.right;
  if (rt.bottom < rt2.bottom) hdiff = rt2.bottom - rt.bottom;
  if (wdiff) rt2.left -= wdiff;
  if (hdiff) rt2.top -= hdiff;
  if (wdiff || hdiff)
	  SetWindowPos(dev->hwnd, 0, rt2.left, rt2.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
  return true;
}
void GDLWINStream::Raise() 
{
  wingcc_Dev *dev = (wingcc_Dev *) pls->dev;
  BringWindowToTop(dev->hwnd);
  return;
}
void GDLWINStream::GetGeometry( long& xSize, long& ySize, long& xoff, long& yoff) {
  wingcc_Dev *dev = (wingcc_Dev *) pls->dev;

  GetWindowInfo(dev->hwnd, &Winfo);
  xSize = Winfo.rcWindow.right - Winfo.rcWindow.left;
  ySize = Winfo.rcWindow.bottom - Winfo.rcWindow.top;
  xoff = Winfo.rcWindow.left;
  yoff = Winfo.rcWindow.top;
}
bool GDLWINStream::GetWindowPosition( long& xpos, long& ypos) {
  wingcc_Dev *dev = (wingcc_Dev *) pls->dev;
  
  GetWindowInfo(dev->hwnd, &Winfo);
  xpos = Winfo.rcWindow.left;
  ypos = Winfo.rcWindow.top;
  return true;
}

void GDLWINStream::Lower()
{
  wingcc_Dev *dev = (wingcc_Dev *) pls->dev;
  UINT SWP = (SWP_NOMOVE | SWP_NOSIZE);
  SetWindowPos(dev->hwnd, HWND_BOTTOM, 0,0,0,0, SWP);
  return;
}

void GDLWINStream::Iconic() {
  wingcc_Dev *dev = (wingcc_Dev *) pls->dev;
 UINT SWP = (SWP_NOMOVE | SWP_NOSIZE |SWP_HIDEWINDOW);  // this does nothinc
  SetWindowPos(dev->hwnd, HWND_BOTTOM, 0,0,0,0, SWP);
  return;
}

void GDLWINStream::DeIconic() {
  wingcc_Dev *dev = (wingcc_Dev *) pls->dev;
 UINT SWP = (SWP_NOMOVE | SWP_NOSIZE |SWP_SHOWWINDOW);
  SetWindowPos(dev->hwnd, HWND_BOTTOM, 0,0,0,0, SWP);
  return;

}
void GDLWINStream::CheckValid() {
  wingcc_Dev *dev = (wingcc_Dev *) pls->dev;
  if(!IsWindow(dev->hwnd)) this->SetValid(false);
}

void GDLWINStream::Flush() {
   GdiFlush();
}

DLong GDLWINStream::GetVisualDepth(){
  wingcc_Dev *dev = (wingcc_Dev *) pls->dev;
  return GetDeviceCaps(dev->hdc,PLANES);
  }

unsigned long  GDLWINStream::GetWindowDepth(){
  return GetVisualDepth();
  }

bool GDLWINStream::UnsetFocus()
{ 
    SetFocus(this->refocus); 
    return true;
 }


void GDLWINStream::Clear()
{
    ::c_plbop();
    ::c_plclear();
}
