#include "defines.h"
#include "Font/FontImporter.h"


#define ADD_LINE(x)                file.AddLine(x)
#define ADD_FLINE_1(s, x)          file.AddLine(wxString::Format(s, x))
#define ADD_FLINE_2(s, x1, x2)     file.AddLine(wxString::Format(s, x1, x2))
#define ADD_FLINE_3(s, x1, x2, x3) file.AddLine(wxString::Format(s, x1, x2, x3))


static int gI = 0;


#define TRACE(num)                                          \
if(gI == 0x21)                                              \
{                                                           \
    std::cout << "Point " <<  num <<  std::endl;            \
    std::cout << "width = " << GetWidth() << std::endl;     \
    std::cout << "height = " << GetHeight() << std::endl;   \
    Log();                                                  \
}


/// Структура символа для импортёра
class SymbolImp
{
    friend class FontImporter;
public:
    SymbolImp(BitmapSymbol *s) : symbol(s)
    {
        CreateBits();

        TRACE(1);

        DeleteFirstEmptyBits();

        TRACE(2);

        DeleteLastEmptyBits();

        TRACE(3);

        DeleteBottomEmptyBits();

        TRACE(4);
    };
    /// Возвращает размер соответствующего символа BitmapSymbol
    int GetSize() const;
private:
    /// Обрабатывемый символ
    BitmapSymbol *symbol;
    /// Функция заполняем массив bits. При этом удаляются пустые начальные биты строк и пустые конечные биты строк
    void CreateBits();
    /// Здесь хранятся биты символа
    std::vector<std::vector<uint8>> bits;
    /// Удалить первые "пустые" биты
    void DeleteFirstEmptyBits();
    /// Удалить последние "пустые" биты
    void DeleteLastEmptyBits();
    /// Удалить нижние "пустые" биты
    void DeleteBottomEmptyBits();
    /// Найти позицию первого ненулевого бита в символе
    int FindPositionFirstBit() const;
    /// Найти позицию последнего ненулевого бита в символе
    int FindPositionLastBit() const;
    /// Возвращает высоту символа
    int GetHeight() const;
    /// Возвращает ширину символа
    int GetWidth() const;
    /// Возвращает число бит в строке
    int BitsInRow() const;
    /// Возвращает число байт на строку
    int BytesInRow() const;
    
    void Log() const;
};


static SymbolImp *symbols[256];

/// Возвращает сумму элементов вектора
static int Sum(std::vector<uint8> &vec);


void FontImporter::Import(BitmapFont &font, wxTextFile &file, const wxString &nameFont)
{
    gI = 0;

    uint16 offsets[256];        // Здесь смещения всех символов

    int sizes[256];             // Здесь размеры всех символов

    CreateSymbols(font);

    CalculateSizes(sizes);

    CalculateOffsets(sizes, offsets);

    WriteFont(file, nameFont, sizes, offsets);

    DeleteSymbols();
}


void FontImporter::CreateSymbols(BitmapFont &font)
{
    for (int row = 0; row < 16; row++)
    {
        for (int col = 0; col < 16; col++)
        {
            symbols[gI++] = new SymbolImp(&font.symbols[row][col]);
        }
    }
}


void FontImporter::WriteFont(wxTextFile &file, const wxString &nameFont, const int [256], const uint16 [256])
{
    ADD_FLINE_1("unsigned int %s[] =", nameFont);
    ADD_LINE("{");

    for (int i = 0; i < 256; i++)
    {
        if (symbols[i]->symbol->enabled)
        {
            ADD_FLINE_3(" num = 0x%X, width = %d, height = %d", i, symbols[i]->GetWidth(), symbols[i]->GetHeight());
        }
    }
}


void FontImporter::CalculateSizes(int *sizes)
{
    int i = 0;

    for (int row = 0; row < 16; row++)
    {
        for (int col = 0; col < 16; col++)
        {
            sizes[i] = symbols[i]->GetSize();
            i++;
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
    //                                 width   height  bytesInRow
    return GetHeight() * BytesInRow() +    1   +   1   +    1;
}


int SymbolImp::BitsInRow() const
{
    return FindPositionLastBit() + 1;
}


int SymbolImp::BytesInRow() const
{
    int result = BitsInRow() / 8;

    if (result % 8)
    {
        result++;
    }

    return result;
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

            dc.GetPixel(col, row, &color);

            bits[static_cast<uint>(row)].push_back(color.Red() ? 0U : 1U);
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
        int start = GetWidth() - 1;

        for (int col = start; col >= 0; col--)
        {
            if (bits[static_cast<uint>(row)][static_cast<uint>(col)])
            {
                if (col > result)
                {
                    result = col;
                }
                if (result == start)
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


void FontImporter::DeleteSymbols()
{
    for (int i = 0; i < 256; i++)
    {
        delete symbols[i];
    }
}


void SymbolImp::DeleteFirstEmptyBits()
{
    int pos = FindPositionFirstBit();

    if (pos != 0)
    {
        for (uint i = 0; i < bits.size(); i++)
        {
            for (int p = 0; p < pos; p++)
            {
                bits[i].erase(bits[i].begin());
            }
        }
    }
}


void SymbolImp::DeleteLastEmptyBits()
{
    int pos = FindPositionLastBit();

    if (pos != static_cast<int>(bits[0].size() - 1))
    {
        int delta = static_cast<int>(bits[0].size()) - pos - 1;

        for (uint i = 0; i < bits.size(); i++)
        {
            for (int p = 0; p < delta; p++)
            {
                bits[i].pop_back();
            }
        }
    }
}


void SymbolImp::DeleteBottomEmptyBits()
{
    while (bits.size() > 1 && Sum(bits[bits.size() - 1]) == 0)
    {
        bits.pop_back();
    }
}


static int Sum(std::vector<uint8> &vec)
{
    int result = 0;

    for (uint i = 0; i < vec.size(); i++)
    {
        result += vec[i];
    }

    return result;
}


void SymbolImp::Log() const
{
    for (uint row = 0; row < bits.size(); row++)
    {
        for (uint col = 0; col < bits[row].size(); col++)
        {
            std::cout << ((bits[row][col]) ? "X" : ".");
        }
        std::cout << std::endl;
    }
}
