#include "defines.h"
#include "Dialogs/ImportDialog.h"
#include "Font/BitmapFont.h"



BitmapSymbol::~BitmapSymbol()
{
    delete bmpSymbol;
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

    delete bmpSymbol;

    bmpSymbol = new wxBitmap(width, height);

    wxMemoryDC memDC;

    memDC.SetFont(font);

    memDC.SelectObject(*bmpSymbol);

    memDC.SetPen(wxPen(*wxWHITE_PEN));
    memDC.SetBrush(wxBrush(*wxWHITE_BRUSH));

    memDC.Clear();

    memDC.SetPen(*wxBLACK_PEN);

    memDC.DrawText(BitmapSymbol::UTFfrom1251(number), { offsetX, offsetY });

    memDC.SelectObject(wxNullBitmap);
}


void BitmapSymbol::Clear()
{
    delete bmpSymbol;
    bmpSymbol = new wxBitmap(width, height);

    wxMemoryDC memDC;
    memDC.SelectObject(*bmpSymbol);

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
    image = bmpSymbol->ConvertToImage().Rescale(width * scale, height * scale);
}


void BitmapFont::Resize()
{
    for (int row = 0; row < 16; row++)
    {
        for (int col = 0; col < 16; col++)
        {
            symbols[row][col].Resize(pixelsInPoint);
        }
    }

    delete bitmap;

    bitmap = new wxBitmap(16 * size.x * pixelsInPoint, 16 * size.y * pixelsInPoint);

    wxMemoryDC dc;
    dc.SelectObject(*bitmap);
    for (int row = 0; row < 16; row++)
    {
        for (int col = 0; col < 16; col++)
        {
            int x0 = col * size.x * pixelsInPoint;
            int y0 = row * size.y * pixelsInPoint;

            symbols[row][col].Draw(dc, x0, y0);
        }
    }
}


void BitmapFont::CreateNew()
{
    DataImport data;
    ImportDialog::GetDataImport(data);

    size.x = data.width;
    size.y = data.height;

    int i = 0;

    for (int row = 0; row < 16; row++)
    {
        for (int col = 0; col < 16; col++)
        {
            symbols[row][col].Build(data.font, static_cast<uint8>(i++), size.x, size.y, data.offsetX, data.offsetY);
        }
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
    for (uint8 i = 0; i < 0x20; i++)
    {
        GetSymbol(i)->Disable();
        GetSymbol(i)->Clear();
    }

    for (uint8 i = 0x7f; i < 0xc0; i++)
    {
        GetSymbol(i)->Disable();
        GetSymbol(i)->Clear();
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


void BitmapSymbol::ToggleState()
{
    enabled = !enabled;
}


BitmapSymbol *BitmapFont::GetSymbol(int row, int col)
{
    return &symbols[row][col];
}


BitmapSymbol *BitmapFont::GetSymbol(uint8 num)
{
    BitmapSymbol *symbol = &symbols[0][0];

    return &symbol[num];
}


wxRect BitmapFont::GetRectForSymbol(const BitmapSymbol *symbol) const
{
    for (int row = 0; row < 16; row++)
    {
        for (int col = 0; col < 16; col++)
        {
            if (&symbols[row][col] == symbol)
            {
                wxRect rect;

                rect.x = col * size.x * pixelsInPoint;
                rect.y = row * size.y * pixelsInPoint;
                rect.width = size.x * pixelsInPoint;
                rect.height = size.y * pixelsInPoint;

                return rect;
            }
        }
    }

    return wxRect();
}


BitmapSymbol *BitmapSymbol::Null()
{
    static BitmapSymbol symbol;
    return &symbol;
}
