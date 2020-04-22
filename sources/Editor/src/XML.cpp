#include "XML.h"
#pragma warning(push, 0)
#include <wx/xml/xml.h>
#pragma warning(pop)


wxXmlNode *XML::FindChildren(wxXmlNode *node, const wxString &name)
{
    wxXmlNode *child = node->GetChildren();

    while(child)
    {
        if(child->GetName() == name)
        {
            return child;
        }

        child = child->GetNext();
    }

    return nullptr;
}
