#include "defines.h"
#include "Canvas.h"
#include <cstdlib>


Canvas *TheCanvas = nullptr;


static const int PIXEL_SIZE = 8;

static const wxSize MIN_SIZE = { PIXEL_SIZE * 4 * 16, PIXEL_SIZE * 4 * 16 };

static const wxSize MAX_SIZE = MIN_SIZE * 2;


Canvas::Canvas(wxWindow *parent) : wxPanel(parent, wxID_ANY)
{
    SetSize(MIN_SIZE);
    SetDoubleBuffered(true);

    Bind(wxEVT_PAINT,      &Canvas::OnPaint, this);
    Bind(wxEVT_MOUSEWHEEL, &Canvas::OnMouse, this);

    wxFont font(11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Courier New"));
    SetFont(font);
}


void Canvas::OnPaint(wxPaintEvent &)
{
    wxPaintDC dc(this);

    dc.SetPen(*wxBLACK);

    dc.SetBrush(wxBrush(*wxYELLOW_BRUSH));

    dc.DrawRectangle(GetRect());

    dc.DrawText(wxT("Test string. Тестовая строка."), { 0, 0 });




//    for (int i = 0; i < 1000; i++)
//    {
//        wxSize size = { std::rand() % 30, std::rand() % 30 };
//
//        wxPoint point = { std::rand() % 3000, std::rand() % 1500 };
//
//        dc.DrawRectangle(point, size);
//    }
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
    if (GetSize().x < MAX_SIZE.x)
    {
        SetSize(GetSize() * 2);
    }
}


void Canvas::Decrease()
{
    if (GetSize().x > MIN_SIZE.x)
    {
        SetSize(GetSize() / 2);
    }
}
