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
#include "Dialogs/ImportDialog.h"

extern void update();
extern void init();


enum
{
    MENU_FILE_QUIT = wxID_EXIT,

    CONTEXT_MENU_DELETE = wxID_HIGHEST + 1,     // ������� �����
    CONTEXT_MENU_PARAMETERS,                    // ��������� �����

    ALIGN_LEFT,                                 // ��������� ����� �� �����
    ALIGN_RIGHT,                                // ��������� ����� �� ������
    ALIGN_LEFT_TOP,                             // ��������� ����� �� ����� �������
    ALIGN_LEFT_DOWN,                            // ��������� ����� �� ����� ������
    ALIGN_RIGHT_TOP,                            // ��������� ����� �� ������ �������
    ALIGN_RIGHT_DOWN,                           // ��������� ����� �� ������ ������

    FILE_OPEN,
    FILE_SAVE,
    FILE_NEW,                                   // �������� �����
    FILE_IMPORT,                                // ������������� �����
    FILE_EXPORT_C,                              // �������������� ����� � ��-����
    FILE_EXPORT_BIN,                            // �������������� ����� � �������� ����
	UNDO,
	REDO,

    TOOL_IMPORT_FONT,                           // ������ ������
    TOOL_SCALE_UP,                              // ��������� �������
    TOOL_SCALE_DOWN,                            // ��������� �������
    TOOL_CLEAR_BAD_SYMBOLS,                     // �������� ������ �� ��������������� ������
    TOOL_CHOICE_SAVED                           // ������� �������, ������� ����� �������� � �����
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

    Bind(wxEVT_MENU,       &Frame::OnQuit,               this, MENU_FILE_QUIT);
    Bind(wxEVT_MENU,       &Frame::OnOpenFile,           this, FILE_OPEN);
    Bind(wxEVT_MENU,       &Frame::OnSaveFile,           this, FILE_SAVE);
    Bind(wxEVT_MENU,       &Frame::OnNewFile,            this, FILE_NEW);
    Bind(wxEVT_MENU,       &Frame::OnImportFont,         this, FILE_IMPORT);
    Bind(wxEVT_MENU,       &Frame::OnExportFontC,        this, FILE_EXPORT_C);
    Bind(wxEVT_MENU,       &Frame::OnExportFontBin,      this, FILE_EXPORT_BIN);
    Bind(wxEVT_MENU,       &Frame::OnImportFont,         this, TOOL_IMPORT_FONT);
    Bind(wxEVT_MENU,       &Frame::OnUndo,               this, UNDO);
    Bind(wxEVT_MENU,       &Frame::OnRedo,               this, REDO);
    Bind(wxEVT_TIMER,      &Frame::OnTimer,              this, TIMER_ID);
    Bind(wxEVT_SIZE,       &Frame::OnResize,             this);
    Bind(wxEVT_PAINT,      &Frame::OnRepaint,            this);
    Bind(wxEVT_KEY_DOWN,   &Frame::OnKeyDown,            this);
    Bind(wxEVT_MENU,       &Frame::OnScaleDown,          this, TOOL_SCALE_DOWN);
    Bind(wxEVT_MENU,       &Frame::OnScaleUp,            this, TOOL_SCALE_UP);
    Bind(wxEVT_MENU,       &Frame::OnClearBadSymbols,    this, TOOL_CLEAR_BAD_SYMBOLS);
    Bind(wxEVT_MENU,       &Frame::OnChoiceSavedSymbols, this, TOOL_CHOICE_SAVED);

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
    fileMenu->Append(FILE_OPEN, wxT("���������\tCtrl+O"), wxT("��������� ������ �� �����"));
    fileMenu->Append(FILE_SAVE, wxT("���������\tCtrl+S"), wxT("��������� ������ � ����"));
    fileMenu->AppendSeparator();
    fileMenu->Append(FILE_IMPORT, wxT("������"), wxT("������������� ��������� �����"));

    wxMenu *menuImport = new wxMenu;
    menuImport->Append(FILE_EXPORT_C, wxT("�������������� � ��-����"));
    menuImport->Append(FILE_EXPORT_BIN, wxT("�������������� � �������� ����"));
    fileMenu->AppendSubMenu(menuImport, wxT("�������"));

    fileMenu->AppendSeparator();
    fileMenu->Append(MENU_FILE_QUIT, wxT("�����\tAlt+X"), wxT("������� ��������"));

    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(fileMenu, wxT("����"));

    SetMenuBar(menuBar);

    wxBitmap imgOpen(wxImage(wxT("icons/open.bmp"), wxBITMAP_TYPE_BMP));
    wxBitmap imgSave(wxImage(wxT("icons/save.bmp"), wxBITMAP_TYPE_BMP));
    wxBitmap imgNew(wxImage(wxT("icons/new.bmp"), wxBITMAP_TYPE_BMP));
    wxBitmap imgChangeFont(wxImage(wxT("icons/font.bmp"), wxBITMAP_TYPE_BMP));
    wxBitmap imgUndo(wxImage(wxT("icons/undo.bmp"), wxBITMAP_TYPE_BMP));
    wxBitmap imgRedo(wxImage(wxT("icons/redo.bmp"), wxBITMAP_TYPE_BMP));
    wxBitmap imgScaleDown(wxImage(wxT("icons/minus.bmp"), wxBITMAP_TYPE_BMP));
    wxBitmap imgScaleUp(wxImage(wxT("icons/plus.bmp"), wxBITMAP_TYPE_BMP));
    wxBitmap imgClear(wxImage(wxT("icons/clear.bmp"), wxBITMAP_TYPE_BMP));
    wxBitmap imgToggle(wxImage(wxT("icons/toggle.bmp"), wxBITMAP_TYPE_BMP));

    wxToolBar* toolBar = CreateToolBar();
    toolBar->AddTool(FILE_OPEN, wxT("�������"), imgOpen, wxT("��������� ����� ��������� ������ �� �����"));
    toolBar->AddTool(FILE_SAVE, wxT("���������"), imgSave, wxT("��������� ������ � ����"));
    toolBar->AddTool(FILE_NEW, wxT("�����"), imgNew, wxT("������� ����� ������"));

    toolBar->AddSeparator();
    toolBar->AddTool(UNDO, wxT("��������"), imgUndo, wxT("�������� ���������� ��������"));
    toolBar->AddTool(REDO, wxT("������������"), imgRedo, wxT("������������ ��������� ��������"));

    toolBar->AddSeparator();
    toolBar->AddTool(TOOL_IMPORT_FONT, wxT("������� �����"), imgChangeFont, wxT("������� �����"));

    toolBar->AddSeparator();
    toolBar->AddTool(TOOL_SCALE_DOWN, wxT("��������� �������"), imgScaleDown, wxT("���������� ��������"));
    toolBar->AddTool(TOOL_SCALE_UP, wxT("��������� �������"), imgScaleUp, wxT("���������� ��������"));

    toolBar->AddSeparator();
    toolBar->AddTool(TOOL_CLEAR_BAD_SYMBOLS, wxT("�������� �������������� �������"), imgClear, wxT("������� �������������� �������"));
    toolBar->AddTool(TOOL_CHOICE_SAVED, wxT("������� ������� ��� ����������"), imgToggle, wxT(""), wxITEM_CHECK);

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

        menuAlign->Append(ALIGN_LEFT, "�����");
        menuAlign->Append(ALIGN_RIGHT, "������");
        menuAlign->Append(ALIGN_LEFT_TOP, "����� ������");
        menuAlign->Append(ALIGN_LEFT_DOWN, "����� �����");
        menuAlign->Append(ALIGN_RIGHT_TOP, "������ ������");
        menuAlign->Append(ALIGN_RIGHT_DOWN, "������ �����");

        menuPoint.Append(CONTEXT_MENU_DELETE, "�������");
        menuPoint.Append(CONTEXT_MENU_PARAMETERS, "���������");
        menuPoint.AppendSubMenu(menuAlign, "���������");

        menuContext.Append(FILE_NEW, "��������");
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
    wxFileDialog saveDialog(nullptr, wxT("���������"), wxEmptyString, wxEmptyString, wxT("*.txt"), wxFD_SAVE);

    if (saveDialog.ShowModal() == wxID_OK)
    {
        wxString fileName = saveDialog.GetPath();
        
        wxTextFile file(fileName);

        if(file.Exists())
        {
            wxMessageDialog message(this, wxT("���� � ����� ������ ��� ����������. ������������?"), wxMessageBoxCaptionStr, wxOK | wxCANCEL | wxCENTRE);
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


void Frame::OnExportFontC(wxCommandEvent &)
{
    wxFileDialog dlg(nullptr, wxT("�������"), wxEmptyString, wxEmptyString, wxT("*.c"), wxFD_SAVE);

    if (dlg.ShowModal() == wxID_OK)
    {
        wxString fileName = dlg.GetPath();

        wxTextFile file(fileName);

        file.Create();

        TheCanvas->ImportFont(file, "font5");

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


void Frame::OnScaleUp(wxCommandEvent &)
{
    TheCanvas->Increase();
}


void Frame::OnScaleDown(wxCommandEvent &)
{
    TheCanvas->Decrease();
}


void Frame::OnImportFont(wxCommandEvent &)
{
    ImportDialog dlg(wxT("������ ������"));
    dlg.ShowModal();
}


void Frame::OnExportFontBin(wxCommandEvent &)
{

}


void Frame::OnClearBadSymbols(wxCommandEvent &)
{
    TheCanvas->ClearBadSymbols();
}


void Frame::OnChoiceSavedSymbols(wxCommandEvent &event)
{
    TheCanvas->EnableModeCheckSymbols(event.IsChecked());
}
