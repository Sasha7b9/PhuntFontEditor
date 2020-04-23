#include "defines.h"
#include "Dialogs/ImportSystemFontDialog.h"
#include "Font/FontImporter.h"


#define ADD_LINE(x)                file.AddLine(x)
#define ADD_FLINE_1(s, x)          file.AddLine(wxString::Format(s, x))
#define ADD_FLINE_2(s, x1, x2)     file.AddLine(wxString::Format(s, x1, x2))
#define ADD_FLINE_3(s, x1, x2, x3) file.AddLine(wxString::Format(s, x1, x2, x3))


//#define TRACE(num)                                          \
//if(gI == 0x21)                                              \
//{                                                           \
//    std::cout << "Point " <<  num <<  std::endl;            \
//    std::cout << "width = " << GetWidth() << std::endl;     \
//    std::cout << "height = " << GetHeight() << std::endl;   \
//    Log();                                                  \
//}


// Структура символа для импортёра
class SymbolImp
{
    friend class FontImporter;
public:
    SymbolImp(BitmapSymbol *s) : symbol(s)
    {
        CreateBits();
//        TRACE(1);
        DeleteFirstEmptyBits();
//        TRACE(2);
        DeleteLastEmptyBits();
//        TRACE(3);
        DeleteBottomEmptyBits();
//        TRACE(4);
    };
    // Возвращает размер соответствующего символа BitmapSymbol
    int GetSize() const;
    // Возвращает в vec последовательность байт, соответсвующую символу
    void PrepareBytes(std::vector<uint8> &vec);
private:
    // Обрабатывемый символ
    BitmapSymbol *symbol;
    // Функция заполняем массив bits. При этом удаляются пустые начальные биты строк и пустые конечные биты строк
    void CreateBits();
    // Здесь хранятся биты символа
    std::vector<std::vector<uint8>> bits;
    // Удалить первые "пустые" биты
    void DeleteFirstEmptyBits();
    // Удалить последние "пустые" биты
    void DeleteLastEmptyBits();
    // Удалить нижние "пустые" биты
    void DeleteBottomEmptyBits();
    // Найти позицию первого ненулевого бита в символе
    int FindPositionFirstBit() const;
    // Найти позицию последнего ненулевого бита в символе
    int FindPositionLastBit() const;
    // Возвращает высоту символа
    int GetHeight() const;
    // Возвращает номер первой непустой строки
    int GetFirstRow() const;
    // Возвращает ширину символа
    int GetWidth() const;
    // Возвращает число бит в строке
    int BitsInRow() const;
    // Возвращает число байт на строку
    int BytesInRow() const;
    
    void Log() const;
    // Возвращает true, если все биты раны 0
    bool Empty() const;
    // Возвращает сумму элементов вектора
    int SumRow(const std::vector<uint8> &vec) const;
    // Добавить в vec байты очередной строки
    void PrepareRow(const std::vector<uint8> &row, std::vector<uint8> &vec);
};


static SymbolImp *symbols[256];


void FontImporter::Import(BitmapFont &font, wxTextFile &file, const wxString &nameFont)
{
    uint16 offsets[256];        // Здесь смещения всех символов

    int sizes[256];             // Здесь размеры всех символов

    CreateSymbols(font);

    CalculateSizes(sizes);

    CalculateOffsets(sizes, offsets);

    WriteFont(file, nameFont, offsets);

    DeleteSymbols();
}


void FontImporter::CreateSymbols(BitmapFont &font)
{
    int i = 0;

    for (int row = 0; row < 16; row++)
    {
        for (int col = 0; col < 16; col++)
        {
            symbols[i++] = new SymbolImp(&font.symbols[row][col]);
        }
    }
}


void FontImporter::WriteFont(wxTextFile &file, const wxString &nameFont, const uint16 offsets[256])
{
    WriteParametersFont(file);

    ADD_FLINE_2("const unsigned char %s[%d] =", nameFont, CalculateFullSize());
    ADD_LINE("{");

    for (int i = 0; i < 256; i++)
    {
        BitSet16 bs(offsets[i]);

        ADD_FLINE_3("/* 0x%02X */   0x%02X, 0x%02X,", i, bs.byte[0], bs.byte[1]);
    }

    for (int i = 0; i < 256; i++)
    {
        if (!symbols[i]->symbol->enabled)
        {
            continue;;
        }

        ADD_LINE("");
        ADD_FLINE_2("/* Symbol 0x%02X, offset %d */", i, offsets[i]);

        ADD_FLINE_3("    %d, %d, %d,", symbols[i]->GetWidth(), symbols[i]->GetHeight(), symbols[i]->GetFirstRow());

        std::vector<uint8> bytes;
        symbols[i]->PrepareBytes(bytes);

        wxString string("    ");

        for (uint numByte = 0; numByte < bytes.size(); numByte++)
        {
            string.Append(wxString::Format("0x%02X, ", bytes[numByte]));
        }

        if(i == 255)
        {
            string.RemoveLast(2);
        }

        ADD_LINE(string);
    }

    ADD_LINE("};");
}


void FontImporter::WriteParametersFont(wxTextFile &file)
{
    SettingsFont set = ImportSystemFontDialog::GetSettingsFont();

    ADD_LINE("/*");
    ADD_FLINE_1("    FaceName - %s",    set.font.GetFaceName());
    ADD_FLINE_1("    PointSize - %d",   set.font.GetPointSize());
    ADD_FLINE_1("    FontWeight - %s",  FontWeight(set.font));
    ADD_FLINE_1("    Style - %s",       FontStyle(set.font));
    ADD_FLINE_1("    Family - %s",      FontFamily(set.font));
    ADD_FLINE_1("    Cell width - %d",  set.width);
    ADD_FLINE_1("    Cell height - %d", set.height);
    ADD_FLINE_1("    Offset x - %d",    set.offsetX);
    ADD_FLINE_1("    Offset y - %d",    set.offsetY);
    ADD_LINE("*/");
}


char *FontImporter::FontWeight(const wxFont &font)
{
    char *result = "invalid";

    switch(font.GetWeight())
    {
    case wxFONTWEIGHT_THIN:         result = "thin";        break;
    case wxFONTWEIGHT_EXTRALIGHT:   result = "extralight";  break;
    case wxFONTWEIGHT_LIGHT:        result = "light";       break;
    case wxFONTWEIGHT_MEDIUM:       result = "medium";      break;
    case wxFONTWEIGHT_SEMIBOLD:     result = "semibold";    break;
    case wxFONTWEIGHT_BOLD:         result = "bold";        break;
    case wxFONTWEIGHT_EXTRABOLD:    result = "extrabold";   break;
    case wxFONTWEIGHT_HEAVY:        result = "heavy";       break;
    case wxFONTWEIGHT_EXTRAHEAVY:   result = "extraheavy";  break;
    case wxFONTWEIGHT_NORMAL:       result = "normal";      break;
    case wxFONTWEIGHT_INVALID:                              break;
    }

    return result;
}


char *FontImporter::FontStyle(const wxFont &font)
{
    char *result = "invalid";

    switch(font.GetStyle())
    {
    case wxFONTSTYLE_NORMAL:    result = "normal";  break;
    case wxFONTSTYLE_ITALIC:    result = "italic";  break;
    case wxFONTSTYLE_SLANT:     result = "slant";   break;
    case wxFONTSTYLE_MAX:                           break;
    }

    return result;
}


char *FontImporter::FontFamily(const wxFont &font)
{
    char *result = "invalid";

    switch(font.GetFamily())
    {
    case wxFONTFAMILY_DEFAULT:      result = "default";     break;
    case wxFONTFAMILY_DECORATIVE:   result = "decorative";  break;
    case wxFONTFAMILY_ROMAN:        result = "roman";       break;
    case wxFONTFAMILY_SCRIPT:       result = "script";      break;
    case wxFONTFAMILY_SWISS:        result = "swiss";       break;
    case wxFONTFAMILY_MODERN:       result = "modern";      break;
    case wxFONTFAMILY_TELETYPE:     result = "teletype";    break;
    case wxFONTFAMILY_UNKNOWN:      result = "unknown";     break;
    }

    return result;
}


void FontImporter::CalculateSizes(int *sizes)
{
    for (int row = 0; row < 16; row++)
    {
        for (int col = 0; col < 16; col++)
        {
            *sizes++ = symbols[row * 16 + col]->GetSize();
        }
    }
}


void FontImporter::CalculateOffsets(const int sizes[256], uint16 offsets[256])
{
    int lastOffset = sizeof(uint16) * 256;      // Это смещение будет у следующего непустого символа. Первое значение будет помещено сразу после таблицы смещений

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
    //                                                width   height  firstRow
    return (GetHeight() - GetFirstRow()) * BytesInRow() +  1   +   1    +  1;
}


int SymbolImp::BitsInRow() const
{
    return FindPositionLastBit() + 1;
}


int SymbolImp::BytesInRow() const
{
    int result = BitsInRow() / 8 ;

    if (BitsInRow() % 8)
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
    if (Empty())
    {
        return static_cast<int>(bits[0].size() - 1);
    }

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


int SymbolImp::GetFirstRow() const
{
    if (Empty())
    {
        return 0;
    }

    for (uint i = 0; i < bits.size(); i++)
    {
        if (SumRow(bits[i]))
        {
            return static_cast<int>(i);
        }
    }

    return 0;
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
    if (!Empty())
    {
        int pos = FindPositionFirstBit();

        if (pos != 0)
        {
            for (uint i = 0; i < bits.size(); i++)
            {
                bits[i].erase(bits[i].begin(), bits[i].begin() + pos);
            }
        }
    }
}


void SymbolImp::DeleteLastEmptyBits()
{
    if (!Empty())
    {
        int pos = FindPositionLastBit();

        if (pos != static_cast<int>(bits[0].size() - 1))
        {
            for (uint i = 0; i < bits.size(); i++)
            {
                bits[i].erase(bits[i].begin() + pos + 1, bits[i].end());
            }
        }
    }
}


void SymbolImp::DeleteBottomEmptyBits()
{
    while (bits.size() > 1 && SumRow(bits[bits.size() - 1]) == 0)
    {
        bits.pop_back();
    }
}


int SymbolImp::SumRow(const std::vector<uint8> &vec) const
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


bool SymbolImp::Empty() const
{
    for (uint row = 0; row < bits.size(); row++)
    {
        if (SumRow(bits[row]) != 0)
        {
            return false;
        }
    }

    return true;
}


void SymbolImp::PrepareBytes(std::vector<uint8> &vec)
{
    vec.clear();

    for (uint row = static_cast<uint>(GetFirstRow()); row < bits.size(); row++)
    {
        PrepareRow(bits[row], vec);
    }
}


void SymbolImp::PrepareRow(const std::vector<uint8> &row, std::vector<uint8> &vec)
{
    uint numBit = 0;

    for (int byte = 0; byte < BytesInRow(); byte++)
    {
        uint8 data = 0;
        for (int bit = 0; bit < 8; bit++)
        {
            if (numBit < row.size())
            {
                if (row[numBit])
                {
                    data |= 1;
                }
            }
            
            if (bit < 7)
            {
                data = static_cast<uint8>(data << 1);
            }

            numBit++;
        }
        vec.push_back(data);
    }
}


int FontImporter::CalculateFullSize()
{
    int result = sizeof(uint16) * 256;

    for(int i = 0; i < 256; i++)
    {
        result += symbols[i]->GetSize();
    }

    return result;

}
