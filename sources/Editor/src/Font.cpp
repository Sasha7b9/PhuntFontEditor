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
