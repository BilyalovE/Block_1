#pragma once
#include <iostream>
#include <vector>
#include <pde_solvers/pde_solvers.h>
/// @brief Pipeline_parameters - структура исходных параметров трубопровода
struct Pipeline_parameters
{
    /// @param L - длина трубопровода, м;
    double L = 200;
    /// @param D - внешний диаметр трубы, м;
    double D = 0.7;
    /// @param d - толщина стенки трубы, м;
    double d = 0.01;
    /// @brief get_inner_diametr - функция, рассчитывающая внутренний диаметр трубы
    double get_inner_diametr() const {
        return D - 2 * d;
    };
    /// @brief get_inner_square - функция, рассчитывающая площадь сечения трубы
    double get_inner_square() const {
        double inner_diametr = get_inner_diametr();
        double S = M_PI * inner_diametr * inner_diametr / 4;
        return S;
    };
    /// @param Q - синтетический ряд изменения расхода, где нулевой элемент массива Q - расход в начале моделирования (t = 0)
    vector <double> Q = { 0.9, 0.99, 0.97, 0.8, 1, 1.2, 1.6 };
    /// @param t - временной ряд изменения расхода, где нулевой элемент массива t = 0
    vector <double> t = { 0, 30, 50, 200, 450, 787, 965 };
    /// @param T - период моделирования, c.
    int T = 700;
};