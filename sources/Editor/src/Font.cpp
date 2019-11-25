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


wxString Symbol::UTFfromCode(uint8 code)
{
    static const wxString str[] =
    {
        wxT('\x0410'), wxT('\x0411'), wxT('\x0412'), wxT('\x0413'), wxT('\x0414'), wxT('\x0415'), wxT('\x0416'), wxT('\x0417'), wxT('\x0418'), wxT('\x0419'), wxT('\x041a'), wxT('\x041b'), wxT('\x041c'), wxT('\x041d'), wxT('\x041e'), wxT('\x041f'),
        wxT('\x0420'), wxT('\x0421'), wxT('\x0422'), wxT('\x0423'), wxT('\x0424'), wxT('\x0425'), wxT('\x0426'), wxT('\x0427'), wxT('\x0428'), wxT('\x0429'), wxT('\x042a'), wxT('\x042b'), wxT('\x042c'), wxT('\x042d'), wxT('\x042e'), wxT('\x042f'),
        wxT('\x0430'), wxT('\x0431'), wxT('\x0432'), wxT('\x0433'), wxT('\x0434'), wxT('\x0435'), wxT('\x0436'), wxT('\x0437'), wxT('\x0438'), wxT('\x0439'), wxT('\x043a'), wxT('\x043b'), wxT('\x043c'), wxT('\x043d'), wxT('\x043e'), wxT('\x043f'),
        wxT('\x0440'), wxT('\x0441'), wxT('\x0442'), wxT('\x0443'), wxT('\x0444'), wxT('\x0445'), wxT('\x0446'), wxT('\x0447'), wxT('\x0448'), wxT('\x0449'), wxT('\x044a'), wxT('\x044b'), wxT('\x044c'), wxT('\x044d'), wxT('\x044e'), wxT('\x044f')
    };

    if(code >= 192)
    {
        return str[code - 192];
    }

    if(code == 168)
    {
        return wxString(wxT('\0x0401'));
    }

    if(code == 184)
    {
        return wxString(wxT('\0x0451'));
    }

    return wxString::Format("%c", code);
}
