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
    // Сделать символ активным - будет в итоговом шрифте
    void Enable();
    // Сделать символ неактивным - не будет в итоговом шрифте
    void Disable();

private:

    wxImage image;
    // Если true, символ будет включён в итоговый шрифт
    bool enabled = true;

    const wxPen *GetPen();
    const wxBrush *GetBrush();
};


class BitmapFont
{
public:
    ~BitmapFont();
    /// Размер символа в точках
    wxSize size = { 0, 0 };
    /// Столько писелей занимает одна точка шрифта в люому направлении
    int pixelsInPoint = 8;

    BitmapSymbol symbols[256];
    /// Создаёт новый шрифт
    void CreateNew();

    void Resize();

    void Draw(wxPaintDC &dc);
    /// Очистить символы, не входящие в основной набор
    void ClearBadSymbols();

    BitmapSymbol *GetSymbol(int row, int col);

private:

    wxBitmap *bitmap = nullptr;
};
