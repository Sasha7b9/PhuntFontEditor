#include "Editor.h"
#pragma warning(push, 0)
#include <wx/wx.h>
#include <wx/display.h>
#include <wx/fontdlg.h>
#include <wx/mstream.h>
#include <wx/textfile.h>
#pragma warning(pop)

#undef main


#include "defines.h"
#include "Canvas.h"
#include "Dialogs/ExportDialogC.h"
#include "Dialogs/ImportDialog.h"

extern void update();
extern void init();


enum
{
    MENU_FILE_QUIT = wxID_EXIT,

    CONTEXT_MENU_DELETE = wxID_HIGHEST + 1,     // Удалить точку
    CONTEXT_MENU_PARAMETERS,                    // Параметры точки

    ALIGN_LEFT,                                 // Выровнять точку по левой
    ALIGN_RIGHT,                                // Выровнять точку по правой
    ALIGN_LEFT_TOP,                             // Выровнять точку по левой верхней
    ALIGN_LEFT_DOWN,                            // Выровнять точку по левой нижней
    ALIGN_RIGHT_TOP,                            // Выровнять точку по правой верхней
    ALIGN_RIGHT_DOWN,                           // Выровнять точку по правой нижней

    FILE_IMPORT_DESCRIPTION_FROM_XML,
    FILE_IMPORT_SYSTEM_FONT,                    // Импортировать шрифт
    FILE_SAVE,
    FILE_EXPORT_C,                              // Экспортировать шрифт в си-файл
	UNDO,
	REDO,

    TOOL_IMPORT_FONT,                           // Импорт шрифта
    TOOL_SCALE_UP,                              // Увеличить масштаб
    TOOL_SCALE_DOWN,                            // Уменьшить масштаб
    TOOL_CLEAR_BAD_SYMBOLS,                     // Очистить сиволы из дополнительного набора
    TOOL_SELECT_SYMBOLS                         // Выбрать символы, которые будут включены в шрифт
};

enum
{
    TIMER_ID = 1
};


wxIMPLEMENT_APP_NO_MAIN(Application);


int main(int argc, char **argv)
{
    setlocale(LC_ALL, "Russian");

    return wxEntry(argc, argv);
}


bool Application::OnInit()
{
    if (!wxApp::OnInit())
    {
        return false;
    }

    init();

    return true;
}


Frame::Frame(const wxString &title)
    : wxFrame(NULL, wxID_ANY, title),
    timer(this, TIMER_ID)
{
    SetIcon(wxICON(sample));

    CreateMenu();

    CreatePanels();

    CreateStatusBar(2);

    SetSizeAndPosition();

    Bind(wxEVT_MENU,     &Frame::OnQuit,                     this, MENU_FILE_QUIT);
    Bind(wxEVT_MENU,     &Frame::OnImportDescriptionFromXML, this, FILE_IMPORT_DESCRIPTION_FROM_XML);
    Bind(wxEVT_MENU,     &Frame::OnImportSystemFont,         this, FILE_IMPORT_SYSTEM_FONT);
    Bind(wxEVT_MENU,     &Frame::OnImportSystemFont,         this, TOOL_IMPORT_FONT);
    Bind(wxEVT_MENU,     &Frame::OnExportFontC,              this, FILE_SAVE);
    Bind(wxEVT_MENU,     &Frame::OnExportFontC,              this, FILE_EXPORT_C);
    Bind(wxEVT_MENU,     &Frame::OnUndo,                     this, UNDO);
    Bind(wxEVT_MENU,     &Frame::OnRedo,                     this, REDO);
    Bind(wxEVT_TIMER,    &Frame::OnTimer,                    this, TIMER_ID);
    Bind(wxEVT_SIZE,     &Frame::OnResize,                   this);
    Bind(wxEVT_PAINT,    &Frame::OnRepaint,                  this);
    Bind(wxEVT_KEY_DOWN, &Frame::OnKeyDown,                  this);
    Bind(wxEVT_MENU,     &Frame::OnScaleDown,                this, TOOL_SCALE_DOWN);
    Bind(wxEVT_MENU,     &Frame::OnScaleUp,                  this, TOOL_SCALE_UP);
    Bind(wxEVT_MENU,     &Frame::OnClearBadSymbols,          this, TOOL_CLEAR_BAD_SYMBOLS);
    Bind(wxEVT_MENU,     &Frame::OnSelectSymbols,            this, TOOL_SELECT_SYMBOLS);

    Show(true);

    timer.Start(0);

    Maximize();
}


Frame::~Frame()
{
}

void Frame::CreatePanels()
{
    wxScrolledWindow *sw = new wxScrolledWindow(this);

    TheCanvas = new Canvas(sw);

    Centre();
}


void Frame::OnTimer(wxTimerEvent &)
{
    update();
}


void Frame::OnResize(wxSizeEvent &event)
{
    event.Skip();
}


void Frame::OnRepaint(wxPaintEvent &)
{
}


void Frame::OnQuit(wxCommandEvent &WXUNUSED(event))
{
    Close(true);
}


void Frame::SetSizeAndPosition()
{
    wxSize size = { 1024, 768 };

	wxSize displaySize = wxGetDisplaySize();
	displaySize.y -= 40;

	if (displaySize.y < size.y)
	{
		size.y = displaySize.y;
	}

    SetSize(size);
    SetMinSize({ 640, 480 });

    wxRect rect = GetMaxDisplay();

    SetPosition({ rect.width / 2 - size.GetWidth() / 2, rect.height / 2 - size.GetHeight() / 2 });
}


wxRect Frame::GetMaxDisplay()
{
    wxRect result = { 0, 0, 0, 0 };

    for (uint i = 0; i < wxDisplay::GetCount(); i++)
    {
        wxDisplay display(i);

        wxRect rect = display.GetClientArea();
        if (rect.width > result.width)
        {
            result.width = rect.width;
            result.height = rect.height;
        }
    }

    return result;
}


void Frame::CreateMenu()
{
    wxMenu *fileMenu = new wxMenu;
    fileMenu->Append(FILE_IMPORT_DESCRIPTION_FROM_XML, wxT("Загрузить\tCtrl+O"), wxT("Загрузить данные из файла"));
    fileMenu->Append(FILE_SAVE, wxT("Сохранить\tCtrl+S"), wxT("Сохранить данные в файл"));
    fileMenu->AppendSeparator();
    fileMenu->Append(FILE_IMPORT_SYSTEM_FONT, wxT("Импорт"), wxT("Импортировать системный шрифт"));

    wxMenu *menuExport = new wxMenu;
    menuExport->Append(FILE_EXPORT_C, wxT("Экспортировать в Си-файл"));
    fileMenu->AppendSubMenu(menuExport, wxT("Экспорт"));

    fileMenu->AppendSeparator();
    fileMenu->Append(MENU_FILE_QUIT, wxT("Выход\tAlt+X"), wxT("Закрыть редактор"));

    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(fileMenu, wxT("Файл"));

    SetMenuBar(menuBar);

    wxBitmap imgOpen(wxImage(wxT("icons/open.bmp"), wxBITMAP_TYPE_BMP));
    wxBitmap imgSave(wxImage(wxT("icons/save.bmp"), wxBITMAP_TYPE_BMP));
    wxBitmap imgChangeFont(wxImage(wxT("icons/font.bmp"), wxBITMAP_TYPE_BMP));
    wxBitmap imgUndo(wxImage(wxT("icons/undo.bmp"), wxBITMAP_TYPE_BMP));
    wxBitmap imgRedo(wxImage(wxT("icons/redo.bmp"), wxBITMAP_TYPE_BMP));
    wxBitmap imgScaleDown(wxImage(wxT("icons/minus.bmp"), wxBITMAP_TYPE_BMP));
    wxBitmap imgScaleUp(wxImage(wxT("icons/plus.bmp"), wxBITMAP_TYPE_BMP));
    wxBitmap imgClear(wxImage(wxT("icons/clear.bmp"), wxBITMAP_TYPE_BMP));
    wxBitmap imgToggle(wxImage(wxT("icons/toggle.bmp"), wxBITMAP_TYPE_BMP));

    wxToolBar* toolBar = CreateToolBar();
    toolBar->AddTool(FILE_IMPORT_DESCRIPTION_FROM_XML, wxT("Открыть"), imgOpen, wxT("Загрузить шрифт из файла описания"));
    toolBar->AddTool(FILE_SAVE, wxT("Сохранить"), imgSave, wxT("Сохранить шрифт в файл описания"));

    toolBar->AddSeparator();
    toolBar->AddTool(UNDO, wxT("Отменить"), imgUndo, wxT("Отменить предыдущее действие"));
    toolBar->AddTool(REDO, wxT("Восстановить"), imgRedo, wxT("Восстановить следующее действие"));

    toolBar->AddSeparator();
    toolBar->AddTool(TOOL_IMPORT_FONT, wxT("Выбрать шрифт"), imgChangeFont, wxT("Выбрать шрифт"));

    toolBar->AddSeparator();
    toolBar->AddTool(TOOL_SCALE_DOWN, wxT("Уменьшить масштаб"), imgScaleDown, wxT("Уменьшение масштаба"));
    toolBar->AddTool(TOOL_SCALE_UP, wxT("Увеличить масштаб"), imgScaleUp, wxT("Увеличение масштаба"));

    toolBar->AddSeparator();
    toolBar->AddTool(TOOL_CLEAR_BAD_SYMBOLS, wxT("Очистить дополнительные символы"), imgClear, wxT("Стереть дополнительные символы"));
    toolBar->AddTool(TOOL_SELECT_SYMBOLS, wxT("Выбрать символы для сохранения"), imgToggle, wxT(""), wxITEM_CHECK);

    toolBar->Realize();
}


void Frame::ShowContextMenu(const wxPoint &pos, bool underPoint)
{
    static wxMenu menuPoint;
    static wxMenu menuContext;
    static wxMenu *menuAlign = nullptr;

    if (menuAlign == nullptr)
    {
        menuAlign = new wxMenu();

        menuAlign->Append(ALIGN_LEFT, "Слева");
        menuAlign->Append(ALIGN_RIGHT, "Справа");
        menuAlign->Append(ALIGN_LEFT_TOP, "Слева вверху");
        menuAlign->Append(ALIGN_LEFT_DOWN, "Слева внизу");
        menuAlign->Append(ALIGN_RIGHT_TOP, "Справа вверху");
        menuAlign->Append(ALIGN_RIGHT_DOWN, "Справа внизу");

        menuPoint.Append(CONTEXT_MENU_DELETE, "Удалить");
        menuPoint.Append(CONTEXT_MENU_PARAMETERS, "Параметры");
        menuPoint.AppendSubMenu(menuAlign, "Выровнять");
    }

    PopupMenu(underPoint ? &menuPoint : &menuContext, pos.x, pos.y);
}


void Frame::OnImportDescriptionFromXML(wxCommandEvent &)
{
    wxFileDialog openDialog(nullptr, wxEmptyString, wxEmptyString, wxEmptyString, wxT("*.xml"), wxFD_OPEN);

    if (openDialog.ShowModal() == wxID_OK)
    {
        wxString fileName = openDialog.GetPath();

        wxTextFile file(fileName);

        if(file.Exists())
        {
            file.Open();

            file.Close();
        }
    }
}


void Frame::OnExportFontC(wxCommandEvent &)
{
    ExportDialogC dlg(wxT("Экспорт шрифта в Си-файл"));
    dlg.ShowModal();
}


void Frame::OnKeyDown(wxKeyEvent &)
{
}


void Frame::OnUndo(wxCommandEvent &)
{

}


void Frame::OnRedo(wxCommandEvent &)
{

}


void Frame::OnScaleUp(wxCommandEvent &)
{
    TheCanvas->Increase();
}


void Frame::OnScaleDown(wxCommandEvent &)
{
    TheCanvas->Decrease();
}


void Frame::OnImportSystemFont(wxCommandEvent &)
{
    ImportDialog dlg(wxT("Импорт шрифта"));
    dlg.ShowModal();
}


void Frame::OnClearBadSymbols(wxCommandEvent &)
{
    TheCanvas->ClearBadSymbols();
}


void Frame::OnSelectSymbols(wxCommandEvent &event) //-V2009
{
    TheCanvas->EnableModeSelectSymbols(event.IsChecked());
}
