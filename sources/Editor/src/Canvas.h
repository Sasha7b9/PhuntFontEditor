#pragma once
#pragma warning(push, 0)
#include <wx/wx.h>
#pragma warning(pop)





class Canvas : public wxPanel
{
public:

    Canvas(wxWindow *parent);

    void OnPaint(wxPaintEvent &);

private:

    void OnMouse(wxMouseEvent &);
    /// ��������� � ��� ����
    void Increase();
    /// ��������� � ��� ����
    void Decrease();
};


extern Canvas *TheCanvas;
