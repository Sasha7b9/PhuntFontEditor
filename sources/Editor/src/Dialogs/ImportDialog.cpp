#include "defines.h"
#include "Canvas.h"
#include "Dialogs/ImportDialog.h"
#include "Dialogs/TextControl.h"
#pragma warning(push, 0)
#include <wx/fontdlg.h>
#pragma warning(pop)


enum
{
    ID_BUTTON_OK,
    ID_BUTTON_CANCEL,
    ID_BUTTON_FONT
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

static wxFont font(11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Courier New"));


ImportDialog::ImportDialog(const wxString &title) : wxDialog(nullptr, wxID_ANY, title)
{
#define BORDER 5
#define SPACER 10
#define _ALIGN wxALIGN_CENTER | wxALL


    wxButton *btnOk = new wxButton(this, ID_BUTTON_OK, wxT("Импортировать"), wxDefaultPosition, BUTTON_SIZE);
    Connect(ID_BUTTON_OK, wxEVT_BUTTON, wxCommandEventHandler(ImportDialog::OnButtonOk));
    wxButton *btnCancel = new wxButton(this, ID_BUTTON_CANCEL, wxT("Отменить"), wxDefaultPosition, BUTTON_SIZE);
    Connect(ID_BUTTON_CANCEL, wxEVT_BUTTON, wxCommandEventHandler(ImportDialog::OnButtonCancel));
    wxBoxSizer *boxButtons = new wxBoxSizer(wxHORIZONTAL);
    boxButtons->Add(btnOk, 1, wxALIGN_CENTER);
    boxButtons->AddSpacer(20);
    boxButtons->Add(btnCancel, 1, wxALIGN_CENTER);

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
}


void ImportDialog::OnChoiceFont(wxCommandEvent &)
{
    wxFontDialog dlg(this);
    
    if (dlg.ShowModal() == wxID_OK)
    {
        font = dlg.GetFontData().GetChosenFont();

        textFont->SetFont(font);

        int pointSize = font.GetPointSize();

        font.SetPointSize(10);

        textFont->SetLabel(wxString::Format("%s %d %s", font.GetFaceName(), pointSize, wxT("Пример")));
    }
}


void ImportDialog::OnButtonOk(wxCommandEvent &)
{

}


void ImportDialog::OnButtonCancel(wxCommandEvent &)
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
