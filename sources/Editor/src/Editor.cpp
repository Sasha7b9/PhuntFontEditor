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
#include "Dialogs/InsertPointsDialog.h"
#include "Dialogs/TriangleDialog.h"

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

    FILE_OPEN,
    FILE_SAVE,
    FILE_NEW,                                   // Очистить форму
	UNDO,
	REDO,

    TOOL_CHANGE_FONT,

    CREATE_TRIANGLE,
    INSERT_POINTS
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

    Bind(wxEVT_MENU,       &Frame::OnQuit,         this, MENU_FILE_QUIT);
    Bind(wxEVT_MENU,       &Frame::OnOpenFile,     this, FILE_OPEN);
    Bind(wxEVT_MENU,       &Frame::OnSaveFile,     this, FILE_SAVE);
    Bind(wxEVT_MENU,       &Frame::OnNewFile,      this, FILE_NEW);
    Bind(wxEVT_MENU,       &Frame::OnChangeFont,   this, TOOL_CHANGE_FONT);
    Bind(wxEVT_MENU,       &Frame::OnUndo,         this, UNDO);
    Bind(wxEVT_MENU,       &Frame::OnRedo,         this, REDO);
    Bind(wxEVT_TIMER,      &Frame::OnTimer,        this, TIMER_ID);
    Bind(wxEVT_SIZE,       &Frame::OnResize,       this);
    Bind(wxEVT_PAINT,      &Frame::OnRepaint,      this);
    Bind(wxEVT_KEY_DOWN,   &Frame::OnKeyDown,      this);
    Bind(wxEVT_MENU,       &Frame::CreateTriangle, this, CREATE_TRIANGLE);
    Bind(wxEVT_MENU,       &Frame::InsertPoints,   this, INSERT_POINTS);

    Show(true);

    timer.Start(0);
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
    fileMenu->Append(FILE_OPEN, wxT("Загрузить\tCtrl+O"), wxT("Загрузить данные из файла"));
    fileMenu->Append(FILE_SAVE, wxT("Сохранить\tCtrl+S"), wxT("Сохранить данные в файл"));
    fileMenu->AppendSeparator();
    fileMenu->Append(MENU_FILE_QUIT, wxT("Выход\tAlt+X"), wxT("Закрыть редактор"));

    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(fileMenu, wxT("Файл"));

    SetMenuBar(menuBar);

    wxBitmap imgOpen(wxImage(wxT("icons/open.bmp"), wxBITMAP_TYPE_BMP));
    wxBitmap imgSave(wxImage(wxT("icons/save.bmp"), wxBITMAP_TYPE_BMP));
    wxBitmap imgNew(wxImage(wxT("icons/new.bmp"), wxBITMAP_TYPE_BMP));
    wxBitmap imgChangeFont(wxImage(wxT("icons/font.bmp"), wxBITMAP_TYPE_BMP));
    wxBitmap imgUndo(wxImage(wxT("icons/undo.bmp"), wxBITMAP_TYPE_BMP));
    wxBitmap imgRedo(wxImage(wxT("icons/redo.bmp"), wxBITMAP_TYPE_BMP));
    wxBitmap imgCreateTriangle(wxImage(wxT("icons/triangle.bmp"), wxBITMAP_TYPE_BMP));
    wxBitmap imgInsertPoints(wxImage(wxT("icons/points.bmp"), wxBITMAP_TYPE_BMP));

    wxToolBar* toolBar = CreateToolBar();
    toolBar->AddTool(FILE_OPEN, wxT("Открыть"), imgOpen, wxT("Загрузить ранее созданный сигнал из файла"));
    toolBar->AddTool(FILE_SAVE, wxT("Сохранить"), imgSave, wxT("Сохранить сигнал в файл"));
    toolBar->AddTool(FILE_NEW, wxT("Новый"), imgNew, wxT("Создать новый сигнал"));

    toolBar->AddSeparator();
    toolBar->AddTool(TOOL_CHANGE_FONT, wxT("Выбрать шрифт"), imgChangeFont, wxT("Выбрать шрифт"));

    toolBar->AddSeparator();
    toolBar->AddTool(UNDO, wxT("Отменить"), imgUndo, wxT("Отменить предыдущее действие"));
    toolBar->AddTool(REDO, wxT("Восстановить"), imgRedo, wxT("Восстановить следующее действие"));

    toolBar->AddSeparator();
    toolBar->AddTool(CREATE_TRIANGLE, wxT("Треугольник"), imgCreateTriangle, wxT("Создать новый сигнал в форме треугольника"));
    toolBar->AddTool(INSERT_POINTS, wxT("Вставить точки"), imgInsertPoints, wxT("Вставить маркеры"));

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

        menuContext.Append(FILE_NEW, "Очистить");
    }

    PopupMenu(underPoint ? &menuPoint : &menuContext, pos.x, pos.y);
}


void Frame::OnOpenFile(wxCommandEvent &)
{
    wxFileDialog openDialog(nullptr, wxEmptyString, wxEmptyString, wxEmptyString, wxT("*.txt"), wxFD_OPEN);

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

void Frame::OnSaveFile(wxCommandEvent &)
{
    wxFileDialog saveDialog(nullptr, wxT("Сохранить"), wxEmptyString, wxEmptyString, wxT("*.txt"), wxFD_SAVE);

    if (saveDialog.ShowModal() == wxID_OK)
    {
        wxString fileName = saveDialog.GetPath();
        
        wxTextFile file(fileName);

        if(file.Exists())
        {
            wxMessageDialog message(this, wxT("Файл с таким именем уже существует. Перезаписать?"), wxMessageBoxCaptionStr, wxOK | wxCANCEL | wxCENTRE);
            if(message.ShowModal() == wxID_CANCEL)
            {
                return;
            }
        }
        
        file.Create();

        file.Write();

        file.Close();
    }
}


void Frame::OnNewFile(wxCommandEvent &)
{
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


void Frame::CreateTriangle(wxCommandEvent &)
{
    TriangleDialog dialog;

    dialog.ShowModal();
}


void Frame::InsertPoints(wxCommandEvent &)
{
    InsertPointsDialog dialog;

    dialog.ShowModal();
}


void Frame::OnChangeFont(wxCommandEvent &)
{
    wxFontDialog dlg(this);

    if (dlg.ShowModal() == wxID_OK)
    {
        TheCanvas->SetFont(dlg.GetFontData().GetChosenFont());
    }
}
