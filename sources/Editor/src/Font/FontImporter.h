#pragma once
#include "Font/BitmapFont.h"
#pragma warning(push, 0)
#include <wx/textfile.h>
#pragma warning(pop)


class FontImporter
{
public:
    static void Import(BitmapFont &font, wxTextFile &file, const wxString &nameFont);
};
