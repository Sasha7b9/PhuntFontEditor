#pragma once
#pragma warning(push, 0)
#include <wx/display.h>
#pragma warning(pop)
#include "Colors.h"


class Canvas
{
public:
    Canvas(wxWindow *parent, int width, int height);
    ~Canvas();
    void Draw();
    void Resize(const wxSize &size);
    const wxSize GetSize() const;
    void SetColor(const Color &color = Color::NUMBER);
    void SetPoint(int x, int y, const Color &color = Color::NUMBER);
    void SetPoint(int x, int y, int size, const Color &color = Color::NUMBER);
    void DrawLine(int x0, int y0, int x1, int y1, const Color &color = Color::NUMBER);
    void Redraw();

private:
    void BeginScene();
    void EndScene();
};


extern Canvas *TheCanvas;
