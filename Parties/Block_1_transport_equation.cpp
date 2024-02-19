#include "Block_1_transport_equation.h"

Block_1_transport_equation::Block_1_transport_equation(Pipeline_parameters& pipeline_characteristics, int n)
{
    this->n = n;
    dx = pipeline_characteristics.L / (n - 1);
    dt = dx / pipeline_characteristics.v;
    number_layers = static_cast<int>(pipeline_characteristics.T / dt);
}

int Block_1_transport_equation::getter_number_layers()
{
    return number_layers;
}


void Block_1_transport_equation::method_characteristic(vector<double>& current_layer, vector<double>& previous_layer, double left_condition)
{
    // ��������� ������ �� ������� � ���������� ���� ������

    for (size_t i = 1; i < n; i++)
    {
        current_layer[i] = previous_layer[i - 1];
    }
    // ���� current_layer �� ��������� ���� ������ ����� ����������. 
    // ��� ����� ������� ������ �������� ���� � ������ �� �������
    current_layer[0] = left_condition;
}

void Block_1_transport_equation::output_data(ring_buffer_t<vector<vector<double>>>& buffer, int i)
{
    // ���������� ������������ ���� std
    using namespace std;

    vector<vector<double>>& previous_layer = buffer.previous();
    //1 ���� � ������� ���������
    if (i == 0) {
        ofstream outFile("Output.csv");
        outFile << "�����,����������,���������,����" << endl;
        // �������� �������� �������� ���� � ����
        for (size_t j = 0; j < previous_layer[0].size(); j++) {
            outFile << i * dt << "," << j * dx << "," << previous_layer[0][j] << "," << previous_layer[1][j]  << endl;
        }
        outFile.close();
    }
    //����������� ����
    else {
        ofstream outFile("Output.csv", ios::app);
        // �������� �������� �������� ���� � ����
        for (size_t j = 0; j < previous_layer[0].size(); j++) {
            outFile << i * dt << "," << j * dx << "," << previous_layer[0][j] << "," << previous_layer[1][j]  << endl;
        }
        outFile.close();
    }
}

