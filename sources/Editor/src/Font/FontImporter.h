#pragma once
#include "Font/BitmapFont.h"
#pragma warning(push, 0)
#include <wx/textfile.h>
#pragma warning(pop)


class FontImporter
{
public:
    static void Import(BitmapFont *font, wxTextFile &file, const wxString &nameFont);

private:
    /// ���������� ������� ��������
    static void CalculateSizes(int *sizes);

    static void CalculateOffsets(const int sizes[256], uint16 offsets[256]);

    static void WriteFont(wxTextFile &file, const wxString &nameFont, const int sizes[256], const uint16 offsets[256]);

    static BitmapFont *font;
};
