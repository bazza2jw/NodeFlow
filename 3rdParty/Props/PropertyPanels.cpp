//////////////////////////////////////////////////////////////////////
// This file was auto-generated by codelite's wxCrafter Plugin
// wxCrafter project file: PropertyPanels.wxcp
// Do not modify this file by hand!
//////////////////////////////////////////////////////////////////////

#include "PropertyPanels.h"


// Declare the bitmap loading function
extern void wxCrafterOa0GMGInitBitmapResources();

static bool bBitmapLoaded = false;


PropertiesEditorDialogBase::PropertiesEditorDialogBase(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
    : wxDialog(parent, id, title, pos, size, style)
{
    if ( !bBitmapLoaded ) {
        // We need to initialise the default bitmap handler
        wxXmlResource::Get()->AddHandler(new wxBitmapXmlHandler);
        wxCrafterOa0GMGInitBitmapResources();
        bBitmapLoaded = true;
    }
    
    boxSizer = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(boxSizer);
    
    m_toolbar = new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), wxTB_FLAT);
    m_toolbar->SetToolBitmapSize(wxSize(16,16));
    
    boxSizer->Add(m_toolbar, 0, wxALL, WXC_FROM_DIP(5));
    
    m_toolbar->AddTool(ID_NOTES, _("Notes"), wxXmlResource::Get()->LoadBitmap(wxT("application24")), wxNullBitmap, wxITEM_NORMAL, _("Notes for object"), _("Free text notes for object"), NULL);
    
    m_toolbar->AddTool(ID_SETTINGS, _("Settings"), wxXmlResource::Get()->LoadBitmap(wxT("configure24")), wxNullBitmap, wxITEM_NORMAL, _("Settings"), _("Additional properties"), NULL);
    m_toolbar->Realize();
    
    wxArrayString m_propertiesArr;
    wxUnusedVar(m_propertiesArr);
    wxArrayInt m_propertiesIntArr;
    wxUnusedVar(m_propertiesIntArr);
    m_properties = new wxPropertyGridManager(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), wxPG_DESCRIPTION|wxPG_SPLITTER_AUTO_CENTER|wxPG_BOLD_MODIFIED);
    m_properties->SetToolTip(_("Properties of Object"));
    
    boxSizer->Add(m_properties, 1, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    m_stdBtnSizer25 = new wxStdDialogButtonSizer();
    
    boxSizer->Add(m_stdBtnSizer25, 0, wxALL|wxALIGN_RIGHT, WXC_FROM_DIP(5));
    
    m_okButton = new wxButton(this, wxID_OK, wxT(""), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);
    m_okButton->SetToolTip(_("Click to save and exit"));
    m_okButton->SetToolTip(_("Click to save and exit"));
    m_stdBtnSizer25->AddButton(m_okButton);
    
    m_button29 = new wxButton(this, wxID_CANCEL, wxT(""), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);
    m_stdBtnSizer25->AddButton(m_button29);
    m_stdBtnSizer25->Realize();
    
    SetName(wxT("PropertiesEditorDialogBase"));
    SetMinClientSize(wxSize(500,400));
    SetSize(wxDLG_UNIT(this, wxSize(500,500)));
    if (GetSizer()) {
         GetSizer()->Fit(this);
    }
    if(GetParent()) {
        CentreOnParent(wxBOTH);
    } else {
        CentreOnScreen(wxBOTH);
    }
#if wxVERSION_NUMBER >= 2900
    if(!wxPersistenceManager::Get().Find(this)) {
        wxPersistenceManager::Get().RegisterAndRestore(this);
    } else {
        wxPersistenceManager::Get().Restore(this);
    }
#endif
    // Connect events
    this->Connect(ID_NOTES, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(PropertiesEditorDialogBase::OnNotes), NULL, this);
    this->Connect(ID_SETTINGS, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(PropertiesEditorDialogBase::OnSettings), NULL, this);
    m_okButton->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(PropertiesEditorDialogBase::OnOk), NULL, this);
    
}

PropertiesEditorDialogBase::~PropertiesEditorDialogBase()
{
    this->Disconnect(ID_NOTES, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(PropertiesEditorDialogBase::OnNotes), NULL, this);
    this->Disconnect(ID_SETTINGS, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(PropertiesEditorDialogBase::OnSettings), NULL, this);
    m_okButton->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(PropertiesEditorDialogBase::OnOk), NULL, this);
    
}

PropertiesViewerPaneBase::PropertiesViewerPaneBase(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
    : wxPanel(parent, id, pos, size, style)
{
    if ( !bBitmapLoaded ) {
        // We need to initialise the default bitmap handler
        wxXmlResource::Get()->AddHandler(new wxBitmapXmlHandler);
        wxCrafterOa0GMGInitBitmapResources();
        bBitmapLoaded = true;
    }
    
    boxSizer = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(boxSizer);
    
    m_toolbar115 = new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), wxTB_FLAT);
    m_toolbar115->SetToolBitmapSize(wxSize(16,16));
    
    boxSizer->Add(m_toolbar115, 0, wxALL, WXC_FROM_DIP(5));
    
    m_toolbar115->AddTool(wxID_ANY, _("Notes"), wxXmlResource::Get()->LoadBitmap(wxT("application24")), wxNullBitmap, wxITEM_NORMAL, _("Show Notes"), _("Show Notes"), NULL);
    m_toolbar115->Realize();
    
    m_type = new wxStaticText(this, wxID_ANY, _("TYPE"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    
    boxSizer->Add(m_type, 0, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    wxArrayString m_propertiesArr;
    wxUnusedVar(m_propertiesArr);
    wxArrayInt m_propertiesIntArr;
    wxUnusedVar(m_propertiesIntArr);
    m_properties = new wxPropertyGridManager(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), wxPG_DESCRIPTION|wxPG_SPLITTER_AUTO_CENTER|wxPG_BOLD_MODIFIED);
    m_properties->SetToolTip(_("Object properties"));
    
    boxSizer->Add(m_properties, 1, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    SetName(wxT("PropertiesViewerPaneBase"));
    SetMinClientSize(wxSize(500,300));
    SetSize(wxDLG_UNIT(this, wxSize(500,300)));
    if (GetSizer()) {
         GetSizer()->Fit(this);
    }
    // Connect events
    this->Connect(wxID_ANY, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(PropertiesViewerPaneBase::OnNotes), NULL, this);
    
}

PropertiesViewerPaneBase::~PropertiesViewerPaneBase()
{
    this->Disconnect(wxID_ANY, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(PropertiesViewerPaneBase::OnNotes), NULL, this);
    
}

PropertiesViewerDialogBase::PropertiesViewerDialogBase(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
    : wxDialog(parent, id, title, pos, size, style)
{
    if ( !bBitmapLoaded ) {
        // We need to initialise the default bitmap handler
        wxXmlResource::Get()->AddHandler(new wxBitmapXmlHandler);
        wxCrafterOa0GMGInitBitmapResources();
        bBitmapLoaded = true;
    }
    
    boxSizer = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(boxSizer);
    
    m_toolbar = new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), wxTB_FLAT);
    m_toolbar->SetToolBitmapSize(wxSize(16,16));
    
    boxSizer->Add(m_toolbar, 0, wxALL, WXC_FROM_DIP(5));
    
    m_toolbar->AddTool(wxID_ANY, _("Notes"), wxXmlResource::Get()->LoadBitmap(wxT("application24")), wxNullBitmap, wxITEM_NORMAL, _("View Notes"), wxT(""), NULL);
    m_toolbar->Realize();
    
    wxArrayString m_propertiesArr;
    wxUnusedVar(m_propertiesArr);
    wxArrayInt m_propertiesIntArr;
    wxUnusedVar(m_propertiesIntArr);
    m_properties = new wxPropertyGridManager(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), wxPG_DESCRIPTION|wxPG_SPLITTER_AUTO_CENTER|wxPG_BOLD_MODIFIED);
    m_properties->Enable(false);
    
    boxSizer->Add(m_properties, 1, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    m_stdBtnSizer49 = new wxStdDialogButtonSizer();
    
    boxSizer->Add(m_stdBtnSizer49, 0, wxALL|wxALIGN_RIGHT, WXC_FROM_DIP(5));
    
    m_button51 = new wxButton(this, wxID_CLOSE, wxT(""), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);
    m_button51->SetDefault();
    m_stdBtnSizer49->AddButton(m_button51);
    m_stdBtnSizer49->Realize();
    
    SetName(wxT("PropertiesViewerDialogBase"));
    SetMinClientSize(wxSize(500,400));
    SetSize(wxDLG_UNIT(this, wxSize(500,400)));
    if (GetSizer()) {
         GetSizer()->Fit(this);
    }
    if(GetParent()) {
        CentreOnParent(wxBOTH);
    } else {
        CentreOnScreen(wxBOTH);
    }
#if wxVERSION_NUMBER >= 2900
    if(!wxPersistenceManager::Get().Find(this)) {
        wxPersistenceManager::Get().RegisterAndRestore(this);
    } else {
        wxPersistenceManager::Get().Restore(this);
    }
#endif
    // Connect events
    this->Connect(wxID_ANY, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(PropertiesViewerDialogBase::OnNotes), NULL, this);
    m_button51->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(PropertiesViewerDialogBase::OnClose), NULL, this);
    
}

PropertiesViewerDialogBase::~PropertiesViewerDialogBase()
{
    this->Disconnect(wxID_ANY, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(PropertiesViewerDialogBase::OnNotes), NULL, this);
    m_button51->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(PropertiesViewerDialogBase::OnClose), NULL, this);
    
}
