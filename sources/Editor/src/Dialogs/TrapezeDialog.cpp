#include "defines.h"
#include "Dialogs/SpinControl.h"
#include "Dialogs/TrapezeDialog.h"
#pragma warning(push, 0)
#include <wx/spinctrl.h>
#include <wx/statline.h>
#pragma warning(pop)
#include <vector>


enum
{
    ID_SPINCTRL_DELAY,
    ID_SPINCTRL_VERTEX_1,
    ID_SPINCTRL_VERTEX_2
};


static SpinControl *scDelay = nullptr;
static SpinControl *scVertex1 = nullptr;
static SpinControl *scVertex2 = nullptr;


static wxPanel *CreatePanelOffsets(wxDialog *dlg)
{
    wxPanel *panel = new wxPanel(dlg);

    int y = 20, x = 10;

    new wxStaticBox(panel, wxID_ANY, wxT("Смещения"), wxDefaultPosition, wxSize(Dialog::WIDTH_PANEL, 73 + 26));

    scDelay = new SpinControl(panel, ID_SPINCTRL_DELAY, wxT("0"), wxPoint(x, y), wxSize(50, 20), 0, 1024, 0, dlg, wxCommandEventHandler(TrapezeDialog::OnControlEvent), wxT("Задержка, точки"));
    scVertex1 = new SpinControl(panel, ID_SPINCTRL_VERTEX_1, wxT("-50"), wxPoint(x, y + 26), wxSize(50, 20), -100, 100, -50, dlg, wxCommandEventHandler(TrapezeDialog::OnControlEvent), wxT("Левая вершина, %"));
    scVertex2 = new SpinControl(panel, ID_SPINCTRL_VERTEX_2, wxT("50"), wxPoint(x, y + 26 * 2), wxSize(50, 20), -100, 100, -50, dlg, wxCommandEventHandler(TrapezeDialog::OnControlEvent), wxT("Правая вершина, %"));

    return panel;
}


TrapezeDialog::TrapezeDialog() : Dialog(wxT("Параметры трапециевидного сигнала"))
{
    wxBoxSizer *vBox = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *hBoxPanels = new wxBoxSizer(wxHORIZONTAL);

    hBoxPanels->Add(CreatePanelPolarity());
    hBoxPanels->AddStretchSpacer();
    hBoxPanels->Add(CreatePanelLevels());
    vBox->Add(hBoxPanels);
    vBox->Add(CreatePanelOffsets(this));

    SetBoxSizer(vBox, { 221, 175 });
}


void TrapezeDialog::SendAdditionForm()
{

}
