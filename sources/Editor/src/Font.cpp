#include "defines.h"
#include "Font.h"


Symbol::Symbol(int w, int h) : width(w), height(h)
{
}


Symbol::~Symbol()
{
    delete bitmap;
}


wxString Symbol::UTFfrom1251(uint8 code)
{
    if(code >= 192)
    {
        return static_cast<wxChar>(0x0410 + code - 192);
    }

    if(code == 168)
    {
        return static_cast<wxChar>(0x0401);
    }
    
    if(code == 184)
    {
        return static_cast<wxChar>(0x0451);
    }

    return wxString::Format("%c", code);
}


void Symbol::Build(const wxFont &font, uint8 number)
{
    delete bitmap;

    bitmap = new wxBitmap(width, height);

    wxMemoryDC memDC;

    memDC.SetFont(font);

    memDC.SelectObject(*bitmap);

    wxPen pen(wxColour(0xff, 0xff, 0xff));
    wxBrush brush(wxColour(0xff, 0xff, 0xff));

    memDC.SetPen(pen);
    memDC.SetBrush(brush);

    memDC.Clear();

    memDC.SetPen(*wxBLACK_PEN);

    memDC.DrawText(Symbol::UTFfrom1251(number), { 0, 0 });

    memDC.SelectObject(wxNullBitmap);
}


void Symbol::Draw(wxMemoryDC &dc, int x, int y)
{
    dc.DrawBitmap(image, x, y);
}


void Symbol::Resize(int scale)
{
    image = bitmap->ConvertToImage().Rescale(width * scale, height * scale);
}


void Font::Resize()
{
    for (int i = 0; i < 256; i++)
    {
        symbols[i].Resize(pixelsInPoint);
    }
}


void Font::Rebuild(const wxFont *f)
{
    if (f)
    {
        font = *f;
    }

    for (int i = 0; i < 256; i++)
    {
        symbols[i].Build(font, static_cast<uint8>(i));
    }

    Resize();

    delete bitmap;

    bitmap = new wxBitmap(16 * size.x * pixelsInPoint, 16 * size.y * pixelsInPoint);

    wxMemoryDC dc;
    dc.SelectObject(*bitmap);
    int num = 0;

    for (int row = 0; row < 16; row++)
    {
        for (int col = 0; col < 16; col++)
        {
            int x0 = col * size.x * pixelsInPoint;
            int y0 = row * size.y * pixelsInPoint;

            symbols[num++].Draw(dc, x0, y0);
        }
    }
}


void Font::Draw(wxPaintDC &dc)
{
    dc.DrawBitmap(*bitmap, 0, 0);
}


Font::~Font()
{
    delete bitmap;
}