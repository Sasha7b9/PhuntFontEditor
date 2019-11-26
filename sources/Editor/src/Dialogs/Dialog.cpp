#include "defines.h"
#include "Editor.h"
#include "Dialogs/Dialog.h"
#include "Dialogs/SpinControl.h"


enum
{
    ID_BUTTON_OK,
    ID_BUTTON_CANCEL
};


Dialog::Dialog(const wxString &title) : wxDialog(nullptr, wxID_ANY, title)
{
    wxButton *btnOk = new wxButton(this, ID_BUTTON_OK, wxT("Принять"), wxDefaultPosition, BUTTON_SIZE);
    Connect(ID_BUTTON_OK, wxEVT_BUTTON, wxCommandEventHandler(Dialog::OnButtonApply));
    wxButton *btnCancel = new wxButton(this, ID_BUTTON_CANCEL, wxT("Отменить"), wxDefaultPosition, BUTTON_SIZE);
    Connect(ID_BUTTON_CANCEL, wxEVT_BUTTON, wxCommandEventHandler(Dialog::OnButtonCancel));

    wxBoxSizer *vBox = new wxBoxSizer(wxVERTICAL);
    panelBox = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *hBox = new wxBoxSizer(wxHORIZONTAL);

    vBox->Add(panelBox);
    hBox->Add(btnOk, 1, wxALIGN_CENTER);
    hBox->AddSpacer(20);
    hBox->Add(btnCancel, 1, wxALIGN_CENTER);
    vBox->AddSpacer(10);
    vBox->Add(hBox, 0, wxALIGN_CENTER);

    SetSizer(vBox);
}


Dialog::~Dialog()
{
}


void Dialog::SetBoxSizer(wxBoxSizer *sizer, wxSize size)
{
    panelBox->Add(sizer);

    size.y += 50;

    SetClientSize(size);

    Centre();

    SetFocus();
}


void Dialog::OnButtonApply(wxCommandEvent &)
{
    Destroy();
}


void Dialog::OnButtonCancel(wxCommandEvent &) //-V524
{
    Destroy();
}
