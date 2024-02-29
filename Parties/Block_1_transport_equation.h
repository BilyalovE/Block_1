#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <locale.h>
#include <fstream>
#include <iomanip>
#include <fixed/fixed.h>
#include <pde_solvers/pde_solvers.h>
#include "Struct_pipe.h"

/// @brief класс Block_1 для решения задач из блока 1 - Модель движения партий
class Block_1_transport_equation
{
    // Поля класса
    /// @param n - количество точек расчетной сетки;
    int n;
    /// @param j - счетчик слоя
    int j;
    /// @param dx - величина шага между узлами расчетной сетки, м;
    double dx;
    /// @param dt - шаг во времени из условия Куранта;
    double dt;
    /// @param speed - скорость движения нефти
    double speed;
    /// @param pipeline_characteristics - параметры трубопровода
    Pipeline_parameters pipeline_characteristics;

    

public:
    /// @brief Конструктор класса Block_1
    /// @param pipeline_characteristics - Структура исходных параметров трубопровода
    /// @param n - количество точек расчетной сетки;
    Block_1_transport_equation(Pipeline_parameters& pipeline_characteristics, int n, int j);

    /// @brief Метод характеристик, рассчитывающий слои
    /// @param solver_parameters - структура параметров, необходимая для алгоритма;
    /// @param buffer - буфер, который для расчёта хранит 2 слоя (текущий и прядущий);
    /// @param left_condition - граничное условие для параметра нефти.
    /// @return previous_layer - возвращает рассчитанный по методу характеристик текущий слой
    void method_characteristic(vector<double>& current_layer, vector<double>& previous_layer, double left_condition) const;

    /// @brief Метод расчета скорости по расходу (расход может быть интерполирован)
    double get_speed();
    
    /// @brief Метод получения шага времени dt
    double get_dt(const int j);

    /// @brief Метод output_data, который выводит рассчитанные по методу характеристик слои в файл формата csv
    /// @param i - счётчик слоев;
    /// @param solver_parameters - структура параметров, необходимых для реализации функции солвера ;
    /// @param buffer - буфер, рассчитанный после солвера;
    /// @return  - пустой возврат (вывод в файл рассчитанного слоя - buffer.current()).
    void output_data(ring_buffer_t<vector<vector<double>>>& buffer, int i) const;

    void interpolation_flow();
};
