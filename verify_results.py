def open_answer_file(filename):
    data_for_compare = []
    with open(filename, 'r') as f:
        data = f.read()
        c = 0
        for el in data.split("\n"):
            if c == 0:
                c += 1
                continue
            if el.strip() == "":
                continue
            data_for_compare.append(el.strip().split('=')[1])
    return data_for_compare


def compare_answers(etalon, result, names):
    # посчитаем относительное отклонение
    for i in range(len(etalon)):
        etalon_value = float(etalon[i])
        result_value = float(result[i])
        if etalon_value == 0:
            if result_value != 0:
                print("Ошибка: эталонное значение равно 0, а результат не равен 0")
            continue
        relative_deviation = abs((result_value - etalon_value) / etalon_value)
        print(f"Ответ для {names[i]}: {result_value} отличается от эталонного {etalon_value}  на {relative_deviation * 100}%")


def main():
    etalon_answers = []
    my_answers = []
    test_cases = [
    (100, 0.1), (100, 1), (100, 10),
    (1000, 0.1), (1000, 1), (1000, 10),
    (10000, 0.1), (10000, 1), (10000, 10)
    ]
    names = ['Средняя частота (mean_freq)', 'Дисперсия частоты (var_freq)', 'Средняя длина (mean_len)', 'Дисперсия длины (var_len)', 'Средний угол (mean_ang)', 'Дисперсия угла (var_ang)']

    for elements in test_cases:
        etalon_answers.append(open_answer_file(f"practika_test_data/test_{elements[0]}_{elements[1]}.txt"))

    print(f"Сравнение с файлом test_{test_cases[8][0]}_{test_cases[8][1]}.txt")

    for i in range(6):
        print(f"Введите ваш ответ для {names[i]}: ")
        my_answers.append(float(input()))

    print("Эталонные ответы:")
    compare_answers(etalon_answers[8], my_answers, names)
    
        
if __name__ == "__main__":
    main()