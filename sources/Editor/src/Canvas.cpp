#include "defines.h"
#include "Canvas.h"
#include "Font.h"
#include <cstdlib>


Canvas *TheCanvas = nullptr;


static Font font;

static wxCoord mouseX;
static wxCoord mouseY;


/// Подсветить клетку, на которую указывает мышь
static void HighlightCell(wxPaintDC &dc);


Canvas::Canvas(wxWindow *parent) : wxPanel(parent, wxID_ANY)
{
    SetDoubleBuffered(true);

    Bind(wxEVT_PAINT,        &Canvas::OnPaint,      this);
    Bind(wxEVT_MOUSEWHEEL,   &Canvas::OnMouseWheel, this);
    Bind(wxEVT_MOTION,       &Canvas::OnMouseMove,  this);
    Bind(wxEVT_LEAVE_WINDOW, &Canvas::OnMouseLeave, this);

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

    HighlightCell(dc);
}


static void HighlightCell(wxPaintDC &dc)
{
    dc.SetPen(*wxBLACK_PEN);
    dc.SetBrush(*wxBLACK_BRUSH);

    int x = (mouseX / (font.pixelsInPoint)) * (font.pixelsInPoint);

    int y = (mouseY / (font.pixelsInPoint)) * (font.pixelsInPoint);

    dc.DrawRectangle(x, y, font.pixelsInPoint, font.pixelsInPoint);
}


void Canvas::OnMouseWheel(wxMouseEvent &event) //-V2009
{
    if (event.GetWheelRotation() > 0)
    {
        Increase();
    }
    else
    {
        Decrease();
    }

    Refresh();
}


void Canvas::OnMouseMove(wxMouseEvent &event) //-V2009
{
    event.GetPosition(&mouseX, &mouseY);

    Refresh();
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


void Canvas::OnMouseLeave(wxMouseEvent &)
{
    mouseX = mouseY = -100;
    Refresh();
}
