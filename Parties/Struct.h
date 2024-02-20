#pragma once
/// @brief Pipeline_parameters - структура исходных параметров трубопровода
/// @param L - длина трубопровода, м;
/// @param v - скорость течения жидкости, м/с;
/// @param T - период моделирования, c.
struct Pipeline_parameters
{
    double L;
    double v;
    int T;
};