/*!
    \brief Метод характеристик (рассчёт по плотности и сере)
    \author Bilyalov Eldar
    \version 4 (правильное использование буффера - без скрытого копирования, корректный вывод)
    \date 30.12.2023 - 6.01.24
*/

// Подключаем необходимые библиотеки
#include <iostream>
#include <string>
#include <vector>
#include <locale.h>
#include <fstream>
#include <iomanip>
#include <fixed/fixed.h>
#include <pde_solvers/pde_solvers.h>


// Используем простравнство имен std
using namespace std;

/// @brief Cтруктура исходных параметров трубопровода
/// @param L - длина трубопровода, м;
/// @param v - скорость течения жидкости, м/с;
/// @param T - период моделирования, c.
struct Pipeline_parameters
{
    double L = 200;
    double v = 50;
    int T = 10;
    Pipeline_parameters() = default;
};

/// @brief Структура параметров, необходимых для рассчёта функции солвера
/// @param n - количество точек расчетной сетки;
/// @param number_layers - количесвто слоев рассчёта;
/// @param dx - величина шага между узлами расчетной сетки, м;
/// @param dt - шаг во времени из условия Куранта.
struct Input_solver_parameters
{
    int n;
    int number_layers;
    double dx;
    double dt;
};

/// @brief класс Block_1 для решения задач из блока 1 - Модель движения партий
class Block_1
{
private:
public:
    /// @brief Метод input_data, который рассчитывает входные параметры, необходимые для метода характеристик по исходным параметрам трубопровода
    /// @param pipeline_characteristics - инициализированная переменная со входными параметрами трубоппровода (тип данных - структура Pipeline_parameters)
    Input_solver_parameters input_data(Pipeline_parameters pipeline_characteristics)
    {
        // Объявление структуры с именем Input_solver_parametres для переменной solver_parameters
        Input_solver_parameters solver_parameters;
        solver_parameters.n = 3;   // - количество точек расчетной сетки;
        solver_parameters.dx = pipeline_characteristics.L / (solver_parameters.n - 1);  // - количесвто слоев рассчёта;          
        solver_parameters.dt = solver_parameters.dx / pipeline_characteristics.v;   // - шаг во времени из условия Куранта;                   
        solver_parameters.number_layers = static_cast<int>(pipeline_characteristics.T / solver_parameters.dt); // - количесвто слоев рассчёта;
        return solver_parameters;
    }

    /// @brief Метод характеристик, рассчитывающий слои
    /// @param solver_parameters - структура параметров, необходимая для алгоритма;
    /// @param buffer - буффер, который для рассчета хранит 2 слоя (текущий и предущий);
    /// @param left_condition - граничное условие для параметра нефти.
    /// @return previous_layer - возвращает рассчитанный по методу характеристик текущий слой
    /// 
    void method_characteristic( vector<double>& current_layer, vector<double>& previous_layer, double left_condition)
    {
        // Получение ссылок на текущий и предыдущий слои буфера

        for (size_t i = 1; i < m_solver_parameters.n; i++)
        {
            current_layer[i] = previous_layer[i - 1];
        }
        // Слой current_layer на следующем шаге должен стать предыдущим. 
        // Для этого сместим индекс текущего слоя в буфере на единицу
        current_layer[0] = left_condition;
    }

    /// @brief Метод output_data, который выводит рассчитанные по методу характеристи слои в файл формата csv
    /// @param i - cчетчик слоев;
    /// @param solver_parameters - структура параметров, необходимых для реализации функции солвера ;
    /// @param buffer - буффер, рассчитанный после солвера;
    /// @return  - пустой возврат (вывод в файл рассчитанного слоя - buffer.current()).
    void output_data(ring_buffer_t<vector<vector<double>>>& buffer, int i) {
        vector<vector<double>>& previous_layer = buffer.previous();
        //1 слой с записью заголовка
        if (i == 0) {
            ofstream outFile("Output.csv");
            outFile << "Время,Координиата,Плотность,Сера" << endl;
            // Записать значения текущего слоя в файл
            for (size_t j = 0; j < previous_layer[0].size(); j++) {
                outFile << i * m_solver_parameters.dt << "," << j * m_solver_parameters.dx << "," << previous_layer[0][j] << "," << previous_layer[1][j] << endl;
            }
            outFile.close();
        }
        //последующие слои
        else {
            ofstream outFile("Output.csv", ios::app);
            // Записать значения текущего слоя в файл
            for (size_t j = 0; j < previous_layer[0].size(); j++) {
                outFile << i * m_solver_parameters.dt << "," << j * m_solver_parameters.dx << "," << previous_layer[0][j] << "," << previous_layer[1][j] << endl;
            }
            outFile.close();
        }
    }
};

/// @brief Главная функция, в которой происходит инициализация структур, краевых и начальных условий, а также вызов функции солвера и функции вывода в файл
int main(int argc, char** argv)
{
    // Объявляем объект solver_1 класса block_1
    Block_1 solver_1;
    /// Объявление структуры с именем Pipeline_parameters для переменной pipeline_characteristics
    Pipeline_parameters  pipeline_characteristics;
    /// Инициализация структуры solver_parameters через вызов функции input_data
    Input_solver_parameters solver_parameters = input_data(pipeline_characteristics);
    // Объявляем объект solver_1 класса Block_1
    Block_1 solver_1(solver_parameters);

    /// Начальное значение плотности нефти в трубе
    double initial_condition_density = 805;

    /// Начальное содержание серы в нефти в трубе
    double initial_condition_sulfar = 0.15;

    /// Предполагаем, что в начальный момент времени всю трубу заполняют нефть с начальными параметрами initial_condition_density
    /// Начальный слой по плотности
    vector <double> initial_density_layer(solver_parameters.n, initial_condition_density);
    /// Вектор, содержащий значения плотностей нефти входных партий 
    vector <double> input_conditions_density(solver_parameters.number_layers);
    input_conditions_density = { 880, 870, 870, 870, 880, 880, 880, 880, 0, 0, 0, 0, 0 };

    /// Начальный слой по сере
    vector <double> initial_sulfar_layer(solver_parameters.n, initial_condition_sulfar);
    /// Вектор содержания серы в нефти входных партий 
    vector <double> input_conditions_sulfar(solver_parameters.number_layers);
    input_conditions_sulfar = { 0.2, 0.1, 0.1, 0.1, 0.25, 0.25, 0.25, 0.25, 0, 0, 0, 0, 0 };

    // Создаем  буфер для решаемой задачи
    // 2 - количество слоев в буфере (для метода характеристик достаточно хранить 2 слоя)
    // initial_density_layer - слой, значениями из которого проинициализируются все слои буфера
    // initial_sulfar_layer - слой, значениями из которого проинициализируются все слои буфера
    ring_buffer_t<vector<vector<double>>> buffer(2, { initial_density_layer, initial_sulfar_layer });

    // Расчёт произвольного числа слоев (solver_parameters.number_layers) через вызов функции  solver в цикле
    /// j - счетчик слоев
    for (size_t j{ 0 }; j < solver_parameters.number_layers + 1; j++)
    {
        solver_1.method_characteristic(solver_parameters, buffer.current()[0], buffer.previous()[0], input_conditions_density[j]);
        solver_1.method_characteristic(solver_parameters, buffer.current()[1], buffer.previous()[1], input_conditions_sulfar[j]);
        solver_1.output_data(solver_parameters, buffer, j);
        buffer.advance(1);
    }
    return 0;
}
