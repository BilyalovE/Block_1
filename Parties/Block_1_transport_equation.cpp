#include "Block_1_transport_equation.h"

Block_1_transport_equation::Block_1_transport_equation(Pipeline_parameters& pipeline_characteristics,int n, int j)
{
    this->n = n;
    this->j = j;
    this->pipeline_characteristics = pipeline_characteristics;
    // dx - величина шага между узлами расчетной сетки, м;
    this->dx = pipeline_characteristics.L / (n - 1);
   
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

//vector <double> Block_1_transport_equation::get_speed() {
//    double square = pipeline_characteristics.get_inner_square();
//    int size_array_Q = pipeline_characteristics.Q.size();
//    vector <double> speed(size_array_Q);
//    for (int i = 0; i < size_array_Q; i++) {
//        speed[i] = (pipeline_characteristics.Q)[i] / square;
//    }
//    return speed;
//}

double Block_1_transport_equation::get_speed() {
    double square = pipeline_characteristics.get_inner_square();
    double speed{};
    if (j == 0) {
        speed = (pipeline_characteristics.Q)[0] / square;
    }
    else {
       double interpolation_Q = interpolation_flow();
       speed = interpolation_Q / square;
    }
    return speed;
}

double Block_1_transport_equation::get_dt()
{
    double speed = get_speed();
    dt = dx / speed;
  
    return dt;
}


/// <summary>
/// 
/// </summary>
/// <param name="buffer"></param>
/// <param name="j"></param>
void Block_1_transport_equation::output_data(ring_buffer_t<vector<vector<double>>>& buffer, double sum_dt) const
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
            outFile << sum_dt << "," << i * dx << "," << previous_layer[0][i] << "," << previous_layer[1][i] << endl;
            
        }
        outFile.close();
    }
    //последующие слои
    else {
        ofstream outFile("Output.csv", ios::app);
        // Записать значения текущего слоя в файл
        for (size_t i = 0; i < previous_layer[0].size(); i++) {
            outFile << sum_dt << "," << i * dx << "," << previous_layer[0][i] << "," << previous_layer[1][i] << endl;
        }
        outFile.close();
    }
}

double Block_1_transport_equation::interpolation_flow()
{
    double interpolation_Q;
    vector <double> t = pipeline_characteristics.t;
    vector <double> Q = pipeline_characteristics.Q;
    int size_array_Q = pipeline_characteristics.Q.size();
    if (size_array_Q > j) {
        if (dt <= t[j]) {
            interpolation_Q = (dt - t[j - 1]) / (t[j] - t[j - 1]) * (Q[j] - Q[j - 1]) + Q[j - 1];
        }
        else {
            j += 1;
            interpolation_Q = (dt - t[j - 1]) / (t[j] - t[j - 1]) * (Q[j] - Q[j - 1]) + Q[j - 1];
        }
    }
    else {
        interpolation_Q = (pipeline_characteristics.Q)[size_array_Q - 1];
    }
    return interpolation_Q;
}
