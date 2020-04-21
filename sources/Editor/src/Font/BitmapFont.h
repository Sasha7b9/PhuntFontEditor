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
    
    // Сделать символ активным - будет в итоговом шрифте
    void Enable();
    
    // Сделать символ неактивным - не будет в итоговом шрифте
    void Disable();

    void ToggleState();

    static BitmapSymbol *Null();

private:
    
    // Здесь хранится исходное изображение в масштабе 1:1
    wxBitmap *bmpSymbol = nullptr;
    
    // Готовое к отрисовке изображение
    wxImage imgDrawable;
    
    // Если true, символ будет включён в итоговый шрифт
    bool enabled = true;

    static wxString UTFfrom1251(uint8 code);
};


class BitmapFont
{
public:
    ~BitmapFont();
    
    // Размер символа в точках
    wxSize size = { 0, 0 };
    
    // Столько писелей занимает одна точка шрифта в люому направлении
    int scale = 8;

    BitmapSymbol symbols[16][16];
    
    // Создаёт новый шрифт
    void CreateNew();

    void Resize();

    void Draw(wxPaintDC &dc);
    
    // Очистить символы, не входящие в основной набор
    void ClearBadSymbols();

    BitmapSymbol *GetSymbol(int row, int col);

    BitmapSymbol *GetSymbol(uint8 num);
    
    // Возращает местоположение символа на холсте
    wxRect GetRectForSymbol(const BitmapSymbol *symbol) const;
    
    // Переключить состояине символа в позиции мыши с координатами mouseX, mouseY
    void ToggleStateSymbol(int mouseX, int mouseY);

    BitmapSymbol *GetSymbolUnderMouse(int x, int y);

    void GetSymbolXY(const BitmapSymbol *symbol, int *x, int *y);

private:

    wxBitmap *bitmap = nullptr;
};
