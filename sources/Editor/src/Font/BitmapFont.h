#pragma once
#pragma warning(push, 0)
#include <wx/wx.h>
#pragma warning(pop)
#include <vector>


class BitmapSymbol
{
public:
    ~BitmapSymbol();
    int width = 0;
    int height = 0;

    static wxString UTFfrom1251(uint8 code);

    wxBitmap *bitmap = nullptr;

    void Build(const wxFont &font, uint8 number, int w, int h, int offsetX, int offsetY);
    
    void Clear();

    void Draw(wxMemoryDC &dc, int x, int y);

    void Resize(int scale);
    // ������� ������ �������� - ����� � �������� ������
    void Enable();
    // ������� ������ ���������� - �� ����� � �������� ������
    void Disable();

private:

    wxImage image;
    // ���� true, ������ ����� ������� � �������� �����
    bool enabled = true;

    const wxPen *GetPen();
    const wxBrush *GetBrush();
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

    BitmapSymbol *GetSymbol(int row, int col);

private:

    wxBitmap *bitmap = nullptr;
};
