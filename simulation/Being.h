#pragma once

#include <vector>

#include "Capability.h"
#include "position.h"
#include "environment.h"


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

using being_ptr = std::shared_ptr< class being >;
using species_ptr = std::shared_ptr< class species >;

class being
{
public:
	being(species_ptr s);
	virtual ~being();

	void append_capability(capability_ptr);

	status calculate_next_step(environment env);

	position get_position() const;

private:	
	std::weak_ptr<species> _species;

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

