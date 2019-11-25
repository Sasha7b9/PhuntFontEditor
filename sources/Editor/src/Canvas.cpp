#include "defines.h"
#include "Canvas.h"


Canvas *TheCanvas = nullptr;


Canvas::Canvas(wxWindow *parent) : wxPanel(parent, wxID_ANY)
{
    SetSize({ 4000, 2000 });
    SetMinSize({ 4000, 2000 });
    SetDoubleBuffered(true);
    Bind(wxEVT_PAINT, &Canvas::OnPaint, this);
}


void Canvas::OnPaint(wxPaintEvent &)
{
    wxPaintDC dc(this);

    dc.SetPen(*wxBLACK);

    dc.SetBrush(wxBrush(*wxYELLOW_BRUSH));

    dc.DrawRectangle(GetRect());
}
