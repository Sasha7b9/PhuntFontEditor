#pragma once
#include "defines.h"
#include "Canvas.h"
#include "MyMath.h"
#include <vector>
#pragma warning(push, 0)
#include <wx/textfile.h>
#pragma warning(pop)

/*
    ����� ������������� ����� �������
*/


struct Align
{
    enum E
    {
        Left,
        Right,
        LeftTop,
        LeftDown,
        RightTop,
        RightDown
    };
};


struct Point
{
	static const uint SIZE = 5;

    static const uint16 MIN = static_cast<uint16>(0);
	static const uint16 MAX = static_cast<uint16>((1 << 12) - 1);
	static const uint16 AVE = static_cast<uint16>((MAX) / 2);

	static const uint NUM_POINTS = 8192;

	Point(int mouseX, int mouseY)
	{
		pos = MyMath::Round<uint16>(mouseX / ScaleX());
		data = static_cast<uint16>(Point::MAX - MyMath::Round<uint16>(mouseY / ScaleY()));
	}
	Point(uint16 p, uint16 d) : pos(p), data(d) {};
	void SetY(int mouseY)
	{
		data = static_cast<uint16>(Point::MAX - MyMath::Round<uint16>(mouseY / ScaleY()));
	}

	uint16 pos;
	uint16 data;
	bool operator < (const Point& point) const
	{
		return (pos < point.pos);
	}
	/// ���������� true, ���� ������ ���� ��������� ��� ���� ������
	double DistanceFromMouse(int mouseX, int mouseY)
	{
        int dX = mouseX - static_cast<int>(pos);

        int dY = mouseY - static_cast<int>(Point::MAX - data);

        return std::sqrt(dX * dX + dY * dY);
	}
	/// ������� �� �����������
	static float ScaleX()
	{
		return TheCanvas->GetSize().x / static_cast<float>(NUM_POINTS);
	}
	/// ������� �� ���������
	static float ScaleY()
	{
		return TheCanvas->GetSize().y / static_cast<float>(MAX);
	}
};


class Form
{
public:
    Form();
    ~Form();
    /// �������� ������
    void Clear();
    /// �������� ����� � ������������ ����
    void SetPoint(int mouseX, int mouseY);
    /// �������� ����� � ����������� ����������
    void SetPoint(uint16 pos, uint16 data);
    /// ������� ����� ���� �������
    void SetPoint(Point point);
    /// �������� ����� � ������� pos
    void SetPoint(uint16 pos);
    /// ���������� true, ���� ������ ���� ��������� ��� ������������ ������. Pressed - ������ �� ������ ����
    bool ExistPoint(int mouseX, int mouseY, bool pressed);
    /// ������� ����� � ������� ����
    void RemovePoint();
    /// ����������� ����� � ����� �������
    void MovePoint(int mouseX, int mouseY);
    /// ��������� �����
    void AlignPoint(Align::E align);

    void Draw();

	void UndoHistory();

	void RedoHistory();
    /// ���������� �������������� �����, ������� ����� ���������� ������ ��������. 
    void SetAdditionForm(const uint16 data[Point::NUM_POINTS]);
    /// ���������� �������� �����
    void SetMainForm(const uint16 data[Point::NUM_POINTS], const std::vector<Point> *points);

    bool IsEquals(const Form *form) const;

    void SaveToFile(wxTextFile &file);

    void LoadFromFile(wxTextFile &file);

private:

	/// ������, ������� ��� �������� � ������
	uint16 data[Point::NUM_POINTS];

	std::vector<Point> points;
	/// ����� �������� ������ �����, ������� ��������� � ������� ������
	uint iCurPoint = 0;
	/// ���������� index ����� � ������� pos. 0xFFFFFFFF, ���� ����� � ���� ������� ���
	uint PointInPosition(uint16 pos);
	/// ���������� �������� � point �����
	void CalculateNeighboringPoints(const Point& point);
	/// ������� ��������������� �����, ������������� ����� pos1 � pos2
	void LinearInterpolation(uint16 pos1, uint16 pos2);
	/// ��������������� ����� ����� �� ����� � �������� index �� points
	void LinearInterpolationLeft(uint index);
	/// ��������������� ����� ������ �� ����� � �������� index �� points
	void LinearInterpolationRight(uint index);
};


extern Form *TheForm;
