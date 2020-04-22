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

    void SetSizeAndPosition();

    wxRect GetMaxDisplay();

    void ShowContextMenu(const wxPoint &pos, bool underPoint);

    void CreateMenu();

    void CreatePanels();

    // ������������� ����� �� xml-����� � ���������
    void OnImportDescriptionFromXML(wxCommandEvent &);

    // ������������� ��������� �����
    void OnImportFont(wxCommandEvent &);

    // �������������� ����� � ��-����
    void OnExportFontC(wxCommandEvent &);

	void OnUndo(wxCommandEvent &);

	void OnRedo(wxCommandEvent &);

    void OnKeyDown(wxKeyEvent &);

    void OnScaleUp(wxCommandEvent &);

    void OnScaleDown(wxCommandEvent &);

    // �������� �������, �� �������� � �������� �����
    void OnClearBadSymbols(wxCommandEvent &);
    
    // ���������� ������ ����������� ��������
    void OnSelectSymbols(wxCommandEvent &);
};
