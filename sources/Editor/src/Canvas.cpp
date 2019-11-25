#include "defines.h"
#include "Canvas.h"
#include <cstdlib>


Canvas *TheCanvas = nullptr;


Canvas::Canvas(wxWindow *parent) : wxPanel(parent, wxID_ANY)
{
    SetSize({ 4000, 2000 });
    SetDoubleBuffered(true);
    Bind(wxEVT_PAINT, &Canvas::OnPaint, this);

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
