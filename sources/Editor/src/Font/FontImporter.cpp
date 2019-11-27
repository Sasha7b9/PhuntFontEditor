#include "defines.h"
#include "Font/FontImporter.h"


#define ADD_LINE(x) file.AddLine(x)
#define ADD_FLINE_1(s, x) file.AddLine(wxString::Format(s, x))


void FontImporter::Import(BitmapFont &, wxTextFile &file, const wxString &nameFont)
{
    ADD_FLINE_1("unsigned int %s[] =", nameFont);
    ADD_LINE("{");
}
