#pragma once
#pragma warning(push, 0)
#include <wx/wx.h>
#include <wx/textfile.h>
#pragma warning(pop)


class BitmapSymbol;


class Canvas : public wxPanel
{
public:

    Canvas(wxWindow *parent);

    void OnPaint(wxPaintEvent &);

    void Rebuild();
    /// ��������� �������
    void Increase();
    /// ��������� �������
    void Decrease();
    /// �������� �������, �� �������� � �������� �����
    void ClearBadSymbols();

    void ImportFont(wxTextFile &file, const wxString &nameFont);
    /// ��������/��������� ����� ������ ��������
    void EnableModeSelectSymbols(bool enable);

private:

    void OnMouseMove(wxMouseEvent &);

    void OnMouseLeave(wxMouseEvent &);

    void OnMouseLeftDown(wxMouseEvent &);

    void OnMouseRightDown(wxMouseEvent &);

    void OnMouseRightUp(wxMouseEvent &);

    void TuneScrollBar();

    void Resize();

    void DrawCursor(wxPaintDC &dc);
    /// ���������� ����� �������, �� ������� ��������� ����
    void HighlightPixel(wxPaintDC &dc);
    /// ��������/����� ��������� � �������, ������� ���������� ��� ��������
    void HighlightSymbol(wxPaintDC &dc);
    /// ����������� ������ ������
    wxSize CurrentSize();

    BitmapSymbol *GetSymbolUnderMouse(int mouseX, int mouseY);

    wxScrolledWindow *sw = nullptr;

    enum Mode
    {
        Edit,           ///< �������� �����, � ������� ����� ������������� �������
        SelectSymbols   ///< ����� ������ ��������
    } mode = Edit;
};


extern Canvas *TheCanvas;
