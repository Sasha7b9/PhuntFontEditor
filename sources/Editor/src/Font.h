#pragma once
#pragma warning(push, 0)
#include <wx/wx.h>
#pragma warning(pop)
#include <vector>


#define SYMBOL_WIDTH  8
#define SYMBOL_HEIGHT 8


struct Symbol
{
    Symbol(int w = SYMBOL_WIDTH, int h = SYMBOL_HEIGHT);
    ~Symbol();
    uint8 Get(int row, int col) const;
    int width;
    int height;
    /// Здесь каждый бит представлен 8-битным значением
    std::vector<std::vector<uint8>> bits;

    static wxString UTFfrom1251(uint8 code);

    wxBitmap *bitmap = nullptr;

    void Build(const wxFont &font, uint8 number);

    void Draw(wxPaintDC &dc, int x, int y);

    void Resize(int scale);

private:
    void Set(int row, int col, uint8 value);

    wxImage image;
};


class Font
{
public:

    /// Размер символа в точках
    wxSize size = { SYMBOL_WIDTH, SYMBOL_HEIGHT };
    /// Столько писелей занимает одна точка шрифта в люому направлении
    int pixelsInPoint = 8;

    Symbol symbols[256];

    wxFont font;

    void Rebuild(const wxFont *font = nullptr);

    void Resize();

    void Draw(wxPaintDC &dc);
};
