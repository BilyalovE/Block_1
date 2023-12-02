// Задача 6. Расчет произвольного числа слоев с произвольным числом партий, рассч1т по сере
// Функция

#include <iostream>
#include <vector>
#include <locale.h>
#include <fstream>

using namespace std;


int main()

{
    // Корректный вывод руского текста
    setlocale(LC_ALL, "Russian");

    // Открываем файл для записи
    std::ofstream csvFile("output55.csv");

    // Проверяем, открыт ли файл
    if (!csvFile.is_open()) {
        std::cerr << "Unable to open the file." << std::endl;
        return 1;
    }

    // Записываем заголовки колонок
    csvFile << "Время,Координата,Плотность,Сера" << std::endl;


    // L - длина трубопровода
    double L = 200;

    // Скорость течения жидкости
    double v = 50;

    // Число разбиений трубопровода
    int n = 3;

    // T - период моделирования;
    int T = 6;

    double dx = L / (n - 1);

    // Условие Куранта
    double dt = dx / v;

    // Число слоев 
    int number_layers = T / dt;

    // Начальная плотность нефти в трубе
    double ro_n = 850;

    // Начальное содердание серы в нефти 
    double sulfar_n = 0.2;

    // Вектор плотностей нефти входных партий 
    vector <double> ro_in(number_layers);
    ro_in = { 870, 870, 870, 870, 880, 880, 880, 880, 0, 0, 0, 0, 0 };

    // Начальный слой по плотности
    vector <double> ro_0(n);

    // Следующий слой по плотности
    vector <double> ro_1(n);

    // Вектор содержания серы в нефти входных партий 
    vector <double> sulfar_in(number_layers);
    sulfar_in = { 0.2, 0.2, 0.2, 0.2, 0.1, 0.1, 0.1, 0.1, 0, 0, 0, 0, 0 };


    // Начальный слой по сере
    vector <double> sulfar_0(n);

    // Следующий слой по сере
    vector <double> sulfar_1(n);



    // Предполагаем, что в начальный момент времени всю трубу заполняют  нефть с плотностью ro_n
    cout << "Заполнение трубы нефтью в начале моделирования" << endl;
    for (int i{ 0 }; i < n; i++)
    {
        // Заполнение трубы нефтью в начале моделирования
        ro_0[i] = ro_n;
        sulfar_0[i] = sulfar_n;
        cout << ro_0[i] << endl;
        csvFile << 0 << "," << dx * i << "," << ro_n << "," << sulfar_n << endl;
    }

    // Рассчёт слоев по плотности и сере
    for (int j = 0; j < number_layers; j++)
    {
        cout << "Слой " << j + 1 << endl;
        ro_1[0] = ro_in[j];
        sulfar_1[0] = sulfar_in[j];
        cout << ro_1[0] << endl;
        // Записываем данные в файл
        csvFile << dt * (j + 1) << "," << 0 << "," << ro_1[0] << "," << sulfar_1[0] << endl;

        // Рассчёт нового слоя
        for (int i = 1; i < n; i++)
        {
            ro_1[i] = ro_0[i - 1];
            sulfar_1[i] = sulfar_0[i - 1];
            cout << ro_1[i] << endl;
            csvFile << dt * (j + 1) << "," << dx * i << "," << ro_1[i] << "," << sulfar_1[i] << endl;
        }

        // Копирование нового слой ro_1 в старый ro_0
        ro_0 = ro_1;
        sulfar_0 = sulfar_1;


        /*// Рассчёт слоев по плотности
        for (int j{ 0 }; j < number_layers; j++)
        {
            cout << "Слой " << j + 1 << endl;
            ro_1[0] = ro_in[j];
            cout << ro_1[0] << endl;
            // Записываем данные в файл
            csvFile << dt*(j+1) << "," << 0 << "," << ro_1[0]  << endl;

            // Рассчёт нового слоя
            for (int i{ 1 }; i < n; i++)
            {
                ro_1[i] = ro_0[i - 1];
                cout << ro_1[i] << endl;
                csvFile << dt*(j+1) << "," << dx * i << "," << ro_1[i] << endl;
            }

            // Копирование нового слой ro_1 в старый ro_0
            ro_0 = ro_1;
            */


            /*// Рассчёт слоев по сере
            for (int j{ 0 }; j < number_layers; j++)
            {
                cout << "Слой " << j + 1 << endl;
                sulfar_1[0] = sulfar_in[j];
                cout << sulfar_1[0] << endl;
                // Записываем данные в файл
                csvFile << dt * (j + 1) << "," << 0 << "," << sulfar_1[0] << endl;

                // Рассчёт нового слоя
                for (int i{ 1 }; i < n; i++)
                {
                    sulfar_1[i] = sulfar_0[i - 1];
                    cout << sulfar_1[i] << endl;
                    csvFile << dt * (j + 1) << "," << dx * i << "," << sulfar_1[i] << endl;
                }

                // Копирование нового слой ro_1 в старый ro_0
                sulfar_0 = sulfar_1;
                */

    }
}