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

#include <deque>
#include <map>

#include "typedefs.hpp"
#include "str.hpp"

// thread save deque
class GDLEventQueue
{
private:
  std::deque<DStructGDL*> dq;
  bool isEmpty;
  wxMutex mutex;
public:
  GDLEventQueue()
  : isEmpty( true)
  {}
  
  DStructGDL* pop()
  {
    mutex.Lock();
    DStructGDL* front = dq.front();
    dq.pop_front();
    isEmpty = dq.empty();
    mutex.Unlock();
    return front;
  }
  void push( DStructGDL* w)
  {
    mutex.Lock();
    dq.push_back( w);
    isEmpty = false;
    mutex.Unlock();    
  }
  bool empty() const 
  { 
    return isEmpty;    
  }
};
// class GDLEventQueuePolledGuard
// {
//   GDLEventQueue* eq;
//   bool polledIn;
//   
// public:
//   GDLEventQueuePolledGuard(GDLEventQueue* e)
//   : eq( e)
//   , polledIn( e->GetIsPolled())
//   {
//     eq->SetIsPolled( true);
//   }
//   ~GDLEventQueuePolledGuard()
//   {
//     eq->SetIsPolled( polledIn);
//   }
// };

class GDLGUIThread : public wxThread
{
  bool exited;
  
public:
  GDLGUIThread() : wxThread(wxTHREAD_JOINABLE)
  , exited(false)
  {};

  bool Exited() const { return exited;}

  // thread execution starts here
  ExitCode Entry();

  // called when the thread exits - whether it terminates normally or is
  // stopped with Delete() (but not when it is Kill()ed!)
  void OnExit();
  
  void Exit(); // end this
};

static GDLGUIThread *gdlGUIThread = NULL;

class GDLWidget;

// global widget list type
typedef DLong                       WidgetIDT;
typedef std::map<WidgetIDT, GDLWidget*> WidgetListT;
// typedef std::deque<DStructGDL*> EventQueueT;


// main App class
class GDLApp: public wxApp
{
public:
  int OnRun(); 
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
  static GDLEventQueue eventQueue;
  static GDLEventQueue readlineEventQueue;
  static int HandleEvents();
  static const WidgetIDT NullID;
  
  // ID for widget (called from widgets constructor)
  static WidgetIDT NewWidget( GDLWidget* w);
  // get widget from ID
  static GDLWidget* GetWidget( WidgetIDT widID);
  static GDLWidget* GetParent( WidgetIDT widID);
  static WidgetIDT GetTopLevelBase( WidgetIDT widID);

  static void Init(); // GUI intialization upon GDL startup

protected:
  wxObject* wxWidget; // deleted only from TLB as the rest is deleted 
                      // automatic
                      // Note: wxWidget is GDL name not wxWidgets (JMG)

  WidgetIDT    widgetID;  // own index to widgetList
  WidgetIDT    parentID;    // parent ID (0 for TLBs)
  BaseGDL*     uValue;    // the UVALUE
  BaseGDL*     vValue;    // the VVALUE
  bool         sensitive;
  bool         managed;
  bool         map;
  bool         buttonSet;
  int          exclusiveMode;
  DLong        xOffset, yOffset, xSize, ySize;
  wxSizer*     topWidgetSizer;
  wxSizer*     widgetSizer;
  wxPanel*     widgetPanel;
  DString      widgetType;
  DString      uName;

private:  
  DString      proValue;
  DString      funcValue;
  DString      eventPro; // event handler PRO
  DString      eventFun; // event handler FUN
  
public:
  GDLWidget( WidgetIDT p=0, BaseGDL* uV=NULL, BaseGDL* vV=NULL,
	     bool s=true, bool mp=true,
	     DLong xO=-1, DLong yO=-1, DLong xS=-1, DLong yS=-1);
  virtual ~GDLWidget();

//   void InitParentID( WidgetIDT p) {assert(parentID == 0); parentID = p;}
  WidgetIDT GetParentID() const { return parentID;}
  
  wxObject* WxWidget() { return wxWidget;}

  BaseGDL* GetUvalue() { return uValue;}
  BaseGDL* GetVvalue() { return vValue;}

  virtual void Realize( bool) {} 
  virtual DLong GetChild( DLong) {return 0;};
  virtual void SetXmanagerActiveCommand() {};
  virtual bool GetXmanagerActiveCommand() {return false;};

  void SetEventPro( const DString& ePro) { eventPro = StrUpCase( ePro);}
  const DString& GetEventPro() const { return eventPro;};
  void SetEventFun( const DString& eFun) { eventFun = StrUpCase( eFun);}
  const DString& GetEventFun() const { return eventFun;};

  static bool GetXmanagerBlock();
//   static bool PollEvents( DLong *, DLong *, DLong *, DLong *);

  WidgetIDT WidgetID() { return widgetID;}

  wxSizer* GetSizer() { return widgetSizer;}
  wxPanel* GetPanel() { return widgetPanel;}
  //  void SetSizer( wxSizer*);

  bool GetManaged() { return managed;}
  void SetManaged( bool manval){managed = manval;}


  bool GetMap() { return map;}
void SetMap( bool mapval){ map = mapval;}

  int  GetExclusiveMode() { return exclusiveMode;}
void SetExclusiveMode( int exclusiveval){exclusiveMode = exclusiveval;}

void SetUvalue( BaseGDL *uV){uValue = uV;}
void SetVvalue( BaseGDL *vV){vValue = vV;}

  const DString& GetWidgetType() { return widgetType;}
void SetWidgetType( const DString& wType){widgetType = wType;}

  bool GetButtonSet() { return buttonSet;}
void SetButtonSet(bool onOff){buttonSet = onOff;}

  const DString& GetUname() { return uName;}
void SetUname( const DString& uname){uName = uname;}

  const DString& GetProValue() { return proValue;}
void SetProValue( const DString&  provalue){proValue = StrUpCase(provalue);}

  const DString& GetFuncValue() { return funcValue;}
void SetFuncValue( const DString&  funcvalue){funcValue = StrUpCase(funcvalue);}
};



class GDLWidgetMbar;

// button widget **************************************************
class GDLWidgetButton: public GDLWidget
{
public:
  GDLWidgetButton( WidgetIDT parentID, BaseGDL *uvalue, DString value);

//   void SetSelectOff();
};


// droplist widget **************************************************
class GDLWidgetDropList: public GDLWidget
{
public:
  //  GDLWidgetDropList( WidgetIDT p, BaseGDL *uV, DStringGDL *value,
  //	     DString title, DLong xSize, DLong style);
  GDLWidgetDropList( WidgetIDT p, BaseGDL *uV, BaseGDL *value,
		     DString title, DLong xSize, DLong style);
//   void SetSelectOff();
};

// list widget **************************************************
class GDLWidgetList : public GDLWidget
{
public:
  GDLWidgetList( WidgetIDT p, BaseGDL *uV, BaseGDL *value,
								 DLong xSize, DLong ySize, DLong style);
//   void SetSelectOff();
};

// bgroup widget **************************************************
class GDLWidgetBGroup: public GDLWidget
{
public:
	typedef enum e_BGroupMode {NORMAL, EXCLUSIVE, NONEXCLUSIVE}
	BGroupMode;
	typedef enum e_BGRoupReturn {RETURN_ID, RETURN_INDEX, RETURN_NAME}
	BGroupReturn;

	GDLWidgetBGroup(WidgetIDT p, DStringGDL* names,
			BaseGDL *uV, DString buttonuvalue,
			DLong xSize, DLong ySize,
			DString labeltop, DLong rows, DLong cols,
			BGroupMode mode, BGroupReturn ret
			);

};

// text widget **************************************************
class GDLWidgetText: public GDLWidget
{
private:
  wxTextCtrl *text;

public:
  GDLWidgetText( WidgetIDT parentID, BaseGDL *uvalue, DString value,
		  DLong xSize, bool editable);
 
  void SetTextValue( DString);
};


// label widget **************************************************
class GDLWidgetLabel: public GDLWidget
{
private:
  wxStaticText *label;

public:
  GDLWidgetLabel( WidgetIDT parentID, BaseGDL *uvalue, DString value,
		  DLong xSize);
 
  void SetLabelValue( DString);
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
//   DString                                 eventHandler;

public:
  GDLWidgetBase( WidgetIDT parentID, 
		 BaseGDL* uvalue, const DString& uname,
		 bool sensitive, bool mapWid,
		 WidgetIDT& mBarIDInOut, bool modal, 
		 WidgetIDT group_leader,
		 DLong col, DLong row,
		 long events,
		 int exclusiveMode, 
		 bool floating,
		 const DString& event_func, const DString& event_pro,
		 const DString& pro_set_value, const DString& func_get_value,
		 const DString& notify_realize, const DString& kill_notify,
		 const DString& resource_name, const DString& rname_mbar,
		 const DString& title,
		 DLong frame, DLong units,
		 const DString& display_name,
		 DLong xpad, DLong ypad,
		 DLong xoffset, DLong yoffset,
		 DLong xsize, DLong ysize,
		 DLong scr_xsize, DLong scr_ysize,
		 DLong x_scroll_size, DLong y_scroll_size);

  GDLWidgetBase( WidgetIDT p=0,           // parent
		 BaseGDL* uV=NULL,        // UVALUE
		 BaseGDL* vV=NULL,        // VVALUE
		 bool s=true,             // SENSITIVE
		 bool mp=true,             // MAP
		 DLong xO=-1, DLong yO=-1,  // offset 
		 DLong xS=-1, DLong yS=-1); // size
  virtual ~GDLWidgetBase();

  void AddChild( WidgetIDT c) 
  { children.push_back( c);}
  void RemoveChild( WidgetIDT  c)
  { children.erase( find( children.begin(), children.end(), c));}

  void Realize( bool);
  
  void SetXmanagerActiveCommand() { xmanActCom = true;}
  bool GetXmanagerActiveCommand() const { return xmanActCom;}

//   void SetEventPro( DString);
//   const DString& GetEventPro() { return eventHandler;}

  DLong GetChild( DLong);

};

class GDLWidgetMBar: public GDLWidget//Base
{
  // disable
  GDLWidgetMBar();
public:
  GDLWidgetMBar( WidgetIDT p): GDLWidget( p)
  {
    this->wxWidget = new wxMenuBar();
  }
};


class GDLFrame : public wxFrame
{
public:
  // ctor(s)
  GDLFrame(wxWindow* parent, wxWindowID id, const wxString& title);
  ~GDLFrame()
  { std::cout << "~GDLFrame: " << this << std::endl;}

  // event handlers (these functions should _not_ be virtual)
  void OnButton( wxCommandEvent& event);
  void OnRadioButton( wxCommandEvent& event);
  void OnIdle( wxIdleEvent& event);

// private:
  // any class wishing to process wxWidgets events must use this macro
  DECLARE_EVENT_TABLE()
};

#endif

