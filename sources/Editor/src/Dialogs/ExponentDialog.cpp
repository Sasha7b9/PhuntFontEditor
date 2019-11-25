#include "defines.h"
#include "Dialogs/ExponentDialog.h"
#include "Dialogs/SpinControl.h"
#pragma warning(push, 0)
#include <wx/spinctrl.h>
#include <wx/statline.h>
#pragma warning(pop)
#include <vector>


enum
{
    ID_SPINCTRL_FRONT_DELAY,
    ID_SPINCTRL_FRONT_TIME,
    ID_TEXTCTRL_FRONT_K,
    ID_TEXTCTRL_BACK_K
};


static double oldFrontK = 1.0;
static double oldBackK = 1.0;


static SpinControl *scDelay = nullptr;          // Задержка перед началом экспоненциального импульса
static SpinControl *scFrontTime = nullptr;      // Время от начала экспоненциального импульса до начала спада
static wxTextCtrl *tcFrontK = nullptr;          // Коэффициент экспоненты
static wxTextCtrl *tcBackK = nullptr;


static wxPanel *CreatePanelParameters(wxDialog *dlg)
{
    wxPanel *panel = new wxPanel(dlg);

    new wxStaticBox(panel, wxID_ANY, wxT("Параметры"), wxDefaultPosition, wxSize(Dialog::WIDTH_PANEL, 75 + 26 * 2));

    int y = 20, x = 10, dY = 26;

    scDelay = new SpinControl(panel, ID_SPINCTRL_FRONT_DELAY, wxT("0"), wxPoint(x, y), wxSize(50, 20), 0, 1024, 0, dlg, wxCommandEventHandler(ExponentDialog::OnControlEvent), wxT("Задержка, точки"));
    scFrontTime = new SpinControl(panel, ID_SPINCTRL_FRONT_TIME, wxT("4095"), wxPoint(x, y + dY), wxSize(50, 20), 0, 1024, 1024 / 2, dlg, wxCommandEventHandler(ExponentDialog::OnControlEvent), wxT("Время нарастания, точки"));

    tcFrontK = new wxTextCtrl(panel, ID_TEXTCTRL_FRONT_K, "1.0", wxPoint(x, y + 2 * dY), wxSize(75, 20));
    dlg->Connect(ID_TEXTCTRL_FRONT_K, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(ExponentDialog::OnControlEvent));

    tcBackK = new wxTextCtrl(panel, ID_TEXTCTRL_BACK_K, "1.0", wxPoint(x, y + 3 * dY), wxSize(75, 20));
    dlg->Connect(ID_TEXTCTRL_BACK_K, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(ExponentDialog::OnControlEvent));

    return panel;
}


ExponentDialog::ExponentDialog() : Dialog(wxT("Параметры экспоненциального сигнала"))
{
    wxBoxSizer *vBox = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *hBoxPanels = new wxBoxSizer(wxHORIZONTAL);

    hBoxPanels->Add(CreatePanelPolarity());
    hBoxPanels->AddStretchSpacer();
    hBoxPanels->Add(CreatePanelLevels());
    vBox->Add(hBoxPanels);
    vBox->Add(CreatePanelParameters(this));

    SetBoxSizer(vBox, { 221, 203 });
}


void ExponentDialog::SendAdditionForm()
{
}
