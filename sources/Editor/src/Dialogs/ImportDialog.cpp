#include "defines.h"
#include "Canvas.h"
#include "Dialogs/ImportDialog.h"
#pragma warning(push, 0)
#include <wx/fontdlg.h>
#pragma warning(pop)


enum
{
    ID_BUTTON_FONT
};


ImportDialog::ImportDialog(const wxString &title) : Dialog(title)
{
    wxBoxSizer *vBox = new wxBoxSizer(wxVERTICAL);

    wxButton *btnFont = new wxButton(this, ID_BUTTON_FONT, wxT("״נטפע"), wxDefaultPosition, BUTTON_SIZE);
    Connect(ID_BUTTON_FONT, wxEVT_BUTTON, wxCommandEventHandler(ImportDialog::OnChoiceFont));

    vBox->Add(btnFont);

    SetBoxSizer(vBox, { 200, 200 });
}


void ImportDialog::OnChoiceFont(wxCommandEvent &)
{
    wxFontDialog dlg(this);
    
    if (dlg.ShowModal() == wxID_OK)
    {
        TheCanvas->Rebuild(dlg.GetFontData().GetChosenFont());
        TheCanvas->Refresh();
    }
}
