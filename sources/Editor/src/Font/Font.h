#pragma once


struct Symbol
{
    uint8 width;            ///< ������ ������� � �����
    uint8 height;           ///< ������ ������� � �����
    uint8 bytesInRow;       ///< ���������� ���� �� ������
    uint8 *d;               ///< ������. ������ ������ ����� ����� ����� ����. ������ ���� �� ������������. ������� ��� ��� ������.

    uint8 GetWidth() const { return width; }
    uint8 GetHeight() const { return height; }
    int GetBit(int row, int col) const;       ///< ���������� 0, ���� ����� ���, � ������������ �������� � �������� ������
    const uint8 *GetRow(int row) const;
};


struct HeaderFont
{
    uint16 offsets[256];            /// ����� �������� �������� ���� �������� ������������ &offsets[0]
    Symbol *symbols;                /// ������ ������. �� ��������� 256
};


uint8 font[25];


struct Font
{
    void SetFont(uint8 *f) { font = f; };
    Symbol *GetSymbol(uint8 code);   /// ���������� ��������� �� ������ � ����� code
private:
    uint8 *font;
};
