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


/// Информация о выбранном шрифте
static wxStaticText *textFont = nullptr;
/// Ширина символа
static TextControl *tcWidthSymbol = nullptr;
static long width = 8;
/// Высота символа
static TextControl *tcHeightSymbol = nullptr;
static long height = 8;
/// Смещение шрифта по горизонтали
static TextControl *tcOffsetX = nullptr;
static long offsetX = 0;
/// Смещение шрифта по вертикали
static TextControl *tcOffsetY = nullptr;
static long offsetY = 0;

static wxFont font(11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Courier New"));

/// Сформировать строку описания шрифта
static void TuneTextFont();
/// Установить все полагающиеся надписи в соотвествии с текущими настройками
static void TuneTexts();


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

    tcWidthSymbol = new TextControl(this, wxT("8"), wxT("ширина символа"));

    tcHeightSymbol = new TextControl(this, wxT("8"), wxT("высота символа"));

    tcOffsetX = new TextControl(this, wxT("0"), wxT("смещение по x"));

    tcOffsetY = new TextControl(this, wxT("0"), wxT("смещение по y"));

    vBox->Add(boxFont, 0, wxALIGN_LEFT, BORDER);
    vBox->Add(tcWidthSymbol, 0, _ALIGN, BORDER);
    vBox->Add(tcHeightSymbol, 0, _ALIGN, BORDER);
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
    tcWidthSymbol->ToLong(width);
    tcHeightSymbol->ToLong(height);
    tcOffsetX->ToLong(offsetX);
    tcOffsetY->ToLong(offsetY);

    TheCanvas->Rebuild();
}


void ImportDialog::OnButtonClose(wxCommandEvent &)
{
    Destroy();
}


void ImportDialog::GetDataImport(DataImport &data)
{
    data.width = width;
    data.height = height;
    data.offsetX = offsetX;
    data.offsetY = offsetY;
    data.font = font;
}


static void TuneTexts()
{
    TuneTextFont();

    tcWidthSymbol->FromLong(width);
    tcHeightSymbol->FromLong(height);
    tcOffsetX->FromLong(offsetX);
    tcOffsetY->FromLong(offsetY);
}


static void TuneTextFont()
{
    int size = font.GetPointSize();

    font.SetPointSize(10);

    textFont->SetFont(font);

    font.SetPointSize(size);

    textFont->SetLabel(wxString::Format("%s %d %s", font.GetFaceName(), font.GetPointSize(), wxT("Пример")));
}
