#include "Block_1_transport_equation.h"

Block_1_transport_equation::Block_1_transport_equation(double dx, double dt, int n)
{
    this->n = n;
    this->dx = dx;
    this->dt = dt;
}

void Block_1_transport_equation::method_characteristic(vector<double>& current_layer, vector<double>& previous_layer, double left_condition)
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

void Block_1_transport_equation::output_data(ring_buffer_t<vector<vector<double>>>& buffer, int i)
{
    // Используем пространство имен std
    using namespace std;
    setlocale(LC_ALL, "rus");
    vector<vector<double>>& previous_layer = buffer.previous();
    //1 слой с записью заголовка
    if (i == 0) {
        ofstream outFile("Output.csv");
        outFile << "Время,Координата,Плотность,Сера" << endl;
        // Записать значения текущего слоя в файл
        for (size_t j = 0; j < previous_layer[0].size(); j++) {
            outFile << i * dt << "," << j * dx << "," << previous_layer[0][j] << "," << previous_layer[1][j] << endl;
        }
        outFile.close();
    }
    //последующие слои
    else {
        ofstream outFile("Output.csv", ios::app);
        // Записать значения текущего слоя в файл
        for (size_t j = 0; j < previous_layer[0].size(); j++) {
            outFile << i * dt << "," << j * dx << "," << previous_layer[0][j] << "," << previous_layer[1][j] << endl;
        }
        outFile.close();
    }
}

