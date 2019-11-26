#pragma once
#pragma warning(push, 0)
#include <wx/wx.h>
#pragma warning(pop)
#include <vector>


struct Symbol
{
    ~Symbol();
    int width;
    int height;

    static wxString UTFfrom1251(uint8 code);

    wxBitmap *bitmap = nullptr;

    void Build(const wxFont &font, uint8 number, int w, int h, int offsetX, int offsetY);
    
    void Clear();

    void Draw(wxMemoryDC &dc, int x, int y);

    void Resize(int scale);

private:

    wxImage image;
};


class Font
{
public:
    ~Font();
    /// Размер символа в точках
    wxSize size = { 0, 0 };
    /// Столько писелей занимает одна точка шрифта в люому направлении
    int pixelsInPoint = 8;

    Symbol symbols[256];
    /// Создаёт новый шрифт
    void CreateNew();

    void Resize();

    void Draw(wxPaintDC &dc);
    /// Очистить символы, не входящие в основной набор
    void ClearBadSymbols();

private:

    wxBitmap *bitmap = nullptr;
};
