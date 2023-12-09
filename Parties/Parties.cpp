// Первичный рефакторинг
#include <iostream>
#include <vector>
#include <locale.h>
#include <fstream>

using namespace std;

// Создание структуры параметров трубопровода
struct Pipeline_parameters
{
    double L = 200;  // L - длина трубопровода
    double v = 50;  // Скорость течения жидкости
    int n = 3;     // Число разбиений трубопровода
    int T = 6;    // T - период моделирования;
    Pipeline_parameters() = default;
};


// Создание структуры параметров необходимых для функции солвера
struct Input_solver_parametres {
    int n; // Число разбиений трубопровода
    int number_layers; // Число слоев 
};

// Создание функции с типом возврата Input_solver_parametres, которая на вход принимает структуру с типом Pipeline_parameters
Input_solver_parametres input_function(Pipeline_parameters input)
{
    // Объявление структуры с именем Input_solver_parametres для переменной input_parametres
    Input_solver_parametres input_parametres;
    double dx = input.L / (input.n - 1);            // Разбиение трубы на секторы длиной dx
    double dt = dx / input.v;                       // Шаг моделирования
    input_parametres.number_layers = input.T / dt;  // Число слоев 
    input_parametres.n = input.n;                   // Число разбиений трубопровода
    return input_parametres;
}


// Создание функции солвера ( решателя, где параметрами функции являются: 
                                   // структура входных парметров для реализации алгоритма солвера, 
                                   // начальное значение измеряемого парметра, 
                                   // входной массив измеряемых парметров )

vector <double> solver(Input_solver_parametres value, vector <double>* initial_buffer, double* ptr)
{
    vector <double> layer_0(value.n);  // Начальный слой
    layer_0 = *initial_buffer;

    vector <double> layer_1(value.n);  // Следующий слой 
    layer_1[0] = *ptr;
    for (int i{ 1 }; i < value.n; i++)
    {
        layer_1[i] = layer_0[i - 1];
        //cout << layer_1[i] << endl;
    }
    // По методу характеристик текущий слой(рассчитанный на данной итерации) 
    // становится начальным слоем на следующей итерации для рассчёта следующего слоя
    layer_0 = layer_1;
    return layer_1;

}

void output_function(int j, Input_solver_parametres value, vector <double>* sloi) {
    vector <double> output_sloi(value.n);
    output_sloi = *sloi;
    cout << "Слой" << j + 1 << endl;
    for (int i{ 0 }; i < value.n; i++) {
        cout << output_sloi[i] << endl;
    }
}

int main()
{
    // Объявление структуры с именем Pipeline_parameters для переменной method_characteristics
    Pipeline_parameters pipeline_characteristics;

    setlocale(LC_ALL, "Russian"); // Корректный вывод кириллицы

    Input_solver_parametres input_parametres = input_function(pipeline_characteristics);

    double ro_n = 850;                         // Начальная плотность нефти в трубе

    vector <double> ro_0(input_parametres.n);  // Начальный слой по плотности

    vector <double> ro_in(input_parametres.number_layers); // Вектор плотностей нефти входных партий 
    ro_in = { 890, 870, 870, 870, 880, 880, 880, 880, 0, 0, 0, 0, 0 };

    vector <double> sloi(input_parametres.n);
    // Предполагаем, что в начальный момент времени всю трубу заполняют нефть с начальными параметрами initial_value
    cout << "Заполнение трубы нефтью в начале моделирования" << endl;
    for (int i{ 0 }; i < input_parametres.n; i++)
    {
        ro_0[i] = ro_n;
        cout << ro_0[i] << endl;
    }
    //input_parametres.number_layers
    for (int j{ 0 }; j < input_parametres.number_layers; j++)
    {
        sloi = solver(input_parametres, &ro_0, &ro_in[j]);
        output_function(j, input_parametres, &sloi);
        ro_0 = sloi;
    }

    cout << "все плохо";
    system("pause");

    return 0;


}