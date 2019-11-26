#pragma once
#pragma warning(push, 0)
#include <wx/wx.h>
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
