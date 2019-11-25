#include "Editor.h"
#pragma warning(push, 0)
#include <SDL.h>
#include <wx/wx.h>
#include <wx/mstream.h>
#include <wx/textfile.h>
#pragma warning(pop)

#undef main


#include "defines.h"
#include "Canvas.h"
#include "Form.h"
#include "History.h"
#include "Dialogs/ExponentDialog.h"
#include "Dialogs/InsertPointsDialog.h"
#include "Dialogs/TrapezeDialog.h"
#include "Dialogs/TriangleDialog.h"

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
	UNDO,
	REDO,

    CREATE_SINE,
    CREATE_TRIANGLE,
    CREATE_TRAPEZE,
    CREATE_EXPONENT,
    INSERT_POINTS
};

enum
{
    TIMER_ID = 1
};


/// ������ � ���� ����
static SDL_Cursor *cursorHand = nullptr;
/// true, ���� ��� ��������� � ������� ���������
static bool mouseIsDown = false;


wxIMPLEMENT_APP_NO_MAIN(Application);


int main(int argc, char **argv)
{
    setlocale(LC_ALL, "Russian");

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0U) //-V2517
    {
        std::cout << "SDL_Init Error:" << SDL_GetError() << std::endl;
    }

    return wxEntry(argc, argv);
}


bool Application::OnInit()
{
    if (!wxApp::OnInit())
    {
        return false;
    }

    init();

    cursorHand = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);

    return true;
}


Frame::Frame(const wxString &title)
    : wxFrame(NULL, wxID_ANY, title),
    timer(this, TIMER_ID)
{
    SetIcon(wxICON(sample));

    CreateMenu();

    CreateStatusBar(2);

    SetSizeAndPosition();

    Bind(wxEVT_MENU, &Frame::OnQuit, this, MENU_FILE_QUIT);
    Bind(wxEVT_MENU, &Frame::OnDeletePoint, this, CONTEXT_MENU_DELETE);
    Bind(wxEVT_MENU, &Frame::OnParametersPoint, this, CONTEXT_MENU_PARAMETERS);
    Bind(wxEVT_MENU, &Frame::OnAlignLeft, this, ALIGN_LEFT);
    Bind(wxEVT_MENU, &Frame::OnAlignRight, this, ALIGN_RIGHT);
    Bind(wxEVT_MENU, &Frame::OnAlignLeftTop, this, ALIGN_LEFT_TOP);
    Bind(wxEVT_MENU, &Frame::OnAlignLeftDown, this, ALIGN_LEFT_DOWN);
    Bind(wxEVT_MENU, &Frame::OnAlignRightTop, this, ALIGN_RIGHT_TOP);
    Bind(wxEVT_MENU, &Frame::OnAlignRightDown, this, ALIGN_RIGHT_DOWN);
    Bind(wxEVT_MENU, &Frame::OnOpenFile, this, FILE_OPEN);
    Bind(wxEVT_MENU, &Frame::OnSaveFile, this, FILE_SAVE);
    Bind(wxEVT_MENU, &Frame::OnNewFile, this, FILE_NEW);
    Bind(wxEVT_MENU, &Frame::OnUndo, this, UNDO);
    Bind(wxEVT_MENU, &Frame::OnRedo, this, REDO);
    Bind(wxEVT_MENU, &Frame::CreateSine, this, CREATE_SINE);
    Bind(wxEVT_MENU, &Frame::CreateTriangle, this, CREATE_TRIANGLE);
    Bind(wxEVT_MENU, &Frame::CreateTrapeze, this, CREATE_TRAPEZE);
    Bind(wxEVT_MENU, &Frame::CreateExponent, this, CREATE_EXPONENT);
    Bind(wxEVT_MENU, &Frame::InsertPoints, this, INSERT_POINTS);
    Bind(wxEVT_TIMER, &Frame::OnTimer, this, TIMER_ID);
    Bind(wxEVT_SIZE, &Frame::OnResize, this);
    Bind(wxEVT_PAINT, &Frame::OnRepaint, this);
    Bind(wxEVT_KEY_DOWN, &Frame::OnKeyDown, this);

    Show(true);

    timer.Start(0);
}


Frame::~Frame()
{
    delete TheForm;
    delete TheCanvas;
}


void Frame::OnTimer(wxTimerEvent &)
{
    HandlerEvents();
    update();
}


void Frame::OnResize(wxSizeEvent &)
{
    TheCanvas->Resize(this->GetClientSize());
}


void Frame::OnRepaint(wxPaintEvent &)
{
    TheCanvas->Redraw();
}


void Frame::HandlerEvents()
{
    SDL_Event event;

    static int mouseX = 0;
    static int mouseY = 0;

    while (SDL_PollEvent(&event))
    {
        SDL_PumpEvents();
        switch (event.type)
        {
        case SDL_MOUSEMOTION:
            mouseX = event.motion.x;
            mouseY = event.motion.y;

            if (mouseIsDown)
            {
                TheForm->MovePoint(mouseX, mouseY);
            }

            TheCanvas->Redraw();

            break;

        case SDL_MOUSEBUTTONDOWN:
            mouseX = event.button.x;
            mouseY = event.button.y;
                
            if (event.button.button == 1)               // "1" ������������ ���
            {
                if (TheForm->ExistPoint(mouseX, mouseY, false))
                {
                    mouseIsDown = true;
                }
                else
                {
                    TheForm->SetPoint(mouseX, mouseY);
                }
                
            }
            else if (event.button.button == 3)          // "3" ������������� ���
            {
                mouseIsDown = false;

                ShowContextMenu({ mouseX, mouseY}, TheForm->ExistPoint(mouseX, mouseY, false));
            }
            else
            {
                // ��������� ������ �� �����������
            }
            break;

        case SDL_MOUSEBUTTONUP:
            mouseIsDown = false;
            History::Add(TheForm);
            break;

        default:
            // ������ �� ������
            break;
        }
    }

    if (TheForm->ExistPoint(mouseX, mouseY, mouseIsDown))
    {
        SDL_SetCursor(cursorHand);
    }
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
    fileMenu->Append(MENU_FILE_QUIT, wxT("�����\tAlt+X"), wxT("������� ��������"));

    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(fileMenu, wxT("����"));

    SetMenuBar(menuBar);

    wxBitmap imgOpen(wxImage(wxT("icons/open.bmp"), wxBITMAP_TYPE_BMP));
    wxBitmap imgSave(wxImage(wxT("icons/save.bmp"), wxBITMAP_TYPE_BMP));
    wxBitmap imgNew(wxImage(wxT("icons/new.bmp"), wxBITMAP_TYPE_BMP));
    wxBitmap imgUndo(wxImage(wxT("icons/undo.bmp"), wxBITMAP_TYPE_BMP));
    wxBitmap imgRedo(wxImage(wxT("icons/redo.bmp"), wxBITMAP_TYPE_BMP));
    wxBitmap imgCreateSine(wxImage(wxT("icons/sine.bmp"), wxBITMAP_TYPE_BMP));
    wxBitmap imgCreateTriangle(wxImage(wxT("icons/triangle.bmp"), wxBITMAP_TYPE_BMP));
    wxBitmap imgCreateTrapeze(wxImage(wxT("icons/trapeze.bmp"), wxBITMAP_TYPE_BMP));
    wxBitmap imgCreateExponent(wxImage(wxT("icons/exponent.bmp"), wxBITMAP_TYPE_BMP));
    wxBitmap imgInsertPoints(wxImage(wxT("icons/points.bmp"), wxBITMAP_TYPE_BMP));

    wxToolBar* toolBar = CreateToolBar();
    toolBar->AddTool(FILE_OPEN, wxT("�������"), imgOpen, wxT("��������� ����� ��������� ������ �� �����"));
    toolBar->AddTool(FILE_SAVE, wxT("���������"), imgSave, wxT("��������� ������ � ����"));
    toolBar->AddTool(FILE_NEW, wxT("�����"), imgNew, wxT("������� ����� ������"));

    toolBar->AddSeparator();
    toolBar->AddTool(UNDO, wxT("��������"), imgUndo, wxT("�������� ���������� ��������"));
    toolBar->AddTool(REDO, wxT("������������"), imgRedo, wxT("������������ ��������� ��������"));

    toolBar->AddSeparator();
    toolBar->AddTool(CREATE_SINE, wxT("���������"), imgCreateSine, wxT("������� ����� ������ � ����� ���������"));
    toolBar->AddTool(CREATE_TRIANGLE, wxT("�����������"), imgCreateTriangle, wxT("������� ����� ������ � ����� ������������"));
    toolBar->AddTool(CREATE_TRAPEZE, wxT("��������"), imgCreateTrapeze, wxT("������� ����� ������ � ����� ��������"));
    toolBar->AddTool(CREATE_EXPONENT, wxT("����������"), imgCreateExponent, wxT("������� ����� ���������������� ������"));

    toolBar->AddSeparator();
    toolBar->AddTool(INSERT_POINTS, wxT("�������� �����"), imgInsertPoints, wxT("�������� �������"));
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


void Frame::OnDeletePoint(wxCommandEvent &)
{
    TheForm->RemovePoint();
}


void Frame::OnParametersPoint(wxCommandEvent &)
{

}


void Frame::OnAlignLeft(wxCommandEvent &)
{
    TheForm->AlignPoint(Align::Left);
}


void Frame::OnAlignRight(wxCommandEvent &)
{
    TheForm->AlignPoint(Align::Right);
}


void Frame::OnAlignLeftTop(wxCommandEvent &)
{
    TheForm->AlignPoint(Align::LeftTop);
}


void Frame::OnAlignLeftDown(wxCommandEvent &)
{
    TheForm->AlignPoint(Align::LeftDown);
}


void Frame::OnAlignRightTop(wxCommandEvent &)
{
    TheForm->AlignPoint(Align::RightTop);
}

void Frame::OnAlignRightDown(wxCommandEvent &)
{
    TheForm->AlignPoint(Align::RightDown);
}

void Frame::OnUndo(wxCommandEvent&)
{
	TheForm->UndoHistory();
}

void Frame::OnRedo(wxCommandEvent&)
{
	TheForm->RedoHistory();
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

            TheForm->LoadFromFile(file);

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

        TheForm->SaveToFile(file);

        file.Write();

        file.Close();
    }
}

void Frame::OnNewFile(wxCommandEvent &)
{
    TheForm->Clear();

    History::Add(TheForm);
}

void Frame::CreateSine(wxCommandEvent &)
{
    static uint16 data[Point::NUM_POINTS];

    for (int i = 0; i < Point::NUM_POINTS; i++)
    {
        data[i] = static_cast<uint16>(Point::AVE + (std::sin(static_cast<float>(i) / Point::NUM_POINTS * 2 * 3.14) * Point::AVE));
    }

    TheForm->SetMainForm(data, nullptr);
}

void Frame::CreateTriangle(wxCommandEvent &)
{
    TriangleDialog dialog;

    dialog.ShowModal();
}

void Frame::CreateTrapeze(wxCommandEvent &)
{
    TrapezeDialog dialog;

    dialog.ShowModal();
}

void Frame::CreateExponent(wxCommandEvent &)
{
    ExponentDialog dialog;

    dialog.ShowModal();
}

void Frame::InsertPoints(wxCommandEvent &)
{
    InsertPointsDialog dialog;

    dialog.ShowModal();
}


void Frame::OnKeyDown(wxKeyEvent &)
{
//    if(event.GetKeyCode() == 'x' || event.GetKeyCode() == 'X')
//    {
//        if(event.ControlDown())
//        {
//            Close(true);
//        }
//    }
}
