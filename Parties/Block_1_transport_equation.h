#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <locale.h>
#include <fstream>
#include <iomanip>
#include <fixed/fixed.h>
#include <pde_solvers/pde_solvers.h>
#include "struct.h"
/// @brief ����� Block_1 ��� ������� ����� �� ����� 1 - ������ �������� ������
class Block_1_transport_equation
{
    // ���� ������
    // n - ���������� ����� ��������� �����;
    int n;
    // number_layers - ���������� ����� �������;
    int number_layers;
    // dx - �������� ���� ����� ������ ��������� �����, �;
    double dx;
    // dt - ��� �� ������� �� ������� �������.
    double dt;

public:
    /// @brief ����������� ������ Block_1
    /// @param pipeline_characteristics - ��������� �������� ���������� ������������
    /// @param n - ���������� ����� ��������� �����;
    Block_1_transport_equation(Pipeline_parameters& pipeline_characteristics, int n);
    
    /// @brief ������ ��� ��������� ����� ����
    /// @return number_layers - ���������� ����� �������;
    int getter_number_layers();

    /// @brief ����� �������������, �������������� ����
    /// @param solver_parameters - ��������� ����������, ����������� ��� ���������;
    /// @param buffer - �����, ������� ��� ������� ������ 2 ���� (������� � ��������);
    /// @param left_condition - ��������� ������� ��� ��������� �����.
    /// @return previous_layer - ���������� ������������ �� ������ ������������� ������� ����
    void method_characteristic(vector<double>& current_layer, vector<double>& previous_layer, double left_condition);

    /// @brief ����� output_data, ������� ������� ������������ �� ������ ������������� ���� � ���� ������� csv
    /// @param i - ������� �����;
    /// @param solver_parameters - ��������� ����������, ����������� ��� ���������� ������� ������� ;
    /// @param buffer - �����, ������������ ����� �������;
    /// @return  - ������ ������� (����� � ���� ������������� ���� - buffer.current()).
    void output_data(ring_buffer_t<vector<vector<double>>>& buffer, int i);
};
