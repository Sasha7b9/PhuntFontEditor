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
    
    // Сформировать строку описания шрифта
    static void TuneTextFont();
    
    // Установить все полагающиеся надписи в соотвествии с текущими настройками
    static void TuneTexts();

    static wxFont font;         // Импортируемый шрифт  
    static long offsetSymbolX;  // Смещение шрифта по горизонтали
    static long offsetSymbolY;  // Смещение шрифта по вертикали
    static long widthCell;      // Ширина символа
    static long heightCell;     // Высота символа
};
