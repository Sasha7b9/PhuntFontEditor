#include "defines.h"
#include "Dialogs/ImportDialog.h"
#include "Font/BitmapFont.h"



BitmapSymbol::~BitmapSymbol()
{
    delete bitmap;
}


wxString BitmapSymbol::UTFfrom1251(uint8 code)
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


void BitmapSymbol::Build(const wxFont &font, uint8 number, int w, int h, int offsetX, int offsetY)
{
    width = w;
    height = h;

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

    memDC.DrawText(BitmapSymbol::UTFfrom1251(number), { offsetX, offsetY });

    memDC.SelectObject(wxNullBitmap);
}


void BitmapSymbol::Clear()
{
    delete bitmap;
    bitmap = new wxBitmap(width, height);

    wxMemoryDC memDC;
    memDC.SelectObject(*bitmap);

    memDC.SetPen(*GetPen());
    memDC.SetBrush(*GetBrush());

    memDC.DrawRectangle(0, 0, width, height);
}


const wxPen *BitmapSymbol::GetPen()
{
    return enabled ? wxWHITE_PEN : wxGREY_PEN;
}


const wxBrush *BitmapSymbol::GetBrush()
{
    return enabled ? wxWHITE_BRUSH : wxGREY_BRUSH;
}


void BitmapSymbol::Draw(wxMemoryDC &dc, int x, int y)
{
    dc.DrawBitmap(image, x, y);
}


void BitmapSymbol::Resize(int scale)
{
    image = bitmap->ConvertToImage().Rescale(width * scale, height * scale);
}


void BitmapFont::Resize()
{
    for (int i = 0; i < 256; i++)
    {
        symbols[i].Resize(pixelsInPoint);
    }

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


void BitmapFont::CreateNew()
{
    DataImport data;
    ImportDialog::GetDataImport(data);

    size.x = data.width;
    size.y = data.height;

    for (int i = 0; i < 256; i++)
    {
        symbols[i].Build(data.font, static_cast<uint8>(i), size.x, size.y, data.offsetX, data.offsetY);
    }

    Resize();
}


void BitmapFont::Draw(wxPaintDC &dc)
{
    dc.DrawBitmap(*bitmap, 0, 0);
}


BitmapFont::~BitmapFont()
{
    delete bitmap;
}


void BitmapFont::ClearBadSymbols()
{
    for (int i = 0; i < 0x20; i++)
    {
        symbols[i].Disable();
        symbols[i].Clear();
    }

    for (int i = 0x7f; i < 0xc0; i++)
    {
        symbols[i].Disable();
        symbols[i].Clear();
    }

    Resize();
}


void BitmapSymbol::Disable()
{
    enabled = false;
}


void BitmapSymbol::Enable()
{
    enabled = true;
}


BitmapSymbol *BitmapFont::GetSymbol(int row, int col)
{
    return &symbols[row * 16 + col];
}
