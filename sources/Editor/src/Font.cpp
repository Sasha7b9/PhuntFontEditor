#include "defines.h"
#include "Font.h"


Symbol::Symbol(int w, int h) : width(w), height(h)
{
    bits.resize(static_cast<uint>(height));

    for(int row = 0; row < height; row++)
    {
        bits[static_cast<uint>(row)].resize(static_cast<uint>(width));
    }
}


Symbol::~Symbol()
{
    if (bitmap)
    {
        delete bitmap;
    }
}


void Symbol::Set(int row, int col, uint8 value)
{
    bits[static_cast<uint>(row)][static_cast<uint>(col)] = value;
}


uint8 Symbol::Get(int row, int col) const
{
    return bits[static_cast<uint>(row)][static_cast<uint>(col)];
}


wxString Symbol::UTFfrom1251(uint8 code)
{
    if(code >= 192)
    {
        return wxChar(0x0410 + (code - 192));
    }

    if(code == 168)
    {
        return wxChar(0x0401);
    }
    
    if(code == 184)
    {
        return wxChar(0x0451);
    }

    return wxString::Format("%c", code);
}


void Symbol::Build(const wxFont &font, uint8 number)
{
    if (bitmap)
    {
        delete bitmap;
    }

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

    wxColour color;

    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            memDC.GetPixel({ col, row }, &color);

            if (color.Red() == 0x00)
            {
                Set(row, col, 1);
            }
            else
            {
                Set(row, col, 0);
            }
        }
    }

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

    if (bitmap)
    {
        delete bitmap;
    }

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