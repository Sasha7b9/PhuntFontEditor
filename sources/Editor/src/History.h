#pragma once
#include "Form.h"


struct History
{
	/// ��������� ����� � �������
	static void Add(const Form *form);
	/// ��������� ���������� ����� �� �������
	static Form* Prev();
	/// ��������� ��������� ����� �� �������
	static Form* Next();
};
