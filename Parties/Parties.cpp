/*!
    \brief Метод характеристик
    \author Eldar
    \version 3 (использование буффера, подключение pde solvers)
    \date 17.12.2023
*/

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
    int T = 10;    // T - период моделирования;
    Pipeline_parameters() = default;
};


// Создание структуры параметров необходимых для функции солвера
struct Input_solver_parameters {
    int n; // Число разбиений трубопровода
    int number_layers; // Число слоев 
    double dx; // Разбиение трубы на секторы длиной dx
    double dt; // Шаг моделирования
};

// Создание функции с типом возврата Input_solver_parametres, которая на вход принимает структуру с типом Pipeline_parameters
Input_solver_parameters input_function(Pipeline_parameters pipeline_characteristics)
{
    // Объявление структуры с именем Input_solver_parametres для переменной solver_parameters
    Input_solver_parameters solver_parameters;
    solver_parameters.n = 3;                   // Число разбиений трубопровода
    solver_parameters.dx = pipeline_characteristics.L / (solver_parameters.n - 1);            // Разбиение трубы на секторы длиной dx
    solver_parameters.dt = solver_parameters.dx / pipeline_characteristics.v;                       // Шаг моделирования
    solver_parameters.number_layers = static_cast<int>(pipeline_characteristics.T / solver_parameters.dt);  // Число слоев 
    return solver_parameters;
}


// Создание функции солвера ( решателя, где параметрами функции являются: 
                                   // структура входных парметров для реализации алгоритма солвера, 
                                   // начальное значение измеряемого парметра, 
                                   // входной массив измеряемых парметров )

vector <double> solver(Input_solver_parameters solver_parameters, vector <double>* ro_0, double ro_in)
{
    vector <double> layer_1(solver_parameters.n);  // Следующий слой 
    layer_1[0] = ro_in;
    for (int i{ 1 }; i < solver_parameters.n; i++)
    {
        layer_1[i] = (*ro_0)[i - 1];
    }
    // По методу характеристик текущий слой(рассчитанный на данной итерации) 
    // становится начальным слоем на следующей итерации для рассчёта следующего слоя
    return layer_1;
}

/// @brief Функция, созданная для вываода
/// @param j Счетчик
/// @param solver_parameters 
/// @param ro_0
/// @param filename 
/// @return Массив в файле
void output_function(int j, Input_solver_parameters solver_parameters, vector <double>* ro_0, const std::string& filename) {
    // Корректный вывод руского текста
    
    setlocale(LC_ALL, "Russian");
    // Записываем заголовки колонок, если файл пустой
     
    // Открыть файл в режиме добавления данных в конец файла или обрезания при новом запуске
    std::ofstream outfile(filename, std::ios::app);

    if (!outfile.is_open()) {
        std::cerr << "Ошибка открытия файла " << filename << std::endl;
        return;
    }
    
    vector <double> output_sloi(solver_parameters.n);
    output_sloi = *ro_0;
    for (int i{ 0 }; i < solver_parameters.n; i++) {
        outfile << solver_parameters.dt * (j) << "," << solver_parameters.dx * i << "," << output_sloi[i]  << endl;
    }

    // Закрыть файл
    outfile.close();
    std::cout << "Массив успешно добавлен в файл " << filename << std::endl;
}

int main()
{
    // Объявление структуры с именем Pipeline_parameters для переменной pipeline_characteristics
    /// @brief Объявление структуры с именем Pipeline_parameters для переменной pipeline_characteristics
    Pipeline_parameters  pipeline_characteristics;
    // Инициализация структуры solver_parameters через вызов функции input_function 
    Input_solver_parameters solver_parameters = input_function(pipeline_characteristics);

    double ro_n = 805;                         // Начальная плотность нефти в трубе

    vector <double> ro_0(solver_parameters.n);  // Начальный слой по плотности

    vector <double> ro_in(solver_parameters.number_layers); // Вектор плотностей нефти входных партий 
    ro_in = { 880, 870, 870, 870, 880, 880, 880, 880, 0, 0, 0, 0, 0 };
    
    // Задаем имя файла на вывод данных
    const std::string filename = "output.csv";

    // Удалить файл, если он существует
    std::remove(filename.c_str());

    // Открыть файл в режиме добавления данных в конец файла или обрезания при новом запуске
    std::ofstream outfile(filename, std::ios::app);
    outfile << "Время,Координата,Плотность" << std::endl;
    
    // Предполагаем, что в начальный момент времени всю трубу заполняют нефть с начальными параметрами initial_value
    for (int i{ 0 }; i < solver_parameters.n; i++)
    {
        ro_0[i] = ro_n;
    }

    output_function(0, solver_parameters, &ro_0, filename);
    // Расчёт произвольного числа слоев (solver_parameters.number_layers) через вызов функции  solver в цикле
    for (int j{ 1 }; j < solver_parameters.number_layers; j++)
    {
        // Вывод в файл 
        ro_0 = solver(solver_parameters, &ro_0, ro_in[j]);
        output_function(j, solver_parameters, &ro_0, filename);
    }

    return 0;
}