#pragma once
#pragma warning(push, 0)
#include <wx/wx.h>
#pragma warning(pop)


class ImportDialog : public wxDialog
{
public:
    ImportDialog(const wxString &title);

protected:

    void OnChoiceFont(wxCommandEvent &);

private:

    void OnButtonOk(wxCommandEvent &);

    void OnButtonCancel(wxCommandEvent &);
};
