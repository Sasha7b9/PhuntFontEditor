#pragma once
#pragma warning(push, 0)
#include "wx/wx.h"
#pragma warning(pop)


class Application : public wxApp
{
public:
    virtual bool OnInit() wxOVERRIDE;
};


class Frame : public wxFrame
{
public:
    Frame(const wxString &title);
    ~Frame();

    void OnQuit(wxCommandEvent &);
    void OnTimer(wxTimerEvent &);
    void OnResize(wxSizeEvent &);
    void OnRepaint(wxPaintEvent &);

private:
    wxTimer timer;

    void HandlerEvents();

    void SetSizeAndPosition();

    wxRect GetMaxDisplay();

    void ShowContextMenu(const wxPoint &pos, bool underPoint);

    void CreateMenu();

	void OnUndo(wxCommandEvent &);

	void OnRedo(wxCommandEvent &);

    void OnOpenFile(wxCommandEvent &);

    void OnSaveFile(wxCommandEvent &);

    void OnNewFile(wxCommandEvent &);

    void OnKeyDown(wxKeyEvent &);

    void CreateTriangle(wxCommandEvent &);

    void CreateTrapeze(wxCommandEvent &);

    void CreateExponent(wxCommandEvent &);

    void InsertPoints(wxCommandEvent &);
};
