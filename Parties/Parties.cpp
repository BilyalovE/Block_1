/*!
    \ brief Метод характеристик (расчёт по плотности и сере)
    \ author Bilyalov Eldar
    \ version 6.0 (Модель с переменным шагом по времени / рефакторинг)
    \ date 01.03.24

*/

// Подключаем необходимые библиотеки
#include <iostream>
#include <vector>
#include <iomanip>
#include <fixed/fixed.h>
#include <pde_solvers/pde_solvers.h>
#include "Block_1_transport_equation.h"
#include "Struct_pipe.h"



/// @brief Главная функция, в которой происходит инициализация структур, краевых и начальных условий, а также вызов функции солвера и функции вывода в файл
int main(int argc, char** argv)
{
    // Используем пространство имен std
    using namespace std;

    setlocale(LC_ALL, "rus");
    /// @param Объявление структуры с именем Pipeline_parameters для переменной pipeline_characteristics
    Pipeline_parameters  pipeline_characteristics;
    /// @param - количество точек расчетной сетки;
    int n = 10;
    /// @param Начальное значение плотности нефти в трубе
    double initial_condition_density = 805;
    /// @param Начальное содержание серы в нефти в трубе
    double initial_condition_sulfar = 0.15;

    /// Предполагаем, что в начальный момент времени всю трубу заполняют нефть с начальными параметрами initial_condition_density
    /// @param Начальный слой по плотности
    vector <double> initial_density_layer(n, initial_condition_density);
    /// @param Вектор, содержащий значения плотностей нефти входных партий 
    vector <double> input_conditions_density = {880, 870, 870, 870, 880, 880, 880, 880, 0 };
    /// @param Начальный слой по сере
    vector <double> initial_sulfar_layer(n, initial_condition_sulfar);
    /// @param Вектор содержания серы в нефти входных партий 
    vector <double> input_conditions_sulfar = {0.2, 0.1, 0.1, 0.1, 0.25, 0.25, 0.25, 0.25, 0 };
    vector <double> time_input_parties = {10, 20, 40, 50, 80, 100, 200, 300, 400 };
    /// @param Число рассчитываемых параметров (2 параметра - сера и плотность)
    int num_parameters = 2; 
    /// @param Вектор векторов параметров нефти входных партий
    vector <vector<double>> input_conditions(num_parameters, vector<double>(input_conditions_sulfar.size()));
    input_conditions = { input_conditions_density, input_conditions_sulfar };
    // Создаем  буфер для решаемой задачи
    /// @param number_layers_buffer - количество слоев в буфере (для метода характеристик достаточно хранить 2 слоя - предыдущий и текущий слои)
    int number_layers_buffer = 2;
    ring_buffer_t <vector<vector<double>>> buffer(number_layers_buffer, { initial_density_layer, initial_sulfar_layer });

    // Расчёт произвольного числа слоев (solver_parameters.number_layers) через вызов функции solver в цикле
    /// @param sum_dt -  сумма времени моделирования 
    double sum_dt = 0;
    /// @param j - счетчик слоев
    int j = 0;
    do {
        /// @param transport_equation (солвер - метод характеристик) - экземпляр класса Block_1
        Block_1_transport_equation transport_equation(pipeline_characteristics, n, j);
        // Проверка выхода за границы массива входных параметров нефти
        if (j < input_conditions_density.size()){
            for (size_t i{ 0 }; i < num_parameters; i++) {
                transport_equation.method_characteristic(buffer.current()[i], buffer.previous()[i], input_conditions[i][j]);
            }
        }
        else {
            for (size_t i{ 0 }; i < num_parameters; i++) {
                double empty_pipe{ 0 };
                transport_equation.method_characteristic(buffer.current()[i], buffer.previous()[i], empty_pipe);
            }
        }
        transport_equation.output_data(buffer, sum_dt);
        buffer.advance(1);
        sum_dt += transport_equation.get_dt();
        j++;
    } while (sum_dt <= pipeline_characteristics.T);
    return 0; 
}
