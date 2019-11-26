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


/// Информация о выбранном шрифте
static wxStaticText *textFont = nullptr;
/// Ширина символа
static wxTextCtrl *tcWidthSymbol = nullptr;
/// Высота символа
static wxTextCtrl *tcHeightSymbol = nullptr;


ImportDialog::ImportDialog(const wxString &title) : Dialog(title)
{
#define BORDER 5
#define SPACER 10
#define _ALIGN wxALIGN_CENTER | wxALL

    wxBoxSizer *vBox = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer *boxFont = new wxBoxSizer(wxHORIZONTAL);
    wxButton *btnFont = new wxButton(this, ID_BUTTON_FONT, wxT("Шрифт"), wxDefaultPosition, BUTTON_SIZE);
    Connect(ID_BUTTON_FONT, wxEVT_BUTTON, wxCommandEventHandler(ImportDialog::OnChoiceFont));
    textFont = new wxStaticText(this, wxID_ANY, "");
    boxFont->Add(btnFont, wxALIGN_LEFT, BORDER);
    boxFont->AddSpacer(SPACER);
    boxFont->Add(textFont, 0, _ALIGN, BORDER);

    wxBoxSizer *boxWidth = new wxBoxSizer(wxHORIZONTAL);
    tcWidthSymbol = new wxTextCtrl(this, wxID_ANY, wxT("8"), wxDefaultPosition, { 10, 20 });
    boxWidth->Add(tcWidthSymbol, _ALIGN, BORDER);
    boxWidth->AddSpacer(SPACER);
    boxWidth->Add(new wxStaticText(this, wxID_ANY, wxT("ширина символа")), _ALIGN, BORDER);

    wxBoxSizer *boxHeight = new wxBoxSizer(wxHORIZONTAL);
    tcHeightSymbol = new wxTextCtrl(this, wxID_ANY, wxT("8"), wxDefaultPosition, { 10, 20 });
    boxHeight->Add(tcHeightSymbol, _ALIGN, BORDER);
    boxHeight->AddSpacer(SPACER);
    boxHeight->Add(new wxStaticText(this, wxID_ANY, wxT("высота символа")), _ALIGN, BORDER);

    vBox->Add(boxFont, 0, _ALIGN, BORDER);
    vBox->Add(boxWidth, 0, _ALIGN, BORDER);
    vBox->Add(boxHeight, 0, _ALIGN, BORDER);

    SetBoxSizer(vBox, { 300, 200 });
}


void ImportDialog::OnChoiceFont(wxCommandEvent &)
{
    wxFontDialog dlg(this);
    
    if (dlg.ShowModal() == wxID_OK)
    {
        wxFont font = dlg.GetFontData().GetChosenFont();

        textFont->SetFont(font);

        int pointSize = font.GetPointSize();

        font.SetPointSize(10);

        textFont->SetLabel(wxString::Format("%s %d %s", font.GetFaceName(), pointSize, wxT("Пример")));
    }
}
