#include "defines.h"
#include "Canvas.h"
#include "Dialogs/ImportDialog.h"
#include "Dialogs/TextControl.h"
#pragma warning(push, 0)
#include <wx/fontdlg.h>
#pragma warning(pop)


enum
{
    ID_BUTTON_FONT = 100
};


/// Информация о выбранном шрифте
static wxStaticText *textFont = nullptr;
/// Ширина символа
static TextControl *tcWidthSymbol = nullptr;
static int width = 8;
/// Высота символа
static TextControl *tcHeightSymbol = nullptr;
static int height = 8;
/// Смещение шрифта по горизонтали
static TextControl *tcOffsetX = nullptr;
static int offsetX = 0;
/// Смещение шрифта по вертикали
static TextControl *tcOffsetY = nullptr;
static int offsetY = 0;


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

    tcWidthSymbol = new TextControl(this, wxT("8"), wxT("ширина символа"));

    tcHeightSymbol = new TextControl(this, wxT("8"), wxT("высота символа"));

    tcOffsetX = new TextControl(this, wxT("0"), wxT("смещение по x"));

    tcOffsetY = new TextControl(this, wxT("0"), wxT("смещение по y"));

    vBox->Add(boxFont, 0, _ALIGN, BORDER);
    vBox->Add(tcWidthSymbol, 0, _ALIGN, BORDER);
    vBox->Add(tcHeightSymbol, 0, _ALIGN, BORDER);
    vBox->Add(tcOffsetX, 0, _ALIGN, BORDER);
    vBox->Add(tcOffsetY, 0, _ALIGN, BORDER);

    SetBoxSizer(vBox, { 200, 200 });
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
