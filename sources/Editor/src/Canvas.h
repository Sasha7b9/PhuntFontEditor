#pragma once
#pragma warning(push, 0)
#include <wx/wx.h>
#include <wx/textfile.h>
#pragma warning(pop)


class BitmapSymbol;


class Canvas : public wxPanel
{
public:

    Canvas(wxWindow *parent);

    void OnPaint(wxPaintEvent &);

    void Rebuild();
    /// Увеличить масштаб
    void Increase();
    /// Уменьшить масштаб
    void Decrease();
    /// Очистить символы, не входящие в основной набор
    void ClearBadSymbols();

    void ImportFont(wxTextFile &file, const wxString &nameFont);
    /// Включить/выключить режим выбора символов
    void EnableModeSelectSymbols(bool enable);

private:

    void OnMouseMove(wxMouseEvent &);

    void OnMouseLeave(wxMouseEvent &);

    void OnMouseLeftDown(wxMouseEvent &);

    void OnMouseRightDown(wxMouseEvent &);

    void OnMouseRightUp(wxMouseEvent &);

    void TuneScrollBar();

    void Resize();

    void DrawCursor(wxPaintDC &dc);
    /// Подсветить точку символа, на которую указывает мышь
    void HighlightPixel(wxPaintDC &dc);
    /// Выделить/снять выделение с символа, который находчится под курсором
    void HighlightSymbol(wxPaintDC &dc);
    /// Действующий размер холста
    wxSize CurrentSize();

    BitmapSymbol *GetSymbolUnderMouse(int mouseX, int mouseY);

    wxScrolledWindow *sw = nullptr;

    enum Mode
    {
        Edit,           ///< Основной режим, в котором можно редактировать символы
        SelectSymbols   ///< Режим выбора символов
    } mode = Edit;
};


extern Canvas *TheCanvas;
