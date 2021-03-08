#pragma once

#include <vector>

#include "Capability.h"
#include "position.h"

using being_ptr = std::shared_ptr<class being>;

enum class strategy
{
	rest,	// ��������, ���������� ���
	wander, // ��������� �������
	afraid, // ������, �������
	hunt,   // �������� ��������� �� ������
};

enum class status
{
	ok,		// ���������� ����
	dead,	// �������
	spawn,	// ������������
};

const int mass_to_energy_factor = 10;

class being
{
public:
	being();
	virtual ~being();

	void appendCapability(capability_ptr);

	status calculate_next_step(environment env);
	status calculate_next_step(environment env);

private:	
	// ����������, ����������� ��������� ���������
	position _pos;	    // ��������� �������� � ���������
	strategy _strategy; // ������� ���������
	int		 _mass;     // ����� ��������� ��������
	int		 _energy;   // �������, ����������� ��������� 
	int	     _temprature;    // ����������� ���� ��������

	// ����������, ����������� �������� ���������
	int		 _mass_limit;
	int		 _speed;
	std::vector<capability_ptr> _cap_list;
};

