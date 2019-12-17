#pragma once
#pragma warning(push, 0)
#include <wx/wx.h>
#pragma warning(pop)


struct DataImport
{
    int width;
    int height;
    int offsetX;
    int offsetY;
    wxFont font;
};


class ImportDialog : public wxDialog
{
public:
    ImportDialog(const wxString &title);

    static void GetDataImport(DataImport &data);
    /// ������������� �����
    static wxFont font;

protected:

    void OnChoiceFont(wxCommandEvent &);

private:

    void OnButtonApply(wxCommandEvent &);

    void OnButtonClose(wxCommandEvent &);
    /// ������������ ������ �������� ������
    static void TuneTextFont();
    /// ���������� ��� ������������ ������� � ����������� � �������� �����������
    static void TuneTexts();
};
