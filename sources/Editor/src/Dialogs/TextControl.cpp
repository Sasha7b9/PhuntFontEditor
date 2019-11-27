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


bool TextControl::ToLong(long &value)
{
    return tc->GetLineText(0).ToLong(&value);
}


void TextControl::FromLong(long value)
{
    tc->Clear();
    tc->WriteText(wxString::Format("%d", value));
}


wxString TextControl::GetLineText()
{
    return tc->GetLineText(0);
}
