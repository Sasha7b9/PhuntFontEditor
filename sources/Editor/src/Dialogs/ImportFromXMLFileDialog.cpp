#include "XML.h"
#include "Dialogs/ImportFromXMLFileDialog.h"

#pragma warning(push, 0)
#include <wx/wx.h>
#include <wx/xml/xml.h>
#pragma warning(pop)


void ImportFromXMLFileDialog::Execute()
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
                common = common;
            }
            else
            {
                common = common;
            }
        }
    }
}
