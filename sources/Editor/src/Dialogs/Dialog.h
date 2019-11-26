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

    static const int WIDTH_PANEL = 220;

protected:

    void SetBoxSizer(wxBoxSizer *sizer, wxSize size);

private:
    wxBoxSizer *panelBox = nullptr;

    void OnButtonApply(wxCommandEvent &);

    void OnButtonCancel(wxCommandEvent &);
};
