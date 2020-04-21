#include "defines.h"
#include "Canvas.h"
#include "Dialogs/ImportDialog.h"
#include "Dialogs/TextControl.h"
#pragma warning(push, 0)
#include <wx/fontdlg.h>
#pragma warning(pop)


enum
{
    ID_BUTTON_APPLY,
    ID_BUTTON_CLOSE,
    ID_BUTTON_FONT
};


// Информация о выбранном шрифте
static wxStaticText *textFont = nullptr;

long ImportDialog::widthCell = 16;
static TextControl *tcWidthCell = nullptr;

long ImportDialog::heightCell = 16;
static TextControl *tcHeightCell = nullptr;

long ImportDialog::offsetSymbolX = 0;
static TextControl *tcOffsetX = nullptr;

long ImportDialog::offsetSymbolY = 0;
static TextControl *tcOffsetY = nullptr;


wxFont ImportDialog::font(11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Courier New"));


ImportDialog::ImportDialog(const wxString &title) : wxDialog(nullptr, wxID_ANY, title)
{
#define BORDER 5
#define SPACER 10
#define _ALIGN wxALIGN_CENTER | wxALL


    wxButton *btnApply = new wxButton(this, ID_BUTTON_APPLY, wxT("Применить"), wxDefaultPosition, BUTTON_SIZE);
    Connect(ID_BUTTON_APPLY, wxEVT_BUTTON, wxCommandEventHandler(ImportDialog::OnButtonApply));
    wxButton *btnClose = new wxButton(this, ID_BUTTON_CLOSE, wxT("Закрыть"), wxDefaultPosition, BUTTON_SIZE);
    Connect(ID_BUTTON_CLOSE, wxEVT_BUTTON, wxCommandEventHandler(ImportDialog::OnButtonClose));
    wxBoxSizer *boxButtons = new wxBoxSizer(wxHORIZONTAL);
    boxButtons->Add(btnApply, 1, wxALIGN_CENTER);
    boxButtons->AddSpacer(20);
    boxButtons->Add(btnClose, 1, wxALIGN_CENTER);

    wxBoxSizer *vBox = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer *boxFont = new wxBoxSizer(wxHORIZONTAL);
    wxButton *btnFont = new wxButton(this, ID_BUTTON_FONT, wxT("Шрифт"), wxDefaultPosition, BUTTON_SIZE);
    Connect(ID_BUTTON_FONT, wxEVT_BUTTON, wxCommandEventHandler(ImportDialog::OnChoiceFont));
    textFont = new wxStaticText(this, wxID_ANY, "");
    boxFont->Add(btnFont, wxALIGN_LEFT, BORDER);
    boxFont->AddSpacer(SPACER);
    boxFont->Add(textFont, 0, wxALIGN_CENTER_VERTICAL, BORDER);

    tcWidthCell = new TextControl(this, wxT("8"), wxT("ширина символа"));

    tcHeightCell = new TextControl(this, wxT("8"), wxT("высота символа"));

    tcOffsetX = new TextControl(this, wxT("0"), wxT("смещение по x"));

    tcOffsetY = new TextControl(this, wxT("0"), wxT("смещение по y"));

    vBox->Add(boxFont, 0, wxALIGN_LEFT, BORDER);
    vBox->Add(tcWidthCell, 0, _ALIGN, BORDER);
    vBox->Add(tcHeightCell, 0, _ALIGN, BORDER);
    vBox->Add(tcOffsetX, 0, _ALIGN, BORDER);
    vBox->Add(tcOffsetY, 0, _ALIGN, BORDER);

    vBox->AddSpacer(10);
    vBox->Add(boxButtons, 0, _ALIGN, BORDER);

    SetSizer(vBox);

    TuneTexts();
}


void ImportDialog::OnChoiceFont(wxCommandEvent &)
{
    wxFontDialog dlg(this);
    
    if (dlg.ShowModal() == wxID_OK)
    {
        font = dlg.GetFontData().GetChosenFont();

        TuneTextFont();
    }
}


void ImportDialog::OnButtonApply(wxCommandEvent &)
{
    tcWidthCell->ToLong(widthCell);
    tcHeightCell->ToLong(heightCell);
    tcOffsetX->ToLong(offsetSymbolX);
    tcOffsetY->ToLong(offsetSymbolY);

    TheCanvas->Rebuild();
}


void ImportDialog::OnButtonClose(wxCommandEvent &)
{
    Destroy();
}


void ImportDialog::GetDataImport(DataImport &data)
{
    data.width = widthCell;
    data.height = heightCell;
    data.offsetX = offsetSymbolX;
    data.offsetY = offsetSymbolY;
    data.font = font;
}


void ImportDialog::TuneTexts()
{
    TuneTextFont();

    tcWidthCell->FromLong(widthCell);
    tcHeightCell->FromLong(heightCell);
    tcOffsetX->FromLong(offsetSymbolX);
    tcOffsetY->FromLong(offsetSymbolY);
}


void ImportDialog::TuneTextFont()
{
    int size = font.GetPointSize();

    font.SetPointSize(10);

    textFont->SetFont(font);

    font.SetPointSize(size);

    textFont->SetLabel(wxString::Format("%s %d %s", font.GetFaceName(), font.GetPointSize(), wxT("Пример")));
}
