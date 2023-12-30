import pandas as pd

# Замените 'file1.csv' и 'file2.csv' на имена ваших файлов
file1_path = 'density.csv'
file2_path = "sulfar.csv"

# Считываем CSV файлы
df1 = pd.read_csv(file1_path, encoding='cp1251')
df2 = pd.read_csv(file2_path, encoding='cp1251')


# Выводим имена столбцов для каждого DataFrame
print("Имена столбцов в df1:", df1.columns)
print("Имена столбцов в df2:", df2.columns)

# Указываем столбцы, по которым будем объединять
merge_columns = ['Время', 'Координата']

# Объединяем файлы по указанным столбцам
merged_df = pd.merge(df1, df2, on=merge_columns)

# Сохраняем объединенный файл
merged_df.to_csv('merged_result.csv', index=False)

print('Объединение завершено. Результат сохранен в merged_result.csv')
