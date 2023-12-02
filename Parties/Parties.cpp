// Первичный рефакторинг
#include <iostream>
#include <vector>
#include <locale.h>
#include <fstream>

using namespace std;

// Создание структуры параметров трубопровода
struct Pipeline_parameters
{
    // L - длина трубопровода
    double L = 200;

    // Скорость течения жидкости
    double v = 50;

    // Число разбиений трубопровода
    int n = 3;

    // T - период моделирования;
    int T = 6;
};


// Создание структуры параметров необходимых для функции солвера
struct Input_solver_parametres {
    // Число разбиений трубопровода
    int n;
    // Число слоев 
    int number_layers;
};

// Объявление структуры с именем Input_solver_parametres для переменной input_parametres
Input_solver_parametres input_parametres;


// Объявление структуры с именем Pipeline_parameters для переменной method_characteristics
Pipeline_parameters method_characteristics;

// Создание функции с типом возврата Input_solver_parametres, которая на вход принимает структуру с типом Pipeline_parameters
Input_solver_parametres input_function(Pipeline_parameters input)
{


    // Разбиение трубы на секторы длиной dx
    double dx;
    dx = input.L / (input.n - 1);

    // Шаг моделирования
    double dt;
    dt = dx / input.v;



    // Число слоев 
    input_parametres.number_layers = input.T / dt;

    // Число разбиений трубопровода
    input_parametres.n = input.n;

    return input_parametres;
}


// Создание функции солвера ( решателя, где параметрами функции являются: 
                                   // структура входных парметров для реализации алгоритма солвера, 
                                   // начальное значение измеряемого парметра, 
                                   // входной массив измеряемых парметров )

void solver(Input_solver_parametres value, vector <double>* initial_buffer, vector <double>* buffer)
{
    // Начальный слой
    vector <double> layer_0(value.n);
    layer_0 = *initial_buffer;

    // Объявление структуры с именем Input_solver_parametres для переменной input_parametres
    Input_solver_parametres input_parametres;

    // Следующий слой 
    vector <double> layer_1(value.n);
    layer_1[0] = (*buffer)[input_parametres.number_layers + 1];
    for (int i{ 1 }; i < value.n; i++)
    {
        layer_1[i] = layer_0[i - 1];
        cout << layer_1[i] << endl;
    }
    // По методу характеристик текущий слой(рассчитанный на данной итерации) 
    // становится начальным слоем на следующей итерации для рассчёта следующего слоя
    layer_0 = layer_1;

}


int main()
{
    setlocale(LC_ALL, "Russian");




    input_function(method_characteristics);

    // Начальная плотность нефти в трубе
    double ro_n = 850;

    // Начальный слой по плотности
    vector <double> ro_0(input_parametres.n);

    // Вектор плотностей нефти входных партий 
    vector <double> ro_in(input_parametres.number_layers);
    ro_in = { 870, 870, 870, 870, 880, 880, 880, 880, 0, 0, 0, 0, 0 };


    // Предполагаем, что в начальный момент времени всю трубу заполняют нефть с начальными параметрами initial_value
    cout << "Заполнение трубы нефтью в начале моделирования" << endl;
    for (int i{ 0 }; i < input_parametres.n; i++)
    {
        ro_0[i] = ro_n;
        cout << ro_0[i] << endl;
    }

    for (int j{ 0 }; j < input_parametres.number_layers; j++)
    {
        cout << "Слой" << j + 1 << endl;
        solver(input_parametres, &ro_0, &ro_in);
    }

}
