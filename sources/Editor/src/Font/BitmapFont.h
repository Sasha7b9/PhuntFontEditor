#pragma once
#pragma warning(push, 0)
#include <wx/wx.h>
#pragma warning(pop)
#include <vector>


class BitmapSymbol
{
    friend class SymbolImp;
    friend class FontImporter;
public:
    ~BitmapSymbol();
    int width = 0;
    int height = 0;

    void Build(const wxFont &font, uint8 number, int w, int h, int offsetX, int offsetY);
    
    void Clear();

    void Draw(wxMemoryDC &dc, int x, int y);

    void Resize(int scale);
    
    // ������� ������ �������� - ����� � �������� ������
    void Enable();
    
    // ������� ������ ���������� - �� ����� � �������� ������
    void Disable();

    void ToggleState();

    static BitmapSymbol *Null();

private:
    
    // ����� �������� �������� ����������� � �������� 1:1
    wxBitmap *bmpSymbol = nullptr;
    
    // ������� � ��������� �����������
    wxImage imgDrawable;
    
    // ���� true, ������ ����� ������� � �������� �����
    bool enabled = true;

    static wxString UTFfrom1251(uint8 code);
};


class BitmapFont
{
public:
    ~BitmapFont();
    
    // ������ ������� � ������
    wxSize size = { 0, 0 };
    
    // ������� ������� �������� ���� ����� ������ � ����� �����������
    int scale = 8;

    BitmapSymbol symbols[16][16];
    
    // ������ ����� �����
    void CreateNew();

    void Resize();

    void Draw(wxPaintDC &dc);
    
    // �������� �������, �� �������� � �������� �����
    void ClearBadSymbols();

    BitmapSymbol *GetSymbol(int row, int col);

    BitmapSymbol *GetSymbol(uint8 num);
    
    // ��������� �������������� ������� �� ������
    wxRect GetRectForSymbol(const BitmapSymbol *symbol) const;
    
    // ����������� ��������� ������� � ������� ���� � ������������ mouseX, mouseY
    void ToggleStateSymbol(int mouseX, int mouseY);

    BitmapSymbol *GetSymbolUnderMouse(int x, int y);

    void GetSymbolXY(const BitmapSymbol *symbol, int *x, int *y);

private:

    wxBitmap *bitmap = nullptr;
};
