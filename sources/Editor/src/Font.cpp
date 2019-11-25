#include "defines.h"
#include "Font.h"


Symbol::Symbol(uint w, uint h) : width(w), height(h)
{
    bits.resize(height);

    for(uint row = 0; row < height; row++)
    {
        bits[row].resize(w);
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
