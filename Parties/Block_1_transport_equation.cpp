#include "Block_1_transport_equation.h"

Block_1_transport_equation::Block_1_transport_equation(double dx, double speed, int n)
{
    this->n = n;
    this->dx = dx;
    this->speed = speed;
}

void Block_1_transport_equation::method_characteristic(vector<double>& current_layer, vector<double>& previous_layer, double left_condition) const
{
    // Получение ссылок на текущий и предыдущий слои буфера

    for (size_t i = 1; i < n; i++)
    {
        current_layer[i] = previous_layer[i - 1];
    }
    // Слой current_layer на следующем шаге должен стать предыдущим. 
    // Для этого сместим индекс текущего слоя в буфере на единицу
    current_layer[0] = left_condition;
}

double Block_1_transport_equation::get_dt(const int j)
{
    if (j > 0) { 
        dt = dx / speed; 
    }
    else {
        dt = 0;
    }
    return dt;
}

/// <summary>
/// 
/// </summary>
/// <param name="buffer"></param>
/// <param name="j"></param>
void Block_1_transport_equation::output_data(ring_buffer_t<vector<vector<double>>>& buffer, int j) const
{
    // Используем пространство имен std
    using namespace std;
    setlocale(LC_ALL, "rus");
    vector<vector<double>>& previous_layer = buffer.previous();
    //1 слой с записью заголовка
    if (j == 0) {
        ofstream outFile("Output.csv");
        outFile << "Время,Координата,Плотность,Сера" << endl;
        // Записать значения текущего слоя в файл
        for (size_t i = 0; i < previous_layer[0].size(); i++) {
            outFile << j * dt << "," << i * dx << "," << previous_layer[0][i] << "," << previous_layer[1][i] << endl;
            
        }
        outFile.close();
    }
    //последующие слои
    else {
        ofstream outFile("Output.csv", ios::app);
        // Записать значения текущего слоя в файл
        for (size_t i = 0; i < previous_layer[0].size(); i++) {
            outFile << dt << "," << i * dx << "," << previous_layer[0][i] << "," << previous_layer[1][i] << endl;
        }
        outFile.close();
    }
}

void Block_1_transport_equation::interpolation_flow(const Pipeline_parameters& pipeline_characteristics, double dt)
{
    double interpolation_Q;
    vector <double> t = pipeline_characteristics.t;
    vector <double> Q = pipeline_characteristics.Q;
    interpolation_Q = (dt - t[0]) / (t[1] - t[0]) * (Q[1] - Q[0]) +Q[0];
}
