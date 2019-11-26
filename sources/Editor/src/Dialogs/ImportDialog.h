#pragma once
#include "Dialogs/Dialog.h"


class ImportDialog : public Dialog
{
public:
    ImportDialog(const wxString &title);

protected:

    void OnChoiceFont(wxCommandEvent &);
};
