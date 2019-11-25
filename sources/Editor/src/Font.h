#pragma once
#pragma warning(push, 0)
#include <wx/wx.h>
#pragma warning(pop)


class Font
{
public:

    /// Размер символа в точках
    wxSize size = { 8, 8 };
    /// Столько писелей занимает одна точка шрифта в люому направлении
    int pixelsInPoint = 8;
};
