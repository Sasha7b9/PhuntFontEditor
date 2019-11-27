#include "defines.h"
#include "Font/FontImporter.h"


#define ADD_LINE(x)            file.AddLine(x)
#define ADD_FLINE_1(s, x)      file.AddLine(wxString::Format(s, x))
#define ADD_FLINE_2(s, x1, x2) file.AddLine(wxString::Format(s, x1, x2))


BitmapFont *FontImporter::font = nullptr;


/// Структура символа для импортёра
class SymbolImp
{
public:
    SymbolImp(BitmapSymbol *s) : symbol(s)
    {
        CreateBits();
    };
    /// Функция заполняем массив bits. При этом удаляются пустые начальные биты строк и пустые конечные биты строк
    void CreateBits();
    /// Возвращает размер соответствующего символа BitmapSymbol
    int GetSize() const;
private:
    /// Обрабатывемый символ
    BitmapSymbol *symbol;
    /// Здесь хранятся биты символа
    std::vector<std::vector<uint8>> bits;
    /// Найти позицию первого ненулевого бита в символе
    int FindPositionFirstBit() const;
    /// Найти позицию последнего ненулевого бита в символе
    int FindPositionLastBit() const;
    /// Возвращает высоту символа
    int GetHeight() const;
    /// Возвращает ширину символа
    int GetWidth() const;
};


void FontImporter::Import(BitmapFont *f, wxTextFile &file, const wxString &nameFont)
{
    font = f;

    uint16 offsets[256];        // Здесь смещения всех символов

    int sizes[256];             // Здесь размеры всех символов

    CalculateSizes(sizes);

    CalculateOffsets(sizes, offsets);

    WriteFont(file, nameFont, sizes, offsets);
}


void FontImporter::WriteFont(wxTextFile &file, const wxString &nameFont, const int sizes[256], const uint16 offsets[256])
{
    ADD_FLINE_1("unsigned int %s[] =", nameFont);
    ADD_LINE("{");

    for (int i = 0; i < 256; i++)
    {
        ADD_FLINE_2("    %d %d", sizes[i], offsets[i]);
    }
}


void FontImporter::CalculateSizes(int *sizes)
{
    int i = 0;

    for (int row = 0; row < 16; row++)
    {
        for (int col = 0; col < 16; col++)
        {
            SymbolImp symbol(&font->symbols[row][col]);

            sizes[i++] = symbol.GetSize();
        }
    }
}


void FontImporter::CalculateOffsets(const int sizes[256], uint16 offsets[256])
{
    int lastOffset = 256;        // Это смещение будет у следующего непустого символа

    for (int i = 0; i < 256; i++)
    {
        if (sizes[i] == 0)
        {
            offsets[i] = 0;
        }
        else
        {
            offsets[i] = static_cast<uint16>(lastOffset);
            lastOffset += sizes[i];

            if (lastOffset > 0xFFFF)
            {
                std::cout << "!!! ERROR !!! Not enough bit depth." << std::endl;
            }
        }
    }
}


int SymbolImp::GetSize() const
{
    if (!symbol->enabled)
    {
        return 0;
    }

    int first = FindPositionFirstBit();
    int last = FindPositionLastBit();

    int bitsInRow = last - first + 1;

    int bytesInRow = bitsInRow / 8;

    if (bitsInRow % 8)
    {
        bytesInRow++;
    }
    //                                 width   height  bytesInRow
    return GetHeight() * bytesInRow +    1   +   1   +    1;
}


void SymbolImp::CreateBits()
{
    wxMemoryDC dc;
    dc.SelectObject(*symbol->bmpSymbol);

    bits.resize(static_cast<uint>(symbol->bmpSymbol->GetHeight()));

    for (int row = 0; row < symbol->bmpSymbol->GetHeight(); row++)
    {
        for (int col = 0; col < symbol->bmpSymbol->GetWidth(); col++)
        {
            wxColour color;

            dc.GetPixel(row, col, &color);

            bits[static_cast<uint>(row)].push_back(color.Red() ? 1U : 0U);
        }
    }
}


int SymbolImp::FindPositionFirstBit() const
{
    int result = INT_MAX;

    for (int row = 0; row < GetHeight(); row++)
    {
        for (int col = 0; col < GetWidth(); col++)
        {
            if (bits[static_cast<uint>(row)][static_cast<uint>(col)])
            {
                if (col < result)
                {
                    result = col;
                }
                if (result == 0)
                {
                    return result;
                }
                break;
            }
        }
    }

    return (result == INT_MAX) ? GetWidth() - 1 : result;
}


int SymbolImp::FindPositionLastBit() const
{
    int result = 0;

    for (int row = 0; row < GetHeight(); row++)
    {
        for (int col = GetWidth() - 1; col >= 0; col--)
        {
            if (bits[static_cast<uint>(row)][static_cast<uint>(col)])
            {
                if (col > result)
                {
                    result = col;
                }
                if (result == GetWidth() - 1)
                {
                    return result;
                }
                break;
            }
        }
    }

    return result;
}


int SymbolImp::GetHeight() const
{
    return static_cast<int>(bits.size());
}


int SymbolImp::GetWidth() const
{
    return static_cast<int>(bits[0].size());
}
