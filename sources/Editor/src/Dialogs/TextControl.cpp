#include "defines.h"
#include "Dialogs/TextControl.h"


TextControl::TextControl(wxWindow *parent, const wxString &value, const wxString &label) : wxPanel(parent)
{
    wxBoxSizer *box = new wxBoxSizer(wxHORIZONTAL);

    tc = new wxTextCtrl(this, wxID_ANY, value, wxDefaultPosition, { 50, 20 });
    tc->SetMaxSize({ 50, 20 });
    box->Add(tc, wxALIGN_CENTER | wxALL);
    box->AddSpacer(10);
    box->Add(new wxStaticText(this, wxID_ANY, label), wxALIGN_CENTER | wxALL, 5);

    SetSizer(box);
}
