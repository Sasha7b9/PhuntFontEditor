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

    void Draw(wxMemoryDC &dc, int x, int y);

    void Resize(int scale);

private:

    wxImage image;
};


class Font
{
public:
    ~Font();
    /// ������ ������� � ������
    wxSize size = { 0, 0 };
    /// ������� ������� �������� ���� ����� ������ � ����� �����������
    int pixelsInPoint = 8;

    Symbol symbols[256];

    wxFont font;

    void Rebuild();

    void Resize();

    void Draw(wxPaintDC &dc);

private:

    wxBitmap *bitmap = nullptr;
};
