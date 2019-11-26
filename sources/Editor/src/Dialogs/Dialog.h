#pragma once
#pragma warning(push, 0)
#include <wx/wx.h>
#pragma warning(pop)


class SpinControl;


class Dialog : public wxDialog
{
public:
    Dialog(const wxString &title);
    virtual ~Dialog();

    void OnControlEvent(wxCommandEvent &);

    static const int WIDTH_PANEL = 220;

protected:

    void SetBoxSizer(wxBoxSizer *sizer, wxSize size);

    void DrawLine(int x1, int y1, int x2, int y2);
    /// ������ ��� ������� � TheForm
    static uint16 data[1024];

    /// ������ ������ ���������� �������
    wxPanel *CreatePanelPolarity();
    wxRadioButton *rbPolarityDirect = nullptr;
    wxRadioButton *rbPolarityBack = nullptr;

    /// ������ ������ ������� �������
    wxPanel *CreatePanelLevels();
    SpinControl *scLevelUp = nullptr;
    SpinControl *scLevelDown = nullptr;

private:
    wxBoxSizer *panelBox = nullptr;

    void OnMove(wxMoveEvent &);

    void OnButtonApply(wxCommandEvent &);

    void OnButtonCancel(wxCommandEvent &);

    void OnKeyDown(wxKeyEvent &);
};
