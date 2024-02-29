/*!
    \ brief Метод характеристик (расчёт по плотности и сере)
    \ author Bilyalov Eldar
    \ version 5 (Исправлен класс, сделан многофайловый проект)
    \ date 20.02.24
*/

// Подключаем необходимые библиотеки
#include <iostream>
#include <vector>
#include <iomanip>
#include <fixed/fixed.h>
#include <pde_solvers/pde_solvers.h>
#include "Block_1_transport_equation.h"
#include "Struct_pipe.h"



vector <double> get_speed(Pipeline_parameters  &pipeline_characteristics) {
    double square = pipeline_characteristics.get_inner_square();
    int size_array_Q = 3;
    vector <double> speed(size_array_Q);
    for (int i = 0; i < size_array_Q; i++) {
        speed[i] = (pipeline_characteristics.Q)[i] / square;
    }
    return speed;
}

/// @brief Главная функция, в которой происходит инициализация структур, краевых и начальных условий, а также вызов функции солвера и функции вывода в файл
int main(int argc, char** argv)
{
    // Используем пространство имен std
    using namespace std;

    setlocale(LC_ALL, "rus");
    /// Объявление структуры с именем Pipeline_parameters для переменной pipeline_characteristics
    Pipeline_parameters  pipeline_characteristics = { 200, 0.7, 0.01, { 0.9, 0.99, 0.97 }, {0, 30, 50}, 100 };
    int size = pipeline_characteristics.Q.size();
    // n - количество точек расчетной сетки;
    int n = 3;
    vector <double> speed = get_speed(pipeline_characteristics);
    // dx - величина шага между узлами расчетной сетки, м;
    double dx = pipeline_characteristics.L / (n - 1);
    /// Начальное значение плотности нефти в трубе
    double initial_condition_density = 805;

    /// Начальное содержание серы в нефти в трубе
    double initial_condition_sulfar = 0.15;

    /// Предполагаем, что в начальный момент времени всю трубу заполняют нефть с начальными параметрами initial_condition_density
    /// Начальный слой по плотности
    vector <double> initial_density_layer(n, initial_condition_density);
    /// Вектор, содержащий значения плотностей нефти входных партий 
    vector <double> input_conditions_density = { 880, 870, 870, 870, 880, 880, 880, 880 };
    /// Начальный слой по сере
    vector <double> initial_sulfar_layer(n, initial_condition_sulfar);
    /// Вектор содержания серы в нефти входных партий 
    vector <double> input_conditions_sulfar = { 0.2, 0.1, 0.1, 0.1, 0.25, 0.25, 0.25, 0.25 };
    // Число рассчитываемых параметров (2 параметра - сера и плотность)
    int num_parameters = 2; 
    // Вектор векторов параметров нефти входных партий
    vector <vector<double>> input_conditions(num_parameters, vector<double>(input_conditions_sulfar.size()));
    input_conditions = { input_conditions_density, input_conditions_sulfar };
    // Создаем  буфер для решаемой задачи
    // number_layers_buffer - количество слоев в буфере (для метода характеристик достаточно хранить 2 слоя - предыдущий и текущий слои)
    int number_layers_buffer = 2;
    // initial_density_layer - слой, значениями из которого проинициализируются все слои буфера
    // initial_sulfar_layer - слой, значениями из которого проинициализируются все слои буфера
    ring_buffer_t <vector<vector<double>>> buffer(number_layers_buffer, { initial_density_layer, initial_sulfar_layer });
    // transport_equation (солвер - метод характеристик) - экземпляр класса Block_1
    Block_1_transport_equation transport_equation(dx, speed[0], n);
    /// Число параметров продукта, рассчитываемого по методу характеристик

    // Расчёт произвольного числа слоев (solver_parameters.number_layers) через вызов функции solver в цикле
    /// @param sum_dt -  сумма времени моделирования 
    double sum_dt = 0;
    /// @param j - счетчик слоев
    int j = 0;
    do {
        for (size_t i{ 0 }; i < num_parameters; i++) {
            transport_equation.method_characteristic(buffer.current()[i], buffer.previous()[i], input_conditions[i][j]);
        }
        sum_dt += transport_equation.get_dt(j);
        transport_equation.output_data(buffer, j);
        buffer.advance(1);
        j++;
    } while (sum_dt <= pipeline_characteristics.T);

    return 0; 
}
