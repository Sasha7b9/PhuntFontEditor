#include "defines.h"
#include "Canvas.h"
#include "Dialogs/ImportSystemFontDialog.h"
#include "Dialogs/Controls/TextControl.h"
#pragma warning(push, 0)
#include <wx/fontdlg.h>
#pragma warning(pop)


enum
{
    ID_BUTTON_APPLY,
    ID_BUTTON_CLOSE,
    ID_BUTTON_FONT
};


SettingsFont ImportSystemFontDialog::settings = { 16, 16, 0, 0, wxFont(11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Courier New")) };

// ���������� � ��������� ������
static wxStaticText *textFont = nullptr;

static TextControl *tcWidthCell = nullptr;
static TextControl *tcHeightCell = nullptr;
static TextControl *tcOffsetX = nullptr;
static TextControl *tcOffsetY = nullptr;


ImportSystemFontDialog::ImportSystemFontDialog(const wxString &title) : wxDialog(nullptr, wxID_ANY, title)
{
#define BORDER 5
#define SPACER 10
#define _ALIGN wxALIGN_CENTER | wxALL


    wxButton *btnApply = new wxButton(this, ID_BUTTON_APPLY, wxT("���������"), wxDefaultPosition, BUTTON_SIZE);
    Connect(ID_BUTTON_APPLY, wxEVT_BUTTON, wxCommandEventHandler(ImportSystemFontDialog::OnButtonApply));
    wxButton *btnClose = new wxButton(this, ID_BUTTON_CLOSE, wxT("�������"), wxDefaultPosition, BUTTON_SIZE);
    Connect(ID_BUTTON_CLOSE, wxEVT_BUTTON, wxCommandEventHandler(ImportSystemFontDialog::OnButtonClose));
    wxBoxSizer *boxButtons = new wxBoxSizer(wxHORIZONTAL);
    boxButtons->Add(btnApply, 1, wxALIGN_CENTER);
    boxButtons->AddSpacer(20);
    boxButtons->Add(btnClose, 1, wxALIGN_CENTER);

    wxBoxSizer *vBox = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer *boxFont = new wxBoxSizer(wxHORIZONTAL);
    wxButton *btnFont = new wxButton(this, ID_BUTTON_FONT, wxT("�����"), wxDefaultPosition, BUTTON_SIZE);
    Connect(ID_BUTTON_FONT, wxEVT_BUTTON, wxCommandEventHandler(ImportSystemFontDialog::OnChoiceFont));
    textFont = new wxStaticText(this, wxID_ANY, "");
    boxFont->Add(btnFont, wxALIGN_LEFT, BORDER);
    boxFont->AddSpacer(SPACER);
    boxFont->Add(textFont, 0, wxALIGN_CENTER_VERTICAL, BORDER);

    tcWidthCell = new TextControl(this, wxT("8"), wxT("������ �������"));

    tcHeightCell = new TextControl(this, wxT("8"), wxT("������ �������"));

    tcOffsetX = new TextControl(this, wxT("0"), wxT("�������� �� x"));

    tcOffsetY = new TextControl(this, wxT("0"), wxT("�������� �� y"));

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


void ImportSystemFontDialog::OnChoiceFont(wxCommandEvent &)
{
    wxFontDialog dlg(this);
    
    if (dlg.ShowModal() == wxID_OK)
    {
        settings.font = dlg.GetFontData().GetChosenFont();

        TuneTextFont();
    }
}


void ImportSystemFontDialog::OnButtonApply(wxCommandEvent &)
{
    tcWidthCell->ToInt(&settings.width);
    tcHeightCell->ToInt(&settings.height);
    tcOffsetX->ToInt(&settings.offsetX);
    tcOffsetY->ToInt(&settings.offsetY);

    TheCanvas->Rebuild();
}


void ImportSystemFontDialog::OnButtonClose(wxCommandEvent &)
{
    Destroy();
}


void ImportSystemFontDialog::GetDataImport(SettingsFont *set)
{
    *set = settings;
}


void ImportSystemFontDialog::TuneTexts()
{
    TuneTextFont();

    tcWidthCell->WriteInt(settings.width);
    tcHeightCell->WriteInt(settings.height);
    tcOffsetX->WriteInt(settings.offsetX);
    tcOffsetY->WriteInt(settings.offsetY);
}


void ImportSystemFontDialog::TuneTextFont()
{
    int size = settings.font.GetPointSize();

    settings.font.SetPointSize(10);

    textFont->SetFont(settings.font);

    settings.font.SetPointSize(size);

    textFont->SetLabel(wxString::Format("%s %d %s", settings.font.GetFaceName(), settings.font.GetPointSize(), wxT("������")));
}
