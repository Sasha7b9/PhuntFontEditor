#include "defines.h"
#include "Canvas.h"
#include "XML.h"
#include "Dialogs/ImportFromXMLFileDialog.h"
#include "Font/FontImporter.h"

#pragma warning(push, 0)
#include <wx/wx.h>
#include <wx/xml/xml.h>
#pragma warning(pop)


bool ImportFromXMLFileDialog::Execute()
{
    wxFileDialog openDialog(nullptr, wxEmptyString, wxEmptyString, wxEmptyString, wxT("*.xml"), wxFD_OPEN);

    if(openDialog.ShowModal() == wxID_OK)
    {
        wxString fileName = openDialog.GetPath();

        wxXmlDocument xml;
        if(xml.Load(fileName))
        {
            wxXmlNode *root = xml.GetRoot();

            wxXmlNode *common = XML::FindChildren(root, "Common");

            if(common)
            {
                long pointSize = 0;
                common->GetAttribute("PointSize").ToLong(&pointSize);
                wxFontFamily fontFamily = FontImporter::FontFamilyToENUM(common->GetAttribute("FontFamily"));
                wxFontStyle fontStyle = FontImporter::FontStyleToENUM(common->GetAttribute("FontStyle"));
                wxFontWeight fontWeigth = FontImporter::FontWeightToENUM(common->GetAttribute("FontWeight"));
                wxString faceName = common->GetAttribute("FaceName");

                SettingsFont settings;

                settings.font.Create(pointSize, fontFamily, fontStyle, fontWeigth, false, faceName);

                wxXmlNode *cell = XML::FindChildren(common, "Cell");

                cell->GetAttribute("Width").ToLong(reinterpret_cast<long *>(&settings.width));
                cell->GetAttribute("Height").ToLong(reinterpret_cast<long *>(&settings.height));
                cell->GetAttribute("OffsetX").ToLong(reinterpret_cast<long *>(&settings.offsetX));
                cell->GetAttribute("OffsetY").ToLong(reinterpret_cast<long *>(&settings.offsetY));

                ImportSystemFontDialog::SetSettingsFont(settings);

                TheCanvas->Rebuild();
            }
        }
    }

    return true;
}
