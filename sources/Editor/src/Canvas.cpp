#include "defines.h"
#include "Canvas.h"
#include "Font.h"
#include <cstdlib>


Canvas *TheCanvas = nullptr;


static Font font;

static wxCoord mouseX;
static wxCoord mouseY;


/// ѕодсветить клетку, на которую указывает мышь
static void HighlightCell(wxPaintDC &dc);
/// ƒействующий размер холста
static wxSize CurrentSize();


Canvas::Canvas(wxWindow *parent) : wxPanel(parent, wxID_ANY)
{
    sw = static_cast<wxScrolledWindow *>(parent);

    SetDoubleBuffered(true);

    Bind(wxEVT_PAINT,        &Canvas::OnPaint,      this);
    Bind(wxEVT_MOUSEWHEEL,   &Canvas::OnMouseWheel, this);
    Bind(wxEVT_MOTION,       &Canvas::OnMouseMove,  this);
    Bind(wxEVT_LEAVE_WINDOW, &Canvas::OnMouseLeave, this);

    TuneScrollBar();
}


void Canvas::OnPaint(wxPaintEvent &)
{
    SetSize(CurrentSize());

    wxPaintDC dc(this);

    dc.SetPen(*wxBLACK);

    dc.SetBrush(wxBrush(wxColor(0xF0, 0xF0, 0xF0)));

    dc.DrawRectangle(GetRect());

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

    DrawSymbols(dc);

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

    TuneScrollBar();

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


static wxSize CurrentSize()
{
    return { 16 * font.size.x * font.pixelsInPoint + 1, 16 * font.size.y * font.pixelsInPoint + 1 };
}


void Canvas::TuneScrollBar()
{
    wxSize size = CurrentSize();

    sw->SetScrollbars(1, 1, size.x, size.y);

    SetPosition({ 0, 0 });
}


void Canvas::OnMouseRightDown(wxMouseEvent &)
{

}


void Canvas::OnMouseRightUp(wxMouseEvent &)
{

}


void Canvas::Rebuild(const wxFont &f)
{
    for(int i = 0; i < 256; i++)
    {
        BuildSymbol(f, static_cast<uint8>(i));
    }
}


void Canvas::BuildSymbol(const wxFont &f, uint8 s)
{
    wxMemoryDC memDC;

    memDC.SetFont(f);

    wxBitmap bitmap(font.size.x, font.size.y);

    memDC.SelectObject(bitmap);

    wxPen pen(wxColour(0xff, 0xff, 0xff));
    wxBrush brush(wxColour(0xff, 0xff, 0xff));

    memDC.SetPen(pen);
    memDC.SetBrush(brush);

    memDC.Clear();

    memDC.SetPen(*wxBLACK_PEN);

    memDC.DrawText(wxString::Format("%c", s), { 0, 0 });

    Symbol &symbol = font.symbols[s];

    wxColour color;

    for(int row = 0; row < font.size.y; row++)
    {
        for(int col = 0; col < font.size.x; col++)
        {
            memDC.GetPixel({ col, row }, &color);

            if(color.Red() == 0x00)
            {
                symbol.Set(row, col, 1);
            }
            else
            {
                symbol.Set(row, col, 0);
            }
        }
    }
}


void Canvas::DrawSymbols(wxPaintDC &dc)
{
    int numSymbol = 0;

    for(int row = 0; row < 16; row++)
    {
        for(int col = 0; col < 16; col++)
        {
            DrawSymbol(dc, row, col, numSymbol++);
        }
    }
}


void Canvas::DrawSymbol(wxPaintDC &dc, int row, int col, int num)
{
    Symbol &symbol = font.symbols[num];

    int step = font.pixelsInPoint;

    int x0 = col * font.size.x * step;
    int y0 = row * font.size.y * step;

    dc.SetPen(*wxBLACK_PEN);
    dc.SetBrush(*wxBLACK_BRUSH);

    for(int y = 0; y < font.size.y; y++)
    {
        for(int x = 0; x < font.size.x; x++)
        {
            if(symbol.Get(y, x))
            {
                dc.DrawRectangle(x0 + x * step, y0 + y * step, step, step);
            }
        }
    }
}
