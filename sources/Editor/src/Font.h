#pragma once
#pragma warning(push, 0)
#include <wx/wx.h>
#pragma warning(pop)
#include <vector>


struct Symbol
{
    Symbol(uint w = 8, uint h = 8);
    void Set(int row, int col, uint8 value);
    uint width;
    uint height;
    /// ����� ������ ��� ����������� 8-������ ���������
    std::vector<std::vector<uint8>> bits;
};


class Font
{
public:

    /// ������ ������� � ������
    wxSize size = { 8, 8 };
    /// ������� ������� �������� ���� ����� ������ � ����� �����������
    int pixelsInPoint = 8;

    Symbol symbols[256];
};
