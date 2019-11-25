#include "defines.h"
#include "Canvas.h"
#include "Font.h"
#include <cstdlib>


Canvas *TheCanvas = nullptr;


static Font font;


Canvas::Canvas(wxWindow *parent) : wxPanel(parent, wxID_ANY)
{
    SetDoubleBuffered(true);

    Bind(wxEVT_PAINT,      &Canvas::OnPaint, this);
    Bind(wxEVT_MOUSEWHEEL, &Canvas::OnMouse, this);

    SetFont(wxFont(11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Courier New")));
}


void Canvas::OnPaint(wxPaintEvent &)
{
    SetSize({ 16 * font.size.x * font.pixelsInPoint + 1, 16 * font.size.y * font.pixelsInPoint + 1 });

    wxPaintDC dc(this);

    dc.SetPen(*wxBLACK);

    dc.SetBrush(wxBrush(wxColor(0xF0, 0xF0, 0xF0)));

    dc.DrawRectangle(GetRect());

    dc.DrawText(wxT("Test string. Тестовая строка."), { 0, 0 });

    dc.SetPen(wxPen(wxColor(0xa0, 0xa0, 0xa0)));

    for (int i = 0; i < GetSize().x; i += font.pixelsInPoint)
    {
        dc.DrawLine(i, 0, i, GetSize().y);
    }

    for (int i = 0; i < GetSize().y; i += font.pixelsInPoint)
    {
        dc.DrawLine(0, i, GetSize().x, i);
    }

    dc.SetPen(*wxBLACK);

    
    for (int i = 0; i < GetSize().x; i += font.size.x * font.pixelsInPoint)
    {
        dc.DrawLine(i, 0, i, GetSize().y);
    }

    for (int i = 0; i < GetSize().y; i += font.size.y * font.pixelsInPoint)
    {
        dc.DrawLine(0, i, GetSize().x, i);
    }
}


void Canvas::OnMouse(wxMouseEvent &event)
{
    if (event.GetWheelRotation() > 0)
    {
        Increase();
    }
    else
    {
        Decrease();
    }
}


void Canvas::Increase()
{
    if (font.pixelsInPoint < 32)
    {
        font.pixelsInPoint++;
    }
}


void Canvas::Decrease()
{
    if (font.pixelsInPoint > 2)
    {
        font.pixelsInPoint--;
    }
}
