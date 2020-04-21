#pragma once
#pragma warning(push, 0)
#include <wx/wx.h>
#pragma warning(pop)


struct Symbol;
class wxXmlNode;


class ExportDialogC : public wxDialog
{
public:
    ExportDialogC(const wxString &title);

private:
    void OnButtonExport(wxCommandEvent &);
    void OnButtonCancel(wxCommandEvent &);

    // �������� � ���� ���������� � ������
    void WriteFileXML(const wxString &nameFileFont);

    // ���������� ���������� � ������ � ������� ���� node
    void WriteInfoSymbolXML(uint8 code, wxXmlNode *node);
};
