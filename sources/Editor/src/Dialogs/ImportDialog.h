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

    static void GetDataImport(DataImport *data);
   
protected:

    void OnChoiceFont(wxCommandEvent &);

private:

    void OnButtonApply(wxCommandEvent &);

    void OnButtonClose(wxCommandEvent &);
    
    // ������������ ������ �������� ������
    static void TuneTextFont();
    
    // ���������� ��� ������������ ������� � ����������� � �������� �����������
    static void TuneTexts();

    static wxFont font;         // ������������� �����  
    static long offsetSymbolX;  // �������� ������ �� �����������
    static long offsetSymbolY;  // �������� ������ �� ���������
    static long widthCell;      // ������ �������
    static long heightCell;     // ������ �������
};
