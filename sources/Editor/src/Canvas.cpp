#include "defines.h"
#include "Canvas.h"


Canvas *TheCanvas = nullptr;


Canvas::Canvas(wxWindow *parent) : wxPanel(parent, wxID_ANY)
{
    SetDoubleBuffered(true);
    Bind(wxEVT_PAINT, &Canvas::OnPaint, this);
}


void Canvas::OnPaint(wxPaintEvent &)
{
    wxPaintDC dc(this);

    dc.SetPen(*wxBLACK);

    dc.SetBrush(wxBrush(*wxGREY_BRUSH));

    dc.DrawRectangle(GetRect());
}
