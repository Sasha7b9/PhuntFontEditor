#pragma once
#pragma warning(push, 0)
#include <wx/wx.h>
#pragma warning(pop)
#include <vector>


#define SIZE_FONT 16


struct Symbol
{
    Symbol(uint w = SIZE_FONT, uint h = SIZE_FONT);
    void Set(int row, int col, uint8 value);
    uint8 Get(int row, int col) const;
    uint width;
    uint height;
    /// Здесь каждый бит представлен 8-битным значением
    std::vector<std::vector<uint8>> bits;

    static wxString UTFfrom1251(uint8 code);
};


class Font
{
public:

    /// Размер символа в точках
    wxSize size = { SIZE_FONT, SIZE_FONT };
    /// Столько писелей занимает одна точка шрифта в люому направлении
    int pixelsInPoint = 5;

    Symbol symbols[256];
};
