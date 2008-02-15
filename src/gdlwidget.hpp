/***************************************************************************
                          gdlwidget  -  base class for GDL widgets
                             -------------------
    begin                : Fri May 7 2004
    copyright            : (C) 2004 by Marc Schellens
    email                : m_schellens@users.sourceforge.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#ifndef GDLWIDGET_HPP
#define GDLWIDGET_HPP

#include <wx/wx.h>

#include <map>

#include "typedefs.hpp"

class guiThread : public wxThread
{
public:
  guiThread() : wxThread(wxTHREAD_JOINABLE) {};

  // thread execution starts here
  virtual void *Entry();

  // called when the thread exits - whether it terminates normally or is
  // stopped with Delete() (but not when it is Kill()ed!)
  virtual void OnExit( guiThread *thread);
};
static guiThread *thread;

class GDLWidget;

// global widget list type
typedef DLong                       WidgetIDT;
typedef std::map<WidgetIDT, GDLWidget*> WidgetListT;

// main App class
class GDLApp: public wxApp
{
  virtual int OnRun();  // Defined in GDLApp::OnRun()
  virtual int OnExit(); // Defined in GDLApp::OnExit()
};

// GUI base class **********************************
class GDLWidget
{ 
  // static part is used for the abstraction
  // all widgets are refered to as IDs
private:
  // the global widget list and the actual index for new widgets
  // shared among all widgets
  static WidgetIDT                   widgetIx;
  static WidgetListT                 widgetList;

protected:
  // removes a widget, (called from widgets destructor -> don't delete)
  static void WidgetRemove( WidgetIDT widID);

public:
  // ID for widget (called from widgets constructor)
  static WidgetIDT NewWidget( GDLWidget* w);
  // get widget from ID
  static GDLWidget* GetWidget( WidgetIDT widID);
  static GDLWidget* GetParent( WidgetIDT widID);

  static void Init(); // GUI intialization upon GDL startup

protected:
  wxObject* wxWidget; // deleted only from TLB as the rest is deleted 
                      // automatic

  WidgetIDT    widgetID;  // own index to widgetList
  WidgetIDT    parent;    // parent ID (0 for TLBs)
  BaseGDL*     uValue;    // the UVALUE
  bool         sensitive;
  bool         managed;
  DLong        xOffset, yOffset, xSize, ySize;

public:
  GDLWidget( WidgetIDT p=0, BaseGDL* uV=NULL, bool s=true,
	     DLong xO=-1, DLong yO=-1, DLong xS=-1, DLong yS=-1);
  virtual ~GDLWidget();

  wxObject* WxWidget() { return wxWidget;}
  
  virtual void Realize() {} 
  virtual DLong GetChild( DLong) {};
  virtual void SetXmanagerActiveCommand() {};
  virtual bool GetXmanagerActiveCommand() {};

  static bool GetXmanagerBlock();
  static bool PollEvents( DLong *, DLong *, DLong *, DLong *);

  WidgetIDT WidgetID() { return widgetID;}

  bool GetManaged() { return managed;}
  void SetManaged();
};



class GDLWidgetMbar;

// button widget **************************************************
class GDLWidgetButton: public GDLWidget
{
public:
  GDLWidgetButton( WidgetIDT parentID, DString value); 
};

// base widget **************************************************
class GDLWidgetBase: public GDLWidget
{
protected:
  typedef std::deque<WidgetIDT>::iterator cIter;
  std::deque<WidgetIDT>                   children;
  
  bool                                    xmanActCom;
  bool                                    modal;
  WidgetIDT                               mbarID;

public:
  GDLWidgetBase( WidgetIDT parentID, 
		 BaseGDL* uvalue, DString uname,
		 bool sensitive, bool mapWid,
		 WidgetIDT mBarID, bool modal, 
		 WidgetIDT group_leader,
		 DLong col, DLong row,
		 long events,
		 int exclusiveMode, 
		 bool floating,
		 DString event_func, DString event_pro,
		 DString pro_set_value, DString func_get_value,
		 DString notify_realize, DString kill_notify,
		 DString resource_name, DString rname_mbar,
		 DString title,
		 DLong frame, DLong units,
		 DString display_name,
		 DLong xpad, DLong ypad,
		 DLong xoffset, DLong yoffset,
		 DLong xsize, DLong ysize,
		 DLong scr_xsize, DLong scr_ysize,
		 DLong x_scroll_size, DLong y_scroll_size);

  GDLWidgetBase( WidgetIDT p=0,           // parent
		 BaseGDL* uV=NULL,        // UVALUE
		 bool s=true,             // SENSITIVE
		 DLong xO=-1, DLong yO=-1,  // offset 
		 DLong xS=-1, DLong yS=-1); // size
  virtual ~GDLWidgetBase();

  void AddChild( WidgetIDT c) 
  { children.push_back( c);}
  void RemoveChild( WidgetIDT  c)
  { children.erase( find( children.begin(), children.end(), c));}

  void Realize();
  void SetXmanagerActiveCommand();
  bool GetXmanagerActiveCommand() { return xmanActCom;}

  DLong GetChild( DLong);

};

class GDLWidgetMBar: public GDLWidgetBase
{
};


class GDLFrame : public wxFrame
{
public:
  // ctor(s)
  GDLFrame(wxWindow* parent, wxWindowID id, const wxString& title);

  // event handlers (these functions should _not_ be virtual)
  void OnButton( wxCommandEvent& event);

private:
    // any class wishing to process wxWidgets events must use this macro
  DECLARE_EVENT_TABLE()
};

#endif

