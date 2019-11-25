#pragma once
#pragma warning(push, 0)
#include <wx/wx.h>
#pragma warning(pop)
#include <vector>


struct Symbol
{
    Symbol(uint w = 8, uint h = 8);
    void Set(int row, int col, uint8 value);
    uint width;
    uint height;
    /// Здесь каждый бит представлен 8-битным значением
    std::vector<std::vector<uint8>> bits;
};


class Font
{
public:

    /// Размер символа в точках
    wxSize size = { 8, 8 };
    /// Столько писелей занимает одна точка шрифта в люому направлении
    int pixelsInPoint = 8;

    Symbol symbols[256];
};
