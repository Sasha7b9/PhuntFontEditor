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

    // Импортировать шрифт из xml-файла с описанием
    void OnImportDescriptionFromXML(wxCommandEvent &);

    // Импортировать системный шрифт
    void OnImportFont(wxCommandEvent &);

    // Экспортировать шрифт в си-файл
    void OnExportFontC(wxCommandEvent &);

	void OnUndo(wxCommandEvent &);

	void OnRedo(wxCommandEvent &);

    void OnKeyDown(wxKeyEvent &);

    void OnScaleUp(wxCommandEvent &);

    void OnScaleDown(wxCommandEvent &);

    // Очистить символы, не входящие в основной набор
    void OnClearBadSymbols(wxCommandEvent &);
    
    // Инструмент выбора сохраняемых символов
    void OnSelectSymbols(wxCommandEvent &);
};
