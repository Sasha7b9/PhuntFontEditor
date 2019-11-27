#pragma once
#pragma warning(push, 0)
#include <wx/wx.h>
#include <wx/textfile.h>
#pragma warning(pop)


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
    void EnableModeCheckSymbols(bool enable);

private:

    void OnMouseMove(wxMouseEvent &);

    void OnMouseLeave(wxMouseEvent &);

    void OnMouseRightDown(wxMouseEvent &);

    void OnMouseRightUp(wxMouseEvent &);

    void TuneScrollBar();

    wxScrolledWindow *sw = nullptr;

    void Resize();
};


extern Canvas *TheCanvas;
