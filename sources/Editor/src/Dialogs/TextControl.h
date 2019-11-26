#pragma once
#pragma warning(push, 0)
#include <wx/wx.h>
#pragma warning(pop)


class TextControl : public wxPanel
{
public:
    TextControl(wxWindow *parent, const wxString &value, const wxString &label);

private:
    wxTextCtrl *tc = nullptr;
};
