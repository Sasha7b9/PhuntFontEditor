#pragma once
#pragma warning(push, 0)
#include <wx/wx.h>
#pragma warning(pop)


class ExportDialogC : public wxDialog
{
public:
    ExportDialogC(const wxString &title);

private:
    void OnButtonExport(wxCommandEvent &);
    void OnButtonCancel(wxCommandEvent &);
};
