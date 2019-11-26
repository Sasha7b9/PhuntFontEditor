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
    /// Увеличить масштаб
    void Increase();
    /// Уменьшить масштаб
    void Decrease();

private:

    void OnMouseWheel(wxMouseEvent &);

    void OnMouseMove(wxMouseEvent &);

    void OnMouseLeave(wxMouseEvent &);

    void OnMouseRightDown(wxMouseEvent &);

    void OnMouseRightUp(wxMouseEvent &);

    void TuneScrollBar();

    wxScrolledWindow *sw = nullptr;

    void Resize();
};


extern Canvas *TheCanvas;
