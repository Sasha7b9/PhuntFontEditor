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
    
    // ������������� �����
    static wxFont font;
    
    // ������ �������
    static long widthCell;
    
    // ������ �������
    static long heightCell;
    
    // �������� ������ �� �����������
    static long offsetSymbolX;
    
private:
    // �������� ������ �� ���������
    static long offsetSymbolY;

protected:

    void OnChoiceFont(wxCommandEvent &);

private:

    void OnButtonApply(wxCommandEvent &);

    void OnButtonClose(wxCommandEvent &);
    
    // ������������ ������ �������� ������
    static void TuneTextFont();
    
    // ���������� ��� ������������ ������� � ����������� � �������� �����������
    static void TuneTexts();
};
