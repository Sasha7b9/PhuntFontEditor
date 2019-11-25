#include "defines.h"
#include "SpinControl.h"
#include "TriangleDialog.h"
#pragma warning(push, 0)
#include <wx/spinctrl.h>
#include <wx/statline.h>
#pragma warning(pop)


enum
{
    ID_SPINCTRL_CENTER,
    ID_SPINCTRL_DELAY
};


static SpinControl *scCenter = nullptr;
static SpinControl *scDelay = nullptr;


static wxPanel *CreatePanelOffsets(wxDialog *dlg)
{
    wxPanel *panel = new wxPanel(dlg);

    int y = 20, x = 10;

    new wxStaticBox(panel, wxID_ANY, wxT("Смещения"), wxDefaultPosition, wxSize(Dialog::WIDTH_PANEL, 75));

    scCenter = new SpinControl(panel, ID_SPINCTRL_CENTER, wxT("0"), wxPoint(x, y), wxSize(50, 20), -100, 100, 0, dlg, wxCommandEventHandler(TriangleDialog::OnControlEvent), wxT("Центр, %"));
    scDelay = new SpinControl(panel, ID_SPINCTRL_DELAY, wxT("0"), wxPoint(x, y + 26), wxSize(50, 20), 0, 1024, 0, dlg, wxCommandEventHandler(TriangleDialog::OnControlEvent), wxT("Задержка, точки"));

    return panel;
}


TriangleDialog::TriangleDialog() : Dialog(wxT("Параметры треугольного сигнала"))
{
    wxBoxSizer *vBox = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *hBoxPanels = new wxBoxSizer(wxHORIZONTAL);

    hBoxPanels->Add(CreatePanelPolarity());
    hBoxPanels->AddStretchSpacer();
    hBoxPanels->Add(CreatePanelLevels());
    vBox->Add(hBoxPanels);
    vBox->Add(CreatePanelOffsets(this));
    
    SetBoxSizer(vBox, { 221, 150 });
}


void TriangleDialog::SendAdditionForm()
{
}
