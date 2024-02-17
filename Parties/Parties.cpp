/*!
    \ brief Метод характеристик (расчёт по плотности и сере)
    \ author Bilyalov Eldar
    \ version 5 (Исправлен класс, сделан многофайловый проект)
    \ date 16.02.24
*/

// Подключаем необходимые библиотеки
#include <iostream>
#include <vector>
#include <iomanip>
#include <fixed/fixed.h>
#include <pde_solvers/pde_solvers.h>
#include "Block_1_transport_equation.h"
#include "struct.h"

/// @brief Главная функция, в которой происходит инициализация структур, краевых и начальных условий, а также вызов функции солвера и функции вывода в файл
int main(int argc, char** argv)
{
    // Используем пространство имен std
    using namespace std;

    /// Объявление структуры с именем Pipeline_parameters для переменной pipeline_characteristics
    Pipeline_parameters  pipeline_characteristics = {200, 50, 10};

    // n - количество точек расчетной сетки;
    int n = 3;

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

    // Создаем  буфер для решаемой задачи
    // 2 - количество слоев в буфере (для метода характеристик достаточно хранить 2 слоя)
    // initial_density_layer - слой, значениями из которого проинициализируются все слои буфера
    // initial_sulfar_layer - слой, значениями из которого проинициализируются все слои буфера
    ring_buffer_t <vector<vector<double>>> buffer(2, { initial_density_layer, initial_sulfar_layer });
    // transport_equation (солвер - метод характеристик) - экземпляр класса Block_1
    Block_1_transport_equation transport_equation(pipeline_characteristics, n);
    /// Число параметров продукта, рассчитываемого по методу характкристик
    int num_parameters = 2; // скра и плотность
    // Расчёт произвольного числа слоев (solver_parameters.number_layers) через вызов функции  solver в цикле
    /// j - счетчик слоев
    for (size_t j{ 0 }; j < transport_equation.getter_number_layers() + 1; j++){
        for (size_t i{ 0 }; i < num_parameters; i++) {
            transport_equation.method_characteristic(buffer.current()[i], buffer.previous()[i], input_conditions_density[j]);
        }
        transport_equation.output_data(buffer, j);
        buffer.advance(1);  
    }
    return 0; 
}
