#pragma once
#pragma warning(push, 0)
#include <wx/wx.h>
#pragma warning(pop)
#include <vector>


struct BitmapSymbol
{
    BitmapSymbol() : width(0), height(0) {};
    ~BitmapSymbol();
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


class BitmapFont
{
public:
    ~BitmapFont();
    /// ������ ������� � ������
    wxSize size = { 0, 0 };
    /// ������� ������� �������� ���� ����� ������ � ����� �����������
    int pixelsInPoint = 8;

    BitmapSymbol symbols[256];
    /// ������ ����� �����
    void CreateNew();

    void Resize();

    void Draw(wxPaintDC &dc);
    /// �������� �������, �� �������� � �������� �����
    void ClearBadSymbols();

private:

    wxBitmap *bitmap = nullptr;
};
