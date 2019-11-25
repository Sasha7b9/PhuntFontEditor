#pragma once
#pragma warning(push, 0)
#include <wx/wx.h>
#pragma warning(pop)





class Canvas : public wxPanel
{
public:

    Canvas(wxWindow *parent);

    void OnPaint(wxPaintEvent &);

private:

    void OnMouseWheel(wxMouseEvent &);

    void OnMouseMove(wxMouseEvent &);

    void OnMouseLeave(wxMouseEvent &);
    /// Увеличить в два раза
    void Increase();
    /// Уменьшить в два раза
    void Decrease();
};


extern Canvas *TheCanvas;
