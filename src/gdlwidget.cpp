/***************************************************************************
                          gdlwidget.cpp  -  GDL widget system implementation 
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

#include "includefirst.hpp"

#include <memory> 

#include "basegdl.hpp"
#include "dstructgdl.hpp"

#include "dinterpreter.hpp"

#ifdef HAVE_LIBWXWIDGETS

#include "gdlwidget.hpp"

//#define GDL_DEBUG_WIDGETS

// instantiation
WidgetIDT                   GDLWidget::widgetIx;
WidgetListT                 GDLWidget::widgetList;

VarListT                    eventVarList;

BEGIN_EVENT_TABLE(GDLFrame, wxFrame)
  EVT_BUTTON( wxID_ANY, GDLFrame::OnButton)
  EVT_RADIOBUTTON(wxID_ANY, GDLFrame::OnRadioButton)
  EVT_IDLE( GDLFrame::OnIdle)
END_EVENT_TABLE()


IMPLEMENT_APP_NO_MAIN( GDLApp)

void getSizer( DLong col, DLong row, DLong frameBox, 
	       wxPanel *panel, wxSizer **sizer) {
  if ( frameBox == 0) {
    if ( row == 0) {
      *sizer = new wxBoxSizer( wxVERTICAL);
    } else if ( row != 0 && col == 0) {
      *sizer = new wxBoxSizer( wxHORIZONTAL);
    } else {
      std::cout << "Shouldn't be here" << std::endl;
      exit(2);
    }
  } else {

    wxStaticBox *box = 
      new wxStaticBox( panel, wxID_ANY, _T(""));
    if ( row == 0) {
      *sizer = new wxStaticBoxSizer( box, wxVERTICAL);
    } else if ( row != 0 && col == 0) {
      *sizer = new wxStaticBoxSizer( box, wxHORIZONTAL);
    } else {
      std::cout << "Shouldn't be here" << std::endl;
      exit(2);
    }
  }
  //  std::cout << "Creating Sizer in getSizer: " << *sizer << std::endl;
}

// next are the abstraction to access all widgets only by their
// handle ID
// ID for widget (called from widgets constructor)
WidgetIDT GDLWidget::NewWidget( GDLWidget* w)
{
  //  std::cout << " In NewWidget()" << std::endl;
  WidgetIDT tmpIx = widgetIx;
  widgetList.insert( widgetList.end(),
		     std::pair<WidgetIDT, GDLWidget*>( widgetIx++, w));

  return tmpIx;
}

// removes a widget, (called from widgets destructor -> don't delete)
void GDLWidget::WidgetRemove( WidgetIDT widID)
{
  widgetList.erase( widID); 
}


// widget from ID
GDLWidget* GDLWidget::GetWidget( WidgetIDT widID)
{
  WidgetListT::iterator it=widgetList.find( widID);
  if( it == widgetList.end()) return NULL;
  return it->second;
}

// widget parent from ID
GDLWidget* GDLWidget::GetParent( WidgetIDT widID)
{
  GDLWidget *widget = GetWidget( widID);
  WidgetIDT parentID = widget->parent;
  GDLWidget *parent = GetWidget( parentID);
  return widget;
}

// base widget ID from ID
WidgetIDT GDLWidget::GetBase( WidgetIDT widID)
{
  GDLWidget *widget, *parent;
  WidgetIDT parentID;

  parentID = widID;
  while ( 1) {
    widget = GetWidget( parentID);
    if ( widget->parent == 0) 
      return parentID; 
    else 
      parentID = widget->parent;
  }
}

void GDLWidget::SetManaged( bool manval)
{
  managed = manval;
}

void GDLWidget::SetMap( bool mapval)
{
  //  std::cout << "set map: " << mapval << std::endl;
  map = mapval;
}

void GDLWidget::SetExclusiveMode( int exclusiveval)
{
  exclusiveMode = exclusiveval;
}

void GDLWidget::SetUvalue( BaseGDL *uV)
{
  uValue = uV;
}

void GDLWidget::SetVvalue( BaseGDL *vV)
{
  vValue = vV;
}

void GDLWidget::SetWidgetType( DString wType)
{
  widgetType = wType;
}

void GDLWidget::SetButtonOff()
{
  buttonSet = false;
}

void GDLWidget::SetButtonOn()
{
  buttonSet = true;
}

void GDLWidget::SetUname( DString uname)
{
  uName = uname;
}

void GDLWidget::SetProValue( DString provalue)
{
  proValue = provalue;
}

void GDLWidget::SetFuncValue( DString funcvalue)
{
  funcValue = funcvalue;
}

//void GDLWidget::SetSizer( wxSizer *sizer)
//{
//  widgetSizer = sizer;
//}

bool GDLWidget::GetXmanagerBlock() {
  bool xmanBlock = false;
  WidgetListT::iterator it;
  // (*it).first is widgetID
  // (*it).second is pointer to widget

  bool managed;
  bool xmanActCom;

  for( it = widgetList.begin(); it != widgetList.end(); ++it) {
    // Only consider base widgets
    if ( (*it).second->parent == 0) {
      managed = (*it).second->GetManaged();
      xmanActCom = (*it).second->GetXmanagerActiveCommand();
    }
    if ( managed && !xmanActCom) {
      xmanBlock = true;
      break;
    }
  }
  return xmanBlock;
}

// PollEvents
bool GDLWidget::PollEvents( DLong *id, DLong *top, 
			    DLong *handler, DLong *select) {
  bool eventFound = false;
  WidgetListT::iterator it;
  // (*it).first is widgetID
  // (*it).second is pointer to widget

  for( it = widgetList.begin(); it != widgetList.end(); ++it) {
    // Only consider base widgets
    if ( (*it).second->parent == 0) {
      if ( (*it).second->GetManaged()) {
	// Get Parent Widget
	GDLWidget *parent = GDLWidget::GetParent( (*it).first);
	DLong nChildren = parent->GetChild( -1);
	for( SizeT j=0; j<nChildren; j++) {
	  WidgetIDT childID = parent->GetChild( j);

	  // Check for BUTTON events

	  // Form button event variable name
	  std::ostringstream varname;
	  varname << "WBUT" << childID ;
	  //	    DString vnamestring = varname.rdbuf()->str();
	  //*vname = vnamestring;

	  // Find name and get SELECT
	  DVar *var =
	    FindInVarList( eventVarList, varname.rdbuf()->str().c_str());
	  //  std::cout << "var: " << var << std::endl;

	  if ( var != NULL) {
	    DStructGDL* s = static_cast<DStructGDL*>( var->Data());
	    *select = (*static_cast<DLongGDL*>
		       (s->GetTag(s->Desc()->TagIndex("SELECT"), 0)))[0];
	    if ( *select) {
	      eventFound = true;
	      *id = (*static_cast<DLongGDL*>
		     (s->GetTag(s->Desc()->TagIndex("ID"), 0)))[0];
	      *top = (*static_cast<DLongGDL*>
		      (s->GetTag(s->Desc()->TagIndex("TOP"), 0)))[0];
	      *handler = (*static_cast<DLongGDL*>
			  (s->GetTag(s->Desc()->TagIndex("HANDLER"), 0)))[0];
	      break;
	    }
	  } // var != NULL

	  // Check for text events

	  // Form text event variable name
	  varname << "WTXT" << childID ;

	  // Find name and get SELECT
	  var = FindInVarList( eventVarList, varname.rdbuf()->str().c_str());
	  //std::cout << "var: " << var << std::endl;

	  if ( var != NULL) {
	    DStructGDL* s = static_cast<DStructGDL*>( var->Data());
	    *select = (*static_cast<DLongGDL*>
		       (s->GetTag(s->Desc()->TagIndex("SELECT"), 0)))[0];
	    if ( *select) {
	      eventFound = true;
	      *id = (*static_cast<DLongGDL*>
		     (s->GetTag(s->Desc()->TagIndex("ID"), 0)))[0];
	      *top = (*static_cast<DLongGDL*>
		      (s->GetTag(s->Desc()->TagIndex("TOP"), 0)))[0];
	      *handler = (*static_cast<DLongGDL*>
			  (s->GetTag(s->Desc()->TagIndex("HANDLER"), 0)))[0];
	      break;
	    }
	  } // var != NULL

	} // child loop
      } // if mananged
    } // if base widget
  } // widget loop
  return eventFound;
}

// Init
void GDLWidget::Init()
{
  // Called by InitObjects() in object.cpp
  //  std::cout << " In GDLWidget::Init()" << std::endl;
  widgetIx = wxID_HIGHEST; // use same wx ID and GDL ID 

  wxInitialize();
}


GDLWidget::GDLWidget( WidgetIDT p, BaseGDL* uV, BaseGDL* vV, bool s, bool mp,
		      DLong xO, DLong yO, DLong xS, DLong yS): 
  wxWidget( NULL),
  parent( p), uValue( uV), vValue( vV), sensitive( s), map( mp),
  xOffset( xO), yOffset( yO), xSize( xS), ySize( yS)
{
  managed = false;
  widgetID = NewWidget( this);
  if( parent != 0)
    {
      GDLWidgetBase* base = dynamic_cast< GDLWidgetBase*>( GetWidget( parent));
      assert( base != NULL); // should be already checked elsewhere
      base->AddChild( widgetID);
    }
}

GDLWidget::~GDLWidget()
{
  //  std::cout << "in ~GDLWidget(): " << std::endl;
  managed = false;

  if( parent != 0) 
    {
      GDLWidgetBase* base = dynamic_cast< GDLWidgetBase*>( GetWidget( parent));
      assert( base != NULL);
      base->RemoveChild( widgetID);
    }
  GDLDelete(uValue);
  GDLDelete(vValue);
  WidgetRemove( widgetID);
}

GDLWidgetBase::GDLWidgetBase( WidgetIDT p, BaseGDL* uV, BaseGDL* vV, 
			      bool s, bool mp,
			      DLong xO, DLong yO, DLong xS, DLong yS): 
  GDLWidget( p, uV, vV, s, mp, xO, yO, xS, yS)
{}

GDLWidgetBase::GDLWidgetBase( WidgetIDT parentID, 
			      BaseGDL* uvalue, DString uname,
			      bool sensitive, bool mapWid,
			      WidgetIDT mBarID, bool modal_, 
			      WidgetIDT group_leader,
			      DLong col, DLong row,
			      long events,
			      int exclusiveMode, 
			      bool floating,
			      DString event_func, DString event_pro,
			      DString pro_set_value, DString func_get_value,
			      DString notify_realize, DString kill_notify,
			      DString resource_name, DString rname_mbar,
			      DString title_,
			      DLong frameBox, DLong units,
			      DString display_name,
			      DLong xpad, DLong ypad,
			      DLong xoffset, DLong yoffset,
			      DLong xsize, DLong ysize,
			      DLong scr_xsize, DLong scr_ysize,
			      DLong x_scroll_size, DLong y_scroll_size):
  GDLWidget( parentID, uvalue, NULL, sensitive, map, xoffset, yoffset, 0, 0),
  modal( modal_), mbarID( mBarID)
{
  //  std::cout << "In GDLWidgetBase::GDLWidgetBase: " << widgetID << std::endl;

  xmanActCom = false;
  wxWindow *wxParent = NULL;

  wxSizer *sizer;
  wxSizer **sizerPtr;
  sizerPtr = &sizer;

  // If first base widget ...
  if ( parentID == 0) {
    if (thread != NULL)
      thread->OnExit( thread);

    thread = new guiThread();

    // Defined in threadpsx.cpp (wxWidgets)
#ifdef GDL_DEBUG_WIDGETS
    std::cout << "Creating thread: " << thread << std::endl;
#endif    
    
    thread->Create();
    thread->Run();

    // GUI lock defined in threadpsx.cpp
#ifdef GDL_DEBUG_WIDGETS
    std::cout << "before wxMutexGuiEnter()" << std::endl;
#endif    
    
    wxMutexGuiEnter();

#ifdef GDL_DEBUG_WIDGETS
    std::cout << "after wxMutexGuiEnter()" << std::endl;
#endif    

    // GDLFrame is derived from wxFrame
    GDLFrame *frame = new GDLFrame( wxParent, widgetID, wxString(title_.c_str(), wxConvUTF8));
    ((wxFrame *) frame)->SetSize( xsize, ysize);
    wxWidget = frame;

    wxPanel *panel = new wxPanel( frame, wxID_ANY);
    widgetPanel = panel;
    //    std::cout << "Creating Panel: " << panel << std::endl;

    getSizer( col, row, frameBox, panel, sizerPtr);
    widgetSizer = *sizerPtr;

    topWidgetSizer = *sizerPtr;
    //std::cout << "SetSizer: " << *sizerPtr << std::endl;
    panel->SetSizer( *sizerPtr);

    this->SetMap( mapWid);

  } else {
    // If parent base widget exists ....
    GDLWidget* gdlParent = GetWidget( parent);
    wxParent = static_cast< wxWindow*>( gdlParent->WxWidget());
    //    std::cout << "Getting Parent: " << parent << " " << gdlParent << " " 
    //      << wxParent << std::endl;

    wxPanel *panel = gdlParent->GetPanel();
    widgetPanel = panel;
    //    std::cout << "Getting Panel: " << panel << std::endl;

    // If map is true check for parent map value
    if ( mapWid) mapWid = gdlParent->GetMap();
    this->SetMap( mapWid);

    // Set exclusiveMode
    // If exclusive then set to -1 to signal first radiobutton
    if ( exclusiveMode == 1)
      this->SetExclusiveMode( -exclusiveMode);
    else
      this->SetExclusiveMode(  exclusiveMode);

    if ( mapWid) {
      if ( frameBox == 0) {
	if ( row == 0) {
	  sizer = new wxBoxSizer( wxVERTICAL);
	} else if ( row != 0 && col == 0) {
	  sizer = new wxBoxSizer( wxHORIZONTAL);
	} else {
	  std::cout << "Shouldn't be here" << std::endl;
	  exit(2);
	}
      } else {
	wxStaticBox *box = 
	  new wxStaticBox( panel, wxID_ANY, _T(""));
	if ( row == 0) {
	  sizer = new wxStaticBoxSizer( box, wxVERTICAL);
	} else if ( row != 0 && col == 0) {
	  sizer = new wxStaticBoxSizer( box, wxHORIZONTAL);
	} else {
	  std::cout << "Shouldn't be here" << std::endl;
	  exit(2);
	}
      }

      widgetSizer = sizer;
      //      std::cout << "Creating Sizer2: " << sizer << std::endl;
    
      if ( frameBox == 0) {
	wxBoxSizer *parentSizer = (wxBoxSizer *) gdlParent->GetSizer();
	//    std::cout << "Getting Parent Sizer:" << parentSizer << std::endl;
	parentSizer->Add( sizer, 0, wxEXPAND|wxALL, 5);
      } else {
	wxStaticBoxSizer *parentSizer = 
	  (wxStaticBoxSizer *) gdlParent->GetSizer();
	//    std::cout << "Getting Parent Sizer:" << parentSizer << std::endl;
	parentSizer->Add( sizer, 0, wxEXPAND|wxALL, 5);
      }

      if( modal)
	wxWidget = new wxDialog( wxParent, widgetID, wxString(title_.c_str(), wxConvUTF8));

    } // if (mapWid == true)
  }
}

GDLWidgetBase::~GDLWidgetBase()
{
  // Close widget frame
  //  std::cout << "In ~GDLWidgetBase() widget: " << this->wxWidget << std::endl;
  ((wxFrame *) this->wxWidget)->Close( true);

  //  bool running = thread->IsRunning();
  //std::cout << "running (in ~GDLWidgetBase): " << running << std::endl;

  // Note: iterator for loop doesn't work when deleting widget
  cIter cI = children.begin();
  for( SizeT i=0; i<children.size(); i++) {
    //delete GetWidget( *cI++);
  }

  // if TLB destroy wxWidget 
  //  if( parent == 0)
  //delete wxWidget;
}

void GDLWidgetBase::Realize( bool map)
{
  wxFrame *frame = (wxFrame *) this->wxWidget;
  bool stat = frame->Show( map);
  //  wxString nme = frame->GetName();
  //std::cout << frame->IsShown() << std::endl;

  // GUI unlock defined in threadpsx.cpp
  //  std::cout << "wxMutexGuiLeave()" << std::endl;
  wxMutexGuiLeave();
}


DLong GDLWidgetBase::GetChild( DLong childIx)
{
  if ( childIx == -1)
    return ((DLong) children.size());
  else
    return ((DLong) children[childIx]);
}


void GDLWidgetBase::SetXmanagerActiveCommand()
{
  xmanActCom = true;
}

void  GDLWidgetBase::SetEventPro( DString eventPro)
{
  std::cout << "Setting up event handler: " << eventPro.c_str() << std::endl;
  eventHandler = eventPro;
}



GDLWidgetButton::GDLWidgetButton( WidgetIDT p, BaseGDL *uV, DString value):
  GDLWidget( p, uV, NULL, 0, 0, 0, 0, 0)
{
  GDLWidget* gdlParent = GetWidget( p);
  wxWindow *wxParent = static_cast< wxWindow*>( gdlParent->WxWidget());

  //  std::cout << "In Button: " << widgetID << " Parent: " << p << " xMode:" <<
  //gdlParent->GetExclusiveMode() << " " << value << std::endl;


  if (gdlParent->GetMap()) {
    wxPanel *panel = gdlParent->GetPanel();

    wxButton *button;
    wxRadioButton *radioButton;
    wxCheckBox *checkBox;
    wxBoxSizer *boxSizer = (wxBoxSizer *) gdlParent->GetSizer();

    if ( gdlParent->GetExclusiveMode() == 0) {
      button = new wxButton( panel, widgetID, wxString(value.c_str(), wxConvUTF8));
      boxSizer->Add( button, 0, wxEXPAND | wxALL, 5);
    } else if ( gdlParent->GetExclusiveMode() == -1) {
      radioButton = new wxRadioButton( panel, widgetID, wxString( value.c_str(), wxConvUTF8),
				       wxDefaultPosition, wxDefaultSize,
				       wxRB_GROUP);
      gdlParent->SetExclusiveMode( 1);
      boxSizer->Add( radioButton, 0, wxEXPAND | wxALL, 5);
    } else if ( gdlParent->GetExclusiveMode() == 1) {
      radioButton = new wxRadioButton( panel, widgetID, wxString(value.c_str(), wxConvUTF8));
      boxSizer->Add( radioButton, 0, wxEXPAND | wxALL, 5);
    } else if ( gdlParent->GetExclusiveMode() == 2) {
      checkBox = new wxCheckBox( panel, wxID_ANY, wxString(value.c_str(), wxConvUTF8));
      boxSizer->Add( checkBox, 0, wxEXPAND | wxALL, 5);
    }

    if ( wxParent != NULL) {
      //      std::cout << "SetSizeHints: " << wxParent << std::endl;
      boxSizer->SetSizeHints( wxParent);
    }
  } // GetMap()


  // Generate event structure
  DStructGDL*  widgbut = new DStructGDL( "WIDGET_BUTTON");
  widgbut->InitTag("ID", DLongGDL( widgetID));
  widgbut->InitTag("TOP", DLongGDL( GDLWidget::GetBase( p)));
  widgbut->InitTag("HANDLER", DLongGDL( 0));
  widgbut->InitTag("SELECT", DLongGDL( 0));

  // Push event structure into event variable list
  std::ostringstream varname;
  varname << "WBUT" << this->WidgetID();
  DVar *v = new DVar( varname.rdbuf()->str().c_str(), widgbut);
  eventVarList.push_back(v);
}

void GDLWidgetButton::SetSelectOff()
{
  // Form button event variable name
  std::ostringstream varname;
  varname << "WBUT" << this->WidgetID();

  // Find name and set SELECT tag to 0
  DVar *var=FindInVarList( eventVarList, varname.rdbuf()->str().c_str());
  DStructGDL* s = static_cast<DStructGDL*>( var->Data());
  (*static_cast<DLongGDL*>
   (s->GetTag(s->Desc()->TagIndex("SELECT"), 0)))[0] = 0;
}

//GDLWidgetDropList::GDLWidgetDropList( WidgetIDT p, BaseGDL *uV, DStringGDL *value,
GDLWidgetDropList::GDLWidgetDropList( WidgetIDT p, BaseGDL *uV, BaseGDL *value,
				      DString title, DLong xSize, DLong style):
  GDLWidget( p, uV, NULL, 0, 0, 0, 0, 0)
{
  //  std::cout << "In DropList: " << widgetID << " " << p << std::endl;

  GDLWidget* gdlParent = GetWidget( p);
  wxWindow *wxParent = static_cast< wxWindow*>( gdlParent->WxWidget());

  wxComboBox *combo;

  if (gdlParent->GetMap()) {
    wxPanel *panel = gdlParent->GetPanel();

    DStringGDL* val = static_cast<DStringGDL*>( value);

    DLong n= val->N_Elements();
    wxString *choices = new wxString[n];
    for( SizeT i=0; i<n; ++i) choices[i] = wxString((*val)[i].c_str(), wxConvUTF8);

    combo = new wxComboBox( panel, widgetID, wxString( (const wchar_t*) choices[0].c_str(), wxConvUTF8),
			    wxDefaultPosition, wxDefaultSize,
			    n, choices, style);

    wxBoxSizer *boxSizer = (wxBoxSizer *) gdlParent->GetSizer();
    boxSizer->Add( combo, 0, wxEXPAND | wxALL, 5);

    if ( wxParent != NULL) {
      //      std::cout << "SetSizeHints: " << wxParent << std::endl;
      boxSizer->SetSizeHints( wxParent);
    }
  } // GetMap()

  // Generate event structure
  DStructGDL*  widgdlist = new DStructGDL( "WIDGET_DROPLIST");
  widgdlist->InitTag("ID", DLongGDL( widgetID));
  widgdlist->InitTag("TOP", DLongGDL( p));
  widgdlist->InitTag("HANDLER", DLongGDL( 0));
  widgdlist->InitTag("SELECT", DLongGDL( 0));

  // Push event structure into event variable list
  std::ostringstream varname;
  varname << "WDLIST" << this->WidgetID();
  DVar *v = new DVar( varname.rdbuf()->str().c_str(), widgdlist);
  eventVarList.push_back(v);
}


GDLWidgetText::GDLWidgetText( WidgetIDT p, BaseGDL *uV, DString value,
			      DLong xSize):
  GDLWidget( p, uV, NULL, 0, 0, 0, 0, 0)
{
  //  std::cout << "In Text: " << widgetID << " " << p << std::endl;

  GDLWidget* gdlParent = GetWidget( p);
  wxWindow *wxParent = static_cast< wxWindow*>( gdlParent->WxWidget());


  if (gdlParent->GetMap()) {
    wxPanel *panel = gdlParent->GetPanel();
    text = new wxTextCtrl( panel, widgetID, wxString( value.c_str(), wxConvUTF8),
			   wxDefaultPosition, wxSize( xSize*5, wxDefaultCoord) );

    wxBoxSizer *boxSizer = (wxBoxSizer *) gdlParent->GetSizer();
    boxSizer->Add( text, 0, wxEXPAND | wxALL, 5);

    if ( wxParent != NULL) {
      //      std::cout << "SetSizeHints: " << wxParent << std::endl;
      boxSizer->SetSizeHints( wxParent);
    }
  } // GetMap()

  // Generate event structure
  DStructGDL*  widgtxt = new DStructGDL( "WIDGET_TEXT");
  widgtxt->InitTag("ID", DLongGDL( widgetID));
  widgtxt->InitTag("TOP", DLongGDL( p));
  widgtxt->InitTag("HANDLER", DLongGDL( 0));
  widgtxt->InitTag("SELECT", DLongGDL( 0));

  // Push event structure into event variable list
  std::ostringstream varname;
  varname << "WTXT" << this->WidgetID();
  DVar *v = new DVar( varname.rdbuf()->str().c_str(), widgtxt);
  eventVarList.push_back(v);
}


void GDLWidgetText::SetTextValue( DString value)
{
  text->SetValue( wxString( value.c_str(), wxConvUTF8));
  //  text->Refresh(); 
  //wxMilliSleep(700); 
 }


GDLWidgetLabel::GDLWidgetLabel( WidgetIDT p, BaseGDL *uV, DString value,
				DLong xSize):
  GDLWidget( p, uV, NULL, 0, 0, 0, 0, 0)
{
  //  std::cout << "In Label: " << widgetID << " " << p << std::endl;

  GDLWidget* gdlParent = GetWidget( p);
  wxWindow *wxParent = static_cast< wxWindow*>( gdlParent->WxWidget());
  
  if ( gdlParent->GetMap()) {
    wxPanel *panel = gdlParent->GetPanel();
    label = new wxStaticText( panel, wxID_ANY, wxString( value.c_str(), wxConvUTF8),
			      wxPoint(10, 10), wxDefaultSize, wxALIGN_CENTRE);

    wxBoxSizer *boxSizer = (wxBoxSizer *) gdlParent->GetSizer();
    boxSizer->Add( label, 0, wxEXPAND | wxALL, 5);

    if ( wxParent != NULL) {
      //      std::cout << "SetSizeHints: " << wxParent << std::endl;
      boxSizer->SetSizeHints( wxParent);
    }
  } // GetMap()

}

void GDLWidgetLabel::SetLabelValue( DString value)
{
  label->SetLabel( wxString( value.c_str(), wxConvUTF8));
 }

// *** GDLFrame ***
GDLFrame::GDLFrame(wxWindow* parent, wxWindowID id, const wxString& title)
       : wxFrame(parent, id, title)
{
  //  std::cout << "in GDLFrame Constructor" << std::endl;
}

void GDLFrame::OnButton( wxCommandEvent& event)
{
  // Called by EVT_BUTTON in EVENT_TABLE in gdlwidget.cpp

  std::cout << "in OnButton: " << event.GetId() << std::endl;

  // Get XmanagerActiveCommand status
  WidgetIDT baseWidgetID = GDLWidget::GetBase( event.GetId());
  // std::cout << "Base Widget ID: " << baseWidgetID << std::endl;
  GDLWidget *baseWidget = GDLWidget::GetWidget( baseWidgetID);
  bool xmanActCom = baseWidget->GetXmanagerActiveCommand();
  //std::cout << "xmanActCom: " << xmanActCom << std::endl;

  // Form button event variable name
  std::ostringstream varname;
  varname << "WBUT" << event.GetId();

  // Find name and set SELECT tag to 1
  DVar *var=FindInVarList( eventVarList, varname.rdbuf()->str().c_str());
  DStructGDL* s = static_cast<DStructGDL*>( var->Data());

  (*static_cast<DLongGDL*>
   (s->GetTag(s->Desc()->TagIndex("SELECT"), 0)))[0] = 1;

  if ( xmanActCom == true) {
    DString eventHandler;
    eventHandler = baseWidget->GetEventPro();
    eventHandler = StrUpCase( eventHandler);

    int proIx = ProIx( eventHandler); 
    DSub *sub =  proList[ proIx];
//     EnvUDT* e;
//     e = new EnvUDT( NULL, sub);
//     std::auto_ptr< EnvUDT> e_guard( e);
//     StackSizeGuard<EnvStackT> guard( GDLInterpreter::CallStack());
//     GDLInterpreter::CallStack().push_back( e);

	// TODO: MS: if the only purpose of e is to get the caller:
	// EnvBaseT* caller = GDLInterpreter::CallStack().back(); is sufficient
// ms: commented out to comply with new stack handling
    EnvBaseT* caller = GDLInterpreter::CallStackBack();
//     caller = e->Caller();
//     e->Interpreter()->CallStack().pop_back();

    DLong id, top, handler, select;
    id = (*static_cast<DLongGDL*>
	  (s->GetTag(s->Desc()->TagIndex("ID"), 0)))[0];
    top = (*static_cast<DLongGDL*>
	   (s->GetTag(s->Desc()->TagIndex("TOP"), 0)))[0];
    handler = (*static_cast<DLongGDL*>
	       (s->GetTag(s->Desc()->TagIndex("HANDLER"), 0)))[0];

    // Build event handler command
    std::ostringstream ostr;
    ostr << "EV={WIDGET_BUTTON, ";
    ostr << "ID: " << id << "L, TOP: " << top << "L, ";
    ostr << "HANDLER: " << handler << "L, SELECT: " << select << "L } ";
    ostr << "& " << eventHandler.c_str() << ", EV";

    DString line = ostr.rdbuf()->str();
    std::istringstream istr(line+"\n");

    RefDNode theAST;

    GDLLexer lexer(istr, "", GDLParser::NONE);
    GDLParser& parser = lexer.Parser();
    parser.interactive();

    theAST = parser.getAST();
    RefDNode trAST;
    GDLTreeParser treeParser( caller);
    treeParser.interactive(theAST);
    trAST = treeParser.getAST();

    ProgNodeP progAST = ProgNode::NewProgNode( trAST);
    std::auto_ptr< ProgNode> progAST_guard( progAST);

    // necessary for correct FOR loop handling
    assert( dynamic_cast<EnvUDT*>(caller) != NULL);
    EnvUDT* env = static_cast<EnvUDT*>(caller);
    int nForLoopsIn = env->NForLoops();
    int nForLoops = ProgNode::NumberForLoops( progAST, nForLoopsIn);
    env->ResizeForLoops( nForLoops);
    env->ResizeForLoops( nForLoopsIn);

    RetCode retCode =
      caller->Interpreter()->execute( progAST);
  }

  // Pause 50 millisecs then refresh widget
  wxMilliSleep( 50);
  Refresh();
}

void GDLFrame::OnRadioButton( wxCommandEvent& event)
{
  std::cout << "in OnRadioButton: " << event.GetId() << std::endl;


}

void GDLFrame::OnIdle( wxIdleEvent&)
{
  //  std::cout << "In OnIdle" << std::endl;
  // Refresh();
}


// *** guiThread ***
void *guiThread::Entry()
{
  // Called from PthreadStart() in threadpsx.cpp (wxWidgets)

  // gui loop

  //  std::cout << "In thread Entry()" << std::endl;

  wxTheApp->OnRun();
  // Calls GDLApp::OnRun()

  return NULL;
}

int GDLApp::OnRun()
{
  // Called by guiThread::Entry()

  //  std::cout << " In OnRun()" << std::endl;

  int exitcode = wxApp::OnRun();
  // Note: Calls wxAppBase::OnRun() in appcmn.cpp (wxWidgets)
  // MainLoop() etc

  if (exitcode!=0)
    return exitcode;
}

int GDLApp::OnExit()
{
  // Called by exitgdl() in basic_pro.cpp

  //  std::cout << " In GDLApp::OnExit()" << std::endl;
  
  //bool running = thread->IsRunning();
  //std::cout << "running: " << running << std::endl;

  // Defined in guiThread::OnExit() in gdlwidget.cpp
  //  std::cout << "Exiting thread (GDLApp::OnExit): " << thread << std::endl;
  if (thread != NULL)
    thread->OnExit( thread);

  return 0;
}


void guiThread::OnExit( guiThread *thread)
{
  // Called by GDLApp::OnExit() in gdlwidget.cpp
#ifdef GDL_DEBUG_WIDGETS
  std::cout << "In guiThread::OnExit(): " << thread << std::endl;
#endif

  delete thread;
}

#endif

