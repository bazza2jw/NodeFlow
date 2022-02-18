#include "NodeEditorChild.h"
#include "NodeEditorMainframe.h"
/*!
 * \brief NodeEditorChild::NodeEditorChild
 * \param parent
 */
NodeEditorChild::NodeEditorChild(wxMDIParentFrame *parent)
    : 	wxMDIChildFrame(parent,wxID_ANY,"Editor")
{
    canvasSizer = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(canvasSizer);
    _panel = new NodeEditorPanel(this);
    canvasSizer->Add(_panel, 1, wxALL|wxEXPAND, WXC_FROM_DIP(5));

    wxMenu *menuFile = new wxMenu;

    menuFile->Append(wxID_NEW, "&New window\tCtrl-N", "Create a new flow editor window");
    menuFile->Append(wxID_OPEN, "&Open File...\tCtrl-O", "Open flow file");
    menuFile->Append(wxID_SAVE, "&Save File\tCtrl-S", "Save flow file");
    menuFile->Append(wxID_SAVEAS, "Save &As\tCtrl-A", "Save flow file as");
    menuFile->Append(wxID_CLOSE,"&Close Window","Close this window");
    menuFile->Append(wxID_EXIT, "&Exit\tAlt-X", "Quit the program");



    wxMenuBar *mbar = new wxMenuBar;
    mbar->Append(menuFile, "&File");
    SetMenuBar(mbar);
    this->Connect(wxID_CLOSE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(NodeEditorChild::onClose), NULL, this);
    this->Connect(wxID_NEW, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(NodeEditorMainframe::onNew), NULL, parent);
    this->Connect(wxID_EXIT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(NodeEditorMainframe::onExit), NULL, parent);
    //
    this->Connect(wxID_OPEN, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(NodeEditorPanel::onOpen), NULL, _panel);
    this->Connect(wxID_SAVE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(NodeEditorPanel::onSave), NULL, _panel);
    this->Connect(wxID_SAVEAS, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(NodeEditorPanel::onSaveAs), NULL, _panel);

}
/*!
 * \brief NodeEditorChild::~NodeEditorChild
 */
NodeEditorChild::~NodeEditorChild()
{
    this->Disconnect(wxID_CLOSE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(NodeEditorChild::onClose), NULL, this);
    this->Disconnect(wxID_NEW, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(NodeEditorMainframe::onNew), NULL, GetParent());
    this->Disconnect(wxID_EXIT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(NodeEditorMainframe::onExit), NULL, GetParent());
    //
    this->Disconnect(wxID_OPEN, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(NodeEditorPanel::onOpen), NULL, _panel);
    this->Disconnect(wxID_SAVE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(NodeEditorPanel::onSave), NULL, _panel);
    this->Disconnect(wxID_SAVEAS, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(NodeEditorPanel::onSaveAs), NULL, _panel);
}

/*!
 * \brief NodeEditorChild::onClose
 * \param event
 */
void NodeEditorChild::onClose(wxCommandEvent& event)
{
    Close();
}
