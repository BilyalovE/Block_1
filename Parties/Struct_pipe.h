#pragma once
#include <iostream>
#include <vector>
#include <pde_solvers/pde_solvers.h>
/// @brief Pipeline_parameters - структура исходных параметров трубопровода
struct Pipeline_parameters
{
    /// @param L - длина трубопровода, м;
    double L;
    /// @param D - внешний диаметр трубы, м;
    double D;
    /// @param d - толщина стенки трубыб м;
    double d;
    double get_inner_diametr() const {
        return D - 2 * d;
    };
    double get_inner_square() const {
        double inner_diametr = get_inner_diametr();
        double S = M_PI * inner_diametr * inner_diametr / 4;
        return S;
    };
    /// @param Q - синтетический ряд изменения расхода, где нулевой элемент массива Q - расход в начале моделирования (t = 0)
    vector <double> Q = {};
    /// @param t - временной ряд изменения расхода, где нулевой элемент массива t = 0
    vector <double> t = {};
    /// @param T - период моделирования, c.
    int T;
};