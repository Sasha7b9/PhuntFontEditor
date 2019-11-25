#pragma once
#pragma warning(push, 0)
#include <wx/wx.h>
#pragma warning(pop)





class Canvas : public wxPanel
{
public:

    Canvas(wxWindow *parent);

    void OnPaint(wxPaintEvent &);

    void Rebuild(const wxFont &font);

private:

    void OnMouseWheel(wxMouseEvent &);

    void OnMouseMove(wxMouseEvent &);

    void OnMouseLeave(wxMouseEvent &);

    void OnMouseRightDown(wxMouseEvent &);

    void OnMouseRightUp(wxMouseEvent &);

    void BuildSymbol(const wxFont &font, uint8 symbol);
    /// Увеличить в два раза
    void Increase();
    /// Уменьшить в два раза
    void Decrease();

    void TuneScrollBar();

    void DrawSymbols(wxPaintDC &dc);

    void DrawSymbol(wxPaintDC &d, int row, int col, int num);

    wxScrolledWindow *sw = nullptr;
};


extern Canvas *TheCanvas;
