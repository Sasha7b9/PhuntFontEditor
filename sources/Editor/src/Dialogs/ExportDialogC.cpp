#include "defines.h"
#include "Canvas.h"
#include "Dialogs/ExportDialogC.h"
#include "Dialogs/TextControl.h"
#pragma warning(push, 0)
#include <wx/textfile.h>
#pragma warning(pop)


enum
{
    ID_BUTTON_EXPORT,
    ID_BUTTON_CANCEL
};


static TextControl *tcNameFont = nullptr;
static wxString nameFont(wxT("font"));

static TextControl *tcNameFile = nullptr;
static wxString nameFile(wxT("font.inc"));


ExportDialogC::ExportDialogC(const wxString &title) : wxDialog(nullptr, wxID_ANY, title)
{
#define BORDER 5
#define SPACER 10
#define _ALIGN wxALIGN_CENTER | wxALL

    wxButton *btnExport = new wxButton(this, ID_BUTTON_EXPORT, wxT("Экспорт"), wxDefaultPosition, BUTTON_SIZE);
    Connect(ID_BUTTON_EXPORT, wxEVT_BUTTON, wxCommandEventHandler(ExportDialogC::OnButtonExport));
    wxButton *btnCancel = new wxButton(this, ID_BUTTON_CANCEL, wxT("Отмена"), wxDefaultPosition, BUTTON_SIZE);
    Connect(ID_BUTTON_CANCEL, wxEVT_BUTTON, wxCommandEventHandler(ExportDialogC::OnButtonCancel));
    wxBoxSizer *boxButtons = new wxBoxSizer(wxHORIZONTAL);
    boxButtons->Add(btnExport, 1, wxALIGN_CENTER);
    boxButtons->AddSpacer(20);
    boxButtons->Add(btnCancel, 1, wxALIGN_CENTER);

    tcNameFont = new TextControl(this, nameFont, wxT("Имя шрифта"));

    tcNameFile = new TextControl(this, nameFile, wxT("Имя файла"));

    wxBoxSizer *vBox = new wxBoxSizer(wxVERTICAL);
    
    vBox->Add(tcNameFont);
    vBox->Add(tcNameFile);
    vBox->Add(boxButtons, 0, _ALIGN, BORDER);

    SetSizer(vBox);
}


void ExportDialogC::OnButtonExport(wxCommandEvent &)
{
    nameFont = tcNameFont->GetLineText();
    nameFile = tcNameFile->GetLineText();

    wxFileDialog dlg(nullptr, wxT("Экспорт"), wxEmptyString, nameFile, wxT("*.inc"), wxFD_SAVE);

    if (dlg.ShowModal() == wxID_OK)
    {
        wxTextFile file(dlg.GetPath());
    
        file.Create();
    
        TheCanvas->ImportFont(file, nameFont);
    
        file.Write();
    
        file.Close();

        Destroy();
    }
}


void ExportDialogC::OnButtonCancel(wxCommandEvent &)
{
    Destroy();
}
