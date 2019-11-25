#pragma once
#pragma warning(push, 0)
#include <wx/wx.h>
#pragma warning(pop)
#include <vector>


struct Symbol
{
    Symbol(uint w = 16, uint h = 16);
    void Set(int row, int col, uint8 value);
    uint8 Get(int row, int col) const;
    uint width;
    uint height;
    /// ����� ������ ��� ����������� 8-������ ���������
    std::vector<std::vector<uint8>> bits;
};


class Font
{
public:

    /// ������ ������� � ������
    wxSize size = { 16, 16 };
    /// ������� ������� �������� ���� ����� ������ � ����� �����������
    int pixelsInPoint = 8;

    Symbol symbols[256];
};
