#pragma once
#pragma warning(push, 0)
#include <wx/wx.h>
#pragma warning(pop)


struct DataImportFont
{
    int width;
    int height;
    int offsetX;
    int offsetY;
    wxFont font;
};


class ImportSystemFontDialog : public wxDialog
{
public:
    ImportSystemFontDialog(const wxString &title);

    static void GetDataImport(DataImportFont *data);
   
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
