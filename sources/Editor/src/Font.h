#pragma once
#pragma warning(push, 0)
#include <wx/wx.h>
#pragma warning(pop)
#include <vector>


#define SYMBOL_WIDTH  16
#define SYMBOL_HEIGHT 16


struct Symbol
{
    Symbol(int w = SYMBOL_WIDTH, int h = SYMBOL_HEIGHT);
    ~Symbol();
    int width;
    int height;

    static wxString UTFfrom1251(uint8 code);

    wxBitmap *bitmap = nullptr;

    void Build(const wxFont &font, uint8 number);

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
    wxSize size = { SYMBOL_WIDTH, SYMBOL_HEIGHT };
    /// Столько писелей занимает одна точка шрифта в люому направлении
    int pixelsInPoint = 8;

    Symbol symbols[256];

    wxFont font;

    void Rebuild(const wxFont *font = nullptr);

    void Resize();

    void Draw(wxPaintDC &dc);

private:

    wxBitmap *bitmap = nullptr;
};
